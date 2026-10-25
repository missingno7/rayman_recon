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
| `time()` | wall clock (the game seeds its random table with srand(time(NULL)) in MakeMyRand) | wall clock in real time; in `--deterministic` runs a fixed epoch (default 1996-01-01, `--epoch`) plus the port clock | reproducible regression runs; the interactive game stays randomized as on DOS |
| CPU | CPUID family | Pentium (5) | only printed |
| DOS / Windows detection | INT 21h 30h, INT 2Fh 1600h/4680h | DOS 6.22, no Windows | printed; Windows paths not taken |
| Video presentation | VGA normal mode or Mode X | normal mode only; Mode X entries stop the program | first milestone; GameModeVideo = Mode X from a config file would stop with a clear message |
| Sound | HMI SOS driver | no driver: LOAD_CONFIG takes the game's no-sound-card path | audio backend is the next platform step |
| Joystick | analog game port | nothing connected (buttons released, axes at calibrated centres) | gamepad mapping is a later step |
| CD drive | MSCDEX + Rayman CD | TEMPORARY (milestone 1): no Rayman CD; the CD checks are skipped | the game's own limited mode follows ("LIMITED VERSION" overlay, exit after the first map); the virtual CD drive replaces this |
| Protection: `exit_if_not_cd_drive` | INT 21h 36h on the CD drive must report a read-only medium | succeeds | the player's own data stands in for the CD |
| Text screen | AfficheEntete writes B8000h | skipped by the mirror | console output only |
| Intro movie | playVideo("intro.dat") | skipped by the mirror | movie player not ported yet |

## Derivative changes (same results, different spelling)

- `load.c`: DOS object records decoded field by field; file words stored into pointer fields sign-extended; BIOS
  tick read through the port clock.
- `sysutils.c`: free-memory answer and arena scale (above).
- `data/blk_215c.c`: `DisplayBufferNormal` points at the port's VGA window instead of linear A0000h.
- `box.c`: `(u8 *)((u32)draw_buffer + n)` -> `draw_buffer + n`.
- `jumelle.c`: wrapped `u32` indices cast to `s32` (the offsets the 32-bit program used).
- `map_ngawe.c`, `frame.c`: argument order of a second C view of the same assembly routine.

## Known open items

- Screen effects (`effect*.c`, `blood.c`) are not 64-bit clean yet (literal record sizes, a pointer kept in u32,
  descriptor offsets): they run correctly on the i686 build; x86_64 needs their derivatives (in progress).
- Floating point: `jumelle.c InitMatriceJumelle` and the effect curve use host `exp`/`atan`/`sqrt` instead of
  Watcom's software routines; results are converted to integers. A last-ulp difference could change one table
  entry; not yet measured against the DOS tables.
- `memcpy` on overlapping buffers: the DOS library copied forward; if any historical call overlaps, the host
  library may differ. No such call is known; not yet audited.
- `savecodec.c` declares `s32`/`u32` as `long`: correct on Windows (LLP64), must be revisited for LP64 hosts.
