"""Hand-written method/limitations text of REPORT.md (report.py adds the measured tables)."""

HEAD = """# Differential execution harness v2 - report

Scope: every NEAR game function of `build/near_census.json` (src/hmi excluded), run as an isolated function on the
decoded GOG v1.21 oracle image (DOS side) and on our pinned-Watcom-10.6 object (C side) in Unicorn 2.1.4, from
identical machine states, with outgoing calls replaced by deterministic stubs. Verdicts are EQUIVALENT_TESTED /
DIVERGENT / OUT_OF_DOMAIN_DIFFERENCE / HARNESS_LIMIT; none of them is a matching verdict and nothing here is STRICT.
EQUIVALENT_TESTED means "all judged seeds agree on every observable, with the stated coverage", not a proof.

Files: `difftest.py` (harness), `cdecls.py` (project-wide declaration table), `domains.py` + `domains/*.json`
(per-function input domains, written by `make_domains.py`), `controls.py` (controls), `report.py` + `report_text.py`
(this report), `tests/test_difftest.py` (unit/pipeline tests). Per-function results: `results/UNIT_FUNCTION.json`;
aggregate `RESULTS.json`; first sweep kept as `RESULTS_v1.json`, `results_v1/`, `CONTROLS_v1.json`;
`PROTOTYPES_UNKNOWN.json` lists the callables without a determinable contract.

## What changed from v1, and why

1. **Code/data aliasing** (difftriage: GET_SPRITE_POS, DISPLAY_CYMBALE). Every page - oracle image, candidate code,
   stack, generated memory - is materialised lazily from one function of (seed, address), so both executions see
   byte-identical memory everywhere, including BOTH code mappings. Candidate code moved to CAND=0x58000000, outside
   every generated pointer region, and is mapped in both executions. A data write into the *executing* function's own
   code is stopped: on one side only it is a generated pointer that aliased code (seed not judged); on both sides it
   is reported as self-modifying code.
2. **Callee contracts** (`cdecls.py`). One table from src/*.c, src/*.h, src/include/*.h, src/data, src/bss and the
   pinned Watcom headers they include: parameter widths, signedness, pointee types and constness, return width,
   `#pragma aux` parm/value/modify (also on typedef'd function types), `__cdecl`/variadic (all arguments on the
   stack, caller pops), typedef'd function declarations (`extern block_copy_t block_copy;`), function-pointer
   variables, tables and struct members (`options_jeu.test_fire1`). The callee's definition wins; otherwise the
   declarations must agree. Arguments are compared at the declared width. Stub return values have the declared
   width; on even seeds they are extended by the declared signedness, on odd seeds the upper bits are callee-defined
   (identical on both sides), so a caller that reads bits its contract does not define is exposed. Declared
   function-pointer globals get a synthetic callee identity carrying the declared contract (NULL on 1/8 of the
   seeds). printf-family calls are compared argument by argument from the format string. The pure Watcom -fpc
   runtime helpers (_FDA/_FDS/_FDM/_FDD/_FDC/_FDN, _I4FD/_U4FD/_FDI4/_FDU4, IF@DSIN/IF@DCOS/IF@DSQRT/IF@DEXP/
   IF@DATAN2, the Pentium FDIV check) are executed from the oracle image on both sides instead of being stubbed.
   memcpy/memmove/memset/strcpy/strncpy/strcat/fread/read/sprintf stubs apply a bounded memory effect.
3. **Pointer arguments by object identity.** A pointer into the caller's frame is compared as `{frame_local: k}`
   (k = order of first escape), into the entry frame as `{entry_frame: off}`, candidate public code as its oracle
   address; globals, argument-derived and generated objects by address (identical on both sides). Non-const scalar
   pointees are written by the stub with deterministic values of the pointee's declared type (seed, call index,
   callee, argument index), identically on both sides, so output parameters (GET_SPRITE_POS(obj,spr,&x,&y,&w,&h),
   sinus_cosinus, get_brik_info, ...) flow into later observables. Scalar pointees are compared at call time when
   both sides defined them (const char* as strings); a pointee defined on one side only is recorded as a note.
4. **New observable: memory at each call.** Every non-frame byte written before call k must hold the same value on
   both sides when call k happens (a callee may read any global; the compiler cannot move a global store across an
   external call). Writes to the function's own stack parameters are no longer observables (callee-owned, dead after
   return).
5. **Budgets.** Instructions run natively (block hooks instead of a Python per-instruction hook). A seed that is still
   running escalates 20k -> 300k -> 2M instructions (400 / 3k / 12k stub calls); only seeds where BOTH sides still
   exceed the top budget are unjudged; one side returning while the other still runs at the top budget is a blocking
   HARNESS_LIMIT. Globals/fields compared against registers or memory (loop bounds) get small values on 3/4 of the
   seeds; compare literals still guide one global per seed.
6. **CPU exceptions** are terminations (vector recorded): both sides faulting with the same vector leaves the seed
   unjudged; a different termination is a DIVERGENT observable.
7. **Domains.** Optional `domains/UNIT_FUNCTION.json` pins globals, fields or arguments (values, ranges, any_of
   alternatives, fresh disjoint allocations), each with evidence. The verdict is measured inside the domain;
   additional unconstrained seeds are run and a difference whose inputs violate the domain is reported as
   OUT_OF_DOMAIN_DIFFERENCE with its counterexample (never hidden); an unconstrained difference whose inputs satisfy
   the domain is DIVERGENT. The 11 out-of-domain cases of difftriage have domain files.
8. **Verdict rule.** A seed is judged when both sides return and every observable was comparable. EQUIVALENT_TESTED
   needs >= 16 judged equal seeds and >= 25% of the seeds judged, no blocking limit (asymmetric termination,
   self-modifying code, unsupported register contract, incomplete static CFG) and no difference. Coverage counts only
   blocks visited by judged seeds (v1 counted every seed, including limited ones).

"""

TAIL = """
## Evidence levels and limits of this measurement

- PROVEN (by construction / controls): both executions see identical memory and registers; the 6 STRICT positive
  controls are EQUIVALENT with zero unjudged seeds; every mutated control, including the 6 new ones exercising the v2
  observables, is DIVERGENT with the expected first observable; a layout-only change of escaped locals stays
  EQUIVALENT; B19 EQUIVALENT; B20 DIVERGENT through the original's uninitialised stack read.
- STRONG: a DIVERGENT verdict reports a first differing observable of the two binaries under the same stub model; it
  is not by itself a game-behaviour claim (the stubs model one admissible callee, the inputs may be outside the game's
  domain unless a domain file says otherwise). These cases need triage (difftriage method), not an automatic fix.
- HYPOTHESIS: the callee contracts read from our C declarations are the contracts of the original callees; an odd-seed
  divergence that disappears on even seeds points at a return-width disagreement between DOS and our declaration.
- OPEN: functions whose only remaining blocker is a missing domain (self-referential link cycles, file contents
  driving loop counts, menu/screen state machines) need domain files with evidence before their HARNESS_LIMIT can be
  lifted; struct pointees are neither compared nor written by the stubs; callee side effects on globals are not
  modelled (identical on both sides, so they cannot create or hide a difference in the caller's own writes, but paths
  that depend on them are only reached through the generated inputs).
"""
