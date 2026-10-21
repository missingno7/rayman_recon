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

## Open user decisions (2026-10-22)
- **Abs helper** (build/workers/r_abs/NOTES.md, proposed.h): `test eax,eax; jge; neg eax` as ORIGINAL_INLINE `Abs` closes
  dark DARK_phase1, DO_DARK_SORT_COMMAND and hybrid doMOSAMScommand (0 losses; Android has a real 10-byte `Abs`), but the
  same core is produced by plain C in 26 STRICT functions, so the AGENTS.md helper gate is not literally met.
- **Link stage**: acquire PMODE/W 1.22 PMWLITE and test determinism (first step only).
Wave 7/8 yield (2026-10-22): 29 finishing passes on exhausted NEAR pools gave +7 STRICT; whole-object/new-unit work
(sysutils +12) and BSS/data fragments gave the larger gains. Remaining NEAR are dominated by register/stack allocation.
