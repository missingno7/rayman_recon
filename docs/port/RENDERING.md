# DOS rendering pipeline and native-port contracts

GOG v1.21, object-relative DOS coordinates. This is a proposal for
`docs/port/RENDERING.md`. The associated semantic owners are
`docs/semantics/affiche.json`, `frame.json`, `display.json`, `types.json`, and
`globals.json`; earlier owners and entity IDs are retained. PROVEN describes the
stated observation, STRONG its interpretation where the C function remains NEAR.
Neither means that a new spelling or a native renderer has been verified against
the entire executable. All source edits in this proposal preserve complete object
contributions; the matching status of each function is unchanged.

The existing `docs/port/ASM_SPEC.md`, `docs/port/TILE_CODE.md`, and
`docs/port/PLATFORM_OPEN.md` remain authoritative for
the original assembly and asset callbacks. Their proofs were referenced, not
reimplemented here. In particular, clipping footprints, transparent runs, VGA
latches, deformation sentinel behavior, and hardware register effects belong to
those contracts. `ASM_SPEC.md`'s old deformation coverage question is resolved by
`PLATFORM_OPEN.md` item 9.

## 1. The prepared frame is presented before the next one is built

The gameplay loops are in `pcmain.c`: normal at **1:070e0**, ModeX at **1:077e0**.
Their original listings, plus `InitData` and the projection/weather/particle
writers, are retained in `evidence/callers_writers.dos.txt`. The C implementation
of `frame.c` includes the pirate lookout routines after the video routines;
`display.c` also contains save menus, world selection, credits, demos, and loader
scenes. File membership alone does not classify something as backend state.

At the start of an ordinary normal-mode iteration:

1. Reset/enable the PIT-based timing service; optionally wait for synchronization
   and sample a deadline.
2. Advance ordinary fade logic; possibly apply the special palette, then process
   active palette-boundary objects.
3. Copy the **already prepared** RAM image to the display target. Copy either the
   viewport or the full HUD-width band according to `fixon`/`fixontemp`; joystick
   polling can be combined with the copy.
4. Read input where the input backend requires it. Run the engine loop. While
   gameplay continues, the loop executes `(u8)display_mode + 1` times.
5. Build the next visible image in the following order.

| Order | Normal-mode pass |
|---:|---|
| 1 | Clear/prepare the firefly HUD state; draw parallax background when enabled, otherwise clear the draw target. |
| 2 | Weather groups behind the map, if enabled. Weather clipping is set separately. |
| 3 | `DRAW_MAP` using `BIG_MAP` and the normal tile atlas. |
| 4 | Grapple-path stars. |
| 5 | `DISPLAY_ALL_OBJECTS`, including the player/cymbal special passes below. |
| 6 | Pixel particles. |
| 7 | Foreground background-sprites. |
| 8 | Foreground weather groups. |
| 9 | Apply/free the firefly lighting result; clear viewport borders. |
| 10 | Fairy dialogue **or** gameplay HUD. |
| 11 | Limited-install/programmer overlays, then bonus time/prompts. |
| 12 | Requested vignette, in-game options, input/idle handling. |

The normal background-disabled clear is 16,008 dwords (64,032 bytes), including
32 bytes beyond the nominal 320x200 image. The native buffer/guard policy should
account for the original footprint rather than interpreting every cleared byte
as a visible pixel. ModeX's direct background clear is 4,000 aperture dwords.

The ModeX iteration swaps/selects its prepared VGA page through `sub_3B580` first,
then handles timing/palettes/input/engine updates. It builds the initial opaque
tile ring when `map_time == 1`. Engine updates maintain the ring and generate
row-run records. The visible passes are background record copy or clear, rear
weather, map ring/partial-tile composition, grapple stars, objects, particles,
front weather, dialogue/HUD, overlays/time, and the final assembly screen-edge
operation. It has its own background-reload policy after menus/vignettes. Do not
add the normal path's separate foreground-sprite/firefly passes to this sequence.

At an elapsed deadline, either gameplay path can run one additional engine update
without drawing another image; it also adjusts the player blink timer as shown in
the original loop. Rendering and simulation therefore do not have a universal
one-to-one relationship.

## 2. Object draw order is an explicit sequence

`DISPLAY_ALL_OBJECTS`, **1:09a2c**, traverses priority buckets **7, 6, 5, 4, 3, 2, 1**.
Within a bucket it retains `actobj.objects` order. Priority is not the active flag,
alive flag, object type, or array ID.

| Point in traversal | Behavior |
|---|---|
| While scanning bucket 1 | Test boss-type flag `flags[type].flags0 & 0x80` and draw its health bar **before** checking bucket equality. |
| Ordinary matching object | Draw its current animation through `display2`, with the exceptions below. |
| Type 157 in world 5, level 8 | Draw only when world x is at least 300. |
| Type 81 | Draw cymbal layers 0..2 instead of the whole animation. |
| Type 254 | Use slope-derived quad deformation with Q12 unity scales. |
| Types 252, 251, 173 | Save the sprite clip box, set its bottom to `Bloc_lim_H2 - 40`, draw, restore. |
| After bucket 3 | Draw Rayman, then the fist, subject to the player visibility predicate and alive/active tests. |
| After bucket 2 | Draw layers 3..5 of each active registered cymbal. |
| After all seven buckets | Draw only the **first** priority-0, type-245 active-list entry. |

Other priority-0 objects and buckets at least 8 have no ordinary draw pass here.
The renderer assumes that active-list construction already chose its members; it
does not retest every ordinary object's activity/aliveness.

Rayman's visibility predicate is:

```c
gele || ray.iframes_timer % (2 * display_mode + 2) <= display_mode ||
    (ray.iframes_timer > 90 && !RayEvts.squashed)
```

Only after that predicate does it require `ray.flags.alive && ray.is_active`.
`DISPLAY_POING` additionally requires `poing.is_active`. This is one reason a
graphics-backend interpretation of `display_mode` is wrong: it changes simulation
update count and the visible blink cadence.

## 3. Coordinate spaces and integer projection

| Quantity/path | Convention |
|---|---|
| `obj.x`, `obj.y` | World-coordinate origins; object geometry functions return world rectangles. |
| `obj.screen_x` | Signed 16-bit `world_x - xmap + 8` from `calc_obj_pos` (1:0e030). |
| `obj.screen_y` | Signed 16-bit `world_y - ymap`. |
| Animation layer offsets | Added to the object's stored screen origin before projection. |
| Sprite draw arguments | Integer screen coordinates; dimension packing narrows to 16 bits, width in CL and height in CH. |
| Animated world-map name positions | Signed Q4 screen positions; arithmetic `>> 4` precedes drawing. Y is a baseline. |
| Pixel-particle positions/speeds | Signed Q6 screen pixels / Q6 pixels per engine update. Draw with arithmetic `>> 6`. |
| Weather positions | Signed integer coordinates **before** perspective projection. They are neither world coordinates nor Q6 particle positions. |
| Text x/y | Screen x anchor and baseline y; top = baseline - glyph outer height. |
| Deformation angle/scales | 512 units per turn; signed Q12 scales, 4096 = unity. |

Packed dimensions are `(projected_height << 8) + projected_width`, followed by
u16 narrowing. The addition is not a pair of independent saturating byte casts.

The +8 bias is a property of specific conversions, not a screen-wide translation.
The star callback draws at `world - camera + star.dist` with **no +8**. World-path
`DISPLAY_PTS_TO` adds `(-xmap + 52, -ymap + 35)`; `DISPLAY_PTS_TO_PLAN2` instead
adds +8 only to its x endpoints. `DISPLAY_POINT` subtracts another 2 from y when
stamping into PLAN2. UI/HUD anchors are already in screen coordinates.

For ordinary projected coordinates, `get_proj_x/y` (1:0d5d8 / 1:0d614) compute:

```text
m = trunc_toward_zero(65536 / (signed16(scale) + 256))
result = center + trunc_toward_zero(m * (signed16(coordinate) - center) / 256)
```

The returned coordinate is signed 16-bit. Scale 0 gives unity; 256 gives one-half.
Scale is a depth parameter, not a Q8 percentage. `get_proj_dist` (1:0d570) returns
the input distance unchanged when `zoom_mode == 0`; otherwise it uses the related
integer distance formula. `zoom_mode` remains owned by the earlier basic registry.

Weather rendering deliberately uses a different lowering:

```text
m = 65536 / (depth + 256)
offset = center - arithmetic_shift_right(m * center, 8)
result = arithmetic_shift_right(m * unprojected_coordinate, 8) + offset
```

It temporarily selects center **(160, 170)**, then restores both center globals.
The independent shifts and the ordinary helper's signed division can differ by a
pixel. For example, center 160, depth 32, coordinate 1 gives weather x = 19 and
ordinary projected x = 20. Do not replace both paths with one floating transform.

Rear weather: groups 3..7, depths 32, 64, 96, 128, 160. Front weather: groups 0..2,
depths -64, -32, 0. Jungle chooses rain stamps; other worlds choose snow stamps.
The renderer derives depth from the group, rather than reading every record's
stored `field_4`. The two proposed field names `unprojected_x/y` are editorial
spellings for quantities established by both `neige.c` writers and these readers.

## 4. Placement reflection and pixel mirroring are separate

`display2`, **1:08ce8**, visits current-frame layers in ascending index order. It
skips sprite-index 0 and sprites whose `id` byte is 0. Without object flip,
`x = layer.x + obj.screen_x`. With object flip:

```text
x = 2 * obj.offset_bx - layer.x - sprite.outer_width + obj.screen_x
```

There is an additional -16 at scale 256, layer 5, animation indices 14..16. Y is
`layer.y + obj.screen_y`. The layer's mirror bit affects pixel orientation, not
this placement formula.

| Entry | Boolean condition | Dispatch selection |
|---|---|---|
| `display2` | `obj.flags.flip_x XOR layer.mirrored` is 0 | Normal slot |
| `display2` | Same expression is 1 | Flip slot |
| `display_sprite` / `display_sprite_NoClip` | `sprite.flags_bits.flip_x XOR flip` is nonzero | Normal slot |
| `display_sprite` / `display_sprite_NoClip` | Same expression is zero | Flip slot |

For the no-clip variants use the corresponding no-clip slots. Their ModeX entry
assignments have counterintuitive assembly names: `FlipNoClipX` is the forward
routine, `NoClipX` mirrors. The actual assignments are correct; the native backend
must follow the behavior, not derive orientation by searching names for “Flip”.

The C wrapper passes the sprite color high nibble. Ordinary normal-mode sprite
drawers ignore that compatibility argument; color drawers OR visible pixels with
`(color_argument * 8) & 255`. Transparent-run/plane behavior differs by backend and
is already specified in `ASM_SPEC.md`. The loader's temporary 256-color drawers
use zero transparency, unlike the usual marker-run sprite decoding.

Deformed-object rendering constructs layer rectangles without applying
`display2`'s facing reflection or mirror selection. `CALCUL_DEFORMATION`, 1:0c030,
first scales each coordinate with a signed product `>> 12` and narrows to signed
16-bit; it then rotates with sine/cosine scaled by 512, shifts `>> 9`, and narrows
again. Preserve both narrowing stages. A single sprite's pivot uses the inner
content center, including the y inset's extra +1. An object's pivot instead uses
its follow-sprite or animation rectangle after camera conversion. The fourth quad
corner is the vector sum of the other three. Rasterization/scratch fringe and the
0xA1 versus 0x81 sentinel quirk remain assembly contracts.

## 5. Clipping state and viewport bounds are different objects

The sprite clip box is mutable assembly state (`XMIN`, `XMAX`, `YMIN`, `YMAX`).
`sprite_clipping` (1:061fa) clamps the four signed inputs independently to
0..320 / 0..200; it does not sort inverted bounds. The sprite clip helpers use
half-open bounds and preserve their original packed-dimension/source-cropping
rules. Some specialized blitters have rounded fringes; refer to their individual
assembly entries rather than imposing a universal pixel-intersection rule.

`Bloc_lim_W1/W2/H1/H2` are the gameplay viewport boundaries used by background,
HUD, menu, and presentation code. Menu restoration explicitly copies columns
`[W1, W2)` and rows `[H1, H2)`. They are not aliases of the current sprite clip box.
The `_Aff` snapshot is used separately for particle culling. Weather clipping is
also a separate assembly contract; normal gameplay supplies bottom `H2 - 6` and
right `W2 - 4` before the rear weather pass.

Scoped changes that the port must retain:

- HUD life/power popups save the box, use (0, 100, 0, 100), then restore it.
- Types 252/251/173 temporarily use bottom `H2 - 40`, then restore the old box.
- `DISPLAY_GAME_VIGNET` saves the caller's clip box, uses full 320x200, and restores
  it after any video-mode round trip.
- `DISPLAY_STAGE_NAMES` sets full-screen clipping and leaves it set.
- UI black boxes independently clip rectangles to 320x200. Byte fill selector
  255 fills; 254 and other values do not. This parameter's old `font_size` spelling
  did not describe a font selection.

## 6. Tile map and scroll origins

ModeX opaque tiles use a **320x224** four-plane ring buffer: 20x14 tiles, tile size
16x16, row pitch 80 aperture bytes, plane size **0x4600 = 80 * 224**. A tile's source
plane occupies 64 bytes. Initial `build_map` copies only valid opaque tiles and
clears other cells. Incremental row/column builders skip nonopaque cells without
clearing them. `update_map` callers pass **current camera first, old camera
second**. It aligns both to 16, rotates row/byte-column offsets, applies a single
80/224 wrap correction, and refreshes the original edge/corner cases. It is not a
general arbitrary-distance camera-jump renderer.

`calc_gros_type` classifies pixels from the 40-column, 640-pixel-wide tile atlas:
0 = opaque, 1 = wholly zero, 2 = mixed. Texture 0 is forced transparent before the
original out-of-range ID clamp. `smap` is this 1200-entry **atlas** lookup, not the
level collision map. Map `tile_type` at +2 and rendering transparency at +4 are
distinct fields.

`InitData`, **1:06f80**, writes the 20-byte shared setup record. Let `rx = x & 15`,
`ry = y & 15`, `L = plan0bit_length`, `S = plan0bit_nb_bytes`:

```text
draw_origin   = draw_buffer - rx/4 - 80*ry + 1
scroll_x      = type_fndscrX ? 0 : origin_x + 16 - rx
scroll_y      = origin_y + 16 - ry
source_origin = PLAN0BIT + scroll_x/4 + (scroll_x%4)*(L>>2) + scroll_y*S
initial_mask  = 1 << (3 - (rx & 3))
```

Both origin fields include their buffer base: they are absolute DOS addresses
stored in signed 32-bit words, **not relative byte offsets**. `planar_copy_offset`
is the initial plane mask despite its old name. The native representation can use
owned surfaces and explicit phase/offset values, while preserving these
calculations and wrap comparisons.

With an enabled scrolling background, `DO_GROS_MOTEUR_X` supplies background
origin x `(xmap / 3) % (u16)plan0_width` and the separately maintained/wrapped
`yp0`. Otherwise it supplies (0,0). Camera x/y passed for the tile phase remain
`xmap/ymap`; background coordinates and map coordinates are not interchangeable.

`sub_3C3BC` builds background and opaque-map run lists. `sub_3BE20` consumes
background records; `sub_3BEE0` copies the opaque ring and paints partial tiles.
`P486` selects alternate cached/latch paths. The 128-byte record-row grammars and
32-byte callback context are already in `ASM_SPEC.md`. In the C context,
words +0/+4/+8 are overwritten with different dimensions/meanings between phases;
do not model them as three immutable general-purpose settings.

The asset callbacks are the constant painters proved in `TILE_CODE.md`; native
code draws the corresponding bitmap tiles and never executes the shipped x86.

`DISPLAY_ANYSIZE_PICTURE`, **1:2cbe8**, is a separate planar image wrapper. Its old
third parameter `source_width` is a **source row offset**, multiplied by source
stride. The proposal names it `source_y`. Source x's low two bits choose a plane;
the remaining x is divided by 4. Destination is `(x >> 2) + 80*y`. Width 0 and
height 0 have asymmetric original defaults: the wrapper divides source length by
4, computes default height with the pre-divided default width, subtracts 4, then
divides a defaulted width by 4. Preserve those steps, not a conventional crop-rect
interpretation inferred from the old parameter names.

## 7. Palette state and exact fades

There are separate objects for level palettes `rvb[3]`, selected palette ID,
current output bytes `current_rvb`, and **768 signed 16-bit Q6 accumulators**
`rvb_fade`. Raw LEV data contains RGB6 components 0..63; PCX-derived palettes have
their separate loader conversion documented by pass 4. A palette entry is an RGB
triple, not four-byte RGBA.

| Operation | Exact state transition |
|---|---|
| `set_fade_palette` | Every accumulator = source component `<< 6`. |
| `start_fade_in(speed, ignored_palette)` | `nb_fade = 1 << (6-speed)`, `fade = 1`, remember speed; clear current bytes and all accumulators; output black for all 256 entries. |
| `start_fade_out(speed, ignored_palette)` | Same count/speed, `fade = 2`; preserve existing accumulators. |
| `do_fade` with count 0 | Return without output or completion-bit change. |
| `do_fade` with count nonzero | Decrement first. Exact state 1 adds `source_byte << speed`; exact state 2 subtracts it only where signed accumulator > 0. No saturation. |
| Conversion | `dest_byte = (u8)(signed_accumulator >> 6)` for all 768 channels. |
| Hardware output | Emit **current_rvb**, even though the destination parameter could identify another palette. |
| Final update | `fade |= 0x40`, producing completed-in 65 or completed-out 66. |

The standard wrappers use speed **2**, giving **16** updates. For an unchanged
component C and correctly initialized state, fade-in after update k is
`floor(C*k/16)`; fade-out is `floor(C*(16-k)/16)`, k = 1..16. These formulae are a
consequence of the integer accumulator algorithm in that domain, not replacements
for its state machine when a palette changes mid-fade. Completed values 65/66 do
not take the exact-1/exact-2 add/subtract branches.

`actualize_palette` copies a newly selected level palette, seeds accumulators, and
outputs immediately only when `fade == 65`. Palette cycling rotates an inclusive
RGB interval left, seeds the accumulators, and requests one output update when
idle. The palette-boundary loop is enabled by nonzero `last_plan1_palette` and
completion bit 0x40. Its six ordered pairs are:

| Substate modulo 6 | Ray coordinate below boundary | Ray coordinate above boundary |
|---:|---:|---:|
| 0 | 0 | 1 |
| 1 | 0 | 2 |
| 2 | 1 | 2 |
| 3 | 1 | 0 |
| 4 | 2 | 0 |
| 5 | 2 | 1 |

Substates 0..5 compare `x + offset_bx`; 6..11 compare `y + offset_by`. Equality
retains the current palette. All type-158 objects are processed in active-list
order, so multiple boundaries can affect one iteration. Existing variable names
“left/right/above/below” are not the spatial truth table.

`SAVE_PALETTE` saves the selected ID and old palette 0, installs a scene palette in
`rvb[0]`/`current_rvb`, and selects 0. It does not seed accumulators or output by
itself. `RESTORE_PALETTE` restores the ID and palette-0 bytes, without restoring
the current bytes, accumulator, or hardware output. This is one save slot, not a
nested palette stack. `SAVE_PLAN3`/`RESTORE_PLAN3` likewise save one image view
(pointer, byte length, stride), without copying or transferring image ownership.

## 8. PC special palettes

`InitPaletteSpecialPC` is at **1:2cf70**; interpolation at **1:2d54c**; effect
output/advancement at **1:2d704**. All divisions below are integer divisions.

For lightning levels **(world, level) = (1,9), (2,4), (4,9)**, let C be each
original RGB6 component. Original palette is saved in slot 15. Every entry
0..255 is assigned at these anchors, clamping each component to at most 63:

| Special slot | R | G | B |
|---:|---|---|---|
| 0 and 14 | `4*R/8` | `4*G/8` | `4*B/5` |
| 5 | `4*R/3` | `4*G/3` | `4*B/3` |
| 7 | `4*R/6` | `4*G/6` | `B` |
| 10 | `2*R` | `2*G` | `2*B` |

Copy slot 0 into `rvb[0]`, then interpolate 0→5→7→10→14. Initial delay is
`myRand(200)+1` effect calls. The output routine increments `CompteurEclair`
before testing it against the delay. During a sequence it outputs the current
phase every invocation; phase 0 plays sound 195 and advances immediately, later
phases advance only on even counters. At phase 15 reset counter and phase to 0
and select `myRand(400)+1` for the next delay. Main gameplay invokes this only
under its death/freeze gating, so these are not raw map ticks or milliseconds.

For **(5,4)**, initialize only entries **0..254** in saved-original slot 15 and
anchors 0/14. The tinted anchor is `(R, 77*G/100, 12*B/100)`, component-clamped to
63, and becomes `rvb[0]`. Interpolate 0→14, then overwrite slot 9 entries
**128..174 inclusive** with `(84*R0/100, 62*G0/100, 12*B0/100)` using the
**already tinted** `rvb[0]`. Re-interpolate 0→9→14. Do not combine the two tint
stages, or initialize the omitted entry 255 as a newly inferred requirement.

For every interpolated slot p strictly between anchors a/b, every RGB component
uses:

```text
weighted = ((b-p)*component[a] + (p-a)*component[b]) / (b-a)
component[p] = min(63, (u8)weighted)
```

The byte narrowing precedes the clamp. Endpoints are not changed by this helper.
The cave animation emits/advances on `horloge[5] == 0`, wraps 15→0, and on a
nonzero clock changes phase 0 to 1 without output. Preserve that exact zero-phase
exception.

Both special effects require `fade & 0x40`. Their hardware output starts at
palette index **1** and streams **765** bytes, leaving DAC color 0 untouched.
Slot 15 is a saved anchor, not part of the 0..14 output cycle.

## 9. Text, HUD, particles, and scene layout

The raw font input is an 8-bit glyph stream, not a Unicode string. Slash centers
the next line around x using `calc_largmax_text`, and advances its baseline only
when its input index exceeds 1. Space advances without drawing. Numeric `]ddd`
escapes select `alpha_numbers` at glyph-1000 and consume three additional bytes.

| Font selector | Resource | Line height | Character spacing | Space advance |
|---:|---|---:|---:|---:|
| 0 | `alpha2`, original glyph index | 36 | 3 | 12 |
| 1 | `alpha2`, index + 41 | 23 | 3 | 10 |
| 2 | `alpha` | 15 | 1 | 8 |

Draw y is baseline minus outer height. Advance x by packed inner x inset plus
inner width minus spacing. Before testing the numeric-glyph threshold, the
deformed and sine variants call `calc_num_let_spr` on the **low byte** of the
32-bit glyph number. Preserve that helper's font adjustment and operation order,
including for numeric codes; do not substitute the plain-text branch ordering.
`display_deform_text` ignores its color formal: its quad sprite path uses sprite
color. `display_text_sin` applies
`ashr16(cosinus((s16)(phase + input_index*32)), 5)` to y; the input index includes
escape consumption. `display_box_text` expands bounds by three pixels per side;
`field_D5` controls the observed fill/text-offset behavior. Its broader intended
label stays unresolved and is not renamed.

The normal HUD branch is selected by **time == -2**. Its main sprite anchors are
life (16,5), tings (244,5), life digits x 55/70, ting digits x 276/290, and HP
(35,27), with the second HP portion at (59,27) when `max_hitp == 4`. ModeX with
`P486 == 1` adds **16 y pixels**. HP 255 or `fin_du_jeu` suppresses the HP draw.
Other time values use the bottom-left tings subset at `W1+14/46/60`, `H2-32`.
Temporary HUD refresh is 300 `DISPLAY_FIXE` invocations; its last normal-mode
tick clears border areas in both buffers. Boss health geometry uses six pixels
per lost HP and a 29-HP reference; no new maximum-HP clamp is inferred.

Bonus time display divides ticks by 60/600/6000 and draws decimal digits. Time
**-2** suppresses the whole display. During `map_time <= 100`, the tings prompt
disables Rayman's activity and animates a color selector through 1..5 while
`horloge[8] < 4`; its u8 direction reverses between +1 and 255. During
`120 < map_time <= 160`, show “go”. The countdown's 60-tick convention does not
prove a universal physical 60-Hz refresh.

Particle motion is Q6. `start_pix_gerbe` initializes x/y multiplied by 64;
`do_pix_gerbes` adds speed minus camera-scroll*64, then adds the unsigned
acceleration byte to signed 16-bit vertical speed. Initial acceleration is
`myRand(8)+8`, in Q6 pixels/update². The renderer tests particle byte +9 >= 128;
its low seven bits add to a color bias. Brightness uses the **unsigned low byte**
of vertical speed, not absolute signed-16 speed; values at least 128 select
palette index 88. Existing neutral member `unk_1` is retained despite this proven
bit-use observation.

Save rows use pitch `ecarty + 23` and baseline `debut_options + pitch*slot`.
Summary sprites show lives, one continue digit, and `floor(cages*100/102)` as
three percentage digits. Save names use one-based character selection and a
four-byte prefix buffer. Repeated `AFFICHE_ECRAN_SAVE` calls advance repeat/blink
and deformation phase each time; they are not necessarily one per presented
frame. Continue rendering distinguishes `loop_timing == 255` (game-over text)
from the signed -1 termination tested by `continue_fonction`.

## 10. Scene presentation, modes, and clocks

`SYNCHRO_LOOP`, **1:2cb54**, presents before calling the scene callback. ModeX
order is end-retrace → swap → retrace; normal order is end-retrace → retrace →
copy/swap. The first iteration skips `DO_FADE`, later iterations call it before
the scene callback. Clock sampling happens when `need_timer` is nonzero; the
callback gets 0 otherwise. Callback clocks are not animation deltas.

`speaker_enable` resets the assembly clock state on each iteration. `clock_ticks`
is a PIT2-derived sample with wrap/stall handling; it is not `map_time` or a
monotonic application timestamp across these resets. `wait_until` waits until a
signed sample is **greater than** its target and returns poll-count minus one in
ECX; disabled timing returns 1. A zero return means the first sample was already
beyond the target. This is the condition under which world selection and gameplay
can run an extra update. The native clock implementation belongs at the platform
boundary; preserve the game's decision inputs and scheduling behavior. The
original clock/register contracts are already specified in `ASM_SPEC.md`.

`word_CF7EE` is a calibrated **clock_ticks interval** used by WORLD_CHOICE. Its
writer uses `word_CF7F0 * 31 / 32`. It is not a millisecond or an audio-playback
flag. `need_timer` is separate from sound permission despite the historical
calibration routine name `InitPcSoundCard`.

Vignette pages count **60 callback invocations** per decrement of `loop_timing`,
allow button dismissal once timing <= 3, and auto-advance at <= 0. A following
pair resets timing to 5; `new_txt_fee == 10` is the terminal page marker.
Inter-animation and victory callbacks draw Rayman **before** advancing animation.

`ModeVideoActuel` is active state: 0 normal, 1 ModeX, 255 text.
`GameModeVideo` is the configured gameplay choice (0/1 once initialized; the
initial image sentinel is 255). `VGA_FREQ` is the last requested timing selection;
`Frequence` is a separate saved/command-line policy byte, including 0x46 for the
FREQ70 path. Re-requesting an already active mode does not reprogram frequency.
The VGA setter has branches for 50/60/70/80/100 and waits 500 ms afterward; this
does not certify the historical monitor's measured physical frequency.

Normal `SWAP_BUFFERS` copies to the display target and resets aliases; it does
not exchange two RAM pointers. ModeX alternates page apertures 0xa0500/0xa6500 and
selects the prepared display page. In-game vignettes save clipping/palette/PLAN3,
temporarily enter normal mode if necessary, run the scene, restore ModeX and the
saved views, then start another fade. Loader animation separately installs the
zero-transparent 256-color sprite dispatch routines and restores ordinary
normal-mode routines afterward.

## 11. State the native backend replaces versus state the game retains

Every directly used C global has a `platform_class` in its existing semantic
owner; `evidence/platform_classes.json` is the complete pass-facing index.

| Class | Representative state | Native responsibility |
|---|---|---|
| Backend representation | Draw/display pointers, VGA page addresses, register snapshots, draw dispatch slots, row-run records, plane mask and address-valued origins | Replace with surfaces/device state while preserving specified visible/copy/clip behavior. |
| Platform policy | Configured mode, refresh policy, synchronization, timer enable, input backend choice | Translate explicitly; do not silently use these flags as gameplay coordinates or audio state. |
| Timing policy | `display_mode` | Preserve its effects on engine-call count, blinking and fade policy; it cannot be dropped with VGA setup. |
| Presentation state | Palette bytes/Q6 accumulators, fade flags/count, special-palette phase, viewport, weather/particle/star state, HUD refresh | Retain as observable game state even if the implementation uses an SDL surface/GPU. |
| Resource state | Font banks, image views/lengths/strides, tile resources, owning memory-arena lifetimes | Use native owned resources; keep selectors, formats, shared mutations, and lifetimes. |
| Scene/input/game state | Menus, save names/progress, demo playback, actor lists/states, input-release latches, callback choice, exit requests | Preserve in the game/scene layer. File membership in display.c does not make it disposable. |

The unused BIOS/VGA copy benchmark and its uninitialized flag pointer are already
an omit decision in `PLATFORM_OPEN.md`; no gameplay pacing depends on its return.
`let_shadow` has located stores but no located value-consuming reader in the
audited direct-reference/source scan; a shadow-rendering meaning is **UNKNOWN**.
Original declaration capacities and unreachable-data questions were not pursued.

The lookout functions retained in `frame.json` are game behavior. Type 78's
horizontal speed uses Q4 displacement (immutable flags byte 1 has bit 0x10), while
its vertical speed uses ordinary pixels/update (bit 0x20 is clear). The spawning
routine writes gravity slots (0,10) for substates 2/18 and (0,0) for 5/13;
their general stepping semantics remain in the moteur registry. Its fuse
decrements on nonzero `horloge[2]`; a command invocation entered with timer 0
explodes. Undefined original paths in held-bomb detection/spawning are recorded,
not assigned newly invented default results.

See `render_UNCERTAIN.md` for the complete remaining UNKNOWN/HYPOTHESIS list and
explicit domain boundaries. Shipped-data counts in the registries describe
initial samples, not limits on runtime assignments.
