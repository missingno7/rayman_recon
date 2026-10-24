"""Guarded, body-only Watcom C permutations. Requires pycparser >= 2.21.

The commutative/reverse-condition passes are adapted from Simon Lindholm's
decomp-permuter/src/randomizer.py (MIT; see permute.LICENSE). All other code is
purpose-built. Unlike the upstream randomizer, no arbitrary type/prototype
changes, dead reads, arithmetic identities with extra overflow, or volatile
tricks are offered. Guards deliberately sacrifice coverage for soundness.
"""
from __future__ import annotations

import copy
import functools
import re
from dataclasses import dataclass

from pycparser import c_ast as C, c_generator, c_parser


class Refusal(ValueError):
    pass


DIRECTIVE = re.compile(r"(?m)^[ \t]*#(?:[^\n]*\\\n)*[^\n]*(?:\n|$)")
LEX = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|'
                 r"'(?:\\.|[^'\\])*'|[A-Za-z_]\w*|\S")
FAR = re.compile(r"\b(?:__far|_far|far)\b")
NEAR = re.compile(r"\b(?:__near|_near|near)\b")
CONVENTION = re.compile(r"\b(?:__cdecl|_cdecl|__watcall|__pascal|_pascal|__stdcall|__inline)\b")
MECHANISMS = {
    "colour": ["declarations", "commute", "temporary", "promote_snapshot"],
    "home": ["declarations", "scope", "temporary", "address_copy"],
    "Score": ["temporary", "promote_snapshot", "goto", "duplicate_tail", "branch"],
    "rover": ["cases", "branch", "predicate", "duplicate_tail"],
    "schedule": ["independent", "temporary", "address_copy", "compound"],
    "selection": ["commute", "threshold", "compound", "predicate", "increment"],
}
CONSERVATIVE = ("commute", "declarations", "independent", "predicate", "branch",
                "compound", "increment", "threshold", "scope", "cases")
AGGRESSIVE = CONSERVATIVE + ("temporary", "promote_snapshot", "address_copy", "goto",
                            "duplicate_tail")


def walk(node):
    yield node
    for _, child in node.children():
        yield from walk(child)


@functools.lru_cache(maxsize=64)
def body_bounds(source, function):
    """Find a unique top-level definition without interpreting comments/strings (memoised: the same unit text is
    scanned for every candidate)."""
    clean = DIRECTIVE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), source)
    ts = [m for m in LEX.finditer(clean) if not m[0].startswith(("//", "/*"))]
    depth, found = 0, []
    for i, tok in enumerate(ts):
        if depth == 0 and tok[0] == function and i + 1 < len(ts) and ts[i + 1][0] == "(":
            j, parens = i + 1, 0
            while j < len(ts):
                parens += (ts[j][0] == "(") - (ts[j][0] == ")")
                j += 1
                if not parens:
                    break
            if j < len(ts) and ts[j][0] == "{":
                k, braces = j, 0
                while k < len(ts):
                    braces += (ts[k][0] == "{") - (ts[k][0] == "}")
                    k += 1
                    if not braces:
                        break
                if braces:
                    raise Refusal("unbalanced function body")
                found.append((ts[j].start(), ts[k - 1].end()))
        depth += (tok[0] == "{") - (tok[0] == "}")
    if len(found) != 1:
        raise Refusal(f"expected one top-level definition of {function}, found {len(found)}")
    return found[0]


def sanitize(pp):
    # Only the parser's view is sanitized. Context/signature are never emitted.
    pp = DIRECTIVE.sub(lambda m: re.sub(r"[^\n]", " ", m[0]), pp)
    tokens = [m[0] for m in LEX.finditer(pp)]
    if any(t in ("restrict", "_Atomic") for t in tokens):
        raise Refusal("reserved parser placeholders restrict/_Atomic already present")
    def fix(m):
        t = m[0]
        if t.startswith(("//", "/*")):
            return re.sub(r"[^\n]", " ", t)
        if FAR.fullmatch(t):
            return "restrict"
        if NEAR.fullmatch(t):
            return "_Atomic"
        return "" if CONVENTION.fullmatch(t) else t
    return LEX.sub(fix, pp)


def braced(node):
    """Copy of node whose if/else branches are compound statements.

    pycparser's generator braces only Compound nodes, so an unbraced nested if in the true arm of an if/else would
    capture the outer else when the C is re-read (pilot_mine 2026-10-23: alter_fist_speed). Braces never change
    the meaning of a statement."""
    node = copy.deepcopy(node)
    for n in walk(node):
        if isinstance(n, C.If):
            if n.iftrue is not None and not isinstance(n.iftrue, C.Compound):
                n.iftrue = C.Compound([n.iftrue])
            if n.iffalse is not None and not isinstance(n.iffalse, (C.Compound, C.If)):
                n.iffalse = C.Compound([n.iffalse])
    return node


def emit(node):
    out = c_generator.CGenerator().visit(braced(node))
    return LEX.sub(lambda m: {"restrict": "__far", "_Atomic": "__near"}.get(m[0], m[0]), out)


def preserve_lines(body, original):
    """Keep downstream __LINE__/assert expansions at their original lines.

    Target-local builtins were already expanded by the pinned preprocessor.
    Change only whitespace outside tokens; never touch literal contents.
    """
    difference = body.count("\n") - original.count("\n")
    if difference < 0:
        return body[:-1] + "\n" * -difference + body[-1:]
    if difference > 0:
        protected = {i for m in LEX.finditer(body) if m[0].startswith(('"', "'"))
                     for i in range(m.start(), m.end())}
        chars = list(body)
        for i, char in enumerate(chars):
            if char == "\n" and i not in protected and difference:
                chars[i] = " "
                difference -= 1
        if difference:
            raise Refusal("cannot preserve source lines without modifying a literal")
        body = "".join(chars)
    return body


def replace(root, old, new):
    for attr in root.__slots__:
        if attr in ("coord", "__weakref__"):
            continue
        value = getattr(root, attr, None)
        if value is old:
            setattr(root, attr, new)
            return True
        if isinstance(value, list):
            for i, item in enumerate(value):
                if item is old:
                    value[i] = new
                    return True
    return any(replace(child, old, new) for _, child in root.children())


@dataclass
class Model:
    source: str
    function: str
    ast: C.FileAST
    macros: frozenset = frozenset()
    context: str = ""
    mini: str = ""

    def round_trip(self, text):
        """The emitted body must re-read as exactly the intended tree (refuse otherwise).

        Parsed in a mini context (one `typedef int T;` per typedef in scope + the signature): typedef-ness is the only
        context the C grammar needs, and re-parsing the whole preprocessed unit per candidate cost ~0.5 s. The full
        context is the fallback, so a mini-context failure never weakens the check."""
        if not self.context:
            return
        again = None
        if self.mini:
            try:
                again = c_parser.CParser().parse(self.mini + sanitize(text) + "\n")
            except Exception:
                again = None
        if again is None:
            a, b = body_bounds(self.context, self.function)
            try:
                again = c_parser.CParser().parse(self.context[:a] + sanitize(text) + self.context[b:])
            except Exception as exc:
                raise Refusal(f"emitted body does not re-parse: {exc}") from exc
        if emit(self.target(again).body).rstrip() != text:
            raise Refusal("emitted body re-reads as a different tree")

    @classmethod
    def parse(cls, source, pp, function, macros=()):
        a, b = body_bounds(source, function)
        raw = source[a:b]
        if DIRECTIVE.search(raw) or CONVENTION.search(raw):
            raise Refusal("target-local directives/calling conventions are not rewritten")
        if re.search(r"\b(?:asm|__asm|_asm|_emit|__declspec|__based)\b", raw):
            raise Refusal("unsupported target-local extension")
        try:
            ast = c_parser.CParser().parse(sanitize(pp))
        except Exception as exc:
            raise Refusal(f"Watcom preprocessed context did not parse: {exc}") from exc
        fns = [n for n in ast.ext if isinstance(n, C.FuncDef) and n.decl.name == function]
        if len(fns) != 1:
            raise Refusal("preprocessed target absent or ambiguous")
        # Fixed automatic arrays are kept, never permuted as declarations.
        # No target-local static storage, VLA, typedef or local prototype.
        for n in walk(fns[0].body):
            if isinstance(n, (C.Typedef, C.Pragma)):
                raise Refusal("target-local typedef/pragma needs a lossless source pass")
            if isinstance(n, C.Decl) and (n.storage or isinstance(n.type, C.FuncDecl)):
                raise Refusal("target-local storage/prototype is outside the safe subset")
            if isinstance(n, C.ArrayDecl) and n.dim is not None and not (isinstance(n.dim, C.Constant) and "int" in n.dim.type):
                raise Refusal("nonliteral array extent (possible VLA) is outside the safe subset")
        macros = frozenset(macros) | frozenset(re.findall(r"(?m)^\s*#\s*define\s+(\w+)", source))
        typedefs = "".join(f"typedef int {n.name};\n" for n in ast.ext if isinstance(n, C.Typedef))
        signature = c_generator.CGenerator().visit(fns[0].decl)
        model = cls(source, function, ast, macros, sanitize(pp), typedefs + signature + "\n")
        model.unit(fns[0])  # reject possible second macro expansion before search
        return model

    def target(self, ast=None):
        return next(n for n in (ast or self.ast).ext
                    if isinstance(n, C.FuncDef) and n.decl.name == self.function)

    def unit(self, fn):
        a, b = body_bounds(self.source, self.function)
        text = emit(fn.body).rstrip()
        self.round_trip(text)
        body = preserve_lines(text, self.source[a:b])
        residual = sorted({m[0] for m in LEX.finditer(body) if m[0].isidentifier()} & self.macros)
        if residual:
            raise Refusal("emitted body could re-expand macro(s): " + ", ".join(residual[:8]))
        return self.source[:a] + body + self.source[b:]


_UNIT_CACHE = {}


def unit_facts(ast):
    """Typedefs and every declared/used name of a parsed unit, computed once per tree (the tree is never mutated:
    mutate() works on a copy of the target body)."""
    key = id(ast)
    if key not in _UNIT_CACHE or _UNIT_CACHE[key][0] is not ast:
        typedefs = {n.name: n.type for n in ast.ext if isinstance(n, C.Typedef)}
        names = frozenset(n.name for n in walk(ast) if isinstance(n, (C.ID, C.Decl, C.Label)))
        _UNIT_CACHE[key] = (ast, typedefs, names)
    return _UNIT_CACHE[key]


def unit_names(ast):
    return unit_facts(ast)[2]


class Facts:
    def __init__(self, ast, fn):
        self.typedefs = unit_facts(ast)[1]
        # Struct members and function-pointer parameter names are not locals.
        # Never infer a global's type from a same-spelled member declaration.
        decls = [n for n in (fn.decl.type.args.params if fn.decl.type.args else []) if isinstance(n, C.Decl)]
        def local_declarations(node):
            if isinstance(node, C.Decl):
                decls.append(node)
                if node.init:
                    local_declarations(node.init)
            else:
                for _, child in node.children():
                    local_declarations(child)
        local_declarations(fn.body)
        self.locals = {n.name: n for n in decls if n.name and
                       sum(x.name == n.name for x in decls) == 1}

    def integer(self, typ, seen=()):
        if getattr(typ, "quals", None):
            # Includes volatile and address-space qualifiers. Conservative.
            return False
        if isinstance(typ, C.TypeDecl):
            return self.integer(typ.type, seen)
        if isinstance(typ, C.IdentifierType):
            ns = typ.names
            if len(ns) == 1 and ns[0] in self.typedefs and ns[0] not in seen:
                return self.integer(self.typedefs[ns[0]], seen + (ns[0],))
            return bool(ns) and all(n in ("signed", "unsigned", "char", "short", "int", "long") for n in ns)
        return False

    def local(self, name):
        return name in self.locals and self.integer(self.locals[name].type)

    def pure(self, n):
        if isinstance(n, C.ID):
            return self.local(n.name)
        if isinstance(n, C.Constant):
            return "int" in n.type or n.type == "char"
        if isinstance(n, C.BinaryOp):
            return self.pure(n.left) and self.pure(n.right)
        if isinstance(n, C.UnaryOp) and n.op in ("+", "-", "!", "~"):
            return self.pure(n.expr)
        if isinstance(n, C.Cast):
            return self.integer(n.to_type.type) and self.pure(n.expr)
        return False


def renamed_decl(decl, name):
    d = copy.deepcopy(decl)
    d.name, d.init, d.storage = name, None, []
    for n in walk(d.type):
        if isinstance(n, C.TypeDecl):
            n.declname = name
    return d


def mutations(ast, fn, family):
    """Yield (replacement target, replacement, proof note) for ONE safe pass."""
    facts = Facts(ast, fn)
    nodes = list(walk(fn.body))
    used = set(unit_names(ast)) | {n.name for n in nodes if isinstance(n, (C.ID, C.Decl, C.Label))}
    def fresh(stem):
        i = 0
        while f"perm_{stem}_{i}" in used:
            i += 1
        result = f"perm_{stem}_{i}"
        used.add(result)
        return result
    for n in nodes:
        if family == "commute" and isinstance(n, C.BinaryOp):
            # Adapted upstream perm_commutative, guarded to pure local integers.
            inverse = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}
            if n.op in ("+", "*", "|", "&", "^", "==", "!=", *inverse) and facts.pure(n):
                q = copy.deepcopy(n)
                q.left, q.right, q.op = q.right, q.left, inverse.get(n.op, n.op)
                yield n, q, "commute pure integral operands; no reassociation"
        if family == "branch" and isinstance(n, (C.If, C.TernaryOp)) and n.iffalse is not None:
            # Adapted upstream perm_reverse_cond. Evaluate condition exactly once.
            q = copy.deepcopy(n)
            q.cond = C.UnaryOp("!", q.cond)
            q.iftrue, q.iffalse = q.iffalse, q.iftrue
            yield n, q, "invert condition and exchange both arms"
        if family == "predicate" and isinstance(n, (C.If, C.For, C.While, C.DoWhile)) and n.cond:
            q = copy.deepcopy(n)
            q.cond = C.BinaryOp("!=", q.cond, C.Constant("int", "0"))
            yield n, q, "condition truth value only; one evaluation"
        if family == "threshold" and isinstance(n, C.BinaryOp) and facts.pure(n.left):
            if isinstance(n.right, C.Constant) and re.fullmatch(r"[0-9]+", n.right.value):
                # A leading zero is octal in C (Python's int(s) is decimal).
                text = n.right.value
                value = int(text, 8 if len(text) > 1 and text.startswith("0") else 10)
                if n.op == ">" and 0 <= value < 2147483647:
                    yield n, C.BinaryOp(">=", copy.deepcopy(n.left), C.Constant("int", str(value + 1))), "integer threshold in range"
                if n.op == "<" and 0 < value <= 2147483647:
                    yield n, C.BinaryOp("<=", copy.deepcopy(n.left), C.Constant("int", str(value - 1))), "integer threshold in range"
        if family == "compound" and isinstance(n, C.Assignment) and isinstance(n.lvalue, C.ID):
            name = n.lvalue.name
            if facts.local(name) and facts.pure(n.rvalue):
                if n.op in ("+=", "-=", "*=", "&=", "|=", "^="):
                    yield n, C.Assignment("=", copy.deepcopy(n.lvalue), C.BinaryOp(n.op[:-1], copy.deepcopy(n.lvalue), copy.deepcopy(n.rvalue))), "single local lvalue; identical arithmetic conversion"
                if (n.op == "=" and isinstance(n.rvalue, C.BinaryOp) and n.rvalue.op in ("+", "-", "*", "&", "|", "^") and
                    isinstance(n.rvalue.left, C.ID) and n.rvalue.left.name == name):
                    yield n, C.Assignment(n.rvalue.op + "=", copy.deepcopy(n.lvalue), copy.deepcopy(n.rvalue.right)), "single local lvalue; identical arithmetic conversion"
        if not isinstance(n, C.Compound):
            continue
        items = n.block_items or []
        if family == "declarations":
            ds = []
            for i, s in enumerate(items):
                if not isinstance(s, C.Decl):
                    break
                if not (s.name and facts.local(s.name) and s.init is None and not s.storage):
                    break
                ds.append(i)
            for i, a in enumerate(ds):
                for b in ds[i + 1:]:
                    q = copy.deepcopy(n)
                    q.block_items[a], q.block_items[b] = q.block_items[b], q.block_items[a]
                    yield n, q, "exchange uninitialized fixed scalar declarations in same scope"
        if family == "cases" and items and all(isinstance(s, (C.Case, C.Default)) for s in items):
            # No fallthrough, declaration visibility, nested labels, or new paths.
            if all(s.stmts and isinstance(s.stmts[-1], (C.Break, C.Return)) and
                   not any(isinstance(x, (C.Decl, C.Label, C.Goto, C.Case, C.Default))
                           for t in s.stmts for x in walk(t)) for s in items):
                for i in range(len(items) - 1):
                    q = copy.deepcopy(n)
                    q.block_items[i], q.block_items[i + 1] = q.block_items[i + 1], q.block_items[i]
                    yield n, q, "exchange terminated switch cases with no fallthrough/declarations"
        for i, s in enumerate(items):
            if family == "increment" and isinstance(s, C.UnaryOp) and isinstance(s.expr, C.ID) and facts.local(s.expr.name):
                inverses = {"p++": "++", "++": "p++", "p--": "--", "--": "p--"}
                if s.op in inverses:
                    yield s, C.UnaryOp(inverses[s.op], copy.deepcopy(s.expr)), "unused prefix/postfix result"
            if family == "scope" and not isinstance(s, (C.Decl, C.Case, C.Default, C.Label, C.Compound)):
                yield s, C.Compound([copy.deepcopy(s)]), "new scope contains no declarations"
            if family == "independent" and i + 1 < len(items):
                t = items[i + 1]
                if all(isinstance(z, C.Assignment) and z.op == "=" and isinstance(z.lvalue, C.ID) and
                       facts.local(z.lvalue.name) and facts.pure(z.rvalue) for z in (s, t)):
                    reads = lambda z: {x.name for x in walk(z.rvalue) if isinstance(x, C.ID)}
                    if s.lvalue.name != t.lvalue.name and s.lvalue.name not in reads(t) and t.lvalue.name not in reads(s):
                        q = copy.deepcopy(n)
                        q.block_items[i], q.block_items[i + 1] = q.block_items[i + 1], q.block_items[i]
                        yield n, q, "adjacent independent pure local assignments; no memory alias"
            if family in ("temporary", "address_copy", "promote_snapshot") and isinstance(s, C.Assignment) and s.op == "=" and isinstance(s.lvalue, C.ID) and facts.local(s.lvalue.name):
                name = fresh("value")
                d = renamed_decl(facts.locals[s.lvalue.name], name)
                if family == "temporary":
                    yield s, C.Compound([d, C.Assignment("=", C.ID(name), copy.deepcopy(s.rvalue)),
                                        C.Assignment("=", copy.deepcopy(s.lvalue), C.ID(name))]), "same destination type; conversion at original assignment point"
                elif family == "address_copy":
                    pn = fresh("address")
                    p = renamed_decl(d, pn)
                    p.type = C.PtrDecl([], p.type)
                    yield s, C.Compound([d, p, C.Assignment("=", C.ID(pn), C.UnaryOp("&", C.ID(name))),
                                        C.Assignment("=", C.UnaryOp("*", C.ID(pn)), copy.deepcopy(s.rvalue)),
                                        C.Assignment("=", copy.deepcopy(s.lvalue), C.ID(name))]), "address-taken fresh local; no escaping pointer; initialized before read"
                elif facts.pure(s.rvalue):
                    for expr in walk(s.rvalue):
                        if isinstance(expr, C.ID) and facts.local(expr.name):
                            # Keep exact type: an explicit copy retains promotion at use.
                            d = renamed_decl(facts.locals[expr.name], name)
                            q = copy.deepcopy(s)
                            # Replace the corresponding occurrence by traversal ordinal.
                            ordinal = list(walk(s.rvalue)).index(expr)
                            target = list(walk(q.rvalue))[ordinal]
                            if target is q.rvalue:
                                q.rvalue = C.ID(name)
                            else:
                                replace(q.rvalue, target, C.ID(name))
                            yield s, C.Compound([d, C.Assignment("=", C.ID(name), copy.deepcopy(expr)), q]), "same-width operand snapshot; original promotion at use"
            if family == "goto" and isinstance(s, C.If) and not any(isinstance(x, (C.Label, C.Goto)) for x in walk(fn.body)):
                other, end = fresh("else"), fresh("end")
                yield s, C.Compound([C.If(C.UnaryOp("!", copy.deepcopy(s.cond)), C.Goto(other), None),
                                    C.Compound([copy.deepcopy(s.iftrue)]), C.Goto(end),
                                    C.Label(other, C.Compound([copy.deepcopy(s.iffalse)] if s.iffalse else [])),
                                    C.Label(end, C.EmptyStatement())]), "structured if lowered to fresh forward labels; no loops/exit targets changed"
            if family == "duplicate_tail" and isinstance(s, C.If) and s.iffalse is None and i + 1 == len(items) - 1 and isinstance(items[-1], C.Return):
                if not any(isinstance(x, (C.Label, C.Goto)) for x in walk(fn.body)):
                    q = copy.deepcopy(n)
                    q.block_items[i:] = [C.If(copy.deepcopy(s.cond), C.Compound([copy.deepcopy(s.iftrue), copy.deepcopy(items[-1])]), copy.deepcopy(items[-1]))]
                    yield n, q, "duplicate trailing return into exclusive paths; evaluate once"


def mutate(model, parent, rng, mode, mechanisms=()):
    # Only the target body changes: copy it, never the whole unit tree (that deepcopy was ~0.9 s per mutation).
    ast = model.ast
    fn = copy.copy(model.target(ast))
    fn.body = copy.deepcopy(parent.body)
    families = list(CONSERVATIVE if mode == "conservative" else AGGRESSIVE)
    guided = [f for m in mechanisms for f in MECHANISMS.get(m, []) if f in families]
    families += guided * 2
    rng.shuffle(families)
    for family in families:
        options = list(mutations(ast, fn, family))
        if options:
            old, new, proof = rng.choice(options)
            replace(fn.body, old, new) if old is not fn.body else setattr(fn, "body", new)
            return fn, {"family": family, "proof": proof,
                        "quality": "SYNTHETIC" if family in AGGRESSIVE[len(CONSERVATIVE):] else "COMPILER_STEERED"}
    raise Refusal("no guarded mutation applies")
