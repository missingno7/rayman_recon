"""Gate: no CD at all (no image, no disc): the full game without CD music (game/cd_policy.c, user decision 2026-11-13).

The original stops at "Please insert Rayman's cdrom" (Escape quits, Space starts a limited mode that sets JeuCracker
and keeps the world map to the first three worlds). The port, with no CD source, skips that prompt and the
save-time disc check: a recorded demo runs to its frame limit with JeuCracker = 0 and the CD music off, and a new
game reaches a level of world 5.

    python port/tests/cd_missing_disc.py RAYMAN_EXE DATA_DIR SAVE_DIR
"""
import os
import shutil
import subprocess
import sys
from pathlib import Path

env = os.environ.copy()
for name in ("RAYMAN_CD", "RAYMAN_CD_TRACE", "RAYMAN_CD_DRIVE", "RAYMAN_START_LEVEL"):
    env.pop(name, None)
exe, data, save = str(Path(sys.argv[1]).resolve()), sys.argv[2], Path(sys.argv[3]).resolve()
keys = Path(__file__).resolve().parent / "keys" / "walk_right.txt"
problems = []


def run(args, extra_env=None):
    shutil.rmtree(save, ignore_errors=True)
    e = dict(env, **(extra_env or {}))
    result = subprocess.run([exe, "--data", data, "--save", str(save), "--headless", "--deterministic"] + args,
                            env=e, capture_output=True, timeout=600)
    return result.returncode, result.stdout.decode("latin-1") + result.stderr.decode("latin-1")


code, out = run(["--demo", "0", "--frames", "600"])
exit_line = next((l for l in out.splitlines() if l.startswith("port exit:")), "")
print("demo:", code, exit_line)
if code != 0 or "Please insert" in out:
    problems.append(f"demo 0 without a CD did not run (code {code}): {out[-400:]!r}")
if "JeuCracker=0" not in exit_line or "MusicCdActive=0" not in exit_line:
    problems.append(f"demo 0 without a CD: expected JeuCracker=0 MusicCdActive=0, got {exit_line!r}")

code, out = run(["--menus", "--keys", str(keys), "--frames", "4000"], {"RAYMAN_START_LEVEL": "5 1"})
exit_line = next((l for l in out.splitlines() if l.startswith("port exit:")), "")
print("world 5:", code, exit_line)
if code != 0 or "JeuCracker=0" not in exit_line or "world=5" not in exit_line:
    problems.append(f"a new game without a CD did not reach world 5 unrestricted: {exit_line!r}")
shutil.rmtree(save, ignore_errors=True)
for p in problems:
    print("FAIL:", p)
sys.exit(1 if problems else 0)
