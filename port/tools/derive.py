"""Port derivatives of historical translation units, kept as patches against ../src.

A historical unit that cannot be compiled natively unchanged is never copied by hand. Its port form is
src/<unit>.c plus a reviewed unified diff in port/derive/<unit>.patch; port/derive/provenance.json records the
base hash, the reason and every semantic difference. The build applies the patch to the CURRENT src file, so
later matching improvements flow into the port automatically; a changed base is reported by `check` (and by
the ctest gate) until a human reviews the change and runs `refresh`.

    python port/tools/derive.py apply --out DIR      # write DIR/<unit>.c for every derivative (build step)
    python port/tools/derive.py check                # patches apply and bases are the reviewed ones
    python port/tools/derive.py edit UNIT            # patched working copy in build/port-derive/UNIT
    python port/tools/derive.py save UNIT            # regenerate the patch from the working copy
    python port/tools/derive.py refresh UNIT         # accept a reviewed base change (patch still applies)
"""
import difflib
import hashlib
import os
import json
import re
import sys
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
ROOT = Path(os.environ["RAY_ROOT"]).resolve() if os.environ.get("RAY_ROOT") else PORT.parent
SRC = ROOT / "src"
DERIVE = PORT / "derive"
PROV = DERIVE / "provenance.json"
WORK = Path(os.environ["PORT_DERIVE_WORK"]) if os.environ.get("PORT_DERIVE_WORK") else ROOT / "build" / "port-derive"


def text(p):
    return Path(p).read_text(encoding="latin-1").replace("\r\n", "\n")


def sha(s):
    return hashlib.sha256(s.encode("latin-1")).hexdigest()


def load():
    return json.loads(PROV.read_text(encoding="utf-8")) if PROV.exists() else {"schema": 1, "units": {}}


def store(prov):
    PROV.write_text(json.dumps(prov, indent=1) + "\n", encoding="utf-8")


def hunks(patch):
    """Parse a unified diff into [(old_start, [(tag, line)])]."""
    out, cur = [], None
    for line in patch.split("\n"):
        m = re.match(r"@@ -(\d+)(?:,\d+)? \+\d+(?:,\d+)? @@", line)
        if m:
            cur = (int(m.group(1)), [])
            out.append(cur)
        elif cur is not None and line[:1] in (" ", "-", "+"):
            cur[1].append((line[0], line[1:]))
    return out


def apply(base, patch):
    """Apply with exact context; a hunk may move (matching changes elsewhere) but never fuzz."""
    lines = base.split("\n")
    result, pos = [], 0
    for start, body in hunks(patch):
        old = [l for t, l in body if t != "+"]
        new = [l for t, l in body if t != "-"]
        want = start - 1
        found = None
        for delta in range(0, len(lines)):
            for at in (want + delta, want - delta):
                if pos <= at <= len(lines) - len(old) and lines[at:at + len(old)] == old:
                    found = at
                    break
            if found is not None:
                break
        if found is None:
            raise ValueError(f"hunk at line {start} does not apply")
        result += lines[pos:found] + new
        pos = found + len(old)
    return "\n".join(result + lines[pos:])


def make_patch(unit, base, derived):
    return "".join(difflib.unified_diff(base.splitlines(True), derived.splitlines(True),
                                        f"a/src/{unit}", f"b/src/{unit}", n=3))


def derived_text(unit, info):
    return apply(text(SRC / unit), text(DERIVE / info["patch"]))


def main(argv):
    prov = load()
    units = prov["units"]
    cmd = argv[1] if len(argv) > 1 else "check"
    if cmd == "apply":
        out = Path(argv[argv.index("--out") + 1])
        out.mkdir(parents=True, exist_ok=True)
        # A derived header must be found by every file that includes it with #include "..." (resolved next to the
        # including file first): "with_directory" copies the header's whole src directory, unchanged, into the
        # derived tree, so all of its units compile from there.
        for unit, info in units.items():
            if info.get("with_directory"):
                for f in (SRC / unit).parent.iterdir():
                    rel = f.relative_to(SRC).as_posix()
                    if f.is_file() and rel not in units:
                        dst = out / rel
                        dst.parent.mkdir(parents=True, exist_ok=True)
                        if not dst.exists() or dst.read_bytes() != f.read_bytes():
                            dst.write_bytes(f.read_bytes())
        for unit, info in units.items():
            data = derived_text(unit, info)
            dst = out / unit
            dst.parent.mkdir(parents=True, exist_ok=True)
            if not dst.exists() or text(dst) != data:
                dst.write_text(data, encoding="latin-1", newline="\n")
            if sha(text(SRC / unit)) != info["base_sha256"]:
                print(f"derive: WARNING src/{unit} changed since the derivative was reviewed "
                      f"(python port/tools/derive.py check)", file=sys.stderr)
        return 0
    if cmd == "check":
        bad = 0
        for unit, info in units.items():
            try:
                derived_text(unit, info)
            except ValueError as e:
                print(f"FAIL {unit}: patch no longer applies ({e}); re-derive from the new src/{unit}")
                bad += 1
                continue
            if sha(text(SRC / unit)) != info["base_sha256"]:
                print(f"REVIEW {unit}: src/{unit} changed since the derivative was reviewed; compare the change "
                      f"with port/derive/{info['patch']} and the divergences listed in provenance.json, then run "
                      f"`python port/tools/derive.py refresh {unit}`")
                bad += 1
        print(f"{len(units)} derivatives, {bad} need attention")
        return 1 if bad else 0
    unit = argv[2]
    if cmd == "edit":
        WORK.mkdir(parents=True, exist_ok=True)
        data = derived_text(unit, units[unit]) if unit in units else text(SRC / unit)
        (WORK / unit).parent.mkdir(parents=True, exist_ok=True)
        (WORK / unit).write_text(data, encoding="latin-1", newline="\n")
        print(WORK / unit)
        return 0
    if cmd in ("save", "refresh"):
        base = text(SRC / unit)
        derived = text(WORK / unit) if cmd == "save" else derived_text(unit, units[unit])
        info = units.setdefault(unit, {"patch": unit.replace("/", "_").replace(".c", ".patch").replace(".h", "_h.patch"),
                                       "reason": "TODO", "divergences": []})
        (DERIVE / info["patch"]).write_text(make_patch(unit, base, derived), encoding="latin-1", newline="\n")
        info["base_sha256"] = sha(base)
        store(prov)
        print(f"{unit}: patch {info['patch']} saved against base {info['base_sha256'][:12]}")
        return 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
