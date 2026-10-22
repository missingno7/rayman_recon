# Working rules (humans, Claude, Codex workers)

Goal: readable C that the historical Watcom toolchain compiles into the exact machine code of the DOS
`RAYMAN.EXE` (primary oracle: GOG **v1.21**, `config/oracles.json`). Matching first; SDL3/modernisation later and
never inside `src/`. Read `README.md`, then `docs/STATUS.md` and `docs/MATCHING.md`; compiler facts are in
`docs/COMPILER.md`.

Hard rules
- `assets/` (originals) and `external/` (reference repos) are read-only. Never modify, clean, commit or copy
  their contents (or anything derived: decoded objects, disassembly dumps) into tracked paths. Generated data goes
  to ignored `build/`, experiments to ignored `scratch/` or `build/workers/<name>/`.
- Historical tools live under `C:\tools` (pinned in `config/toolchain.json`) and are immutable. Compile only through
  `tools/wcc.py` / `tools/match.py` / `tools/verify.py` (hash-checked, isolated env, TMP in `build/tmp`). Never put
  them on the global PATH. Watcom 10.x compilers run natively (Win32 loader stub + bound payload). For DOS
  programs use MS-DOS Player (`C:\tools\nmlgcdos\msdos.exe`) first; fall back to DOSBox-X (`C:\tools\dosbox-x`)
  only when MS-DOS Player cannot host the program (e.g. DOS/4GW/PMODE-extended images). Never run an unknown DOS
  program directly on the host.
- The verifier decides. STRICT = all bytes equal at the bound address, identical fixup-site sets, every reference
  target verified. Masked / approximate equality is never STRICT. Never weaken `tools/match.py` to fit a candidate;
  if a check looks wrong, prove it with independent evidence and report it.
- No inline asm, `#pragma aux` byte tricks, `_emit`, copied bytes, artificial dead code or volatile hacks to force
  bytes. Hand-written assembly is reconstructed as assembly only where the original was assembly (evidence first):
  `.asm` units (profile `asm`, pinned WASM through tools/wcc.py `build`, checked by the same match.py/verify.py).
  Readable instructions, symbolic labels and typed DB/DW/DD data only - never copied code bytes, INCBIN or padding
  invented to hide a mismatch; every CODE segment starts with a public (whole contribution compared).
  Allowed: `#pragma aux NAME parm [...] value [...] modify [...]` that only declares the register ABI of an external
  hand-written assembly routine (e.g. DisplayAnyPictureNormal, proven from its DOS callers) - no code in pragmas.
  Allowed (user decision 2026-10-21), evidence-gated: a Watcom `#pragma aux NAME = ...` inline-code sequence ONLY
  for instructions plain C cannot generate (debug/control registers DR0-DR7/CRn, INT n/INT3, STI/CLI, IN/OUT and
  port string ops, LGDT-class system instructions), when the DOS bytes show them inline inside a C function. Each
  pragma: the exact instruction sequence from the DOS disassembly (cite address), its register effects declared,
  one pragma per sequence, named after its purpose, and a comment marking it ORIGINAL_INLINE. The same gate covers
  inline code whose BEHAVIOUR C cannot express even though each instruction could be (user decision 2026-10-21):
  self-modifying code and stack-pointer tricks inside a C function (e.g. menu INIT_GENERAL_CHOICE 1:39eb5..39edf),
  with the mechanism PROVEN from the DOS bytes (writers, targets) at the cited site. Also covered (user decision
  2026-10-23): a whole copy-protection / interrupt sequence whose DOS bytes contain instructions Watcom 10.6 C provably
  cannot select (e.g. `CMP reg,0`, docs/COMPILER.md "Compare with zero") next to its DR/INT instructions may be one
  ORIGINAL_INLINE pragma spanning exactly that sequence (from its first DR/INT or C-unselectable instruction through
  the end of its compare/branch logic), each site cited by address, nothing outside the evidenced sequence. Ordinary
  C-expressible setup before that point (e.g. a service-number MOV) stays C. Otherwise never use
  an inline pragma for anything C can express (arithmetic, loads/stores, calls, branches) or to patch register
  allocation - with ONE exception (user decision 2026-10-21, ai_decomp review): original inline helpers. A
  C-expressible sequence may be an ORIGINAL_INLINE helper only when its exact DOS bytes recur with fixed registers
  in at least 2 functions (none of them STRICT through plain C), every site is cited by DOS address, the
  register/clobber contract (`parm/value/modify exact`) is read off at least 2 sites, and the helper is declared once
  in the shared header src/include/inline_helpers.h (never redefined per unit, no new helper pragmas in units).
  Current helpers: ashr16, ashl16, ashl32, inrange, sgn, min32, max32, fill, copy, palette_output,
  write_movie_palette (evidence: docs/COMPILER.md "Original inline helpers").
- Bind every global at its true start with its true type (one address = one symbol). wcc386 -5 loads a `short`
  as `mov r,[x-2]; sar r,16` (fixup field -2) by itself; never declare a wider type or a shifted alias to get bytes.
- Zero regression: `python tools/verify.py` must stay at 0 regressions; `python -m unittest discover -s tests` must
  pass. Only the integrator edits `src/`, `config/units.json` and `config/symbols.json`.
- Workers: write only inside `build/workers/<name>/`; propose bindings in an overlay file used with
  `match.py --symbols`; no git commits; no background processes; bounded runs, one compile at a time.
- Keep facts, hypotheses and open questions apart (PROVEN / STRONG / HYPOTHESIS / OPEN) and keep negative results.
- Names: Rayverse/Android-derived names (RAYVERSE), PS1 decomp names (PS1), or INVENTED (say so). Check
  `python tools/android.py NAME` (original Android symbol: exists? data size/section = true type) before inventing;
  `python tools/android.py --dis NAME` shows the Android x86 code of a function (semantics only, never bytes). Rayverse and
  the PS1 decomp are semantic references only; the DOS binary wins every disagreement.

Commands (all run from the repo root with `python`, stdlib + capstone):
`tools/inventory.py --compare`, `tools/extract.py --all`, `tools/libscan.py`, `tools/xref.py [Q]`,
`tools/tumap.py --list`, `tools/seed.py draft START END` (seeded starting point for a new object),
`tools/census.py [--brief UNIT]` (structural NEAR diagnosis; docs/CENSUS.md),
`tools/explain.py UNIT FUNC` (traces the pinned 10.6 compiler and explains each NEAR difference by allocator
mechanism: rover offset, home sort, colour tie, Score substitution, selection - start NEAR work here),
`tools/cgtrace.py SRC.c --func NAME --out build/cgtrace/<new dir>` (raw allocator trace),
`tools/disasm.py NAME|1:off [LEN]`, `tools/match.py SRC.c [FUNC]`, `tools/verify.py`,
`tools/integrate.py WORKER_DIR UNIT=FILE.c ... -m MSG` (integrator: copy, merge, register, gate, commit only if green).

Worker final answer: first line `STATUS: done | partial | blocked | failed`, then <=15 lines: unit/range, hypotheses
tested, result per function (STRICT/CODE_BYTES_MATCH/NEAR_MATCH + first diff), candidate path, overlay bindings,
verifier command + output, new compiler facts or blockers.
