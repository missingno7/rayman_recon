"""Progress summary derived from config/units.json and the last build/verify/report.json (run verify first).

    python tools/status.py
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, load_json  # noqa: E402

GAME_CODE_END = {"gog_v121_us": 0x6fbe6}   # first CLIB member (strupr) - game code + hand asm below it
GAME_DATA_END = 0x2d998                      # first runtime _DATA member (cstrt386), docs/DATA.md


def main():
    units = load_json(CONFIG / "units.json")["units"]
    rep_p = BUILD / "verify" / "report.json"
    rep = load_json(rep_p) if rep_p.exists() else {}
    counts, strict_bytes, total = {}, 0, 0
    data_b = sum(g["size"] for segs in rep.get("_segments", {}).values() for g in segs if g["status"] == "VERIFIED")
    for name, u in units.items():
        for fn, ent in u["functions"].items():
            counts[ent["status"]] = counts.get(ent["status"], 0) + 1
            total += 1
        for r in rep.get(name, []):
            if r["status"] == "STRICT":
                strict_bytes += r["verified_bytes"]
    sym = load_json(CONFIG / "symbols.json").get("gog_v121_us", {})
    print(f"units: {len(units)}   functions recorded: {total}   " +
          "  ".join(f"{k}: {v}" for k, v in sorted(counts.items())))
    end = GAME_CODE_END["gog_v121_us"]
    print(f"STRICT code bytes (last verify): {strict_bytes} of ~{end} game code bytes in 1:00000-1:{end:05x} "
          f"({100.0 * strict_bytes / end:.2f}%)")
    # mapped = bytes of game _DATA covered by named bindings with a known size (claims, not reconstruction)
    import promote
    from image import parse_addr
    spans = []
    for n, e in sym.get("data", {}).items():
        o, a = parse_addr(e["addr"])
        size = promote.type_size(e.get("type")) or e.get("size")
        if o == 2 and size and a < GAME_DATA_END:
            spans.append((a, min(a + size, GAME_DATA_END)))
    mapped, last = 0, 0
    for a, b in sorted(spans):
        a = max(a, last)
        if b > a:
            mapped += b - a
            last = b
    print(f"data: reconstructed (VERIFIED segments, last verify) {data_b} bytes; mapped game _DATA (named+sized "
          f"bindings) {mapped} of {GAME_DATA_END} bytes ({100.0 * mapped / GAME_DATA_END:.1f}%)")
    print(f"bindings: {len(sym.get('functions', {}))} functions, {len(sym.get('data', {}))} data")
    data_units = [n for n, u in units.items() if not u["functions"]]
    for name, u in units.items():
        if not u["functions"]:
            continue
        st = {}
        for ent in u["functions"].values():
            st[ent["status"]] = st.get(ent["status"], 0) + 1
        segs = " ".join(f"{k}:{v}" for k, v in u.get("segments", {}).items())
        print(f"  {name:10s} {u['source']:18s} " + " ".join(f"{k}={v}" for k, v in sorted(st.items()))
              + (f"  [{segs}]" if segs else ""))
    print(f"  + {len(data_units)} data-only units (src/data_coll.c, src/data/*.c)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
