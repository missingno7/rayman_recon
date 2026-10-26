# Native port architecture

The port runs the **reconstructed original game code** natively. It is not a rewrite: every gameplay, rendering
decision, file format, timing decision and scene of the DOS program executes from `src/` (the historical
reconstruction, matched byte for byte against GOG v1.21 RAYMAN.EXE). The port replaces only what the DOS machine
provided: the hand-written assembly layer (by C implementing its proven contracts), the VGA/PIT/keyboard
hardware, DOS/BIOS/DPMI services, MSCDEX and the HMI sound driver.

## Source ownership

| Path | Owner | Rule |
|---|---|---|
| `src/` | matching reconstruction | authoritative; never edited for the port |
| `port/derive/` | port | reviewed patches for units that cannot be compiled unchanged; applied to the *current* `src/` at build time |
| `port/compat/` | port | Watcom/DOS dialect (pre-included headers) and the DOS-machine services behind it |
| `port/video/` | port | VGA device model and native implementations of the assembly drawing contracts |
| `port/platform/` | port | SDL3 host (window, presentation, keyboard), file service, clock |
| `port/game/` | port | entry points, per-unit prototype headers, serialized-record decoders, unit treatment |
| `port/tests/` | port | native gates and the differential proof against the original assembly |

## Compiling the historical C

Every `game`-profile unit of `config/units.json` is compiled by a modern C compiler with:

- `compat/include/port_watcom.h` pre-included: Watcom keywords (`__far`, `__interrupt`, ...) with no meaning in a
  flat address space, and C-library calls routed to explicit port services (`fopen` -> DOS path service,
  `open` with *Watcom* flag values, `rand` -> Watcom's LCG, `itoa`, `kbhit`, ...);
- compat versions of `<i86.h>`, `<dos.h>`, `<conio.h>`, `<io.h>` (register images, port I/O, vectors);
- `game/protos/<unit>.h` where the unit calls functions without a declaration (Watcom's implicit `int` was
  harmless on its 32-bit register ABI; natively a pointer or `s16` result needs the real prototype). Generated
  from the semantic registries by `tools/gen_protos.py`; kept current by a gate;
- Watcom 10.6 language semantics: unsigned plain `char`, wrapping signed arithmetic (`-fwrapv`), no strict
  aliasing, shared tentative definitions (`-fcommon`), SysV bit-field layout;
- unknown pragmas ignored: `#pragma aux` ABI declarations have no native meaning; the `#pragma aux NAME = ...`
  ORIGINAL_INLINE sequences are implemented as C functions in `compat/src/inline_helpers.c`.
- the Watcom `<fcntl.h>` flag values (`compat/include/port_watcom_fcntl.h`, applied after every host header;
  `port_open` decodes them). A static assertion in `game/port_main.c` and the `port_config_roundtrip` gate
  (RAYMAN.CFG written, reloaded, sound card kept) guard it.

`pcmain.c`'s `void main(int, char **)` is compiled as `rayman_dos_main` (game/units.json); the SDL3 host owns
the process entry point.

### Derivatives

A unit whose code depends on the DOS machine in a way no boundary service can absorb (a 32-bit address in an
`s32`, a file record read straight into a struct, a hard-coded VGA address) gets a derivative: a minimal patch
against its `src/` file, each change marked `/* PORT: ... */`, with reason and divergence list in
`derive/provenance.json`. The build applies the patch to the current `src/` file, so matching improvements flow
into the port automatically; `tools/derive.py check` (a ctest gate) fails when the base changed since review.
Current derivatives (17): `bss/bt.h` (BSS layout), `cdaudio.c` (CD transport), `hifi.c` (serialized banks, request/config native views), `sysutils.c`, `load.c`, `data/blk_215c.c`, `box.c`, `map_ngawe.c`, `frame.c`, `jumelle.c`, and the screen effects `effect2.c`, `effect50350.c`, `effect633e0.c`, `effect6c3b0.c`, `effect6ec80.c`, `effecthelp.c`, `blood.c` (literal sizes of pointer-carrying records, a texture pointer kept in `u32`, descriptor offsets, truncated address comparison).

`game/port_main.c` mirrors `PcMain` call for call for the test entries; `tools/mirror.py` pins the mirrored
bodies so a change to `PcMain` forces a review.

## 64-bit hosts

The port targets 64-bit hosts first; every 64-bit hazard is fixed so that the 32-bit behaviour is preserved, not
by truncating pointers or emulating a DOS address space:

- **Serialized layouts stay separate from runtime structures.** Level files store 132-byte DOS `obj_t` images;
  `game/dos_records.c` decodes them field by field from their DOS offsets (generated from the checked semantic
  registry, `docs/semantics/generated/semantics_dos.h`) into native `obj_t`, and the loader rebinds the four
  index words exactly as DOS did. Four places that read a 32-bit file word into a pointer field use
  `port_fread_pointer_word` (sign-extended, so the `-1` sentinel and the `(s32)` narrowing behave as on DOS).
- **Address arithmetic through integers** (`(u8 *)((u32)ptr + n)`) becomes pointer arithmetic in a derivative.
- **Unsigned 32-bit index wraparound** (`ptr[x - y*224]` with `u32 x, y`) is kept with an `(s32)` cast.
- **Allocation sizes and arenas.** Literal DOS sizes of pointer-carrying records are replaced by `sizeof`;
  the game's block arenas are scaled (`PORT_ARENA_SCALE`) because native records are larger.
- **Packing pragmas.** `src/bss/bt.h` opens `#pragma pack(4)` (never closed) before the shared headers, for the
  DOS BSS placement. Natively it capped pointer alignment, so the BSS units defined pointer-carrying types with a
  different layout than the code units (`mem_t` 16 vs 24 bytes: the arena capacity aliased `cdTime`; the
  `bande_t` table of the world 5 heat haze). A derivative of `bt.h` removes the global pragma (keeping it only
  around `temp_mem_entry_t`, as `sysutils_local.h` does); the whole `src/bss` directory is compiled from the
  derived tree (`with_directory` in provenance.json) so its units find the derived header.
- **One assembly routine, several C views.** On DOS a `#pragma aux` register ABI let callers see one routine
  through prototypes with different argument orders; natively each extra view gets an adapter
  (`video/lowlevel_adapters.c`) or a one-line derivative.

Gates: `tools/abi_audit.py` lists every pointer/integer conversion and incompatible pointer assignment the
compiler reports in the historical units; each carries a reviewed verdict in `docs/abi_sites.json` (HARMLESS,
DERIVED, NOT_ON_PORT_PATH, OPEN). Hazards a compiler cannot see (index wraparound, literal sizes) are caught by
the **i686 reference build**: the same port compiled for 32-bit x86 has the DOS pointer width, so any divergence
between the x86_64 and i686 builds on the same deterministic input exposes a 64-bit problem.

## The machine model

- **Clock** (`platform/clock.c`): one time source, real time or deterministic (time advances only when the game
  waits for a future event: the behaviour of an infinitely fast machine, reproducible bit for bit).
- **PIT channel 2 / timing entries** (`compat/src/pc_hardware.c`): `clock_ticks`, `wait_until`, `speaker_enable`,
  `synchro`, `endsynchro` keep their original algorithms (docs/port/ASM_SPEC.md) on a modelled counter. Each latch
  advances at least one PIT tick, as an ISA port read did; `clock_ticks` treats equal reads as a wrap.
- **VGA** (`video/vga.c`): the A0000h window the game draws into, the 6-bit DAC, the retrace signal derived from
  the programmed CRTC/misc registers (the game selects 50..100 Hz), BIOS modes. Presentation is a side effect of
  the device, like a monitor: an image is shown at each retrace or swap when the window or DAC changed.
- **Keyboard**: SDL key events become PC/AT set-1 scan codes and run the game's own INT 9 handler
  (`input.c PC_keyboard_interrupt_handler`) with a latched port 0x60.
- **DOS/BIOS/DPMI**: answered as the port's machine (DOS 6.22, no Windows, no mouse, 16 MiB free); anything
  untreated stops the program with its name (`port_unimplemented`), never silently.

## Entry points and tests

`game/entry.c`: `--demo N` runs `PcMain`'s initialisation and one of the six original recorded demos
(`DemoRecord`, played by the game's own demo code: authentic human input); `--menus` runs the original menus and
game interactively; `--dos-main` the unmodified DOS `main`. `--deterministic --headless --frame-log` give
reproducible per-frame hashes for regression runs. `ctest` runs: derivative/mirror/prototype/ABI gates, native unit
tests of the compat layer, the screen-effect differential (`tests/effect_diff.c`: 18,428 effect frames, x86_64
and i686 derivatives equal to the unmodified units at DOS pointer width) and the six original demos end to end against
their golden summaries (shared by all architectures); `tests/asm_diff/` proves the native drawing layer against the original assembly in
Unicorn (14,594 cases, zero mismatches).
