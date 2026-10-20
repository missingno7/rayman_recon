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
  bytes. Hand-written assembly is reconstructed as assembly only where the original was assembly (evidence first).
  Allowed: `#pragma aux NAME parm [...] value [...] modify [...]` that only declares the register ABI of an external
  hand-written assembly routine (e.g. DisplayAnyPictureNormal, proven from its DOS callers) - no code in pragmas.
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
`tools/disasm.py NAME|1:off [LEN]`, `tools/match.py SRC.c [FUNC]`, `tools/verify.py`.

Worker final answer: first line `STATUS: done | partial | blocked | failed`, then <=15 lines: unit/range, hypotheses
tested, result per function (STRICT/CODE_BYTES_MATCH/NEAR_MATCH + first diff), candidate path, overlay bindings,
verifier command + output, new compiler facts or blockers.
