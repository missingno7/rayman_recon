# Semantic divergences from the DOS program

Everything the port does differently from the DOS executable, with the reason. Derivative-level details are in
`port/derive/provenance.json`; boundary services in `BOUNDARY.md`.

## Port decisions (behaviour of the machine, not of the game)

| Area | DOS | Port | Why / effect on the game |
|---|---|---|---|
| Startup debug-register watch (PcMain 1:08442, load_world) | DR0/DR7 watch on the BIOS tick, INT3 traps in the loader | omitted; INT3 sites return immediately; DR reads are 0 | docs/port/PLATFORM_OPEN.md port decision; no game state depends on it |
| `menu_check_debug_breakpoints` | exits when DR0..DR3 hold a breakpoint | never exits | no hardware breakpoints exist natively |
| `menu_patch_once` | self-modifying run-once trampoline | no-op | it only patches its own code |
| Free memory (DPMI 0500h) | measured | 16 MiB | InitMemoryVariable takes its full-feature branch (normal mode, all sprites, backgrounds) |
| Block arenas | TailleMainMem* bytes | twice that on 64-bit hosts | native records with pointers are larger; only the out-of-memory threshold moves |
| Heap contents | whatever the DPMI host left (zero on fresh pages) | every `malloc` block zero-filled | some effects read freshly allocated memory before writing it; zero-fill makes runs reproducible instead of depending on host heap layout |
| Uninitialized locals | whatever the stack held | zero (`-ftrivial-auto-var-init=zero` for the historical units; CMake `RAY_UNINIT_LOCALS`, see below) | some paths read locals they never wrote, e.g. `SET_RAY_DIST` after a failed `GET_SPRITE_POS` (src/collision1.c), whose out-parameters stay unwritten: found by the gameplay-state log (object 46 of demo 4, `ray_dist`); scope measured below |
| `time()` | wall clock (the game seeds its random table with srand(time(NULL)) in MakeMyRand) | wall clock in real time; in `--deterministic` runs a fixed epoch (default 1996-01-01, `--epoch`) plus the port clock | reproducible regression runs; the interactive game stays randomized as on DOS |
| Machine time and pacing (platform/clock.h, 2026-11-12) | the game reads the PIT, the VGA retrace and the BIOS tick of a real machine, on which drawing and the game's own code take time | machine time is virtual in play as in the tests: it advances only when the game waits (a retrace, a timer deadline, the next value of a polled counter) - an ideal, fast enough PC. Real time only decides how long the host sleeps in those waits. The game runs on its own thread; the main thread presents the newest frame (platform/host.h `port_host_run`). When the game's frame is a whole number of display refreshes to within 1 % (60 Hz game on 60/120/240 Hz displays), a machine second is stretched to make it exact (measured here: 16.715 ms frame shown as 4 refreshes of 239.76 Hz = 16.683 ms, 0.19 % faster). A host that falls more than 8 ms behind is not chased: the game slows down, it never fast-forwards | the original's timer calibration (`sub_3609C`), its 31/32-frame deadline and its frame-skip decision (`display_mode`) come out the same in every run; the slow-machine behaviours (automatic frame skip, missed-deadline engine steps) do not occur. Before this, host presentation time and late wake-ups were read by the game as machine time: measured 19 % of gameplay frames lasting two refresh periods and six different calibration results in one run |
| CPU | CPUID family | Pentium (5) | only printed |
| DOS / Windows detection | INT 21h 30h, INT 2Fh 1600h/4680h | DOS 6.22, no Windows | printed; Windows paths not taken |
| Video presentation | VGA normal mode or Mode X | normal mode only; Mode X entries stop the program | first milestone; GameModeVideo = Mode X from a config file would stop with a clear message |
| Sound presentation | HMI SOS driver / Sound Blaster mixing and hardware rate | clock-driven HMI API, SDL resampling/mixing; linear balance pan with full centre gain | effects and speech enabled; exact driver mixing arithmetic and physical rate quantization are presentation differences (BOUNDARY.md) |
| Joystick | analog game port | nothing connected (buttons released, axes at calibrated centres) | a gamepad and the Android touch controls drive the game through its keyboard instead (virtual pad, platform/pad.c, TOUCH_CONTROLS.md): the game's own joystick code is not used |
| Pixel shape | 320x200 on a 4:3 monitor: each pixel 1.2 times as tall as wide | square pixels by default (user decision 2026-11-12: the art suits them better): 320x200 shown as 16:10; `"square_pixels": false` / `--aspect original` gives the original 4:3 picture | presentation only (platform/host.c); the game renders 320x200 in both |
| Graphics Details: video mode | the row shows pci1 / pci2 / vesa and opens the video-mode screen (normal mode, Mode X, VESA) | the row shows the wider view's mode and opens the port's Display screen: widescreen (off / 16:9 / 21:9 / fit), pixels (square / 4:3), fullscreen; also from the in-game options, where the original hid the row (still hidden in the fairy levels and world 6 level 4) | user decision 2026-11-13: the DOS choice is meaningless in the port (Mode X is not provided); derive/menu.patch, port/game/display_screen.c; the settings are presentation only and saved in the settings file, not in RAYMAN.CFG |
| CD drive | MSCDEX 2.21 + the Rayman CD | virtual MSCDEX drive (default D:) on the player's CUE/BIN image or the disc in a host CD drive (`--cd PATH.cue` / `--cd E:`, read-only device access, platform/cd_host.c): the game's own CD code builds and interprets every request; the port serves INT 2Fh 15xxh and the device packets (IOCTL, PLAY, STOP, RESUME, READ LONG) with position from the port clock at 75 sectors/s; CD audio plays from the BIN through SDL3. Files on the CD letter are read from the installed data directory; writes fail (EROFS) | the protection probe and the music behave as with the disc; without an image the drive is empty and the original "insert CD" prompt appears |
| Protection: `exit_if_not_cd_drive` | INT 21h 36h on the CD drive must report a read-only medium | succeeds | the player's own data stands in for the CD |
| CD audio source | the Rayman CD in the drive (GOG: DOSBox mounts `game.ins`, game.gog with Music/raymanNN.ogg as tracks 2..20) | the first usable of: the original CD (an image's *.cue, a disc in a host drive), then GOG's `game.ins`; Ogg tracks decoded to the CD's 44.1 kHz PCM (platform/ogg.c, stb_vorbis) | user decision 2026-11-13: measured against the original disc, GOG's Ogg files are the same recordings (sample-aligned correlation 0.96..0.996, no drift) except tracks 8, 13 and 14: 61 s, 60 s and 102 s instead of 285, 261 and 297 s, and 14 (most Jungle levels) another piece; the game restarts them only after timeCd (282, 258, 293 s), so GOG's DOSBox plays silence in between. The original disc wins when present; with GOG's files the port plays them as GOG does |
| Save folder | where the game runs (GOG: DOSBox overlays `cloud_saves` on the game folder) | `save_dir`, else GOG's `cloud_saves` next to the game folder, else the user's application-data folder | GOG installs share saves with GOG's launcher and Galaxy's sync |
| No CD | "Please insert Rayman's cdrom": Escape quits, Space starts a limited mode (JeuCracker: the world map keeps to the first three worlds); the save-time disc check sets JeuCracker without a disc | with no CD source at all (no image, no disc in a host drive), the full game without CD music: `verify_cdrom_response` answers the original's own "play without music" outcome (OptionMusicCdActive = 0) without the prompt, `check_cd_for_save` does not run (game/cd_policy.c, link-time wrappers); with a CD both run unchanged | user decision 2026-11-13: a port plays without its CD, only the CD music is missing; gate port_cd_missing_disc |
| Movies | read only from the install folder (where the installer could copy them from the CD) | the same names searched in the save folder and the data roots, then in the inserted CD's `RAYMAN` folder: a host drive's file in place, a CUE/BIN image's copied once into the save folder's `cd-cache` (platform/files.c movie_from_cd) | user decision 2026-11-13: the CD is a source of what the game folder lacks; only INTRO.DAT and CONCLU.DAT, which play with every version's data; gate port_cd_movie |
| CD request memory | conventional memory below 1 MiB (DPMI 0100h) | one bounded port-owned area addressed by real-mode tokens, used only by the CD transport | the request blocks carry segment:offset addresses by protocol; the tokens never encode host addresses |
| Text screen | AfficheEntete writes B8000h | skipped by the mirror | console output only |
| Data search path | one installed game directory (or save path for CD launch) | save overlay, then ordered player-chosen `--data` roots | lets the player supply installed movie files; no automatic movie/CD-image lookup |

## Derivative changes (same results, different spelling)

- `display.c`: declare the actual `playVideo(const char *, const char *, u8)` signature and pass its path buffer; the conclusion keeps the same address, track 21 and call order.
- `movie.c`: native work/surface/info records, pointer-sized palette context, `sizeof` clears and named timer/sound members. File-header prefix stays identical; decoder, calculation, pacing, input order, CD policy and final 32-step fade stay original. Optional movie trace observes state only.
- PIT0 reloading restarts its fractional phase while retaining accumulated whole BIOS ticks (BOUNDARY.md); IRQ0 and INT 1Ah share this model.

- `hifi.c`: decode serialized 16-byte bank records into native fields, bind pointers without truncation, use the full native config layout matching data_6664.c, and replace the speech flag byte offset with a field operation. All calculations and call order remain.
- `load.c`: DOS object records decoded field by field; file words stored into pointer fields sign-extended; BIOS
  tick read through the port clock.
- `sysutils.c`: free-memory answer and arena scale (above).
- `data/blk_215c.c`: `DisplayBufferNormal` points at the port's VGA window instead of linear A0000h.
- `box.c`: `(u8 *)((u32)draw_buffer + n)` -> `draw_buffer + n`.
- `jumelle.c`: wrapped `u32` indices cast to `s32` (the offsets the 32-bit program used).
- `cdaudio.c`: the INT 2Fh register block is passed as a pointer and the eleven far-pointer request views are
  native pointers into the port's request area; all request building, status interpretation, TOC/MSF logic and
  play/pause/resume routing stay original.
- `map_ngawe.c`, `frame.c`: argument order of a second C view of the same assembly routine.
- Screen effects (`effect2/50350/633e0/6c3b0/6ec80/effecthelp.c`, `blood.c`): `sizeof` instead of literal DOS sizes,
  a native texture pointer instead of a u32, descriptor writes at native offsets, pointer comparison instead of
  truncated s32 addresses. Proven equal frame by frame to the unmodified units at DOS pointer width.

## Known open items

- Floating point: `jumelle.c InitMatriceJumelle` and the effect curve use host `exp`/`atan`/`sqrt` instead of
  Watcom's software routines; results are converted to integers. A last-ulp difference could change one table
  entry; not yet measured against the DOS tables.
- `memcpy` on overlapping buffers: the DOS library copied forward; if any historical call overlaps, the host
  library may differ. No such call is known; not yet audited.
- `savecodec.c` declares `s32`/`u32` as `long`: correct on Windows (LLP64), must be revisited for LP64 hosts.

## Uninitialized locals: scope (2026-10-26)

A port policy about the historical C, independent of rendering: the original read whatever the DOS stack held;
a native build reads host stack contents, which differ between hosts and runs. The policy fixes the value (zero) so
that every run and architecture computes the same state. It is not used to hide any rendering difference: the
renderer's verification compares the native and the original image inside the same run, where both read the same
game state.

Measured with `port/tests/uninit_scope.py` (per-swap gameplay-state digests and presented-frame hashes of the six
original demos, two builds compared):

| Builds compared | x86_64 | i686 |
|---|---|---|
| zero vs `pattern` (every local starts as 0xFE bytes) | all six identical | - |
| zero vs `none` (host stack contents) | all six identical | demo 4: gameplay state differs from swap 2,165 on (images identical); others identical |

So in the demos the policy changes the outcome only where the stack would otherwise inject host-dependent values
(demo 4, i686); the values the affected paths read do not change the state for either fill tested. Compiler flow
analysis (`-Wmaybe-uninitialized`, all historical units without the policy) reports 60 candidate sites; it cannot
see unwritten out-parameters such as the `SET_RAY_DIST` case, so it is a lower bound, not the scope. Only
`RAY_UNINIT_LOCALS=zero` is a supported build; `pattern` and `none` exist for this measurement.

