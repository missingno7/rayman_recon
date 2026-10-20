# Next steps (ordered by value)

1. **Keep scaling TU matching** with the brief in build/workers/TEMPLATE_P2.md (it carries every proven lesson).
   Remaining candidate objects: `python tools/tumap.py --list`. Prefer gameplay objects with PS1 bodies; integrate in
   batches with `tools/promote.py` (+ review for byte-forcing declarations and duplicated shared types).
2. **Finish whole objects**: cage (1 NEAR), poisson (2), bat (3), audiocd (3) - closing the last functions makes the
   layout contiguous, which also verifies intra-object references (jump tables, function pointers). A strong model
   (gpt-6.1-sol) pass per object worked well for basic (19 -> 36).
3. **Data**: (a) bind the ObjectsFonctions/t_world_info pointer targets (function addresses from Rayverse/Android
   names + code matches) and reconstruct those tables; (b) start the central BSS: declare named globals in an order
   that reproduces the oracle BSS addresses under the proven rule (tools/bssrule.py), verified by a new BSS-placement
   check in match.py (BSS segment publics bound -> layout must equal); (c) consolidate duplicated block-local struct
   types (archive_header_t) into shared headers.
4. **Systematise the return-width/operand-order rules** in a small helper (try narrower return types and swapped
   comparison operands automatically for NEAR functions whose first difference is in the prologue) - cheap, many hits.
5. **Undetermined compiler options**: `-on` (needs FP code), `-ol`; Watcom 10.5 vs 10.6 still non-discriminating.
6. **Hand-written assembly** (1:00000.. low-level drawing, self-modifying code) - classify, then ORIGINAL_ASM units.
7. **Link stage** (later): WLINK 10.6 object order = tumap order; PMWLITE round trip after enough coverage.
8. **v1.12**: rerun accepted units against `dos_v112_us` to classify cross-version changes.
