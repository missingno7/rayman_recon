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

## H4 - A source-available compiler is codegen-equivalent to Watcom 10.6 (REFUTED, cc_equiv)
- Whole corpus with frozen 10.6 headers, STRICT C entries vs DOS: wc106 1,345, wc105 1,345 (10.5 = 10.6 codegen),
  wc100 1,198, Watcom 11.0 307, Open Watcom 1.4 77. 11.0 and OW 1.4 already change register allocation in myRand and
  getbit. OW source cannot model 10.6 allocation; 10.6a is byte-identical to 10.6 (only WLINK changed).

## H5 - The 10.6 tie rule can be read from the compiler binary (CONFIRMED mechanism, cg_re)
- Recovered: unstable Shell sort over all backend names for homes and conflicts, savings-only conflict order, colouring
  rule with EBX-before-ECX, byte rover pre-increment (AH first), loop cost 20^depth (docs/COMPILER.md). Validated on
  thousands of real decisions. Our NEAR choices are now predicted exactly from OUR records; what remains is which
  ORIGINAL source produced DOS's record graph.

## H6 - Original locals/capacity our source lacks (PARTLY CONFIRMED, h6_locals; follow-up h6_capacity)
- REFUTED for unused scalars: an extra unused local creates no backend record (22 -> 22 records, same sort/homes) - it
  cannot change ties (2 no-effect trials; 1 worsening with a pointer local).
- CONFIRMED for CAPACITY: frame/sub_3BE20 - DOS reserves 24 bytes with a 4-byte gap after three written context words;
  context[3] -> context[4] gives STRICT (9 -> 0 diffs). A larger DOS frame is evidence of larger original aggregates
  (arrays/structs), which change home offsets and sizes.
- Scan: 39 of 301 NEAR functions have larger DOS frames. h6_capacity audited all 39: 31 have no local aggregate, none of
  the other 8 supports a further capacity edit (movieprefix sub_3AF20's tail follows a pointer - owner OPEN). EXHAUSTED.

## H7 - Inverse allocation: derive the record order DOS needs, map it to source (CONFIRMED in an instance, inv_alloc)
- With the 10.6 rules known (H5), a pure home/register tie in a NEAR function can be inverted: which backend record
  set/order produces DOS's choice under the unstable sort and colouring rule? Map the required records to source
  variables/temporaries and change the source accordingly (a derived edit, not a grid).
- Result: effecthelp effect_helper_168c8 STRICT - equal savings (122) between descriptor and points, descriptor must
  colour first; inverting the EARLIER name sort shows the local declaration order item/i decides it (the obvious swap of
  the two pointers provably does nothing). Non-homed names shape home ties in 4 units (18/18 homes). Three-address
  expansion temporaries can survive into the home sort (AfficheEntete) - some inverses need a change before
  instruction expansion. Next: scale the method across NEAR functions with pure ties (workers inv_*).
- Scaled run (inv_a/b/c/d, 39 units): 1 further closure - ray stackRay (snapshot locals reproduce the byte-rover history
  and equal-savings conflict order); 2 closures in total from ~95 small-diff candidates. Typical limit: the tie needs a different SET of local
  conflicts/records (created by the frontend from a different expression/statement shape), not a different order, so a
  declaration permutation cannot reach it; derived constraints are retained per function (inv_*/NOTES.md). One partial:
  stoneman GetY register pair recovered by a derived inverse but the function stays NEAR (scheduling).
- B5 TEST residues LOCATED (inv_a): the DOS EAX sign predicate exists after global allocation and LdStAlloc; the Score
  pass rewrites it to the copied source register (IS_MIT_PAF ECX, DO_STONEWOMAN_COMMAND EDX, Display_and_free_luciole
  EDX x2). The original Score input differs; which source produces it is OPEN.

## Confirmed / refuted causes (short)
- CONFIRMED: switch-table blockers (13 owners) = allocation-driven instruction length, not case order (sw_layout).
- CONFIRMED: census DIFF tags were mostly tool blind spots (t_census), not semantic errors.
- CONFIRMED: first natural-link divergences were structural (asm slicing, data ownership, const table), not NEAR drift.
- CONFIRMED: HMI link blocker = wrong assembler/object format; original library resolves it.
- CONFIRMED (inverse of H2, HMI): our invented aggregates spanned the vendor library's individual data publics; replaced
  by the original publics (hm_names2) - ownership must follow the original definitions in both directions.
- CONFIRMED (H2-type, undersized object): 2:32190 is the 48-byte DPMI 0500h memory-info record (implicit API writes hid its
  size; Android DPMIMemoryInfo 0x30) - our 4-byte pointer binding left a 44-byte gap (dt_cells).
- CONFIRMED (instances of H2): game_title = language_txt[231]; menu row 9 = text_to_display[0]; init_data_state/record fields.
- REFUTED: another third-party library beyond Watcom runtime and HMI SOS (lib_audit coverage map).
- REFUTED: pure register renaming class (0 functions); case-order hypothesis; v1.12 source changes as NEAR cause;
  generic source rewrites (staging, operand order, predicate extraction) as cross-unit rules.
