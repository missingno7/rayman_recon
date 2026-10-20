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

## Open
- `_BSS`: order of uninitialised globals inside the central data TU(s). kegg_recon proved a Watcom 10.0 rule
  (size rank, symbol-table index group, identifier hash); re-prove it for 10.6 with probes before using it.
- `_DATA` object boundaries and order; pointer tables in `_DATA` (relocations inside data) need verification of
  data fixups (match.py currently marks such segments MISMATCH rather than claiming them).
- CONST alignment/padding between objects (bonus 13 bytes + pad -> next literal at 2:30a28: consistent with 4-byte
  or 8-byte alignment; not yet determined).
