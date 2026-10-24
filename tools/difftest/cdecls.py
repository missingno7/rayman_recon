"""Project-wide C declaration table for the differential harness.

Reads only the project's own declarations (src/*.c, src/*.h, src/include/*.h, src/data, src/bss and the pinned
Watcom headers they include).  Produces, per callable name, one Prototype with parameter widths/signedness,
pointee types (for output-parameter stubs), return width and the Watcom register contract (default watcall
EAX, EDX, EBX, ECX then stack, or the declared `#pragma aux` parm/value/modify lists).  Function-pointer
typedefs, function-pointer variables, tables and struct members are resolved too, so indirect calls through
`extern island_draw_t *DrawSpriteNormalEtX` or `options_jeu.test_fire1` get their declared contract.

Nothing here decides a verdict: an unknown or conflicting declaration only makes a call un-judgeable.
"""
from __future__ import annotations

import dataclasses
import re
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'config/units.json').exists())
WATCOM_H = Path('C:/tools/watcom-10.6/H')

TYPE_WORDS = {'signed', 'unsigned', 'char', 'short', 'int', 'long', 'void', 'float', 'double', 'const',
              'volatile', 'struct', 'union', 'enum', '_Packed', '__far', '__near', '__huge'}
BUILTIN = {  # (width, signed); Watcom 10.6 386: char is unsigned without -j, int = long = 32 bits
    'char': (8, False), 'signed char': (8, True), 'unsigned char': (8, False),
    'short': (16, True), 'signed short': (16, True), 'unsigned short': (16, False),
    'short int': (16, True), 'unsigned short int': (16, False), 'signed short int': (16, True),
    'int': (32, True), 'signed int': (32, True), 'unsigned int': (32, False), 'signed': (32, True),
    'unsigned': (32, False), 'long': (32, True), 'long int': (32, True), 'unsigned long': (32, False),
    'signed long': (32, True), 'unsigned long int': (32, False),
}


@dataclasses.dataclass
class CType:
    text: str
    width: int | None
    pointer: bool = False
    floating: bool = False
    signed: bool = False
    pointee: 'CType | None' = None
    const_pointee: bool = False
    fnproto: 'Prototype | None' = None
    struct: str | None = None

    def brief(self):
        return self.text


@dataclasses.dataclass
class Prototype:
    name: str
    ret: CType
    args: list
    known: bool = True
    variadic: bool = False
    parm: list = dataclasses.field(default_factory=lambda: ['eax', 'edx', 'ebx', 'ecx'])
    value: list = dataclasses.field(default_factory=lambda: ['eax'])
    modify: list = dataclasses.field(default_factory=lambda: ['eax'])
    cdecl: bool = False
    source: str = ''
    defined: bool = False
    pragma: bool = False
    contract: str = 'watcall default'

    def signature(self):
        return (self.ret.width, self.ret.pointer, tuple((a.width, a.pointer) for a in self.args), self.variadic,
                tuple(self.parm), tuple(self.value))

    def complete(self):
        """Every argument and the return have a known register/stack width."""
        return (self.known and self.ret.width is not None and all(a.width is not None for a in self.args)
                and (not self.args or len(self.parm) >= 0))


def strip_comments(text):
    return re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S)


def split_args(text):
    out = []; depth = 0; start = 0
    for k, ch in enumerate(text):
        if ch in '([': depth += 1
        elif ch in ')]': depth -= 1
        elif ch == ',' and depth == 0:
            out.append(text[start:k].strip()); start = k + 1
    out.append(text[start:].strip())
    return out


class Context:
    """Typedef environment of one translation unit (its include closure)."""

    def __init__(self):
        self.scalar = {}      # typedef name -> base text
        self.fntype = {}      # typedef name -> Prototype (function type, `typedef R (T)(A)`)
        self.fnptr = {}       # typedef name -> Prototype (pointer to function, `typedef R (*T)(A)`)
        self.ptrtype = {}     # typedef name -> pointee text (`typedef X *T`)
        self.structs = {}     # struct/typedef name -> member list text
        self.pragmas = {}     # name -> spec text

    def ctype(self, text, depth_guard=0):
        t = re.sub(r'\b(extern|static|register|typedef|volatile|__far|__near|__watcall|__cdecl|__pascal|_Packed)\b',
                   ' ', text).strip()
        t = re.sub(r'=.*$', '', t, flags=re.S).strip()
        m = re.match(r'^(.*?)\(\s*(?:__\w+\s+)?\*\s*(\w*)\s*(\[[^\]]*\])?\s*\)\s*\((.*)\)\s*$', t, re.S)
        if m:
            proto = self.make_proto('<fnptr>', m[1], m[4])
            return CType(' '.join(t.split()), 32, True, fnproto=proto)
        const_pointee = False
        star = t.find('*')
        if star >= 0 and re.search(r'\bconst\b', t[:star]):
            const_pointee = True
        depth = t.count('*') + len(re.findall(r'\[', t))
        base = re.sub(r'\[[^\]]*\]', ' ', t).replace('*', ' ')
        base = re.sub(r'\bconst\b', ' ', base)
        words = base.split()
        known_types = TYPE_WORDS | set(self.scalar) | set(self.fntype) | set(self.fnptr) | set(self.ptrtype) | set(self.structs)
        if len(words) > 1 and words[-1] not in known_types and words[-2] not in ('struct', 'union', 'enum'):
            words = words[:-1]          # drop the declarator name
        elif len(words) == 1 and words[0] not in known_types and depth == 0 and words[0] not in BUILTIN:
            pass                        # an unknown type name alone (kept: width unknown)
        base = ' '.join(words)
        text_out = ' '.join(t.split())
        if depth:
            pointee = self.ctype(base + ' *' * (depth - 1), depth_guard + 1) if depth_guard < 8 else None
            fn = None
            if depth == 1 and base in self.fntype:
                fn = self.fntype[base]
            elif depth == 1 and base in self.fnptr:
                fn = None  # pointer to a function pointer: pointee is a function pointer
            return CType(text_out, 32, True, pointee=pointee, const_pointee=const_pointee, fnproto=fn)
        return self.scalar_type(base, text_out, depth_guard)

    def scalar_type(self, base, text_out, guard=0):
        if base in self.fntype:
            # a parameter of function type decays to a pointer to that function type
            return CType(text_out, 32, True, fnproto=self.fntype[base])
        if base in self.fnptr:
            return CType(text_out, 32, True, fnproto=self.fnptr[base])
        if base in self.ptrtype and guard < 8:
            pointee = self.ctype(self.ptrtype[base], guard + 1)
            return CType(text_out, 32, True, pointee=pointee)
        if base in self.scalar and guard < 8:
            x = self.ctype(self.scalar[base], guard + 1)
            return dataclasses.replace(x, text=text_out)
        if base == 'void':
            return CType(text_out, 0)
        if base in ('float',):
            return CType(text_out, None, floating=True)
        if base in ('double', 'long double'):
            return CType(text_out, None, floating=True)
        if base in BUILTIN:
            w, s = BUILTIN[base]
            return CType(text_out, w, signed=s)
        if base in ('size_t', 'FILE', 'time_t', 'clock_t'):
            # Watcom 10.6 headers: size_t/time_t/clock_t are unsigned (long); FILE only appears behind pointers.
            return CType(text_out, 32, signed=False) if base != 'FILE' else CType(text_out, None, struct='FILE')
        m = re.match(r'^(struct|union)\s+(\w+)$', base)
        if m:
            return CType(text_out, None, struct=m[2])
        if base.startswith('enum'):
            return CType(text_out, None)  # Watcom sizes enums by range (-ei not used): never guessed
        if base in self.structs:
            return CType(text_out, None, struct=base)
        return CType(text_out, None)

    def make_proto(self, name, rtext, argtext, source='', defined=False):
        rt = self.ctype(rtext + ' x' if rtext.strip() and not rtext.strip().endswith('*') else rtext)
        a = argtext.strip()
        variadic = '...' in a
        known = a != ''          # `f()` is an unprototyped (K&R) declaration
        args = [] if a in ('', 'void') else [self.ctype(x) for x in split_args(a) if x.strip() != '...']
        cdecl = '__cdecl' in rtext
        p = Prototype(name, rt, args, known, variadic, cdecl=cdecl, source=source, defined=defined)
        if cdecl or variadic:
            # Watcom: __cdecl and variable-argument routines receive every argument on the stack, caller pops.
            p.parm = []; p.cdecl = True
            p.contract = '__cdecl' if cdecl else 'variadic (all arguments on stack, caller pops)'
        if rt.width == 0:
            p.value = []
        return p


def apply_pragma(p, spec):
    p = dataclasses.replace(p, args=list(p.args))
    a = re.search(r'\bparm\s+(?:caller\s+|routine\s+|reverse\s+)*((?:\[[^]]*\]\s*)*)', spec)
    if not a:
        a = re.search(r'\bparm\s*((?:\[[^]]*\]\s*)*)', spec)
    v = re.search(r'\bvalue\s*\[([^]]*)\]', spec)
    m = re.search(r'\bmodify\s+(exact\s*)?\[([^]]*)\]', spec)
    if a:
        groups = re.findall(r'\[([^]]*)\]', a[1])
        p.parm = [x.strip().lower() for x in groups if x.strip()]
    if v:
        p.value = v[1].lower().split()
    elif p.ret.width == 0:
        p.value = []
    if m:
        regs = m[2].lower().split()
        p.modify = regs if m[1] else list(dict.fromkeys(p.value + regs))
    elif p.ret.width == 0:
        p.modify = ['eax']
    p.pragma = True
    p.contract = '#pragma aux ' + ' '.join(spec.split())
    return p


def read_closure(src, cache):
    """Files of src's include closure, in inclusion order (project headers and pinned Watcom headers)."""
    seen = set(); out = []

    def visit(p):
        p = p.resolve()
        if p in seen or not p.exists():
            return
        seen.add(p)
        if p not in cache:
            cache[p] = p.read_text(encoding='utf-8', errors='replace')
        t = cache[p]
        for inc in re.findall(r'^\s*#\s*include\s*["<]([^">\n]+)[">]', t, re.M):
            for d in (p.parent, ROOT / 'src', ROOT / 'src/include', WATCOM_H):
                if (d / inc).exists():
                    visit(d / inc)
                    break
        out.append(p)
    visit(src)
    return out


FUNC_DECL = re.compile(r'^\s*((?:(?:extern|static|unsigned|signed|short|long|const|struct|union|__cdecl|__watcall|_WCRTLINK|_WCIRTLINK|_WCNEAR|_WMRTLINK)\s+)*\w+(?:\s*\*\s*)*(?:\s+(?:__cdecl|__watcall|_WCRTLINK|_WCIRTLINK|_WMRTLINK))?(?:\s*\*\s*)*)\s*(\w+)\s*\(', re.M)


class Table:
    """Global table: name -> Prototype, plus function-pointer variables and struct members."""

    def __init__(self, files=None):
        self.cache = {}
        self.functions = {}       # name -> list of Prototype (one per declaring TU context)
        self.variables = {}       # name -> (CType, source)
        self.struct_members = {}  # struct name -> list of (offset, name, CType)
        self.typedef_struct = {}  # typedef name -> struct tag/name used in struct_members
        self.contexts = {}
        files = files or self.project_files()
        for f in files:
            self.scan_unit(f)

    @staticmethod
    def project_files():
        src = ROOT / 'src'
        return (sorted(src.glob('*.c')) + sorted((src / 'data').glob('*.c')) + sorted((src / 'bss').glob('*.c')))

    def context_for(self, unit_file):
        unit_file = Path(unit_file).resolve()
        if unit_file not in self.contexts:
            self.scan_unit(unit_file)
        return self.contexts[unit_file]

    def scan_unit(self, unit_file):
        unit_file = Path(unit_file).resolve()
        ctx = Context()
        protos_here = {}
        for p in read_closure(unit_file, self.cache):
            raw = self.cache[p]
            s = strip_comments(raw)
            s = re.sub(r'#\s*ifdef\s+__cplusplus\b(?:(?!#\s*(?:if|else|endif)).)*(?:#\s*else((?:(?!#\s*(?:if|endif)).)*))?#\s*endif',
                       lambda m: m[1] or '', s, flags=re.S)
            s = re.sub(r'^\s*#(?!\s*pragma)[^\n]*(?:\\\n[^\n]*)*', '', s, flags=re.M)
            rel = str(p.relative_to(ROOT)) if p.is_relative_to(ROOT) else str(p)
            self.scan_typedefs(ctx, s)
            for m in re.finditer(r'^\s*#\s*pragma\s+aux\s+(\w+)\s+([^\n]*(?:\\\n[^\n]*)*)', s, re.M):
                if re.match(r'\s*=', m[2]) or m[2].lstrip().startswith('"'):
                    continue
                ctx.pragmas[m[1]] = m[2]
            body_free = self.blank_bodies(s)
            for m in FUNC_DECL.finditer(body_free):
                rt, name = m.groups()
                if name in ('if', 'while', 'switch', 'for', 'return', 'sizeof') or rt.strip() in ('return', 'else', 'case', 'goto', 'typedef'):
                    continue
                end = m.end(); depth = 1
                while end < len(body_free) and depth:
                    if body_free[end] == '(': depth += 1
                    elif body_free[end] == ')': depth -= 1
                    end += 1
                if depth:
                    continue
                tail = body_free[end:end + 40]
                if not re.match(r'\s*(;|\{)', tail):
                    continue
                defined = bool(re.match(r'\s*\{', tail))
                rtc = re.sub(r'\b(extern|static|_WCRTLINK|_WCIRTLINK|_WCNEAR|_WMRTLINK)\b', ' ', rt)
                proto = ctx.make_proto(name, rtc, body_free[m.end():end - 1], rel, defined and p == unit_file)
                if '__cdecl' in rt:
                    proto.cdecl = True; proto.parm = []; proto.contract = '__cdecl'
                protos_here[name] = proto
            self.scan_variables(ctx, body_free, rel)
        for name, proto in protos_here.items():
            if name in ctx.pragmas:
                proto = apply_pragma(proto, ctx.pragmas[name])
            self.functions.setdefault(name, []).append(proto)
        self.contexts[unit_file] = ctx

    @staticmethod
    def blank_bodies(s):
        """Replace function bodies and struct bodies by spaces (keeping offsets) for top-level scans."""
        out = list(s); depth = 0
        for k, ch in enumerate(s):
            if ch == '{':
                depth += 1
                if depth > 1: out[k] = ' '
            elif ch == '}':
                if depth > 1: out[k] = ' '
                depth -= 1
            elif depth >= 1 and ch != '\n':
                out[k] = ' '
        return ''.join(out)

    def scan_typedefs(self, ctx, s):
        for a, b in re.findall(r'\btypedef\s+((?:(?:signed|unsigned)\s+)?(?:char|short|int|long|s8|u8|s16|u16|s32|u32|bool|size_t)(?:\s+int)?)\s+(\w+)\s*;', s):
            ctx.scalar[b] = a
        for m in re.finditer(r'\btypedef\s+([\w\s\*]+?)\(\s*(\*?)\s*(\w+)\s*\)\s*\(([^;{}]*)\)\s*;', s):
            proto = ctx.make_proto(m[3], m[1], m[4])
            if m[3] in ctx.pragmas:
                proto = apply_pragma(proto, ctx.pragmas[m[3]])
            (ctx.fnptr if m[2] else ctx.fntype)[m[3]] = proto
        for m in re.finditer(r'\btypedef\s+([\w\s]+?)\s+(\w+)\s*\(([^;{}()]*)\)\s*;', s):
            if m[1].strip() in ('struct', 'union'):
                continue
            ctx.fntype[m[2]] = ctx.make_proto(m[2], m[1], m[3])
        for m in re.finditer(r'\btypedef\s+((?:struct|union)\s+\w+|\w+)\s*\*\s*(\w+)\s*;', s):
            ctx.ptrtype[m[2]] = m[1]
        for m in re.finditer(r'\b(?:typedef\s+)?(struct|union)\s+(\w*)\s*\{', s):
            start = m.end(); depth = 1; k = start
            while k < len(s) and depth:
                if s[k] == '{': depth += 1
                elif s[k] == '}': depth -= 1
                k += 1
            body = s[start:k - 1]
            after = re.match(r'\s*(\w+)\s*;', s[k:])
            names = [x for x in (m[2], after[1] if after and m[0].startswith('typedef') else None) if x]
            for n in names:
                ctx.structs[n] = (m[1], body)
        # Pragmas on typedef'd function types (declared after the typedef).
        for name, spec in list(ctx.pragmas.items()):
            for tab in (ctx.fntype, ctx.fnptr):
                if name in tab and not tab[name].pragma:
                    tab[name] = apply_pragma(tab[name], spec)
        for m in re.finditer(r'^\s*#\s*pragma\s+aux\s+(\w+)\s+([^\n]*)', s, re.M):
            for tab in (ctx.fntype, ctx.fnptr):
                if m[1] in tab and not re.match(r'\s*=', m[2]):
                    tab[m[1]] = apply_pragma(tab[m[1]], m[2])
            ctx.pragmas.setdefault(m[1], m[2])

    def scan_variables(self, ctx, s, rel):
        for raw in s.split(';'):
            raw = re.sub(r'^\s*#[^\n]*', '', raw, flags=re.M).strip()
            if '{' in raw or '}' in raw:
                raw = re.split(r'[{}]', raw)[-1].strip()
            raw = raw.split('=', 1)[0].strip()
            text = re.sub(r'^(?:(?:extern|static|const|volatile)\s+)+', '', raw).strip()
            if not text or text.startswith('typedef') or len(text) > 400:
                continue
            # R (*name[N])(args)
            m = re.match(r'^(.*?)\(\s*\*\s*(\w+)\s*((?:\[[^\]]*\])*)\s*\)\s*\((.*)\)\s*$', text, re.S)
            if m:
                proto = ctx.make_proto(m[2], m[1], m[4], rel)
                t = CType(' '.join(text.split()), 32, True, fnproto=proto)
                self.add_variable(m[2], (t, rel, bool(m[3])))
                continue
            if '(' in text:
                continue
            m = re.match(r'^([\w\s\*]+?)\s*(\w+)\s*((?:\[[^\]]*\])*)$', text)
            if not m:
                continue
            base, name, dims = m[1].strip(), m[2], m[3]
            bare = base.replace('*', ' ').split()
            if not bare:
                continue
            tname = bare[-1]
            stars = base.count('*')
            if tname in ctx.fntype and stars == 0 and not dims:
                # `extern block_copy_t block_copy;` declares a FUNCTION of a typedef'd function type.
                proto = dataclasses.replace(ctx.fntype[tname], name=name, source=rel)
                self.functions.setdefault(name, []).append(proto)
            elif tname in ctx.fntype and stars == 1:
                self.add_variable(name, (CType(text, 32, True, fnproto=ctx.fntype[tname]), rel, bool(dims)))
            elif tname in ctx.fnptr and stars == 0:
                self.add_variable(name, (CType(text, 32, True, fnproto=ctx.fnptr[tname]), rel, bool(dims)))
            elif tname in ctx.structs and stars == 0 and not dims:
                members = self.layout(ctx, tname)
                if members:
                    self.variables.setdefault(name, (CType(text, None, struct=tname), rel, False))
                    self.struct_members.setdefault(tname, members)

    def add_variable(self, name, entry):
        def rank(e):
            fp = e[0].fnproto
            return (fp is not None and fp.complete(), fp is not None and fp.pragma)
        old = self.variables.get(name)
        if old is None or rank(entry) > rank(old):
            self.variables[name] = entry

    def layout(self, ctx, name, guard=0):
        """Member offsets of a struct whose layout is the same under pack(1) and natural alignment."""
        if name not in ctx.structs or guard > 4:
            return None
        kind, body = ctx.structs[name]
        out = []; off1 = offn = 0
        for decl in body.split(';'):
            decl = decl.strip()
            if not decl:
                continue
            if ':' in decl or '{' in decl or '}' in decl:
                return out or None   # bitfields / nested definitions: stop; earlier members stay valid
            t = ctx.ctype(decl)
            dims = [int(x, 0) for x in re.findall(r'\[\s*(\w+)\s*\]', re.sub(r'\(.*', '', decl)) if re.fullmatch(r'0x[0-9a-fA-F]+|\d+', x)]
            count = 1
            for d in dims: count *= d
            if t.width is None or t.width == 0:
                return out or None
            size = t.width // 8
            align = size if size in (1, 2, 4) else 4
            offn = (offn + align - 1) // align * align
            if off1 != offn:
                return out or None
            mname = re.findall(r'(\w+)\s*(?:\[[^\]]*\]\s*)*$', re.sub(r'\(\s*\*\s*(\w+)\s*\)\s*\(.*\)', r'\1', decl))
            out.append((off1, mname[0] if mname else '?', t))
            off1 += size * count; offn += size * count
            if kind == 'union':
                off1 = offn = 0
        return out

    def prototype(self, name):
        """(Prototype or None, note).  Definition wins; otherwise all complete declarations must agree."""
        cands = self.functions.get(name, [])
        if not cands:
            return None, 'no declaration in src/ or included Watcom headers'
        defs = [p for p in cands if p.defined]
        if defs:
            best = defs[0]
            prag = [p for p in cands if p.pragma]
            if prag and not best.pragma:
                best = dataclasses.replace(best, parm=prag[0].parm, value=prag[0].value, modify=prag[0].modify,
                                           pragma=True, contract=prag[0].contract)
            return best, 'definition ' + best.source
        full = [p for p in cands if p.known and p.complete()]
        if not full:
            return cands[0], 'only unprototyped/incomplete declarations'
        prag = [p for p in full if p.pragma]
        pool = prag or full
        sigs = {p.signature() for p in pool}
        if len(sigs) > 1:
            # Report a conflict; prefer the pragma (register contract) / project-header declaration.
            hdr = [p for p in pool if '/include/' in p.source.replace('\\', '/') or p.source.endswith('.h')]
            best = (hdr or pool)[0]
            return best, 'conflicting declarations: ' + '; '.join(sorted({p.source for p in pool}))
        return pool[0], 'declaration ' + pool[0].source

    def fn_variable(self, name):
        v = self.variables.get(name)
        if v and v[0].fnproto:
            return v
        return None
