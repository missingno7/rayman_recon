"""Integrator helper: merge a worker's overlay bindings into config/symbols.json and register a unit.

    python tools/promote.py SYMBOLS_OVERLAY.json [--rename OLD=NEW ...]
    python tools/promote.py --unit NAME SRC.c [--oracle O] [--provenance TAG,TAG]

Bindings: a name already bound to a different address, or an address already bound under a different name in
the same section, is refused. --unit adds/updates config/units.json with the statuses that tools/verify.py
measures now (fresh compile) and refuses if any already-accepted unit regresses.
"""
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


def merge_symbols(overlay, renames):
    canon = load_json(CONFIG / "symbols.json")
    ov = load_json(overlay)
    added = 0
    for oracle, secs in ov.items():
        dst = canon.setdefault(oracle, {"functions": {}, "data": {}})
        for sec in ("functions", "data"):
            have = dst.setdefault(sec, {})
            by_addr = {norm(v["addr"]): k for k, v in have.items()}
            for name, ent in secs.get(sec, {}).items():
                name = renames.get(name, name)
                ent = dict(ent, addr=norm(ent["addr"]))
                if name in have:
                    if norm(have[name]["addr"]) != ent["addr"]:
                        raise SystemExit(f"conflict: {name} bound to {have[name]['addr']}, overlay {ent['addr']}")
                    continue
                if ent["addr"] in by_addr:
                    raise SystemExit(f"conflict: {ent['addr']} already bound as {by_addr[ent['addr']]}, overlay {name}")
                have[name] = ent
                added += 1
    bad = overlaps(canon) + android_mismatches(canon)
    if bad:
        raise SystemExit("refusing: overlapping or wrongly sized data bindings:\n  " + "\n  ".join(bad))
    dump_json(CONFIG / "symbols.json", canon)
    print(f"merged {added} bindings into config/symbols.json")


def register_unit(name, src, oracle, prov):
    units = load_json(CONFIG / "units.json")
    units["units"].setdefault(name, {"source": src, "oracle": oracle, "profile": "game", "functions": {}})
    dump_json(CONFIG / "units.json", units)
    r = subprocess.run([sys.executable, str(ROOT / "tools" / "verify.py"), "--accept", name], cwd=ROOT)
    units = load_json(CONFIG / "units.json")
    for fn in units["units"][name]["functions"].values():
        if not fn.get("provenance"):
            fn["provenance"] = prov
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
