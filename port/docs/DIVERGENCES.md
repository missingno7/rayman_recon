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
| CPU | CPUID family | Pentium (5) | only printed |
| DOS / Windows detection | INT 21h 30h, INT 2Fh 1600h/4680h | DOS 6.22, no Windows | printed; Windows paths not taken |
| Video presentation | VGA normal mode or Mode X | normal mode only; Mode X entries stop the program | first milestone; GameModeVideo = Mode X from a config file would stop with a clear message |
| Sound presentation | HMI SOS driver / Sound Blaster mixing and hardware rate | clock-driven HMI API, SDL resampling/mixing; linear balance pan with full centre gain | effects and speech enabled; exact driver mixing arithmetic and physical rate quantization are presentation differences (BOUNDARY.md) |
| Joystick | analog game port | nothing connected (buttons released, axes at calibrated centres) | gamepad mapping is a later step |
| CD drive | MSCDEX 2.21 + the Rayman CD | virtual MSCDEX drive (default D:) on the player's CUE/BIN image (`--cd`): the game's own CD code builds and interprets every request; the port serves INT 2Fh 15xxh and the device packets (IOCTL, PLAY, STOP, RESUME, READ LONG) with position from the port clock at 75 sectors/s; CD audio plays from the BIN through SDL3. Files on the CD letter are read from the installed data directory; writes fail (EROFS) | the protection probe and the music behave as with the disc; without an image the drive is empty and the original "insert CD" prompt appears |
| Protection: `exit_if_not_cd_drive` | INT 21h 36h on the CD drive must report a read-only medium | succeeds | the player's own data stands in for the CD |
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

