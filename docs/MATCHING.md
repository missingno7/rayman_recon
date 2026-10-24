# Matching workflow and status definitions

## Loop

```
python tools/disasm.py NAME              # oracle listing with fixup targets + Rayverse names
python tools/xref.py NAME                # Rayverse file:line, PS1 address/file
# write a candidate TU in scratch/ or build/workers/<you>/ (whole original object when possible: tools/tumap.py)
python tools/match.py CAND.c [FUNC] [--symbols overlay.json]   # compiles with profile 'game', diffs, verifies refs
```
Inspect the first difference -> form one hypothesis (type, statement order, declaration, flag) -> change the C ->
re-run. Keep falsified hypotheses in your notes. Promotion (integrator): copy to `src/`, add bindings to
`config/symbols.json`, add the unit to `config/units.json`, run `python tools/verify.py` and the tests.

## What the verifier checks (tools/match.py)
For each public function of the compiled object's `_TEXT`:
1. Placement: the function name must be bound in `config/symbols.json` (or overlay). All bound publics of the
   object must imply one `_TEXT` base (objects are contiguous).
2. Bytes: every byte of the candidate extent (public .. next public or segment end, i.e. including intra-object
   padding) outside fixup fields equals the oracle.
3. Fixup sites: candidate `off32` fixup sites == oracle PMW1 fixup sites in the extent (no missing, no extra).
4. References: `rel32` targets are decoded from the original bytes and must equal the bound address of the
   candidate's target symbol; `off32` targets must equal bound address + displacement (Watcom keeps the addend in the
   field). Library symbols bind through `libsyms.json` (exact library-member matches, `tools/libscan.py --symbols`).
   A reference to the object's own CONST/_DATA counts only if the whole segment's bytes equal the oracle at the
   implied location and all references agree. A reference into the object's own CODE prefix (the private
   initialisers Watcom places before the first public: local arrays, pointer tables) counts only if every prefix
   byte equals the oracle and every prefix relocation is verified (added 2026-10-24 after gap_code found
   select_level_prg STRICT with a wrong `num[6]` initialiser; tests/test_code_prefix.py).

## Status vocabulary
| status | meaning |
|---|---|
| STRICT | 1-4 all hold at the bound address |
| CODE_BYTES_MATCH | bytes and fixup sites equal, at least one reference target unverified (unbound symbol) |
| NEAR_MATCH | differences (reported with the first differing instruction pair) |
| BEHAVIOR_MATCH | behaviour verified by other means, code differs (not produced by match.py) |
| UNVERIFIED | source exists without adequate proof |
| NOT_RECONSTRUCTED | no acceptable source |

Function STRICT does not imply data/BSS layout, object order or whole-executable identity; those are separate
milestones (TU data verification, relink of `_TEXT`, PMW1 re-packing).

## Acceptance policy and roadmap (user decision 2026-10-23; supersedes the authentic-spelling requirement)
STRICT is unchanged (the verifier is the only authority, never weakened). What changed is which C is acceptable: any
valid, semantics-preserving C compiled by the pinned toolchain that reproduces the bytes - including compiler-steering
temporaries and locals, reordered statements/declarations, goto and decompiler-style flow, split/combined expressions,
repeated conditions, duplicated case bodies, unusual scopes and permuter-found transformations. We reconstruct a
program from source, not its original spelling, and never patch compiled output.

Hard boundary: C / evidenced original asm -> Watcom wcc386/WASM -> WLINK -> PMWLITE. Never modify objects, the linked
or packed executable, fixups, relocations, layouts, the oracle or assets; no copied instruction bytes, `_emit`, raw
opcode arrays or code-bearing pragmas outside the existing evidence-gated ORIGINAL_INLINE rules; no undefined
behaviour; public symbols/ABI, data ownership and TU boundaries change only with separate structural evidence.

Source-quality tag (per function in config/units.json, independent of status and provenance):
| quality | meaning |
|---|---|
| (absent) = NATURAL | straightforward reconstruction supported by semantic/structural evidence |
| COMPILER_STEERED | source deliberately adjusted to reproduce allocation, scheduling or selection |
| SYNTHETIC | substantially artificial valid C (permutation/automated search) |
A SYNTHETIC function that passes the verifier is fully STRICT. Provenance tags stay as they are. Readable earlier
implementations are preserved by git history and the tag `semantic-baseline-2026-10-23` (1,519 STRICT, 276,176 code
bytes); do not keep second copies. Revisit synthetic code only when it blocks matching/linking, better evidence appears,
or maintenance/porting needs it.

Priorities: (1) verified STRICT code bytes, (2) completed objects, (3) natural-link convergence, (4) reduced blocker
complexity, (5) cost/throughput. Local steering must not change TU structure, CONST/_DATA/_BSS ownership, public
identities or relocation order; check whole-object effects, not only the function.

Phases: 1 semantic/structural reconstruction (largely done); 2 aggressive source-level matching (PS1-derived seeds,
compiler-guided search, permutation); 3 whole-image convergence (TUs, data/BSS, linker, relocations, packing) -
concurrent with 2; 4 semantic cleanup and native SDL3 port outside src/, from the matching reference.
100% is the target, not a promise: blockers that need unavailable compilers or data stay documented, never bypassed.

## Provenance tags (config/units.json)
DOS_BINARY_DERIVED, RAYVERSE_GUIDED, PS1_SOURCE_GUIDED, CROSS_VERSION_CONFIRMED, HISTORICAL_LIBRARY,
ORIGINAL_ASM, NEW_TOOLING - evidence ancestry, not matching status.

## Own data-segment placement from data relocations (2026-10-22, build/workers/t_match)
An initialised segment that no code fixup or bound public places (e.g. the CONST literal pool of a data-only unit
whose `_DATA` pointer table points into it) is placed from the object's own off32 data relocations whose source segment
is anchored by consistent bound publics: every vote (oracle target - field - displacement) must agree, otherwise the
segment is MISMATCH; inferred segments never anchor further inference. All existing checks (complete bytes, coverage,
fixup sites, targets) still apply. tests/test_data_placement.py has positive and negative controls.
