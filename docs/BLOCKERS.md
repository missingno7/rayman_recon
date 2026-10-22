# Blocker inventory (living document; choose work from here)

Baseline 2026-10-22: 1,385 STRICT functions (262,197 code bytes, 57.3% of game code), 301 NEAR, every game code range
has a unit. Classification sources: `python tools/census.py` (tags), build/workers/ra_tax/taxonomy.md (mechanical
difference classes), docs/COMPILER.md (rules and negatives). Status: ACTIONABLE / IN PROGRESS / BLOCKED / EXHAUSTED.

| # | Blocker | Functions (bytes) | Root cause / hypothesis | Dependencies | Evidence / tried | Next / value | Status |
|---|---|---|---|---|---|---|---|
| B1 | Later switch body moves table targets | 13 owners close 26 functions (~16 KB) | Case-block layout of large command functions (basic snapToSprite, moskito prepareNewMoskitoAttack, fee DO_FEE_ETAPE, saxo DO_SAXO2_COMMAND, meredenis prepareNewMereDenisAttack, collision1 SET_DETECT_ZONE_FLAG, bbmont DO_BBMONT_COMMAND/2, blocs CALC_MOV_ON_BLOC, breakout collision_briques/init_briques, ray RAY_RESPOND_TO_DIR, hybrid DO_DARK_PHASE2_COMMAND) | each owner blocks one preceding function | ra_eclass (PROVEN mapping); per-unit passes failed on the bodies | shared case-layout rule (sw_layout) - high (x2 payoff) | IN PROGRESS |
| B2 | `_TEXT` initialiser prefix shifted by earlier drift | 4 (deter_num_let, LOAD_CREDITS_VIGNET, AFFICHE_BUTTON_JOYSTICK, AFFICHE_VIDEOMODE_SCREEN) | first size-drifting function of the object | earlier NEAR in basic/cheat/menu | ra_eclass | closes with B4 work in those units | BLOCKED |
| B3 | Semantic/binding differences (census DIFF_calls/globals/fields/consts) | ~98 functions | wrong binding/field/constant/call or prototype; DIFF is a pointer (~60%) | shared headers (obj.h), bindings | census; per-unit passes did not audit systematically | sem_a/b/c audits - medium-high | IN PROGRESS |
| B4 | Shape-only NEAR (census SHAPE) | ~184 functions | register/stack allocation entangled with instruction selection (taxonomy: 0 pure renaming) | none | ra_probe/ra_pairs/r_regalloc/r_slots negatives; TEST-after-copy predictor; loop weight ~20; tie order not declaration order | only with a new compiler fact | EXHAUSTED (for generic rewrites) |
| B5 | Open TEST-register residues | 5 (DO_STONEWOMAN_COMMAND, IS_MIT_PAF, Display_and_free_luciole x2, RAY_BALANCE_ANIM) | DOS tests EAX against the copy predictor | - | ra_probe | none | BLOCKED |
| B6 | Absolute-value context | 3 (dark DARK_phase1, DO_DARK_SORT_COMMAND, hybrid doMOSAMScommand) | allocation around abs; helper rejected (user 2026-10-22) | - | r_abs | revisit only with independent evidence | BLOCKED |
| B7 | Guarded copy / sar32 helper candidates | movieprefix, effect6c3b0 sites | one STRICT proof each | second proof | w9_copy, w7_shift32 | needs a second unit | BLOCKED |
| B8 | Link/pack stage | whole EXE | PMWLITE 1.22 deterministic, stub/header reproduced; level and multi-block OPEN | decoded oracle objects | lk_pmw122 | oracle repack, then linker plan (lk_repack) - high | IN PROGRESS |
| B9 | BSS remainder | ~3.5 KB (2:3bd08..3c4a0, 2:32b48..32ee4 language table, small holes) | name/identity conflicts | - | bs_3218a, bs_central, bs_rest | low | BLOCKED |
| B10 | `_DATA` 2:01970 400-byte zero run | 400 B | no reference identifies the object | lowlevel_data.asm covers bytes | dt_196a | low | BLOCKED |
