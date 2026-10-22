# Compiler and toolchain evidence

Evidence levels: PROVEN (measured, reproducible command), STRONG (several consistent observations),
HYPOTHESIS, OPEN. Probe sources live in `scratch/probe/` (ignored); conclusions here.

## Matching rules (read this first)
Distilled from ~100 worker notes (2026-10-21; counts = distinct units where the fix recurred). `tools/census.py`
prints the rule IDs to try for each NEAR function. One hypothesis at a time; record failures in your NOTES.md.

**Triage order - check the cheap outside causes before touching the body:**
1. **W-LAYOUT** code bytes already equal, only call/jump-table/self references fail (census `REFS_LAYOUT`, or a
   "missing call" whose target is in the same object): the cause is a *neighbour* - an earlier function's size, a
   missing/empty function, a switch table, a shared `ret`. Fix that function; leave this one. (~30 units, 60+
   functions; start_cd_gameover took ~40 variants before the missing empty start_cd_continue was found.)
2. **W-PROTO** caller-side difference (`test al/ax/eax`, extra `movsx/and 0xff`, wrong arg count/registers): fix the
   *callee's prototype in this unit*. Prototypes legitimately differ between original objects (BTYP is `u8` in blocs
   but `s16` in ray; instantSpeed `s16` in basic, `s32` elsewhere - a shared prototype header regressed 3-143 STRICT),
   so copy the declaration from a STRICT unit that calls the same helper the same way (`census --brief` lists them).
   (~20 units; on_block_chdir/test_allowed `s16` was re-derived 8 times.)
3. **W-BIND** a global/field reference differs: check the binding (true start, true type, Rayverse address
   conversion, Android size can be wrong) and the -5 short load (`mov r,[x-2]; sar r,16` = plain `short x`).
   (~15 units of wrong bindings; ~10 where a -2 addend was misread as another global.) The same trick loads a
   promoted *signed byte*: `mov r,[x-3]; sar r,24` reads `(s8)x` (collision1 slope, PROVEN) - a -3 displacement
   is not another field.
4. **W-PORT** size or calls differ substantially: remove Rayverse/PS1 port additions (debug, clamps, `&0x7f`, guards),
   restore DOS-only calls, missing (empty) functions. (~25 units)
Only then shape the body:

| ID | Symptom in the diff | Try | Not when / note |
|---|---|---|---|
| W-RETW | prologue push set differs; `cwde`/`movsx`/`and eax,0ffh` vs `xor ah,ah` | narrower **own return** (s16/u8/u16) | ~25 units (get_proj_x, but*pressed, MURDUR) |
| W-WIDTH | `cmp ax` vs `cmp eax`, movzx vs movsx, frame size | narrower/wider locals, loop index (s16), params, signed vs unsigned | ~25 units |
| W-UNUSED | extra EDX/EBX save, `ret 4` | add/remove an unused formal parameter as the DOS callers pass it | 7 units |
| W-OPORD | load order, `jbe`/`jae` swap, ESI vs EDI | write operands in the binary's load order (`a<b` vs `b>a`, `x+y` vs `y+x`) | ~25 units |
| W-THRESH | `cmp 2/jbe` vs `cmp 3/jb`, `test ax` vs `cmp ax,1` | other spelling of the same bound: `>9`/`>=10`, `>0`/`!=0`/`>=1`, `-x+c`/`c-x`, `/2` vs `>>1`, `~m` vs `0xff-m` | ~11 units |
| W-BRANCH | inverted jcc / swapped blocks | reorder if/else (often "death/zero-HP case first"); independent `if`s vs `else if`; keep DOS guards | ~22 units |
| W-STORE | store/mov scheduling | DOS statement order of field/global stores | ~20 units |
| W-TEMP | saved-register set, extra spill, reloads | usually *remove* a cached local/pointer (re-read `obj->x`, globals); sometimes a block-scoped s16/s32 snapshot. -oa caches and writes back globals itself - never add a manual writeback | ~28 units |
| W-DECL | stack slot / register colouring only | local declaration order or block scope; prefer block scoping - exhaustive permutation sweeps (24-720 orders, ~20 functions) succeeded ~2 times | ~15 units |
| W-BITF | and/shift vs `test byte`, clear/store sequences | bitfield view (union with `raw`) instead of mask arithmetic; `!flip` not `^` | ~13 units; shared views in obj.h/map.h/game.h |
| W-BOOL | `setne`/`and`, byte-register choice | natural boolean forms: `b=1; if(x) b=0;`, `!(a&&b)` vs `!a\|\|!b`, s32 flag local | casts that only force bytes (`?(s16)1:(s16)0`, `(u8)(a!=b)`) are rejected (OPEN) |
| W-LOOP | counter register, saved regs, `__STOSB` call | pointer walk with cached count; `for(;c;)` vs `while(c)` vs do/while; advance at bottom | ~16 units |
| W-SWITCH | extra tables, reloads | grouped labels vs duplicated bodies; remove DOS-absent empty cases (each emits a table entry); `case 0:` vs `case 0: default:`; a one-case switch is not an if | ~16 units |
| W-ABS | `cdq/xor/sub` vs `test/jge/neg` | Abs macro or ternary on a promoted `(s32)` operand; never `<stdlib.h>` abs/labs | ~11 units |
| W-COPY | `rep movsd` forms, `memset_` call | `memcpy` with `<string.h>` (intrinsic, F2 prefix) vs struct assignment per site; memset is a call; C zero loop -> `__STOSB`; `strcpy(x,"")`/`strcmp(x,"")` add CONST slots | F3 STOSD has no known source (OPEN) |
| W-ABI | wrong argument registers, caller saves around asm calls | ABI-only `#pragma aux parm [...] modify exact [...]` (src/include/lowlevel.h; docs/reference/lowlevel_abi.h) | 8 units |
| W-INTR | port/interrupt code as calls | `<i86.h>` `_disable`/`_enable` (adds CLD), `inp`/`outp`, `_dos_getvect`/`_dos_setvect`, `__interrupt __far`; else ORIGINAL_INLINE pragma (AGENTS.md) | 5 units |
| W-CONST | CONST segment mismatch | string literal order = first appearance in source; duplicates merged | input, worldmap |

**Do not try (measured dead ends):**
- Compiler options: `-on/-oi/-ol/-ol+/-om/-or/-oc/-ou` were probed in ~40 units / 100+ functions with **zero STRICT
  gain**; `-oe`/`-oe=N` (~12 units) always inlines and regresses; `-oh/-ok/-oi+` are rejected. Releases 10.0/10.5:
  identical to 10.6; 10.0a worse (~15 units). Measured over all 121 units / 1,497 functions (ai_decomp review
  2026-10-21): `-d1` no change; `-d2` changes every function, 0 gains; `-j` 1 loss (load GetStringTxt), 0 gains;
  `-ei` no change. The profile is settled: `-zq -oaxt -s -5r -fpc`.
- `inline`/`__inline`/`static __inline` (E1022) and C99 declarations: rejected by Watcom 10.6 (~11 units retried).
- `register` keyword: no effect in most of ~8 units.
- Exhaustive declaration-order or width grids on a function whose residue is a helper idiom (ashr16 toward zero,
  sgn, CL-count shifts, inclusive-range booleans, fill/copy): plain C never produces these; they are original
  inline helpers - use src/include/inline_helpers.h (AGENTS.md exception, section "Original inline helpers").
- Byte-forcing constructs (dead redundant init, empty cases, scratch clobbers, type-forcing block-scope externs)
  make bytes match but are rejected.

## Original inline helpers (ORIGINAL_INLINE, user decision 2026-10-21)
Five fixed-register byte sequences recur across the game and never occur in a STRICT function (counts against the
oracle, `python scratch/helpers_sites.py -v`): plain C never produces them (constant shifts through CL, constant bounds
loaded into registers, identical registers and jump distances at every site, materialised 0/1 results). They are
original inline helpers, declared once in src/include/inline_helpers.h (contracts and proofs:
build/workers/w6_helpers/REPORT.md; source of the finding: the ai_decomp review handoff, section 1).
| helper | DOS bytes (core) | sites / functions | contract | proof |
|---|---|---|---|---|
| ashr16 (shift toward zero) | `66 85 c0 78 05 66 d3 f8 eb 09 66 f7 d8 66 d3 f8 66 f7 d8` | 58 / 29 | AX value, CL count -> AX; `modify exact [ax cx]` (`[ax cl]` fails: frame) | DO_BAT_FLASH, frapsol STRICT |
| inrange (signed inclusive) | `39 d8 7c 0b 39 c8 7f 07 b8 01 00 00 00 eb 02 31 c0` | 44 / 26 | EAX in [EBX, ECX] -> EAX 0/1; `modify exact [eax ebx ecx]` | fades, mereDenisCanAttak STRICT |
| sgn | `83 f8 00 74 09 b8 01 00 00 00 7f 02 f7 d8` | 46 / 22 | EAX -> -1/0/1; `modify exact [eax]` | DoPoissonBleuRaymanZDD, DO_SURF_CHANGE_HAIRS STRICT |
| fill (64000-byte zero, no CLD) | `b8 00 00 00 00 b9 80 3e 00 00 f3 ab` | 11 / 9 | EDI; `modify exact [eax ecx edi]` | 4 effect1 functions, DEPART_LEVEL, confirmation_msg STRICT |
| copy (CLD; bytes then dwords) | `fc 89 c8 83 e1 03 74 02 f3 a4 89 c1 c1 e9 02 f3 a5` | 15 / 10 | EDI, ESI, ECX; advertised clobbers OPEN | bytes reproduce; full functions still NEAR |
Rules: use the header as declared (no per-unit redefinitions, no new helper pragmas); the CL/bounds setup is caller
code. UPDATE 2026-10-22 (build/workers/w10_proofs): the bare `shl eax,cl` form is now the accepted helper **ashl32** (proofs
set_ball_pos/breakout, RAY_GOING_BALANCE/ray); the sar32 JE-form and copy_guarded still have one proof each (OPEN).
Earlier status - OPEN candidates (one STRICT proof each, both in breakout; build/workers/w7_shift32/REPORT.md):
bare `shl eax,cl` after `mov cl,N` (4 sites / 2 functions; set_ball_pos STRICT in scratch) and a 32-bit
`sar r32,cl` JE-form body (9 bodies / 4 functions; move_ball STRICT in scratch), both EAX,CL -> EAX. They block
moteur instantSpeed and breakout set_ball_pos; accept only with a second-unit proof. Added 2026-10-21 (w8_mine, user approval): **palette_output** (`29 c1 41 6b c9 03 66 ba c8 03 ee 6b c0 03 ...` - OUT,
CLI, REP OUTSB, STI with the count arithmetic through fixed registers; EAX first, ECX inclusive last, ESI palette;
`modify exact [eax ecx edx esi]`; STRICT proofs InitPlasma, SYNCHRO_LOOP_PALETTE, actualize_palette) and
**min32/max32** (`39 d8 7e/7d 02 89 d8`, EAX/EBX, constant kept in a register by the caller; STRICT proofs
ChangeDeltaSizeJumelle, ChangeDeltaPosX/YJumelleWithoutLimit, effect_prepare_rows; advertised EBX clobber OPEN).
CORRECTION: the earlier note "clamp family is ordinary C" was wrong - its two STRICT counterexamples were a 16-bit
`66 39 d8` hit and a hit outside the function's true extent; plain C min/max ternaries fold the constant into the
compare instead. Families shown to be ordinary C (no helper): flag bitfield extraction `c0 e0 04 ... c0 e8 07`
(W-BITF), animation index arithmetic `8d 04 95 .. 29 d0` (struct indexing), menu screen call setup.
First integration (2026-10-21): +13 STRICT in bat, moteur, meredenis, poisson, ray, effect1, menu, cheat, 0 losses.

## Evidence log (established facts, with their proofs)
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

- **Floating point is software (`-fpc`)** [PROVEN, jumelle InitMatriceJumelle 372 bytes STRICT]: the original calls
  the CLIB3R helpers `__I4FD`, `__FDN`, `__FDM`, `__FDI4` and MATH3R `exp` (IF@DEXP, not MATH387R) in double
  precision: `ExpPoint[i] = (s32)(exp(-(double)i / 1024.0) * 65536.0)`. `-fpi` (default) / `-fpi87` emit x87 code
  plus a `__CHP` call (absent from the image). With `-fpc`, `-fp3`/`-fp5` and `-on` are non-discriminating.
  `-fpc` is now part of the game profile; all previously accepted units are unchanged by it (verify: 0 regressions).
- **Adjacent-function fallthrough** [PROVEN, jumelle]: ChangeRealPosJumelle compiles to a 28-byte prefix that falls
  through into ChangeJumelleVariable (1:33670), without `-oe`.
- **Empty same-TU callees vanish from callers** [PROVEN, jumelle]: an empty PrepareJumelleZoom body removes its call
  from DefaultJumelleVariable (which then regresses) - reconstruct non-empty callees before judging callers.
- **Hand-written drawing ABI** [PROVEN, jumelle DisplayJumellesFondNormal]: DisplayAnyPictureNormal takes ESI src,
  EDI dst, EAX/EBX src x/y, ECX/EDX dst x/y plus three callee-popped stack args; declared with an ABI-only
  `#pragma aux ... parm [esi] [edi] [eax] [ebx] [ecx] [edx]`.
- Release check on jumelle: 10.0, 10.5, 10.6 identical; **10.0a differs** in four bodies (none of them STRICT-relevant).

- **Grouped case labels vs duplicated case bodies** [PROVEN, objupdate2 DoPoingPowerupRaymanCollision]: identical
  C semantics, different Watcom code (grouped labels reload pointers and share stores across switches).
- OPEN, not accepted: objupdate2 TEST_SIGNPOST is reproduced only by a materialised boolean inside `&&`
  (`ray.main_etat == 2 && (ray.sub_etat != 8 ? 1 : 0)` or `(u8)(ray.sub_etat != 8)`); the plain form (as in the PS1
  matching source) and a local flag do not. No source evidence for that form (a macro/helper is possible) -> kept
  NEAR rather than forcing bytes. Same class (input, 2026-10-20): one joystick-recording function matches only
  with operand-typed ternaries `(buttons & 0x10) ? (s16)1 : (s16)0`; `? 1 : 0`, `!= 0`, TRUE/FALSE macros and an
  outer cast do not -> kept NEAR. Materialised-boolean shapes recur; an original typed-constant macro is a
  hypothesis to test when more cases accumulate.

- **Bitfield views of packed bytes** [PROVEN, basic]: `sprite_t.sprite_pos` low/high nibbles (calc_let_Width2) and
  `flags[type]` byte-1 bits 4/5 move_x/move_y (makeUturn, 1324 bytes) only match as bitfield reads - mask arithmetic
  on the raw byte compiles differently (same lesson as block_flags). Shared as unions with a `raw` view.

- **Callee body moved beside a later caller, cold epilogue left behind** [PROVEN, cdaudio 2026-10-21]: with the game
  profile, a non-adjacent earlier-defined callee can have its entry/fall-through body emitted next to a later wrapper,
  leaving only a cold common epilogue at its own definition position; branch nesting decides which body moves. DOS:
  cold code 1:1a493..1a49c, late entry 1:1a734 (same in v1.12 at 1:1a2e8/1:1a048, so not a GOG patch). Consequence for
  W-LAYOUT: a function's DOS extent may include another function's cold return - measure extents from the disassembly,
  not from symbol spacing. Probes: build/workers/w4_cdaudio (before_pool_probe.c, pool_location_probe, pool_nested).

- **Unused prefix vs postfix decrement of a short differ** [PROVEN, ray_pc 2026-10-21]: a statement `--x;` and `x--;`
  on an s16 whose value is unused compile to different code under the game profile; DOS updateLogo (1:61940) needs the
  postfix form (312 -> 304 bytes, STRICT). Treat `++x/x++` spelling as a shape choice to read off the DOS bytes (W-TEMP).

- **TEST register after a copy** [OPEN, r_testreg 2026-10-21]: DOS tests EAX where candidates test EDX/ECX in
  DO_STONEWOMAN_COMMAND, IS_MIT_PAF, Display_and_free_luciole, RAY_BALANCE_ANIM (1-2 bytes from STRICT). Observed rule:
  Watcom may test the SOURCE register of a preceding copy even when the condition names the destination variable.
  Negatives (no fix): assignment-in-condition, value widths, liveness after the test, compound updates. Evidence:
  build/workers/r_testreg/REPORT.md. Do not re-probe these spellings.

- **Register/stack-slot allocation residue** [OPEN, r_regalloc 2026-10-21]: 46 probes over 15 residues in 8 units found
  no rule with two cross-unit STRICT proofs: no universal EBX/ESI/EDI/ECX order, no source-level slot formula. The
  later Open Watcom allocator ranks conflicts by estimated savings (bld/cg/c/regalloc.c) - consistent with the
  observations, not proof of 10.6's costs. Further declaration permutations repeat the documented dead end; a new
  discriminator needs the 10.6 allocator's cost/tie behaviour. Evidence: build/workers/r_regalloc/REPORT.md.

- **Stack homes: ascending raw size, rounded to 4** [PROVEN, r_owcg 2026-10-22]: Watcom 10.6 assigns stack homes to
  locals sorted by ascending raw size (allocations rounded to 4 bytes), not by first use; declaration order only breaks
  ties among equal sizes (probe p_size: char[3], short[3], int[3] -> homes 20, 12, 0, frame 24, all exact). Use it to
  read a DOS frame: the slot order tells the size ranks of the locals (W-DECL/W-WIDTH). Also PROVEN: register choice
  is use/savings-sensitive (a value with more post-call uses avoids spilling; reversing a consuming expression swaps
  EDI/EBP), byte-typed values can recolor earlier inputs, stack reuse needs non-address-taken temps (lexical braces
  alone do not reuse). OW2's general register order list is REFUTED for 10.6. Evidence: build/workers/r_owcg/REPORT.md.
  Applied to 9 units (r_slots): 0 new STRICT - DOS dword accesses do not prove a wider declaration, and parameter
  homes / split aliases block simple rank inversion; useful to read frames, not a fix by itself.

## Undetermined / next discriminators
- `-oi`, `-ol`/`-ol+`, `-om`, `-on`: non-discriminating on ~40 units (see "Do not try"); only revisit with concrete new
  evidence (e.g. an FP function whose residue changes under `-on`: box CalcTab 71 -> 64 diffs, still NEAR).
- OPEN: the DOS `CLD; REP MOVSB` + dword copy sequence in jumelle is not produced by Watcom 10.6 inline memcpy (F2
  strings) - a different helper or source idiom.
- Compiler release for v1.21 game code: 10.0, 10.0a, 10.5 and 10.6 give identical code on everything tested
  (only the 10.6 libraries match the v1.21 runtime). The 10.5 compiler was recovered from the CD's packed
  `DISKIMGS` set (`C:\tools\watcom-10.5-pck`, docs/TOOLCHAIN.md).
- `-ei` enums, `-j` char signedness: settled - `-ei` no change, `-j` 1 loss / 0 gains over all units (see "Do not
  try").
- Toolchain pitfall: WCC386 10.6 (BINNT) crashes with a register dump when source/working paths are ~150
  characters long (fine at ~50). Keep worker scratch paths short (build/workers/<short>/, build/tmp).
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
- **String literals go to CONST in order of first appearance; duplicates are merged** [PROVEN, input load_record]:
  the oracle order FILE NOT FOUND / "DANY" / greeting / prompt / ".REC" is reproduced only when "DANY" appears
  before the greeting in the source (`char *default_name = "DANY";` at the top of load_record); the CONST segment
  (113 bytes) then VERIFIES. The literal order is evidence for the statement order.
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

- NEGATIVE / not accepted (user decision 2026-10-22): absolute value `85 c0 7d 02 f7 d8` (test eax,eax; jge; neg eax)
  occurs at 200 sites, 34 of them in 26 STRICT functions generated by plain C. Plain-C forms tested at the three blocked
  sites (dark DARK_phase1 +0x15, DO_DARK_SORT_COMMAND +0x11, hybrid doMOSAMScommand +0x10c): stdlib abs/labs with and
  without intrinsic, `#pragma function`, ternary forms, s16/s32 operands - none close them. An EAX-only helper would, but
  is rejected as allocation steering (build/workers/r_abs). The blocker there is context (allocation), not the idiom.

## Cross-unit register/stack allocation investigation (2026-10-22; build/workers/ra_tax, ra_probe, ra_pairs)
- Taxonomy (ra_tax, PROVEN mechanical classification of all 301 NEAR): register renaming only 0; stack displacement
  only 3; instruction order only 7; extra/missing instructions only 2; reference/layout only (bytes equal) 17; mixed
  272. There is no pure register-permutation class: register choice is entangled with instruction selection and
  frame layout, so no single colouring rule can close a group of functions.
- TEST/CMP after a register copy (ra_probe, STRONG scoped predictor, 44/44 sites in 33 STRICT functions / 23 units):
  for `mov dst,src` immediately followed in the same machine basic block by `test r,r`/`cmp r,imm`, the comparison
  uses the SOURCE register's part at the comparison width; at a join (branch target) it uses the destination; if the
  source is EBP and the comparison is word-sized, the destination (BP is not a selectable word register: 5,938 word
  operands in 71 units, zero BP). It predicts from emitted MOVs, not the MOV's allocation from C, and does NOT explain
  the five OPEN residues (DO_STONEWOMAN_COMMAND, IS_MIT_PAF, Display_and_free_luciole x2, RAY_BALANCE_ANIM).
- Spill cost (PROVEN probes, STRONG interpretation): uses inside one more loop level weigh about 20x (crossover at
  20/21 uses at depth 0 vs 1 and again at 1 vs 2). Not a validated source-rewrite rule.
- Stack homes: ascending raw size (rounded to 4) stays PROVEN, but equal-size ties are NOT a declaration-order rule -
  removing an unrelated array or changing the width of a register-resident local flips tie order (hidden backend
  records participate). Incoming stack parameters keep 4-byte ABI slots (22/22 accesses, 33/33 callee pops).
- NEGATIVE (ra_pairs, 12 NEAR in 12 units, 72 trials with STRICT controls): staging temporaries broke 10/12 controls,
  operand reordering 6/12, predicate extraction 12/12; 0 functions closed. NEGATIVE (ra_probe): moving branch results
  before a join (RAY_BALANCE_ANIM) and naming the scanner lookahead pointer (deter_num_let) - no gain.
- Reference/layout-only NEAR (ra_eclass, PROVEN for all 17): 13 functions in 11 units carry a FOLLOWING function's
  switch jump table inside their extent; the table entries fail because that later function's internal blocks
  differ, even when object base and the table owner's bytes agree - they close only when the later body is fixed
  (W-LAYOUT refinement, not a binding fix). The other 4 (basic deter_num_let, cheat LOAD_CREDITS_VIGNET, menu
  AFFICHE_BUTTON_JOYSTICK, AFFICHE_VIDEOMODE_SCREEN) reference automatic-aggregate initialisers at the object's _TEXT
  prefix, which are correct; earlier size drift moves the implied base. Class b (3 displacement-only functions, 26
  differences mapped): no source fix validated.
- Switch layout (sw_layout, PROVEN on 13 blocked owners / 17 tables / 202 entries and 85 tables in 68 STRICT functions):
  our case-entry order, grouping (shared targets incl. repeated defaults), table placement and RET counts already equal
  DOS for all 13 table owners of reference-only NEAR functions; their wrong table destinations come from local
  instruction-LENGTH changes inside the bodies: byte-register choice changes encodings (accumulator AL/AX absolute
  loads/stores and AL immediate compares are 1 byte shorter: 8 of 13 owners), [esp] vs [esp+4] homes, scaled
  addressing vs LEA/SHL, CSE snapshots, predicate lowering. So B1 reduces to allocation/lowering (no case-order fix).
  W-SWITCH limits: case bodies are not sorted by value (moskito tellNextMoskitoAction 2,3,5,12,default); grouped and
  duplicated bodies may be indistinguishable after optimisation (blocs init_calcbloc_func); no 4-byte case alignment
  (573 entries unaligned); a case block may lie OUTSIDE its public extent via tail sharing/fallthrough into the next
  function (neige DoNeigeRaymanCollision: case 31/default -> previous RET 1:4c3ab, case 30 -> after set_SNSEQ_list).
- Special instructions in C code (h_int, PROVEN): 209 sites / 54 functions / 19 units scanned; 194 already reproduced
  (all IN/OUT, REP OUTSB, CLI/STI, debug registers, INT3, PUSHFD/POPFD, IRETD). Watcom 10.6 intrinsics: inp/outp inline
  through DX (even for immediate ports), _disable = CLI, _enable = STI;CLD, int386 = marshalling call. Six real INT
  sites; only LOAD_CONTINUE_SCREEN 1:2662a/2663f missing (DRM disk check, AX/BX results: every tested C carrier spills -
  OPEN, kept out of src). 13 CLD sites (movieprefix x7, luciole plot2line x4, effect633e0 x2) are missing in our builds:
  a CLD-only pragma reproduces them but gains no function (the surrounding clear/copy bodies differ) - NEGATIVE.
  outpw for effect1 SaveScreen/RestoreScreen: NEGATIVE (adds promotion, 10->84 diffs).
- REP string instructions (h_fill, PROVEN inventory: 287 sites / 99 functions / 26 units; 156 in 56 STRICT functions):
  plain-C origins in STRICT code are the `<string.h>` memcpy intrinsic (`57 89 c8 c1 e9 02 f2 a5 8a c8 80 e1 03 f2 a4
  5f`, 94 sites), str* intrinsics (F2 SCASB, 32), and aggregate assignment / automatic aggregate initialisation
  (F3 MOVSD, 14 sites in 8 functions / 4 units - this answers the earlier OPEN "F3 rep movs" question: F3 MOVSD comes
  from struct/array aggregate copies, not memcpy). Watcom's 386 string.h does NOT make memset intrinsic; explicit
  `#pragma intrinsic(memset)` emits F2 word/byte stores. The remaining NEAR fill/copy variants (PC main-loop clears 16008
  and 4000 dwords, movieprefix CLD/XOR/STOSD x3 in one unit, luciole zero-tail x4 in one function, colour fills,
  rectangle PUSHAD copies, guarded copy) are singletons or single-unit: no helper meets the gate; seven bounded plain-C
  trials (zero aggregates, colour fills, guarded copies, zero-tail fills) gained nothing - NEGATIVE, source OPEN.
- Byte-register choice (ra_byte, corpus 5,910 byte instructions in 729 STRICT functions): PROVEN probe facts - a
  direct memory predicate/increment (`if (a==2||a==3)`, `++a`) uses AH where the same code through an explicit `u8 x = a`
  snapshot uses AL; independent byte values share one dword's lanes (AL/AH, DL/DH, CL/CH - 70 witnesses in 61 STRICT
  functions); a byte surviving an opaque call goes to DL, then BL, then CL, then a stack home as more byte arguments
  occupy registers. High-byte registers are first-class allocation results (AH does not imply a packed word). NEGATIVE:
  "reserve AL, take the next free byte register" predicts 220/834 (26%); no general predictor. Applying snapshot
  removal reproduced DOS's DH (collision1 1:1e0e2) and AX (meredenis) locally but neither owner closed (0/8 switch owners).
- Cross-version NEAR check (v112_near, all 301 NEAR mapped to DOS v1.12): 0 functions STRICT on v1.12 but NEAR on v1.21;
  275 have identical DOS bytes in both versions (our mismatch is ours, no version information); 26 differ between the
  versions only by already-reproduced edits (glyph ABI, protection, archive banks, palette state). NEGATIVE as a lever.
