"""The original headless insert-CD prompt waits for input; terminate our own child after observing it."""
import os
import subprocess
import sys

env = os.environ.copy()
env.pop("RAYMAN_CD", None)
env.pop("RAYMAN_CD_TRACE", None)
env.pop("RAYMAN_CD_DRIVE", None)
command = [sys.argv[1], "--data", sys.argv[2], "--save", sys.argv[3],
           "--demo", "0", "--headless", "--deterministic", "--frames", "1"]
try:
    result = subprocess.run(command, env=env, capture_output=True, timeout=1)
except subprocess.TimeoutExpired as e:
    output = (e.stdout or b"").decode("latin-1")
    assert "CD ROM drive found on D:" in output, output
    assert "Please insert Rayman's cdrom." in output, output
    assert "limited mode, press SPACE" in output, output
    print("No image: original insert-CD / Escape / Space prompt observed; child stopped.")
else:
    raise AssertionError(f"Expected an input wait, got {result.returncode}: {result.stdout!r} {result.stderr!r}")
