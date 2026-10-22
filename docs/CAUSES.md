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

## H2 - Phantom globals: Rayverse-derived bindings split original objects (TESTING: worker alias_audit)
- Observations (3 PROVEN instances so far): init_data_state/record fields were bound as separate globals (lk_stage4);
  game_title is language_txt[231] (id_conflicts: DOS ToDoAtExit reads slot 231); menu_to_display "row 9" is
  text_to_display[0] scratch (9 menu rows, not 10). Each caused an overlap/unresolved name; fixing them closed BSS
  (+44, +1,940 B) with no STRICT change. Remaining: 1,505 BSS bytes uncovered, language_txt capacity OPEN (237 used,
  Android 300), credits_init declaration-index model off by +2 mod 25 (tu_central).
- Hypothesis: more bound "globals" are interior elements/fields of original arrays/structs (Rayverse names every
  address it saw). Phantom globals add symbol-table declarations that the original TU did not have -> the BSS
  declaration-index groups drift (credits_init +2?), holes and overlaps appear, and fragments cannot be merged into a TU.
- Links to H1: both are "our declaration set differs from the original".
- Smallest experiment: classify every data binding by DOS access pattern (only ever accessed as base+k of a neighbour,
  same stride as an adjacent array, address inside a typed extent); count candidates around the credits_init and the
  remaining holes; remove/merge the proven ones and recompute the declaration-index model - does the +2 disappear?
- Would unblock: central BSS TU(s), remaining holes, natural-link BSS placement.

## Confirmed / refuted causes (short)
- CONFIRMED: switch-table blockers (13 owners) = allocation-driven instruction length, not case order (sw_layout).
- CONFIRMED: census DIFF tags were mostly tool blind spots (t_census), not semantic errors.
- CONFIRMED: first natural-link divergences were structural (asm slicing, data ownership, const table), not NEAR drift.
- CONFIRMED: HMI link blocker = wrong assembler/object format; original library resolves it.
- CONFIRMED (instances of H2): game_title = language_txt[231]; menu row 9 = text_to_display[0]; init_data_state/record fields.
- REFUTED: pure register renaming class (0 functions); case-order hypothesis; v1.12 source changes as NEAR cause;
  generic source rewrites (staging, operand order, predicate extraction) as cross-unit rules.
