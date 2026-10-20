# Status (Phase 2 continued - systematic DOS matching, 2026-10-20)

Live numbers: `python tools/verify.py && python tools/status.py` (config/units.json + the verifier are the record).
NEAR diagnosis: `python tools/census.py` (docs/CENSUS.md).

## Closure levels (kept separate)
| level | state |
|---|---|
| function-level STRICT | **503 of 780 recorded functions, 70,689 code bytes** (~15.4% of game code 1:00000-1:6fbe6) in 39 code units; 277 NEAR_MATCH |
| whole-object code closure | rand, audiocd, mathutil, bonus, clown, objupdate1 - every function STRICT |
| data closure | **25,741 bytes VERIFIED** (`_DATA` units + CONST of bonus, basic, input, jumelle, worldmap); mapped (named+sized bindings): 89.7% of game `_DATA` |
| BSS | order rule PROVEN; layout VERIFIED 6,919 bytes (rand, moskito, central-block palette prefix) |
| linked binary / packed EXE | not started (by design) |

Compiler profile: Watcom 10.6 `wcc386 -zq -oaxt -s -5r -fpc` (software FP proven by jumelle).
Bindings: 933 functions, 732 data. Tests: `python -m unittest discover -s tests` (15).

## NEAR_MATCH (277) by structural class (tools/census.py)
SHAPE / same structure 128 (logic right, Watcom shape left), REFS_LAYOUT 23 (bytes equal; follows object layout),
DIFF pointers 126 (calls/globals/fields/constants differ - logic, binding or prototype gaps, ~60% real).

## Code units (GOG v1.21)
| unit | STRICT | notes |
|---|---|---|
| rand | 2/2 | whole object ; _BSS VERIFIED |
| audiocd | 12/12 | whole object |
| mathutil | 12/12 | whole object |
| bat | 7/9 | |
| bonus | 8/8 | whole object ; CONST VERIFIED |
| trumpet | 3/6 | |
| ufo | 3/5 | |
| cage | 9/10 | |
| blacktoon | 4/8 | |
| poing | 4/11 | |
| poisson | 7/8 | |
| basic | 36/58 | ; CONST VERIFIED |
| mite | 5/11 | |
| moskito | 3/14 | ; _BSS VERIFIED |
| stoneman | 21/24 | |
| luciole | 4/10 | |
| special | 21/30 | |
| clown | 12/12 | whole object |
| fee | 1/8 | |
| dark | 16/22 | |
| saxo | 19/25 | |
| pmama | 19/28 | |
| meredenis | 3/18 | |
| collision1 | 24/37 | |
| collision2 | 12/36 | |
| objupdate1 | 46/46 | whole object |
| bbmont | 19/29 | |
| blocs | 18/23 | |
| objupdate2 | 34/51 | |
| map | 5/13 | |
| map_ngawe | 5/11 | |
| objupdate3 | 1/3 | |
| objupdate4 | 1/2 | |
| objinit | 1/7 | |
| breakout | 19/39 | |
| jumelle | 6/19 | ; CONST VERIFIED |
| input | 21/31 | ; CONST VERIFIED |
| worldmap | 46/50 | ; CONST VERIFIED |
| display | 14/32 | |
Not integrated: cymbal (3 large functions), blood (DoEffectBloodOut); untouched objects: tumap --list.

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
