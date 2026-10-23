# Status (2026-10-23)

Live numbers: `python tools/verify.py && python tools/status.py` (config/units.json and the verifier are the record).
Priorities: docs/NEXT_STEPS.md. Blockers: docs/BLOCKERS.md. Causes: docs/CAUSES.md. Compiler facts: docs/COMPILER.md.

## Coverage (kept separate)
| level | state |
|---|---|
| game code 1:00000-1:6fbe6 | **264,283 of ~457,702 bytes STRICT (57.7%)**: 1,520 STRICT functions (game C + all 106 original hand-written asm entries); every range has a unit |
| remaining game NEAR | ~292 functions, ~182 KB: ~225 SHAPE (calls/globals/fields/constants equal DOS; code shape only), ~30 DIFF_* (possible behaviour or binding differences - triage first), ~20 layout-only (close with their neighbours), ~18 not traced by explain.py |
| HMI SOS vendor library | original library pinned and linked; src/hmi is a separate reconstruction (not game code) |
| Watcom runtime | pinned CLIB3R/MATH3R/MATH387R, linked as-is |
| initialised data / CONST | 50,227 bytes VERIFIED; 94.1% of game `_DATA` mapped by named, sized bindings |
| BSS layout | 151,789 bytes VERIFIED (incl. language_txt at its proven 237-entry minimum) |
| natural link | **complete**: all units + pinned libraries -> WLINK 10.6 -> image, no unresolved symbols; /C4 packing works; not byte-identical (NEAR drift/layout) |
| packer | PMWLITE 1.22 /C4 rebuilds the original file from its decoded objects (PROVEN) |

Profiles: game C Watcom 10.6 `wcc386 -zq -oaxt -s -5r -fpc`; game asm WASM 10.6. Tests: `python -m unittest discover -s tests`.

## Policy and baseline
Since 2026-10-23 any valid, semantics-preserving C that the unchanged verifier reports STRICT is accepted
(docs/MATCHING.md); per-function `quality` records COMPILER_STEERED or SYNTHETIC source. The readable reconstruction
before that change is tag `semantic-baseline-2026-10-23`.

## Tools in use
explain.py (allocator/Score/rover/home/colour/selection/schedule mechanisms per difference), cgtrace.py, census.py
(SHAPE vs DIFF classification), permute.py (guided source permuter, docs in tools/permute.md), match.py / verify.py
(strict gate), link.py / manifest.py (natural link), seed.py (new objects from PS1/Rayverse).

## References used
Rayverse and the PS1 decomp (CC0) as semantic and structural sources; Android Rayman Classic symbols for names and
data sizes; v1.12, the v1.12 Preview, FR v1.21 and five demo builds for cross-version evidence (docs/VERSIONS.md).
