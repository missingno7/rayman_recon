"""Integrator: accept a worker's candidate units in one checked step (fails closed; never commits on failure).

    python tools/integrate.py WORKER_DIR UNIT=FILE.c [UNIT=FILE.c ...] [--new] [--rename OLD=NEW ...] -m "message"

For each UNIT: copies WORKER_DIR/FILE.c to the unit's source (config/units.json; with --new, src/UNIT.c) and any
worker-local headers it includes that are not shared headers, merges the worker overlay (tools/promote.py),
registers the unit, then runs the full gate (tools/verify.py exit 0 and "regressions 0", unit tests).

Safety (fail closed):
- refuses to start if `git status --porcelain -- src config` is not empty;
- a worker name proposed at an address that is already bound under another name is refused; `--rename OLD=NEW`
  states that they are the same symbol: OLD is rewritten to NEW in the sources and recorded as an alias of NEW, so
  a later claim of OLD at a different address conflicts;
- refuses to overwrite an existing different local header;
- every exit code (promote, verify, tests) is checked;
- on failure only the files this run wrote are restored (no git clean); the commit contains only those files.
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


def collisions(overlay, cfg_path):
    """Worker names (not bound yet) whose address is already bound under another name: [(sec, name, existing)]."""
    if not overlay.exists():
        return []
    ov = load_json(overlay)
    cfg = load_json(cfg_path)["gog_v121_us"]
    out = []
    for sec in ("functions", "data"):
        by = {}
        for k, v in cfg[sec].items():
            by.setdefault(parse_addr(v["addr"]), []).append(k)
        for k, v in ov.get("gog_v121_us", {}).get(sec, {}).items():
            names = by.get(parse_addr(v["addr"]))
            if k not in cfg[sec] and names:
                out.append((sec, k, names[0]))
    return out


def apply_renames(overlay, sources, renames, cfg_path):
    """Explicit --rename OLD=NEW: rewrite OLD in the sources, drop it from the overlay, record it as an alias of NEW."""
    ov = load_json(overlay)
    cfg_all = load_json(cfg_path)
    cfg = cfg_all["gog_v121_us"]
    for p in sources:
        t = p.read_text(encoding="utf-8")
        for old, new in renames.items():
            t = re.sub(r"(?<![\w@])%s(?![\w@])" % re.escape(old), new, t)
        p.write_text(t, encoding="utf-8", newline="\n")
    for sec in ("functions", "data"):
        secd = ov.get("gog_v121_us", {}).get(sec, {})
        for old, new in renames.items():
            if old not in secd:
                continue
            if new not in cfg[sec] or parse_addr(cfg[sec][new]["addr"]) != parse_addr(secd[old]["addr"]):
                raise SystemExit(f"--rename {old}={new}: {new} is not bound at {secd[old]['addr']}")
            al = cfg[sec][new].setdefault("aliases", [])
            if old not in al:
                al.append(old)
            del secd[old]
    dump_json(overlay, ov)
    dump_json(cfg_path, cfg_all)


class Writes:
    """Remembers the original content of every path this run writes, to restore exactly those on failure."""

    def __init__(self):
        self.orig = {}

    def touch(self, path):
        path = Path(path)
        if path not in self.orig:
            self.orig[path] = path.read_bytes() if path.exists() else None

    def restore(self):
        for p, data in self.orig.items():
            if data is None:
                p.unlink(missing_ok=True)
            else:
                p.write_bytes(data)

    def paths(self):
        return [str(p.relative_to(ROOT)) for p in self.orig]


def main(argv):
    msg = argv[argv.index("-m") + 1] if "-m" in argv else None
    new = "--new" in argv
    renames, args, skip = {}, [], False
    for i, a in enumerate(argv[1:], 1):
        if skip:
            skip = False
            continue
        if a in ("-m", "--rename"):
            skip = a == "-m"
            if a == "--rename":
                k, v = argv[i + 1].split("=", 1)
                renames[k] = v
                skip = True
            continue
        if not a.startswith("-"):
            args.append(a)
    if not msg or len(args) < 2:
        raise SystemExit(__doc__)
    wdir, pairs = ROOT / args[0], [a.split("=", 1) for a in args[1:]]
    code, out = run("git", "status", "--porcelain", "--", "src", "config")
    if code or out.strip():
        raise SystemExit("src/ or config/ has uncommitted or untracked changes; commit or move them first:\n" + out)
    units = load_json(ROOT / "config" / "units.json")["units"]
    cfg_path = ROOT / "config" / "symbols.json"
    overlay = wdir / "symbols.json"
    w = Writes()
    try:
        targets = []
        for unit, fname in pairs:
            dst = ROOT / (units[unit]["source"] if unit in units and not new else f"src/{unit}.c")
            if new and unit in units:
                raise SystemExit(f"--new: unit {unit} already exists")
            w.touch(dst)
            shutil.copy(wdir / fname, dst)
            for h in re.findall(r'#include "([^"]+)"', dst.read_text(encoding="utf-8")):
                hsrc, hdst = wdir / h, ROOT / "src" / Path(h).name
                if h in SHARED or not hsrc.exists():
                    continue
                if hdst.exists():
                    if hdst.read_bytes() != hsrc.read_bytes():
                        raise SystemExit(f"local header {hdst.name} exists in src/ with different content; "
                                         "reconcile it first (it may belong to another unit)")
                    continue
                w.touch(hdst)
                shutil.copy(hsrc, hdst)
                print(f"  local header {h} -> src/")
            targets.append((unit, dst))
        if overlay.exists():
            ovcopy = wdir / "symbols.integrate.json"      # never modify the worker's own overlay
            shutil.copy(overlay, ovcopy)
            w.touch(cfg_path)
            if renames:
                apply_renames(ovcopy, [d for _, d in targets], renames, cfg_path)
            col = collisions(ovcopy, cfg_path)
            if col:
                raise SystemExit("worker names at already-bound addresses (decide; pass --rename OLD=NEW only if they "
                                 "are the same symbol, with independent evidence):\n" +
                                 "\n".join(f"  {s}: {k} -> bound as {e}" for s, k, e in col))
            code, out = run(sys.executable, "tools/promote.py", str(ovcopy))
            ovcopy.unlink()
            print("  " + (out.strip().splitlines() or ["(no output)"])[-1])
            if code:
                raise SystemExit("promote (bindings) failed:\n" + out[-1500:])
        w.touch(ROOT / "config" / "units.json")
        for unit, dst in targets:
            code, out = run(sys.executable, "tools/promote.py", "--unit", unit, str(dst.relative_to(ROOT)))
            print(f"  {unit}: " + (out.strip().splitlines() or ["(no output)"])[-1])
            if code:
                raise SystemExit(f"promote --unit {unit} failed (exit {code}):\n" + out[-1500:])
        code, out = run(sys.executable, "tools/verify.py")
        last = (out.strip().splitlines() or [""])[-1]
        print("  " + last)
        if code or not last.endswith("regressions 0"):
            raise SystemExit(f"verify failed (exit {code})")
        code, out = run(sys.executable, "-m", "unittest", "discover", "-s", "tests")
        if code:
            raise SystemExit("unit tests failed:\n" + out[-1500:])
    except BaseException as e:
        w.restore()
        print(e if isinstance(e, SystemExit) else repr(e))
        raise SystemExit("GATE FAILED - files written by this run restored, nothing committed")
    paths = w.paths()
    code, out = run("git", "add", "--", *paths)
    if not code:
        code, out = run("git", "commit", "-q", "-m",
                        msg + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>", "--", *paths)
    print("  committed" if code == 0 else out)
    return code


if __name__ == "__main__":
    sys.exit(main(sys.argv))
