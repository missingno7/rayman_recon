"""64-bit ABI audit of the historical units as the port compiles them.

Every pointer<->integer conversion and incompatible pointer assignment the native compiler reports is a place
where the 32-bit DOS program may depend on its address size. Each site must carry a reviewed verdict in
port/docs/abi_sites.json; a new or changed site (e.g. after a matching improvement) fails the gate.

    python port/tools/abi_audit.py            # check against the reviewed list
    python port/tools/abi_audit.py --list     # print the current sites (for review)
"""
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
ROOT = Path(os.environ["RAY_ROOT"]).resolve() if os.environ.get("RAY_ROOT") else PORT.parent
sys.path.insert(0, str(PORT / "tools"))
import derive  # noqa: E402

REVIEW = PORT / "docs" / "abi_sites.json"
WARN = ["-Wpointer-to-int-cast", "-Wint-to-pointer-cast", "-Wincompatible-pointer-types", "-Wint-conversion"]


def units():
    cfg = json.loads((ROOT / "config" / "units.json").read_text(encoding="utf-8"))["units"]
    port = json.loads((PORT / "game" / "units.json").read_text(encoding="utf-8"))
    for v in cfg.values():
        if v.get("profile") == "game" and v["source"] not in port.get("replaced_units", {}):
            yield v["source"]


def sites():
    prov = derive.load()["units"]
    out = []
    with tempfile.TemporaryDirectory() as tmp:
        for source in units():
            rel = source[len("src/"):]
            if not rel.endswith(".c") or rel.startswith(("data/", "bss/")):
                continue
            path = ROOT / source
            if rel in prov:
                path = Path(tmp) / rel.replace("/", "_")
                path.write_text(derive.derived_text(rel, prov[rel]), encoding="latin-1")
            cmd = [os.environ.get("CC", "gcc"), "-fsyntax-only", "-std=gnu11", "-funsigned-char", "-Wno-unknown-pragmas",
                   *WARN, "-include", str(PORT / "compat/include/port_watcom.h")]
            protos = PORT / "game" / "protos" / (Path(rel).stem + ".h")
            if protos.exists():
                cmd += ["-include", str(protos)]
            cmd += ["-I", str(PORT / "compat/include"), "-I", str(ROOT / "src/include"), "-I", str(ROOT / "src"),
                    "-I", str(PORT), str(path)]
            err = subprocess.run(cmd, capture_output=True, text=True).stderr
            lines = path.read_text(encoding="latin-1").replace("\r\n", "\n").split("\n")
            for m in re.finditer(r":(\d+):\d+: warning: (.*?) \[(-W[\w-]+)\]", err):
                text = lines[int(m.group(1)) - 1].strip()
                out.append({"unit": source, "code": text, "warning": m.group(3)})
    uniq = {(s["unit"], s["code"], s["warning"]): s for s in out}
    return [uniq[k] for k in sorted(uniq)]


def main(argv):
    now = sites()
    if "--list" in argv:
        print(json.dumps(now, indent=1))
        return 0
    reviewed = json.loads(REVIEW.read_text(encoding="utf-8"))["sites"] if REVIEW.exists() else []
    known = {(s["unit"], s["code"], s["warning"]) for s in reviewed}
    new = [s for s in now if (s["unit"], s["code"], s["warning"]) not in known]
    for s in new:
        print(f"UNREVIEWED {s['unit']}: {s['warning']}: {s['code']}")
    print(f"{len(now)} ABI-sensitive sites, {len(new)} without a reviewed verdict")
    return 1 if new else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
