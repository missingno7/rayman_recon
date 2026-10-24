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
  EDX x2). The original Score input differs; which source produces it is OPEN. cg_score read the Score rule from the
  binary: equivalence-ring substitution, severed by redefinitions, reset at joins. C forms that create a join or
  redefinition DO flip the TEST, but change many other bytes - the original's surrounding structure differs too.
- Construct -> record table (inv_shape, 58 probes; docs/COMPILER.md "C constructs and backend records"): naming, compound
  assignment, result reuse, snapshots, pointer post-increment, casts change record count/ownership/order in calibrated
  ways; but six full-unit trials on constrained functions gained nothing - record count/ownership alone does not fix the
  combined colour/home/scheduling constraints. STATUS: allocation (B4) is now well understood mechanically, but the
  original record graphs remain unrecoverable from the bytes without more source evidence. EXHAUSTED for now.

## H8 - Switch owners' byte registers come from case SOURCE order incl. merged duplicate bodies (1 proof; sw_rover 0 more)
- fee DO_FEE_ETAPE closed (whole fee object 8/8): a separate first `case 0` with default's body adds one early rover
  request; merged identical blocks hide it in the final code. B1 (13 switch owners; 8 with byte-register length
  changes) may share this cause - sw_layout verified entry order, not the source order of merged bodies.
- sw_rover (13 functions / 11 units, 668/668 rover choices replayed): exact per-function request offsets computed
  (e.g. moskito needs one unexcluded request between requests 6 and 7; bbmont COMMAND one before 55; ray's removal is
  cancelled by a new AL exclusion), but no source construct inserting exactly those requests was found; duplicated
  bodies/separate labels insert requests at other points. Constraints retained in build/workers/sw_rover/NOTES.md.
- Focused wave (mos/bb/saxo/ray_rover, 2026-10-22): 0 closures, constraints sharpened. moskito: request 7 is case 26's
  already-correct AL clear; the gap is between requests 46 and 47 (case 12's clear). bbmont COMMAND: insertion must be
  among requests 39-46; guard splits recover the registers but leave extra emitted tests; COMMAND2: no single
  insertion/deletion/rotation fits. saxo: request 16 serves FinAnim with live DX = x_pos (excludes DL/DH); boolean
  assignment breaks the prefix, nested conditions add a request that is absorbed at request 8. ray: the snapshot holds
  AL across joy_done++ (the AL exclusion cancels the removed request); a one-case switch restores DL/BL/AH but counter,
  dispatch and predicate differences remain. STATUS: EXHAUSTED for these owners without new source evidence.

## H9 - Most NEAR functions have several independent allocator causes, not one (explain_sweep, 2026-10-23)
- tools/explain.py over all 295 game NEAR functions (277 traced; 18 attribution failures: 2 sound snd_sqrt_table,
  16 frame): class A (one isolated cause) 2 - mite IS_MIT_PAF (Score, joint payoff with DO_MIT_ATTAK/DO_MIT_COMMAND),
  ray RAY_BALANCE_ANIM (Score); class B (2-3 cause fingerprints, no structural finding) 4 - ufo
  DO_MOVING_WITH_INDICATOR_COMMAND (colour), basic calc_btyp_square / INIT_TXT_BOX (home sort), and one Score case;
  class C (structural or mixed) 271. Functions with at least one finding: size/structure 255, colour tie 220,
  colour decision 152, rover 137, home sort 108, Score 71, selection 22.
- Consequence: per-function single-edit fixes are available only for a handful of functions; the bulk differs in IR
  shape (size/structure) before allocation. Report: build/workers/explain_sweep/sweep.md (+ out/<unit>_<function>/).
- Follow-up wave fx_ufo / fx_basic / fx_small / fx_score (2026-10-23): 0 closures, 0 losses. ufo: one equal-savings
  (61) conflict pair needs the opposite temporary creation order; two natural conditional forms fix EDX but add 368
  diffs. basic: 108/108 home positions replay; equal-count source variants keep wrong homes. moskito/special: 5
  variants all worse. Score: ring confirmed at 5 sites, 4 candidates rejected. The A/B set is EXHAUSTED without new
  source evidence; small-diff triage also found box CalcTab = compiler anomaly (B17), not a source error.

## H10 - Pure instruction-order differences come from the scheduler (REFUTED as a source-order lever; sched 2026-10-23)
- The 10.6 scheduler is recovered (docs/COMPILER.md). Of the 7 pure-reorder NEAR functions, 2 have DOS orders that are
  illegal for our DAG and 5 are legal orders that the priority rules rank differently; either way the scheduler's
  input (IR, value lifetimes, address-taken status, generation rows) differs, so the fix is upstream, not in
  statement order. Natural trials (direct return in dosmem, moving the
  second-screen computation in effect_helper_50738, a typed workspace view) gained nothing.

## H11 - Structural debt: local matching drifted from shared source idioms (structural experiment 2026-10-23)
- Premise check (scratch/core_ops.py over all 291 game NEAR functions; "core" = arithmetic/compare/branch/call
  mnemonics, moves/push/pop/lea/frame adjustments ignored): 88 functions (40 KB) already have DOS's exact core
  (category 4: allocation/homes/scheduling only); 108 (70 KB) differ by <= 4 core ops; 95 (81 KB) by more. bbmont 6/9,
  collision1 SET_DETECT_ZONE_FLAG, blocs IS_ON_RESSORT_BLOC are core-equal: structural rewrites cannot help them.
- Recurring op substitutions across functions point to idioms: abs (our cdq/xor/sub vs DOS test/jge/neg), branch
  polarity (function-specific), fill/copy (rep stos/movs).
- PROVEN (abs): DOS game code uses ONLY the branching abs form (43 units; the branchless form occurs only twice, in
  sound). Our sources had intrinsic abs()/labs() in 7 units, abs-wrapping macros in 3 and ~28 per-file Abs/ABS macro
  variants in 5 shapes. Branch exp/abs-idiom (worktree): replacing intrinsic routes gives the DOS neg at 11/11 affected
  functions; unifying every macro on the PLAIN form loses STRICT (blocs, mite, ray) - the (s32) casts are real; unifying
  on the CAST form ((s32)(x) >= 0 ? (s32)(x) : -(s32)(x)) keeps every STRICT except one layout casualty (moteur
  DO_PLACE_RAY, 0 code differences) and improves the core in 9 units, incl. 2 not used to derive it (frame 105->98,
  moskito 11->5, both also fewer differing bytes); neige worsens (its variant tests < 0). Byte differences rise where
  earlier compiler steering had been tuned around the wrong idiom (dark DARK_phase1 91 -> ~800); 3-minute permuter runs
  on 5 corrected functions closed none (calc_esquive_poing 128->103 vs old 29 flat).
- FALSIFIED: switch-dispatch idiom (31/34 dispatches already match; 2 functions differ); C-level fill idiom (memset is
  inlined as stosw/stosb; parameterised pragmas load ECX first and XOR EAX - DOS's mov eax,0; mov ecx,N; rep stosd keeps
  both constants inside hand-written inline code: counts 16000 x11 sites (existing fill helper), 4000 x1, 16008 x1).
- Islands (2026-10-23): Watcom compiles each function in isolation - a callee's body, storage class or definition
  order never changes a caller (probe: identical caller code for 4 variants). Cross-function code channels are only
  shared declarations (types, struct fields, prototypes, pragma contracts) and source idioms; layout couples bytes
  only within one object. Measured: all 289 NEAR functions access exactly DOS's struct-field offsets, and with
  relocated operands excluded the field access widths/signedness match DOS (scratch/field_widths.py); 206/289 use only
  STRICT-validated globals/callees; decl_audit checked the other 139 symbols: 121 consistent, 13 undetermined,
  5 mismatches (word_CF7E6 u16, new_sprite_deform/PLAN1_BUFFER_START_POINTER u8*, init_data_state pointer members,
  droppedBombIds s16[8][8]) - correcting them changes no compiled code. False leads retained: first-divergence
  clustering on `ray` (branch-target shifts) and the menu display_item_t cluster (unlinked vs linked displacements).
  OPEN: 62 globals carry CONFLICTING extern types across units (decl_unify) - a real one-object-one-type defect.
- Conclusion: structural debt is real but narrow (one demonstrated idiom, ~14 functions directly, ~9 units of core
  improvement); most remaining NEAR are category 4. A correct idiom does not by itself yield STRICT - it resets the
  allocation work for the affected functions.

## H12 - Wave-3 synthesis: closures are local; the reusable lever is layout dependency (2026-10-24)
Read-only synthesis of build/workers/w3_*/FINDINGS.md (25 closures, ~98k compiles, ~18 worker-hours) plus a fresh census
(build/near_census.json: 242 game NEAR = 182 SHAPE, 18 REFS_LAYOUT, 38 DIFF/CALL_ORDER).
- Closure edits: declaration order (8), operand commute (6), accepted abs/ashr helper at DOS-cited sites (7), local
  width flip (3), address-taken local copy (5, SYNTHETIC), snapshot removal (3) / addition (1), store reorder (3).
  Each is a known allocator knob (home sort, rover, colour); almost all closures needed 2-3 COUPLED edits whose
  single-edit ablations fail. No new compiler mechanism.
- REFUTED as a transferable lever (predictive test, 2026-10-24): converting the remaining plain-C abs sites to
  abs32_inline in the three closest NEAR functions with DOS TEST/JGE/NEG EAX sites made all worse
  (Display_and_free_luciole 2->58, DO_BBL_COMMAND 5->17, moskitoDropFruitOnRay 11->276): there the ternary already
  emits the DOS bytes. The helper is a coupled knob, not a standalone fix.
- HYPOTHESIS (inconsistent evidence): PS1-decomp-inherited snapshot locals are wrong for Watcom (3 removals closed
  PAR_REACT_TO_RAY_IN_ZONE, move_ball, set_rubis_list/Do_Effet_Chaleur; one addition closed DO_TOTEM_COMMAND).
- PROVEN reusable mechanism: a reference is verified at our object's internal offset, so a function whose bytes are
  equal becomes STRICT as soon as the code it references sits at the DOS offset - the referenced function itself
  may stay NEAR. Instances: DO_CORDE_COMMAND stays NEAR (worse) yet its 5 case labels at DOS offsets close
  DO_POELLE_COMMAND; set_rubis_list extent 452 closes Do_Effet_Chaleur; menu key/video initializer extents close
  AFFICHE_BUTTON_JOYSTICK and DO_COMMANDE_GRAPHIC (w3_menu/collateral_pair, not integrated: conflicts with CalcSpeed).
  Carrier -> owner map (census deltas, ours - DOS): IS_MIT_PAF -> DO_MIT_COMMAND (+8 uniform), IS_ON_RESSORT_BLOC ->
  CALC_MOV_ON_BLOC (-4 uniform), swapMereDenisCollZones -> prepareNewMereDenisAttack (+1 uniform), standard_frontZone
  -> SET_DETECT_ZONE_FLAG (-2,+1), DO_SAXO_ATTER -> DO_SAXO2_COMMAND (-12,1,2), RAY_RESPOND_TO_UP ->
  RAY_RESPOND_TO_DIR (-6,-1,1), INIT_OPTIONS_CHOICE -> INIT_AFFICHE_ECRAN_OPTIONS (-4,-2), setMoskitoAtScrollBorder ->
  prepareNewMoskitoAttack (-8,-5), DO_BBMONT_ATTER -> DO_BBMONT_COMMAND (8,15), DO_BBMONT_TOUCHE -> DO_BBMONT2_COMMAND
  (-20,-4), restore_objects_flags -> snapToSprite (-4,-2,20), bonus_fall -> collision_briques (82..85),
  DO_DARK2_AFFICHE_TEXT -> DO_DARK_PHASE2_COMMAND (-9,-5,1). Prefix/literal references (deter_num_let +60,
  LOAD_CREDITS_VIGNET +48, AFFICHE_BUTTON_JOYSTICK +52, AFFICHE_VIDEOMODE_SCREEN +148) follow cumulative TU drift;
  LoadTchatchPerdu -> PlayTchatch (-5) persists although PlayTchatch is STRICT (OPEN).
- Tool defect found: explain.py/cgtrace.group_events rejects units with case-different names (frame do_fade/DO_FADE).
- Layout pilot (lay_pilot 2026-10-24, 664 compiles, 15 min): carriers IS_MIT_PAF and swapMereDenisCollZones STRICT by
  moving owner labels only (DO_MIT_COMMAND 499->638 stays NEAR; prepareNewMereDenisAttack 718->221); blocs
  IS_ON_RESSORT_BLOC not closed (27 failing entries at -4/+2, not 3 uniform; best 27->9). REFUTED shortcut: a uniform
  census delta does not mean one size divergence (meredenis +1 = three compensating 1-byte encodings). Method for the
  rest: enumerate ALL table entries and cumulative size boundaries, objective = carrier STRICT + 0 losses.
- Semantic triage (sem_a3 2026-10-24): 24/25 untriaged DIFF/CALL_ORDER functions EQUIVALENT (shared call tails, switch
  partitioning - ray_jump case sets checked for all 256 btypes -, masks, literal materialization; restore_gendoor_link
  u8 lossless: all 82 PCMAP levels have links <= 253); MOVE_OBJECT UNRESOLVED (B20). With sem_triage: 42/43 flagged
  functions EQUIVALENT, 0 confirmed behaviour differences in NEAR code.
- Layout round 2 (lay2_a/lay2_b 2026-10-24, ~2,460 compiles, 45 min): +5 STRICT (INIT_OPTIONS_CHOICE,
  AFFICHE_BUTTON_JOYSTICK, DO_COMMANDE_GRAPHIC, AFFICHE_VIDEOMODE_SCREEN, restore_objects_flags; owners stay NEAR).
  Stopped (FINDINGS keep all reference tables): standard_frontZone (6 entries, best still 2 off), DO_SAXO_ATTER (16, 7
  failing), RAY_RESPOND_TO_UP (7 -> 3 failing), setMoskitoAtScrollBorder (25 failing), IS_ON_RESSORT_BLOC (9),
  DO_DARK2_AFFICHE_TEXT (12, 7 failing), DO_BBMONT_ATTER (14, 10 failing at 7 deltas), DO_BBMONT_TOUCHE, bonus_fall.
  Observation (not a validated predictor): closed carriers had a few failing entries whose size boundaries were
  local edits; stopped ones had many failing entries or boundaries spread through the owner (DO_SAXO_ATTER: 3 deltas
  but independent dispatch/store boundaries). Those need the owner itself matched - ordinary NEAR work.

## H13 - Causal clustering of NEAR functions does not yield transferable repairs (rc_cluster, 2026-10-24)
45 normalised diagnostic records (30 current NEAR functions in 19 units + 15 historical fixes recompiled from their
pre-fix sources; split into training / held-out before any trace; 140 compiles; build/workers/rc_cluster/REPORT.md,
records.json). Mechanism profiles: record ownership/conflict order 11, home set/unstable sort 8, scheduler input 7,
rover history 5, layout ownership 5, Score rings 2, unresolved mixed 5.
- NEGATIVE: one mechanism label maps to different repairs. Ownership-order symptoms were repaired by a declaration
  swap (c01), a swap plus a temporary (c03), a count snapshot plus store moves (c09), an abs helper plus a commute
  (c12), or a single address copy (c14); 8 of 15 historical repairs are multi-family bundles. A label alone does not
  predict the edit or its direction.
- Limited positive: source twins in one unit transfer quantitatively (jumelle X/Y: identical name counts, phase
  histograms and rover streams; the same edit worsened both 18->23). Held-out mechanism exclusions held (aff_luciole has
  no sign-TEST Score residue). Layout-owner direction confirmed, but "0 differing bytes" is too narrow a carrier
  criterion: Do_Effet_Chaleur had one internal CALL displacement byte without a fixup.
- Stopped hypotheses: removing synthetic address copies reduces surviving names (X: 38->38, stopped after 2
  compiles); the initialised `T *slot = &v` spelling is the mechanism (DO_STONEWOMAN stays STRICT when split).
- Tool defect fixed: explain.py reported a schedule window "legal" whenever several machine->IR mappings existed even
  if all were illegal (dosmem dos_allocate_conventional_block: {'illegal': 3}); now legal only if one mapping is.
Decision: no broad clustering campaign; reopen only for a function pair whose complete record/eligibility/dependency
constraints match (e.g. source twins).

## Confirmed / refuted causes (short)
- CONFIRMED: switch-table blockers (13 owners) = allocation-driven instruction length, not case order (sw_layout).
- CONFIRMED: census DIFF tags were mostly tool blind spots (t_census), not semantic errors.
- CONFIRMED: first natural-link divergences were structural (asm slicing, data ownership, const table), not NEAR drift.
- CONFIRMED: HMI link blocker = wrong assembler/object format; original library resolves it.
- CONFIRMED (inverse of H2, HMI): our invented aggregates spanned the vendor library's individual data publics; replaced
  by the original publics (hm_names2) - ownership must follow the original definitions in both directions.
- CONFIRMED (H2-type, undersized object): 2:32190 is the 48-byte DPMI 0500h memory-info record (implicit API writes hid its
  size; Android DPMIMemoryInfo 0x30) - our 4-byte pointer binding left a 44-byte gap (dt_cells).
- CONFIRMED (instance of H2, found at RUNTIME 2026-10-23): the four cd_packet_* globals are the ax/bx/cx/es fields of one
  dos_regs_t at 2:4e8a2 - natural-link adjacency broke the MSCDEX request (docs/LINK.md, B18).
- CONFIRMED (instances of H2): game_title = language_txt[231]; menu row 9 = text_to_display[0]; init_data_state/record fields.
- REFUTED: another third-party library beyond Watcom runtime and HMI SOS (lib_audit coverage map).
- REFUTED: pure register renaming class (0 functions); case-order hypothesis; v1.12 source changes as NEAR cause;
  generic source rewrites (staging, operand order, predicate extraction) as cross-unit rules.
