# Cross-platform source seeding (tools/seed.py) - pilot results and scope

Goal: reuse the PS1 matching decompilation and Rayverse C instead of decompiling every DOS function again.
The tool is deliberately small and subordinate to the TU workflow: it writes only to `build/seed/`, never edits
`src/` or `config/`; promotion stays manual through `tools/promote.py` + `tools/verify.py`.

## What it does
1. **Source selection**: the PS1 body (rayman-ps1-decomp `src/**`) and the Rayverse body (skipped if a stub).
2. **Adaptation** with `config/seed_rules.json` - only rewrites mined from accepted STRICT DOS code
   (`python tools/seed.py mine`: 173 STRICT functions with a PS1 body, token-level PS1->DOS edit counts):
   Obj->obj_t (x136), x_pos/y_pos->x/y (x129), field23/24/20 -> param/link/follow_id, init_x/y, PS1 flag word ->
   DOS bitfields (`flags & FLG(OBJ_FLIP_X)` -> `flags.flip_x`, `|=`/`&= ~` -> `= 1/0`) and `FLG(OBJ_ACTIVE)` -> the
   separate `is_active` field, true/false, calc_obj_pos_map->calc_obj_pos, __builtin_memcpy/abs -> Watcom intrinsics.
   PS1 helper macros come from a guarded compat prelude (a unit's own DOS definitions win).
3. **Context**: inside the accepted unit (function swapped in) or a generated minimal TU: shared headers, typed
   externs from symbols.json, DOS-verified signatures from src/ before Rayverse/PS1 prototypes, PS1/Rayverse
   constants and Rayverse typedef blocks; compiler-reported undeclared names are declared from the same tables and
   the compile retried. Names with no evidence are reported, never invented.
4. **Variants**: as-is, plus return-width variants (s16/u8/u16/s32/void) - the proven "widened returns" rule; a
   declaration-only change. The oracle picks; nothing else is searched.
5. **Diagnosis**: the normal strict verifier, plus an instruction-alignment similarity (absolute operands masked)
   because byte-diff counts exaggerate after the first inserted instruction.
6. **draft START END**: seeds every function of an untouched object into one compiling draft unit
   (`build/seed/draft/draft_<start>.c` + bindings overlay + per-function report) - the worker's starting point.

## Measurements (2026-10-20, Watcom 10.6 game profile)
| set | result |
|---|---|
| positive control: 198 already-STRICT functions with a PS1 body, seeded in their unit context | **87 AUTO_STRICT (44%)**: PS1 as-is 56, PS1 + return width 7, Rayverse 24; 4 NEAR_SHAPE (sim >= 0.9), 22 NEAR_CLOSE, 64 NEAR_FAR, 21 COMPILE_FAIL |
| pilot A: 15 random unmatched functions where Rayverse is a stub and PS1 has C (standalone) | 0 STRICT; 13 COMPILE_FAIL (PS1-only globals/typedefs with no known DOS name, e.g. `sko_ecroule_plat`, `D_801E4E10`, `record_t`, `scene_func`), 2 NEAR_FAR |
| pilot B: 15 random accepted NEAR functions (the hard tail workers already attempted) | 0 STRICT; seeds closer than the accepted versions for 2/15 (mereDenisCanAttak, prepareNewMoskitoAttack), worse for 13/15 |
| draft of untouched object blocs (1:14e90, 23 functions) | **8 STRICT** in the assembled, compiling draft |
| draft of untouched object bbmont (1:100b0, 29 functions) | **8 STRICT** |
For comparison, worker first passes on new objects reached ~35-40% STRICT (cage 9/10, saxo 10/25, special 6/30,
collision1 11/37, ...); the draft reaches ~30% in ~2 minutes without a model.

## Conclusions
- **Useful**: as the first step for an untouched object (`python tools/seed.py draft START END`): ~30-45% of functions
  are STRICT with zero manual work, the context (externs, prototypes, types, bindings) is assembled, and the worker
  starts difference-first on the rest. This removes the mechanical part workers repeated per function.
- **Not useful**: for the hard tail (register/frame allocation, layout drift) and for PS1-only context. Do not
  expand the tool there; those stay with difference-first workers.
- Mined facts reconfirmed: PS1 bodies need DOS types/fields/flags; ~4% need only a narrower return type; PS1-only
  globals are the main blocker - binding them needs DOS evidence (address from the DOS function's own fixups), which a
  worker supplies when it reviews the draft.
- Bug found by the positive-control test and fixed before the final numbers: the flag-read rule had eaten the
  parentheses of `if (...)`.

Commands: `python tools/seed.py mine | try NAME | pilot NAME... [--json OUT] | draft START END` (hex obj1 offsets).
Tests: `tests/test_seed.py` (PS1 DO_CAGE seed must be AUTO_STRICT; flag rule shape).
