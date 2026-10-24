"""semir.py - semantic-IR block comparison (Experiment B, adopted 2026-10-24). Diagnostic only, never a matching verdict.

    python tools/semir.py UNIT [FUNC...] [--src COPY.c] [--json OUT]

Use: the earliest COMPUTATIONAL difference between the DOS function and ours (local width, branch/test order, operand
order, load/store order, an extra or missing store) - a source clue; EXACT means only allocation/scheduling is left.
Validated: 287/288 STRICT functions EXACT, 11/11 semantic mutants MISMATCH; guided editing took RecaleRayPosInJumelle
581 -> 24 differing bytes (docs/CAUSES.md H14). Limits: CFG-shape differences, escaped locals, no FPU model.

Lifts the DOS function (bound address) and our compiled function (current src unit or a copy) into width-exact
expressions per basic block, aligns the CFGs by successor structure and canonical branch predicates, and compares the
observable effects of each aligned block pair (non-stack stores, calls with arguments, rep string ops, branch
predicate, return value) under a bijective renaming of join values (phi locations).

Verdicts: EXACT_LOCAL_EQUIVALENT (all aligned, all effects equal under the stated assumptions), STRUCTURALLY_SIMILAR
(no difference found but alignment/stack-escape prevents EXACT), MISMATCH (earliest definite difference), UNKNOWN
(an unsupported instruction reaches a compared value before any definite difference).

Assumptions (stated per result): Watcom call ABI (callee parameters = registers live at the callee's entry in the DOS
code; registers preserved per census call_effects), stack slots are only accessed directly unless their address
escapes (escape caps the verdict at STRUCTURALLY_SIMILAR), DF=0 for string instructions, returns compared at 32 bits
unless our source declares the function void.
"""
import collections
import itertools
import json
import re
import struct
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(ROOT / "tools"))
import capstone  # noqa: E402
from capstone import x86 as X  # noqa: E402
import census  # noqa: E402
import match  # noqa: E402
import omf  # noqa: E402
import wcc  # noqa: E402
from common import load_json  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True
M32 = 0xFFFFFFFF
REGS = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")
PART = {"eax": ("eax", 0, 32), "ax": ("eax", 0, 16), "al": ("eax", 0, 8), "ah": ("eax", 8, 8),
        "ebx": ("ebx", 0, 32), "bx": ("ebx", 0, 16), "bl": ("ebx", 0, 8), "bh": ("ebx", 8, 8),
        "ecx": ("ecx", 0, 32), "cx": ("ecx", 0, 16), "cl": ("ecx", 0, 8), "ch": ("ecx", 8, 8),
        "edx": ("edx", 0, 32), "dx": ("edx", 0, 16), "dl": ("edx", 0, 8), "dh": ("edx", 8, 8),
        "esi": ("esi", 0, 32), "si": ("esi", 0, 16), "edi": ("edi", 0, 32), "di": ("edi", 0, 16),
        "ebp": ("ebp", 0, 32), "bp": ("ebp", 0, 16), "esp": ("esp", 0, 32), "sp": ("esp", 0, 16)}


def mask(w):
    return (1 << w) - 1


def sgn(v, w):
    v &= mask(w)
    return v - (1 << w) if v >> (w - 1) else v


class Unsupported(Exception):
    pass


# ----------------------------------------------------------------------------------------------- expressions
# Every expression is a tuple (op, width, ...). Constructors normalise; tuples compare structurally.
_uid = itertools.count()


def C(w, v):
    return ("c", w, v & mask(w))


def is_c(e):
    return e[0] == "c"


def unk(w, why):
    return ("unk", w, next(_uid), why)


_kmemo = {}
_smemo = {}
ABSTRACT = ("phi", "unk", "code", "Mphi", "local", "opaque")


def key(e):
    """Side-independent sort key (int hash; renamable atoms abstracted)."""
    if not isinstance(e, tuple) or not e:
        return hash(e)
    k = _kmemo.get(id(e))
    if k is not None and k[0] is e:
        return k[1]
    op = e[0]
    if op in ABSTRACT:
        r = hash((op,))
    elif op == "tr" and (e[2][0] == "phi" or (e[2][0] == "shr" and e[2][2][0] == "phi" and is_c(e[2][3]))):
        r = hash(("phiview", e[1]))
    elif op == "sum" and any(t[0] == "SP0" for _, t in e[3]):
        r = hash(("sum", e[1], "SP0"))
    else:
        r = hash(tuple(key(x) for x in e))
    _kmemo[id(e)] = (e, r)
    return r


def skey(e):
    """Exact structural hash (side-local identity), memoised by object."""
    if not isinstance(e, tuple) or not e:
        return hash(e)
    k = _smemo.get(id(e))
    if k is not None and k[0] is e:
        return k[1]
    r = hash(("#",) + tuple(skey(x) for x in e))
    _smemo[id(e)] = (e, r)
    return r


def as_sum(e):
    """(const, {skey: [term, coef]})"""
    if e[0] == "sum":
        return e[2], {skey(t): [t, c] for c, t in e[3]}
    if e[0] == "c":
        return e[2], {}
    return 0, {skey(e): [e, 1]}


def mk_sum(w, c, terms):
    if any(t[0] in ("sum", "c") and t[1] == w for t, _ in terms.values()):     # flatten nested sums
        flat = {}
        for t, v in terms.values():
            if t[0] in ("sum", "c") and t[1] == w:
                tc, tt = as_sum(t)
                c += v * tc
                for h, (x, xv) in tt.items():
                    flat.setdefault(h, [x, 0])[1] += v * xv
            else:
                flat.setdefault(skey(t), [t, 0])[1] += v
        terms = flat
    c &= mask(w)
    ts = [(v & mask(w), t) for t, v in terms.values() if v & mask(w)]
    if not ts:
        return C(w, c)
    if len(ts) == 1 and ts[0][0] == 1 and c == 0:
        return ts[0][1]
    ts.sort(key=lambda ct: (key(ct[1]), ct[0]))
    return ("sum", w, c, tuple(ts))


def add(w, a, b, sb=1):
    ca, ta = as_sum(a)
    cb, tb = as_sum(b)
    for k, (x, v) in tb.items():
        if k in ta:
            ta[k][1] += sb * v
        else:
            ta[k] = [x, sb * v]
    return mk_sum(w, ca + sb * cb, ta)


def scale(w, a, k):
    ca, ta = as_sum(a)
    return mk_sum(w, ca * k, {h: [x, v * k] for h, (x, v) in ta.items()})


def neg(w, a):
    return scale(w, a, -1)


def bnot(w, a):
    return add(w, neg(w, a), C(w, -1))


def mul(w, a, b):
    if is_c(b):
        return scale(w, a, b[2])
    if is_c(a):
        return scale(w, b, a[2])
    x, y = sorted((a, b), key=key)
    return ("mul", w, x, y)


def known_zero_above(e):
    """Number of low bits that can be non-zero (None = unknown)."""
    if e[0] == "c":
        return e[2].bit_length()
    if e[0] == "zx":
        return e[2][1]
    if e[0] == "bool":
        return 1
    if e[0] == "and":
        cs = [x for x in e[2] if is_c(x)]
        if cs:
            return cs[0][2].bit_length()
    return None


def logic(op, w, a, b):
    args = []
    for x in (a, b):
        if x[0] == op and x[1] == w:
            args.extend(x[2])
        else:
            args.append(x)
    cs = [x[2] for x in args if is_c(x)]
    rest = [x for x in args if not is_c(x)]
    if op == "and":
        cv = mask(w)
        for v in cs:
            cv &= v
        if cv == 0:
            return C(w, 0)
        rest = list({skey(x): x for x in rest}.values())
        if len(rest) == 1 and cv in (0xFF, 0xFFFF) and w > cv.bit_length():
            return zx(w, trunc(cv.bit_length(), rest[0]))
        if len(rest) == 1 and cv != mask(w):
            kz = known_zero_above(rest[0])
            if kz is not None and mask(kz) & cv == mask(kz):
                return rest[0]
        if cv != mask(w):
            rest.append(C(w, cv))
    elif op == "or":
        cv = 0
        for v in cs:
            cv |= v
        if cv == mask(w):
            return C(w, cv)
        rest = list({skey(x): x for x in rest}.values())
        if cv:
            rest.append(C(w, cv))
    else:  # xor: pairs cancel
        cv = 0
        for v in cs:
            cv ^= v
        cnt = {}
        for x in rest:
            cnt.setdefault(skey(x), [x, 0])[1] += 1
        rest = [x for x, n in cnt.values() if n % 2]
        if cv == mask(w):
            if not rest:
                return C(w, cv)
            return bnot(w, logic("xor", w, rest[0], rest[1]) if len(rest) == 2 else
                        (rest[0] if len(rest) == 1 else ("xor", w, tuple(sorted(rest, key=key)))))
        if cv:
            rest.append(C(w, cv))
    if not rest:
        return C(w, 0 if op != "and" else mask(w))
    if len(rest) == 1:
        return rest[0]
    return (op, w, tuple(sorted(rest, key=key)))


def trunc(w, e):
    """Low w bits of e (w <= width(e))."""
    ew = e[1]
    if w == ew:
        return e
    op = e[0]
    if op == "c":
        return C(w, e[2])
    if op == "sum":
        return mk_sum(w, e[2], _merge_tr(w, e))
    if op in ("and", "or", "xor"):
        r = trunc(w, e[2][0])
        for x in e[2][1:]:
            r = logic(op, w, r, trunc(w, x))
        return r
    if op == "mul":
        return mul(w, trunc(w, e[2]), trunc(w, e[3]))
    if op in ("zx", "sx"):
        iw = e[2][1]
        if iw == w:
            return e[2]
        if iw > w:
            return trunc(w, e[2])
        return (op, w, e[2])
    if op == "ld":
        return ld(w, e[2], e[3])
    if op == "ins":
        old, v, pos, vw = e[2], e[3], e[4], e[5]
        if pos == 0 and w <= vw:
            return trunc(w, v)
        if pos >= w:
            return trunc(w, old)
        return ("ins", w, trunc(w, old), v, pos, vw) if pos + vw <= w else ("tr", w, e)
    if op == "shl":
        return ("shl", w, trunc(w, e[2]), e[3])
    if op == "bool":
        return ("bool", w, e[2])
    if op == "cat" and w <= ew // 2:
        return trunc(w, e[2])
    return ("tr", w, e)


def _merge_tr(w, e):
    t = {}
    for c, x in e[3]:
        tx = trunc(w, x)
        t.setdefault(skey(tx), [tx, 0])[1] += c
    return t


def zx(w, e):
    if e[1] == w:
        return e
    if is_c(e):
        return C(w, e[2])
    if e[0] == "zx":
        return ("zx", w, e[2])
    return ("zx", w, e)


def sx(w, e):
    if e[1] == w:
        return e
    if is_c(e):
        return C(w, sgn(e[2], e[1]))
    if e[0] == "sx":
        return ("sx", w, e[2])
    if e[0] == "zx" and e[2][1] < e[1]:
        return ("zx", w, e[2])
    return ("sx", w, e)


def shift(op, w, x, k):
    """shl/shr/sar by expression k (already masked to 0..31 if variable)."""
    if is_c(k):
        n = k[2]
        if n == 0:
            return x
        if op == "shl":
            return scale(w, x, 1 << n) if n < w else C(w, 0)
        if is_c(x):
            v = x[2] >> n if op == "shr" else sgn(x[2], w) >> n
            return C(w, v)
        if x[0] == "cat" and n == w // 2:
            return (sx if op == "sar" else zx)(w, x[3])
        if x[0] == "ld" and w == 32 and n in (16, 24):      # -5 short load: mov r,[a-2]; sar r,16
            nw = 32 - n
            a = add(32, x[2], C(32, n // 8))
            return (sx if op == "sar" else zx)(32, ld(nw, a, x[3]))
        if x[0] in ("zx", "sx") and op == "shr" and x[0] == "zx" and n >= x[2][1]:
            return C(w, 0)
        return (op, w, x, k)
    return (op, w, x, k)


def mk_ins(old, v, pos, w):
    """Insert w-bit v at bit pos of 32-bit old; nested inserts kept disjoint and ordered by position."""
    if old[0] == "ins":
        opos, ow = old[4], old[5]
        if pos <= opos and opos + ow <= pos + w:          # new insert covers the old one
            return mk_ins(old[2], v, pos, w)
        if opos + ow <= pos or pos + w <= opos:           # disjoint: order by position
            if opos > pos:
                return ("ins", 32, mk_ins(old[2], v, pos, w), old[3], opos, ow)
    return ("ins", 32, old, v, pos, w)


def extract(v, pos, w):
    """Bits [pos, pos+w) of a 32-bit register value."""
    while True:
        if v[0] == "ins":
            ipos, iw = v[4], v[5]
            if ipos == pos and iw == w:
                return v[3]
            if pos >= ipos + iw or pos + w <= ipos:
                v = v[2]
                continue
            if ipos == pos and iw > w:
                return trunc(w, v[3])
        if v[0] == "zx" and pos >= v[2][1]:
            return C(w, 0)
        break
    if pos:
        v = shift("shr", 32, v, C(32, pos))
    return trunc(w, v) if w < v[1] else v


def cat(w, lo, hi):
    """Little-endian concatenation of two w/2-bit halves."""
    if is_c(lo) and is_c(hi):
        return C(w, lo[2] | (hi[2] << (w // 2)))
    return ("cat", w, lo, hi)


# predicates: ("eq"|"lt_s"|"lt_u", w, a, b), ("not", p), ("true",), ("false",)
def pnot(p):
    if p[0] == "not":
        return p[1]
    if p[0] == "true":
        return ("false",)
    if p[0] == "false":
        return ("true",)
    return ("not", p)


def _narrow(kind, w, a, b):
    """Compare of extended values at their original width (exact for these cases)."""
    def ext_of(x):
        if x[0] in ("zx", "sx"):
            return x[0], x[2][1], x[2]
        return None
    ea, eb = ext_of(a), ext_of(b)
    for _ in range(2):
        if ea and eb and ea[1] == eb[1] and ea[0] == eb[0]:
            nk = kind
            if kind == "lt_s" and ea[0] == "zx":
                nk = "lt_u"
            return nk, ea[1], ea[2], eb[2]
        if ea and is_c(b):
            n = ea[1]
            v = b[2]
            if ea[0] == "zx" and v < (1 << n) and kind in ("eq", "lt_u"):
                return kind, n, ea[2], C(n, v)
            if ea[0] == "zx" and kind == "lt_s" and v < (1 << n):
                return "lt_u", n, ea[2], C(n, v)
            sv = sgn(v, w)
            if ea[0] == "sx" and -(1 << (n - 1)) <= sv < (1 << (n - 1)):
                return kind, n, ea[2], C(n, sv)
        if eb and is_c(a):
            n = eb[1]
            v = a[2]
            if eb[0] == "zx" and v < (1 << n) and kind in ("eq", "lt_u"):
                return kind, n, C(n, v), eb[2]
            if eb[0] == "zx" and kind == "lt_s" and v < (1 << n):
                return "lt_u", n, C(n, v), eb[2]
            sv = sgn(v, w)
            if eb[0] == "sx" and -(1 << (n - 1)) <= sv < (1 << (n - 1)):
                return kind, n, C(n, sv), eb[2]
        break
    return None


def pred(kind, w, a, b):
    for _ in range(4):
        r = _narrow(kind, w, a, b)
        if not r:
            break
        kind, w, a, b = r
    if kind == "eq":
        d = add(w, a, b, -1)
        if is_c(d):
            return ("true",) if d[2] == 0 else ("false",)
        # eq(bool p, 0) == not p
        if d[0] == "bool":
            return pnot(d[2])
        cd, td = as_sum(d)
        if cd and td:     # x + c == 0  ->  x == -c  (keep the constant on the right)
            return ("eq", w, mk_sum(w, 0, td), C(w, -cd))
        return ("eq", w, d, C(w, 0))
    signed = kind == "lt_s"
    lo = -(1 << (w - 1)) if signed else 0
    hi = (1 << (w - 1)) - 1 if signed else mask(w)
    val = (lambda x: sgn(x[2], w)) if signed else (lambda x: x[2])
    if is_c(a) and is_c(b):
        return ("true",) if val(a) < val(b) else ("false",)
    if is_c(a):     # c < x  ==  not (x < c+1)
        v = val(a)
        if v == hi:
            return ("false",)
        return pnot(pred(kind, w, b, C(w, v + 1)))
    if is_c(b):
        v = val(b)
        if v == lo:
            return ("false",)
        if not signed and v == 1:
            return pred("eq", w, a, C(w, 0))
    return (kind, w, a, b)


def mk_bool(w, p):
    if p[0] == "true":
        return C(w, 1)
    if p[0] == "false":
        return C(w, 0)
    return ("bool", w, p)


# memory: state = (barrier, stores) ; store = ("st", w, addr, val)
def addr_parts(a):
    c, t = as_sum(a)
    return c, frozenset((h, v) for h, (x, v) in t.items())


def distinct(a1, w1, a2, w2):
    c1, t1 = addr_parts(a1)
    c2, t2 = addr_parts(a2)
    if t1 != t2:
        return False
    d = (c2 - c1) & M32
    d = d - (1 << 32) if d >> 31 else d
    return d >= w1 // 8 or -d >= w2 // 8


def ld(w, a, mem):
    barrier, stores = mem
    alias = []
    for s in reversed(stores):
        if s[0] == "st" and distinct(s[2], s[1], a, w):
            continue
        alias.append(s)
    if alias and alias[0][0] == "st" and alias[0][2] == a and alias[0][1] >= w:
        return trunc(w, alias[0][3])                      # store-to-load forwarding (same address)
    return ("ld", w, a, (barrier, canon_stores(alias[::-1])))


def canon_stores(alias):
    """Canonical order among provably independent stores (program order kept for possibly aliasing ones)."""
    alias = list(alias)
    for _ in range(len(alias)):
        moved = False
        for j in range(len(alias) - 1):
            x, y = alias[j], alias[j + 1]
            if x[0] == "st" and y[0] == "st" and key(x) > key(y) and distinct(x[2], x[1], y[2], y[1]):
                alias[j], alias[j + 1] = y, x
                moved = True
        if not moved:
            break
    return tuple(alias)



# ----------------------------------------------------------------------------------------------- sides
class Side:
    """One function's code with symbolic references. Coordinates are side-local (oracle obj1 offset or candidate
    segment offset)."""
    tag = "?"

    def mem(self, addr, n):
        return bytes(self.data[addr:addr + n])

    def decode(self, addr):
        for i in MD.disasm(self.mem(addr, 16), addr):
            return i
        return None

    def jump_target(self, ins):
        if ins.size >= 5:
            return self.rel_target(ins)
        t = ins.operands[0].imm & M32
        return ("code", t) if self.lo <= t < self.hi else self.outside(t)


def lit_key(b):
    z = b.find(b"\0")
    if 0 <= z <= 64 and all(32 <= x < 127 or x in (9, 10, 13) for x in b[:z]):
        return "str:" + b[:z].decode("latin1")
    return "raw:" + b[:8].hex()


class OracleSide(Side):
    tag = "DOS"

    def __init__(self, orc, entry, hint):
        self.orc = orc
        self.img = orc.img
        self.data = self.img.obj[1]
        self.lo = entry
        self.hi = orc.extent(entry)
        self.hi = min(self.hi, entry + max(hint * 3, 4096))
        self.entry = entry

    def ref(self, site):
        fx = self.img.fixups.get((1, site))
        if not fx:
            return None
        _, tobj, toff = fx
        toff &= M32
        if tobj == 2 and census.LIT_LO <= toff < census.LIT_HI:
            return ("lit", lit_key(bytes(self.img.obj[2][toff:toff + 72])), bytes(self.img.obj[2][toff:toff + 72]))
        if tobj == 1 and self.lo <= toff < self.hi:
            return ("code", toff)
        return ("abs", tobj, toff)

    def table_addr(self, site):
        fx = self.img.fixups.get((1, site))
        return fx[2] & M32 if fx and fx[1] == 1 else None

    def rel_target(self, ins):
        t = ins.operands[0].imm & M32
        if self.lo <= t < self.hi:
            return ("code", t)
        return ("abs", 1, t)

    def outside(self, t):
        return ("abs", 1, t)


class CandSide(Side):
    tag = "C"

    def __init__(self, orc, mod, r, segbases):
        self.orc = orc
        self.mod = mod
        self.seg = next(s for s in mod.segments if s and s.name == r["segment"])
        self.si = mod.segments.index(self.seg)
        self.data = self.seg.data
        self.lo = r["seg_offset"]
        self.hi = self.lo + r["size"]
        self.entry = self.lo
        self.segbases = segbases
        self.fx = {}
        for f in mod.fixups:
            if f.seg == self.si:
                self.fx[f.off] = f

    def _code(self, off):
        if self.lo <= off < self.hi:
            return ("code", off)
        for n, s, o, _ in self.mod.publics:
            if (s, o) == (self.si, off):
                a, _ = self.orc.binds.lookup(n)
                return ("abs", a[0], a[1]) if a else ("name", match.demangle(n))
        base = self.text_base()
        if base is not None:            # label inside another function of this TU (placement of its bound publics)
            return ("abs", 1, base + off)
        return ("name", f"{self.seg.name}+{off:x}")

    def outside(self, t):
        return self._code(t)

    def text_base(self):
        if getattr(self, "fbase", None) is not None:      # this function's own placement (as tools/match.py)
            return self.fbase
        votes = set()
        for n, s, o, _ in self.mod.publics:
            if s == self.si:
                a, _ = self.orc.binds.lookup(n)
                if a and a[0] == 1:
                    votes.add(a[1] - o)
        return next(iter(votes)) if len(votes) == 1 else None

    def ref(self, site):
        f = self.fx.get(site)
        if not f:
            return None
        tname = self.mod.target_name(f)
        field = struct.unpack_from("<i", self.seg.data, f.off)[0] if f.size() == 4 else 0
        if f.self_rel:
            a, _ = self.orc.binds.lookup(tname)
            if a:
                return ("abs", a[0], (a[1] + f.disp) & M32)
            for n, s, o, _ in self.mod.publics:
                if n == tname:
                    return self._code(o + f.disp) if s == self.si else ("name", match.demangle(n))
            return ("name", match.demangle(tname))
        if (f.target[0] & 3) == 0:
            tseg = self.mod.segments[f.target[1]]
            off = (field + f.disp) & M32
            if tseg.cls.upper() == "CODE":
                return self._code(off)
            if tseg.name.upper().startswith("CONST"):
                b = bytes(tseg.data[off:off + 72])
                return ("lit", lit_key(b), b)
            base = self.segbases.get(tseg.name)
            if base:
                return ("abs", base[0], (base[1] + off) & M32)
            return ("segsym", tseg.name, off)
        a, _ = self.orc.binds.lookup(tname)
        if a:
            return ("abs", a[0], (a[1] + f.disp + field) & M32)
        # extern/public of this object without binding
        return ("name", match.demangle(tname), (f.disp + field) & M32)

    def table_addr(self, site):
        f = self.fx.get(site)
        if not f or f.self_rel or (f.target[0] & 3) != 0 or self.mod.segments[f.target[1]] is not self.seg:
            return None
        return (struct.unpack_from("<i", self.seg.data, f.off)[0] + f.disp) & M32

    def rel_target(self, ins):
        site = ins.address + ins.size - 4
        r = self.ref(site)
        if r:
            return r
        t = (ins.operands[0].imm) & M32
        return self._code(t)


def ref_expr(r):
    """Symbolic value of a relocated field."""
    k = r[0]
    if k == "abs":
        return add(32, ("seg", 32, r[1]), C(32, r[2]))
    if k == "lit":
        return ("lit", 32, r[1])
    if k == "code":
        return ("code", 32, r[1])
    if k == "segsym":
        return add(32, ("segsym", 32, r[1]), C(32, r[2]))
    if k == "name":
        return add(32, ("name", 32, r[1]), C(32, r[2] if len(r) > 2 else 0))
    raise Unsupported(k)


def target_id(r):
    if r[0] == "abs":
        return f"{r[1]}:{r[2]:x}"
    if r[0] == "name":
        return "name:" + r[1]
    return str(r)


# ----------------------------------------------------------------------------------------------- CFG
JCC = {"je", "jne", "jl", "jle", "jg", "jge", "jb", "jbe", "ja", "jae", "js", "jns", "jo", "jno", "jp", "jnp"}


class Block:
    def __init__(self, bid):
        self.id = bid
        self.ins = []
        self.term = None        # ("jcc", cc, taken, fall) ("jmp", t) ("ret",) ("fall", t) ("switch", idx_ins, [t]) ("tail", ins) ("stop", why)
        self.succ = []
        self.pred = []


def table_targets(side, tbl, lo, hi, count=None):
    """Switch-table entries: int (own code) or ('out', ref) for entries leaving the function. Without a bound
    from a preceding CMP only entries into the function are accepted."""
    out = []
    for k in range(count if count else 256):
        r = side.ref(tbl + 4 * k)
        if r and r[0] == "code":
            out.append(r[1])
        elif r and count and r[0] == "abs" and r[1] == 1:
            out.append(("out", r))
        else:
            break
    return out


def shared_ret(side, t):
    """A jump out of the function onto another function's bare RET is a return (cross-jumped epilogue)."""
    if t[0] != "abs" or t[1] != 1:
        return None
    if isinstance(side, OracleSide):
        i = side.decode(t[2])
    else:
        base = side.text_base()
        i = side.decode(t[2] - base) if base is not None else None
    return i if i is not None and i.mnemonic == "ret" else None


def build_cfg(side):
    lo, hi = side.lo, side.hi
    insmap = {}
    work = [side.entry]
    leaders = {side.entry}
    tables = {}
    seen = set()
    bound_at = {}
    while work:
        a = work.pop()
        last_cmp = None
        cur_bound = bound_at.get(a)
        while a not in seen:
            if not (lo <= a < hi):
                break
            seen.add(a)
            i = side.decode(a)
            if i is None:
                break
            insmap[a] = i
            m = i.mnemonic
            nxt = a + i.size
            if m == "cmp" and len(i.operands) == 2 and i.operands[1].type == X.X86_OP_IMM:
                last_cmp = i.operands[1].imm
            elif m not in JCC:
                last_cmp = None
            if m in JCC and last_cmp is not None:
                bound_at[nxt] = last_cmp
            if m in JCC or m == "jmp" or m == "jecxz":
                op = i.operands[0]
                if op.type == X.X86_OP_IMM:
                    t = side.jump_target(i)
                    if t[0] == "code":
                        leaders.add(t[1])
                        work.append(t[1])
                    if m == "jmp":
                        leaders.add(nxt)
                        break
                    leaders.add(nxt)
                    work.append(nxt)
                    break
                if m == "jmp" and op.type == X.X86_OP_MEM and op.mem.index != 0 and op.mem.scale == 4:
                    tbl = side.table_addr(a + i.disp_offset) if i.disp_offset else None
                    if tbl is not None:
                        n = cur_bound
                        ts = table_targets(side, tbl, lo, hi, n + 1 if n is not None and 0 <= n < 512 else None)
                        tables[a] = ts
                        for t in ts:
                            if isinstance(t, int):
                                leaders.add(t)
                                work.append(t)
                leaders.add(nxt)
                break
            if m == "ret" or m in ("hlt", "iretd", "iret"):
                leaders.add(nxt)
                break
            a = nxt
    blocks = {}
    tails = []
    order = sorted(insmap)
    cur = None
    for a in order:
        if a in leaders or cur is None or insmap.get(prev_end) is None or prev_end != a:
            cur = Block(a)
            blocks[a] = cur
        cur.ins.append(insmap[a])
        prev_end = a + insmap[a].size
    for b in blocks.values():
        last = b.ins[-1]
        m = last.mnemonic
        nxt = last.address + last.size
        if m in JCC or m == "jecxz":
            t = side.jump_target(last)
            if t[0] != "code":
                pid = ("tail", last.address)
                pb = Block(pid)
                pb.term = ("tail", t)
                ri = shared_ret(side, t)
                if ri is not None:
                    pb.ins, pb.term = [ri], ("ret",)
                tails.append(pb)
                b.term = ("jcc", m[1:], pid, nxt)
            else:
                b.term = ("jcc", m[1:], t[1], nxt)
        elif m == "jmp":
            op = last.operands[0]
            if op.type == X.X86_OP_IMM:
                t = side.jump_target(last)
                b.term = ("jmp", t[1]) if t[0] == "code" else ("tail", t)
                ri = shared_ret(side, t) if t[0] != "code" else None
                if ri is not None:
                    b.ins = b.ins[:-1] + [ri]
                    b.term = ("ret",)
            elif last.address in tables and tables[last.address]:
                ts = []
                for k, t in enumerate(tables[last.address]):
                    if not isinstance(t, int):
                        pid = ("case", last.address, k)
                        pb = Block(pid)
                        pb.term = ("tail", t[1])
                        ri = shared_ret(side, t[1])
                        if ri is not None:
                            pb.ins, pb.term = [ri], ("ret",)
                        tails.append(pb)
                        t = pid
                    ts.append(t)
                b.term = ("switch", ts)
            else:
                b.term = ("stop", "indirect jump " + last.op_str)
        elif m == "ret":
            b.term = ("ret",)
        elif m in ("hlt", "iretd", "iret"):
            b.term = ("stop", m)
        elif nxt in blocks:
            b.term = ("fall", nxt)
        else:
            b.term = ("stop", "falls off decoded range")
    for pb in tails:
        blocks[pb.id] = pb
    for b in blocks.values():
        t = b.term
        b.succ = ([t[2], t[3]] if t[0] == "jcc" else [t[1]] if t[0] in ("jmp", "fall") else
                  list(t[1]) if t[0] == "switch" else [])
        b.succ = [s for s in b.succ if s in blocks] if all(s in blocks for s in b.succ) else b.succ
    return normalise_cfg(blocks, side.entry)


EPI = {"pop", "ret", "leave"}


def is_epilogue(b):
    for i in b.ins:
        m = i.mnemonic
        if m in EPI:
            continue
        if m in ("add", "lea", "mov") and i.op_str.startswith("esp"):
            continue
        return False
    return b.term[0] == "ret"


def normalise_cfg(blocks, entry):
    """Tail-duplicate pure epilogues per predecessor and merge straight-line chains (both sides alike)."""
    preds = collections.defaultdict(list)
    for b in blocks.values():
        for s in b.succ:
            preds[s].append(b.id)
    # tail duplication of epilogues
    for eid in [k for k, b in blocks.items() if is_epilogue(b) and len(preds[k]) > 1 and k != entry]:
        e = blocks[eid]
        for p in preds[eid]:
            pb = blocks[p]
            nid = (eid, p)
            nb = Block(nid)
            nb.ins = list(e.ins)
            nb.term = e.term
            nb.succ = []
            blocks[nid] = nb
            pb.succ = [nid if s == eid else s for s in pb.succ]
            t = pb.term
            if t[0] == "jcc":
                pb.term = ("jcc", t[1], nid if t[2] == eid else t[2], nid if t[3] == eid else t[3])
            elif t[0] in ("jmp", "fall"):
                pb.term = (t[0], nid)
            elif t[0] == "switch":
                pb.term = ("switch", [nid if s == eid else s for s in t[1]])
        del blocks[eid]
    # chain merge
    changed = True
    while changed:
        changed = False
        preds = collections.defaultdict(list)
        for b in blocks.values():
            for s in b.succ:
                preds[s].append(b.id)
        for b in list(blocks.values()):
            if b.id not in blocks or b.term[0] not in ("jmp", "fall"):
                continue
            s = b.term[1]
            if s == entry or s == b.id or s not in blocks or len(preds[s]) != 1:
                continue
            sb = blocks.pop(s)
            if b.term[0] == "jmp":
                b.ins = b.ins[:-1] + sb.ins       # drop the jmp itself
            else:
                b.ins = b.ins + sb.ins
            b.term = sb.term
            b.succ = sb.succ
            changed = True
            break
    for b in blocks.values():
        b.pred = []
    for b in blocks.values():
        for s in b.succ:
            if s in blocks:
                blocks[s].pred.append(b.id)
    return blocks


def rpo(blocks, entry):
    seen, out = set(), []

    def dfs(n):
        stack = [(n, iter(blocks[n].succ))]
        seen.add(n)
        while stack:
            node, it = stack[-1]
            for s in it:
                if s in blocks and s not in seen:
                    seen.add(s)
                    stack.append((s, iter(blocks[s].succ)))
                    break
            else:
                stack.pop()
                out.append(node)
    dfs(entry)
    return out[::-1]


# ----------------------------------------------------------------------------------------------- callee model
class CalleeModel:
    """Parameters (registers live at entry, with read width) and preserved registers of DOS callees."""

    def __init__(self, orc):
        self.orc = orc
        self.cache = {}

    depth = 0

    def __init__(self, orc):
        self.orc = orc
        self.cache = {}
        self.pres_cache = {}

    def params(self, tid):
        """Registers whose entry value reaches an observable effect of the DOS callee (lifted with this same
        model; demanded width from truncations), else a liveness fallback."""
        if tid in self.cache:
            return self.cache[tid]
        if not tid.startswith("1:") or self.depth > 3:
            return self.live_params(tid)
        self.cache[tid] = {"eax": 32, "edx": 32, "ebx": 32, "ecx": 32}      # recursion guard
        self.depth += 1
        try:
            side = OracleSide(self.orc, int(tid[2:], 16), 256)
            L = Lifter(side, build_cfg(side), self).run()
            res = demanded(L)
            self.pres_cache[tid] = preserved_regs(L, self)
        except Exception:
            res = None
        finally:
            self.depth -= 1
        if res is None:
            del self.cache[tid]
            res = self.live_params(tid)
        self.cache[tid] = res
        return res

    def live_params(self, tid, depth=0):
        key_ = ("live", tid)
        if key_ in self.cache:
            return self.cache[key_]
        default = {"eax": 32, "edx": 32, "ebx": 32, "ecx": 32}
        if not tid.startswith("1:") or depth > 2:
            return default
        self.cache[key_] = default            # recursion guard
        addr = int(tid[2:], 16)
        try:
            side = OracleSide(self.orc, addr, 256)
            blocks = build_cfg(side)
        except Exception:
            return default
        live = {b: {} for b in blocks}
        changed = True
        order = rpo(blocks, side.entry)[::-1]
        it = 0
        while changed and it < 50:
            changed = False
            it += 1
            for bid in order:
                b = blocks[bid]
                out = {}
                for s in b.succ:
                    for r, w in live.get(s, {}).items():
                        out[r] = max(out.get(r, 0), w)
                cur = dict(out)
                prologue = bid == side.entry
                for i in reversed(b.ins):
                    cur = self._transfer(i, cur, depth)
                # prologue pushes are saves, not uses: drop reads by leading pushes of the entry block
                if prologue:
                    pass
                if cur != live[bid]:
                    live[bid] = cur
                    changed = True
        entry = blocks[side.entry]
        # recompute entry with prologue pushes ignored
        out = {}
        for s in entry.succ:
            for r, w in live.get(s, {}).items():
                out[r] = max(out.get(r, 0), w)
        cur = dict(out)
        k = 0
        while k < len(entry.ins) and entry.ins[k].mnemonic == "push" and entry.ins[k].operands[0].type == X.X86_OP_REG:
            k += 1
        for i in reversed(entry.ins[k:]):
            cur = self._transfer(i, cur, depth)
        res = {r: w for r, w in cur.items() if r in REGS}
        self.cache[key_] = res
        return res

    def _transfer(self, i, live, depth):
        m = i.mnemonic
        if m == "call":
            live = dict(live)
            live.pop("eax", None)
            op = i.operands[0]
            if op.type == X.X86_OP_IMM:
                ps = self.live_params(f"1:{op.imm & M32:x}", depth + 1)
            else:
                ps = {"eax": 32, "edx": 32, "ebx": 32, "ecx": 32}
            for r, w in ps.items():
                live[r] = max(live.get(r, 0), w)
            return live
        try:
            rd, wr = i.regs_access()
        except capstone.CsError:
            return {r: 32 for r in REGS}
        live = dict(live)
        zero_idiom = m in ("xor", "sub") and len(i.operands) == 2 and i.operands[0].type == X.X86_OP_REG and \
            i.operands[1].type == X.X86_OP_REG and i.operands[0].reg == i.operands[1].reg
        for r in wr:
            n = i.reg_name(r)
            if n in PART:
                p, pos, w = PART[n]
                if w == 32 or (pos == 0 and live.get(p, 0) <= w):
                    live.pop(p, None)
        if zero_idiom:
            return live
        for r in rd:
            n = i.reg_name(r)
            if n in PART:
                p, pos, w = PART[n]
                live[p] = max(live.get(p, 0), pos + w)
        return live

    def preserved(self, tid):
        """Registers equal to their entry value at every return of the lifted DOS callee (Watcom ABI - all but
        EAX - when the callee cannot be lifted or is being lifted)."""
        if not tid.startswith("1:"):
            return set(REGS) - {"eax"}
        if tid not in self.pres_cache and tid not in self.cache:
            self.params(tid)
        return self.pres_cache.get(tid, set(REGS) - {"eax"})

    def pops(self, tid):
        if not tid.startswith("1:"):
            return 0
        return self.orc.call_effects({0: tid})[0][0]


def preserved_regs(L, model):
    pres = {"ebx", "ecx", "edx", "esi", "edi", "ebp"}
    rets = 0
    for bid, (_, ex, effects, term) in L.out.items():
        if term[0] != "ret":
            continue
        rets += 1
        tail = term[1][2][2] if len(term) > 4 and term[4] == "tail" else None
        for r in list(pres):
            if ex.r.get(r) != ("in", 32, r) and not (tail and ex.r.get(r) == ("clob", 32, r, term[1][2])):
                pres.discard(r)
        if tail:
            pres &= model.preserved(tail) if tail != "self" else pres
    return pres if rets else set(REGS) - {"eax"}


def demanded(L):
    """{reg: width} of entry registers reaching effects, branch predicates or a (non-identity) return value."""
    dem = {}
    seen = set()
    stack = []

    def push(e, w):
        if isinstance(e, tuple) and e:
            if isinstance(e[0], str) and len(e) > 1 and isinstance(e[1], int) and 0 < w < e[1] and                     e[0] not in ("phi", "in", "tr"):
                t = trunc(w, e)         # only the demanded low bits matter (when truncation simplifies)
                if t[0] != "tr":
                    e = t
            stack.append((e, w))

    for bid, (_, ex, effects, term) in L.out.items():
        for e in effects:
            push(e, 32)
        if term[0] == "br":
            push(term[1], 32)
        elif term[0] == "switch":
            push(term[1], 32)
        elif term[0] == "ret" and term[1] != ("in", 32, "eax"):
            push(term[1], 32)
    for v in L.opaque_uses:
        push(v, 32)
    while stack:
        e, w = stack.pop()
        k = (id(e), w)
        if k in seen:
            continue
        seen.add(k)
        op = e[0]
        if not isinstance(op, str):
            for x in e:
                push(x, 32)
            continue
        if op == "in":
            dem[e[2]] = max(dem.get(e[2], 0), w)
            continue
        if op == "phi":
            loc = e[3]
            for p in L.blocks[e[2]].pred:
                if p in L.out:
                    st = L.out[p][1]
                    v = st.r.get(loc[1]) if loc[0] == "r" else (st.stk.get(loc[1]) or (0, None))[1]
                    if v:
                        push(v, min(w, v[1]))
            continue
        if op == "tr":
            x = e[2]
            if x[0] in ("phi", "in"):
                push(x, e[1])
            else:
                push(x, x[1] if isinstance(x[1], int) else 32)
            continue
        for x in e[1:]:
            if isinstance(x, tuple):
                push(x, x[1] if len(x) > 1 and isinstance(x[1], int) and x[0] not in ("in",) else 32)
    return {r: (8 if w <= 8 else 16 if w <= 16 else 32) for r, w in dem.items() if r in REGS}


# ----------------------------------------------------------------------------------------------- lifter
class State:
    def __init__(self):
        self.r = {}
        self.stk = {}       # offset -> (w, expr)
        self.flags = None
        self.mem = (("Mentry",), ())
        self.io = ("IOentry",)

    def copy(self):
        s = State()
        s.r = dict(self.r)
        s.stk = dict(self.stk)
        s.flags = self.flags
        s.mem = self.mem
        s.io = self.io
        return s


SP0 = ("SP0", 32)


def canon_local(a):
    """SP0 + c + rest  ->  local(c) + rest (frame offsets are renamed bijectively)."""
    c, t = as_sum(a)
    rest = {h: xv for h, xv in t.items() if xv[0] != SP0}
    return add(32, ("local", 32, sgn(c, 32)), mk_sum(32, 0, rest))


def sp_off(a):
    """Offset if a == SP0 + c exactly."""
    c, t = as_sum(a)
    if len(t) == 1:
        (x, k), = t.values()
        if x == SP0 and k == 1:
            return sgn(c, 32)
    return None


class Lifter:
    def __init__(self, side, blocks, callees):
        self.side = side
        self.blocks = blocks
        self.callees = callees
        self.out = {}           # bid -> (entry_state, exit_state, effects, term)
        self.escaped = False
        self.opaque_uses = []
        self.side_lits = _LitTable(side)
        self.esc_min = None
        self.unsupported = collections.Counter()
        self.n_ins = 0
        self.n_unk = 0
        self.notes = []

    def run(self):
        """Two passes when a stack address escapes (the second knows the escaped region from the start), then
        re-lift while loop-header phis prove trivial (every incoming value is the forward value or the phi)."""
        self.esc_known = None
        self.trivial = set()
        self._run()
        if self.esc_min is not None:
            self.esc_known = self.esc_min
            self._rerun()
        for _ in range(6):
            new = self.trivial_phis() - self.trivial
            if not new:
                break
            self.trivial |= new
            self._rerun()
        return self

    def _rerun(self):
        self.n_ins = self.n_unk = 0
        self.unsupported.clear()
        self.opaque_uses = []
        self.notes = []
        self._run()

    def trivial_phis(self):
        out = set()
        for bid, fwd in self.headers.items():
            entry = self.out[bid][0]
            preds = self.blocks[bid].pred
            exits = [self.out[p][1] for p in preds if p in self.out]
            fstates = [self.out[p][1] for p in fwd]
            if not fstates:
                continue
            for r in REGS:
                phi = ("phi", 32, bid, ("r", r))
                if entry.r.get(r) != phi:
                    continue
                v = fstates[0].r.get(r)
                if all(f.r.get(r) == v for f in fstates) and all(e.r.get(r) in (v, phi) for e in exits):
                    out.add((bid, ("r", r)))
            for o, (w, val) in entry.stk.items():
                if val[0] != "phi" or val[2] != bid:
                    continue
                v = fstates[0].stk.get(o)
                if v and all(f.stk.get(o) == v for f in fstates) and                         all(e.stk.get(o) in (v, (w, val)) for e in exits):
                    out.add((bid, ("s", o)))
            if entry.mem == (("Mphi", bid), ()):
                v = fstates[0].mem
                if all(f.mem == v for f in fstates) and all(e.mem in (v, entry.mem) for e in exits):
                    out.add((bid, "mem"))
        return out

    def _run(self):
        self.headers = {}
        order = rpo(self.blocks, self.side.entry)
        done = set()
        for bid in order:
            b = self.blocks[bid]
            if bid == self.side.entry:
                st = State()
                for r in REGS:
                    st.r[r] = ("in", 32, r)
                st.r["esp"] = SP0
            else:
                ps = [p for p in b.pred if p in done]
                if len(ps) == len(b.pred) and len(ps) == 1:
                    st = self.out[ps[0]][1].copy()
                else:
                    if len(ps) != len(b.pred):
                        self.headers[bid] = ps
                    st = self.join(bid, [self.out[p][1] for p in ps], len(ps) != len(b.pred))
            entry = st.copy()
            effects = []
            term = self.lift_block(b, st, effects)
            self.out[bid] = (entry, st, effects, term)
            done.add(bid)
        # path facts: x == c along single-predecessor chains below an eq-branch's taken edge
        self.facts = {}
        for bid in order:
            b = self.blocks[bid]
            f = []
            if len(b.pred) == 1 and b.pred[0] in self.facts:
                p = b.pred[0]
                f = list(self.facts[p])
                t = self.out[p][3]
                if t[0] == "br" and t[2] == bid and t[3] != bid and t[1][0] == "eq" and is_c(t[1][3]):
                    f.append((t[1][2], t[1][3]))
            self.facts[bid] = f

    def join(self, bid, states, loop):
        st = State()
        for r in REGS + ("esp",):
            vals = [s.r.get(r) for s in states]
            if r == "esp":
                st.r[r] = vals[0] if vals and all(v == vals[0] for v in vals) else unk(32, "esp join")
                continue
            if (not loop or (bid, ("r", r)) in self.trivial) and vals and all(v == vals[0] for v in vals):
                st.r[r] = vals[0]
            else:
                st.r[r] = ("phi", 32, bid, ("r", r))
        offs = set()
        for s in states:
            offs |= set(s.stk)
        for o in offs:
            vals = [s.stk.get(o) for s in states]
            if (not loop or (bid, ("s", o)) in self.trivial) and all(v == vals[0] for v in vals):
                st.stk[o] = vals[0]
            else:
                ws = {v[0] for v in vals if v}
                w = max(ws) if ws else 32
                st.stk[o] = (w, ("phi", w, bid, ("s", o)))
        fl = [s.flags for s in states]
        st.flags = fl[0] if not loop and fl and all(f == fl[0] for f in fl) else ("phi", bid)
        ms = [s.mem for s in states]
        st.mem = ms[0] if (not loop or (bid, "mem") in self.trivial) and ms and all(m == ms[0] for m in ms)             else (("Mphi", bid), ())
        io = [s.io for s in states]
        st.io = io[0] if not loop and io and all(x == io[0] for x in io) else ("Mphi", bid)
        return st

    # --- operand helpers
    def rd_reg(self, st, name):
        if name in ("ds", "es", "ss", "cs", "fs", "gs"):
            return ("sreg", 16, name)          # flat-model selectors: same run-time constant on both sides
        if name not in PART:
            raise Unsupported("register " + name)
        p, pos, w = PART[name]
        return extract(st.r[p], pos, w)

    def wr_reg(self, st, name, v):
        if name not in PART:
            raise Unsupported("register " + name)
        p, pos, w = PART[name]
        if w == 32:
            st.r[p] = v
            return
        old = st.r[p]
        kz = known_zero_above(old)
        if pos == 0 and kz is not None and kz <= w:
            st.r[p] = zx(32, v)
        else:
            st.r[p] = mk_ins(old, v, pos, w)

    def addr(self, st, i, op):
        m = op.mem
        if m.segment != 0:
            raise Unsupported("segment override")
        a = C(32, 0)
        if m.base:
            a = add(32, a, st.r[PART[i.reg_name(m.base)][0]] if i.reg_name(m.base) in ("esp", "ebp", "eax", "ebx", "ecx", "edx", "esi", "edi") else self._badreg(i))
        if m.index:
            a = add(32, a, scale(32, st.r[PART[i.reg_name(m.index)][0]], m.scale))
        r = self.side.ref(i.address + i.disp_offset) if i.disp_offset and i.disp_size == 4 else None
        if r:
            a = add(32, a, ref_expr(r))
        else:
            a = add(32, a, C(32, m.disp))
        return a

    def _badreg(self, i):
        raise Unsupported("16-bit address register")

    def load(self, st, a, w):
        o = sp_off(a)
        if o is not None:
            v = st.stk.get(o)
            if v and v[0] == w:
                return v[1]
            if v and v[0] > w:
                return trunc(w, v[1])
            # partial / unknown slot
            for so, (sw, sv) in st.stk.items():
                if so < o + w // 8 and o < so + sw // 8:
                    if so <= o and o + w // 8 <= so + sw // 8:
                        return trunc(w, shift("shr", sw, sv, C(sw, (o - so) * 8))) if o != so else trunc(w, sv)
                    if w > 8:      # straddles slots: assemble from halves
                        h = w // 2
                        return cat(w, self.load(st, a, h), self.load(st, add(32, a, C(32, h // 8)), h))
                    return unk(w, "partial stack slot")
            if w == 32 and o % 4 == 2:          # -5 short-load idiom on the stack: assemble halves
                return cat(32, self.load(st, a, 16), self.load(st, add(32, a, C(32, 2)), 16))
            if o >= 4:
                return ("argstk", w, o)
            if o == 0:
                return unk(w, "return address")
            lo = self.esc_known if self.esc_known is not None else self.esc_min
            if lo is not None and o >= lo:
                return ld(w, ("local", 32, o), st.mem)
            return unk(w, f"uninitialised stack {o}")
        if self._stack_based(a):
            self.escaped = True
            self.esc_min = -1 << 20
            a = canon_local(a)
        lit = self._lit(a)
        if lit is not None:
            data, off = lit
            if off + w // 8 <= len(data):
                return C(w, int.from_bytes(data[off:off + w // 8], "little"))
        return ld(w, a, st.mem)

    def _lit(self, a):
        c, t = as_sum(a)
        if len(t) == 1:
            (x, k), = t.values()
            if x[0] == "lit" and k == 1:
                return self.side_lits.get(x[2]), c
        return None

    def _stack_based(self, a):
        c, t = as_sum(a)
        return any(x == SP0 for x, _ in t.values())

    def store(self, st, a, w, v, effects):
        o = sp_off(a)
        if o is not None:
            for so in list(st.stk):
                sw = st.stk[so][0]
                if so < o + w // 8 and o < so + sw // 8 and not (so == o):
                    del st.stk[so]
            st.stk[o] = (w, v)
            if self.esc_known is not None and self.esc_known <= o < 0:     # escaped local: also memory
                b, ss = st.mem
                st.mem = (b, ss + (("st", w, ("local", 32, o), self._escape_check(v)),))
            return
        if self._stack_based(a):
            self.escaped = True
            self.esc_min = -1 << 20
            a = canon_local(a)
        v = self._escape_check(v)
        if v[0] == "ld" and v[1] == w and v[2] == a and ld(w, a, st.mem) == v:
            self.notes.append("silent store dropped (writes back the value just read; no intervening aliasing store)")
            return
        s = ("st", w, a, v)
        effects.append(s)
        b, ss = st.mem
        st.mem = (b, ss + (s,))
        self.invalidate(st)

    def invalidate(self, st):
        """Tracked values of escaped locals may have been written through a pointer or by a callee."""
        lo = self.esc_known if self.esc_known is not None else self.esc_min
        if lo is None:
            return
        for o in [o for o in st.stk if lo <= o < 0]:
            w = st.stk[o][0]
            st.stk[o] = (w, ld(w, ("local", 32, o), st.mem))

    def _escape_check(self, v):
        """Escaping stack address -> renamable local atom (frame layouts differ between sides)."""
        o = sp_off(v)
        if o is not None:
            self.escaped = True
            self.esc_min = o if self.esc_min is None else min(self.esc_min, o)
            return ("local", 32, o)
        if v[0] == "sum" and self._stack_based(v):
            self.escaped = True
            self.esc_min = -1 << 20
            return canon_local(v)
        return v

    def opval(self, st, i, op, w=None):
        if op.type == X.X86_OP_REG:
            return self.rd_reg(st, i.reg_name(op.reg))
        if op.type == X.X86_OP_IMM:
            size = op.size * 8
            if i.imm_offset and i.imm_size == 4:
                r = self.side.ref(i.address + i.imm_offset)
                if r:
                    return ref_expr(r)
            return C(size, op.imm)
        if op.type == X.X86_OP_MEM:
            return self.load(st, self.addr(st, i, op), op.size * 8)
        raise Unsupported("operand")

    def setop(self, st, i, op, v, effects):
        if op.type == X.X86_OP_REG:
            self.wr_reg(st, i.reg_name(op.reg), v)
        elif op.type == X.X86_OP_MEM:
            self.store(st, self.addr(st, i, op), op.size * 8, v, effects)
        else:
            raise Unsupported("dest")

    def push(self, st, v):
        st.r["esp"] = add(32, st.r["esp"], C(32, -4))
        o = sp_off(st.r["esp"])
        if o is None:
            raise Unsupported("esp unknown")
        st.stk[o] = (32, v)

    def pop(self, st):
        v = self.load(st, st.r["esp"], 32)
        st.r["esp"] = add(32, st.r["esp"], C(32, 4))
        return v

    def cond(self, cc, F):
        if F is None or F[0] == "phi":
            return None
        k = F[0]
        if k == "sub":
            _, w, a, b, nocf = F
            tbl = {"e": lambda: pred("eq", w, a, b), "l": lambda: pred("lt_s", w, a, b),
                   "g": lambda: pred("lt_s", w, b, a), "s": lambda: pred("lt_s", w, add(w, a, b, -1), C(w, 0))}
            if not nocf:
                tbl.update({"b": lambda: pred("lt_u", w, a, b), "a": lambda: pred("lt_u", w, b, a)})
            neg_of = {"ne": "e", "ge": "l", "le": "g", "ns": "s", "ae": "b", "be": "a"}
        elif k in ("logic", "res"):
            w, r = F[1], F[2]
            tbl = {"e": lambda: pred("eq", w, r, C(w, 0)), "s": lambda: pred("lt_s", w, r, C(w, 0))}
            if k == "logic":
                tbl.update({"l": lambda: pred("lt_s", w, r, C(w, 0)), "g": lambda: pred("lt_s", w, C(w, 0), r),
                            "b": lambda: ("false",), "a": lambda: pnot(pred("eq", w, r, C(w, 0)))})
            neg_of = {"ne": "e", "ns": "s", "ge": "l", "le": "g", "ae": "b", "be": "a"}
        elif k == "inc":
            _, w, a, r = F
            tbl = {"e": lambda: pred("eq", w, r, C(w, 0)), "s": lambda: pred("lt_s", w, r, C(w, 0)),
                   "l": lambda: pred("lt_s", w, a, C(w, -1)), "g": lambda: pred("lt_s", w, C(w, -1), a)}
            neg_of = {"ne": "e", "ns": "s", "ge": "l", "le": "g"}
        else:
            return None
        if cc in ("z", "nz"):
            cc = {"z": "e", "nz": "ne"}[cc]
        cc = {"nae": "b", "c": "b", "nb": "ae", "nc": "ae", "na": "be", "nbe": "a", "nge": "l", "nl": "ge",
              "ng": "le", "nle": "g"}.get(cc, cc)
        if cc in tbl:
            return tbl[cc]()
        if cc in neg_of and neg_of[cc] in tbl:
            return pnot(tbl[neg_of[cc]]())
        return None

    def cf_val(self, F, w):
        """Carry flag as a w-bit 0/1 value."""
        if F is None or F[0] == "phi":
            return None
        if F[0] == "sub" and not F[4]:
            return zx(w, mk_bool(8, pred("lt_u", F[1], F[2], F[3])))
        if F[0] == "logic":
            return C(w, 0)
        if F[0] == "res" and len(F) > 3:
            c = F[3]
            return c if c[1] == w else (zx(w, c) if c[1] < w else trunc(w, c))
        return None

    def call_effect(self, st, tid, effects, tail=False):
        ps = self.callees.params(tid)
        args = []
        for r in ("eax", "edx", "ebx", "ecx", "esi", "edi", "ebp"):
            if r in ps:
                w = min(ps[r], 32)
                w = 8 if w <= 8 else 16 if w <= 16 else 32
                v = self._escape_check(st.r[r])
                args.append((r, trunc(w, v) if w < 32 else v))
        pops = self.callees.pops(tid)
        esp = sp_off(st.r["esp"])
        stk = []
        if pops and esp is not None:
            for k in range(0, pops, 4):
                v = st.stk.get(esp + k + (4 if tail else 0))
                stk.append(self._escape_check(v[1]) if v else unk(32, "missing stack arg"))
        node = ("call", 32, tid, tuple(args), tuple(stk), (st.mem[0], canon_stores(st.mem[1])))
        effects.append(node)
        st.mem = (node, ())
        self.invalidate(st)
        pres = self.callees.preserved(tid)
        st.r["eax"] = ("callret", 32, node)
        for r in ("ebx", "ecx", "edx", "esi", "edi", "ebp"):
            if r not in pres:
                st.r[r] = ("clob", 32, r, node)     # deterministic output of the same callee on the same inputs
        if not tail and pops and esp is not None:
            st.r["esp"] = add(32, st.r["esp"], C(32, pops))
        st.flags = None
        return node

    def lift_block(self, b, st, effects):
        try:
            for i in b.ins:
                self.n_ins += 1
                last = i is b.ins[-1]
                try:
                    r = self.lift(i, st, effects, last)
                except Unsupported as e:
                    self.n_unk += 1
                    self.unsupported[i.mnemonic] += 1
                    self.clobber_unknown(i, st, effects, str(e))
                    r = None
                if last:
                    return self.terminator(b, i, st, effects)
        except Unsupported as e:
            return ("unk", str(e))
        return self.terminator(b, None, st, effects)

    def clobber_unknown(self, i, st, effects, why):
        try:
            rd, wr = i.regs_access()
        except capstone.CsError:
            rd, wr = [], []
        for r in rd:
            n = i.reg_name(r)
            if n in PART and PART[n][0] in st.r:
                self.opaque_uses.append(st.r[PART[n][0]])
        for r in wr:
            n = i.reg_name(r)
            if n in PART:
                p = PART[n][0]
                st.r[p] = unk(32, f"{i.mnemonic} {why}")
        st.flags = None
        writes_mem = any(op.type == X.X86_OP_MEM and (op.access & capstone.CS_AC_WRITE) for op in i.operands) or \
            i.mnemonic.startswith(("f", "rep", "stos", "movs", "out", "int"))
        if writes_mem:
            node = ("opaque", 0, f"{i.mnemonic}", next(_uid))
            effects.append(node)
            st.mem = (node, ())

    def terminator(self, b, i, st, effects):
        t = b.term
        if t[0] == "jcc":
            p = self.cond(t[1], st.flags)
            if p is None:
                return ("br", ("unknown-cond", t[1]), t[2], t[3])
            if p[0] == "not":
                return ("br", p[1], t[3], t[2])
            return ("br", p, t[2], t[3])
        if t[0] in ("jmp", "fall"):
            return ("jmp", t[1])
        if t[0] == "ret":
            esp = sp_off(st.r["esp"])
            n = i.operands[0].imm if i is not None and i.operands else 0
            return ("ret", st.r["eax"], n, esp)
        if t[0] == "switch":
            op = i.operands[0]
            idx = st.r[PART[i.reg_name(op.mem.index)][0]]
            return ("switch", idx, t[1])
        if t[0] == "tail":
            node = self.call_effect(st, target_id(t[1]), effects, tail=True)
            return ("ret", ("callret", 32, node), 0, sp_off(st.r["esp"]), "tail")
        return ("unk", t[1])

    def lift(self, i, st, effects, last):
        m = i.mnemonic
        ops = i.operands
        if m in JCC or m == "jmp" or m == "ret":
            if m == "jmp" and ops[0].type == X.X86_OP_MEM:
                return None
            if m == "ret":
                st.r["esp"] = add(32, st.r["esp"], C(32, 4 + (ops[0].imm if ops else 0)))
            return None
        if i.prefix[0] in (0xF3, 0xF2) and m.startswith(("rep", "movs", "stos")):
            return self.rep(i, st, effects)
        if m in ("nop",):
            return None
        if m == "mov":
            self.setop(st, i, ops[0], self.opval(st, i, ops[1]), effects)
            return None
        if m in ("movzx", "movsx"):
            v = self.opval(st, i, ops[1])
            w = ops[0].size * 8
            self.setop(st, i, ops[0], (zx if m == "movzx" else sx)(w, v), effects)
            return None
        if m == "lea":
            a = self.addr(st, i, ops[1])
            w = ops[0].size * 8
            self.setop(st, i, ops[0], trunc(w, a) if w < 32 else a, effects)
            return None
        if m in ("pushal", "pushad"):
            sp = st.r["esp"]
            for r in ("eax", "ecx", "edx", "ebx", None, "ebp", "esi", "edi"):
                self.push(st, st.r[r] if r else sp)
            return None
        if m in ("popal", "popad"):
            for r in ("edi", "esi", "ebp", None, "ebx", "edx", "ecx", "eax"):
                v = self.pop(st)
                if r:
                    st.r[r] = v
            return None
        if m in ("push", "pop") and ops[0].type == X.X86_OP_REG and i.reg_name(ops[0].reg) not in PART:
            if m == "push":
                self.push(st, unk(32, "segment register"))
            else:
                self.pop(st)
            return None
        if m == "push":
            v = self.opval(st, i, ops[0])
            if v[1] != 32:
                v = sx(32, v) if ops[0].type == X.X86_OP_IMM else zx(32, v)
            self.push(st, v)
            return None
        if m == "pop":
            v = self.pop(st)
            self.setop(st, i, ops[0], v, effects)
            return None
        if m == "leave":
            st.r["esp"] = st.r["ebp"]
            st.r["ebp"] = self.pop(st)
            return None
        if m == "xchg":
            a, b = self.opval(st, i, ops[0]), self.opval(st, i, ops[1])
            self.setop(st, i, ops[0], b, effects)
            self.setop(st, i, ops[1], a, effects)
            return None
        if m in ("add", "sub", "and", "or", "xor", "cmp", "test", "adc", "sbb"):
            w = ops[0].size * 8
            a = self.opval(st, i, ops[0])
            bv = self.opval(st, i, ops[1])
            if bv[1] != w:
                bv = sx(w, bv) if bv[1] < w else trunc(w, bv)
            if m in ("add",):
                r = add(w, a, bv)
                st.flags = ("res", w, r, zx(w, mk_bool(8, pred("lt_u", w, r, a))))
            elif m in ("sub", "cmp"):
                r = add(w, a, bv, -1)
                st.flags = ("sub", w, a, bv, False)
            elif m in ("sbb", "adc"):
                cf = self.cf_val(st.flags, w)
                if cf is None:
                    raise Unsupported(m + " flags")
                same = ops[0].type == X.X86_OP_REG and ops[1].type == X.X86_OP_REG and ops[0].reg == ops[1].reg
                if m == "sbb":
                    r = neg(w, cf) if same else add(w, add(w, a, bv, -1), cf, -1)
                else:
                    r = add(w, add(w, a, bv), cf)
                st.flags = None
            else:
                op = "and" if m == "test" else m
                if m == "xor" and ops[0].type == X.X86_OP_REG and ops[1].type == X.X86_OP_REG and \
                        ops[0].reg == ops[1].reg:
                    r = C(w, 0)
                else:
                    r = logic(op, w, a, bv)
                st.flags = ("logic", w, r)
            if m not in ("cmp", "test"):
                self.setop(st, i, ops[0], r, effects)
            return None
        if m in ("inc", "dec"):
            w = ops[0].size * 8
            a = self.opval(st, i, ops[0])
            r = add(w, a, C(w, 1 if m == "inc" else -1))
            st.flags = ("inc", w, a, r) if m == "inc" else ("sub", w, a, C(w, 1), True)
            self.setop(st, i, ops[0], r, effects)
            return None
        if m == "neg":
            w = ops[0].size * 8
            a = self.opval(st, i, ops[0])
            r = neg(w, a)
            st.flags = ("sub", w, C(w, 0), a, False)
            self.setop(st, i, ops[0], r, effects)
            return None
        if m == "not":
            w = ops[0].size * 8
            self.setop(st, i, ops[0], bnot(w, self.opval(st, i, ops[0])), effects)
            return None
        if m in ("shl", "sal", "shr", "sar"):
            w = ops[0].size * 8
            a = self.opval(st, i, ops[0])
            if len(ops) == 1:
                k = C(32, 1)
            elif ops[1].type == X.X86_OP_IMM:
                k = C(32, ops[1].imm & 31)
            else:
                k = logic("and", 32, zx(32, self.rd_reg(st, "cl")), C(32, 31))
            r = shift("shl" if m == "sal" else m, w, a, k)
            if is_c(k) and k[2]:
                n = k[2]
                bit = (w - n) if m in ("shl", "sal") else (n - 1)
                st.flags = ("res", w, r, logic("and", w, shift("shr", w, a, C(32, bit)), C(w, 1)))
            else:
                st.flags = None
            self.setop(st, i, ops[0], r, effects)
            return None
        if m in ("rol", "ror"):
            w = ops[0].size * 8
            a = self.opval(st, i, ops[0])
            k = C(32, ops[1].imm & 31) if len(ops) > 1 and ops[1].type == X.X86_OP_IMM else \
                (C(32, 1) if len(ops) == 1 else logic("and", 32, zx(32, self.rd_reg(st, "cl")), C(32, 31)))
            st.flags = None
            self.setop(st, i, ops[0], (m, w, a, k), effects)
            return None
        if m == "imul":
            if len(ops) == 3:
                w = ops[0].size * 8
                r = mul(w, self.opval(st, i, ops[1]), C(w, ops[2].imm))
                self.setop(st, i, ops[0], r, effects)
                st.flags = None
                return None
            if len(ops) == 2:
                w = ops[0].size * 8
                r = mul(w, self.opval(st, i, ops[0]), self.opval(st, i, ops[1]))
                self.setop(st, i, ops[0], r, effects)
                st.flags = None
                return None
        if m in ("imul", "mul") and len(ops) == 1:
            w = ops[0].size * 8
            if w != 32:
                raise Unsupported("narrow mul")
            a = st.r["eax"]
            bv = self.opval(st, i, ops[0])
            st.r["eax"] = mul(32, a, bv)
            x, y = sorted((a, bv), key=key)
            st.r["edx"] = ("mulhi_" + ("s" if m == "imul" else "u"), 32, x, y)
            st.flags = None
            return None
        if m in ("idiv", "div"):
            w = ops[0].size * 8
            d = self.opval(st, i, ops[0])
            if w == 32:
                lo, hi = st.r["eax"], st.r["edx"]
                signed = m == "idiv"
                plain = (signed and hi == shift("sar", 32, lo, C(32, 31))) or (not signed and is_c(hi) and hi[2] == 0)
                if plain:
                    q = ("sdiv" if signed else "udiv", 32, lo, d)
                    rm = ("srem" if signed else "urem", 32, lo, d)
                else:
                    q = (m + "64q", 32, hi, lo, d)
                    rm = (m + "64r", 32, hi, lo, d)
                st.r["eax"], st.r["edx"] = q, rm
            elif w == 16:
                lo, hi = self.rd_reg(st, "ax"), self.rd_reg(st, "dx")
                signed = m == "idiv"
                plain = (signed and hi == shift("sar", 16, lo, C(32, 15))) or (not signed and is_c(hi) and hi[2] == 0)
                if not plain:
                    raise Unsupported("16-bit wide divide")
                self.wr_reg(st, "ax", ("sdiv" if signed else "udiv", 16, lo, d))
                self.wr_reg(st, "dx", ("srem" if signed else "urem", 16, lo, d))
            else:
                raise Unsupported("byte divide")
            st.flags = None
            return None
        if m == "cdq":
            st.r["edx"] = shift("sar", 32, st.r["eax"], C(32, 31))
            return None
        if m == "cwd":
            self.wr_reg(st, "dx", shift("sar", 16, self.rd_reg(st, "ax"), C(32, 15)))
            return None
        if m == "cwde":
            st.r["eax"] = sx(32, self.rd_reg(st, "ax"))
            return None
        if m == "cbw":
            self.wr_reg(st, "ax", sx(16, self.rd_reg(st, "al")))
            return None
        if m.startswith("set") and len(ops) == 1:
            p = self.cond(m[3:], st.flags)
            if p is None:
                raise Unsupported("setcc flags")
            self.setop(st, i, ops[0], mk_bool(8, p), effects)
            return None
        if m in ("movsd", "movsb", "movsw", "stosd", "stosb", "stosw") and not i.prefix[0]:
            w = {"d": 32, "w": 16, "b": 8}[m[-1]]
            if m.startswith("movs"):
                v = self.load(st, st.r["esi"], w)
                st.r["esi"] = add(32, st.r["esi"], C(32, w // 8))
            else:
                v = self.rd_reg(st, {32: "eax", 16: "ax", 8: "al"}[w])
            self.store(st, st.r["edi"], w, v, effects)
            st.r["edi"] = add(32, st.r["edi"], C(32, w // 8))
            return None
        if m == "call":
            op = ops[0]
            if op.type != X.X86_OP_IMM:
                raise Unsupported("indirect call")
            t = self.side.rel_target(i)
            self.call_effect(st, target_id(t), effects)
            return None
        if m in ("cld",):
            return None
        if m == "out":
            port = self.opval(st, i, ops[0])
            v = self.opval(st, i, ops[1])
            node = ("out", v[1], zx(16, port) if port[1] < 16 else port, v, st.io)
            effects.append(node)
            st.io = node
            return None
        if m == "in":
            port = self.opval(st, i, ops[1])
            w = ops[0].size * 8
            self.setop(st, i, ops[0], ("inport", w, zx(16, port) if port[1] < 16 else port, st.io), effects)
            return None
        raise Unsupported(m)

    def rep(self, i, st, effects):
        m = i.mnemonic.split()[-1]
        w = {"d": 32, "w": 16, "b": 8}[m[-1]]
        if m.startswith("movs"):
            node = ("rep", 0, "movs", w, st.r["edi"], st.r["esi"], st.r["ecx"], (st.mem[0], canon_stores(st.mem[1])))
            n = st.r["ecx"]
            st.r["esi"] = add(32, st.r["esi"], scale(32, n, w // 8))
        elif m.startswith("stos"):
            node = ("rep", 0, "stos", w, st.r["edi"], trunc(w, st.r["eax"]) if w < 32 else st.r["eax"],
                    st.r["ecx"], st.mem)
            n = st.r["ecx"]
        else:
            raise Unsupported("rep " + m)
        node = node[:4] + tuple(self._escape_check(v) for v in node[4:7]) + node[7:]
        effects.append(node)
        st.mem = (node, ())
        st.r["edi"] = add(32, st.r["edi"], scale(32, n, w // 8))
        st.r["ecx"] = C(32, 0)
        return None


# ----------------------------------------------------------------------------------------------- comparison
class Cmp:
    """Compare two lifted functions under block alignment and a bijective renaming of phi locations.

    eq() returns True / False / None (unknown: an unsupported value is involved). Bindings, obligations and
    memoised successes are trail-logged so failed alternatives are undone completely."""

    def __init__(self, la, lb, align):
        self.la, self.lb, self.align = la, lb, align
        self.inv = {v: k for k, v in align.items()}
        self.map = {}       # ("A", phi) -> B expr ; ("B", phi) -> A phi  (phi<->phi bijective)
        self.trail = []     # undo log: ("map", key) | ("memo", key)
        self.oblig = []
        self.memo = {}
        self.unknown = False
        self.why = None
        self.eager = True
        self.notes = set()
        self.loose = False      # diagnostic: compare loads by address only (memory order ignored)

    def mark(self):
        return len(self.trail), len(self.oblig)

    def undo(self, m):
        t, o = m
        while len(self.trail) > t:
            kind, k = self.trail.pop()
            (self.map if kind == "map" else self.memo).pop(k, None)
        del self.oblig[o:]

    def bind(self, x, y, w):
        self.map[("A", x, w)] = y
        self.trail.append(("map", ("A", x, w)))
        if y[0] == "phi":
            self.map[("B", y, w)] = x
            self.trail.append(("map", ("B", y, w)))

    def eq(self, a, b):
        if a is b:
            return True
        if not isinstance(a, tuple) or not isinstance(b, tuple):
            return a == b
        k = (id(a), id(b))
        if k in self.memo:
            return True
        r = self._eq(a, b)
        if r is True:
            self.memo[k] = (a, b)
            self.trail.append(("memo", k))
        return r

    def seq(self, xs, ys):
        if len(xs) != len(ys):
            return False
        res = True
        for x, y in zip(xs, ys):
            r = self.eq(x, y)
            if r is False:
                return False
            if r is None:
                res = None
        return res

    def _eq(self, a, b):
        if not a or not b:
            return a == b
        oa, ob = a[0], b[0]
        if not isinstance(oa, str) or not isinstance(ob, str):
            return self.seq(a, b)
        if oa in ("unk", "opaque") or ob in ("unk", "opaque"):
            self.unknown = True
            return None
        pa, pb = phiview(a), phiview(b)
        if pa or pb:
            if a[1] != b[1]:
                return False
            return self.eq_phi(pa or (a, None), pb or (b, None), a[1])
        if oa != ob or len(a) != len(b):
            return False
        if oa == "code":
            return self.align.get(a[2]) == b[2]
        if oa == "local":
            ka, kb = ("LA", a[2]), ("LB", b[2])
            if ka in self.map or kb in self.map:
                return self.map.get(ka) == b[2] and self.map.get(kb) == a[2]
            self.map[ka] = b[2]
            self.map[kb] = a[2]
            self.trail += [("map", ka), ("map", kb)]
            return True
        if oa == "Mphi":
            return self.align.get(a[1]) == b[1]
        if oa == "ld" and self.loose:
            return self.seq(a[1:3], b[1:3])
        if oa == "call" and self.loose:
            return self.seq(a[1:5], b[1:5])
        if oa == "rep" and self.loose:
            return self.seq(a[1:7], b[1:7])
        if oa in ("and", "or", "xor"):
            if a[1] != b[1]:
                return False
            return self.eq_multiset(list(a[2]), list(b[2]), False)
        if oa == "sum":
            if a[1] != b[1] or a[2] != b[2]:
                return False
            return self.eq_multiset(list(a[3]), list(b[3]), True)
        if oa == "mul" and a[1] == b[1]:
            m = self.mark()
            r = self.seq(a[2:], b[2:])
            if r is False:
                self.undo(m)
                return self.seq(a[2:], (b[3], b[2]))
            return r
        if oa == "eq" and a[1] == b[1]:      # x == c  <=>  -x == -c
            m = self.mark()
            r = self.seq(a[2:], b[2:])
            if r is False:
                self.undo(m)
                return self.seq(a[2:], (neg(b[1], b[2]), neg(b[1], b[3])))
            return r
        return self.seq(a[1:], b[1:])

    def eq_multiset(self, xs, ys, pair):
        if len(xs) != len(ys):
            return False
        kx = (lambda e: (key(e[1]), e[0])) if pair else key
        xs = sorted(xs, key=kx)
        ys = sorted(ys, key=kx)
        if [kx(x) for x in xs] != [kx(y) for y in ys]:
            return self.eq_unordered(xs, ys, pair)       # keys differ only through join-value bindings
        res = True
        i = 0
        while i < len(xs):
            j = i
            while j < len(xs) and kx(xs[j]) == kx(xs[i]):
                j += 1
            gx, gy = xs[i:j], ys[i:j]
            ok = False
            perms = itertools.permutations(gy) if len(gy) <= 5 else [gy]
            for p in perms:
                m = self.mark()
                rr = True
                for x, y in zip(gx, p):
                    r = (self.eq(x[1], y[1]) if x[0] == y[0] else False) if pair else self.eq(x, y)
                    if r is False:
                        rr = False
                        break
                    if r is None:
                        rr = None
                if rr is not False:
                    ok = rr
                    break
                self.undo(m)
            if ok is False:
                return False
            if ok is None:
                res = None
            i = j
        return res

    def eq_unordered(self, xs, ys, pair):
        """Bounded search for a pairing when sort keys disagree (a join value bound to a plain value)."""
        def one(x, y):
            if pair:
                return self.eq(x[1], y[1]) if x[0] == y[0] else False
            return self.eq(x, y)
        used = [False] * len(ys)
        res = True
        for x in xs:
            hit = None
            for j, y in enumerate(ys):
                if used[j]:
                    continue
                m = self.mark()
                r = one(x, y)
                if r is True:
                    hit = j
                    break
                if r is None:
                    hit, res = j, None
                    break
                self.undo(m)
            if hit is None:
                return False
            used[hit] = True
        return res

    def eq_phi(self, av, bv, w):
        """av/bv: (phi atom, bit position) views at width w, or (plain w-bit value, None)."""
        a, pa = av
        b, pb = bv
        if a[0] != "phi":
            kb = ("B", b, pb, w)
            if kb in self.map:
                return self.map[kb] is a or self.map[kb] == a
            m = self.mark()
            self.map[kb] = a
            self.trail.append(("map", kb))
            if self.eager:
                if self.check_one(av, bv, w) is False:
                    self.undo(m)
                    return False
            else:
                self.oblig.append((av, bv, w))
            return True
        ka = ("A", a, pa, w)
        if ka in self.map:
            return self.map[ka] == (b, pb)
        if b[0] == "phi":
            if ("B", b, pb, w) in self.map or self.align.get(a[2]) != b[2]:
                return False
        m = self.mark()
        self.map[ka] = (b, pb)
        self.trail.append(("map", ka))
        if b[0] == "phi":
            self.map[("B", b, pb, w)] = (a, pa)
            self.trail.append(("map", ("B", b, pb, w)))
        if self.eager:
            if self.check_one(av, bv, w) is False:
                self.undo(m)
                return False
        else:
            self.oblig.append((av, bv, w))
        return True

    def incoming(self, lifter, pbid, loc, w, pos=0):
        st = lifter.out[pbid][1]
        if loc[0] == "r":
            v = st.r.get(loc[1])
        elif loc[0] == "s":
            s = st.stk.get(loc[1])
            v = s[1] if s else None
        else:
            return None
        if v is None or v[1] < w + pos:
            return unk(w, "missing incoming")
        if pos:
            return extract(v, pos, w) if v[1] == 32 else trunc(w, shift("shr", v[1], v, C(32, pos)))
        return trunc(w, v) if v[1] > w else v

    def discharge(self):
        while self.oblig:
            if self.check_one(*self.oblig.pop()) is False:
                return False
        return True

    def check_one(self, av, bv, w):
        """Incoming values of a bound phi pair along aligned predecessor edges (at the bound width/position)."""
        a, pa = av
        b, pb = bv
        if True:
            if a[0] != "phi":         # C-side phi against a DOS value
                predsB = self.lb.blocks[b[2]].pred
                aA = self.inv.get(b[2])
                if aA is not None:
                    sel = [p for p in predsB if self.inv.get(p) in self.la.blocks[aA].pred]
                    predsB = sel or predsB
                for pB in predsB:
                    vb = self.incoming(self.lb, pB, b[3], w, pb)
                    m = self.mark()
                    r = self.eq(a, vb)
                    if r is False:
                        self.undo(m)
                        r = self.eq(a, subst(vb, self.lb.facts.get(pB)))
                    if r is False:
                        self.why = (f"join value {loc_s(b)} at C {fmt_bid(b[2])} differs on edge from {fmt_bid(pB)}: "
                                    f"DOS {show(a)} | C {show(vb)}")
                        return False
                    if r is None:
                        self.unknown = True
                return True
            blkA = a[2]
            predsA = self.la.blocks[blkA].pred
            if b[0] == "phi":
                predsB = self.lb.blocks[b[2]].pred
                if sorted(map(str, (self.align.get(p) for p in predsA))) != sorted(map(str, predsB)):
                    self.why = f"predecessors of join {fmt_bid(blkA)} not aligned"
                    return False
            for pA in predsA:
                pB = self.align.get(pA)
                if pB is None:
                    self.why = f"predecessor {fmt_bid(pA)} of join {fmt_bid(blkA)} not aligned"
                    return False
                va = self.incoming(self.la, pA, a[3], w, pa)
                vb = self.incoming(self.lb, pB, b[3], w, pb) if b[0] == "phi" else b
                m = self.mark()
                r = self.eq(va, vb)
                if r is False:
                    self.undo(m)
                    r = self.eq(subst(va, self.la.facts.get(pA)), subst(vb, self.lb.facts.get(pB)))
                if r is False:
                    self.why = (f"join value {loc_s(a)}~{loc_s(b)} at {fmt_bid(blkA)} differs on edge from "
                                f"{fmt_bid(pA)}: DOS {show(va)} | C {show(vb)}")
                    return False
                if r is None:
                    self.unknown = True
        return True


def subst(e, facts, memo=None):
    """Replace sub-expressions known equal to constants on this path."""
    if not facts or not isinstance(e, tuple) or not e:
        return e
    if memo is None:
        memo = {}
        facts = {skey(x): c for x, c in facts}
    k = id(e)
    if k in memo:
        return memo[k][1]
    h = skey(e)
    if h in facts:
        r = facts[h]
    elif isinstance(e[0], str) and e[0] == "tr" and skey(trunc(e[1], e[2])) in facts:
        r = facts[skey(trunc(e[1], e[2]))]
    else:
        parts = tuple(subst(x, facts, memo) if isinstance(x, tuple) else x for x in e)
        r = e if all(p is q for p, q in zip(parts, e)) else rebuild(parts)
    memo[k] = (e, r)
    return r


def rebuild(e):
    """Re-normalise a node after substitution."""
    op = e[0]
    if not isinstance(op, str):
        return e
    w = e[1] if len(e) > 1 else None
    if op == "sum":
        r = C(w, e[2])
        for c, t in e[3]:
            r = add(w, r, scale(w, t, c))
        return r
    if op in ("and", "or", "xor"):
        r = e[2][0]
        for x in e[2][1:]:
            r = logic(op, w, r, x)
        return r
    if op == "tr":
        return trunc(w, e[2])
    if op == "zx":
        return zx(w, e[2])
    if op == "sx":
        return sx(w, e[2])
    if op == "mul":
        return mul(w, e[2], e[3])
    if op in ("eq", "lt_s", "lt_u"):
        return pred(op, w, e[2], e[3])
    if op == "not":
        return pnot(e[1])
    if op == "bool":
        return mk_bool(w, e[2])
    if op == "ins":
        return mk_ins(e[2], e[3], e[4], e[5]) if w == 32 else e
    if op in ("shl", "shr", "sar") and len(e) == 4:
        return shift(op, w, e[2], e[3])
    return e


def phiview(e):
    """(phi, bit position) when e is phi, tr(phi) or tr(shr(phi, k))."""
    if e[0] == "phi":
        return e, 0
    if e[0] == "tr":
        x = e[2]
        if x[0] == "phi":
            return x, 0
        if x[0] == "shr" and x[2][0] == "phi" and is_c(x[3]):
            return x[2], x[3][2]
    return None


def loc_s(p):
    if p[0] != "phi":
        return show(p)
    l = p[3]
    return l[1] if l[0] == "r" else f"[sp{l[1]:+d}]"


def fmt_bid(b):
    if isinstance(b, tuple):
        return f"{fmt_bid(b[0])}<{fmt_bid(b[1])}"
    return f"{b:x}" if isinstance(b, int) else str(b)


# ----------------------------------------------------------------------------------------------- alignment
def align_cfg(la, lb):
    A, B = la.out, lb.out
    align = {}
    problems = []
    work = [(la.side.entry, lb.side.entry)]
    while work:
        a, b = work.pop(0)
        if a in align:
            if align[a] != b:
                problems.append(f"block {fmt_bid(a)} aligned to {fmt_bid(align[a])} and {fmt_bid(b)}")
            continue
        if b in align.values():
            problems.append(f"C block {fmt_bid(b)} aligned twice")
            continue
        if a not in A or b not in B:
            problems.append(f"successor {fmt_bid(a)}/{fmt_bid(b)} not decoded")
            continue
        align[a] = b
        ta, tb = A[a][3], B[b][3]
        if ta[0] != tb[0]:
            problems.append(f"terminator kind {ta[0]} vs {tb[0]} at {fmt_bid(a)}/{fmt_bid(b)}")
            continue
        if ta[0] == "br":
            work += [(ta[2], tb[2]), (ta[3], tb[3])]
        elif ta[0] == "jmp":
            work.append((ta[1], tb[1]))
        elif ta[0] == "switch":
            if len(ta[2]) != len(tb[2]):
                problems.append(f"switch table length {len(ta[2])} vs {len(tb[2])}")
                continue
            work += list(zip(ta[2], tb[2]))
    unaligned_a = [x for x in A if x not in align]
    unaligned_b = [x for x in B if x not in align.values()]
    return align, problems, unaligned_a, unaligned_b


# ----------------------------------------------------------------------------------------------- pretty
def show(e, depth=0):
    if not isinstance(e, tuple):
        return str(e)
    if depth > 6:
        return "..."
    if not e:
        return "()"
    op = e[0]
    if not isinstance(op, str):
        return "<" + ", ".join(show(x, depth + 1) for x in e) + ">"
    d = depth + 1
    if op == "c":
        v = sgn(e[2], e[1])
        return f"{v}" if -4096 < v < 4096 else f"0x{e[2]:x}"
    if op == "sum":
        parts = []
        for c, t in e[3]:
            c = sgn(c, e[1])
            parts.append(show(t, d) if c == 1 else f"-{show(t, d)}" if c == -1 else f"{c}*{show(t, d)}")
        if e[2]:
            parts.append(show(C(e[1], e[2]), d))
        return "(" + " + ".join(parts) + ")"
    if op == "seg":
        return f"obj{e[2]}"
    if op == "in":
        return f"in_{e[2]}"
    if op == "SP0":
        return "SP0"
    if op == "argstk":
        return f"arg[{e[2]}]{e[1]}"
    if op == "ld":
        return f"ld{e[1]}[{show_addr(e[2])}]"
    if op == "phi":
        l = e[3]
        return f"phi@{fmt_bid(e[2])}:{l[1] if l[0] == 'r' else 'sp%+d' % l[1]}"
    if op == "callret":
        return f"ret({e[2][2]})"
    if op == "clob":
        return f"clob_{e[2]}({show(e[3], d)})"
    if op in ("zx", "sx", "tr"):
        return f"{op}{e[1]}({show(e[2], d)})"
    if op in ("and", "or", "xor"):
        return "(" + f" {op} ".join(show(x, d) for x in e[2]) + ")"
    if op in ("eq", "lt_s", "lt_u"):
        return f"{op}{e[1]}({show(e[2], d)}, {show(e[3], d)})"
    if op == "not":
        return f"!{show(e[1], d)}"
    if op == "bool":
        return f"bool({show(e[2], d)})"
    if op == "st":
        return f"st{e[1]} [{show_addr(e[2])}] = {show(e[3], d)}"
    if op == "call":
        args = ", ".join(f"{r}={show(v, d)}" for r, v in e[3])
        sa = ", ".join(show(v, d) for v in e[4])
        return f"call {symname(e[2])}({args}{'; stack: ' + sa if sa else ''})"
    if op == "unk":
        return f"UNK({e[3]})"
    if op == "lit":
        return f"lit<{e[2][:24]}>"
    if op in REGS:
        return f"{op}={show(e[1], d)}"
    return op + (str(e[1]) if len(e) > 1 and isinstance(e[1], int) else "") + "(" +         ", ".join(show(x, d) for x in e[1:] if isinstance(x, tuple)) + ")"


_names = None


def symname(tid):
    global _names
    if _names is None:
        _names = {}
        sym = load_json(ROOT / "config" / "symbols.json")
        for orac in sym.values():
            if not isinstance(orac, dict):
                continue
            for kind in ("functions", "data"):
                for n, ent in orac.get(kind, {}).items():
                    o, a = ent["addr"].split(":")
                    _names[(int(o), int(a, 16))] = n
        _names_sorted.extend(sorted(_names))
    if tid.startswith(("1:", "2:")):
        o, a = tid.split(":")
        return _names.get((int(o), int(a, 16)), tid)
    return tid


_names_sorted = []


def show_addr(a):
    c, t = as_sum(a)
    segs = [x for x, v in t.values() if x[0] == "seg" and v == 1]
    if segs:
        symname("1:0")
        import bisect
        obj = segs[0][2]
        k = bisect.bisect_right(_names_sorted, (obj, c)) - 1
        base = _names_sorted[k] if k >= 0 else None
        rest = {h: xv for h, xv in t.items() if xv[0] != segs[0]}
        nm = f"{_names[base]}+{c - base[1]:x}" if base and base[0] == obj else f"{obj}:{c:x}"
        return nm + ("" if not rest else " + " + show(mk_sum(32, 0, rest)))
    return show(a)


# ----------------------------------------------------------------------------------------------- driver
def compare_function(oside, cside, callees, rw=32):
    void = rw == 0
    la = Lifter(oside, build_cfg(oside), callees)
    la.side_lits = {}
    lb = Lifter(cside, build_cfg(cside), callees)
    lb.side_lits = {}
    for L in (la, lb):
        # literal contents for loads from CONST literals
        L.side_lits = _LitTable(L.side)
        L.run()
    align, problems, ua, ub = align_cfg(la, lb)
    cmp = Cmp(la, lb, align)
    first = None
    nunk = 0
    blocks_eq = 0
    order = [b for b in rpo(la.blocks, oside.entry) if b in align]
    for a in order:
        b = align[a]
        _, _, ea, ta = la.out[a]
        _, _, eb, tb = lb.out[b]
        cmp.unknown = False
        m = cmp.mark()
        diff = compare_effects(cmp, ea, eb, ta, tb, void, rw)
        if diff is not None and diff[0] != "unknown" and (la.facts.get(a) or lb.facts.get(b)):
            cmp.undo(m)
            fa, fb = la.facts.get(a), lb.facts.get(b)
            sa = [subst(e, fa) for e in ea]
            sb = [subst(e, fb) for e in eb]
            ta2 = tuple(subst(x, fa) if isinstance(x, tuple) else x for x in ta)
            tb2 = tuple(subst(x, fb) if isinstance(x, tuple) else x for x in tb)
            d2 = compare_effects(cmp, sa, sb, ta2, tb2, void, rw)
            if d2 is None or d2[0] == "unknown":
                diff = d2
        if diff is None and not cmp.discharge():
            diff = ("join", cmp.why)
        if diff is None and cmp.unknown:
            nunk += 1
            diff_unknown = True
        if diff is not None and diff[0] in ("effect", "branch", "return"):
            loose = Cmp(la, lb, align)
            loose.loose = True
            if compare_effects(loose, ea, eb, ta, tb, void, rw) is None:
                diff = (diff[0] + ":memory_order", diff[1], diff[2],
                        (diff[3] or "") + " - equal if loads ignore intervening stores (alias/ordering difference)")
        if diff is not None:
            if first is None or (first[0] == "unknown" and diff[0] != "unknown"):
                first = (diff[0], a, b, diff)
            if diff[0] != "unknown":
                break
        else:
            blocks_eq += 1
    total_ins = la.n_ins + lb.n_ins
    unk_ins = la.n_unk + lb.n_unk
    assumptions = []
    if la.escaped or lb.escaped:
        assumptions.append("stack addresses escape: escaped locals modelled as memory, invalidated by every call "
                           "and pointer store; frame offsets renamed bijectively")
    if void:
        assumptions.append("return value ignored (our source declares void)")
    elif rw < 32:
        assumptions.append(f"return value compared at {rw} bits (our source's return type)")
    assumptions += sorted(set(la.notes + lb.notes))
    assumptions += sorted(cmp.notes)
    if first and first[0] != "unknown":
        verdict = "MISMATCH"
    elif first:
        verdict = "UNKNOWN"
    elif problems or ua or ub:
        verdict = "STRUCTURALLY_SIMILAR"
    else:
        verdict = "EXACT_LOCAL_EQUIVALENT"
    res = {
        "verdict": verdict,
        "blocks": [len(la.blocks), len(lb.blocks)], "aligned": len(align), "blocks_equal": blocks_eq,
        "cfg_problems": problems[:5], "unaligned": [list(map(fmt_bid, ua))[:8], list(map(fmt_bid, ub))[:8]],
        "instructions": total_ins, "unsupported_instructions": unk_ins,
        "unsupported_mnemonics": dict(la.unsupported + lb.unsupported),
        "assumptions": assumptions,
    }
    if first:
        kind, a, b, d = first
        res["first_difference"] = {
            "kind": kind, "dos_block": fmt_bid(a), "c_block": fmt_bid(b),
            "dos_block_insn": block_addr(la, a), "c_block_insn": block_addr(lb, b),
            "dos": d[1] if len(d) > 1 else None, "c": d[2] if len(d) > 2 else None,
            "detail": d[3] if len(d) > 3 else None}
    return res


def block_addr(L, bid):
    b = L.blocks[bid]
    return f"{b.ins[0].address:x}" if b.ins else fmt_bid(bid)


class _LitTable(dict):
    def __init__(self, side):
        super().__init__()
        self.side = side

    def get(self, k, default=None):
        if k.startswith("str:"):
            return k[4:].encode("latin1") + b"\0"
        if k.startswith("raw:"):
            return bytes.fromhex(k[4:])
        return default


def eff_kind(e):
    return e[0]


def commute(x, y):
    return x[0] == "st" and y[0] == "st" and distinct(x[2], x[1], y[2], y[1])


def core_diff(cmp, a, b, depth=0):
    """Descend to the smallest differing sub-expression pair (diagnostic text)."""
    cmp.why = None
    m = cmp.mark()
    r = cmp.eq(a, b)
    why = cmp.why
    cmp.undo(m)
    if r is not False:
        return "equal in isolation (binding conflict with earlier comparisons)"
    if depth < 25 and isinstance(a, tuple) and isinstance(b, tuple) and a and b and a[0] == b[0] and             len(a) == len(b) and a[0] not in ("phi",) and not phiview(a) and not phiview(b):
        kids = [(x, y) for x, y in zip(a, b) if isinstance(x, tuple) and isinstance(y, tuple)]
        if a[0] == "sum" and len(a[3]) == len(b[3]):
            kids = list(zip(sorted((t for _, t in a[3]), key=key), sorted((t for _, t in b[3]), key=key)))
        elif a[0] in ("sum", "and", "or", "xor", "mul", "eq"):
            kids = []       # commutative: report the node itself
        for x, y in kids:
            m = cmp.mark()
            rr = cmp.eq(x, y)
            cmp.undo(m)
            if rr is False:
                return core_diff(cmp, x, y, depth + 1)
    txt = f"innermost difference: DOS {show(a)} | C {show(b)}"
    return txt + (f" [{why}]" if why else "")


def compare_effects(cmp, ea, eb, ta, tb, void, rw=32):
    """Return None if equal, ('unknown', ...) or (kind, dos, c, detail). DOS effects are matched in order; a C
    effect may be matched out of order only if it commutes (provably distinct store addresses, C side) with every
    unmatched C effect before it."""
    used = [False] * len(eb)
    for k, x in enumerate(ea):
        found = None
        for j, y in enumerate(eb):
            if used[j] or x[0] != y[0]:
                continue
            if not all(used[q] or commute(eb[q], y) or (cmp.loose and eb[q][0] == y[0] == "st")
                       for q in range(j)):
                break
            m = cmp.mark()
            r = cmp.eq(x, y)
            if r is True:
                found = j
                break
            if r is None:
                return ("unknown", show(x), show(y), "unsupported value in effect")
            cmp.undo(m)
        if found is None:
            j = next((q for q in range(len(eb)) if not used[q]), None)
            if j is None:
                return ("effect", show(x), "<none>", f"DOS effect #{k} has no partner")
            y = eb[j]
            return ("effect", show(x), show(y), f"effect #{k} differs; " +
                    (core_diff(cmp, x, y) if x[0] == y[0] else "different effect kinds"))
        used[found] = True
    if not all(used):
        j = used.index(False)
        return ("effect", "<none>", show(eb[j]), "C has an extra effect")
    if ta[0] != tb[0]:
        return ("terminator", str(ta[0]), str(tb[0]), "")
    if ta[0] == "br":
        if ta[1][0] == "unknown-cond" or tb[1][0] == "unknown-cond":
            return ("unknown", str(ta[1]), str(tb[1]), "branch on unmodelled flags")
        r = cmp.eq(ta[1], tb[1])
        if r is False:
            return ("branch", show(ta[1]), show(tb[1]), "branch predicate; " + core_diff(cmp, ta[1], tb[1]))
        if r is None:
            return ("unknown", show(ta[1]), show(tb[1]), "branch predicate")
    if ta[0] == "ret":
        if ta[2] != tb[2]:
            return ("return", f"ret {ta[2]}", f"ret {tb[2]}", "stack cleanup")
        if ta[3] != 4 + ta[2] or tb[3] != 4 + tb[2]:
            pass
        if not void:
            ra, rb = (ta[1], tb[1]) if rw >= 32 else (trunc(rw, ta[1]), trunc(rw, tb[1]))
            r = cmp.eq(ra, rb)
            if r is False:
                return ("return", show(ta[1]), show(tb[1]), "return value (eax); " + core_diff(cmp, ta[1], tb[1]))
            if r is None:
                return ("unknown", show(ta[1]), show(tb[1]), "return value")
    if ta[0] == "switch":
        r = cmp.eq(ta[1], tb[1])
        if r is False:
            return ("branch", show(ta[1]), show(tb[1]), "switch index")
        if r is None:
            return ("unknown", show(ta[1]), show(tb[1]), "switch index")
    if ta[0] == "unk":
        if ta[1] == tb[1] == "falls off decoded range":
            cmp.notes.add("a block ends in a call followed by the next function (noreturn callee assumed)")
            return None
        return ("unknown", str(ta[1]), str(tb[1]), "unsupported terminator")
    return None


# ----------------------------------------------------------------------------------------------- unit handling
_orc = None


def oracle():
    global _orc
    if _orc is None:
        _orc = census.Oracle()
    return _orc


def compile_unit(src, tag):
    obj = ROOT / "build" / "semir" / (tag + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    stamp = obj.with_suffix(".src")
    text = Path(src).read_bytes()
    if obj.exists() and stamp.exists() and stamp.read_bytes() == text:
        return obj
    prof = wcc.profile("game")
    rc, out = wcc.compile_c(Path(src), obj, prof["flags"], prof["install"], extra_include=[ROOT / "src"])
    if rc:
        raise SystemExit(f"compile failed: {src}\n{out[-800:]}")
    stamp.write_bytes(text)
    return obj


def seg_bases(results):
    votes = collections.defaultdict(set)
    for r in results:
        for ref in r.get("references", []):
            t = ref.get("target", "")
            if t.startswith("seg:") and "oracle_target" in ref and ref.get("verified_by") and not ref["self_rel"]:
                o, a = ref["oracle_target"].split(":")
                votes[t[4:]].add((int(o), (int(a, 16) - ref["disp"] - ref["field"]) & M32))
    return {k: next(iter(v)) for k, v in votes.items() if len(v) == 1}


def ret_width(src_text, func):
    """Declared return width of our C definition: 0 (void), 8, 16 or 32."""
    m = re.search(r"(?:^|\n)[ \t]*(?:static\s+)?([A-Za-z_][\w \t]*?)\s*(\**)\s*(?:__\w+\s+)?(?<![\w])" + re.escape(func) +
                  r"\s*\([^;]*?\)\s*\{", src_text)
    if not m:
        return 32
    t, star = m.group(1).split(), m.group(2)
    if star:
        return 32
    if t and t[-1] == "void":
        return 0
    if any(x in ("u8", "s8", "char", "bool", "BOOL", "uchar", "byte") for x in t):
        return 8
    if any(x in ("u16", "s16", "short", "ushort", "word") for x in t):
        return 16
    return 32


def run_unit(unit, funcs=None, src=None, tag=None):
    orc = oracle()
    units = load_json(ROOT / "config" / "units.json")["units"]
    u = units[unit]
    src = Path(src) if src else ROOT / u["source"]
    obj = compile_unit(src, tag or unit)
    text = src.read_text(encoding="latin1")
    callees = CalleeModel(orc)
    out = []
    for mod in omf.load(obj):
        results = match.verify_object(orc.img, mod)
        bases = seg_bases(results)
        for r in results:
            if funcs and r["function"] not in funcs:
                continue
            if not r.get("oracle"):
                continue
            if r["size"] == 0:
                out.append(dict(unit=unit, function=r["function"], match_status=r["status"], size=0,
                                verdict="UNKNOWN", error="empty extent (alias of the next public)"))
                continue
            o, a = r["oracle"].split(":")
            oside = OracleSide(orc, int(a, 16), r["size"])
            cside = CandSide(orc, mod, r, bases)
            cside.fbase = int(a, 16) - r["seg_offset"]
            try:
                res = compare_function(oside, cside, callees, rw=ret_width(text, r["function"]))
            except RecursionError:
                res = {"verdict": "UNKNOWN", "error": "recursion"}
            except Exception as e:      # prototype: report, never equivalence
                import traceback
                res = {"verdict": "UNKNOWN", "error": f"{type(e).__name__}: {e}",
                       "trace": traceback.format_exc()[-600:]}
            res.update(unit=unit, function=r["function"], match_status=r["status"], seg_offset=r["seg_offset"],
                       oracle=r["oracle"],
                       match_first_diff=r.get("first_diff"), byte_diffs=r.get("byte_diffs"), size=r["size"])
            out.append(res)
    return out


def main(argv):
    sys.setrecursionlimit(20000)
    src = None
    jout = None
    if "--src" in argv:
        k = argv.index("--src")
        src = argv[k + 1]
        del argv[k:k + 2]
    if "--json" in argv:
        k = argv.index("--json")
        jout = argv[k + 1]
        del argv[k:k + 2]
    unit, funcs = argv[0], set(argv[1:]) or None
    res = run_unit(unit, funcs, src, tag=Path(src).stem if src else None)
    for r in res:
        fd = r.get("first_difference")
        print(f"{r['function']:32} {r['match_status']:12} {r['verdict']:24} blocks {r.get('blocks')} "
              f"unsup {r.get('unsupported_instructions')}/{r.get('instructions')}")
        if fd:
            print(f"    first {fd['kind']} DOS@{fd['dos_block_insn']} C@{fd['c_block_insn']}: {fd.get('detail')}")
            print(f"      DOS: {str(fd['dos'])[:300]}")
            print(f"      C  : {str(fd['c'])[:300]}")
        if r.get("cfg_problems"):
            print("    cfg:", r["cfg_problems"][:3])
        if r.get("error"):
            print("    ERROR", r["error"], r.get("trace", "")[-400:])
    if jout:
        Path(jout).write_text(json.dumps(res, indent=1, default=str))


if __name__ == "__main__":
    main(sys.argv[1:])
