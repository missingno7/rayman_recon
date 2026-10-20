"""Progress summary derived from config/units.json and the last build/verify/report.json (run verify first).

    python tools/status.py
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, load_json  # noqa: E402

GAME_CODE_END = {"gog_v121_us": 0x6fbe6}   # first CLIB member (strupr) - game code + hand asm below it


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
    print(f"VERIFIED data segment bytes (last verify): {data_b}")
    print(f"bindings: {len(sym.get('functions', {}))} functions, {len(sym.get('data', {}))} data")
    for name, u in units.items():
        st = {}
        for ent in u["functions"].values():
            st[ent["status"]] = st.get(ent["status"], 0) + 1
        print(f"  {name:10s} {u['source']:18s} " + " ".join(f"{k}={v}" for k, v in sorted(st.items())))
    return 0


if __name__ == "__main__":
    sys.exit(main())
