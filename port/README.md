# Rayman native port (SDL3)

Runs the reconstructed DOS Rayman (GOG v1.21) natively: the original game code from `../src`, compiled by a modern
compiler, on an SDL3 host. Design: [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md); what replaces the DOS machine:
[docs/BOUNDARY.md](docs/BOUNDARY.md); every behavioural difference: [docs/DIVERGENCES.md](docs/DIVERGENCES.md).

## Build (Windows, MinGW-w64 + the CMake/Ninja shipped with Visual Studio)

```bash
cmake -S port -B build/port -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_PREFIX_PATH=C:/tools/sdl3-3.4.16-mingw/SDL3-3.4.16/x86_64-w64-mingw32 -DRAY_DATA=D:/Prog/rayman_recon/assets/GOG
```

```bash
cmake --build build/port
```

`RAY_DATA` is the player's original game directory (RAYMAN.EXE, RAY.LNG, PCMAP, ...); two data tables are taken
from that RAYMAN.EXE at build time. The i686 reference build uses `C:/msys64/mingw32/bin/gcc.exe` and the
`i686-w64-mingw32` SDL3 package (see docs/ARCHITECTURE.md, "64-bit hosts"). Presets: `CMakePresets.json`.

## Run

```bash
build/port/rayman.exe --data D:/Prog/rayman_recon/assets/GOG --cd D:/Prog/rayman_recon/assets/DOS_CD/RAYMAN.cue --menus
```

`--cd PATH.cue` inserts the player's Rayman CD image in the virtual drive (CD music, protection checks); without
it the drive is empty and the game asks for the CD as the original did. `--demo N` (0..5) plays one of the original recorded demos; `--headless --deterministic --frames N --frame-log F`
gives a reproducible per-frame hash log; `--dump-frames DIR --dump-every N` writes BMPs. Saves go to the SDL pref
directory unless `--save DIR` is given. Alt+Enter toggles fullscreen.

## Gates

`ctest` in the build directory: derivatives still apply to the reviewed `src` bases, the PcMain mirror is current,
the generated prototypes are current, every 64-bit-sensitive site has a verdict, the compat unit tests pass.
`python -B port/tests/asm_diff/test_lowlevel_native.py` proves the native drawing layer against the original
assembly (needs unicorn and the extracted oracle, `tools/extract.py`).
