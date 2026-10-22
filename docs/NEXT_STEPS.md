# Next steps (ordered by measured value, 2026-10-21)

1. **Low-level assembly: done** (2026-10-21): 1:00010-06f7e in 8 STRICT `.asm` slices + VERIFIED data prefix. Open:
   one-module consolidation (only with evidence for the original module structure), assembler identity.
2. **Layout drift**: three drift rounds gave +44, +6, +0 (plus menu's trampoline +2); the units whose drift did not move
   are blocked by register/stack allocation or by OPEN shift helpers (bare shl / sar32 through CL: one proof each,
   docs/COMPILER.md). Re-run a unit only when its first drift has a new lead. Consolidation: write_movie_palette (same
   ORIGINAL_INLINE sequence at 1:2ae90 and 1:2b2ed) belongs in inline_helpers.h.
3. **Finishing passes only with new evidence.** Most NEAR pools have had >=2 sol passes; wave-4 passes on exhausted
   units returned in 3-9 minutes with no gain (effect1/2, cymbal/save/display, affiche, pcfee). Re-open a unit when a
   new compiler fact appears (docs/COMPILER.md evidence log) or a layout fix lands before its NEAR functions.
   Largest pools: moteur 32, menu 28, ray 25, frame 22, breakout 17, affiche 16, basic 13, collision1 13.
4. **Layout closure**: each census brief names the first size-drifting function per object (W-LAYOUT); fixing it is
   the cheapest way to flip REFS_LAYOUT functions. fnd: matching INIT_FND also fixes LOAD_FND's jump table.
5. **copy_guarded (OPEN)**: the guarded 19-byte copy variant (JE before REP MOVSD) has one STRICT proof (w9_copy) and
   is not in inline_helpers.h; movieprefix (1:2a2d8 area) and effect6c3b0 (two sites) call it undeclared, so those
   functions stay NEAR. A second-unit proof admits it to the header (same gate); until then, do not declare it per unit.
6. **OPEN compiler questions** with the widest payoff: the F3 `rep movs`/`rep stos` copy sequence (jumelle, effect1/2,
   six effect objects; not a 10.5/10.6 difference), clamp lowering in the effect objects, materialised booleans.
6. **Data**: ObjectsFonctions/t_world_info pointer tables; central BSS declaration order; `_DATA` object boundaries.
7. Link stage (WLINK object order = tumap order) - later. Not started. (v1.12 cross-version classification: done,
   docs/STATUS.md; a v1.12 build profile would need the rand/FatalError/resource-ID deltas as a version switch.)
   First step only (before any WLINK work): the oracle stub is PMODE/W **1.22** and C:\tools has PMWLITE 1.33 only -
   acquire PMODE/W 1.22 PMWLITE.EXE, then test on a trivial WLINK-built LE whether it is deterministic and whether
   our decoder round-trips its output. Only then ask whether decoded oracle objects re-compress to the original
   stream (packer reproducibility before link work, as in earlier projects).

## User decisions (2026-10-22)
- **Abs helper: NOT accepted** (gate kept strict). Evidence kept in build/workers/r_abs (NOTES.md, proposed.h): an
  EAX-only `test/jge/neg` helper would close dark DARK_phase1, DO_DARK_SORT_COMMAND and hybrid doMOSAMScommand, but plain
  C produces the same core in 26 STRICT functions, so a helper would only steer allocation. Revisit only with new
  independent evidence of an original inline helper or macro.
- **Link stage: first exploratory step approved only** - find/authenticate PMODE/W 1.22 (PMWLITE.EXE), test
  deterministic compression on a minimal Watcom/WLINK executable (repeat builds, relocations, decoder round-trip).
  No Rayman linking/packing reconstruction yet.
- **Next investigation: cross-unit register/stack allocation rules** (controlled experiments, STRICT vs NEAR
  structural pairs across unrelated units; rules enter docs/COMPILER.md only after multi-unit validation). One-unit
  finishing passes stopped (measured yield too low).
Wave 7/8 yield (2026-10-22): 29 finishing passes on exhausted NEAR pools gave +7 STRICT; whole-object/new-unit work
(sysutils +12) and BSS/data fragments gave the larger gains. Remaining NEAR are dominated by register/stack allocation.

## Link stage, first step - result (2026-10-22, build/workers/lk_pmw122/NOTES.md)
PROVEN on a minimal Watcom 10.6 program (pointer table, BSS, 297 relocations), PMODE/W 1.22 run under MS-DOS Player:
- WLINK `system pmodew` (shipped PMODEW.LNK) emits LE (LE at 0x2728); PMWLITE turns it into PMW1 at 0x2730. PMWBIND
  /B or /R + PMWLITE gives the identical file; PMWBIND never emits PMW1.
- The shipped utilities reproduce the oracle's complete first 10,032 bytes (64-byte MZ header, unchanged stub body,
  13 zero bytes) with no PMWSETUP change; PMW1 header fields (version 0x1601, flags, table offsets, object flags
  0x2045/0x2043) match the oracle.
- PMWLITE is deterministic (120 runs; PMWBIND 25) across names/dirs/timestamps; default = /C2; levels /C0../C4 all
  round-trip byte-exactly through tools/pmw1.py, and re-packing the decoded content reproduces the whole file.
OPEN: the oracle's compression level (no stored level field; levels change only encoding choices) and multi-block
behaviour (the oracle has 13+2 relocation blocks of 16,380 bytes; the probe had one block per object).
Proposed next step (needs user approval - it is packer reproducibility on oracle data): re-pack the decoded oracle
objects/fixups with PMWLITE /C0../C4 and compare with the oracle stream; this would fix the level and test
multi-block behaviour without any Rayman linking.
