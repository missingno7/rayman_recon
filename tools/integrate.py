"""Integrator: accept a worker's candidate units in one checked step (fails closed; never commits on failure).

    python tools/integrate.py WORKER_DIR UNIT=FILE.c [UNIT=FILE.c ...] [--new] [--rename OLD=NEW]... -m "message"

For each UNIT: copies WORKER_DIR/FILE.c to the unit's source (config/units.json; with --new, src/UNIT.c) and any
worker-local headers it includes that are not shared headers, merges the worker overlay (tools/promote.py),
registers the unit, then runs the full gate (tools/verify.py exit 0 and "regressions 0", unit tests).

Safety (fail closed):
- refuses to start if `git status --porcelain -- src config` is not empty;
- a worker name proposed at an address that is already bound under another name is refused; `--rename OLD=NEW`
  (repeat the flag per rename) states that they are the same symbol: OLD must be claimed in the overlay at NEW's
  address and must not be claimed anywhere else; it is rewritten to NEW in the sources and recorded as an alias of
  NEW, so a later claim of OLD at a different address conflicts (tools/promote.py claim index);
- unit names are identifiers; every destination must resolve inside src/; one physical destination per run;
- refuses to overwrite an existing different local header;
- every exit code (promote, verify, tests, git add/commit) is checked;
- on failure only the files this run wrote are restored and unstaged (no git clean); the commit contains only those
  files (literal pathspecs). Generated build/ outputs (verify reports) are not restored: rerun tools/verify.py.
"""
import argparse
import os
import re
import shutil
import stat
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from common import dump_json, load_json  # noqa: E402
from image import parse_addr  # noqa: E402

SHARED = {p.name for p in (ROOT / "src" / "include").glob("*.h")}
IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
FILENAME = re.compile(r"^[A-Za-z0-9_][A-Za-z0-9_.-]*\.(c|h|asm)$")


def run(*cmd):
    r = subprocess.run(list(cmd), cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    return r.returncode, r.stdout + r.stderr


def git(*args):
    return run("git", "--literal-pathspecs", *args)


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
    """Explicit --rename OLD=NEW: validate against the overlay claim and the canonical claim index, rewrite OLD in the
    sources, drop it from the overlay, record it as an alias of NEW."""
    import promote
    ov = load_json(overlay)
    cfg_all = load_json(cfg_path)
    cfg = cfg_all["gog_v121_us"]
    used = set()
    for sec in ("functions", "data"):
        secd = ov.get("gog_v121_us", {}).get(sec, {})
        claims = promote.claim_index(cfg[sec])
        for old, new in renames.items():
            if old not in secd:
                continue
            used.add(old)
            addr = promote.norm(secd[old]["addr"])
            if new not in cfg[sec] or promote.norm(cfg[sec][new]["addr"]) != addr:
                raise SystemExit(f"--rename {old}={new}: {new} is not bound at {secd[old]['addr']}")
            if old in claims and claims[old] != (new, addr):
                raise SystemExit(f"--rename {old}={new}: {old} is already claimed by {claims[old][0]} at "
                                 f"{claims[old][1]}")
            al = cfg[sec][new].setdefault("aliases", [])
            if old not in al:
                al.append(old)
            del secd[old]
    unused = set(renames) - used
    if unused:
        raise SystemExit(f"--rename without a matching overlay claim: {sorted(unused)}")
    for p in sources:
        t = p.read_text(encoding="utf-8")
        for old, new in renames.items():
            # also the Watcom-decorated spellings in assembly sources: _data and code_
            t = re.sub(r"(?<![\w@])(_?)%s(_?)(?![\w@])" % re.escape(old), lambda m: m.group(1) + new + m.group(2), t)
        p.write_text(t, encoding="utf-8", newline="\n")
    dump_json(overlay, ov)
    dump_json(cfg_path, cfg_all)


class Writes:
    """Journal of every destination this run writes (resolved path -> original bytes or None), restored on failure."""

    def __init__(self):
        self.orig = {}

    def touch(self, path):
        path = Path(os.path.realpath(path))
        if path not in self.orig:
            self.orig[path] = path.read_bytes() if path.exists() else None
            if path.exists():
                os.chmod(path, os.stat(path).st_mode | stat.S_IWRITE)
        return path

    def restore(self):
        errors = []
        for p, data in self.orig.items():
            try:
                if p.exists():
                    os.chmod(p, os.stat(p).st_mode | stat.S_IWRITE)
                if data is None:
                    p.unlink(missing_ok=True)
                else:
                    p.write_bytes(data)
            except OSError as e:
                errors.append(f"{p}: {e}")
        return errors

    def paths(self):
        return [str(p.relative_to(Path(os.path.realpath(ROOT)))) for p in self.orig]


def copy_into(w, src, dst):
    """Copy contents only (never the worker file's mode, e.g. read-only)."""
    dst = w.touch(dst)
    shutil.copyfile(src, dst)
    return dst


def inside(path, base):
    p, b = os.path.realpath(path), os.path.realpath(base)
    return os.path.commonpath([p, b]) == b


def parse(argv):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0], allow_abbrev=False)
    ap.add_argument("worker_dir")
    ap.add_argument("pairs", nargs="+", metavar="UNIT=FILE.c")
    ap.add_argument("--new", action="store_true")
    ap.add_argument("--rename", action="append", default=[], metavar="OLD=NEW")
    ap.add_argument("-m", dest="msg", required=True)
    a = ap.parse_args(argv)
    renames = {}
    for r in a.rename:
        old, sep, new = r.partition("=")
        if not sep or not old or not new:
            ap.error(f"bad --rename {r!r}")
        renames[old] = new
    pairs = []
    for p in a.pairs:
        unit, sep, fname = p.partition("=")
        if not sep or not IDENT.match(unit) or not FILENAME.match(fname):
            ap.error(f"bad UNIT=FILE pair {p!r} (identifier unit, plain .c/.asm file name)")
        pairs.append((unit, fname))
    return a, pairs, renames


def main(argv):
    a, pairs, renames = parse(argv[1:])
    wdir = ROOT / a.worker_dir
    code, out = git("status", "--porcelain", "--", "src", "config")
    if code or out.strip():
        raise SystemExit("src/ or config/ has uncommitted or untracked changes; commit or move them first:\n" + out)
    code, head0 = git("rev-parse", "HEAD")
    if code:
        raise SystemExit("cannot read HEAD:\n" + head0)
    head0 = head0.strip()
    (ROOT / "build" / "tmp").mkdir(parents=True, exist_ok=True)
    units = load_json(ROOT / "config" / "units.json")["units"]
    cfg_path = ROOT / "config" / "symbols.json"
    src_dir = ROOT / "src"
    # preflight: destinations resolved, contained, distinct; candidates exist
    plan, seen = [], set()
    for unit, fname in pairs:
        if a.new and unit in units:
            raise SystemExit(f"--new: unit {unit} already exists")
        if not a.new and unit not in units:
            raise SystemExit(f"unknown unit {unit} (use --new for a new unit)")
        dst = ROOT / (f"src/{unit}{Path(fname).suffix}" if a.new else units[unit]["source"])
        if not a.new and Path(fname).suffix != dst.suffix:
            raise SystemExit(f"{unit}: candidate {fname} and unit source {dst.name} differ in language")
        if not inside(dst, src_dir) or not (wdir / fname).is_file():
            raise SystemExit(f"{unit}: destination {dst} outside src/ or candidate {wdir / fname} missing")
        ids = {os.path.normcase(os.path.realpath(dst))}
        if dst.exists():                                  # hard links: same file under another name
            st = os.stat(dst)
            ids.add((st.st_dev, st.st_ino))
        if ids & seen:
            raise SystemExit(f"{unit}: destination {dst} already written by another pair in this run")
        seen |= ids
        plan.append((unit, wdir / fname, dst))
    w = Writes()
    ovtmp = None
    committed = attempted = False
    try:
        targets = []
        for unit, src, dst in plan:
            dst = copy_into(w, src, dst)
            for h in re.findall(r'#include "([^"]+)"', dst.read_text(encoding="utf-8")):
                hsrc, hdst = wdir / h, src_dir / Path(h).name
                if h in SHARED or not hsrc.is_file():
                    continue
                if hdst.exists():
                    if hdst.read_bytes() == hsrc.read_bytes():
                        continue
                    users = {os.path.normcase(os.path.realpath(p)) for p in src_dir.glob("*.c")
                             if f'#include "{hdst.name}"' in p.read_text(encoding="utf-8", errors="replace")}
                    mine = {os.path.normcase(os.path.realpath(d)) for _, _, d in plan}
                    if not users or not users <= mine:
                        raise SystemExit(f"local header {hdst.name} exists in src/ with different content and is "
                                         f"included by units outside this run; reconcile it first")
                    copy_into(w, hsrc, hdst)                  # private to the units replaced in this run
                    print(f"  local header {h} updated (used only by this run's units)")
                    continue
                copy_into(w, hsrc, hdst)
                print(f"  local header {h} -> src/")
            targets.append((unit, dst))
        overlay = wdir / "symbols.json"
        if overlay.exists() or renames:
            if not overlay.exists():
                raise SystemExit("--rename given but the worker has no symbols.json overlay")
            fd, name = tempfile.mkstemp(suffix=".json", dir=ROOT / "build" / "tmp")
            os.close(fd)
            ovtmp = Path(name)
            shutil.copyfile(overlay, ovtmp)               # never modify the worker's own overlay
            w.touch(cfg_path)
            if renames:
                apply_renames(ovtmp, [d for _, d in targets], renames, cfg_path)
            col = collisions(ovtmp, cfg_path)
            if col:
                raise SystemExit("worker names at already-bound addresses (decide; pass --rename OLD=NEW only if they "
                                 "are the same symbol, with independent evidence):\n" +
                                 "\n".join(f"  {s}: {k} -> bound as {e}" for s, k, e in col))
            code, out = run(sys.executable, "tools/promote.py", str(ovtmp))
            print("  " + (out.strip().splitlines() or ["(no output)"])[-1])
            if code:
                raise SystemExit("promote (bindings) failed:\n" + out[-1500:])
        w.touch(ROOT / "config" / "units.json")
        for unit, dst in targets:
            code, out = run(sys.executable, "tools/promote.py", "--unit", unit, str(dst.relative_to(
                Path(os.path.realpath(ROOT)))))
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
        paths = w.paths()
        code, out = git("add", "--", *paths)
        if code:
            raise SystemExit("git add failed:\n" + out)
        if not git("diff", "--cached", "--quiet", "--", *paths)[0]:
            print("  no changes: the candidate equals the accepted state; nothing to commit")
            committed = True
            return 0
        attempted = True
        code, out = git("commit", "-q", "-m", a.msg + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>",
                        "--", *paths)
        if code:
            raise SystemExit("git commit failed:\n" + out)
        committed = True
        print("  committed")
    except BaseException as e:
        if attempted and not committed:                   # interrupted while git commit ran: did it land?
            try:
                c1, head1 = git("rev-parse", "HEAD")
                c2, parent = git("rev-parse", "HEAD^")
                committed = (not c1 and not c2 and head1.strip() != head0 and parent.strip() == head0
                             and not git("diff", "--quiet", "HEAD", "--", *w.paths())[0])
            except BaseException:
                committed = False
            if committed:
                print("  committed (interrupted afterwards)")
                return 0
        if committed:
            return 0
        errors = w.restore()
        if w.orig:                                        # never an unrestricted reset
            code, out = git("reset", "-q", "--", *w.paths())
            if code:
                errors.append("git reset of the written paths failed: " + out.strip())
        print(e if isinstance(e, SystemExit) else repr(e))
        if errors:
            print("RESTORE ERRORS:\n  " + "\n  ".join(errors))
        raise SystemExit("GATE FAILED - files written by this run restored and unstaged, nothing committed")
    finally:
        if ovtmp is not None:
            ovtmp.unlink(missing_ok=True)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
