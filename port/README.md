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
gives a reproducible per-frame hash log; `--dump-frames DIR --dump-every N` writes BMPs. `--data DIR` may be repeated (up to 32 roots); reads search saves first, then those directories in the given
order, using the first existing match. Explicit `--data` roots replace `RAYMAN_DATA`. Writes still go only to
the save directory. Every root participates in all reads: saves/configuration missing from an earlier root
can also come from a later root. Movies retain the DOS filenames `intro.dat` / `conclu.dat`: missing files return -5 and
play is skipped. To use movie files from a full install without replacing the GOG game data:

```bash
rayman --data D:/Prog/rayman_recon/assets/GOG --data D:/Prog/rayman_recon/assets/DOS --cd D:/Prog/rayman_recon/assets/DOS_CD/RAYMAN.cue --menus
```

No movie lookup is performed on the virtual CD image. As in PcMain, the intro runs after the player leaves
the menus to start a game (except demos and the `Frequence == 'F'` path); the conclusion uses the existing
victory path. The original player, frame pacing, palette fade and CD track windows run natively.
`RAYMAN_MOVIE_TRACE=FILE` optionally logs entry/result, decoded frames, selected presentation frames, CD row/state,
port-clock timestamps and CD audio queue size; `RAYMAN_CD_TRACE=FILE` logs the actual device commands.

Rendering (docs/RENDERER.md): the native renderer records what the original code draws each frame and draws it
itself. `--renderer native` presents only its image (every gameplay frame of the original demos is complete and
identical to the original; menus and other scenes are not native yet and show gaps); the default `reference`
presents the original draw path. `--verify-render` compares the native renderer with the original in every frame.
`--widescreen` / `--wide-pad N` is the first experiment (reference image in the centre, background and map in
the margins); it will be replaced by the native renderer drawing wider views itself.

Saves go to the SDL pref
directory unless `--save DIR` is given. Alt+Enter toggles fullscreen.

## Gates

`ctest` in the build directory: derivatives still apply to the reviewed `src` bases, the PcMain mirror is current,
the generated prototypes are current, every 64-bit-sensitive site has a verdict, the compat unit tests pass.
`python -B port/tests/asm_diff/test_lowlevel_native.py` proves the native drawing layer against the original
assembly (needs unicorn and the extracted oracle, `tools/extract.py`).
