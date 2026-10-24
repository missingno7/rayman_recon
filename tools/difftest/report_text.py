"""Hand-written method/limitations text of REPORT.md (report.py adds the measured tables)."""

HEAD = """# Differential execution harness v3 - report

Scope: every NEAR game function of `build/near_census.json` (src/hmi excluded), run as an isolated function on the
decoded GOG v1.21 oracle image (DOS side) and on our pinned-Watcom-10.6 object (C side) in Unicorn 2.1.4, from
identical machine states, with outgoing calls replaced by deterministic stubs. Verdicts are EQUIVALENT_TESTED /
DIVERGENT / OUT_OF_DOMAIN_DIFFERENCE / HARNESS_LIMIT; none of them is a matching verdict and nothing here is STRICT.
EQUIVALENT_TESTED means "all judged seeds agree on every observable, with the stated coverage", not a proof.

Files: `difftest.py` (harness), `cdecls.py` (project-wide declaration table), `domains.py` + `domains/*.json`
(per-function input domains, written by `make_domains.py`), `controls.py` (controls), `report.py` + `report_text.py`
(this report), `tests/test_difftest.py` (unit/pipeline tests). Per-function results: `results/UNIT_FUNCTION.json`;
aggregate `RESULTS.json`; first sweep kept as `RESULTS_v1.json`, `results_v1/`, `CONTROLS_v1.json`;
`PROTOTYPES_UNKNOWN.json` lists the callables without a determinable contract. v3 adds `models.py` (environment
models) and `prototypes.json` (evidence-backed contracts of undeclared callables); the previous sweep is kept as
`RESULTS_v2.json`, `REPORT_v2.md`, `CONTROLS_v2.json`.

## What changed from v2 (harness v3), and why

The v2 sweep left 28 functions at HARNESS_LIMIT: loaders whose loops are driven by file contents, main loops that
only end through callee side effects, divide-by-zero / #GP on generated state, runs too slow for the budget, and
missing contracts. v3 adds models of the environment; every model is applied identically to both executions and
only replaces a random stub or generated value by the behaviour of the real environment.

1. **File layer** (`models.FileLayer`, opt-in per function via `environment.file_layer` in its domain file). The
   Watcom C runtime calls fopen / fread / fseek / ftell / fclose are modelled on the REAL installed game data
   (assets/GOG, read at run time, never written or copied): fopen reads the path the game built from emulated memory
   and maps it case-insensitively below the game directory (a drive letter, leading backslash or `..` component - the
   game's path_has_changed variant - and missing files return NULL; a writing mode is refused); each open file has a
   position; fread copies min(size*n, bytes left) real bytes into the destination (identically on both sides,
   tracked as written memory) and returns the number of complete items; fseek implements SEEK_SET/CUR/END with a
   signed long offset (negative result: -1, beyond EOF allowed); ftell returns the position; fclose releases the
   handle (unknown handle: -1). FILE* values are synthetic identities in the k-th-fopen order. Not modelled (no
   function in scope uses them): fgetc/fgets/fscanf, open/read/_dos_read/lseek/filelength/close, any writing call.
   Inputs become "which file / which entry" seeds: the six world files, every level file of each world directory,
   ALLFIX.DAT, BRAY.DAT, the five RAY.LNG blocks, the 75 VIGNET.DAT entries, the background ids of the world files.
2. **Allocators** (`environment.allocators`): block_malloc / temp_malloc return fresh disjoint aligned regions of the
   requested size (bump allocator), so loaded data is not overwritten by unrelated allocations.
3. **No-return callees** (`environment.noreturn`): FatalError ends in exit(-1) (src/basic.c:1651); after its call is
   compared both sides end with status EXIT.
4. **Natively executed STRICT leaf callees** (`environment.execute`): sinus_cosinus, sinus, GetStringTxt are STRICT
   and contain no calls (checked at load); after the call is compared they run from the oracle image on both sides,
   so their results are the game's (real sine table, real text splitting).
5. **Comparison boundary** (`environment.boundary`): for loops that cannot end under stubs (PcMain), whose length is
   irrelevant (DoEffectBloodOut frames) or that busy-wait on a stubbed clock (sub_3ABE4), both executions stop at the
   Nth call of a named callee or the Nth logged event. The boundary is the same event on both sides only when their
   event sequences agree up to it; everything before it (calls, arguments, pointees, system events, memory written
   before each call and at the boundary) is compared. Termination status BOUNDARY; nothing after it is judged.
6. **Hardware models (automatic).** BIOS tick 0:046C: in functions that reference 0x46c, every read access observes
   the counter advanced by one (DOS busy-waits for the next tick end). Selector loads (lgs/lfs/les/lds, mov/pop sreg):
   a flat descriptor table (every GDT/LDT selector = writable 4 GB data segment, base 0; SS reloaded as a 32-bit
   segment) so far pointers address flat memory. `void __interrupt __far` entries: no arguments, IRETD at the
   interrupt frame is the return.
7. **Far pointers** (`__far *` parameters) take two Watcom register slots or two stack words (offset, selector); the
   selector is compared as a 16-bit argument. Before, a far-pointer argument shifted every later argument and the
   callee did not pop its stack word (sub_195ec escaped through a wrong return address).
8. **Undeclared callables** get a contract from `prototypes.json` only with DOS-code evidence (set_speaker_on:
   hand-written assembly 1:06eaa, no register inputs, preserves all registers; __GETDS 1:70408: loads DS only).
9. **Speed only:** library/runtime-helper code runs without a Python block hook, on an unobserved alternate stack
   (top 64 bytes of the caller's stack mirrored and written back), and stack accesses are observed only from the
   function's own frame upwards (in-function ESP is checked against that range). Per-function time, top-tier
   instruction/call budgets and main-phase seed counts can be raised in `environment.budget`.
10. **Domains** additionally support fresh NUL-terminated strings, slices of shipped data files (FLC movie frames of
   assets/DOS/INTRO.DAT and CONCLU.DAT), arguments derived from other arguments, and arrays of structures with
   per-element ranges, index-derived values (grids) and per-element fresh allocations. Domains with an environment
   but no input constraints skip the (identical) unconstrained exploration phase.

## What changed from v1 to v2, and why

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
- PROVEN (v3 controls): with the file layer, an identical copy of a loader is EQUIVALENT on real VIGNET.DAT data;
  reading the wrong offset (LoadPcxOptions header at +1: the integrity check of the real data fails and the mutant
  ends in FatalError), the wrong size (load_big_ray nb_sprites) and a wrong decode of real bytes (load_big_ray XOR
  key) are DIVERGENT.
- STRONG (v3 models): the file layer follows the C library semantics stated above; the domain files cite the setters
  and the parsed game data for every pinned value. HYPOTHESIS: the flat selector model (all selectors base 0) and the
  per-read BIOS tick are admissible environments, not the DOS/4GW selector layout or a 18.2 Hz clock; a boundary
  verdict says nothing about behaviour after the boundary; the movie data comes from the assets/DOS install copy of
  the same game (the movies are not in assets/GOG; INTRO.DAT is listed on the game CD image assets/DOS_CD).
- OPEN: functions whose only remaining blocker is a missing domain (self-referential link cycles, file contents
  driving loop counts, menu/screen state machines) need domain files with evidence before their HARNESS_LIMIT can be
  lifted; struct pointees are neither compared nor written by the stubs; callee side effects on globals are not
  modelled (identical on both sides, so they cannot create or hide a difference in the caller's own writes, but paths
  that depend on them are only reached through the generated inputs).
"""
