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

## H2 - Phantom globals: Rayverse-derived bindings split original objects (PARTLY CONFIRMED, exhausted for now)
- CONFIRMED instances (fixed): init_data_state/record fields (lk_stage4); game_title = language_txt[231]; menu row 9 =
  text_to_display[0] (id_conflicts); inverse case: HMI aggregates spanning vendor publics (hm_names2). Effect: BSS
  holes 7 -> 5, +1,984 BSS bytes, last non-HMI unresolved name reduced to _language_txt.
- alias_audit (all 1,281 game-data bindings classified by DOS access): no further phantom proven; weak candidates
  listed in build/workers/alias_audit/CANDIDATES.md. not the cause of the credits_init declaration-index +2 (RESOLVED by idx_credits: 29..32-byte literals take two index entries
  and are not merged)
  (unchanged after the known consolidations). Remaining: 5 central BSS holes, language_txt capacity (237 used,
  Android 300) - both need independent size/ownership evidence, not adjacency.
- Next only with new evidence (e.g. original TU partition of the central data, an allocation size). New OPEN constraint:
  the central BSS needs 29 rank-2 index groups, the known publics allow 24 - ~5 groups of original declarations
  interleaved in the central TU are unidentified (headers/prototypes/statics?); never fill with dummy identifiers.

## H3 - Some objects were built with a different compiler profile (REFUTED, causal_profile)
- 11 objects without Rayman source ancestry (effect*, effecthelp, movieprefix, savecodec, dosmem, cdaudio) + 3 controls,
  168 whole-object builds over CPU -3r/-4r/-5r, optimisation subsets (-ox/-oaxt/-oneatx/-od), -s, -zp, releases
  10.0/10.5/10.6: zero STRICT gains; isolated diff reductions do not generalise. These objects use the game profile;
  their NEAR functions are the same allocation problem as the rest (H1 refuted, B4).

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
