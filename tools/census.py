"""NEAR_MATCH structural census (diagnostic only - never a verdict; STRICT is decided by tools/match.py).

    python tools/census.py                 all accepted units -> build/near_census.json
    python tools/census.py --brief UNIT..  per-unit worker brief -> build/near_census_UNIT.md (run the census first)
    python tools/census.py --file SRC.c [--symbols OVERLAY.json]   brief for any candidate -> SRC.census.md

For every accepted NEAR function the compiled candidate is compared with the DOS bytes on structure:
  calls   ordered callee identities          globals  data targets (address+addend; CONST literals folded)
  fields  struct displacements (non-stack)   consts   program constants (width masks, 0/1, +-1 comparison
  frame   prologue push set / sub esp                 rewrites and width shifts ignored)
-5 short loads (`mov r,[x-2] ... sar r,16`) are normalised so width changes do not look like new targets.
Classes: REFS_LAYOUT (bytes equal, only intra-object references differ), SHAPE/REGALLOC/FRAME/TYPE_WIDTH/CALL_ORDER
(same calls/globals/fields/constants: Watcom shape only), DIFF_<what> (pointers to a logic/binding/prototype fix).
Validation (docs/CENSUS.md): SAME-structure verdicts were shape-only fixes in 28/30 historical NEAR->STRICT cases;
DIFF pointers were right in ~60%; census-guided finish passes solved 87% / 50% of NEAR functions vs ~46% unguided.
"""
import collections
import json
import difflib
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import capstone  # noqa: E402
import match  # noqa: E402
import omf  # noqa: E402
from image import Image  # noqa: E402
import seed  # noqa: E402
from common import load_json  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp|e?sp)\b")
LIT_LO, LIT_HI = 0x2fd00, 0x3218a          # runtime/CONST literal area of obj2 (docs/DATA.md)
PAD = {"nop", "int3"}
WIDTH = {0xff, 0xffff}                   # masks from width conversions, not program constants


def is_pad(ins):
    return ins.mnemonic in PAD or (ins.mnemonic in ("lea", "mov") and ins.op_str in (
        "eax, [eax]", "eax, eax", "esi, [esi]", "edi, [edi]", "edx, [edx]", "ebx, [ebx]", "ecx, [ecx]"))


def insns(code):
    out = list(MD.disasm(code, 0))
    while out and is_pad(out[-1]):
        out.pop()
    return out


def features(code, refs, rel_calls):
    """refs: {site: target_id} for absolute data refs; rel_calls: {site: target_id} for rel32 calls/jmps."""
    f = {"calls": [], "globals": collections.Counter(), "fields": collections.Counter(),
         "consts": collections.Counter(), "jcc": 0, "frame": [], "toks": [], "rtoks": []}
    ins = insns(code)
    prologue = True
    # -5 short loads `mov r,[x-2] ... sar/shr r,16` read x: undo the -2 so width changes do not look like new
    # targets (the shift may be scheduled a few instructions later)
    short_at = set()
    for k, a in enumerate(ins):
        m0 = re.match(r"mov (e\w\w), dword ptr \[", a.mnemonic + " " + a.op_str)
        if not m0:
            continue
        r = m0.group(1)
        for b in ins[k + 1:k + 9]:
            if b.mnemonic in ("sar", "shr") and b.op_str == r + ", 0x10":
                short_at.add(a.address)
                break
            if b.op_str.split(",")[0].strip() in (r, r[1:]) and b.mnemonic not in ("cmp", "test", "push"):
                break
    for i in ins:
        m, ops = i.mnemonic, i.op_str
        if prologue and m == "push":
            f["frame"].append(ops)
        elif prologue and m == "sub" and ops.startswith("esp,"):
            f["frame"].append(ops)
        else:
            prologue = False
        has_fix = False
        for s in range(i.address, i.address + i.size):
            if s in rel_calls:
                has_fix = True
                if m == "call":
                    f["calls"].append(rel_calls[s])
            if s in refs:
                has_fix = True
                g = refs[s]
                if i.address in short_at and re.match(r"\d:[0-9a-f]+$", g):
                    o, a = g.split(":")
                    g = f"{o}:{int(a, 16) + 2:x}"
                f["globals"][g] += 1
        if m.startswith("j") and m != "jmp":
            f["jcc"] += 1
        t = m + " " + re.sub(r"0x[0-9a-f]{5,}", "A", ops)
        if m.startswith("j") or m in ("call", "loop") or has_fix:
            t = m + " " + ("T" if (m.startswith("j") or m == "loop") else re.sub(r"0x[0-9a-f]+", "A", ops))
            f["rtoks"].append(t)
            f["toks"].append(REG.sub("R", t))
            continue
        for d in re.findall(r"\[(e[a-z]{2})[^\]]*?([+-] 0x[0-9a-f]+|[+-] \d+)\]", ops):
            base, disp = d
            if base not in ("esp", "ebp"):
                v = int(disp.replace(" ", ""), 0) + (2 if i.address in short_at else 0)
                if abs(v) < 0x10000:
                    f["fields"][v] += 1
        for imm in re.findall(r"(?:, |^)(0x[0-9a-f]+|\d+)$", ops):
            v = int(imm, 0)
            if m in ("shl", "sar", "shr", "sal") and v in (16, 24):
                continue                  # width conversion shifts, not program constants
            if v < 0x10000 and not (m in ("sub", "add") and ops.startswith("esp")):
                f["consts"][v] += 1
        f["rtoks"].append(t)
        f["toks"].append(REG.sub("R", t))
    return f


def ext_only(a, b):
    """True if the two token multisets differ only by width-extension / masking instructions."""
    ext = re.compile(r"^(movsx|movzx|cwde|cbw|cwd|cdq|and R, (0xff|0xffff)|shl R, 0x10|sar R, 0x10|shr R, 0x10)")
    ca, cb = collections.Counter(a), collections.Counter(b)
    d = (ca - cb) + (cb - ca)
    return bool(d) and all(ext.match(t) for t in d)


def const_diff(a, b):
    """Program constants that differ, ignoring width masks, 0/1 (boolean materialisation) and +-1 pairs (the same
    comparison written as `< n` or `<= n-1`)."""
    a = set(a) - WIDTH - {0, 1}
    b = set(b) - WIDTH - {0, 1}
    ma, mb = a - b, b - a
    for x in sorted(ma):
        for y in (x - 1, x + 1):
            if y in mb:
                ma.discard(x)
                mb.discard(y)
                break
    return sorted(ma), sorted(mb)


def classify(fc, fo, byte_diffs, problems):
    """Root-cause class of a NEAR function from candidate vs oracle features."""
    if byte_diffs == 0:
        if any("unbound" in p for p in problems):
            return "REFS_UNRESOLVED"
        return "REFS_LAYOUT"
    sem = []
    if collections.Counter(fc["calls"]) != collections.Counter(fo["calls"]):
        sem.append("calls")
    if set(fc["globals"]) != set(fo["globals"]):
        sem.append("globals")
    if set(fc["fields"]) != set(fo["fields"]):
        sem.append("fields")
    if any(const_diff(fc["consts"], fo["consts"])):
        sem.append("consts")
    if sem:
        return "DIFF_" + "+".join(sem)
    if collections.Counter(fc["calls"]) == collections.Counter(fo["calls"]) and fc["calls"] != fo["calls"]:
        return "CALL_ORDER"
    if fc["toks"] == fo["toks"]:
        return "REGALLOC" if fc["frame"] == fo["frame"] else "FRAME"
    if ext_only(fc["toks"], fo["toks"]):
        return "TYPE_WIDTH"
    if fc["calls"] != fo["calls"]:
        return "ORDER"
    return "SHAPE"


class Oracle:
    def __init__(self):
        self.img = Image()
        self.binds = match.Bindings(self.img)
        starts = sorted({a for a in list(self.binds.game.values()) + list(self.binds.lib.values()) if a[0] == 1})
        self.starts = [a[1] for a in starts]
        self.name_at = {}
        for n, a in list(self.binds.game.items()) + list(self.binds.lib.items()):
            self.name_at.setdefault(a, n)

    def extent(self, addr, minimum=0):
        import bisect
        k = bisect.bisect_right(self.starts, addr)
        end = self.starts[k] if k < len(self.starts) else len(self.img.obj[1])
        # the next bound start can be far away when unbound functions follow: cap at 1.5x the candidate + 64
        return min(end, addr + minimum + minimum // 2 + 64) if minimum else end

    def target_id(self, obj, off, text=None):
        if obj == 2 and LIT_LO <= off < LIT_HI:
            return "LIT"
        if obj == 1 and text and text[0] <= off < text[1]:
            return "TEXT"
        return f"{obj}:{off:x}"

    def oracle_features(self, addr, size_hint):
        end = self.extent(addr, size_hint)
        code = bytes(self.img.obj[1][addr:end])
        refs, calls = {}, {}
        for (o, s), (_, to, toff) in self.img.fixups.items():
            if o == 1 and addr <= s < end:
                refs[s - addr] = self.target_id(to, toff, (addr - 0x4000, end + 0x4000))
        for i in MD.disasm(code, 0):
            if i.mnemonic in ("call", "jmp") and i.size == 5 and code[i.address] in (0xE8, 0xE9):
                t = addr + i.address + 5 + struct.unpack_from("<i", code, i.address + 1)[0]
                calls[i.address + 1] = f"1:{t:x}"
        return features(code, refs, calls), end - addr


def candidate_features(orc, mod, r):
    """Features of one verify_object result r from module mod."""
    seg = next(s for s in mod.segments if s and s.name == r["segment"])
    o0 = r["seg_offset"]
    code = bytes(seg.data[o0:o0 + r["size"]])
    si = mod.segments.index(seg)
    refs, calls = {}, {}
    for f in mod.fixups:
        if f.seg != si or not (o0 <= f.off < o0 + r["size"]):
            continue
        rel = f.off - o0
        tname = mod.target_name(f)
        a, _ = orc.binds.lookup(tname)
        if f.self_rel:
            calls[rel] = f"1:{a[1]:x}" if a else "name:" + match.demangle(tname)
        else:
            field = struct.unpack_from("<i", seg.data, f.off)[0] if f.size() == 4 else 0
            if (f.target[0] & 3) == 0:          # segment-relative: own CONST/DATA literal
                cls = mod.segments[f.target[1]].cls.upper()
                refs[rel] = "LIT" if cls == "CONST" else "TEXT" if cls == "CODE" else "SEG"
            else:
                refs[rel] = (("TEXT" if a[0] == 1 else orc.target_id(a[0], a[1] + f.disp + field)) if a
                             else "name:" + match.demangle(tname))
    return features(code, refs, calls)


def compare_unit(orc, src, symbols=None, only=None):
    """Compile src with the game profile and return [(result, cand_features, oracle_features, class, sim)]."""
    import wcc
    obj = ROOT / "build" / "tmp" / ("structcmp_" + Path(src).stem + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    prof = wcc.profile("game")
    rc, text = wcc.compile_c(Path(src), obj, prof["flags"], prof["install"], extra_include=[ROOT / "src"])
    if rc:
        raise SystemExit(f"compile failed: {src}\n{text[-800:]}")
    match.Bindings.extra = str(symbols) if symbols else None
    try:
        out = []
        for mod in omf.load(obj):
            for r in match.verify_object(orc.img, mod, set(only) if only else None):
                if not r.get("oracle") or r["status"] == "STRICT":
                    if r.get("oracle") and r["status"] == "STRICT":
                        out.append((r, None, None, "STRICT", 1.0))
                    continue
                fc = candidate_features(orc, mod, r)
                o, a = r["oracle"].split(":")
                fo, osize = orc.oracle_features(int(a, 16), r["size"])
                r["oracle_size"] = osize
                cls = classify(fc, fo, r.get("byte_diffs", 0), r["problems"] + r["unverified"])
                sim = difflib.SequenceMatcher(None, fc["toks"], fo["toks"], autojunk=False).ratio()
                out.append((r, fc, fo, cls, round(sim, 3)))
        return out
    finally:
        match.Bindings.extra = None




def rows_for(un, res):
    rows = []
    for r, fc, fo, cls, sim in res:
        if cls == "STRICT":
            continue
        rv = seed.rayverse_body(r["function"])[0]
        rows.append({
            "unit": un, "function": r["function"], "class": cls, "sim": sim,
            "size": r["size"], "oracle_size": r.get("oracle_size"), "byte_diffs": r.get("byte_diffs"),
            "first_diff": r.get("first_diff"),
            "ps1": bool(seed.ps1_body(r["function"])[0]),
            "rayverse": bool(rv and "Not implemented" not in rv and "//stub" not in rv),
            "calls_c": len(fc["calls"]), "calls_o": len(fo["calls"]),
            "calls_missing": sorted((collections.Counter(fo["calls"]) - collections.Counter(fc["calls"])).elements()),
            "calls_extra": sorted((collections.Counter(fc["calls"]) - collections.Counter(fo["calls"])).elements()),
            "globals_missing": sorted(set(fo["globals"]) - set(fc["globals"])),
            "globals_extra": sorted(set(fc["globals"]) - set(fo["globals"])),
            "fields_missing": sorted(set(fo["fields"]) - set(fc["fields"])),
            "fields_extra": sorted(set(fc["fields"]) - set(fo["fields"])),
            "consts_missing": sorted(set(fo["consts"]) - set(fc["consts"])),
            "consts_extra": sorted(set(fc["consts"]) - set(fo["consts"])),
            "jcc": [fc["jcc"], fo["jcc"]], "frame": [fc["frame"], fo["frame"]],
            "problems": r["problems"][:3],
        })
    return rows


def census():
    units = load_json(ROOT / "config" / "units.json")["units"]
    orc = Oracle()
    rows = []
    for un, u in units.items():
        near = [f for f, e in u["functions"].items() if e["status"] != "STRICT"]
        if not near:
            continue
        try:
            res = compare_unit(orc, ROOT / u["source"], only=near)
        except SystemExit as e:
            print("FAIL", un, str(e)[:200])
            continue
        rows += rows_for(un, res)
        print(un, collections.Counter(x["class"] for x in rows if x["unit"] == un), flush=True)
    out = ROOT / "build" / "near_census.json"
    out.write_text(json.dumps(rows, indent=1), encoding="utf-8")
    print("total", len(rows), collections.Counter(x["class"] for x in rows))


GUIDE = {
    "REFS_LAYOUT": "code bytes already equal; only references into this object (jump tables / calls) differ - fixed "
                   "by making earlier functions the right size. Do not edit this function.",
    "SHAPE": "calls, globals, struct fields and constants all agree with DOS: logic is right, only Watcom shape "
             "differs (operand/branch order, temporaries, types/widths, loop form). Do not change semantics.",
}


def name_of(orc, ident):
    if ident.startswith("1:"):
        return orc.name_at.get((1, int(ident[2:], 16)), ident)
    if ident.startswith("2:"):
        a = int(ident[2:], 16)
        best = None
        for (o, x), n in orc.name_at.items():
            if o == 2 and x <= a and (best is None or x > best[0]):
                best = (x, n)
        return f"{best[1]}+0x{a - best[0]:x}" if best and a - best[0] < 0x400 else ident
    return ident


def brief(unit, rows=None, path=None, orc=None):
    orc = orc or Oracle()
    if rows is None:
        rows = [r for r in json.load(open(ROOT / "build" / "near_census.json")) if r["unit"] == unit]
    out = [f"# NEAR census for `{unit}` ({len(rows)} functions)", "",
           "Structural comparison of the accepted candidate vs DOS (tools/census.py; docs/CENSUS.md). Validated:",
           "SAME-STRUCTURE (SHAPE) -> the fix was shape-only in 28/30 past cases; DIFF pointers were real in ~16/30",
           "(sometimes the fix is outside the function: a missing/empty callee, a prototype). 'oracle-only' = DOS has",
           "it, candidate does not; 'cand-only' = the reverse. Addresses are resolved to bound names where possible.", ""]
    for r in sorted(rows, key=lambda r: -r["sim"]):
        cls = r["class"]
        out.append(f"## {r['function']}  [{cls}]  sim {r['sim']}  size cand {r['size']} / DOS ~{r['oracle_size']}")
        if cls in GUIDE:
            out.append("- " + GUIDE[cls])
        for k in ("calls", "globals", "fields", "consts"):
            miss, extra = r.get(f"{k}_missing", []), r.get(f"{k}_extra", [])
            if k == "consts":
                extra, miss = const_diff(r.get("consts_extra", []), r.get("consts_missing", []))
            if miss or extra:
                f = (lambda x: name_of(orc, x)) if k in ("calls", "globals") else str
                out.append(f"- {k}: oracle-only {[f(x) for x in miss][:8]}  cand-only {[f(x) for x in extra][:8]}")
        if r["frame"][0] != r["frame"][1]:
            out.append(f"- prologue: cand {r['frame'][0]}  DOS {r['frame'][1]}")
        out.append("")
    p = Path(path) if path else ROOT / "build" / f"near_census_{unit}.md"
    p.write_text("\n".join(out), encoding="utf-8")
    print(p)



def brief_file(src, symbols=None):
    """Census of any candidate source (e.g. a worker's in-progress unit) -> <src>.census.md next to it."""
    orc = Oracle()
    res = compare_unit(orc, Path(src), symbols=symbols)
    brief(Path(src).stem, rows_for(Path(src).stem, res), Path(src).with_suffix(".census.md"), orc)


def main(argv):
    if len(argv) > 2 and argv[1] == "--file":
        sym = argv[argv.index("--symbols") + 1] if "--symbols" in argv else None
        brief_file(argv[2], sym)
    elif len(argv) > 1 and argv[1] == "--brief":
        for u in argv[2:]:
            brief(u)
    else:
        census()


if __name__ == "__main__":
    main(sys.argv)
