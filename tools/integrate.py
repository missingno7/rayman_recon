"""Integrator: accept a worker's candidate units in one checked step (never commits on failure).

    python tools/integrate.py WORKER_DIR UNIT=FILE.c [UNIT=FILE.c ...] [--new] -m "commit message"

For each UNIT: copies WORKER_DIR/FILE.c to the unit's source (config/units.json; with --new, src/UNIT.c), copies
any worker-local headers it includes that are not shared headers, maps worker names that duplicate existing
bindings onto those bindings (never touching names already bound - compiler aliases share addresses), merges the
worker overlay (tools/promote.py), registers the unit, then runs the full gate (tools/verify.py: 0 regressions,
unit tests). Only if everything passes is a git commit made; otherwise the working tree is restored.
"""
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from common import dump_json, load_json  # noqa: E402
from image import parse_addr  # noqa: E402

SHARED = {p.name for p in (ROOT / "src" / "include").glob("*.h")}


def run(*cmd):
    r = subprocess.run(list(cmd), cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    return r.returncode, r.stdout + r.stderr


def dedupe(overlay, sources):
    if not overlay.exists():
        return
    ov = load_json(overlay)
    cfg = load_json(ROOT / "config" / "symbols.json")["gog_v121_us"]
    texts = {p: p.read_text(encoding="utf-8") for p in sources}
    for sec in ("functions", "data"):
        by = {}
        for k, v in cfg[sec].items():
            by.setdefault(parse_addr(v["addr"]), []).append(k)
        for k, v in list(ov.get("gog_v121_us", {}).get(sec, {}).items()):
            names = by.get(parse_addr(v["addr"]))
            if k in cfg[sec] or not names:
                continue
            print(f"  name {k} -> existing binding {names[0]}")
            for p in texts:
                texts[p] = re.sub(r"\b%s\b" % re.escape(k), names[0], texts[p])
            del ov["gog_v121_us"][sec][k]
    for p, t in texts.items():
        p.write_text(t, encoding="utf-8", newline="\n")
    dump_json(overlay, ov)


def main(argv):
    msg = argv[argv.index("-m") + 1] if "-m" in argv else None
    new = "--new" in argv
    args = [a for i, a in enumerate(argv[1:], 1) if not a.startswith("-") and (i < 2 or argv[i - 1] != "-m")]
    wdir, pairs = ROOT / args[0], [a.split("=", 1) for a in args[1:]]
    if not msg or not pairs:
        raise SystemExit(__doc__)
    units = load_json(ROOT / "config" / "units.json")["units"]
    code, _ = run("git", "diff", "--quiet")
    if code:
        raise SystemExit("working tree has uncommitted changes; commit or stash first")
    targets = []
    for unit, fname in pairs:
        dst = ROOT / (units[unit]["source"] if unit in units and not new else f"src/{unit}.c")
        shutil.copy(wdir / fname, dst)
        for h in re.findall(r'#include "([^"]+)"', dst.read_text(encoding="utf-8")):
            if h not in SHARED and (wdir / h).exists():
                shutil.copy(wdir / h, ROOT / "src" / Path(h).name)
                print(f"  local header {h} -> src/")
        targets.append((unit, dst))
    overlay = wdir / "symbols.json"
    dedupe(overlay, [d for _, d in targets])
    ok = True
    if overlay.exists():
        code, out = run(sys.executable, "tools/promote.py", str(overlay))
        print("  " + out.strip().splitlines()[-1])
        ok = code == 0
    for unit, dst in targets if ok else []:
        code, out = run(sys.executable, "tools/promote.py", "--unit", unit, str(dst.relative_to(ROOT)))
        print(f"  {unit}: " + out.strip().splitlines()[-1])
    if ok:
        code, out = run(sys.executable, "tools/verify.py")
        last = out.strip().splitlines()[-1]
        print("  " + last)
        ok = last.endswith("regressions 0")
    if ok:
        code, out = run(sys.executable, "-m", "unittest", "discover", "-s", "tests")
        ok = code == 0
        if not ok:
            print(out[-800:])
    if not ok:
        run("git", "checkout", "--", "src", "config")
        run("git", "clean", "-fdq", "--", "src")
        raise SystemExit("GATE FAILED - working tree restored, nothing committed")
    run("git", "add", "-A", "src", "config")
    code, out = run("git", "commit", "-q", "-m", msg + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
    print("  committed" if code == 0 else out)


if __name__ == "__main__":
    main(sys.argv)
