"""Deterministic extraction of a pinned DOS oracle into build/oracles/<name>/ and its self-checks.

    python tools/extract.py [NAME|--all]   default: the primary oracle; fails closed on any problem

Checks performed (all must pass):
  1. input RAYMAN.EXE matches config/oracles.json;
  2. every compressed object and relocation block decodes to exactly its declared size and consumes
     exactly its declared compressed size (tools/pmw1.py);
  3. decoded object hashes match the pinned hashes;
  4. cross-stream consistency: the 32-bit value stored at every fixup site in the decoded objects equals
     the target offset from the independently compressed relocation stream (mod 2^32);
  5. duplicate fixup sites are identical and lie across a 4 KiB page boundary (LE page-split fixups);
  6. every fixup target lies inside its target object's virtual size (negative offsets reported).
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, load_json, oracle  # noqa: E402
import pmw1  # noqa: E402


def extract(name):
    cfg = load_json(CONFIG / "oracles.json")["oracles"][name]
    exe = oracle(name)
    p = pmw1.PMW1(exe)
    out = BUILD / "oracles" / name
    out.mkdir(parents=True, exist_ok=True)
    meta = {"oracle": name, "exe_sha256": cfg["sha256"], "pmw1_offset": p.base, "version": p.version_text(), "flags": p.flags,
            "cs_obj": p.cs_obj, "eip": p.eip, "ss_obj": p.ss_obj, "esp": p.esp, "objects": []}
    fx, spans = p.fixups()
    objs = {}
    for idx, data, span in p.object_data():
        want = cfg["objects"][str(idx)]["sha256"]
        got = pmw1.sha256_bytes(data)
        if got != want:
            raise SystemExit(f"obj{idx}: decoded sha256 {got} != pinned {want}")
        objs[idx] = data
        (out / f"obj{idx}.bin").write_bytes(data)
        o = dict(p.objects[idx - 1])
        o.update({"index": idx, "file_span": list(span), "sha256": got,
                  "reloc_blocks": [list(b) for b in spans[idx]]})
        meta["objects"].append(o)
    vs = {i + 1: o["virtual_size"] for i, o in enumerate(p.objects)}
    problems = 0
    negative = 0
    for k, items in fx.items():
        seen = {}
        for it in items:
            v = struct.unpack_from("<I", objs[k], it["src"])[0]
            if v != it["toff"] & 0xFFFFFFFF:
                problems += 1
                print(f"  value mismatch at {k}:{it['src']:x}: stored {v:x} target {it['toff'] & 0xFFFFFFFF:x}")
            if it["toff"] < 0:
                negative += 1
            elif it["toff"] > vs[it["tobj"]]:
                problems += 1
                print(f"  target outside object at {k}:{it['src']:x} -> {it['tobj']}:{it['toff']:x}")
            key = it["src"]
            if key in seen:
                if seen[key] != it or key % 4096 <= 4092:
                    problems += 1
                    print(f"  unexplained duplicate fixup at {k}:{key:x}")
            seen[key] = it
        if len(items) != cfg["objects"][str(k)]["fixups"]:
            problems += 1
            print(f"  obj{k}: {len(items)} fixup records, pinned {cfg['objects'][str(k)]['fixups']}")
    pmw1.dump_json(out / "pmw1.json", meta)
    pmw1.dump_json(out / "fixups.json", {str(k): v for k, v in fx.items()})
    total = sum(len(v) for v in fx.values())
    print(f"[{name}] PMW1 v{p.version_text()} entry {p.cs_obj}:{p.eip:x}; objects " +
          ", ".join(f"obj{k} {len(v)} bytes" for k, v in objs.items()))
    print(f"fixup records {total}; negative-offset targets {negative}; consistency problems {problems}")
    if problems:
        print("EXTRACTION FAILED")
        return 1
    print(f"EXTRACTION OK -> build/oracles/{name}/")
    return 0


def main(argv):
    cfg = load_json(CONFIG / "oracles.json")
    if "--all" in argv:
        names = [k for k, v in cfg["oracles"].items() if "pmw1" in v]
    else:
        names = [argv[1] if len(argv) > 1 else cfg["primary"]]
    return max(extract(n) for n in names)


if __name__ == "__main__":
    sys.exit(main(sys.argv))
