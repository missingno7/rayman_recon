# Semantic renderer

Goal (user brief 2026-10-26): a native renderer that consumes semantic scene information (tiles, sprites, world
positions, camera, layers, palettes, HUD, effects) instead of the finished 320x200 frame, so the port can later offer
widescreen views of more of the original world, presentation at the monitor's refresh rate, interpolated motion that
does not change simulation speed, in-game display settings, and an Android port. Constraints: `src/` and STRICT
matching are untouched; `DO_MOTEUR`, object activation, the gameplay tick and the RNG are unchanged; the original
pixel path stays the reference; the semantic renderer must reproduce the original image before it is trusted with
anything wider; effects that need special handling are listed, not approximated.

## 1. Where a gameplay frame comes from

`DO_MAIN_LOOP_PC_NORMAL` (src/pcmain.c 478, DOS 1:070e0) runs one iteration per presented frame:

1. present the frame built in the previous iteration: palette work (`fades`, `DoPaletteSpecialPC`,
   `DO_SWAP_PALETTE`), then `Swap_To_Screen` copies the draw buffer to the display;
2. `display_mode + 1` engine updates (`DO_GROS_MOTEUR_NORMAL`: `DO_MOTEUR`, `DO_MOTEUR2`, `get_luciole`,
   `update_display_map`);
3. build the next frame into the draw buffer, in this order (docs/port/RENDERING.md 1):

| Pass | Function | State it reads | Semantic layer |
|---|---|---|---|
| background | `Calcul_Deplacement_Bande` (updates `Bande[].field_6`), `Display_Back_Screen` or a clear | bands, background image, `ymap`, `Val_*_Scroll_Y`, sprites | **implemented** |
| rear weather | `display_flocons_behind` | weather groups | open |
| map | `DRAW_MAP` | `BIG_MAP`, `display_map` (camera) | **implemented** |
| grapple stars | `display_grp_stars` | star list | open |
| objects | `DISPLAY_ALL_OBJECTS` (+ Rayman, fist, cymbal layers, boss bar) | active list, animations, sprites | next slice |
| particles | `display_pix_gerbes` | Q6 particles | open |
| foreground sprites | `Display_Sprite_On_Front` | background sprites on zero-length bands | **implemented** |
| front weather | `display_flocons_before` | weather groups | open |
| firefly lighting | `Display_and_free_luciole` | lit window | special (section 5) |
| border | `clear_borders_Normal` | `Bloc_lim` | special (section 5) |
| HUD / overlays | `DISPLAY_TEXT_FEE` or `DISPLAY_FIXE`, crack/programmer messages, `display_time`, vignettes, options | screen-space state | open |

4. a missed deadline may run one more engine update without drawing (`DO_GROS_MOTEUR_NORMAL(1)`).

So the image swapped at the start of iteration N+1 was built from the state at the end of iteration N's engine
updates, and the state may advance once more before it is shown. A snapshot therefore has to be taken while the
frame is built, not when it is presented.

## 2. Architecture

```
historical units (src/, unchanged) ──DRAW_MAP call──▶ render/scene_capture.c ──port_scene──▶ render/world.c
        │ draw buffer                                   (reads game state only)              (portable renderer)
        ▼                                                                                        │
  Swap_To_Screen ──▶ video/vga.c ──▶ render/compose.c (original image + semantic margins) ──▶ platform/host.c
```

- **Observation point.** The `DRAW_MAP` call in `DO_MAIN_LOOP_PC_NORMAL` is redirected at link time
  (`-Wl,--wrap=DRAW_MAP`, attached to `rayman_hist` in port/CMakeLists.txt) to `__wrap_DRAW_MAP`, which captures the
  snapshot and then calls the unchanged `DRAW_MAP`. No source changes, no derivative. At that point the frame's
  engine updates, camera, band scrolls and background are final. The capture calls no game draw function and
  writes no game variable (`Calcul_Deplacement_Bande`, which advances auto-scrolling bands, is NOT replayed: its
  result `field_6` is read).
- **Snapshot** (`render/scene.h`, plain data, no game headers): camera `xmap/ymap`, the frame's viewport
  (`Bloc_lim`), background mode/image/bands/sprites and the derived vertical offset (`Display_Back_Screen`'s local
  `v68`, recomputed with the original expressions), and a map window of tile records (transparency + texture) 42x14
  tiles around the camera, enough for margins up to 160 pixels. Logical coordinates: world point (wx, wy) is at
  (wx - xmap + 8, wy - ymap), the projection of `update_display_map`/`calc_obj_pos`.
- **Renderer** (`render/world.c`, portable C): background, map and foreground layers into any indexed surface and
  rectangle. Two fidelities: **ORIGINAL** reproduces what the original code draws in the frame's own viewport,
  including its viewport-dependent decisions; **EXTENDED** draws the same world for any rectangle.
- **Presentation viewport** (`render/compose.c`): the display's 320x200 image, unchanged, in the centre of a frame
  `320 + 2 * pad` wide; the margins are drawn with EXTENDED fidelity from the snapshot of the frame being shown
  (bound at `Swap_To_Screen`). Margins stay black unless the display still holds that swapped gameplay frame (menus,
  vignettes written directly to the screen, movies), the viewport is the original full screen, and the background
  can extend (section 5).
- **Host**: the indexed frame is converted with the DAC palette, as before; the window keeps 200 rows at the height of
  a 4:3 picture of 320 columns (pixel aspect 1.2), so `--widescreen` (pad 54, 428 columns) is 16:9.

Options: `--widescreen`, `--wide-pad N` (0..160), `--verify-render`, `--state-log FILE` (game/entry.c).

## 3. Viewport-dependent rules found by the exactness gate

Reproduced in ORIGINAL fidelity; EXTENDED does not apply them to margins.

- `DRAW_MAP` draws nothing when its `display_map` grid is one tile wide or high (src/map_ngawe.c 81). Seen in the
  firefly level, whose lit window starts a few pixels wide.
- Band rows are copied from `W1 - BufferNormalDeplt` (the draw buffer's 1..4-byte alignment shift), `W2 - W1` bytes
  rounded up to 8 (`Copy_Plan0Diff_To_Buf`): the full screen is covered, a narrow viewport's right edge can stay
  unwritten.
- A repeating background sprite is drawn at most twice: at its position (one period earlier when it exceeds 320 or
  200) if it reaches past the viewport start, and one period further if that is before the viewport end.
- The original clips tiles and diff sprites in 4-pixel units; the extra pixels fall outside the clip rectangle, so
  per-pixel clipping inside it is identical.

## 4. Verification

- **Exactness gate** (`--verify-render`, in `__wrap_DRAW_MAP`): every frame, (1) the semantic background (ORIGINAL)
  against the draw buffer before `DRAW_MAP`, on the pixels it writes; (2) the semantic map drawn over the original's
  own pre-map buffer against the buffer after `DRAW_MAP`, on the whole viewport. Rear rain/snow lies between the two
  in weather levels, so their background has no reference there (the map is still checked).
  Result on the six original demos (x86_64): 11,818 map frames and 9,239 background frames, 0 differing pixels;
  backgrounds: horizontal bands (demos 0, 2, 5), vertical columns (demo 1), narrowed firefly viewport (demo 3),
  weather level (demo 4, map only).
- **Presentation independence** (`tests/e2e_wide.py`, ctest `port_wide_demos`): each demo at the original width and
  with `--widescreen`: identical frame-hash logs (original image + DAC), identical gameplay-state logs (camera,
  `map_time`, random table and index, Rayman and every object from `is_active` to the end of `obj_t`), every dumped
  wide frame holds the original frame unchanged in its centre, margins drawn (except demo 3, section 5), and the
  exactness gate clean. The state log also agrees between x86_64 and i686 (demos 0 and 4 compared).
- The state log found a nondeterminism unrelated to rendering: original paths reading uninitialized locals gave
  repeated identical runs different `ray_dist` values; the historical units now start locals at zero
  (DIVERGENCES.md).

## 5. Effects and states that need special handling

| Item | Current handling | Note |
|---|---|---|
| Original left border (`Bloc_lim_W1 = 4`, `clear_borders_Normal`) | kept: a 4-column black strip at the left of the exact centre | policy decision: fill with EXTENDED layers once objects are in the margins |
| Firefly levels (`RayEvts.firefly`: `Bloc_lim` = lit window) | margins black | the original shows nothing outside the lit window either |
| Narrowed viewports (breakout game, world 6 level 4 `InitClipping`) | margins black | not the original full screen |
| Background image wrap for margins | only when the stored image is two identical halves (checked per image) | true for horizontal bands by construction and for the shipped column backgrounds (demos 1 and 4: 640-wide images, halves equal) |
| Curtain levels (world 4 levels 4, 11) | background rows above the curtain are not drawn (original leaves them unwritten) | margins black there; original shows stale buffer content |
| Heat effect (world 5 level 10, one band per row) | covered by the band model | not in the demos: unverified |
| Rear/front weather, grapple stars, particles | not in margins | weather positions are projected screen coordinates; extending needs the weather state's extent |
| Objects, Rayman, fist, cymbals, boss bar | not in margins | next slice; objects off-screen are already active (activation window ~1320x1200 around the camera, `build_active_table`) |
| HUD, fairy text, vignettes, messages, options, pause effect | screen space, centre only | vignettes drawn over gameplay keep the world in the margins |
| Palette: fades, special palettes, cycling, palette-boundary objects | automatic | margins are indexed and use the same DAC |
| Effects that read the frame back (SaveScreen/RestoreScreen, deformation, mosaic, fire, transitions) | margins black | they run outside the swapped gameplay frame |
| Mode X | not presented by the port | |

## 6. Roadmap

1. **Objects** (next): capture the `DISPLAY_ALL_OBJECTS` draw sequence as data: priority buckets 7..1 in active-list
   order with its special cases (docs/port/RENDERING.md 2), per layer sprite, position, flip and colour, with the
   object's array index as its identity. Gate: wrap `DISPLAY_ALL_OBJECTS` and compare like the map. Then particles,
   stars, weather and HUD, until the semantic frame equals the original 320x200 image in every demo frame.
2. **Full semantic frame**: with every layer reproduced, the presenter can draw the whole frame semantically
   (original image still the reference and the gate); the left border and margins become a policy.
3. **Presentation decoupling**: present at the monitor's refresh from the latest snapshot; the game loop, its
   clock and `display_mode` keep their original timing.
4. **Interpolation**: previous/current snapshots with stable identities (object index + allocation generation),
   camera and object positions interpolated, discontinuities (teleports, animation changes, level changes) snap.
5. **Settings and Android**: display options in a port menu; `render/` is portable C without game headers.
