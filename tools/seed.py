"""Cross-platform source seeding: turn PS1-decomp / Rayverse C into DOS candidates and diagnose them.

    python tools/seed.py mine                     token-level PS1->DOS differences in accepted STRICT functions
    python tools/seed.py try NAME [--src ps1|rayverse|both] [--keep]
                                                  seed one function into its unit context, compile, verify
    python tools/seed.py pilot NAME... | --pool N [--json OUT]
                                                  run `try` over many functions and summarise

Seeding is subordinate to the TU workflow: candidates are written only to build/seed/; nothing in src/ or config/ is
changed (promotion stays manual through tools/promote.py). Only evidence-backed, semantics-preserving rewrites are
applied (config/seed_rules.json lists each rule with its evidence); the variant search covers declaration widths
that leave the C semantics of the body unchanged for in-range values (return/parameter width), never byte tricks.
"""
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, ROOT, dump_json, load_json  # noqa: E402

PS1 = ROOT / "external" / "rayman-ps1-decomp"
RV = ROOT / "external" / "rayverse" / "src"
OUT = BUILD / "seed"
DEF = r"^(?P<head>[A-Za-z_][\w \*]*?\b{name}\s*\((?P<params>[^;{{]*)\))\s*\{{"


# ----------------------------------------------------------------------------------------------- source access
def find_function(text, name):
    """Return (start, end, header, params) of the definition of `name` in C text, or None. Brace matched."""
    m = re.search(DEF.format(name=re.escape(name)), text, re.M)
    if not m:
        return None
    i = text.index("{", m.end() - 1)
    depth = 0
    for j in range(i, len(text)):
        c = text[j]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return m.start(), j + 1, m.group("head"), m.group("params")
    return None


def ps1_body(name):
    for f in sorted((PS1 / "src").rglob("*.c")):
        t = f.read_text(encoding="latin-1")
        if name + "(" not in t:
            continue
        r = find_function(t, name)
        if r:
            return t[r[0]:r[1]], f.relative_to(PS1).as_posix()
    return None, None


def rayverse_body(name):
    for f in sorted(RV.glob("*.c")):
        t = f.read_text(encoding="latin-1")
        if name + "(" not in t:
            continue
        r = find_function(t, name)
        if r:
            body = t[r[0]:r[1]]
            return body, f.name
    return None, None


def units():
    return load_json(CONFIG / "units.json")["units"]


def unit_of(name):
    for un, u in units().items():
        if name in u["functions"]:
            return un, u
    return None, None


# ----------------------------------------------------------------------------------------------- mining
TOK = re.compile(r"[A-Za-z_]\w*|0x[0-9A-Fa-f]+|\d+|->|<<|>>|<=|>=|==|!=|&&|\|\||\S")


def tokens(code):
    code = re.sub(r"/\*.*?\*/|//[^\n]*", " ", code, flags=re.S)
    return TOK.findall(code)


def mine():
    import difflib
    from collections import Counter
    pairs, same, diff, n = Counter(), 0, 0, 0
    report = []
    for un, u in units().items():
        if not u["functions"]:
            continue
        src = (ROOT / u["source"]).read_text(encoding="utf-8", errors="replace")
        for fn, ent in u["functions"].items():
            if ent["status"] != "STRICT":
                continue
            r = find_function(src, fn)
            pb, _ = ps1_body(fn)
            if not r or not pb:
                continue
            n += 1
            a, b = tokens(pb), tokens(src[r[0]:r[1]])
            sm = difflib.SequenceMatcher(a=a, b=b, autojunk=False)
            ops = [op for op in sm.get_opcodes() if op[0] != "equal"]
            if not ops:
                same += 1
            else:
                diff += 1
            for tag, i1, i2, j1, j2 in ops:
                pairs[(" ".join(a[i1:i2])[:60], " ".join(b[j1:j2])[:60])] += 1
            report.append({"function": fn, "unit": un, "edits": len(ops), "ratio": round(sm.ratio(), 3)})
    dump_json(OUT / "mine.json", {"functions": report, "pairs": [[k[0], k[1], v] for k, v in pairs.most_common(300)]})
    print(f"STRICT DOS functions with a PS1 body: {n}; token-identical {same}; differing {diff}")
    print("most frequent PS1 -> DOS token edits:")
    for (x, y), v in pairs.most_common(40):
        print(f"  {v:4d}  {x!r:42} -> {y!r}")
    return 0


# ----------------------------------------------------------------------------------------------- adaptation
def rules():
    return load_json(CONFIG / "seed_rules.json")


def adapt_ps1(body):
    for r in rules()["rules"]:
        body = re.sub(r["pattern"], r["repl"], body)
    return body


KEYWORDS = set("""auto break case char const continue default do double else enum extern float for goto if int long
register return short signed sizeof static struct switch typedef union unsigned void volatile while u8 s8 u16 s16 u32
s32 obj_t eta_t anim_t sprite_t zdc_t level_t level_obj_t active_objects_t map_tile_t map_data_t block_flags_t
status_bar_t rgb_t rgb_palette_t world_info_t poing_t rayevts_t finbosslevel_t NULL memcpy memset strlen strcpy abs
FLG LEN MAX_1 MAX_2 MAX_3 MIN_1 MIN_2 MIN_3 ABS SGN ashl16 ashr16""".split())
HDR = re.compile(r"^([A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\))\s*\{", re.M)
PROTO = re.compile(r"^([A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\))\s*;", re.M)


def dos_signatures():
    """name -> header of functions defined in accepted src units (DOS-verified signatures)."""
    sigs = {}
    for f in sorted((ROOT / "src").glob("*.c")):
        for m in HDR.finditer(f.read_text(encoding="utf-8", errors="replace")):
            sigs.setdefault(m.group(2), m.group(1))
    return sigs


def proto_sources():
    """name -> (origin, prototype) from Rayverse proto.h, then PS1 headers (adapted)."""
    out = {}
    if (RV / "proto.h").exists():
        for m in PROTO.finditer((RV / "proto.h").read_text(encoding="latin-1")):
            out.setdefault(m.group(2), ("rayverse", m.group(1)))
    for f in sorted((PS1 / "include").rglob("*.h")):
        for m in PROTO.finditer(f.read_text(encoding="latin-1")):
            out.setdefault(m.group(2), ("ps1", adapt_ps1(m.group(1))))
    return out


def ps1_defines():
    """Numeric constants of the PS1 headers: `#define NAME n` and enum members (explicit or sequential)."""
    out = {}
    for f in sorted((PS1 / "include").rglob("*.h")):
        t = f.read_text(encoding="latin-1")
        for m in re.finditer(r"^#define\s+([A-Z_][A-Z0-9_]*)\s+(-?(?:0x[0-9A-Fa-f]+|\d+))\s*$", t, re.M):
            out.setdefault(m.group(1), m.group(2))
        for e in re.finditer(r"\benum\b[^{;]*\{([^}]*)\}", t):
            val = -1
            body = re.sub(r"/\*.*?\*/|//[^\n]*", " ", e.group(1), flags=re.S)
            for item in body.split(","):
                item = item.strip()
                if not item:
                    continue
                name, _, expr = item.partition("=")
                name = name.strip()
                if expr.strip():
                    try:
                        val = int(expr.strip(), 0)
                    except ValueError:
                        break        # symbolic initialiser: stop (do not guess)
                else:
                    val += 1
                if re.fullmatch(r"[A-Za-z_]\w*", name):
                    out.setdefault(name, str(val))
    return out


def rayverse_constants_and_types():
    """Rayverse header constants (#define / enum members) and typedef blocks; DOS-derived, used as draft context
    after the verified shared headers (src/include) - the matcher still decides."""
    consts, types = {}, {}
    for f in sorted(RV.glob("*.h")):
        t = f.read_text(encoding="latin-1")
        for m in re.finditer(r"^#define\s+([A-Za-z_]\w*)\s+(-?(?:0x[0-9A-Fa-f]+|\d+))\s*(?://.*)?$", t, re.M):
            consts.setdefault(m.group(1), m.group(2))
        for e in re.finditer(r"\benum\b[^{;]*\{([^}]*)\}", t):
            val = -1
            for item in re.sub(r"/\*.*?\*/|//[^\n]*", " ", e.group(1), flags=re.S).split(","):
                nm, _, expr = item.strip().partition("=")
                nm = nm.strip()
                if not nm:
                    continue
                try:
                    val = int(expr.strip(), 0) if expr.strip() else val + 1
                except ValueError:
                    break
                if re.fullmatch(r"[A-Za-z_]\w*", nm):
                    consts.setdefault(nm, str(val))
        for m in re.finditer(r"^typedef\s+(?:struct|union)\s+\w*\s*\{.*?^\}\s*(\w+)\s*;", t, re.M | re.S):
            types.setdefault(m.group(1), m.group(0))
    return consts, types


def decl_from_type(name, t):
    """'s16[129]' -> 'extern s16 name[129];'   'char*[300]' -> 'extern char *name[300];'"""
    base, _, dims = t.partition("[")
    dims = "[" + dims if dims else ""
    stars = ""
    while base.endswith("*"):
        stars, base = stars + "*", base[:-1]
    return f"extern {base.strip()} {stars}{name}{dims};"


def rayverse_data_decls():
    out = {}
    for ln in (RV / "data.c").read_text(encoding="latin-1").splitlines():
        m = re.match(r"^([A-Za-z_][\w ]*?\s+\**)(\w+)((?:\s*\[[^\]]*\])*)\s*[=;]", ln)
        if m and not ln.startswith(("typedef", "static", "return")):
            out.setdefault(m.group(2), f"extern {m.group(1).strip()} {m.group(2)}{m.group(3)};")
    return out


def identifiers(code):
    code = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"', " ", code, flags=re.S)
    return {m.group(2) for m in re.finditer(r"(->|\.)?\b([A-Za-z_]\w*)\b", code) if not m.group(1)}


# ----------------------------------------------------------------------------------------------- context
_cache = {}


def ctx():
    if not _cache:
        x = load_json(BUILD / "xref" / "xref.json")
        sym = load_json(CONFIG / "symbols.json")["gog_v121_us"]
        rv_consts, rv_types = rayverse_constants_and_types()
        shared = ""
        for h in ("obj.h", "game.h", "map.h", "rtypes.h"):
            shared += (ROOT / "src" / "include" / h).read_text(encoding="utf-8")
        defines = dict(rv_consts)
        defines.update(ps1_defines())          # PS1 constants for PS1 bodies; Rayverse ones fill the rest
        _cache.update(addr={r["name"]: r["dos"] for r in x}, dos_sigs=dos_signatures(), protos=proto_sources(),
                      defines=defines, rv_data=rayverse_data_decls(), sym_data=sym.get("data", {}),
                      sym_funcs=sym.get("functions", {}),
                      rv_types={k: v for k, v in rv_types.items() if not re.search(r"\b%s\b" % k, shared)})
    return _cache


def type_blocks(code):
    """Rayverse typedef blocks needed by `code` (recursively), for types the shared headers do not define."""
    t = ctx()
    out, seen = [], set()

    def need(text):
        for name in sorted(set(re.findall(r"\b\w+_t\b", text)) & set(t["rv_types"]) - seen):
            seen.add(name)
            need(t["rv_types"][name])
            out.append(t["rv_types"][name])
    need(code)
    return out


def declarations_for(idents):
    """(defines, externs, prototypes, missing) for identifiers, from the best evidence available:
    typed bindings (symbols.json) > DOS-verified signatures (src/) > Rayverse/PS1 prototypes > Rayverse data.c >
    PS1 constants. Missing names are reported, never invented."""
    t = ctx()
    defs, decls, protos, missing = [], [], [], []
    for ident in sorted(idents):
        if ident in t["sym_data"] and t["sym_data"][ident].get("type"):
            decls.append(decl_from_type(ident, t["sym_data"][ident]["type"]))
        elif ident in t["dos_sigs"]:
            protos.append(t["dos_sigs"][ident] + ";")
        elif ident in t["protos"]:
            protos.append(t["protos"][ident][1] + ";")
        elif ident in t["rv_data"]:
            decls.append(t["rv_data"][ident])
        elif ident in t["defines"]:
            defs.append(f"#define {ident} {t['defines'][ident]}")
        else:
            missing.append(ident)
    return defs, decls, protos, missing


def standalone_tu(name, cand):
    """Minimal TU: shared headers, compat prelude, typed externs, best-evidence prototypes, PS1 constants."""
    head = ['#include "obj.h"', '#include "game.h"', '#include "map.h"', rules()["prelude"]]
    defs, decls, protos, missing = declarations_for(identifiers(cand) - KEYWORDS - {name})
    return "\n".join(head + defs + decls + protos) + "\n\n" + cand + "\n", missing


def unit_tu(src_text, name, cand, header):
    """The accepted unit with NAME's definition replaced by the candidate (prototypes kept consistent); identifiers
    the candidate needs but the unit never mentions get declarations from the same evidence order."""
    r = find_function(src_text, name)
    if not r:
        return None, []
    rest = src_text[:r[0]] + src_text[r[1]:]
    known = set(re.findall(r"\b[A-Za-z_]\w*\b", rest))
    local_hdrs = [ROOT / "src" / h for h in re.findall(r'#include\s+"([^"]+)"', rest) if (ROOT / "src" / h).exists()]
    for h in local_hdrs + [ROOT / "src" / "include" / x for x in ("obj.h", "game.h", "map.h", "rtypes.h")]:
        if h.exists():
            known |= set(re.findall(r"\b[A-Za-z_]\w*\b", h.read_text(encoding="utf-8", errors="replace")))
    defs, decls, protos, missing = declarations_for(identifiers(cand) - KEYWORDS - known - {name})
    # prelude immediately before the candidate: the unit's own (DOS-verified) macros are already defined there and
    # the per-macro guards skip ours
    text = (src_text[:r[0]] + rules()["prelude"] + "\n".join(defs + decls + protos) + "\n" + cand
            + src_text[r[1]:])
    text = re.sub(r"^[A-Za-z_][\w \*]*?\b%s\s*\([^;{]*\)\s*;" % re.escape(name), lambda m: header + ";",
                  text, flags=re.M)
    return text, missing


def overlay_for(name, code):
    t = ctx()
    funcs = {i: {"addr": t["addr"][i], "name": "RAYVERSE", "evidence": "seed: Rayverse address annotation"}
             for i in identifiers(code) | {name}
             if i in t["addr"] and i not in t["sym_funcs"] and i not in t["sym_data"]}
    return {"gog_v121_us": {"functions": funcs, "data": {}}}


# ----------------------------------------------------------------------------------------------- try / pilot
RET_WIDTHS = ["s16", "u8", "u16", "s32", "void"]


def variants_of(body):
    """(label, body): the body, then return-width variants (declaration-only change; proven DOS rule)."""
    yield "as-is", body
    m = re.match(r"\s*(?:static\s+)?([A-Za-z_][\w ]*?\s*\**)\s*\b\w+\s*\(", body)
    if not m:
        return
    cur = m.group(1).strip()
    returns_value = re.search(r"\breturn\s+[^;\s]", body) is not None
    for w in RET_WIDTHS:
        if w == cur or (w == "void") == returns_value:
            continue
        yield f"ret={w}", body[:m.start(1)] + w + " " + body[m.end(1):]


_IMG = []


def compile_and_check(tu_text, name, overlay, tag):
    import match
    import omf
    import wcc
    from image import Image
    if not _IMG:
        _IMG.append(Image())
    d = OUT / name
    d.mkdir(parents=True, exist_ok=True)
    src, ov, obj = d / f"{tag}.c", d / f"{tag}.symbols.json", d / f"{tag}.obj"
    src.write_text(tu_text, encoding="utf-8")
    dump_json(ov, overlay)
    prof = wcc.profile("game")
    added = set()
    for _ in range(4):
        rc, text = wcc.compile_c(src, obj, prof["flags"], prof["install"], extra_include=[ROOT / "src"])
        if not rc:
            break
        # compiler-reported undeclared names: declare them from the evidence tables (never invented) and retry
        undeclared = set(re.findall(r"E1011: Symbol '(\w+)' has not been declared", text)) - added
        if not undeclared:
            break
        defs, decls, protos, missing = declarations_for(undeclared)
        added |= undeclared
        if not (defs or decls or protos):
            break
        r = find_function(tu_text, name)
        ins = "\n".join(defs + decls + protos) + "\n"
        tu_text = tu_text[:r[0]] + ins + tu_text[r[0]:] if r else ins + tu_text
        src.write_text(tu_text, encoding="utf-8")
    if rc:
        errs = [ln.split(": ", 1)[-1] for ln in text.splitlines() if "Error!" in ln]
        return {"status": "COMPILE_FAIL", "error": (errs[0] if errs else text)[-120:]}
    match.Bindings.extra = str(ov)
    try:
        res = [r for m in omf.load(obj) for r in match.verify_object(_IMG[0], m) if r["function"] == name]
    except SystemExit as e:
        return {"status": "BINDING_CONFLICT", "error": str(e)[:120]}
    finally:
        match.Bindings.extra = None
    if not res:
        return {"status": "NOT_EMITTED"}
    r = res[0]
    out = {"status": r["status"], "byte_diffs": r.get("byte_diffs", 0), "size": r["size"],
           "first_diff": r.get("first_diff"), "problems": r["problems"][:2], "unverified": len(r["unverified"])}
    if r.get("oracle"):
        out["insn_similarity"] = insn_similarity(obj, r, _IMG[0])
    return out


def insn_similarity(obj, r, img):
    """Instruction-level similarity of candidate vs oracle (0..1): byte diff counts exaggerate, because one extra
    instruction shifts everything after it. Absolute addresses/displacements are masked; diagnostic only."""
    import difflib
    import capstone
    import omf
    m = omf.load(obj)[0]
    seg = next(s for s in m.segments if s and s.name == r["segment"])
    cand = bytes(seg.data[r["seg_offset"]:r["seg_offset"] + r["size"]])
    o, a = r["oracle"].split(":")
    orig = img.obj[int(o)][int(a, 16):int(a, 16) + r["size"]]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)

    def toks(b):
        return [i.mnemonic + " " + re.sub(r"0x[0-9a-f]{4,}", "A", i.op_str) for i in md.disasm(b, 0)]
    return round(difflib.SequenceMatcher(None, toks(cand), toks(orig), autojunk=False).ratio(), 3)


def try_one(name, sources=("ps1", "rayverse")):
    un, u = unit_of(name)
    src_text = (ROOT / u["source"]).read_text(encoding="utf-8", errors="replace") if u else None
    cands = []
    if "ps1" in sources:
        b, f = ps1_body(name)
        if b:
            cands.append(("ps1", f, adapt_ps1(b)))
    if "rayverse" in sources:
        b, f = rayverse_body(name)
        if b and "Not implemented" not in b:
            cands.append(("rayverse", f, b))
    out = {"function": name, "unit": un, "context": "unit" if u else "standalone", "trials": []}
    if not cands:
        out["result"] = "NO_SOURCE"
        return out
    for origin, f, body in cands:
        for label, vb in variants_of(body):
            hdr = vb[:vb.index("{")].strip()
            tu, missing = unit_tu(src_text, name, vb, hdr) if u else (None, [])
            if tu is None:
                tu, missing = standalone_tu(name, vb)
            res = compile_and_check(tu, name, overlay_for(name, vb), f"{origin}_{label.replace('=', '_')}")
            res.update(origin=origin, file=f, variant=label, undeclared=missing[:8])
            out["trials"].append(res)
            if res["status"] == "STRICT":
                break
        if any(t["status"] == "STRICT" for t in out["trials"]):
            break
    rank = {"STRICT": 0, "CODE_BYTES_MATCH": 1, "NEAR_MATCH": 2}
    best = min(out["trials"], key=lambda t: (rank.get(t["status"], 9), -t.get("insn_similarity", 0),
                                             t.get("byte_diffs", 1e9)))
    out["best"] = best
    sim = best.get("insn_similarity", 0)
    if best["status"] == "STRICT":
        out["result"] = "AUTO_STRICT"
    elif best["status"] in ("NEAR_MATCH", "CODE_BYTES_MATCH"):
        # >= 0.90: source-shape differences only; 0.70..0.90: shape + local logic; < 0.70: likely variant/context
        out["result"] = "NEAR_SHAPE" if sim >= 0.9 else ("NEAR_CLOSE" if sim >= 0.7 else "NEAR_FAR")
    else:
        out["result"] = best["status"]
    return out


def pilot(names, out_json=None):
    from collections import Counter
    rows = []
    for n in names:
        try:
            r = try_one(n)
        except Exception as e:   # the pilot measures the pipeline: record tool failures and continue
            r = {"function": n, "result": "TOOL_ERROR", "error": repr(e)[:200]}
        rows.append(r)
        b = r.get("best", {})
        print(f"{r['result']:13s} {n:34s} {r.get('context', ''):10s} {b.get('origin', ''):8s} "
              f"{b.get('variant', ''):9s} sim={b.get('insn_similarity', '')} diffs={b.get('byte_diffs', '')}/{b.get('size', '')} "
              f"{(b.get('error') or r.get('error') or '')[:60]}")
    print("# " + ", ".join(f"{k} {v}" for k, v in Counter(r["result"] for r in rows).most_common()))
    dump_json(Path(out_json) if out_json else OUT / "pilot.json", rows)
    return rows


def draft(start, end):
    """Seed every Rayverse-annotated function of an untouched object range into one draft unit in build/seed/draft/
    and verify it as a whole: the worker's starting point (functions in address order, best seed per function chosen
    by an individual trial). Nothing in src/ or config/ is changed."""
    import match
    import omf
    import wcc
    from image import Image
    x = load_json(BUILD / "xref" / "xref.json")
    names = [r["name"] for r in sorted(x, key=lambda r: r["dos"]) if start <= int(r["dos"].split(":")[1], 16) < end]
    bodies, report = [], []
    for n in names:
        r = try_one(n)
        b = r.get("best")
        chosen = None
        if b and b.get("origin"):
            src = (OUT / n / f"{b['origin']}_{b['variant'].replace('=', '_')}.c").read_text(encoding="utf-8")
            loc = find_function(src, n)
            chosen = src[loc[0]:loc[1]] if loc else None
        if r["result"] in ("COMPILE_FAIL", "NO_SOURCE", "TOOL_ERROR", "BINDING_CONFLICT", "NOT_EMITTED"):
            chosen = None                       # keep the draft compilable; the worker writes these
        bodies.append((n, chosen))
        report.append({"function": n, "seed_result": r["result"], "origin": (b or {}).get("origin"),
                       "variant": (b or {}).get("variant")})
    code = "\n\n".join(b if b else f"/* {n}: no seed source */" for n, b in bodies)
    defs, decls, protos, missing = declarations_for(identifiers(code) - KEYWORDS - set(names))
    head = ['#include "obj.h"', '#include "game.h"', '#include "map.h"', rules()["prelude"]]
    protos_own = [b[:b.index("{")].strip() + ";" for _, b in bodies if b]
    tu = "\n".join(head + defs + decls + protos + protos_own) + "\n\n" + code + "\n"
    d = OUT / "draft"
    d.mkdir(parents=True, exist_ok=True)
    src = d / f"draft_{start:05x}.c"
    src.write_text(tu, encoding="utf-8")
    ov = d / f"draft_{start:05x}.symbols.json"
    dump_json(ov, overlay_for(names[0] if names else "", code))
    obj = src.with_suffix(".obj")
    prof = wcc.profile("game")
    rc, text = wcc.compile_c(src, obj, prof["flags"], prof["install"], extra_include=[ROOT / "src"])
    whole = {}
    if rc == 0:
        match.Bindings.extra = str(ov)
        try:
            whole = {r["function"]: r["status"] for m in omf.load(obj) for r in match.verify_object(Image(), m)}
        finally:
            match.Bindings.extra = None
    for row in report:
        row["in_draft"] = whole.get(row["function"], "COMPILE_FAIL" if rc else "MISSING")
    dump_json(d / f"draft_{start:05x}.json", {"range": [start, end], "missing_decls": missing[:40], "rows": report})
    strict = sum(1 for r in report if r["in_draft"] == "STRICT")
    print(f"draft 1:{start:05x}..1:{end:05x}: {len(names)} functions, {strict} STRICT in the assembled draft "
          f"({sum(1 for r in report if r['seed_result'] == 'AUTO_STRICT')} individually); compile rc={rc}; "
          f"undeclared {len(missing)} -> {src.relative_to(ROOT)}")
    if rc:
        print("  " + "\n  ".join(ln for ln in text.splitlines() if "Error!" in ln)[:600])
    return report


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    if argv[1] == "draft":
        draft(int(argv[2], 16), int(argv[3], 16))
        return 0
    if argv[1] == "mine":
        return mine()
    if argv[1] == "try":
        print(json.dumps(try_one(argv[2]), indent=1))
        return 0
    if argv[1] == "pilot":
        out = argv[argv.index("--json") + 1] if "--json" in argv else None
        pilot([a for a in argv[2:] if not a.startswith("--") and a != out], out)
        return 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
