# Structural census and PS1-reuse investigations (2026-10-20)

Question: can more value be extracted from the PS1 matching decompilation (and from what we already have) without
growing the regex seeder? Three bounded investigations; all numbers measured on the GOG v1.21 oracle with the
strict verifier unchanged. Start state: 450 STRICT / 298 NEAR (748 recorded). End state: **503 STRICT / 277 NEAR
(780 recorded), 70,689 code bytes, 0 regressions.**

## Tool kept: `tools/census.py` (structural comparison, diagnostic only)
`python tools/census.py` compiles every accepted unit and, for each NEAR function, compares candidate and DOS bytes
on structure rather than bytes: ordered callees, data targets, struct displacements, program constants, prologue.
Normalisations (each proven on real cases, tests/test_census.py): `-5` short loads (`mov r,[x-2] ... sar r,16`)
are the same target; width masks (0xff/0xffff), width shifts (16/24), 0/1 and +-1 comparison rewrites are not
program constants; CONST literals and own-`_TEXT` references are folded. `python tools/census.py --brief UNIT`
writes `build/near_census_UNIT.md`, the per-function brief a finishing worker reads first.
Ghidra (`C:\tools\ghidra_*`) was not needed: capstone + OMF fixups + the oracle fixup table suffice.

## 1. Cross-platform semantic alignment (PS1 C vs DOS)
Method: compile the adapted PS1 body in its DOS unit, classify against the DOS bytes with the census features.
Ground truth (a): worker transferability labels; (b) **retrospective**: the 67 functions that went NEAR -> STRICT
in git history - classify the old source, compare with the kind of edit that fixed it.

| verdict on old source | fix was shape-only | fix changed calls/globals/fields/constants |
|---|---|---|
| SAME structure (SHAPE/REGALLOC/...) | **28** | 2 |
| REFS_LAYOUT | 8 | 0 |
| DIFF | 14 (incl. 3 where the real fix was outside the function: an empty callee, a missing empty function) | 16 |

- **Reliable direction**: "same calls/globals/fields/constants" => the remaining work is Watcom shape (93%). That
  removes the most expensive repeated reasoning ("is this a different algorithm?") for 45% of NEAR functions.
- **DIFF is a pointer, not a verdict** (~55-63% right in retrospect and in the two guided passes). It must be
  confirmed in the DOS disassembly. Misses come from extents (unbound neighbours), switch-table shape, bitfield
  byte addressing, loop-bound constants.
- PS1 side: labelled variants flagged 16/19; labelled same-logic flagged DIFF 29/138 (mostly real differences of
  the adapted seed: field/flag mapping, PS1-only helpers). 197/487 labelled PS1 bodies do not compile in DOS
  context (see 3).

## 2. NEAR_MATCH diagnosis (298 functions at start)
| group | n | first-pass only | after a finishing pass |
|---|---|---|---|
| A bytes equal, only intra-object references (layout) | 24 | 15 | 9 |
| B same structure, small shape gap (sim >= 0.85) | 48 | 23 | 25 |
| C same structure, larger shape gap | 76 | 50 | 26 |
| D data pointers (globals / fields / constants) | 105 | 81 | 24 |
| E call pointers | 45 | 42 | 3 |
**211 of 298 had never been investigated** (first-pass units); 87 are persistent blockers in finished units.
Shared root causes found and fixed:
- Hand-written assembly ABIs (`src/include/lowlevel.h`: sprite_clipping, DisplayAnyPictureNormal, N_CLRSCR) and a
  hidden parameter: `init_build_map(map, MAP_BLOCKS)` - DOS passes EDX and the callee clobbers it unsaved
  (parameter registers are not callee-saved) -> STRICT. A worker proved 101 of 104 assembly-routine ABIs
  (`docs/reference/lowlevel_abi.h`); measured gain in current units **0** (callers are not yet integrated), so the
  table is kept as reference and applied unit by unit.
- Duplicated local types consolidated into shared headers: record_t (3 copies), mem_t (4), credit_t (DOS stride 12),
  display_item_t, options_jeu_t, save_state_t, bb_attack_entry_t, scene_func_t.
- Unbound callees named by the census pointer (RAY_TOMBE, RAY_HURT, sko_get_eject_sens: Rayverse addresses).
**Effort comparison** (same worker model gpt-6.1-sol high, one bounded pass per unit):

| finishing pass | NEAR solved |
|---|---|
| 8 earlier unguided passes (special, saxo, dark, blocs, pmama, basic, bbmont, jumelle) | 4-17 each, **mean 46%** |
| census-guided worldmap (22 min) | **26 of 30 (87%)**, 20 -> 46/50 |
| census-guided collision1 (33 min) | **13 of 26 (50%)**, 11 -> 24/37 |
| census-guided collision2 (19 min) | **16 of 24 (67%)**, 12 -> 28/36 |
| census-guided objupdate2 (23 min) | **13 of 17 (76%)**, 34 -> 47/51 (one more rejected: forced boolean form) |
| census-guided breakout (27 min) | **3 of 20 (15%)**, 19 -> 22/39 - C written from disassembly (no PS1, Rayverse stubs) |
| census-guided display (21 min) | **11 of 18 (61%)**, 14 -> 25/32 |
| census-guided meredenis (29 min) | **10 of 15 (67%)**, 3 -> 13/18 (one forced boolean form rejected) |
| census-guided basic (32 min, second pass on a finished unit) | **9 of 22 (41%)**, 36 -> 45/58 |
| census-guided moskito (22 min) | **5 of 11 (45%)**, 3 -> 8/14 |
| census-guided input (12 min) | **6 of 10 (60%)**, 21 -> 27/31 (one more rejected: forced typed-ternary form) |
| census-guided jumelle (9 min, second pass, PC-only FP code) | **1 of 13 (8%)**, 6 -> 7/19 |
| **guided total (11 passes)** | **113 of 206 (55%)** vs 46% unguided; weakest where no reference C exists (breakout, jumelle) |
The worldmap pass shows the upper end: the brief told it which functions were logic gaps (missing calls/guards/
globals) and which were pure shape, so it did not search shape on logic gaps or vice versa.

## 3. PS1-only symbol recovery
Pilot: the 13 seeds that failed to compile because of PS1 names. Method (scratch/ps1sym.py, not kept as a tool):
declare missing globals from the PS1 headers (typed externs), compile, align the ordered global-reference events
of candidate and DOS (bound globals as anchors) and read the DOS target at each unknown reference; accept only if
all votes agree, the address is free, the Android symbol exists and fits, and the DOS access width agrees.
- Compiling: 0/13 -> 8/13 (PS1 externs + shared record_t/scene_func_t); new STRICT: **0** (similarities 0.17-0.6:
  these PS1 bodies differ from DOS - skops/sko_* object untouched, input functions are PS1 platform variants).
- Bindings recovered: 3 (sko_ecroule_plat 2:3f804, sko_rayon_dx 2:3f8e8, sko_lave_obj 2:3eee0), all Android
  symbols, widths agree. PS1-only state (PS1_DisableInputs, PS1_* , unnamed D_801xxxxx) was correctly NOT bound.
- Ceiling: of 197 PS1-seed compile failures in unit context only 26 are undeclared names; the rest are PS1->DOS
  field/flag mapping (E1079/E1080 49+24, unknown fields 35, parameter counts 13) - seeder-rule territory, which
  was deliberately not expanded. **Stopped: poor return.** The event alignment is the method to use when the
  skops object is reconstructed.

## How to use (workflow)
New object: `tools/seed.py draft` (unchanged). Finishing pass: `python tools/census.py && python tools/census.py
--brief UNIT`, give the brief to the worker (TEMPLATE_FINISH): DIFF_calls -> DIFF_globals/fields -> SHAPE by
similarity; REFS_LAYOUT functions are skipped (they follow the layout).
