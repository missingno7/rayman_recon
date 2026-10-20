# Next steps (ordered by measured value)

1. **Census-guided finishing passes on every first-pass unit** (docs/CENSUS.md: guided passes solved 87% / 50% of
   NEAR functions vs ~46% unguided). Per unit: `python tools/census.py && python tools/census.py --brief UNIT`, worker
   brief = build/workers/TEMPLATE_FINISH.md + the census section. Largest open pools: collision2 (24 NEAR), basic (22),
   breakout (20), display (18), objupdate2 (17), meredenis (15), jumelle (13), moskito (11), input (10), bbmont (10).
2. **New objects**: `tools/seed.py draft START END` first (unchanged), then the census-guided pass. Untouched:
   `python tools/tumap.py --list` (menu/misc_source, moteur/neige, frame/guetteur, hifi/hybrid, load, box, effect,
   cheat, action/pcmain, skops (3 PS1-recovered bindings ready), lowlevel assembly).
3. **Layout closure**: REFS_LAYOUT functions (23) flip once earlier functions in their object have the right size;
   prioritise the last NEAR function before them in each object.
4. **Hand-written assembly ABIs**: when a unit calling a low-level routine is integrated, move its proven entry from
   docs/reference/lowlevel_abi.h into src/include/lowlevel.h and check the call sites.
5. **Data**: ObjectsFonctions/t_world_info pointer tables (targets now largely bound); central BSS declaration order;
   `_DATA` object boundaries.
6. Open compiler questions: `-oi`/`-ol`/`-om` non-discriminating so far; the DOS `CLD; REP MOVSB` copy idiom in
   jumelle; eta_t flags bit-view (collision1 GET_OBJ_ZDC proposal, OPEN).
7. Link stage (WLINK object order = tumap order) and v1.12 cross-version classification - later.
