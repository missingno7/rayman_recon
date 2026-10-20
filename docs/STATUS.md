# Status (Phase 2 - systematic DOS matching, 2026-10-20)

Live numbers: `python tools/verify.py && python tools/status.py` (config/units.json + the verifier are the record).

## Closure levels (kept separate)
| level | state |
|---|---|
| function-level STRICT | **127 functions, 12,374 code bytes** (bootstrap end: 34 / 1,900) in 16 code units; 80 NEAR_MATCH recorded |
| whole-object code closure | rand (1:5bf60), mathutil (1:4def0), bonus (1:13a00; incl. its CONST) - every function STRICT and one contiguous `_TEXT` |
| data closure | **25,532 bytes VERIFIED** (49 `_DATA` units + CONST of bonus and basic); mapped (named+sized bindings, not reconstruction): 89.7% of game `_DATA` |
| BSS | order rule PROVEN for Watcom 10.5/10.6; names largely original; no BSS unit reconstructed yet |
| linked binary / packed EXE | not started (by design) |

## Code units (GOG v1.21)
| unit | STRICT | notes |
|---|---|---|
| rand | 2/2 | whole object |
| mathutil | 12/12 | whole object |
| bonus | 8/8 | whole object, CONST VERIFIED |
| basic (core engine, 1:0d070) | 36/58 | object starts with deter_num_let's initialiser; CONST VERIFIED |
| stoneman | 14/24 | |
| cage | 9/10 | contiguous layout |
| audiocd | 8/11 | start_cd_gameover `mov eax,eax` OPEN (~40 falsified variants) |
| bat | 6/9 | |
| poisson | 6/8 | |
| mite | 5/11 | |
| blacktoon | 4/8 | |
| poing | 4/11 | |
| trumpet | 3/6 | |
| ufo | 3/5 | PS1 bodies (Rayverse stubs) STRICT unchanged |
| moskito | 3/14 | |
| luciole | 4/10 | Rayverse stubs; PS1 + binary |
Worker results not integrated (0 STRICT): cymbal (3 large functions), blood (DoEffectBloodOut).
Verifier self-tests 9/9 (incl. layout and data-tamper tests); `python tools/verify.py`: 0 regressions.

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
