# Status (Phase 2 continued - systematic DOS matching, 2026-10-21)

Live numbers: `python tools/verify.py && python tools/status.py` (config/units.json + the verifier are the record;
the per-unit table is printed by tools/status.py and is not duplicated here). NEAR diagnosis: `python tools/census.py`
(docs/CENSUS.md). Wave history and measured worker effectiveness: docs/CENSUS.md.

## Closure levels (kept separate)
| level | state |
|---|---|
| function-level STRICT | **1151 of 1552 recorded functions, 192,300 code bytes** (~42.0% of game code 1:00000-1:6fbe6) in 124 code units; 401 NEAR_MATCH |
| whole-object code closure | rand, audiocd, mathutil, bonus, clown, objupdate1, objupdate3, block_malloc, init_memory, sprite_blocks, savecodec, cdsave, dosprobe, trumpet, poisson |
| data closure | **30,058 bytes VERIFIED**; mapped (named+sized bindings): 89.7% of game `_DATA` |
| BSS | order rule PROVEN; layout VERIFIED 6,919 bytes |
| linked binary / packed EXE | not started (by design; first step only: PMODE/W 1.22 PMWLITE determinism, docs/NEXT_STEPS.md) |

Compiler profile: Watcom 10.6 `wcc386 -zq -oaxt -s -5r -fpc`. Bindings: 1715 functions, 1155 data. Tests: 29.

## Original inline helpers (2026-10-21)
Six ORIGINAL_INLINE helpers in src/include/inline_helpers.h (ashr16, inrange, sgn, fill, copy, ashl16; AGENTS.md
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
Every C range of the game code below 1:6fbe6 now has a unit except the hand-written low-level assembly block
1:00000-1:06f80 (~28 KB; scoping report build/workers/w5_asmscope/REPORT.md: ~119 routines, WASM 10.5/10.6 run
natively, assembler identity OPEN, pilot 2/2 STRICT; not started) and four effect drafts without a unit (1:633e0,
1:6c3b0, 1:6e380, 1:50350 partly). Remaining NEAR blockers: object layout drift (first size-drifting function per
object), register/stack allocation, clamp lowering, materialised booleans.

## Proven in Phase 2 (docs/COMPILER.md, docs/DATA.md)
- Structs pack to 1 byte by default; bitfields LSB-first; `_DATA` items naturally aligned (short 2, int 4).
- Intrinsics via Watcom `<string.h>` are part of `-oaxt` (inline memcpy); local aggregate initialisers live at `_TEXT`
  offset 0; `_BSS` order rule (size rank, 25-declaration groups, name hash) holds for 10.5/10.6.
- Source-shape rules that convert Rayverse/PS1 bodies: narrow return widths (s16 get_proj_x), comparison operand order
  is source order (obj_hurt), callee prototype widths change callers, branch/predicate/switch order, bitfield reads
  differ from mask arithmetic (block_flags), declaration order.
- obj2 layout: game `_DATA` 2:00000..2:2d998, runtime `_DATA`, game `CONST` in `_TEXT` link order, `CONST2`, `DATA32`,
  BSS 2:3218a. String literals belong to the referencing object's CONST.

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
