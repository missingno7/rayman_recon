"""Pin the historical functions that port-owned code mirrors line by line.

game/port_main.c follows src/pcmain.c main() and PcMain() call for call. If a matching improvement changes those
bodies, the mirror must be reviewed: `check` fails until `refresh` records the reviewed text.

    python port/tools/mirror.py check | refresh
"""
import hashlib
import os
import json
import re
import sys
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
ROOT = Path(os.environ["RAY_ROOT"]).resolve() if os.environ.get("RAY_ROOT") else PORT.parent
PIN = PORT / "game" / "mirror.json"
MIRRORS = {"game/port_main.c": [("src/pcmain.c", "main"), ("src/pcmain.c", "PcMain")]}


def body(path, name):
    text = (ROOT / path).read_text(encoding="latin-1").replace("\r\n", "\n")
    m = re.search(r"^[^\n;]*\b" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", text, re.M)
    if not m:
        raise SystemExit(f"{path}: function {name} not found")
    depth, i = 0, m.end() - 1
    while True:
        depth += {"{": 1, "}": -1}.get(text[i], 0)
        i += 1
        if depth == 0:
            return text[m.start():i]


def current():
    return {mirror: {f"{p}:{n}": hashlib.sha256(body(p, n).encode("latin-1")).hexdigest() for p, n in funcs}
            for mirror, funcs in MIRRORS.items()}


def main(argv):
    now = current()
    if len(argv) > 1 and argv[1] == "refresh":
        PIN.write_text(json.dumps(now, indent=1) + "\n")
        print("mirror pins refreshed")
        return 0
    pinned = json.loads(PIN.read_text()) if PIN.exists() else {}
    bad = [(m, f) for m, fs in now.items() for f, h in fs.items() if pinned.get(m, {}).get(f) != h]
    for m, f in bad:
        print(f"REVIEW {m}: {f} changed since the mirror was written; update {m}, then "
              f"`python port/tools/mirror.py refresh`")
    print(f"{sum(len(v) for v in now.values())} mirrored functions, {len(bad)} need review")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
