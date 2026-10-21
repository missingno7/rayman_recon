# Status (Phase 2 continued - systematic DOS matching, 2026-10-22)

Live numbers: `python tools/verify.py && python tools/status.py` (config/units.json + the verifier are the record;
the per-unit table is printed by tools/status.py and is not duplicated here). NEAR diagnosis: `python tools/census.py`
(docs/CENSUS.md). Wave history and measured worker effectiveness: docs/CENSUS.md.

## Closure levels (kept separate)
| level | state |
|---|---|
| function-level STRICT | **1385 recorded STRICT, 262,197 code bytes** (57.3% of game code 1:00000-1:6fbe6), C + hand-written assembly: 301 NEAR; every game code range now has a unit (new: sysutils 1:6f3b0 replacing block_malloc/init_memory/sprite_blocks, blood 1:13800); `python tools/status.py` for the per-unit table |
| whole-object code closure | rand, audiocd, mathutil, bonus, clown, objupdate1, objupdate3, block_malloc, init_memory, sprite_blocks, savecodec, cdsave, dosprobe, trumpet, poisson; the whole hand-written block 1:00010-06f7e (8 asm slices) |
| data closure | **47,143 bytes VERIFIED** `_DATA`/`CONST` (incl. the 5.2 KB ObjectsFonctions table, gap blocks, menu/savecodec/block_malloc CONST); game `_DATA` gaps left: only the 400-byte anonymous zero run 2:01970 (in lowlevel_data.asm) and external assets |
| BSS | order rule PROVEN; layout VERIFIED **148,811 of 152,302 game-BSS bytes (97.7%)**, 2:3218a..2:57478; runtime (CLIB) BSS starts at 2:57478 (PROVEN, atexit `_BSS`). Fragments in src/bss/ are layout units, not original TU boundaries; 3.6 KB OPEN |
| linked binary / packed EXE | not started (by design; first step only: PMODE/W 1.22 PMWLITE determinism, docs/NEXT_STEPS.md) |

Compiler profile: Watcom 10.6 `wcc386 -zq -oaxt -s -5r -fpc`. Bindings: 1715 functions, 1155 data. Tests: 29.

## Original inline helpers (2026-10-21)
Ten ORIGINAL_INLINE helpers in src/include/inline_helpers.h (ashr16, inrange, sgn, fill, copy, ashl16, palette_output, min32, max32, write_movie_palette; AGENTS.md
exception, docs/COMPILER.md "Original inline helpers"). Applied at every recorded DOS site in 37 units. This pass:
1105 -> 1151 STRICT = helpers +27, new movieprefix unit +18, display CONST fix +1; 0 regressions. Most remaining helper sites have exact helper bytes displaced by size
drift of earlier code in the same function (fix the first size-drifting function, W-LAYOUT). copy's advertised
clobber list and ashl16's CL/CX width remain OPEN.

## Integrator
tools/integrate.py fails closed (ai_decomp review 2026-10-21, two adversarial re-reviews): no silent name
de-duplication (refuse or explicit --rename recorded as an immutable alias), claim index in promote.py, contained
destinations, exact rollback of written files only, literal-pathspec commit of written files only, local headers
replaced only when private to the run's units. tests/test_integrate.py.

## Coverage
Hand-written assembly 1:00010-06f7e (28,526 bytes) is reconstructed as 8 contiguous `.asm` slices, 106 entries, all
STRICT (lowlevel_00010, _02640, _03125, _04430, _04ab7, _05609, _0611e, _066df; WASM 10.6 via tools/wcc.py build;
readable instructions, typed state only), plus lowlevel_data (2:00010-0215c, VERIFIED). OPEN: historical assembler
identity and original module count (slices are not claimed as original objects); consolidation into one module
would also reconcile the two mask-table names at 1:0311d/03121 (local labels in _02640, extern names in _03125);
record-field and timer partitions; runtime callback payloads (table 2:34268). 1:00000-00010 is Watcom cstrt386
BEGTEXT (runtime). Remaining C work: 311 NEAR functions (layout drift, register/stack allocation, clamp
lowering, materialised booleans) and four effect drafts without a unit (1:633e0, 1:6c3b0, 1:6e380, part of 1:50350).

## Proven in Phase 2 (docs/COMPILER.md, docs/DATA.md)
- Structs pack to 1 byte by default; bitfields LSB-first; `_DATA` items naturally aligned (short 2, int 4).
- Intrinsics via Watcom `<string.h>` are part of `-oaxt` (inline memcpy); local aggregate initialisers live at `_TEXT`
  offset 0; `_BSS` order rule (size rank, 25-declaration groups, name hash) holds for 10.5/10.6.
- Source-shape rules that convert Rayverse/PS1 bodies: narrow return widths (s16 get_proj_x), comparison operand order
  is source order (obj_hurt), callee prototype widths change callers, branch/predicate/switch order, bitfield reads
  differ from mask arithmetic (block_flags), declaration order.
- obj2 layout: game `_DATA` 2:00000..2:2d998, runtime `_DATA`, game `CONST` in `_TEXT` link order, `CONST2`, `DATA32`,
  BSS 2:3218a. String literals belong to the referencing object's CONST.

## Cross-version: DOS v1.12 (2026-10-22, build/workers/v112/REPORT.md)
The 1,359 v1.21 STRICT functions (86 units) rebuilt unchanged against the `dos_v112_us` oracle (worker overlay
v112_symbols.json, unmodified matcher): 1,290 STRICT on v1.12 (93.2% of their bytes), 9 relocation-only, 44 changed,
12 missing (CD-protection revision), 4 ambiguous. PROVEN source/build changes between the versions: MakeMyRand drops
`& 0xEFFF` in v1.12 (rand object STRICT with that one change); `FatalError` is STRICT on v1.12 only with the 10.5
headers (errno declared `volatile` in 10.6 errno.h/stdlib.h - header-sensitive output, not a different backend);
screen resource IDs, cheat strings, joystick calibration text index; asm_04d55 is an empty stub in v1.12; DetectCpu
saves EDX around CPUID only in v1.21. Not integrated (no v1.12 build target yet).

## Shared types (src/include)
obj.h (obj_t 0x84, eta_t, anim/sprite, zdc_t, level_t, level_obj_t, active_objects_t), game.h (status_bar_t, rgb,
world_info_t, poing_t, rayevts_t, finbosslevel_t), map.h (map_tile_t, map_data_t, block_flags_t). Offsets pinned by
tests/test_layout.py (PROVEN vs RAYVERSE marked). Corrections vs Rayverse: offset_hy u8 (two units), block_flags
bitfield union. PS1's Obj layout is different (not used).

## Cross-platform transferability (worker records, ~170 functions)
CONFIRMED_SAME_LOGIC ~52, LIKELY_SHARED (source-shape changes only) ~83, CONFIRMED_VARIANT ~20, PC_ONLY ~12.
Typical conversions: return widths, operand/branch order, Rayverse port adaptations (strncpy for memcpy, safety
clamps, widened globals, ogg/CD replacements). Curated rows: config/xref_evidence.json.

## Other references added
Android Rayman Classic/Anniversary symbols and APK: original names, true data sizes (gate), initialised-data
content matches (61 bindings), x86 code by name (semantic reference). Android widened a few globals and uses
natural struct alignment; DOS evidence overrides (dos_size_evidence).

## Unresolved
- basic 22 NEAR (register/stack allocation, range-helper shapes, layout drift); moskito/poing/mite frame and
  register allocation; cymbal (large functions, frame layout); blood; start_cd_gameover; bat_dir jump-table offset.
- Whole-object layout conflicts block intra-object reference verification in several units.
- Data: ObjectsFonctions pointer table (2:035fa..2:04a74) and t_world_info pointers unbound; BSS not reconstructed;
  `_DATA` object boundaries unknown (blocks are invented splits); hand-written low-level code/data at 1:00000/2:00000.
- No WLINK / PMWLITE experiments yet.
