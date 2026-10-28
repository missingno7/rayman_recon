# Native renderer

## 1. Target architecture (user brief 2026-10-26, revised the same day)

```
reconstructed original game logic (src/, unchanged; runs once per frame, as on DOS)
        |  its own draw-order, culling and animation decisions
        v
complete semantic render frame (render/frame.h): ordered operations + semantic layers
        |
        v
ONE native rendering pipeline (render/native.c, render/world.c)
        |
        v
configurable viewport and presentation (4:3, 16:9, 21:9), later optional interpolation
```

- The original algorithms and data stay the source of truth. Nothing in the renderer re-decides gameplay,
  animation, camera or draw order; the original high-level draw code runs exactly once per frame and only its
  low-level output (the primitive calls) is translated into native operations.
- Every visible pixel of the native image comes from the native renderer. The original draw path is kept as an
  explicit **reference** for verification; it never contributes pixels to the native image, and it is not a
  fallback for unimplemented layers.
- 4:3 first: the native renderer must reproduce complete original 320x200 frames before wider viewports are built
  on it. Widescreen then means a larger rectangle for the same renderer, plus explicit policies (section 9).
- `src/` and STRICT matching are untouched. Port code observes the original at link time (no source change) or,
  where unavoidable, through reviewed derivatives (port/derive/).

## 2. Status (2026-10-28)

| Scope | Native? | Evidence |
|---|---|---|
| Gameplay frames of the normal-mode loop (`DO_MAIN_LOOP_PC_NORMAL`): background bands/columns or clear, rear and front weather, map, objects (Rayman, enemies, projectiles, cymbals as sprites), pixel particles, foreground band sprites, firefly darkness, border clear, HUD sprites, time display | **yes, complete in all six demos** | 11,812 gameplay frames, every one drawn only from its recorded frame and equal to the original draw buffer and display over the full 320x200 image (section 8) |
| Native operations no demo frame reaches (background-off clear, stars, deformation-free boss bar, fairy text, time display, colour sprites) | yes, unverified | same operations as above, but no frame of the demos exercises them (section 4 "parity"); test scenarios needed |
| Scenes outside the gameplay loop reached by the test scenarios: start-up logo, title animation, main menu with plasma boxes, options menu with the sound gauge, save-slot screen with name entry (deformed letters), world map with its binocular view, world vignettes with the page-curl, segment-quad and moving-lens transitions, the in-game options menu (greyscale capture of the frame, translucent frame, zoom-in and fades), the pause screen (spinning snapshot of the frame), the credits, the intro movie (FLC player), the world map's fire-animated quit box | **yes** | every frame and every presented image of the fifteen scenarios (section 8) equal to the reference; further start-up clocks (epochs 1000-8000) reach the row-wipe and segment-rasterizer transitions, all exact |
| Remaining primitives: dword copies, diff sprites and tiles outside their semantic layers, read-backs into game memory | **no**: recorded as unsupported operations | counted per pass by the gate; none in the scenarios |
| Scenes no scenario reaches yet: the other transition effects (effect2, effect50350, effect6ec80, effecthelp), level animations, in-game vignettes, the pause screensaver's later phase, continue, game over, victory, the conclusion movie, blood melt, the later worlds' levels | **unknown** (untested) | each will be measured when a scenario reaches it; pixel loops in C need a captured operation (section 6) |
| Wider view (`--widescreen`, `--wide-pad N`) | **yes**, drawn by the same native renderer | gameplay state unchanged in all six demos, margins drawn (section 9) |
| Palette: DAC, fades, special palettes, cycling | shared | both renderers present indexed pixels through the same emulated DAC; palette state is game/presentation state, not pixels |
| Mode X | not presented by the port | DIVERGENCES.md |

The **default presentation is still the reference** (`--renderer reference`). `--renderer native` presents only
the native renderer's display; it shows exactly the reference images through the whole of every demo run, and
elsewhere what the native renderer can draw (gaps are visible, never filled from the original). The default
switches at stage D (section 10), once the menus and the remaining scenes are native.

## 3. Where a frame comes from

`DO_MAIN_LOOP_PC_NORMAL` (src/pcmain.c 478, DOS 1:070e0), one iteration per built frame:

1. palette work (`fades`, `DoPaletteSpecialPC`, `DO_SWAP_PALETTE`), then **swap**: `Swap_To_Screen` copies the
   frame built in the previous iteration from the draw buffer to the display (the viewport rectangle, or the full
   width while `fixon`/`fixontemp`);
2. `display_mode + 1` engine updates (`DO_GROS_MOTEUR_NORMAL`); `update_display_map` also moves the draw buffer's
   origin (`DrawBufferNormal`) by the camera's 1..4-byte alignment;
3. build the next frame: the passes of section 4, in order;
4. a missed deadline runs one more engine update without drawing (`DO_GROS_MOTEUR_NORMAL(1)`).

Simulation and presentation are therefore not one-to-one; the native frame is recorded while the original builds
it and drawn at its swap, so it is always the frame the original swaps. Nested scene loops (vignettes, in-game
options, pause) run their own swaps inside an iteration (section 6). Scenes outside the gameplay loop swap through
`SYNCHRO_LOOP`'s full-screen copy (`DisplayBufferModeNormal`).

## 4. Rendering coverage matrix (normal-mode gameplay frame)

Order = pass order in the loop. "Native op" = the frame operation(s) that represent the pass; "parity" = frames of
the six demos in which the pass drew and was checked (pass check of section 8, plus the full-frame check).

| # | Original pass (file) | Consumes | Output, clipping, transparency | Side effects (original, kept: the pass runs once) | Native op | Parity / gaps |
|---|---|---|---|---|---|---|
| 1 | `CLEAR_FIXE_LUCIOLE` (luciole.c 81) | `RayEvts.firefly`, `id_Cling_Old` | firefly HUD black boxes (`DrawBlackBoxNormal`, unclipped) | `id_Cling_Old = 0` | BLACK_BOX | demo 3 (4,032 boxes) |
| 2a | loop clear (pcmain.c 538) when the background is off | - | `memset` 16008 dwords from the draw buffer origin | - | CLEAR (captured at the end of pass 1) | not reached: every demo level has its background on |
| 2b | `Calcul_Deplacement_Bande` + `Display_Back_Screen` (skops.c 200, 450) | bands, `PLAN0BIT`, `ymap`, `Val_*_Scroll_Y`, curtain object, sprite clip box | band rows from `W1 - BufferNormalDeplt`, width rounded up to 8 (writes up to 4 px left of the viewport); band sprites as diff sprites (4-px units, `(dest & mask) | image`, exact clip-box arithmetic incl. its gaps) | `Calcul_Deplacement_Bande` (called by the loop, not the pass) advances `Bande[].field_6` once per drawn frame; the heat effect advances its phase | BACKGROUND (semantic: bands, scrolls, sprites captured at pass start) | all six demos; heat effect (world 5 level 10) unverified |
| 3 | `display_flocons_behind` (affiche.c 838), after `flocon_clipping(H1, H2-6, W1, W2-4)` | weather groups 3..7 (unprojected positions, depth by group) | weather stamps, anchor tested against the weather box (upper bounds unsigned, inclusive), whole stamp written | none found | WEATHER | demo 4 (518,178 stamps) |
| 4 | `DRAW_MAP` (map_ngawe.c 67) | `BIG_MAP`, `display_map` window | 16x16 tiles: opaque / two-bit row kinds, `(dest & mask) | tex`; 4-px clip rounding; nothing when the grid is <= 1 tile | none | MAP (semantic: tile window captured at pass start) | all six demos |
| 5 | `display_grp_stars` -> `display_etoile` (affiche.c 1246, 1216) | grapple star list | sprites; culled against 320x200, not the viewport | **consumes `myRand`**, advances `grp_stars`/`current_star` | SPRITE | not drawn in the demos |
| 6 | `DISPLAY_ALL_OBJECTS` (affiche.c 753) -> `display2`, `DISPLAY_CYMBALE`, `display_bar_boss`, `DISPLAY_DEFORMATION` | active list, priorities 7..1 (+ Rayman after 3, cymbals after 2, first priority-0 type 245), animations, `obj->screen_x/y`, scale | sprites (marker runs, mirror slot, colour OR), clip box (scoped H2-40 for types 252/251/173); deformed quads for type 254 | Rayman blink predicate reads `display_mode`; deformation scratch | SPRITE; deform = unsupported | all six demos (494,605 sprites); deformation, boss bar not drawn in the demos |
| 7 | `display_pix_gerbes` (affiche.c 951) | Q6 particles | `fplot_Normal`, top-edge cull only | none (the engine kills particles outside the `_Aff` box) | PIXEL | demos 0 and 4 (92,654 pixels) |
| 8 | `Display_Sprite_On_Front` (skops.c 381) | foreground band sprites (zero-length bands), `ymapmax` | diff sprites by the level's scroll type, **whether or not the background is on** | none | FRONT (semantic) | all six demos |
| 9 | `display_flocons_before` (affiche.c 898) | weather groups 0..2 | weather stamps | none found | WEATHER | demo 4 (128,900 stamps) |
| 10 | `Display_and_free_luciole` (luciole.c 269) -> `aff_luciole`/`plot2line` | lit-window centre, `rayon_luciole`, full viewport `Bloc_lim_*_Glob` | **C writes**: zeroed spans of a ring | computes `ADDLUCLIP`, updates `old_*_luc`, **restores `Bloc_lim`** (the sprite clip box stays narrowed) | FIREFLY (semantic: inputs read after the pass) | demo 3 |
| 11 | `clear_borders_Normal` (loop) | viewport | 4 bytes left of the viewport and at its right edge, height rounded to even | none | CLEAR_BORDERS | all six demos |
| 12 | `DISPLAY_TEXT_FEE` or `DISPLAY_FIXE` (affiche.c 1275, 605) | status bar, text, `fixontemp` | no-clip sprites, text glyph sprites, black boxes; at `fixontemp == 1` **C writes** `ClearBorder` into the display, then the draw buffer | `fixontemp` countdown (also selects the swap width), fairy `screen_y`, cling `screen_x/y`, `id_Cling_Old` | SPRITE, BLACK_BOX, CLEAR_OUTSIDE | HUD sprites in all six demos (82,709); ClearBorder into display and draw buffer in demo 3; fairy text not drawn |
| 13 | `DisplayCrackers`, `DisplayProgrammerMessage` | - | text sprites | none | SPRITE | not drawn |
| 14 | `display_time` (affiche.c 1373) | `left_time`, `map_time` | digit sprites | **`ray.is_active = 0` while `map_time <= 100`**, prompt colour | SPRITE | not drawn in the demos |
| 15 | `DISPLAY_GAME_VIGNET`, `DO_OPTIONS_IN_GAME`, `Do_Effect_Pause` | | nested scene loops with their own swaps, picture copies, read-backs of the built frame (menu greyscale capture, pause snapshot) | many (audit: menu resets, `myRand` in pause) | - | **not native** (section 6) |
| 16 | `HANDLE_KEY` | keys | none | none | - | - |

Coordinates: objects `obj->screen_x/y` (world - camera + 8 / - camera, calc_obj_pos), map tiles 16c - xmap + 8,
stars world - camera (no +8), particles Q6 screen, weather projected screen positions (centre 160,170), HUD fixed
or viewport-relative anchors. docs/port/RENDERING.md has the full contracts.

### Draw functions that change state

Passes 1, 2b, 5, 10, 12, 14, 15 change state later frames depend on (star RNG, `ray.is_active`, `fixontemp`,
`Bloc_lim`, band scroll). The architecture keeps them correct by construction: the original pass runs exactly once
per built frame, in the original loop, and the native renderer never calls game code. Rendering the same recorded
frame again (a wider view, a later interpolated presentation) touches no game state. A future presentation that
draws more often than the game builds frames must therefore interpolate recorded frames, never call these passes.

## 5. Implementation

```
DO_MAIN_LOOP_PC_NORMAL (src/, unchanged)
   | pass calls ------------------- -Wl,--wrap ---> render/scene_capture.c (pass markers; semantic layers read from
   |                                                game state at pass start: background, map, front; firefly after)
   | primitive calls (slots) ------ -Wl,--wrap ---> render/record.c: append operation, then call the original
   |                                                primitive (which keeps building the reference draw buffer)
   | Swap_To_Screen / SYNCHRO swap - -Wl,--wrap ---> render/record.c: draw the frame natively (render/native.c,
   v                                                render/world.c) into the native draw surface, swap it into the
 reference draw buffer -> display                  native display; verify; next frame
                                                          |
 video/vga.c present(): native display (--renderer native) or the original display (reference) -> platform/host.c
```

- **Recording** (render/record.c). Every drawing primitive of port/video/lowlevel_normal.c, the joystick swaps of
  compat/src/gameport.c and the passes of the loop are wrapped at link time (port/CMakeLists.txt
  `RAY_RENDER_PASSES`, `RAY_RENDER_PRIMITIVES`). A wrapper appends one operation (logical coordinates converted
  from buffer offsets, the clip state in force, the image pointer, the pass) and calls the original once. Inside the
  semantic passes (background, map, front) primitives are not recorded: the layer operation stands for them, and
  the pass check compares it with what they drew.
- **Memory operations**: the historical units' `memcpy`/`memmove`/`memset` are routed to port services by the
  pre-included compat/include/port_watcom.h (like `fopen`), and the original inline helpers `fill`/`copy` and the
  library's `clear_memory` are wrapped. A copy or fill into a target becomes a FILL or COPY operation; a copy's
  source bytes are captured into the frame's arena when recorded (the game reuses scratch buffers such as PLAN2
  later in the same frame), or, when the source is itself a target, the native renderer copies between its own
  surfaces. A copy out of a target into other game memory is a **read-back** (counted as unsupported). Scenes
  outside the gameplay loop (logo, title animation, vignettes) consist largely of these operations.
- **Scene labels**: the scene functions PcMain calls (`DO_UBI_LOGO`, `DEPART_WORLD`, `DO_MAIN_LOOP_PC_NORMAL`, ...)
  are wrapped only to label frames; `--verify-render` reports frames, complete and exact frames per scene
  (`render scene`), operations per pass (`render ops`) and effect passes by kind (`render effect`).
- **Screen effects** (render/effect_capture.c, render/effects.c): an effect's pixel pass is reached from another
  unit, so it is wrapped like a primitive. The wrapper records the pass's inputs (the effect state before its update,
  its tables, the source and destination buffers as target + offset) as one EFFECT operation and runs the original
  once with recording suspended; render/effects.c reproduces the pixel pass on the native surfaces. The effect's
  private state layouts are copied in render/effect_layouts.h and held to the originals by a build-time gate
  (render/effect_layout_*.c compiles the unit next to the copies and asserts every field offset and size).
  Implemented: page curl (effect633e0.c `effect_helper_63838`), moving lens (effect6c3b0.c `effect_helper_6c9bc`),
  plasma boxes (box.c `PlasmaBox`), world-map binoculars (jumelle.c `DisplayJumellesNormal`), and the segment
  rasterizer `draw_effect_segment` (a primitive, recorded with its descriptor; its texture is staged in the effect
  buffer).
- **Deformed sprites**: `draw_sprite_deform` renders into a game scratch buffer (`new_sprite_deform`) that the next
  sprite draws and the next deform reuses. The deform is a native operation writing the native renderer's copy of
  that buffer (a scratch slot, render/effects.h); sprites whose image lies in a registered scratch range read the
  slot. No byte of the original deform output is used.
- **Images decoded or read into video memory**: asset decoders that write a target directly (`LoadPcxOptions`, the
  menus' pictures in the effect buffer; the FLC movie player's frames in the draw buffer) and file reads into a
  target (`read`/`_dos_read`: the movie's compressed frames in the effect buffer) are game code, not drawing; their
  output, read from the buffer when the decoder or read returns, becomes one image operation (no drawing primitive
  has touched those bytes). The movie decoder's frame loop is inside its unit: the existing movie.c derivative calls
  `port_record_asset` after each frame's chunks are decoded, before the frame is paced and copied to the display
  (provenance.json).
- **Frames without swaps**: scenes that copy to the display and present without swapping (the movie player) end a
  frame at each presentation outside gameplay, so a frame stays one presented image.
- **Fire**: `FireBox` (message boxes, credits) advances the fire field, consuming random numbers, then copies its
  rows into the draw buffer; the wrapper runs it once and records the field's output rows as an image.
- **Image lifetimes**: sprites drawn from the level and fixed atlases keep pointers (stable until a load); the
  loader animation's zero-transparent sprites, decoded per picture into a reused buffer, are copied into the frame.
  Under `--verify-render` every pointer-held sprite image is hashed when recorded and again when drawn
  (`volatile_images` must stay 0).
- **Menu fades**: `apparition_prg`, `fondu_prg` and `FonduPixel_prg` are `SYNCHRO_LOOP` scene callbacks called
  through a pointer inside frame.c; `SYNCHRO_LOOP` itself is wrapped (render/menu_capture.c) and substitutes
  trampolines that record each pass with the step it starts from, then run the callback once. `CaptureVideo`
  (greyscale capture of the built frame for the in-game menu), `CadreTrans` (translucent frame) and
  `DISPLAY_FOND_MENU` (in-game viewport copy of the menu background) are wrapped calls. `FonduOption`'s two
  in-place greyscale loops run inside a function called within its own unit: the one derivative of this work,
  port/derive/menu.patch, adds an observation call before each (no other change; provenance.json).
- **Draw-buffer origin**: when `DrawBufferNormal` moves (the camera alignment in `update_display_map`, the reset in
  `FIN_GAME_MODE_NORMAL`), an ORIGIN operation shifts the native draw surface by the same number of bytes, so
  pixels that are not redrawn keep their memory position as in the original (found by a frame swapped by the menu
  right after a demo).
- **Frame** (render/frame.h): ordered operations plus the semantic payload (render/scene.h). Targets: the draw
  buffer, the display (primitives and scenes that write it directly), and the off-screen effect buffer
  `EffetBufferNormal` (two 64000-byte images: menus, pause, world map and transitions draw and stage pictures there).
  The swap is an operation too, so the native renderer draws operations incrementally: at every swap and before
  every present.
- **Native renderer** (render/native.c, render/world.c; portable C, no game headers): one function per operation,
  each reproducing the original per-pixel rules (ASM_SPEC.md) on a surface of any size and origin. Sprites keep the
  clip-box arithmetic of `clip_sprite_on_screen` (unsigned upper compares, source advance rules, crop inside a
  marker run); band sprites keep the diff-sprite unit clipping including its two gaps.
- **Native surfaces**: a draw surface with 32-pixel margins (a write past a row's end lands beyond the image, where
  the original's border clear leaves no visible difference; the full-frame check confirms it), and a 320x200
  display, and a 320x400 effect surface. They persist between frames like the original buffers; nothing is ever
  copied into them from the original buffers.
- **Presentation**: `--renderer native` makes video/vga.c present the native display; the frame log, dumps and the
  host see that image.

## 6. Where original pixels are still required

1. **Pixel loops in C** outside the scenes already covered: the credits' fire (box.c), the remaining transition
   effects (effect2, effect50350, effect6ec80, effecthelp; effect34580's spans go through `copy` and are
   already native), the pause screensaver, blood melt, and the FLC movie player writing the display. Each needs an
   operation captured where the original produces it, as for the page curl, lens, plasma and binoculars (section 5:
   wrap of a cross-unit pixel pass, otherwise a minimal reviewed derivative). Copies, fills and decoded pictures
   (logo, vignettes, `DISPLAY_FOND3`, loading screens, menu backgrounds) are already native.
2. **Read-backs**: the pause snapshot (`effect_copy_band(DrawBufferNormal)`), the simple pause
   (`SaveScreen(display)`), blood melt. They read the reference buffers; in the native architecture they must read
   the native surfaces, as the in-game menu's greyscale capture and the menu fades now do (their operations read
   the native draw surface and the native effect buffer).
3. **Unsupported primitives** (counted by the gate): `draw_sprite_deform`, `DrawFondBoxNormal`,
   `DrawBorderBoxNormal`, `draw_effect_segment`, `copy_dwords`, buffer copies other than the full swap, diff sprites
   and tiles outside their semantic layers, copies that straddle a target's bounds.
4. **The reference draw path still runs** in both modes (each wrapper calls the original primitive). Stage D can
   stop it outside verification only when no game code reads the reference buffers any more (item 2).

Persistence: the original never clears the draw buffer while the background is on; pixels outside the viewport,
the HUD area and the firefly darkness persist, and the swap copies only the viewport. The native surfaces persist
the same way, including the origin shifts of the draw buffer (ORIGIN operations, section 5).

## 7. Frames, snapshots and resource lifetimes

A recorded frame holds plain values (positions, clip boxes, colours, pass, target) and pointers to game-owned
resources: sprite images (`obj->img_buffer` atlases), band image `PLAN0BIT`, masks, tile textures. They are valid
until the level's resources are released, at the earliest at the next level load; the current presentation draws a
frame synchronously at its swap, so this holds. Before frames are retained for asynchronous presentation or
interpolation (stage F) the frame gets a resource generation (incremented where the game frees or reloads level
memory) and the presenter drops frames of an older generation. Images are not copied per frame.

Identity: operations carry `source = -1` today. Stage B tags each object operation with its object index and an
allocation generation (needed for interpolation, section 10); the tagging point is the object loop of
`DISPLAY_ALL_OBJECTS`, which calls `display2` inside its own unit (not wrappable at link time): a small reviewed
derivative or an observation inside the sprite wrapper keyed by the active-list traversal is required.

Drawing a frame reads only the frame: rendering the same frame twice gives the same pixels and changes no game
state, consumes no random numbers.

## 8. Verification

- **Full-frame parity** (`--verify-render`, at every swap): the native draw surface against the original draw
  buffer and the native display against the original display, all 320x200 pixels. A frame is *complete* when every
  operation was drawn natively and none was lost. Reported separately for gameplay frames and other frames.
- **Pass checks** (diagnostics): each pass's operations drawn over the original buffers as they were when the pass
  began, compared with the original buffers when it ends (draw buffer inside the viewport, where later passes do not
  overwrite scratch pixels; whole display). They localise any difference, including direct C writes no primitive
  reports.
- **Presentation independence**: one gameplay-state digest per frame swap (camera, clock, random table and index,
  every object from `is_active` to the end of `obj_t`); identical with `--renderer native`, with `--widescreen`,
  across runs and across x86_64/i686.
- **Presented images**: before every present the native display is compared with the original display
  (`presents`, `present_exact`), and the frame log of a `--renderer native` run hashes the native image.
- Gates: `tests/e2e_native.py` (ctest `port_native_demos`): fifteen scenarios (six demos, the idle menu path, and
  key scripts in tests/keys: a new game to world 1 level 1, the same under two other start-up clocks (`--epoch`:
  the vignette transition is drawn from the game's random table, seeded from the clock), the options menu, the
  in-game options menu, pause, the credits, and with the movie data the intro movie, world map and quit box),
  reference and native presentation:
  presented-frame logs identical over the whole run, every frame (gameplay and other) complete and exact, every
  presented image exact, every gameplay pass exact, state logs identical. `tests/e2e_wide.py` keeps the margin
  experiment honest (section 9).
- Debugging: `RAYMAN_RENDER_TRAP="FRAME X Y [TARGET]"` calls `port_record_trap(address)` when frame FRAME starts,
  with the address of pixel (X, Y) of the draw buffer, display or effect buffer: a debugger watchpoint there names
  the code that writes it (`gdb -x` with `break port_record_trap`, `watch -l *(unsigned char *)$rcx`). This found
  each pixel loop above. `RAYMAN_RENDER_EXPLAIN="FRAME X Y [TARGET]"` lists the native operations that change that
  pixel (`RAYMAN_RENDER_EXPLAIN_ALL=1`: every operation of the frame).
- Result 2026-10-26 (x86_64, 6000 presented frames per demo): gameplay frames 2,094 / 1,628 / 1,800 / 2,016 /
  2,578 / 1,696 (demos 0-5, 11,812 in all), every one complete and equal to the original in draw buffer and
  display; every gameplay pass exact; state logs identical in reference and native presentation. Operations drawn
  natively: 494,605 object sprites, 82,709 HUD sprites, 92,654 particle pixels, 647,078 weather stamps, 4,032 black
  boxes, 2,016 firefly rings, 2 border clears, and one background, map, front and border-clear operation per
  frame. `--verify-render` prints these counts (`render ops`).
- Whole runs (6000 presented frames each): the 1,758 frames before gameplay (logo 2, title animation 1,419,
  world vignette and page curl 336, ...) are complete and exact in every demo; the idle menu path (1,817 menu frames,
  6,839 presents) and the scripted new game (1,859 menu and name-entry frames, 70 world-map frames, 279 vignette and
  lens-transition frames, 5,995 gameplay frames; 16,416 presents), the options menu (2,296 menu frames) and the
  in-game menu (7,317 gameplay-loop frames including the menu's capture, zoom-in and fades; 18,461 presents),
  pause (20,422 presents), the credits (9,779 presents) and the intro movie (1,805 movie frames, 6,813 presents)
  likewise; every presented image equals the original display, and the native presentation's frame logs equal the
  reference's.

## 9. Widescreen

`--widescreen` (54 more columns per side: 428 x 200, 16:9 at the original pixel aspect) or `--wide-pad N` (0..160)
makes the native renderer draw a wider view of the same recorded frames. There is one renderer and one frame: the
view is the `pad` parameter of `port_native_draw` (render/native.h); the original draw path, which has 320 columns,
is not involved in what is shown (`--widescreen` implies `--renderer native`). The slice-1 margin compositor, which
framed the original image with separately drawn margins, is removed.

Rules of the wider view (render/native.c), applied to the draw buffer of gameplay frames:

| Item | Original rule | Wider view |
|---|---|---|
| Presentation camera | the game's camera `xmap`, clamped to `[scroll_start_x, scroll_end_x]` (moteur.c) | unchanged; the wider window onto the world (`-pad .. 320 + pad`, world x = screen x + xmap - 8) gets a presentation offset that keeps it inside the level map: 0 when there is room, otherwise the window stops at the map edge and the original 4:3 area moves off centre. A map narrower than the window is centred, with black beyond its edges. World passes (background, weather, map, stars, objects, particles, foreground, firefly) are drawn through the window; HUD and text passes stay at their screen positions. Computed from the recorded frame (camera, map width); the game never sees it |
| Bounds equal to the full viewport (x within 0..8 to 312..320) | the original's clip boxes and viewport | widen to the window: sprite clip box (its arithmetic runs shifted right by `pad`, so the unsigned comparisons see the original's non-negative values), weather bounds, firefly darkness, the world layers, the gameplay swap |
| Background, map, foreground band sprites | original viewport rules (band copy extent, two sprite instances, no map when the grid is one tile) | the layers' extended rules over the whole view; background columns beyond the stored image wrap through its other half only when the image is two identical halves (checked per image), otherwise they stay black |
| Original 4-pixel left border (`clear_borders_Normal`, ClearBorder's columns) | cleared every frame | **not drawn** (user decision 2026-10-28): the world continues there |
| Objects, Rayman, projectiles | drawn for every active object (activation ~1320x1200 around the camera), clipped to the box | visible in the margins as far as active objects reach |
| Particles | the engine kills them outside the `_Aff` box (gameplay state) | none beyond that box; the simulation is not changed |
| Stars | culled against 320x200 in the pass, consuming random numbers | none beyond it; the cull and its random numbers stay |
| Weather | flakes spawn through a 320x200 inverse projection | drawn wherever flakes exist; few in the margins |
| Firefly levels | darkness outside the lit window, within the viewport | the darkness covers the margins too |
| Narrowed viewports (breakout, world 6 level 4) and their swaps | the swap copies the viewport | the original 320 columns, black margins |
| HUD | fixed anchors / viewport-relative | kept at the 4:3 positions (policy; moving it to the edges is a later option) |
| Menus, vignettes, movies, transitions, pause (screen-space scenes) | 320x200 images | the original 320 columns centred, black margins |
| Draw-buffer origin moves | linear memory shift | each row shifts by the same number of pixels (rows stay apart in the wider surface) |

A map narrower than the window is exercised by demo 4 (a vertical-scrolling level 320 pixels wide: centred, black
borders). Not yet exercised by a test: boss arenas whose scroll limits lock the camera (the window follows the map,
so it can show the surroundings of an arena).

Known content limits (the original art, not the renderer): a column background is a 320-pixel picture, so its
repetition in the margins shows a seam where its edges meet; where a level places no object or tile beyond the
original screen (for example the water sprites at the bottom of some levels), the margin stays black.

Verification: the original view stays gated pixel for pixel (`port_native_demos`); `--verify-render` runs at the
original width only. The wider view is gated by `port_wide_demos` (tests/e2e_wide.py): gameplay-state logs identical
to the original-view run in all six demos, every frame 428 columns, margins drawn in gameplay frames (black by
design in demo 3, firefly darkness, and demo 4, a map 320 pixels wide).

## 10. Roadmap

| Stage | Content | Status |
|---|---|---|
| A ownership audit | draw path, stateful passes, direct writes, read-backs, viewport decisions; full-frame parity strategy | done for the normal gameplay loop (this document, audit facts in sections 4 and 6); Mode X loop not audited |
| B complete semantic frame | display list of every drawing operation + semantic layers; identity tags; resource generations | operations and layers done for gameplay frames; identity and generations open (section 7) |
| C complete native software renderer | every operation drawn natively, full 320x200 parity | fifteen scenarios complete: demos, menus, options, save slots and name entry, world map, three transitions, gameplay, in-game options menu, pause, credits, intro movie; next: scenarios for the scenes still unreached (level endings, vignettes, continue, game over, victory, the other transitions, later worlds) |
| D native renderer is the normal output | default `--renderer native`; read-backs from native surfaces; reference path only under verification | open |
| E viewport independence | wider draw surface, same operations, section 9 policies; remove render/compose.c | first version done (section 9): one renderer draws the wider view, the compositor is removed; open: HUD placement, wider vertical views, finer policies for screen-space scenes |
| F presentation decoupling and interpolation | present recorded frames at the monitor's rate; interpolate tagged positions; snap on teleports, spawns, slot reuse, animation resets, scene changes, camera jumps; never more game updates | open |
| G optional GPU backend | same frame contract | open |

Next concrete step: scenarios that reach a level ending (vignette, continue), game over, victory and the later
worlds, which need recorded play (BizHawk replays converted to key scripts, or saved games), then each scene's
remaining pixel loops as captured operations, found with the trap and explain tools and measured scene by scene with
the per-scene report.
