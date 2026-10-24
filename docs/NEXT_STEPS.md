# Next steps (current priorities; updated 2026-10-24)

Goal: a faithful, source-complete DOS Rayman that the historical toolchain builds, as the base for a native SDL3 port.
Acceptance (docs/MATCHING.md "Acceptance policy and roadmap"): any valid, semantics-preserving C that the unchanged
verifier reports STRICT; mark compiler-steered or synthetic source with `quality`. Never patch compiled output.

## Priorities (by recovered bytes, functions unlocked and measured effort)
Targeting by tools/semir.py (2026-10-24, all 235 NEAR functions, build/semir_near.json): 78 EXACT_LOCAL_EQUIVALENT
(6,026 differing bytes, ~77 per function: only allocation/scheduling left -> explain.py + the fast permuter), 152
MISMATCH (62,569 bytes: first fix the computation semir names - local widths, test order, operand order, load/store
order, stores - before any allocation work; guided editing beat the permuter 4/8 vs 2/8 to exact equivalence and took
RecaleRayPosInJumelle 581 -> 24), 5 UNKNOWN/similar. Clue kinds in MISMATCH: effect 66, branch 32, effect order 24,
branch order 17, terminator 12. Causal clustering (H13) is not a targeting method; source twins are the exception.
Wave 4 (2026-10-24, w4_a/b/c, semir-targeted, disjoint unit groups, ~4,400 compiles, ~30 min per worker): +2 STRICT
(do_sko_rayon2, SET_RAY_DIST_MULTISPR_CANTCHANGE), ~410 fewer differing bytes, 0 losses. Yield stays low (~1 closure
per 2,000 compiles); the semantic route mostly improves fidelity and bytes, closures still come from allocation knobs.
Potential contribution to D:/Prog/ai_decomp (not exported): (1) semantic-IR first difference as the step between
"is it equivalent" and "why did the compiler differ" - lift both sides to width-exact block expressions with symbolic
relocations, align CFGs by predicates, report the earliest differing effect; validate with STRICT controls and semantic
mutants (0 false equivalences); (2) per-candidate disk/CPU hygiene for permuters (prune artifacts, persistent workers).
Latest wave (wave 3, 2026-10-24): 8 parallel Sol workers on the 181 NEAR functions whose computational core is within
4 ops of DOS (scratch/core_ops.py): +25 STRICT (1,547 -> 1,572), several banked reductions, 0 losses; worker
FINDINGS.md under build/workers/w3_*/ keep the negatives. Yield per wave is falling (~14% of targets closed); the
remaining NEAR set is ~260 functions. Idiom census (build/workers/idiom_mine) is exhausted: fill_cld was the only new
helper (docs/COMPILER.md).
1. **Behaviour first: DIFF-class functions.** tools/census.py tags ~30 NEAR functions DIFF_* (a call, global or
   constant differs from DOS). Real behaviour errors matter for the port and are fixed before any matching work;
   equivalent lowerings (memset vs inline REP STOSD fill, mask spellings) become ordinary matching work.
   Result 2026-10-23 (sem_triage, DOS disassembly at every flagged site): 18/18 non-pilot DIFF functions are
   EQUIVALENT lowerings, 0 behaviour errors. Result 2026-10-24 (sem_a3): the remaining 25 (pilot units and CALL_ORDER) are 24 EQUIVALENT,
   1 UNRESOLVED (MOVE_OBJECT, B20) - no confirmed behaviour difference in NEAR code.
2. **Large coherent SHAPE functions.** ~150 NEAR functions (~125 KB) already have DOS calls/globals/fields/constants
   and differ mainly in Watcom code shape (strong evidence, not a certified equivalence). An agent restructures one coherent region (a unit or a jump-table owner and
   its neighbour) until it is close or STRICT; a bounded permuter run (tools/permute.py) finishes the last bytes.
   Jump-table owners (docs/BLOCKERS.md B1) come first because each also unlocks its preceding function.
3. **Small residues with the permuter** (aggressive mode only, ~3 minutes/400 compiles per function - solvable
   functions fall within ~90 s; docs/COMPILER.md "Source permutation") for <= ~20 differing bytes (~80 functions, ~30 KB);
   minimise every hit by ablation (keep only the necessary mutations) before integration. Use explain.py on hits and
   large score drops only.
4. **Whole objects.** Units with one or two NEAR functions left (dosmem, action, blood, bat, map, neige, map_ngawe,
   objupdate4, objinit, ...) close their object and stop layout drift for REFS_LAYOUT neighbours.
5. **Ordinary source build.** The natural link now resolves every symbol (docs/LINK.md); keep it linking, keep data
   correct and never add placeholder data. Runtime: the rebuilt game reaches the first level from main (docs/LINK.md); re-check after large integrations.

## Deliberately postponed (do not start without a concrete reason)
- Byte-identical whole-image work: original linker placement, relocation ordering, packed-EXE identity (PMWLITE /C4
  reproduction is already PROVEN on oracle data). Pursue only where it unblocks reconstruction.
- Matching the HMI SOS vendor source (the original library is pinned and linked).
- Isolated compiler anomalies: box CalcTab's exact double constant (B17), LOAD_CONTINUE_SCREEN's service MOV (user
  decision: stays NEAR).
- New compiler-internals research. The allocator, Score pass, byte rover, compare selection and scheduler are
  recovered (docs/COMPILER.md). Reopen only with a concrete unexplained observation, a falsifiable hypothesis and a
  payoff across several functions.
- Historical identity questions without reconstruction value: language_txt's exact capacity (defined at the proven
  237 entries), central BSS declaration groups, TU boundaries that end exactly on 16 bytes.

## Working loop
`tools/explain.py UNIT FUNC` (mechanisms) -> edit a unit copy in build/workers/<you>/ -> `tools/match.py` (full unit,
0 losses) -> optional `tools/permute.py ... --aggressive` for the last bytes -> integrate with tools/integrate.py or the
integrator's gate (`tools/verify.py`, tests) -> record `quality`.
