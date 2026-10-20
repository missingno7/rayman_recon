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
   implied location and all references agree.

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

## Provenance tags (config/units.json)
DOS_BINARY_DERIVED, RAYVERSE_GUIDED, PS1_SOURCE_GUIDED, CROSS_VERSION_CONFIRMED, HISTORICAL_LIBRARY,
ORIGINAL_ASM, NEW_TOOLING - evidence ancestry, not matching status.
