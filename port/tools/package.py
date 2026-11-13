"""Release packages of the native port (port/docs/RELEASE.md).

    python port/tools/package.py windows BUILD_DIR OUT_DIR
    python port/tools/package.py linux   BUILD_DIR OUT_DIR
    python port/tools/package.py android APK OUT_DIR

windows / linux: the program and the SDL3 library from a CMake build directory (SDL built from the pinned tarball,
-DRAY_SDL3_FROM_SOURCE=ON, so its licence is in the build's _deps), stripped; their debug symbols go to a separate
archive (rayman_sdl3-VERSION-PLATFORM-symbols.zip) for crash reports. Every package carries README.txt
(port/packaging/README.txt), LICENSE.txt (the project's GPL) and THIRD_PARTY.txt (SDL's zlib licence, stb_vorbis's
public-domain/MIT notice). The version is the one the program reports (rayman_sdl3 --version, CMake's project
VERSION). Writes only OUT_DIR.
"""
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile
import zipfile
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
ROOT = PORT.parent


def version():
    text = (PORT / "CMakeLists.txt").read_text(encoding="utf-8")
    return re.search(r"project\(rayman_port VERSION (\d+\.\d+\.\d+)", text).group(1)


def tool(name):
    """strip / objcopy of the toolchain that built the program (CMAKE_STRIP's directory), else PATH."""
    return shutil.which(name) or name


def third_party(build):
    parts = []
    sdl = next(iter(sorted(Path(build).glob("_deps/sdl3-src/LICENSE.txt"))), None)
    if sdl is None:
        raise SystemExit("SDL's LICENSE.txt not found in the build's _deps (build with -DRAY_SDL3_FROM_SOURCE=ON)")
    parts.append("SDL 3 (https://libsdl.org), zlib licence:\n\n" + sdl.read_text(encoding="utf-8", errors="replace"))
    cache = (Path(build) / "CMakeCache.txt").read_text(encoding="utf-8", errors="replace")
    stb = Path(re.search(r"^RAY_STB_VORBIS:FILEPATH=(.*)$", cache, re.M).group(1).strip())
    if not stb.is_file():
        stb = Path(build) / "stb" / stb.name
    text = stb.read_text(encoding="utf-8", errors="replace")
    licence = text[text.index("This software is available under 2 licenses"):]
    parts.append("stb_vorbis v1.22 (Sean Barrett, https://github.com/nothings/stb), used to decode Ogg Vorbis music:"
                 "\n\n" + licence.replace("*/", "").strip())
    return "\n\n" + ("\n\n" + "-" * 78 + "\n\n").join(parts) + "\n"


def common_files(dest, build):
    v = version()
    (dest / "README.txt").write_text((PORT / "packaging" / "README.txt").read_text(encoding="utf-8")
                                     .replace("{version}", v), encoding="utf-8", newline="\r\n")
    shutil.copyfile(ROOT / "LICENSE", dest / "LICENSE.txt")
    if build:
        (dest / "THIRD_PARTY.txt").write_text(third_party(build), encoding="utf-8", newline="\r\n")


def strip_into(src, dest, symbols):
    """dest: src stripped; symbols/NAME.debug: its debug information, linked by a debuglink."""
    debug = symbols / (dest.name + ".debug")
    shutil.copyfile(src, dest)
    subprocess.run([tool("objcopy"), "--only-keep-debug", str(dest), str(debug)], check=True)
    subprocess.run([tool("strip"), "--strip-debug", "--strip-unneeded", str(dest)], check=True)
    subprocess.run([tool("objcopy"), "--add-gnu-debuglink=" + str(debug), str(dest)], check=True, cwd=symbols)


def desktop(platform, build, out):
    build = Path(build).resolve()
    v = version()
    name = f"rayman_sdl3-{v}-{platform}-x64"
    work = Path(tempfile.mkdtemp(prefix="rayman-pkg-"))
    try:
        dest, symbols = work / name, work / (name + "-symbols")
        dest.mkdir()
        symbols.mkdir()
        if platform == "windows":
            files = [("rayman_sdl3.exe", "rayman_sdl3.exe"), ("SDL3.dll", "SDL3.dll")]
        else:
            sdl = next(iter(sorted(build.glob("_deps/sdl3-build/libSDL3.so.0"))), None) or \
                next(iter(sorted(build.glob("**/libSDL3.so.0"))))
            files = [("rayman_sdl3", "rayman_sdl3"), (str(sdl.relative_to(build)), "libSDL3.so.0")]
        for src, target in files:
            strip_into((build / src).resolve(), dest / target, symbols)
        if platform == "linux":
            (dest / "rayman_sdl3").chmod(0o755)
        common_files(dest, build)
        out.mkdir(parents=True, exist_ok=True)
        if platform == "windows":
            archive = out / (name + ".zip")
            with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
                for f in sorted(dest.iterdir()):
                    z.write(f, f.name)
        else:
            archive = out / (name + ".tar.gz")
            with tarfile.open(archive, "w:gz") as t:
                for f in sorted(dest.iterdir()):
                    t.add(f, arcname=f"{name}/{f.name}")
        with zipfile.ZipFile(out / (name + "-symbols.zip"), "w", zipfile.ZIP_DEFLATED) as z:
            for f in sorted(symbols.iterdir()):
                z.write(f, f.name)
        print(archive)
    finally:
        shutil.rmtree(work, ignore_errors=True)


def android(apk, out):
    v = version()
    out.mkdir(parents=True, exist_ok=True)
    target = out / f"rayman_sdl3-{v}-android.apk"
    shutil.copyfile(apk, target)
    print(target)


def main(argv):
    if len(argv) != 4 or argv[1] not in ("windows", "linux", "android"):
        print(__doc__)
        return 2
    out = Path(argv[3]).resolve()
    if argv[1] == "android":
        android(Path(argv[2]), out)
    else:
        desktop(argv[1], argv[2], out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
