# Data object (obj2) organisation and verification

Generated evidence: `python tools/datamap.py` (needs `tools/tumap.py` output). GOG v1.21 numbers below.

## Layout of obj2 [STRONG; runtime anchors are PROVEN byte matches]
| region | evidence |
|---|---|
| 2:00000..2:2d998 game `_DATA` (initialised globals, all game objects) | first runtime `_DATA` member (`cstrt386`) is at 2:2d998 |
| 2:2d998..~2:2ff00 runtime `_DATA` | 8 CLIB modules' `_DATA` found exactly once (iob, dosret, iomode, stk386, tzset, signl, ...) |
| ~2:2ff00..2:317f4 game `CONST` (string literals) | 156 literals referenced by a single code object form 25 runs in `_TEXT` link order (1 inversion, a runtime object) |
| 2:317f4.. runtime `CONST` | math2err, noefgfmt, _matherr, chipbug, initfile, nan_inf, abort |
| 2:31c40.. `CONST2` | istable, timeutil, scale, fpow10 |
| 2:31ec4.. `DATA32` | chipd32, chipt32 |
| 2:3218a..2:59960 BSS + stack | PMW1 object table (initialised size 0x3218a, virtual 0x59960; SS:ESP = 2:59960) |

So WLINK emitted DGROUP as `_DATA`, `CONST`, `CONST2`, `DATA32`, then BSS, each class concatenated in object link order
(game objects first, runtime after) [STRONG; consistent with every anchor].

## Where globals are defined [STRONG]
Data referenced by exactly one code object is NOT clustered per object (e.g. load.c-only references span
2:0215c..2:310e0): almost all game globals are defined in a few central data translation units (Rayverse's single
`data.c` reflects this), not in the code object that uses them. String literals are the exception: wcc386 puts
them in the referencing object's own `CONST`, so a TU's literals are verifiable together with its code.
Consequence for reconstruction: code TUs declare globals `extern`; data TUs (initialised `_DATA`, `_BSS`) are a
separate reconstruction target whose object boundaries are not known yet.

## Verification implemented
`tools/match.py` / `tools/verify.py` report every data segment of a compiled object:
VERIFIED (whole contents equal the oracle at one base implied by all references and data bindings; no unverified
relocations inside), MISMATCH, UNPLACED, BSS (not checked). Accepted VERIFIED segments are recorded in
`config/units.json` (`segments`) and protected by the zero-regression gate; `tests/test_match.py` has a tamper test.

| unit | segment | bytes | status |
|---|---|---|---|
| bonus | CONST (`"/%2d : %02d/"`) | 13 | VERIFIED at 2:30a18 |

Found while mapping: the bonus worker had declared this literal as an external array `bonus_time_format`; the address
lies inside bonus's own CONST run, so the original source used a literal. Lesson for workers: a referenced
address inside the object's CONST run is a string literal, not a global.

## BSS order and original names [STRONG]
The Watcom `_BSS` rule (docs/COMPILER.md, PROVEN for 10.5/10.6) sorts uninitialised globals by size rank, then
declaration-index group (groups of 25, later groups first), then hashpjw(name) % 241 ascending. Applied to the 640
BSS globals that Rayverse data.c names with address and simple type: among 530 pairs adjacent in memory with the
same size rank, 425 have ascending name hashes and 105 descending (random names: ~50/50). The descents match the
expected number of group resets (~25 groups x size ranks). Hence (a) the Android-derived names are, by and large,
the original identifiers, and (b) the BSS is mostly one large central data TU (~640+ declarations). This makes a
verified reconstruction of the central BSS feasible: the rule turns names + declaration order into addresses.
Probe: `tools/probes/bss_names.py`.

## Initialised `_DATA` coverage [measured]
Named objects with known size (Rayverse data.c addresses with Android sizes, plus Android content matches from
`tools/androiddata.py`) cover 166,573 of the 186,776 bytes of game `_DATA` (2:00000..2:2d998), 89.2%, in 203 objects
with no conflicting overlap (the single overlap is an excluded 8-byte C++ library constant matched by chance).
47 gaps remain; the largest are 2:00000..2:01957 (6.5 KB, data of the hand-written low-level code at 1:00000..),
2:035fa..2:04a74 (5.2 KB) and 2:01c9a..2:0251c (2.2 KB). Because `_DATA` is laid out in definition order, the
named objects in address order already give the definition order of the central data TU(s).

Policy question for the data stage: reconstructing initialised data means writing the initial values as C
initialisers. Those values can only come from the oracle (or Android/PS1 where identical). This is ordinary source
reconstruction (the original source contained these literals), but it must be expressed as typed initialisers with
symbolic pointers, verified by the data-segment check - never as raw byte blobs.

## Reconstructed `_DATA` (src/data_coll.c, src/data/blk_*.c) [VERIFIED]
49 data units, 25,489 bytes of game `_DATA` VERIFIED (plus 43 bytes of verified CONST in code units): typed tables and
structures from Rayverse data.c corrected against the oracle (build/workers/data_rv/NOTES.md lists every change).
Pointer initialisers (e.g. cptr_tab: 38 command-handler functions, sequence tables) are verified as relocations.
Kept external as assets (not reconstructed): PalPlasma 2:05f70 (48 B), PalFire 2:05fb2 (48 B), Def_Bande
2:080fc..2:2b37c (144,000 B level band table). Open: ObjectsFonctions pointer table 2:035fa..2:04a74 and
t_world_info (pointer targets unbound), smaller unclassified gaps.
DOS-vs-Rayverse value differences found: timeCd (87 of 132 entries; Rayverse's CD timing table is a port change),
PC_TingsToGet_Col/ColAdd = 1 on DOS (Rayverse 0), cheat code table bytes 0xb1 at 4 indices (Rayverse's `_()` macro
yields 0x27), mapCd is 132 bytes (Rayverse 128, so its timeCd address comment is 4 bytes early), effect_speed is
u16 (Rayverse u8). Rayverse declarations wider than DOS (Android sizes + byte accesses in DOS code):
nb_levels_in_world u8[7], LastTouched u8, speed_ball_level u8.

## Open
- Central data TU(s): reconstruct declaration order (index groups) from the BSS layout + names; flag globals whose
  hash breaks an ascending run as candidates for wrong names.
- `_DATA` object boundaries and order; pointer tables in `_DATA` (relocations inside data) need verification of
  data fixups (match.py currently marks such segments MISMATCH rather than claiming them).
- CONST alignment/padding between objects (bonus 13 bytes + pad -> next literal at 2:30a28: consistent with 4-byte
  or 8-byte alignment; not yet determined).
