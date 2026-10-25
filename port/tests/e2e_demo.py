"""End-to-end regression gate: an original recorded demo through the whole native game.

Runs `rayman --demo N --headless --deterministic` (fixed epoch, fresh save directory) and compares the per-frame
image hash log with the reviewed golden summary in tests/golden/demo<N>.json. Every build architecture compares
against the same golden values, so the x86_64 build must reproduce the i686 reference (DOS pointer width) exactly.

    python port/tests/e2e_demo.py RAYMAN_EXE DATA_DIR DEMO [CD_CUE] [--update]
"""
import os
import collections
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
    args = [a for a in argv[1:] if a != "--update"]
    exe, data, demo = str(Path(args[0]).resolve()), args[1], int(args[2])
    cd = args[3] if len(args) > 3 else None
    update = "--update" in argv
    work = Path(tempfile.mkdtemp(prefix="rayman-e2e-"))
    try:
        log = work / "frames.txt"
        cmd = [exe, "--data", data, "--save", str(work / "save"), "--demo", str(demo), "--headless",
               "--deterministic", "--frames", str(FRAMES), "--frame-log", str(log)]
        if cd:
            cmd += ["--cd", cd]
        env = os.environ.copy()
        env["RAYMAN_SOUND_TRACE"] = str(work / "sound.txt")
        run = subprocess.run(cmd, env=env, capture_output=True, text=True, timeout=1800)
        sound = (work / "sound.txt").read_text() if (work / "sound.txt").exists() else ""
        cues = collections.Counter((int(v[3]), int(v[4])) for line in sound.splitlines()
                                   if (v := line.split()) and v[0] == "StartSample")
        audio = {"enabled": "CarteSonAutorisee=1" in run.stdout,
                 "starts": sum(cues.values()),
                 "cues": {f"{b}:{p}": n for (b,p),n in sorted(cues.items())},
                 "stop_calls": sum(l.startswith("StopSample ") for l in sound.splitlines()),
                 "game_exit": next((l for l in run.stdout.splitlines() if l.startswith("port exit:")), "")}
        evidence = os.environ.get("RAYMAN_E2E_EVIDENCE")
        if evidence:
            dest = Path(evidence); dest.mkdir(parents=True, exist_ok=True)
            (dest / f"demo{demo}.sound.txt").write_text(sound)
            (dest / f"demo{demo}.frames.txt").write_text(log.read_text() if log.exists() else "")
            (dest / f"demo{demo}.stdout.txt").write_text(run.stdout)
            (dest / f"demo{demo}.stderr.txt").write_text(run.stderr)
            (dest / f"demo{demo}.audio.json").write_text(json.dumps(audio, indent=1)+"\n")
        got = summary(log.read_text() if log.exists() else "")
        got["exit"] = run.returncode
    finally:
        shutil.rmtree(work, ignore_errors=True)
    path = GOLDEN / f"demo{demo}.json"
    print(f"demo{demo} HMI: {audio}")
    if not audio["enabled"] or not audio["starts"]:
        print("FAIL: expected enabled sound card and sample activity")
        print(run.stderr[-2000:])
        return 1
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
