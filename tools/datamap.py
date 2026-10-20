"""Data-object (obj2) layout evidence: segment-class regions and per-object CONST order.

    python tools/datamap.py [--oracle NAME]     print the map; write build/oracles/NAME/datamap.json

Evidence used:
  1. runtime anchors: _DATA/CONST/CONST2/DATA32 segments of the oracle's runtime libraries (config/oracles.json
     runtime_libs) found exactly once in obj2 (bytes equal modulo fixup fields) -> where each class's library part is;
  2. string-literal order: string literals referenced by exactly one code object (tools/tumap.py objects) are
     listed by address; wcc386 places literals in the object's own CONST segment, so their order shows whether
     CONST contributions follow the _TEXT link order;
  3. the initialised/BSS split from the PMW1 object table.
"""
import bisect
import collections
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import libscan  # noqa: E402
import omf  # noqa: E402
from common import BUILD, CONFIG, TOOLS_DIR, dump_json, load_json  # noqa: E402
from image import Image  # noqa: E402


def runtime_anchors(img, libs):
    d = img.obj[2]
    hits = []
    for lib in libs:
        for m in omf.load(lib):
            for si, s in enumerate(m.segments):
                if not s or s.cls.upper() in ("CODE", "BSS") or s.size < 8:
                    continue
                pat, mask, _ = libscan.masked_pattern(m, si)
                if sum(mask) < 8:
                    continue
                h = libscan.find_masked(d, pat, mask, min_anchor=6)
                if len(h) == 1:
                    hits.append({"at": h[0], "size": s.size, "segment": s.name, "module": m.name})
    return sorted(hits, key=lambda x: x["at"])


def string_runs(img, units, init_end):
    d = img.obj[2]
    starts = [int(u["start"].split(":")[1], 16) for u in units]
    refs = collections.defaultdict(set)
    for (o, s), (_, to, toff) in img.fixups.items():
        if o == 1 and to == 2 and 0 <= toff < init_end:
            refs[toff].add(bisect.bisect_right(starts, s) - 1)
    strs = sorted((a, next(iter(v))) for a, v in refs.items()
                  if len(v) == 1 and re.match(rb"[\x20-\x7e\t\r\n]{3,}\x00", d[a:a + 256]))
    runs = []
    for a, u in strs:
        if runs and runs[-1]["object"] == u:
            runs[-1]["end"] = a
            runs[-1]["n"] += 1
        else:
            runs.append({"start": a, "end": a, "object": u, "n": 1})
    inversions = sum(1 for i in range(1, len(strs)) if strs[i][1] < strs[i - 1][1])
    return runs, len(strs), inversions


def main(argv):
    name = argv[argv.index("--oracle") + 1] if "--oracle" in argv else None
    img = Image(name)
    o2 = next(o for o in img.meta["objects"] if o["index"] == 2)
    init_end = o2["data_size"]
    libs = [str(TOOLS_DIR / p) for p in load_json(CONFIG / "oracles.json")["oracles"][img.name]["runtime_libs"]]
    tp = BUILD / "oracles" / img.name / "tumap.json"
    if not tp.exists():
        raise SystemExit("run python tools/tumap.py first")
    units = load_json(tp)
    anchors = runtime_anchors(img, libs)
    runs, nstr, inv = string_runs(img, units, init_end)
    by_class = collections.OrderedDict()
    for a in anchors:
        r = by_class.setdefault(a["segment"], [a["at"], a["at"] + a["size"]])
        r[0], r[1] = min(r[0], a["at"]), max(r[1], a["at"] + a["size"])
    print(f"[{img.name}] obj2: initialised 2:00000..2:{init_end:05x}, BSS/stack 2:{init_end:05x}..2:{o2['virtual_size']:05x}")
    print("runtime data anchors (first..last runtime module of each segment class):")
    for seg, (lo, hi) in by_class.items():
        mods = [a["module"] for a in anchors if a["segment"] == seg]
        print(f"  {seg:7s} 2:{lo:05x}..2:{hi:05x}  {len(mods)} modules (first {mods[0]})")
    print(f"string literals referenced by one code object: {nstr}, order inversions vs _TEXT order: {inv}")
    for r in runs:
        files = ",".join(units[r["object"]]["rayverse_files"]) or units[r["object"]]["start"]
        print(f"  2:{r['start']:05x}..2:{r['end']:05x}  {r['n']:3d} strings  object {units[r['object']]['start']} {files}")
    dump_json(BUILD / "oracles" / img.name / "datamap.json",
              {"init_end": init_end, "anchors": anchors, "string_runs": runs, "string_inversions": inv})
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
