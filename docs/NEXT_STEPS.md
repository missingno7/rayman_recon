# Next steps (ordered by measured value, 2026-10-21)

1. **Low-level assembly: done** (2026-10-21): 1:00010-06f7e in 8 STRICT `.asm` slices + VERIFIED data prefix. Open:
   one-module consolidation (only with evidence for the original module structure), assembler identity.
2. **Movie prefix 1:2a1c0-1:2b288** (34 anonymous routines; worker w5_flc decides C vs assembly).
3. **Finishing passes only with new evidence.** Most NEAR pools have had >=2 sol passes; wave-4 passes on exhausted
   units returned in 3-9 minutes with no gain (effect1/2, cymbal/save/display, affiche, pcfee). Re-open a unit when a
   new compiler fact appears (docs/COMPILER.md evidence log) or a layout fix lands before its NEAR functions.
   Largest pools: moteur 32, menu 28, ray 25, frame 22, breakout 17, affiche 16, basic 13, collision1 13.
4. **Layout closure**: each census brief names the first size-drifting function per object (W-LAYOUT); fixing it is
   the cheapest way to flip REFS_LAYOUT functions. fnd: matching INIT_FND also fixes LOAD_FND's jump table.
5. **OPEN compiler questions** with the widest payoff: the F3 `rep movs`/`rep stos` copy sequence (jumelle, effect1/2,
   six effect objects; not a 10.5/10.6 difference), clamp lowering in the effect objects, materialised booleans.
6. **Data**: ObjectsFonctions/t_world_info pointer tables; central BSS declaration order; `_DATA` object boundaries.
7. Link stage (WLINK object order = tumap order) and v1.12 cross-version classification - later. Not started.
   First step only (before any WLINK work): the oracle stub is PMODE/W **1.22** and C:\tools has PMWLITE 1.33 only -
   acquire PMODE/W 1.22 PMWLITE.EXE, then test on a trivial WLINK-built LE whether it is deterministic and whether
   our decoder round-trips its output. Only then ask whether decoded oracle objects re-compress to the original
   stream (packer reproducibility before link work, as in earlier projects).
