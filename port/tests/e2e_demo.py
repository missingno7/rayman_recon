"""End-to-end regression gate: an original recorded demo through the whole native game.

Runs `rayman --demo N --headless --deterministic` (fixed epoch, fresh save directory) and compares the per-frame
image hash log with the reviewed golden summary in tests/golden/demo<N>.json. Every build architecture compares
against the same golden values, so the x86_64 build must reproduce the i686 reference (DOS pointer width) exactly.

    python port/tests/e2e_demo.py RAYMAN_EXE DATA_DIR DEMO [--update]
"""
import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

GOLDEN = Path(__file__).resolve().parent / "golden"
FRAMES = 6000


def summary(log_text):
    h = 1469598103934665603
    lines = [l for l in log_text.splitlines() if l.strip()]
    for line in lines:
        for b in line.encode():
            h = ((h ^ b) * 1099511628211) & 0xFFFFFFFFFFFFFFFF
    return {"frames": len(lines), "log_fnv": "%016x" % h, "last": lines[-1] if lines else ""}


def main(argv):
    exe, data, demo = argv[1], argv[2], int(argv[3])
    update = "--update" in argv
    work = Path(tempfile.mkdtemp(prefix="rayman-e2e-"))
    try:
        log = work / "frames.txt"
        cmd = [exe, "--data", data, "--save", str(work / "save"), "--demo", str(demo), "--headless",
               "--deterministic", "--frames", str(FRAMES), "--frame-log", str(log)]
        run = subprocess.run(cmd, capture_output=True, text=True, timeout=1800)
        got = summary(log.read_text() if log.exists() else "")
        got["exit"] = run.returncode
    finally:
        shutil.rmtree(work, ignore_errors=True)
    path = GOLDEN / f"demo{demo}.json"
    if update:
        GOLDEN.mkdir(exist_ok=True)
        path.write_text(json.dumps(got, indent=1) + "\n")
        print(f"golden demo{demo} written: {got}")
        return 0
    want = json.loads(path.read_text()) if path.exists() else None
    print(f"demo{demo}: {got}")
    if want != got:
        print(f"MISMATCH with {path}: expected {want}")
        if run.stderr:
            print(run.stderr[-2000:])
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
