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

`RAY_DATA` is the player's original game directory (RAY.LNG, PCMAP, the .DAT files, ...), used only by the tests:
the build reads no game file, and the program reads only the game's data files, never RAYMAN.EXE. The i686 reference build uses `C:/msys64/mingw32/bin/gcc.exe` and the
`i686-w64-mingw32` SDL3 package (see docs/ARCHITECTURE.md, "64-bit hosts"). Presets: `CMakePresets.json`.

Android (arm64-v8a / x86_64 APK with the Android release's touch controls and gamepad support):
[docs/ANDROID.md](docs/ANDROID.md), [docs/TOUCH_CONTROLS.md](docs/TOUCH_CONTROLS.md).

## Play (from the game folder)

Copy `rayman_sdl3.exe` and `SDL3.dll` into a GOG v1.21 Rayman folder (the one
with RAY.LNG and PCMAP; in GOG's Rayman Forever: `Rayman\`) and start it without arguments. In a GOG install
it uses GOG's own CD (`..\game.ins`: `game.gog` and the music in `Music\raymanNN.ogg`, the drive GOG's DOSBox
mounts) unless the original CD is at hand (below), and keeps RAYMAN.CFG and the saves in GOG's `..\cloud_saves`,
shared with GOG's launcher. The program then uses its own folder as the game and
creates `rayman_sdl3.json` next to itself on the first run (platform/config.h); a file written by an earlier
version is completed with the newer settings, unknown settings are ignored:

| Setting | Default | Meaning |
|---|---|---|
| `widescreen` | `"fit"` | wider view drawn by the native renderer: `"off"`, `"16:9"`, `"21:9"`, or `"fit"` (as wide as the window, or the display in fullscreen); earlier files' `true`/`false` read as `"fit"`/`"off"`. Also in the game: Options, Graphics Details, Video mode opens the Display screen (widescreen, pixels, fullscreen), which changes them while the game runs and saves them here |
| `square_pixels` | `true` | each game pixel square: the 320x200 picture is shown as 16:10; `false` = the original 4:3 picture (pixels 1.2 times as tall, as on a 1990s monitor); `--aspect square\|original` |
| `fullscreen` | `false` | start in fullscreen (Alt+Enter toggles) |
| `window_scale` | `0` | integer window scale; 0 = automatic |
| `vsync` | `true` | frames are shown at the display's refreshes (no tearing); `--no-vsync` on the command line |
| `cd` | `"auto"` | the Rayman CD: `"auto"` takes the first usable of `RAYMAN.cue` in the game folder, another `*.cue` there or in its parent folder, a CD drive whose disc is labelled RAYMAN (a real disc or a mounted image, read through the drive; Windows), and GOG's `*.ins` (game folder or parent): the original CD before GOG's, whose music cuts tracks 8, 13 and 14 short (docs/DIVERGENCES.md); else none. Cue sheets may name several files: BIN data and audio, or Ogg Vorbis audio tracks (GOG); `"E:"` = that drive; a `.cue` path; `"none"` = no CD. Without a CD the full game runs without CD music |
| `save_dir` | `""` | where saves and RAYMAN.CFG are written; empty = GOG's `cloud_saves` next to the game folder when there is one, else the user's application-data folder |
| `extra_data_dirs` | `[]` | further read-only folders searched after the game folder, e.g. one holding INTRO.DAT / CONCLU.DAT |

Command-line options override the file. Runs that name their data (`--data`, `RAYMAN_DATA`: tests, development)
read a settings file only when `--config FILE` names one. Data of other versions (e.g. v1.12) does not load.

## Run

```bash
build/port/rayman_sdl3.exe --data D:/Prog/rayman_recon/assets/GOG --cd D:/Prog/rayman_recon/assets/DOS_CD/RAYMAN.cue --menus
```

`--cd PATH.cue` inserts the player's Rayman CD image in the virtual drive (CD music, protection checks, and the
movies when the game folder has none). Without a CD the full game runs without CD music (the original asked for the
CD and offered a limited mode; game/cd_policy.c, docs/DIVERGENCES.md). `--demo N` (0..5) plays one of the original recorded demos; `--headless --deterministic --frames N --frame-log F`
gives a reproducible per-frame hash log; `--dump-frames DIR --dump-every N` writes BMPs. `--data DIR` may be repeated (up to 32 roots); reads search saves first, then those directories in the given
order, using the first existing match. Explicit `--data` roots replace `RAYMAN_DATA`. Writes still go only to
the save directory. Every root participates in all reads: saves/configuration missing from an earlier root
can also come from a later root. Movies retain the DOS filenames `intro.dat` / `conclu.dat`: missing files return -5 and
play is skipped. To use movie files from a full install without replacing the GOG game data:

```bash
rayman --data D:/Prog/rayman_recon/assets/GOG --data D:/Prog/rayman_recon/assets/DOS --cd D:/Prog/rayman_recon/assets/DOS_CD/RAYMAN.cue --menus
```

When no data root has a movie, it comes from the inserted CD's `RAYMAN` folder: read in place from a disc in a
host drive, copied once from a CUE/BIN image into `cd-cache` in the save folder (as the original installer copied
it to the hard disk; platform/files.c). Only the two movies: they play with every version's game data, while the
CD's other files belong to its own version (`--no-cd-movies` turns this off: the test scenarios, timed for a new
game without the intro, use it). As in PcMain, the intro runs after the player leaves
the menus to start a game (except demos and the `Frequence == 'F'` path); the conclusion uses the existing
victory path. The original player, frame pacing, palette fade and CD track windows run natively.
`RAYMAN_MOVIE_TRACE=FILE` optionally logs entry/result, decoded frames, selected presentation frames, CD row/state,
port-clock timestamps and CD audio queue size; `RAYMAN_CD_TRACE=FILE` logs the actual device commands.

Rendering (docs/RENDERER.md): the native renderer records what the original code draws each frame and draws it
itself, and by default its image is presented (the demos, menus, save slots, world map and gameplay, the first
levels of all six worlds, continue and game over are complete and identical to the original, and so are the options
and in-game menus, pause, credits and the intro movie); while it cannot draw a frame completely (scenes no test
reaches yet may have such frames) the original image is shown instead. `--renderer native` presents only the native
image (gaps visible), `--renderer reference` only the original draw path. `--verify-render` compares the native renderer with the original in every frame.
`--widescreen` (16:9: 18 columns per side with square pixels, 54 with `--aspect original`) or `--wide-pad N`
(0..160 columns per side) makes the native renderer draw a wider view of the same frames: the world, objects and
weather continue past the original 320 columns, without the original's 4-pixel border; static objects the game keeps
inactive beyond its action zone are drawn in the margins, and the HUD keeps its distance from the view's corners. At
a level edge the view stops at the map's edge (the game's camera is untouched), and a level narrower than the view
gets black borders; menus, movies and other screen-space scenes stay 320 columns wide in the centre. Pixel shape:
square by default (the 320x200 picture shown as 16:10), `--aspect original` for the original 4:3 picture.

Saves go to the SDL pref
directory unless `--save DIR` is given. Alt+Enter toggles fullscreen.

## Gates

`ctest` in the build directory: derivatives still apply to the reviewed `src` bases, the PcMain mirror is current,
the generated prototypes are current, every 64-bit-sensitive site has a verdict, the compat unit tests pass.
`python -B port/tests/asm_diff/test_lowlevel_native.py` proves the native drawing layer against the original
assembly (needs unicorn and the extracted oracle, `tools/extract.py`).
