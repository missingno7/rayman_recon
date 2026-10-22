"""Run pinned Watcom tools natively or a DOS assembler under pinned MS-DOS Player.

    python tools/wcc.py [--install wc106] [--profile game] [FLAGS...] SRC.c -fo=OUT.obj
    python tools/wcc.py --profile hmi_tasm SRC.asm -fo=OUT.obj

Key files of the install are hash-checked before every run. The environment is reduced to WATCOM, INCLUDE
(install H/ plus src/include), PATH (install BINNT) and TMP/TEMP inside build/tmp. Prints the compiler banner.
"""
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, ROOT, TOOLS_DIR, load_json, sha256_file  # noqa: E402

_checked = set()


def install(name):
    tc = load_json(CONFIG / "toolchain.json")
    ent = tc["installs"][name]
    base = TOOLS_DIR / ent["dir"]
    if name not in _checked:
        for rel, want in ent.get("key_files", {}).items():
            if want is None:
                continue
            got = sha256_file(base / rel)
            if got != want:
                raise SystemExit(f"{name}: {rel} sha256 {got} != pinned {want}")
        _checked.add(name)
    return ent, base


def profile(name):
    tc = load_json(CONFIG / "toolchain.json")
    p = tc["profiles"][name]
    if p.get("kind") != tc["installs"][p["install"]].get("kind"):
        raise SystemExit(f"{name}: profile/install kind mismatch")
    return p


def assemble_dos(src, out, flags, inst):
    """Pinned DOS assembler, fresh private 8.3 invocation names, no object conversion.

    This backend currently accepts self-contained sources (including the HMI units).
    Install kind dispatch also works through existing match/verify build callers.
    """
    ent, base = install(inst)
    # Recheck every invocation, including repeated builds in one verifier process.
    for rel, want in ent["key_files"].items():
        if not want or sha256_file(base / rel) != want:
            raise SystemExit(f"{inst}: {rel} does not match pinned sha256")
    runner = TOOLS_DIR / ent["runner"]["path"]
    if sha256_file(runner) != ent["runner"]["sha256"]:
        raise SystemExit(f"{inst}: DOS runner does not match pinned sha256")
    src, out = Path(src).resolve(), Path(out).resolve()
    tmp_root = BUILD / "tmp"
    tmp_root.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="", dir=tmp_root) as invocation:
        tmp = Path(invocation)
        # Local executable and file aliases avoid long paths in DOS command tails.
        exe = tmp / "TASM.EXE"
        shutil.copyfile(base / ent["dos_assembler"], exe)
        if sha256_file(exe) != ent["key_files"][ent["dos_assembler"]]:
            raise SystemExit(f"{inst}: private assembler copy hash mismatch")
        shutil.copyfile(src, tmp / "UNIT.ASM")
        obj = tmp / "UNIT.OBJ"
        assert not obj.exists()  # new invocation directory; never accept a stale object
        env = {"SYSTEMROOT": os.environ.get("SYSTEMROOT", "C:\\Windows"),
               "PATH": str(tmp), "TMP": str(tmp), "TEMP": str(tmp)}
        cmd = [str(runner), *ent["runner"]["flags"], exe.name,
               *flags, "UNIT.ASM,UNIT.OBJ;"]
        try:
            run = subprocess.run(cmd, cwd=tmp, env=env, capture_output=True, timeout=30)
        except subprocess.TimeoutExpired as exc:
            text = ((exc.stdout or b"") + (exc.stderr or b"")).decode("cp437", "replace")
            return 1, text + "\nDOS assembler timed out after 30 seconds"
        text = (run.stdout + run.stderr).decode("cp437", "replace")
        fatal = re.search(r"\bfatal\b|\*\*\s*error\s*\*\*|\berror\s+messages\s*:\s*[1-9]\d*", text, re.I)
        if run.returncode or fatal or not obj.is_file() or not obj.stat().st_size:
            return run.returncode or 1, text + "\nDOS assembler failed or did not produce a new object"
        # Reject malformed/truncated output before publishing it to the caller.
        import omf
        try:
            if not omf.load(obj):
                raise ValueError("empty OMF module list")
        except (ValueError, IndexError, KeyError, struct.error) as exc:
            return 1, text + f"\nDOS assembler produced invalid OMF: {exc}"
        shutil.copyfile(obj, out)
        return 0, text


def compile_c(src, out, flags, inst="wc106", extra_include=(), tool="wcc386"):
    ent, base = install(inst)
    tmp = BUILD / "tmp" / f"wcc{os.getpid()}"
    tmp.mkdir(parents=True, exist_ok=True)
    inc = [str(base / "H")] + [str(p) for p in extra_include] + [str(ROOT / "src" / "include")]
    env = {"SYSTEMROOT": os.environ.get("SYSTEMROOT", "C:\\Windows"), "WATCOM": str(base),
           "INCLUDE": ";".join(inc), "PATH": str(base / "BINNT"), "TMP": str(tmp), "TEMP": str(tmp)}
    if tool not in ent:
        raise SystemExit(f"{inst}: no pinned {tool}")
    exe = base / ent[tool]["nt"]
    # Watcom accepts '/' as an option prefix: always pass absolute backslash paths.
    cmd = [str(exe)] + list(flags) + [str(Path(src).resolve()), f"-fo={Path(out).resolve()}"]
    try:
        r = subprocess.run(cmd, env=env, capture_output=True, text=True, timeout=120, cwd=str(tmp))
    finally:
        for f in tmp.iterdir():
            f.unlink()
        tmp.rmdir()
    return r.returncode, r.stdout + r.stderr


def build(src, out, flags, inst="wc106"):
    """Compile C or assemble with pinned WASM / the install's explicit DOS backend.

    .asm rule (checked here, never in the comparator): every CODE segment must have a public at offset 0, so the
    per-public extents compare the whole contribution, including embedded data between routines."""
    ent = load_json(CONFIG / "toolchain.json")["installs"][inst]
    kind = ent.get("kind")
    if kind not in (None, "dos_assembler"):
        raise SystemExit(f"{inst}: unsupported install kind {kind}")
    if kind == "dos_assembler" and Path(src).suffix.lower() != ".asm":
        return 1, f"{inst}: DOS assembler requires an .asm source"
    if Path(src).suffix.lower() != ".asm":
        return compile_c(src, out, flags, inst)
    if kind == "dos_assembler":
        rc, text = assemble_dos(src, out, flags, inst)
    else:
        rc, text = compile_c(src, out, flags, inst, tool="wasm")
    if rc:
        return rc, text
    import omf
    for m in omf.load(out):
        for si, seg in enumerate(m.segments):            # 1-based OMF indices; slot 0 is unused
            if seg is None or seg.cls.upper() != "CODE" or not seg.size:
                continue
            if not any(s == si and o == 0 for _, s, o, _ in m.publics):
                return 1, text + f"\n{src}: CODE segment {seg.name} has no public at offset 0 (uncompared prefix)"
    return 0, text


def main(argv):
    argv = list(argv[1:])
    inst = "wc106"
    flags = []
    if "--install" in argv:
        i = argv.index("--install")
        inst = argv[i + 1]
        del argv[i:i + 2]
    if "--profile" in argv:
        i = argv.index("--profile")
        p = profile(argv[i + 1])
        inst, flags = p["install"], list(p["flags"])
        del argv[i:i + 2]
    out = [a for a in argv if a.startswith("-fo=")]
    srcs = [a for a in argv if not a.startswith("-")]
    flags += [a for a in argv if a.startswith("-") and not a.startswith("-fo=")]
    if len(srcs) != 1 or len(out) != 1:
        print(__doc__)
        return 2
    rc, text = build(srcs[0], out[0][4:], flags, inst)
    print(text, end="")
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv))
