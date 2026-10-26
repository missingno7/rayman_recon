"""Gate: the game's own configuration survives a restart.

Two consecutive headless sessions on one save directory, each walking the original menus into a new game with
scripted keys (tests/keys/new_game.txt). The first session writes RAYMAN.CFG through SaveOptionsOnDisk; the
second must load it (LoadOptionsOnDisk accepts exactly 0x84 bytes) and keep the sound card enabled, and the
saved sound-driver settings must equal those of the player's original RAYMAN.CFG.

    python port/tests/config_roundtrip.py RAYMAN_EXE DATA_DIR CD_CUE
"""
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

KEYS = Path(__file__).resolve().parent / "keys" / "new_game.txt"


def run(exe, data, cd, save):
    cmd = [exe, "--data", data, "--cd", cd, "--save", str(save), "--menus", "--headless", "--deterministic",
           "--frames", "4200", "--keys", str(KEYS)]
    out = subprocess.run(cmd, capture_output=True, text=True, timeout=600).stdout
    return next((l for l in out.splitlines() if l.startswith("port exit:")), "")


def main(argv):
    exe, data, cd = str(Path(argv[1]).resolve()), argv[2], argv[3]
    save = Path(tempfile.mkdtemp(prefix="rayman-cfg-"))
    try:
        first = run(exe, data, cd, save)
        cfg = save / "RAYMAN.CFG"
        written = cfg.read_bytes() if cfg.exists() else b""
        second = run(exe, data, cd, save)
    finally:
        shutil.rmtree(save, ignore_errors=True)
    original = (Path(data) / "RAYMAN.CFG").read_bytes()
    print("first: ", first)
    print("second:", second)
    print("RAYMAN.CFG bytes:", len(written))
    problems = []
    if len(written) != 0x84:
        problems.append(f"RAYMAN.CFG is {len(written)} bytes, the game only loads 0x84")
    if written[1:21] != original[1:21]:
        problems.append("saved sound-driver settings (port/irq/dma/param/device) differ from the original file")
    for name, line in (("first", first), ("second", second)):
        if "CarteSonAutorisee=1" not in line:
            problems.append(f"{name} session runs without the sound card: {line!r}")
    for p in problems:
        print("FAIL:", p)
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
