# Compiler and toolchain evidence

Evidence levels: PROVEN (measured, reproducible command), STRONG (several consistent observations),
HYPOTHESIS, OPEN. Probe sources live in `scratch/probe/` (ignored); conclusions here.

## Established
- **Watcom C, 32-bit flat, register calling convention** [PROVEN]: args in EAX, EDX, EBX, ECX; callee saves used
  registers (`push ebx/ecx/edx`); symbols `name_` for functions, `_name` for data (OMF from wcc386 matches).
- **Runtime = Watcom CLIB3R + MATH387R** (register-convention libs) [PROVEN, tools/libscan.py]:
  v1.21 links **10.6** libraries (116 CLIB3R + 1 MATH387R members exact incl. fixup sites; 11 members exact only
  with 10.6, 0 only with 10.5); v1.12 links **10.5** libraries (112 exact; 10 only with 10.5, 0 only with 10.6).
  10.0/10.0a: 41-44 members only. Copyright string "1988-1995" fits 10.5 and 10.6.
- **Game profile `-oaxt -s -5r` with wcc386 10.6 reproduces the rand object exactly** [PROVEN]:
  `python tools/verify.py` -> myRand, MakeMyRand STRICT (111 bytes, whole object `_TEXT`). Discriminators:
  - `-5` (Pentium scheduling): short array element load as `mov edx,[i*2+arr-2]; sar edx,16` (`-4r` gives `movsx`).
  - `-oa` (relaxed aliasing): the global `RandomIndex` is kept in EBX and written back at exit *after* the return
    value is in EAX; without `-oa` every access goes to memory (41 byte diffs).
  - `-ot` required together with `-oa` (`-oxa` does not reproduce it, `-oaxt` does).
  - `-s` (no stack checks): no `__CHK` calls anywhere in game code.
- **Object layout** [PROVEN on rand and mathutil (whole objects contiguous); 83 decode-validated boundaries]: `_TEXT` paragraph aligned (OMF align 3);
  functions inside 4-aligned with nop padding (90, 8bc0, 8d4000); WLINK pads between objects with 00.
- **Structure packing 1 by default** [PROVEN, wcc386 10.6 probe]: `struct {char a; int b;}` -> b at +1, size 5;
  no `-zp` flag or pragma is needed for the packed game structures (src/include/obj.h).
- **`_DATA` items are naturally aligned** [PROVEN probe + oracle]: `-zp1` packs struct members only; file-scope
  initialised objects are aligned to their type (short 2, int 4, double 8). Oracle: `u8 joy_done` at 2:02994 is
  followed by one pad byte before `s16 zonediffx[]` at 2:02996 (src/data_coll.c VERIFIED).
- **Bitfields allocate LSB-first in `unsigned char` units** [PROVEN]: BAT_ray_in_zone (STRICT) clears `flip_x`
  (4th field) with `and byte ptr [obj+0x81],0xf7`.
- **Data reference encoding**: wcc386 keeps the addend in the fixup field (`RandArray-2` -> field -2); PMW1 stores
  absolute object offsets, so `match.py` checks bound address + field.

- **`_BSS` order rule = kegg_recon's Watcom 10.0 rule** [PROVEN for 10.6 and 10.5]: 30 random TUs (2,457
  uninitialised globals, mixed sizes/arrays/names) laid out exactly as predicted (size rank, declaration-index group
  of 25, hashpjw(name) % 241, later declaration first). Probe: `tools/probes/bss_rule.py` (uses kegg's bssorder.py).
- **Intrinsics are on (part of `-ox`/`-oaxt`)** [PROVEN]: with `#include <string.h>`, `memcpy(dst, src, 3)` compiles
  to Watcom's inline copy `push edi; mov eax,ecx; shr ecx,2; repne movsd; mov cl,al; and cl,3; repne movsb; pop edi`
  (note the `f2` prefix), exactly as in the original deter_num_let (1:0efb4). Adding `-oi` changes nothing.
  The runtime has no memcpy_/strlen_/strncpy_ members: such calls in Rayverse/PS1 bodies must become the intrinsic
  form (include the Watcom headers; do not hand-declare memcpy/strlen, which disables the intrinsic).
- **Local aggregate initialisers live in `_TEXT` at the start of the object** [PROVEN]: `char t[4] = {0};` puts a
  4-byte constant at `_TEXT` offset 0 (loaded `mov eax,[const]; mov [esp],eax`). The basic.c object therefore starts
  at 1:0d070 with that constant, and EOA is at 1:0d074 (probe scratch/oi/p.c reproduces the prologue and copy).
- **No `-oe`** [STRONG]: with `-oe` (alone or inside `-oneatx`/`-otexan`) `DoCdRap` inlines `cd_playing` from the
  same object, while the original calls it; audiocd drops from 8 to 7 STRICT. rand and mathutil are unaffected.
- **Watcom 10.5 and 10.6 compilers give identical code** on all accepted units (rand 2/2, audiocd 8/11,
  mathutil 12/12 with both) [non-discriminating so far].

## Undetermined / next discriminators
- `-on` (fast FP), `-oi`, `-ol`/`-ol+`, `-om`: no accepted unit discriminates them yet. Need FP code (`-on`,
  `-fp3/-fp5`, `-fpi87`), loops (`-ol`), intrinsic candidates (`-oi`: memcpy/strcpy/abs inline forms).
- Compiler release for v1.21 game code: 10.0, 10.0a, 10.5 and 10.6 give identical code on everything tested
  (only the 10.6 libraries match the v1.21 runtime). The 10.5 compiler was recovered from the CD's packed
  `DISKIMGS` set (`C:\tools\watcom-10.5-pck`, docs/TOOLCHAIN.md).
- `-ei` enums, `-j` char signedness: not yet discriminated.
- Linker: WLINK version/options, object order, `_DATA`/`_BSS` order not studied yet.
- v1.12 rand variant is STRICT with the 10.6 compiler against v1.12 (scratch), although its runtime is 10.5.

- **Prototype return width shapes the caller** [PROVEN, bonus do_perfect_bonus]: declaring `but0pressed()..but3pressed()`
  as returning `short` instead of `int` changes the caller's test sequence (4 bytes) and is required for STRICT.
- **Return width is visible in the callee's own prologue** [PROVEN, basic get_proj_x]: `s16 get_proj_x(s16, s16)` is
  STRICT; Rayverse's `s32` return (also in the PS1 headers and the Android x86 build) is not - the narrower return
  changes the saved-register set and turns `movsx ecx,ax; ...; add ecx,0x100` into `cwde; lea ecx,[eax+0x100]`.
  Rayverse/PS1/Android widen return types; only the DOS code decides. When a function's first difference is in the
  register-save prologue or a sign extension, test narrower return/parameter types first.
- **Bitfield reads are distinguishable from mask arithmetic** [PROVEN, basic on_block_chdir]: only a bitfield read of
  bit 5 of `block_flags[]` is STRICT; `(x & 0x20) != 0`, `x & 0x20`, `(x & 0x20) >> 5` all differ. Hence block_flags is
  declared as a union with a bitfield view (src/include/map.h); flags[] stays u32 (shift/mask reads match there).
- **Declaration order, predicate grouping and switch-case order affect code** [basic_sol: EOA, GET_BB1_ZDCs, switchOff].
- **Comparison operands are evaluated in source order** [PROVEN, basic obj_hurt]: the original loads
  `target->hit_points` first, i.e. `if (target->hit_points < poing.damage)`; Rayverse's equivalent
  `poing.damage > target->hit_points` swaps the loads and the branch (`jbe` vs `jae`). Read the operand order and the
  branch condition off the binary.
- **Index operand order affects register allocation** [PROVEN, bonus calc_left_time]: `index = level_index +
  world_index` vs the reverse selects ESI vs EDI at entry.

- **Empty functions share the preceding function's aligned return** [PROVEN, audiocd]: start_cd_gameover's
  "extra `mov eax,eax`" was the alignment no-op before the `ret` of a missing empty function, start_cd_continue
  (1:0d024; DOS caller MAIN_CONTINUE_PRG 1:5a7ea; name from Android + PS1). Watcom falls through to the shared `ret`.
  Lesson: an unexplained no-op before `ret` can mean a missing (empty) function, not a codegen quirk.
- **Identical adjacent function bodies are merged** [PROVEN, objupdate1]: `u8 readNoArg(obj_t*){return 0;}` followed
  by an identical `skipNoArg` compile to one `xor al,al; ret` at 1:50afc; wcc386 emits readNoArg as a size-0 public at
  the same offset. Two C functions, one address - the binding is a compiler alias, not a duplicate symbol.

## Negative results (keep)
- Before start_cd_continue was found, ~40 variants of start_cd_gameover itself failed (return types, conventions,
  -d/-o flags, four compiler releases) - correctly: the cause was outside the function.
- `-ox`, `-oxa`, `-os`, `-ot`, `-oil`, `-or`, `-otr`, `-olr`, `-omr`, `-oir`, `-oxe`, `-oxn`, `-oxl+`, `-ox -oe=100`:
  myRand not STRICT. `-ob*` is not a 10.6 option.
- `myRand` with a local index (Rayverse shape) is never STRICT: 3-10 byte diffs (store/return order).
- `extern` vs defining the globals in the TU: no effect on these functions.
