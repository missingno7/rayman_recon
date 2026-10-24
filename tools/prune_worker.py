"""Keep a worker directory small: deliverables stay, bulk moves to to_delete/ (the user empties it).

    python tools/prune_worker.py WORKER [WORKER ...]   prune build/workers/WORKER after review/integration
    python tools/prune_worker.py --check               disk gate before launching workers (exit 1 = do not launch)

Kept in build/workers/WORKER: top-level *.md, *.json and *.py files under 2 MB, and units/ (the delivered sources).
Everything else (trial copies, objects, logs, traces, permuter runs) moves to to_delete/WORKER/. Nothing is deleted.
Background (2026-10-24): ~650 worker directories kept every trial's unit copy, object, log and full match report and
filled the disk (tens of GB); docs/MATCHING.md "Disk budget".
"""
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WORKERS = ROOT / "build" / "workers"
TRASH = ROOT / "to_delete"
KEEP_SUFFIXES = {".md", ".json", ".py"}
KEEP_MAX = 2 << 20
MIN_FREE_GB = 50
MAX_BUILD_GB = 5


def size(path):
    return sum(p.stat().st_size for p in path.rglob("*") if p.is_file())


def prune(name):
    d = WORKERS / name
    if not d.is_dir():
        print(f"{name}: no such worker directory")
        return
    before = size(d)
    dest = TRASH / name
    for p in d.iterdir():
        keep = p.name == "units" or (p.is_file() and p.suffix in KEEP_SUFFIXES and p.stat().st_size <= KEEP_MAX)
        if keep:
            continue
        dest.mkdir(parents=True, exist_ok=True)
        target = dest / p.name
        if target.exists():
            target = dest / (p.name + ".dup")
        shutil.move(str(p), str(target))
    print(f"{name}: {before / 2**20:.1f} MB -> {size(d) / 2**20:.1f} MB kept")


def check():
    free = shutil.disk_usage(ROOT).free / 2**30
    build = size(ROOT / "build") / 2**30
    ok = free >= MIN_FREE_GB and build <= MAX_BUILD_GB
    print(f"free {free:.1f} GB (min {MIN_FREE_GB}), build/ {build:.2f} GB (max {MAX_BUILD_GB}): "
          f"{'OK' if ok else 'STOP - prune workers / empty to_delete first'}")
    return 0 if ok else 1


if __name__ == "__main__":
    args = sys.argv[1:]
    if args == ["--check"]:
        sys.exit(check())
    if not args:
        sys.exit(__doc__)
    for a in args:
        prune(a)
