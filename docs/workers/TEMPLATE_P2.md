# Task: reconstruct original DOS object(s) of Rayman v1.21: @RANGES@

Repository D:\Prog\rayman_recon (AGENTS.md is loaded; obey it). Write ONLY inside
D:\Prog\rayman_recon\build\workers\@NAME@\. Never edit src/, config/, tools/, tests/, docs/; never touch assets/ or
external/ (read-only). No git commands. One compile at a time; no background processes.

## Context (established; do not re-research)
- Oracle: GOG v1.21, `build/oracles/gog_v121_us/` (`python tools/verify.py` regenerates it if missing).
- Profile 'game' = Watcom 10.6 `wcc386 -zq -oaxt -s -5r -fpc` (register calling convention; -fpc = software
  floating point, proven by jumelle InitMatriceJumelle). `-oe` is excluded
  (it inlines calls the original keeps). `-on`, `-oi`, `-ol` are undetermined: if a function's code depends on one
  of them, test it and report the exact experiment (this is valuable).
- Read first: docs/MATCHING.md, docs/COMPILER.md (proven Watcom idioms), and the STRICT examples src/rand.c,
  src/mathutil.c, src/bat.c, src/bonus.c.
- Shared headers: `#include "obj.h"` / `#include "game.h"` / `#include "rtypes.h"` (src/include, already on the
  include path). obj_t/eta_t/anim_t/sprite_t/zdc_t come from obj.h - never redefine them locally. If you need a
  new shared type or believe a field in obj.h is wrong, put the proposal in `build/workers/@NAME@/header_proposal.md`
  with the DOS instruction(s) that prove it; you may define genuinely new structs in a local header in your dir.
- Object ranges come from `python tools/tumap.py --list` (zero padding before 16-byte boundaries, decode-checked).
  They are candidates: one range may contain several original objects (a boundary that ends exactly on 16 bytes is
  invisible). If evidence shows a different split (e.g. the matched functions imply a different _TEXT base), say so.
- References: Rayverse (external/rayverse/src/@RVFILE@, types.h) is derived from this exact PC version - start from
  it. The PS1 decomp (`python tools/xref.py NAME` gives its file) has matching C for many functions but is a
  different platform; some functions genuinely differ. The DOS binary decides.

- Android x86 code of a function: `python tools/android.py --dis NAME` (semantic reference, e.g. for Rayverse stubs;
  different compiler and platform layer - never a byte reference).
- Android symbols (`python tools/android.py NAME`): original names, true data sizes (.data/.bss). Every data
  binding's type must match the Android size; unnamed functions/globals should get their Android name if it can be
  identified. A referenced address inside the object's own CONST string run is a string literal (write the literal),
  not a global (see docs/DATA.md).

- Compiler knowledge: docs/COMPILER.md "Matching rules" is the single list of proven fixes (rule IDs W-*),
  with the triage order and the "Do not try" list. The census (step 3) prints the rule IDs to try for each NEAR
  function and any callee whose prototype differs from other units (W-PROTO). Follow the triage order: W-LAYOUT
  (neighbour/size/missing function), W-PROTO (callee declaration), W-BIND (binding/short load), W-PORT (port
  additions) before reshaping a body. No compiler-option or compiler-release probes (settled; see "Do not try").
  Tools may refresh caches under build/ (xref, android): that is expected, not a scope violation.

## Workflow (difference-first)
First write a C body for EVERY function of the range (in original order, compiling) before refining any of
them: an incomplete object cannot verify its layout, and unfinished passes cost a second run.

0. Start from the seeded draft: `python tools/seed.py draft <start> <end>` (hex obj1 offsets of your range) writes
   build/seed/draft/draft_<start>.c (+ .symbols.json, .json report): PS1/Rayverse bodies adapted to DOS types with
   context assembled; typically ~30-45% of functions are already STRICT (docs/SEEDING.md). Copy it into your dir,
   keep its STRICT functions, and work difference-first on the rest (bodies it could not seed are left as comments).
1. `python tools/disasm.py NAME` (or `1:XXXXX LEN`) per function; note globals, struct offsets, callees.
2. Write `build/workers/@NAME@/unit.c` containing the whole object in original order, and
   `build/workers/@NAME@/symbols.json` (layout of config/symbols.json, key "gog_v121_us"; data entries need
   "type" = the true C type; names RAYVERSE/PS1/INVENTED + evidence). Rayverse data.c comments `//CFxxx` give
   data addresses (obj2 offset = value - 0x90000). Bind every function of the object, every called function and
   every global you reference. Check config/symbols.json first: reuse existing names/addresses, never conflict.
3. `python tools/match.py build/workers/@NAME@/unit.c --symbols build/workers/@NAME@/symbols.json`, then
   `python tools/census.py --file build/workers/@NAME@/unit.c --symbols build/workers/@NAME@/symbols.json` ->
   unit.census.md: per NEAR function a structural verdict vs DOS (docs/CENSUS.md). [SHAPE] = calls/globals/fields/
   constants already agree (logic right, only Watcom shape left); [DIFF_*] = concrete pointers (missing/extra calls,
   globals, field offsets, constants; ~60% real, confirm in the disassembly); [REFS_LAYOUT] = bytes equal, follows
   layout. Work DIFF_calls -> DIFF_globals/fields -> SHAPE; re-run the census after big changes.
   For each mismatch: read the first difference, state a hypothesis + predicted code change, make the smallest
   source change, re-run. Keep NOTES.md with hypotheses, results, and failures (negative evidence matters).
4. Forbidden (will be rejected): inline asm, #pragma aux code (ABI-only `#pragma aux NAME parm [...]` for external asm routines is allowed), volatile hacks, dead code, copied bytes, shifted or
   widened/narrowed global declarations, aliases at offset addresses, types chosen only to force bytes. Every
   global is declared with its true type at its true start. The `-5` short load `mov r,[x-2]; sar r,16` comes from a
   plain `short` declaration.
5. Transferability record (in NOTES.md, a table) for every function you touch that has a Rayverse body and/or a
   PS1 C body: | function | Rayverse verbatim result | PS1 verbatim result (if any) | changes needed for STRICT |
   cause (source revision / struct layout / compiler / platform logic / Rayverse port adaptation) | category
   (CONFIRMED_SAME_LOGIC / LIKELY_SHARED / CONFIRMED_VARIANT / PC_ONLY). "verbatim" = only type/name adaptation.
   Try the verbatim compile first; it is cheap and the result is evidence.

Done = every function in the range STRICT, or productive hypotheses exhausted for the rest (do not grind on one
stubborn compiler decision; move on and come back if new evidence appears).

## Final answer
First line `STATUS: done | partial | blocked | failed`, then <=15 lines: per-function status with remaining first
differences, whole-object layout (contiguous or conflict), compiler findings, header proposals, transferability
summary (counts per category), files, and the final `python tools/match.py ...` summary line.


Inline machine code (AGENTS.md, evidence-gated): instructions plain C cannot generate (debug registers, INT/INT3,
STI/CLI, IN/OUT and port string ops) found inline in a DOS C function may be written as a Watcom
`#pragma aux NAME = ...` sequence copied from the disassembly (cite address, mark ORIGINAL_INLINE). Nothing else.
