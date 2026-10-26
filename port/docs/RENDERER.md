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

## 2. Status (2026-10-26)

| Scope | Native? | Evidence |
|---|---|---|
| Gameplay frames of the normal-mode loop (`DO_MAIN_LOOP_PC_NORMAL`): background bands/columns or clear, rear and front weather, map, objects (Rayman, enemies, projectiles, cymbals as sprites), pixel particles, foreground band sprites, firefly darkness, border clear, HUD sprites, time display | **yes, complete in all six demos** | 11,812 gameplay frames, every one drawn only from its recorded frame and equal to the original draw buffer and display over the full 320x200 image (section 8) |
| Native operations no demo frame reaches (background-off clear, stars, deformation-free boss bar, fairy text, time display, colour sprites) | yes, unverified | same operations as above, but no frame of the demos exercises them (section 4 "parity"); test scenarios needed |
| Deformed sprites (type 254, `draw_sprite_deform`), fond/border boxes, picture copies, effect segments, dword copies, diff sprites outside the band layers | **no**: recorded as unsupported operations | counted per pass by the gate |
| Scenes outside the gameplay loop: start-up logos, menus, world map, vignettes, in-game options, pause, credits, loading screens, movies | **no** | their C code writes the buffers directly (section 6) |
| Palette: DAC, fades, special palettes, cycling | shared | both renderers present indexed pixels through the same emulated DAC; palette state is game/presentation state, not pixels |
| Mode X | not presented by the port | DIVERGENCES.md |

The **default presentation is still the reference** (`--renderer reference`). `--renderer native` presents only
the native renderer's display; in gameplay frames it is identical to the reference, outside them it shows what the
native renderer can draw (gaps are visible, never filled from the original). The default switches at stage D
(section 10).

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
- **Frame** (render/frame.h): ordered operations plus the semantic payload (render/scene.h). Targets: the draw
  buffer, or the display for primitives that write it directly.
- **Native renderer** (render/native.c, render/world.c; portable C, no game headers): one function per operation,
  each reproducing the original per-pixel rules (ASM_SPEC.md) on a surface of any size and origin. Sprites keep the
  clip-box arithmetic of `clip_sprite_on_screen` (unsigned upper compares, source advance rules, crop inside a
  marker run); band sprites keep the diff-sprite unit clipping including its two gaps.
- **Native surfaces**: a draw surface with 32-pixel margins (a write past a row's end lands beyond the image, where
  the original's border clear leaves no visible difference; the full-frame check confirms it), and a 320x200
  display. They persist between frames like the original buffers; nothing is ever copied into them from the
  original buffers.
- **Presentation**: `--renderer native` makes video/vga.c present the native display; the frame log, dumps and the
  host see that image.

## 6. Where original pixels are still required

1. **Scenes outside the gameplay loop** write the buffers directly from C (audit: start-up logo `memcpy` of PLAN2
   (ray_pc.c 58), menus: `CaptureVideo`, `FonduOption`, `fondu_prg`, `apparition_prg`, `CadreTrans`, plasma, fire,
   gauges; world map: binoculars `DisplayJumellesNormal`; vignettes: `DISPLAY_FOND3` PLAN3 copy; pause and screen
   effects: `effect_*` band copies and segment rasterizer; loading screens: `fill`; movies: the FLC player writes
   the display). Each needs a semantic operation captured where the original produces it (link-time wrap of a
   cross-unit call where one exists, otherwise a minimal reviewed derivative).
2. **Read-backs**: the in-game menu's greyscale background (`CaptureVideo(draw_buffer)`), the pause snapshot
   (`effect_copy_band(DrawBufferNormal)`), the simple pause (`SaveScreen(display)`), menu fades (read-modify-write),
   blood melt. They read the reference buffers. In the native architecture they must read the native surfaces
   (a native operation "capture the built frame" whose source is the native surface).
3. **Unsupported primitives** (counted by the gate): `draw_sprite_deform`, `DrawFondBoxNormal`,
   `DrawBorderBoxNormal`, `DisplayAnyPictureNormal`, `draw_effect_segment`, `copy_dwords`, buffer copies other than
   the full swap, `ClearDrawAndDisplayBufferNormal`, diff sprites and tiles outside their semantic layers.
4. **The reference draw path still runs** in both modes (each wrapper calls the original primitive). Stage D can
   stop it outside verification only when no game code reads the reference buffers any more (item 2).

Persistence: the original never clears the draw buffer while the background is on; pixels outside the viewport,
the HUD area and the firefly darkness persist, and the swap copies only the viewport. The native surfaces persist
the same way. The origin shift of item 3.2 is not modelled (only pixels never redrawn are affected; the full-frame
check would show it).

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
- Gates: `tests/e2e_native.py` (ctest `port_native_demos`): all six demos, reference and native presentation, every
  gameplay frame complete and exact, every gameplay pass exact, state logs identical. `tests/e2e_wide.py` keeps the
  margin experiment honest (section 9).
- Result 2026-10-26 (x86_64, 6000 presented frames per demo): gameplay frames 2,094 / 1,628 / 1,800 / 2,016 /
  2,578 / 1,696 (demos 0-5, 11,812 in all), every one complete and equal to the original in draw buffer and
  display; every gameplay pass exact; state logs identical in reference and native presentation. Operations drawn
  natively: 494,605 object sprites, 82,709 HUD sprites, 92,654 particle pixels, 647,078 weather stamps, 4,032 black
  boxes, 2,016 firefly rings, 2 border clears, and one background, map, front and border-clear operation per
  frame. `--verify-render` prints these counts (`render ops`).
- Non-gameplay frames: 1,758 start-up frames per demo run are recorded but not yet native (section 6.1).

## 9. Widescreen

Slice 1 (commit 9ae7217) added `--widescreen`: the reference 320x200 image unchanged in the centre and margins
drawn by world.c from the frame's background/map/front layers (render/compose.c). That **hybrid compositor is an
experiment and will be removed**: it combines two pipelines in one image, and objects, weather, particles and the
HUD are missing from its margins. It stays only as a reference-mode option with its test (`port_wide_demos`) until
stage E replaces it; it is not extended, and `--renderer native` refuses it.

Stage E draws the whole wider frame with the native renderer from the same frame: the viewport becomes a parameter
of the draw surface and of each operation's clip rule. Policies to decide explicitly (not by wrapping by default):

| Item | Original rule | Policy needed |
|---|---|---|
| Object sprites | clip box [8,312)x[0,200) or Bloc_lim; active objects already exist ~1320x1200 around the camera | extend the clip box with the viewport; crop of a marker run differs from an uncropped draw (draw uncropped) |
| Particles | engine kills them outside the `_Aff` box (gameplay state) | margins show no particles beyond it, unless the simulation is changed (not allowed) |
| Stars | culled against 320x200 in the pass, consuming random numbers | cull decision must stay; margins without stars |
| Weather | anchors tested against a viewport box; spawn uses a 320x200 inverse projection | margins only where flakes exist |
| Background images | two identical halves for horizontal bands; column backgrounds checked per image | already checked per image (slice 1) |
| Map edges, curtain, firefly, breakout, narrowed viewports | viewport-dependent decisions (RENDERING.md 5, section 4 above) | the original viewport rules apply inside the original rectangle; outside, black or explicit policy |
| HUD | fixed anchors / viewport-relative | anchor policy (keep 4:3 positions or move to edges) |
| Original 4-px left border (`Bloc_lim_W1 = 4`) | cleared every frame | policy: keep or draw the world there |

## 10. Roadmap

| Stage | Content | Status |
|---|---|---|
| A ownership audit | draw path, stateful passes, direct writes, read-backs, viewport decisions; full-frame parity strategy | done for the normal gameplay loop (this document, audit facts in sections 4 and 6); Mode X loop not audited |
| B complete semantic frame | display list of every drawing operation + semantic layers; identity tags; resource generations | operations and layers done for gameplay frames; identity and generations open (section 7) |
| C complete native software renderer | every operation drawn natively, full 320x200 parity | gameplay frames of the demos complete; next: unsupported primitives, nested scenes, non-gameplay scenes (section 6), test scenarios beyond the demos (key scripts, recorded BizHawk replays) |
| D native renderer is the normal output | default `--renderer native`; read-backs from native surfaces; reference path only under verification | open |
| E viewport independence | wider draw surface, same operations, section 9 policies; remove render/compose.c | open |
| F presentation decoupling and interpolation | present recorded frames at the monitor's rate; interpolate tagged positions; snap on teleports, spawns, slot reuse, animation resets, scene changes, camera jumps; never more game updates | open |
| G optional GPU backend | same frame contract | open |

Next concrete step: the remaining primitives of gameplay scenes (deformation for type 254, fond/border boxes for
text boxes, picture copies) with targeted test scenarios, then the start-up logo and the menu scenes, each as
semantic operations captured where the original produces them.
