"""Integrator helper: merge a worker's overlay bindings into config/symbols.json and register a unit.

    python tools/promote.py SYMBOLS_OVERLAY.json [--rename OLD=NEW ...]
    python tools/promote.py --unit NAME SRC.c [--oracle O] [--provenance TAG,TAG]   (default tags: from available reference bodies)

Bindings: a name already bound to a different address, or an address already bound under a different name in
the same section, is refused. --unit adds/updates config/units.json with the statuses that tools/verify.py
measures now (fresh compile) and refuses if any already-accepted unit regresses.
"""
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import CONFIG, ROOT, dump_json, load_json  # noqa: E402


SIZES = {"char": 1, "u8": 1, "s8": 1, "unsigned char": 1, "short": 2, "s16": 2, "u16": 2, "unsigned short": 2,
         "int": 4, "s32": 4, "u32": 4, "unsigned": 4, "long": 4, "unsigned int": 4}


def type_size(t):
    """Size of a simple 'type' string ('s16', 'short[256]', 'u8[2][3]'); None if unknown (structs etc.)."""
    if not t:
        return None
    base = t.split("[")[0].strip()
    if base not in SIZES:
        return None
    n = SIZES[base]
    for dim in t.split("[")[1:]:
        try:
            n *= int(dim.rstrip("] "), 0)
        except ValueError:
            return None
    return n


def overlaps(canon):
    """Data bindings whose [addr, addr+size) ranges overlap (sizes from their 'type'). One address = one symbol."""
    from image import parse_addr
    bad = []
    for oracle, secs in canon.items():
        if not isinstance(secs, dict):
            continue
        spans = []
        for name, ent in secs.get("data", {}).items():
            o, a = parse_addr(ent["addr"])
            spans.append((o, a, a + (type_size(ent.get("type")) or ent.get("size") or 1), name))
        spans.sort()
        for x, y in zip(spans, spans[1:]):
            if x[0] == y[0] and y[1] < x[2]:
                bad.append(f"{oracle}: {x[3]} {x[0]}:{x[1]:x}+{x[2] - x[1]} overlaps {y[3]} {y[0]}:{y[1]:x}")
    return bad


def norm(addr):
    """Canonical 'obj:offset' text (offset lower-case hex, 5 digits for code, 4+ for data)."""
    from image import parse_addr
    o, a = parse_addr(addr)
    return f"{o}:{a:05x}" if o == 1 else f"{o}:{a:04x}"


def android_mismatches(canon):
    """Data bindings whose simple type size disagrees with the Android symbol size of the same name.
    A binding may override Android only with an explicit "dos_size_evidence" (the Android port widened some
    globals; e.g. world_index, proven 1 byte on DOS by the BSS size-rank layout)."""
    import android
    try:
        A = android.load()
    except FileNotFoundError:
        return []
    bad = []
    for oracle, secs in canon.items():
        if not isinstance(secs, dict):
            continue
        for name, ent in secs.get("data", {}).items():
            ts = type_size(ent.get("type"))
            a = A.get(name)
            if ts is not None and a and a["kind"] == "O" and a["size"] != ts and not ent.get("dos_size_evidence"):
                bad.append(f"{oracle}: {name} type {ent.get('type')} = {ts} bytes, Android size {a['size']}")
    return bad


RUNTIME = re.compile(r"^(IF@|__)")                       # compiler-generated / runtime entry names
RUNTIME_EVIDENCE = re.compile(r"libscan|\.LIB\b|probe", re.I)


def claim_index(have):
    """name -> (canonical name, address) for every canonical name and recorded alias of one section; refuses an
    index in which one name is claimed at two addresses."""
    claims = {}
    for k, v in have.items():
        for n in [k] + list(v.get("aliases", [])):
            c = (k, norm(v["addr"]))
            if n in claims and claims[n][1] != c[1]:
                raise SystemExit(f"contradictory bindings: {n} claimed by {claims[n][0]} at {claims[n][1]} and by "
                                 f"{k} at {c[1]}")
            claims.setdefault(n, c)
    return claims


def merge_symbols(overlay, renames):
    """Merge an overlay. A rename OLD=NEW onto an existing NEW at the same address records OLD as NEW's alias."""
    canon = load_json(CONFIG / "symbols.json")
    ov = load_json(overlay)
    added = 0
    for oracle, secs in ov.items():
        if not isinstance(secs, dict):          # e.g. a top-level "note"
            continue
        dst = canon.setdefault(oracle, {"functions": {}, "data": {}})
        for sec in ("functions", "data"):
            have = dst.setdefault(sec, {})
            by_addr = {norm(v["addr"]): k for k, v in have.items()}
            claims = claim_index(have)
            for oname, ent in secs.get(sec, {}).items():
                if "aliases" in ent:
                    raise SystemExit(f"overlay entry {oname} carries aliases; only tools/integrate.py --rename "
                                     "records aliases")
                name = renames.get(oname, oname)
                ent = dict(ent, addr=norm(ent["addr"]))
                if oname != name and oname in claims and claims[oname][1] != ent["addr"]:
                    raise SystemExit(f"conflict: {oname} already claimed by {claims[oname][0]} at {claims[oname][1]}")
                if oname != name and name not in claims:
                    raise SystemExit(f"--rename {oname}={name}: {name} is not bound; a rename must target an existing "
                                     "binding at the same address (the old name is then recorded as its alias)")
                if name in claims:
                    if claims[name][1] != ent["addr"]:
                        raise SystemExit(f"conflict: {name} claimed by {claims[name][0]} at {claims[name][1]}, "
                                         f"overlay {ent['addr']}")
                    if oname != name and oname not in claims:      # explicit rename: keep the old name claimed
                        have[claims[name][0]].setdefault("aliases", []).append(oname)
                        claims[oname] = claims[name]
                    continue
                if ent["addr"] in by_addr:
                    raise SystemExit(f"conflict: {ent['addr']} already bound as {by_addr[ent['addr']]}, overlay {name}")
                if RUNTIME.match(name) and not RUNTIME_EVIDENCE.search(ent.get("evidence", "")):
                    raise SystemExit(f"refusing runtime/compiler name {name}: needs library-member or compile-probe "
                                     "evidence (libscan, .LIB member, probe), not 'the target of my own call'")
                have[name] = ent
                by_addr[ent["addr"]] = name
                claims[name] = (name, ent["addr"])
                added += 1
    bad = overlaps(canon) + android_mismatches(canon)
    if bad:
        raise SystemExit("refusing: overlapping or wrongly sized data bindings:\n  " + "\n  ".join(bad))
    dump_json(CONFIG / "symbols.json", canon)
    print(f"merged {added} bindings into config/symbols.json")


def default_provenance(fname):
    """docs/MATCHING.md tags from which reference bodies exist (a Rayverse stub does not count)."""
    import seed
    tags = []
    rv = seed.rayverse_body(fname)[0]
    if rv and "Not implemented" not in rv and "//stub" not in rv:
        tags.append("RAYVERSE_GUIDED")
    if seed.ps1_body(fname)[0]:
        tags.append("PS1_SOURCE_GUIDED")
    return tags + ["DOS_BINARY_DERIVED"]


def register_unit(name, src, oracle, prov):
    units = load_json(CONFIG / "units.json")
    units["units"].setdefault(name, {"source": src, "oracle": oracle, "profile": "game", "functions": {}})
    dump_json(CONFIG / "units.json", units)
    r = subprocess.run([sys.executable, str(ROOT / "tools" / "verify.py"), "--accept", name], cwd=ROOT)
    units = load_json(CONFIG / "units.json")
    for fname, fn in units["units"][name]["functions"].items():
        if not fn.get("provenance"):
            fn["provenance"] = prov or default_provenance(fname)
    dump_json(CONFIG / "units.json", units)
    return r.returncode


def main(argv):
    if argv[1] == "--unit":
        name, src = argv[2], argv[3]
        oracle = argv[argv.index("--oracle") + 1] if "--oracle" in argv else "gog_v121_us"
        prov = argv[argv.index("--provenance") + 1].split(",") if "--provenance" in argv else []
        return register_unit(name, src, oracle, prov)
    renames = {}
    for a in argv[2:]:
        if a.startswith("--rename"):
            continue
        if "=" in a:
            old, new = a.split("=", 1)
            renames[old] = new
    merge_symbols(argv[1], renames)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
