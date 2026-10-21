"""Run a pinned Watcom wcc386 natively (Win32 loader stub + bound compiler payload).

    python tools/wcc.py [--install wc106] [--profile game] [FLAGS...] SRC.c -fo=OUT.obj

Key files of the install are hash-checked before every run. The environment is reduced to WATCOM, INCLUDE
(install H/ plus src/include), PATH (install BINNT) and TMP/TEMP inside build/tmp. Prints the compiler banner.
"""
import os
import subprocess
import sys
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
    return load_json(CONFIG / "toolchain.json")["profiles"][name]


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
    """Compile a .c unit with wcc386 or assemble a hand-written .asm unit with the pinned WASM of the same install.

    .asm rule (checked here, never in the comparator): every CODE segment must have a public at offset 0, so the
    per-public extents compare the whole contribution, including embedded data between routines."""
    if Path(src).suffix.lower() != ".asm":
        return compile_c(src, out, flags, inst)
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
    rc, text = compile_c(srcs[0], out[0][4:], flags, inst)
    print(text, end="")
    return rc


if __name__ == "__main__":
    sys.exit(main(sys.argv))
