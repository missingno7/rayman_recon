# Causal map of the remaining blockers (lightweight, living)

Purpose: find what we still misunderstand about the original source/build, so one fix removes a class of blockers.
Each entry: observations -> hypothesis -> who else might share it -> evidence for/against -> smallest discriminating
experiment -> what it would unblock. Status: OPEN / TESTING / CONFIRMED / REFUTED. Keep refuted entries (negatives).

## H1 - Our TUs' symbol-table context differs from the original (TESTING: worker causal_symtab)
- Observations: (a) _BSS order depends on a declaration index counting every prior symbol-table entry incl. headers
  (PROVEN; one extra extern in game.h broke bss_palettes); (b) equal-size stack-home ties flip when an unrelated
  declaration is added/removed - "hidden backend records participate" (ra_probe, PROVEN); (c) the declaration-index
  model fails by +2 mod 25 exactly inside credits_init (tu_central); (d) 244 SHAPE NEAR functions resist every
  source-shape rewrite and probe (ra_tax/ra_pairs/ra_probe/ra_byte negatives).
- Hypothesis: Watcom 10.6 breaks allocation ties (register colour, stack homes, maybe CSE/spill choice) by symbol or
  temporary creation order; our units include large shared Rayverse-derived headers (game.h, obj.h, ...) and different
  extern/prototype lists and function order than the original files, so ties resolve differently.
- Would also explain: some "allocation" NEAR in many units; switch owners' byte-register lengths (B1); B5 TEST residues.
- Against: ra_probe's lookahead-naming trial (deter_num_let) changed nothing; generic rewrites never helped.
- Smallest experiment: identical function body, vary ONLY the declarations before it (count/order of externs,
  typedefs, prototypes, preceding function definitions) and check whether its register/home choices change at all.
- Would unblock: if confirmed, reconstruct original header/declaration context (original per-file includes, extern
  order) as a structural fix across units instead of per-function work.

## H2 - Central data ownership mistakes behind BSS holes, credits_init index anomaly and overlapping bindings (OPEN)
- Observations: 7 BSS holes / 2,988 B; language_txt (300 pointers) overlaps game_title etc.; menu_to_display overlaps
  text_to_display; credits_init +2 index anomaly; 103 BSS fragments cannot be placed by any link order.
- Hypothesis: some bound "globals" are fields/sub-arrays of larger original objects (or vice versa), and the true objects
  belong to TUs we have not reconstructed; the same wrong partition creates holes, overlaps and index drift.
- Smallest experiment: pending id_conflicts (identity of the overlapping objects); then test whether the corrected
  identities close holes and change the index count in the predicted direction.
- Would unblock: whole-BSS central TU(s) -> natural-link BSS placement; last non-HMI unresolved names.

## Confirmed / refuted causes (short)
- CONFIRMED: switch-table blockers (13 owners) = allocation-driven instruction length, not case order (sw_layout).
- CONFIRMED: census DIFF tags were mostly tool blind spots (t_census), not semantic errors.
- CONFIRMED: first natural-link divergences were structural (asm slicing, data ownership, const table), not NEAR drift.
- CONFIRMED: HMI link blocker = wrong assembler/object format; original library resolves it.
- REFUTED: pure register renaming class (0 functions); case-order hypothesis; v1.12 source changes as NEAR cause;
  generic source rewrites (staging, operand order, predicate extraction) as cross-unit rules.
