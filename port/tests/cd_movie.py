"""Gate: the movies come from the inserted CD when the game folder has none (platform/files.c movie_from_cd).

A new game (tests/keys/start_game.txt) with the game folder and the CD image only: the intro starts after the menus
and decodes frames (RAYMAN_MOVIE_TRACE), its file copied from the image's RAYMAN folder into the save folder's
cd-cache at the size the disc's directory gives; a second run reuses that copy.

    python port/tests/cd_movie.py RAYMAN_EXE DATA_DIR CD_CUE
"""
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

exe, data, cue = str(Path(sys.argv[1]).resolve()), sys.argv[2], sys.argv[3]
keys = Path(__file__).resolve().parent / "keys" / "start_game.txt"
work = Path(tempfile.mkdtemp(prefix="rayman-cdmovie-"))
problems = []
try:
    if any(p.name.lower() in ("intro.dat", "conclu.dat") for p in Path(data).iterdir()):
        print("the game folder has movies of its own: nothing to test")
        sys.exit(0)
    stamps = []
    for run in (1, 2):
        trace = work / f"movie{run}.txt"
        env = dict(os.environ, RAYMAN_MOVIE_TRACE=str(trace))
        env.pop("RAYMAN_START_LEVEL", None)
        subprocess.run([exe, "--data", data, "--cd", cue, "--save", str(work / "save"), "--menus", "--keys", str(keys),
                        "--headless", "--deterministic", "--frames", "3700"], env=env, capture_output=True, timeout=900)
        lines = trace.read_text().splitlines() if trace.exists() else []
        frames = sum(1 for line in lines if line.startswith("frame "))
        cached = work / "save" / "cd-cache" / "INTRO.DAT"
        size = cached.stat().st_size if cached.exists() else 0
        stamps.append(cached.stat().st_mtime_ns if cached.exists() else 0)
        print(f"run {run}: intro begun={any(l.startswith('begin ') and 'intro.dat' in l for l in lines)} "
              f"frames={frames} cached={size}")
        if frames < 100 or size < 1000000:
            problems.append(f"run {run}: the intro did not play from the CD image (frames {frames}, cache {size})")
    if stamps[0] != stamps[1]:
        problems.append("the second run copied the movie again instead of using the cache")
finally:
    shutil.rmtree(work, ignore_errors=True)
for p in problems:
    print("FAIL:", p)
sys.exit(1 if problems else 0)
