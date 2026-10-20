# Next steps (ordered by value)

1. **Scale TU matching with workers** (highest value). `python tools/tumap.py --list` gives 83 candidate objects
   with their Rayverse functions; brief one worker per object (template: build/workers/tu_bat/PROMPT.md, rules in
   AGENTS.md), review every result for byte-forcing declarations before `tools/promote.py`. Prefer engine/gameplay
   objects with PS1 C counterparts (cross-platform evidence) and small objects first: basic.c (1:0d070), blood
   (1:13800), blacktoon (1:13030), cage (1:18b40), cymbal (1:21f00), ngawe region (1:4d630.., inside a larger
   candidate object). Integrate in small batches; `tools/verify.py` must stay at 0 regressions.
2. **Shared headers**: build `src/include/` types (obj_t, eta_t, anim_t, ...) from Rayverse types.h + PS1 include/,
   validating every field offset against STRICT code; replace per-unit partial structs. Settles `-zp` too.
3. **Open discriminators** (docs/COMPILER.md): match one FP-using function (`-on`, `-fp*`), one with intrinsic
   candidates (`-oi`), loop-heavy code (`-ol`); try 10.5 vs 10.6 on each new unit.
4. **Data verification**: extend match.py (or a datacmp tool) to compare a TU's `_DATA`/`CONST` against obj2 and to
   order `_BSS` (kegg_recon docs/compiler-notes.md has the Watcom 10 `_BSS` hash-order rule; re-prove it for 10.6).
5. **Hard leftovers** for a stronger model (gpt-6.1-sol): `start_cd_gameover` `mov eax,eax`; DO_BAT_COMMAND frame.
6. **Hand-written assembly**: classify non-Watcom code regions (no cstrt-style prologues, self-modifying code at
   1:00010...), decide ORIGINAL_ASM reconstruction with WASM/TASM once evidence of the original assembler exists.
7. **Link stage**: WLINK 10.6 experiments (object order = tumap order, `_TEXT` alignment, stub), then PMWLITE
   recompression round trip (PMODE/W 1.22 is not available; 1.33 PMWLITE behaviour must be compared first).
8. **v1.12**: rerun units against `dos_v112_us` (bindings overlay per oracle) to classify each function as
   CONFIRMED_SAME_LOGIC / CONFIRMED_VARIANT between versions; consider version conditionals in src/ later.
9. Dynamic analysis only when needed: MS-DOS Player first, DOSBox-X when the PMODE/W program needs it.
