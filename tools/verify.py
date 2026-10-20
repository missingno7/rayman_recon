"""Zero-regression gate: rebuild every accepted unit from scratch and verify it against its oracle.

    python tools/verify.py                 verify all units in config/units.json (exit 1 on any regression)
    python tools/verify.py UNIT...         only these units
    python tools/verify.py --accept UNIT   after a clean pass, record improved statuses for UNIT

Objects are compiled into a fresh build/verify/<unit>/ directory every run (no object cache). A function
regresses if its status ranks below the recorded one. The report is written to build/verify/report.json.
"""
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import match  # noqa: E402
import omf  # noqa: E402
import wcc  # noqa: E402
from common import BUILD, CONFIG, ROOT, dump_json, load_json  # noqa: E402
from image import Image  # noqa: E402

RANK = {"NOT_RECONSTRUCTED": 0, "UNPLACED": 0, "UNVERIFIED": 1, "NEAR_MATCH": 2, "BEHAVIOR_MATCH": 2,
        "CODE_BYTES_MATCH": 3, "STRICT": 4}


def ensure_prerequisites(oracle):
    """Regenerate build/oracles/<oracle>/ (extraction) and libsyms.json (library bindings) when missing."""
    import extract
    import libscan
    from common import TOOLS_DIR
    d = BUILD / "oracles" / oracle
    if not (d / "pmw1.json").exists():
        if extract.extract(oracle):
            raise SystemExit(f"extraction of {oracle} failed")
    if not (d / "libsyms.json").exists():
        libs = [str(TOOLS_DIR / p) for p in load_json(CONFIG / "oracles.json")["oracles"][oracle]["runtime_libs"]]
        libscan.main(["libscan.py", "--oracle", oracle, "--symbols"] + libs)


def verify_unit(name, unit, images):
    out = BUILD / "verify" / name
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    prof = wcc.profile(unit["profile"])
    obj = out / (Path(unit["source"]).stem + ".obj")
    rc, text = wcc.compile_c(ROOT / unit["source"], obj, prof["flags"], prof["install"])
    if rc:
        return None, f"compile failed:\n{text}"
    if unit["oracle"] not in images:
        ensure_prerequisites(unit["oracle"])
    img = images.setdefault(unit["oracle"], Image(unit["oracle"]))
    results = []
    segs = []
    for m in omf.load(obj):
        results += match.verify_object(img, m, None, segs)
    return (results, segs), None


def main(argv):
    units = load_json(CONFIG / "units.json")
    accept = "--accept" in argv
    names = [a for a in argv[1:] if not a.startswith("--")] or list(units["units"])
    import promote
    canon = load_json(CONFIG / "symbols.json")
    bad = promote.overlaps(canon) + promote.android_mismatches(canon)
    for b in bad:
        print(f"BINDING ERROR {b}")
    images = {}
    report = {}
    regressions = len(bad)
    strict_f = strict_b = data_b = bss_b = 0
    for name in names:
        unit = units["units"][name]
        out, err = verify_unit(name, unit, images)
        if err:
            print(f"[{name}] {err}")
            regressions += len(unit["functions"])
            continue
        results, segs = out
        seg_now = {g["segment"]: g for g in segs}
        for sname, want in unit.get("segments", {}).items():
            got_s = seg_now.get(sname, {}).get("status", "MISSING")
            if want == "VERIFIED" and got_s != "VERIFIED":
                regressions += 1
                print(f"[{name}] REGRESSION data segment {sname}: VERIFIED -> {got_s} {seg_now.get(sname, {}).get('problem', '')}")
        for sname, g in seg_now.items():
            if g["status"] == "VERIFIED":
                if g.get("layout_only"):
                    bss_b += g["size"]
                else:
                    data_b += g["size"]
                if unit.get("segments", {}).get(sname) != "VERIFIED":
                    print(f"[{name}] data segment {sname} VERIFIED" + (" (recorded)" if accept else " (not recorded)"))
                    if accept:
                        unit.setdefault("segments", {})[sname] = "VERIFIED"
        got = {r["function"]: r for r in results}
        report[name] = results
        report.setdefault("_segments", {})[name] = segs
        for fn, ent in unit["functions"].items():
            r = got.get(fn)
            st = r["status"] if r else "NOT_RECONSTRUCTED"
            if RANK[st] < RANK[ent["status"]]:
                regressions += 1
                print(f"[{name}] REGRESSION {fn}: {ent['status']} -> {st}")
                match.report([r] if r else [])
            elif RANK[st] > RANK[ent["status"]]:
                print(f"[{name}] improved {fn}: {ent['status']} -> {st}" + ("" if accept else " (not recorded)"))
                if accept:
                    ent["status"] = st
        for fn, r in got.items():
            if fn not in unit["functions"]:
                print(f"[{name}] new function {fn}: {r['status']}" + (" (recorded)" if accept else ""))
                if accept:
                    unit["functions"][fn] = {"status": r["status"], "provenance": []}
            if r["status"] == "STRICT":
                strict_f += 1
                strict_b += r["verified_bytes"]
        print(f"[{name}] " + ", ".join(f"{r['function']}={r['status']}" for r in results))
    dump_json(BUILD / "verify" / "report.json", report)
    if accept:
        if regressions:
            print("not accepting: regressions present")
        else:
            from common import dump_json as dj
            dj(CONFIG / "units.json", units)
            print("accepted statuses written to config/units.json")
    print(f"# STRICT functions {strict_f}, STRICT code bytes {strict_b}; VERIFIED data bytes {data_b}; VERIFIED BSS layout bytes {bss_b}; "
          f"regressions {regressions}")
    return 1 if regressions else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
