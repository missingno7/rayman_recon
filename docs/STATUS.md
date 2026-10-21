# Status (Phase 2 continued - systematic DOS matching, 2026-10-21)

Live numbers: `python tools/verify.py && python tools/status.py` (config/units.json + the verifier are the record;
the per-unit table is printed by tools/status.py and is not duplicated here). NEAR diagnosis: `python tools/census.py`
(docs/CENSUS.md). Wave history and measured worker effectiveness: docs/CENSUS.md sections 2-5.

## Closure levels (kept separate)
| level | state |
|---|---|
| function-level STRICT | **1105 of 1511 recorded functions, 184,133 code bytes** (~40.2% of game code 1:00000-1:6fbe6) in 123 code units; 406 NEAR_MATCH, 1 CODE_BYTES_MATCH |
| whole-object code closure | rand, audiocd, mathutil, bonus, clown, objupdate1, objupdate3, block_malloc, init_memory, sprite_blocks, savecodec, cdsave, dosprobe |
| data closure | **30,027 bytes VERIFIED**; mapped (named+sized bindings): 89.7% of game `_DATA` |
| BSS | order rule PROVEN; layout VERIFIED 6,919 bytes |
| linked binary / packed EXE | not started (by design) |

Compiler profile: Watcom 10.6 `wcc386 -zq -oaxt -s -5r -fpc`. Bindings: 1683 functions, 1143 data. Tests: 17.

## Coverage
Every C range of the game code below 1:6fbe6 now has a unit except: the hand-written low-level assembly block
1:00000-1:06f80 (~28 KB; ABIs in docs/reference/lowlevel_abi.h; needs an assembly reconstruction path - not started)
and the anonymous prefix of the movie object 1:2a1c0-1:2b288 (34 routines; C-vs-assembly being decided).
Of 406 NEAR functions most have had two or more finishing passes; remaining blockers are register/stack allocation,
the F3 `rep movs` copy idiom (OPEN, not a compiler-release difference: 10.5 and 10.6 emit identical prefixes), clamp
lowering and object layout drift (each brief names the first size-drifting function).

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
