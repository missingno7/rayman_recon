# Causal map of the remaining blockers (lightweight, living)

Purpose: find what we still misunderstand about the original source/build, so one fix removes a class of blockers.
Each entry: observations -> hypothesis -> who else might share it -> evidence for/against -> smallest discriminating
experiment -> what it would unblock. Status: OPEN / TESTING / CONFIRMED / REFUTED. Keep refuted entries (negatives).

## H1 - Unrelated declaration context steers allocation (REFUTED in controlled tests, causal_symtab)
- Test: 4 frozen synthetic bodies (equal-size homes, call-live spills, competing byte values, CSE) x 30 prefix variants
  (extern count up to 241, order, hash collisions, typedefs, prototypes, static/extern, enums, preceding functions,
  literals): 120/120 identical bytes and fixups; a real store-order change (positive control) does change registers.
  Real code: 6 NEAR functions from 6 units compiled in-unit / alone / headers reordered / extra extern: 18/18 identical.
- Mechanism separated: the frontend declaration index (BSS order) IS context-sensitive; executable allocation is NOT.
  The function-local tie mechanism (ra_probe) remains OPEN and is not the frontend index.
- Consequence: reconstruct header sets/declaration order only where BSS ownership needs it; do not use extra/arbitrary
  declarations, Android order or PS1 file boundaries as an allocation fix. The credits_init +2 belongs to H2 (data).

## H2 - Phantom globals: Rayverse-derived bindings split original objects (TESTING: worker alias_audit)
- Observations (3 PROVEN instances so far): init_data_state/record fields were bound as separate globals (lk_stage4);
  game_title is language_txt[231] (id_conflicts: DOS ToDoAtExit reads slot 231); menu_to_display "row 9" is
  text_to_display[0] scratch (9 menu rows, not 10). Each caused an overlap/unresolved name; fixing them closed BSS
  (+44, +1,940 B) with no STRICT change. Remaining: 1,505 BSS bytes uncovered, language_txt capacity OPEN (237 used,
  Android 300), credits_init declaration-index model off by +2 mod 25 (tu_central).
- Hypothesis: more bound "globals" are interior elements/fields of original arrays/structs (Rayverse names every
  address it saw). Phantom globals add symbol-table declarations that the original TU did not have -> the BSS
  declaration-index groups drift (credits_init +2?), holes and overlaps appear, and fragments cannot be merged into a TU.
- Relation to H1: H1 is refuted for code, so the declaration-set question matters only for data/BSS (this hypothesis).
- Smallest experiment: classify every data binding by DOS access pattern (only ever accessed as base+k of a neighbour,
  same stride as an adjacent array, address inside a typed extent); count candidates around the credits_init and the
  remaining holes; remove/merge the proven ones and recompute the declaration-index model - does the +2 disappear?
- Would unblock: central BSS TU(s), remaining holes, natural-link BSS placement.

## H3 - Some objects were built with a different compiler profile (TESTING: worker causal_profile)
- Observations: lib_audit (2026-10-22) finds no third-party library beyond Watcom and HMI, but 24 KB of code without
  Rayman source ancestry (effect34580/50350/633e0/6c3b0/6e380/6ec80, effecthelp, movieprefix FLC decoder, cdaudio MSCDEX
  part, savecodec, dosmem). Those units are among the worst matched (effecthelp 1/5, effect6c3b0 1/5, other effect
  units 3/5) although their code is small. HMI proved a linked component can use other flags (-od -s -3r).
- Hypothesis: these objects (possibly an in-house effects/FLC/CD module) were compiled with a different option set
  (CPU target -3r/-4r/-5r, optimisation subset, stack checks, -zp, possibly another Watcom release).
- Against: the game profile was settled on game units; earlier per-unit passes on these objects found allocation
  differences only.
- Smallest experiment: per OBJECT (not per function), compile the unchanged source with a small, justified set of
  alternative profiles and compare total diff counts across all its functions; a real profile difference shows as a
  large, consistent improvement across the whole object, not a single function.
- Would unblock: whole groups of effect NEAR functions; per-object profile records in config/units.json.

## Confirmed / refuted causes (short)
- CONFIRMED: switch-table blockers (13 owners) = allocation-driven instruction length, not case order (sw_layout).
- CONFIRMED: census DIFF tags were mostly tool blind spots (t_census), not semantic errors.
- CONFIRMED: first natural-link divergences were structural (asm slicing, data ownership, const table), not NEAR drift.
- CONFIRMED: HMI link blocker = wrong assembler/object format; original library resolves it.
- CONFIRMED (inverse of H2, HMI): our invented aggregates spanned the vendor library's individual data publics; replaced
  by the original publics (hm_names2) - ownership must follow the original definitions in both directions.
- CONFIRMED (instances of H2): game_title = language_txt[231]; menu row 9 = text_to_display[0]; init_data_state/record fields.
- REFUTED: another third-party library beyond Watcom runtime and HMI SOS (lib_audit coverage map).
- REFUTED: pure register renaming class (0 functions); case-order hypothesis; v1.12 source changes as NEAR cause;
  generic source rewrites (staging, operand order, predicate extraction) as cross-unit rules.
