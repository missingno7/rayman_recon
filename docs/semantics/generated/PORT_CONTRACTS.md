# Port-facing contracts

Generated from all selected semantic registries. UNKNOWN and HYPOTHESIS are observations, not implementation requirements.

## basic.json

Meaning confidence is separate from compiler layout and STRICT status. UNKNOWN retains neutral names. Raw source/displacement/external token hits are candidates, not type proofs.

### function:EOA

PROVEN: Report the terminal animation frame only on a zero phase of its selected animation clock.

Units: see parameters/result contract. DOS layout/address: 1:0d074.

Reverse flag 0x10 makes frame 0 terminal; otherwise frames_count-1. This is a timed event, not a persistent last-frame predicate.

Moteur clarification: low timing nibble0 freezes DO_ANIM advancement, but horloge[0] remains0 and EOA still accepts a terminal frame on every call. Period1 also has zero phase on every update. Clock-gated does not imply a one-shot event for these cases.

### function:save_objects_flags

PROVEN: Pack alive and is_active into two bits per level object.

Units: see parameters/result contract. DOS layout/address: 1:0d0ec.

16 objects per u32: entry i uses word i>>4 and shift2*(15-(i&15)); low bit within the pair is alive, high bit active. Entry pairs run from most-significant to least-significant bits.

### function:restore_objects_flags

PROVEN: Restore packed alive and active bits for every level object except type 58.

Units: see parameters/result contract. DOS layout/address: 1:0d15c.

Cage flags are deliberately left alone; do not apply the snapshot blindly to all objects.

### function:snapToSprite

PROVEN: Compute follower speeds towards a selected sprite of the other object at its predicted next animation frame.

Units: see parameters/result contract. DOS layout/address: 1:0d204.

Temporarily replaces then restores the target animation fields; moves follower coordinates in signed 16-pixel chunks; speed conversion is per axis and object type.

### function:findfirstObject

PROVEN: Return the first object of the requested type, or the pointer just past the level object array.

Units: see parameters/result contract. DOS layout/address: 1:0d4c4.

No NULL failure result; caller must distinguish one-past-end before dereferencing.

### function:findfirstInactiveObject

PROVEN: Return the first inactive object of the requested type, or the pointer just past the level object array.

Units: see parameters/result contract. DOS layout/address: 1:0d4f4.

Inactive means is_active==0, independent of alive.

### function:sinYspeed

PROVEN: Advance a 12-bit phase and return the sine target displacement minus current displacement from init_y.

Units: see parameters/result contract. DOS layout/address: 1:0d528.

phase=(phase+step)&4095; sinus(phase>>3)*amplitude arithmetic-shifted by 9, then subtract signed-16 y-init_y; return narrows to s16. Preserve the integer costab: its129 stored quarter-wave entries at2:06fe0 range from1 to512, including quantised endpoints.

### function:set_proj_center

PROVEN: Store the signed-16 projection centre coordinates.

Units: see parameters/result contract. DOS layout/address: 1:0d560.

Global mutable centre; GET_RAY_ZDC restores a screen-space centre after its world-space calculation.

### function:get_proj_dist

PROVEN: Return a dimension unchanged with zoom disabled, otherwise apply the integer projection factor.

Units: see parameters/result contract. DOS layout/address: 1:0d570.

factor=65536/(signed_scale+256), then factor*dimension/256 with division toward zero; invalid denominator is not guarded.

### function:get_proj_dist2

PROVEN: Scale a signed dimension by the integer projection factor without consulting zoom_mode.

Units: see parameters/result contract. DOS layout/address: 1:0d5b0.

Arithmetic >>8 differs from /256 for negative products; preserve both helpers.

### function:get_proj_x

PROVEN: Project a signed-16 horizontal coordinate about PROJ_CENTER_X.

Units: see parameters/result contract. DOS layout/address: 1:0d5d8.

Two integer divisions: first 65536/(scale+256), then product/256 toward zero; result narrows to s16.

### function:get_proj_y

PROVEN: Project a signed-16 vertical coordinate about PROJ_CENTER_Y.

Units: see parameters/result contract. DOS layout/address: 1:0d614.

Same rounding and narrowing as get_proj_x.

### function:set_zoom_mode

PROVEN: Store the byte controlling the optional projection branch of get_proj_dist.

Units: see parameters/result contract. DOS layout/address: 1:0d650.

Consumers test zero versus nonzero; this helper does not validate the byte.

### function:inverse_proj_x

PROVEN: Map a horizontal coordinate back through the quantised projection factor.

Units: see parameters/result contract. DOS layout/address: 1:0d658.

Inverse uses the already-truncated divisor; it need not undo forward projection exactly. DOS left shift has two-complement machine behaviour.

### function:inverse_proj_y

PROVEN: Map a vertical coordinate back through the quantised projection factor.

Units: see parameters/result contract. DOS layout/address: 1:0d690.

Same quantised divisor, signed division and narrowing as inverse_proj_x.

### function:vblToEOA

PROVEN: Compute a positive, signed-16-truncated duration from the state animation period, frame count, clock phase and multiplier.

Units: see parameters/result contract. DOS layout/address: 1:0d6c8.

max(1, s16(s16(period*frames_count)*multiplier-clock+1)); ignores current anim_frame, so its name must not imply an exact remaining-frame count.

### function:GET_ANIM_POS

PROVEN: Write the current animation-frame rectangle in world coordinates.

Units: see parameters/result contract. DOS layout/address: 1:0d738.

Horizontal flip reflects around x+offset_bx, using frame width; output pointers are s16.

### function:add_actobj

PROVEN: Append an object id to the active-object id array and write a following -1 sentinel.

Units: see parameters/result contract. DOS layout/address: 1:0d7d8.

No capacity check; count and ids are signed 16-bit; no duplicate suppression.

### function:set_sub_etat

PROVEN: Set the object substate and request animation reset if the selected animation changes.

Units: see parameters/result contract. DOS layout/address: 1:0d808.

Writes change_anim_mode=1 even if the numeric substate is unchanged.

### function:set_main_etat

PROVEN: Set the object main state and request animation reset if the selected animation changes.

Units: see parameters/result contract. DOS layout/address: 1:0d810.

State indices are bytes; preserve per-TU prototypes rather than consolidating ABI declarations.

### function:set_main_and_sub_etat

PROVEN: Set both state indices and request animation reset if the selected animation changes.

Units: see parameters/result contract. DOS layout/address: 1:0d818.

Mode byte is set to 1 before the state stores.

### function:get_center_x

PROVEN: Choose a type-dependent horizontal offset for the turn-marker block probe.

Units: see parameters/result contract. DOS layout/address: 1:0d824.

Not a general sprite centre: table cases include world and follow_sprite-dependent constants.

### function:get_center_y

PROVEN: Choose a type-dependent vertical offset for the turn-marker block probe.

Units: see parameters/result contract. DOS layout/address: 1:0d954.

Blacktoon follow_sprite outside 5..7 leaves the DOS return unassigned; do not silently invent a fallback.

### function:on_block_chdir

PROVEN: Test bit 5 of the block flags at the object position plus supplied probe offsets.

Units: see parameters/result contract. DOS layout/address: 1:0dac8.

Coordinates use arithmetic >>4, unlike calc_btyp_square negative-coordinate division.

### function:CALC_FOLLOW_SPRITE_SPEED

PROVEN: Store current-minus-next selected animation-layer offsets into follow_x and follow_y.

Units: see parameters/result contract. DOS layout/address: 1:0db04.

Stride is layers_per_frame&0x3fff; mirrored x uses the current sprite outer width for both frames.

### function:GET_SPRITE_POS

PROVEN: Write the inner sprite rectangle for a layer of the current object animation when its sprite index and reference id are nonzero.

Units: see parameters/result contract. DOS layout/address: 1:0dbb4.

Layer index is not a sprite-table index. Failure returns 0 and leaves outputs unchanged. X reflection uses object flip XOR sprite flag bit 2, not layer.mirrored.

### function:GET_RAY_ZDC

PROVEN: Write a state-dependent Rayman collision rectangle and optionally project it around the object anchor.

Units: see parameters/result contract. DOS layout/address: 1:0dcfc.

Special eta flag0x40 and states5/7 choose rectangles; nonzero scale applies projection regardless of zoom_mode and changes the global centre twice.

### function:GET_BB1_ZDCs

PROVEN: Derive two BB1 collision rectangles from selected sprite bounds and current animation height.

Units: see parameters/result contract. DOS layout/address: 1:0dec0.

Includes state-dependent layer selection, asymmetric margins, and a later layer-0 override.

### function:calc_obj_dir

PROVEN: Set object flip_x from the sign of Rayman anchor x minus object anchor x.

Units: see parameters/result contract. DOS layout/address: 1:0dfb8.

Equal anchors clear flip_x; it is not equivalent to retaining the previous facing.

### function:OBJ_IN_ZONE

PROVEN: Test the object detection-zone result with type-specific acceptance of values above one.

Units: see parameters/result contract. DOS layout/address: 1:0dff4.

Types0x9/0xa5/3/0x90 require exactly1; all other types accept >=1.

### function:calc_obj_pos

PROVEN: Convert object world coordinates to stored screen coordinates using the camera offsets.

Units: see parameters/result contract. DOS layout/address: 1:0e030.

screen_x=x-xmap+8, screen_y=y-ymap, both narrowed to s16; preserve the +8 bias.

### function:makeUturn

PROVEN: Apply type- and state-specific facing/script changes when an object turns.

Units: see parameters/result contract. DOS layout/address: 1:0e05c.

Label255 means no script jump; when a label is selected, undo this tick x movement, clear speed_x, then skipToLabel. MITE2 deliberately falls through to MITE.

### function:BTYP

PROVEN: Return the collision-type byte of an in-bounds tile, or zero outside the map.

Units: see parameters/result contract. DOS layout/address: 1:0e588.

Signed-16 input coordinates, inclusive range tests; DOS returns full zero-extended s32 in this TU.

### function:calc_btyp_square

PROVEN: Sample centre, left, right, above and below block types around an object anchor.

Units: see parameters/result contract. DOS layout/address: 1:0e608.

Tile size16; ashr16 divides negative positions toward zero. Side distance16 normally,8 for Rayman,4 for tiny Rayman. Airborne centre passes through bloc_floor.

### function:DO_OBJ_REBOND_EN_X

PROVEN: Negate horizontal speed when either side block has flag bit 4.

Units: see parameters/result contract. DOS layout/address: 1:0e76c.

No facing or state update; signed-16 speed storage retains the DOS narrowing behaviour.

### function:calc_btyp

PROVEN: Select the effective supporting block type and perform type-specific side-edge turning and Rayman edge bookkeeping.

Units: see parameters/result contract. DOS layout/address: 1:0e790.

Rayman param uses -1/0/1 here; ray_last_ground_btyp retains its previous value for unhandled types including25; Rayman ray_dist is a tile index in this path.

### function:init_obj_in_the_air

PROVEN: Clear the two byte gravity accumulators.

Units: see parameters/result contract. DOS layout/address: 1:0eaa8.

Does not itself set an airborne state or a vertical speed.

### function:make_my_fruit_go_down

PROVEN: Accelerate horizontal speed below magnitude32 and select the type-dependent falling state.

Units: see parameters/result contract. DOS layout/address: 1:0eab4.

Acceleration is conditional on state2; gravity bytes reset only when changing to the desired main/substate.

### function:switchOff

PROVEN: On the timed animation-end event, clear object alive/active flags and remove its always-object entry with exceptions.

Units: see parameters/result contract. DOS layout/address: 1:0eb18.

Breakout WIZ stays in level_obj; types170/75 are moved to -32000 on both axes.

### function:obj_hurt

PROVEN: Subtract current fist damage from target hit points with saturation at zero.

Units: see parameters/result contract. DOS layout/address: 1:0eb80.

Both damage and HP are unsigned bytes; equality also produces zero.

### function:Projectil_to_RM

PROVEN: Compute facing-constrained projectile speeds towards the stored follow_x/y target, then advance that stored target.

Units: see parameters/result contract. DOS layout/address: 1:0eba0.

Target comes from follow_x/y, not a fresh ray.x/y read. Horizontal-only branch initially chooses the opposite x sign; later facing correction may invert it. Increment target by twice the quotient/sign pair; preserve +16 and +2 special cases.

### function:del_actobj

PROVEN: Clear object activity, find its id in actobj, shift entries using the original j-1 destination, and decrement the count.

Units: see parameters/result contract. DOS layout/address: 1:0ee18.

The loop starts at i and writes objects[j-1], including i-1: unusual DOS behaviour. Keep this specified before designing a safe native container; no absent-id guard.

### function:calc_let_Width

PROVEN: Return the outer glyph width from the selected font or numeric sprite bank.

Units: see parameters/result contract. DOS layout/address: 1:0eeb8.

font_size1 adds41; glyph>=1000 selects alpha_numbers at glyph-1000. Unsupported font_size leaves sprite uninitialised in DOS.

### function:calc_let_Width2

PROVEN: Return glyph inner width plus its low-nibble x inset.

Units: see parameters/result contract. DOS layout/address: 1:0ef1c.

This advance measure differs from calc_let_Width; same bank selection and unsupported-size caveat.

### function:INIT_TEXT_TO_DISPLAY

PROVEN: Clear text and two drawing-control bytes for display slots1 through10.

Units: see parameters/result contract. DOS layout/address: 1:0ef80.

Slot0 is intentionally excluded; 11 physical slots are declared.

### function:deter_num_let

PROVEN: Decode one text byte or a bracket-prefixed three-character numeric sprite escape into a glyph number.

Units: see parameters/result contract. DOS layout/address: 1:0efb4.

Escape introducer is ] (not [); returns atoi(next3)+1000; unknown bytes return0. Extended byte mappings are explicit DOS encoding values.

### function:calc_num_let_spr

PROVEN: Add41 modulo256 to the pointed byte glyph number when font_size equals1.

Units: see parameters/result contract. DOS layout/address: 1:0f194.

The <1000 check is always true for u8; do not widen the pointed type to make the apparent check meaningful.

### function:calc_largmax_text

PROVEN: Accumulate glyph advances from the character after char_index until slash or NUL.

Units: see parameters/result contract. DOS layout/address: 1:0f1b0.

Loop increments before its first test; numeric escapes consume three additional bytes; slash ends a line.

### function:INIT_TXT_BOX

PROVEN: Compute display-box rectangle from text glyph advances and the original slash-delimited line accounting.

Units: see parameters/result contract. DOS layout/address: 1:0f21c.

200-byte local text copy; do not replace the original slash count/final-line logic with conventional line splitting. Empty text leaves rectangle unchanged.

### function:Deter_Option_Caract

PROVEN: Replace the selected text position with a..d for key0..3, optionally searching for the last dollar sign.

Units: see parameters/result contract. DOS layout/address: 1:0f4a0.

offset255 requests search and is also the no-position sentinel; the scan index is u8.

### function:SwapAB

PROVEN: Exchange the pointed signed-16 values.

Units: see parameters/result contract. DOS layout/address: 1:0f50c.

The C parameter and store widths are part of the contract.

### function:Bresenham

PROVEN: Visit spaced integer line points through a signed-16 callback using an endpoint fraction divided by128.

Units: see parameters/result contract. DOS layout/address: 1:0f520.

percent128 is full extent; strict error>0 branch and counter>gap condition matter. Preserve direction normalisation, initial counter and pre-increment quirks; this is not a drop-in standard Bresenham routine.

### function:init_finBossLevel

PROVEN: Clear the twelve named progression bits in finBosslevel.

Units: see parameters/result contract. DOS layout/address: 1:0f880.

Clears named bits only; the upper four bits of its second byte are not named or explicitly cleared here.

### function:Change_Wait_Anim

PROVEN: Swap the state0/0 and state0/56 animation ids, copy the latter into state3/20, and toggle first_boss_meet.

Units: see parameters/result contract. DOS layout/address: 1:0f8a4.

Other eta bytes stay unchanged; first_boss_meet uses arithmetic 1-value.

### function:add_alwobj

PROVEN: Append an object id to the level_obj array and increment its signed-16 count.

Units: see parameters/result contract. DOS layout/address: 1:0f8e8.

Unlike add_actobj, no following -1 sentinel is written.

### function:del_alwobj

PROVEN: Remove the first matching id from level_obj by replacing it with the last counted id.

Units: see parameters/result contract. DOS layout/address: 1:0f918.

Order is not preserved; missing ids leave count and entries unchanged.

### function:FatalError

PROVEN: Stop CD playback, enter text mode, print diagnostics, optionally wait for input, then exit with -1.

Units: see parameters/result contract. DOS layout/address: 1:0f988.

errno is a Watcom runtime accessor expression; its getter returns2:574fc in this DOS build. It has no game-data binding named errno; input_mode==1 suppresses the key wait.

### function:sprite_of_ref

PROVEN: Return the first sprite-table index whose reference id matches, or zero if absent.

Units: see parameters/result contract. DOS layout/address: 1:0fa00.

Search count is cast to u16 although stored as s16; result narrows to u8. Index0 and failure share a result.

### global:saveobj

PROVEN: Packed snapshot of object alive/active flags.

Units: two-bit entries. DOS layout/address: 2:3ce08.

16 u32 words;16 object entries per word, most-significant pair first: shift2*(15-(i&15)). Alive is pair bit0 and active pair bit1. No extra capacity claim is inferred.

### global:PROJ_CENTER_X

PROVEN: Global horizontal projection centre.

Units: pixels. DOS layout/address: 2:3f838.

s16; caller changes coordinate space deliberately.

### global:PROJ_CENTER_Y

PROVEN: Global vertical projection centre.

Units: pixels. DOS layout/address: 2:3f836.

s16.

### global:zoom_mode

PROVEN: Optional projection switch read by get_proj_dist.

Units: zero/nonzero. DOS layout/address: 2:3fa63.

Other projection helpers do not consult this byte.

### global:alpha

PROVEN: Sprite font bank selected by font_size2.

Units: font_t. DOS layout/address: 2:3e6e0.

Glyph table with outer/inner widths and pixel insets.

### global:alpha2

PROVEN: Sprite font bank selected by font_size0/1.

Units: font_t. DOS layout/address: 2:3edc8.

font_size1 offsets glyph index by41.

### global:alpha_numbers

PROVEN: Sprite bank selected by glyph numbers>=1000.

Units: font_t. DOS layout/address: 2:3e644.

Index is glyph-1000; numeric ]ddd escapes feed it.

### global:text_to_display

PROVEN: Array of eleven game text display descriptors.

Units: display_item_t[11]. DOS layout/address: 2:3c4a0.

Initialiser clears slots1..10, excluding0.

### global:errno

PROVEN: Watcom runtime error value obtained through __get_errno_ptr.

Units: runtime error code. DOS layout/address: 2:574fc.

Macro expression has no game-data binding named errno. FatalError calls1:704e8, whose getter returns fixed2:574fc, then reads32 bits.

### global:input_mode

PROVEN: Input-mode byte whose value1 suppresses FatalError key waiting.

Units: mode. DOS layout/address: 2:3fa51.

This effect does not establish a complete meaning for its other values.

## collision1.json

DOS GOG1.21 operations/consumers are primary; STRICT is independent of semantic confidence. PROVEN locals identify observed dataflow, not recovered historical C names. NEAR computational contracts use STRONG unless separately proved. Cross-build names only corroborate. Neutral unused or multipurpose storage is not renamed.

### function:get_nb_zdc

PROVEN: Extract the collision-zone count from the packed object reference.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b2d0.

Unsigned16 zdc>>11: high5 bits, range0..31.

### function:get_zdc_index

PROVEN: Extract the collision-zone table start index from the packed object reference.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b2e0.

zdc&0x7ff: low11 bits, range0..2047; this representational range does not assert a200-entry safe API domain.

### function:get_zdc

PROVEN: Locate one zone relative to the packed object start index.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b2e8.

Returns zdc_tab+((obj.zdc&0x7ff)+relative index); record stride8. No bounds check.

### function:get_ZDCPTR

PROVEN: Return the current unsigned16 ZDCPTR slot.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b300.

No semantic interpretation of the slot is established by this accessor or the negative writer search.

### function:in_coll_sprite_list

PROVEN: Test membership in the object's eligible sprite-collision layer list.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b308.

Default layer0; generated badguy0/1/2; mite1 plus state/frame-dependent2/3. Byte0xff terminates the list.

### function:box_inter_v_line

PROVEN: Test strict box-edge intersection with a vertical line segment.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b3b8.

Requires left<line_x<right and either top or bottom strictly between segment endpoints. A line segment entirely inside the box can return0; touching endpoints return0.

### function:box_inter_h_line

PROVEN: Test strict box-edge intersection with a horizontal line segment.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b3f4.

Requires top<line_y<bottom and either left or right strictly between segment endpoints. Touching endpoints and full segment containment need not count.

### function:inter_box

PROVEN: Test closed rectangle overlap with signed16 intermediate narrowing.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b430.

x1-w2,y1-h2,w1+w2,h1+h2 each narrow to signed16 before inclusive comparisons. Ordinary-domain touching edges/corners count. Width/height are coordinate spans here; this differs from support x+w-1.

### function:GET_OBJ_ZDC

PROVEN: Compute the type/state-specific object collision rectangle.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1b48c.

Outputs signed16 world-pixel x/y/spans using animation/sprite bounds or explicit type geometry. Generating-door inset=24-12*(eta bit7+2*bit6). Type-specific zero spans are retained.

### function:GET_SPRITE_ZDC

PROVEN: Adjust a sprite-layer rectangle for collision according to object type and state.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1c028.

GET_SPRITE_POS success controls validity. Mites, cymbals and generated badguys change offsets/dimensions; failed retrieval leaves output pointers unwritten.

### function:BOX_HIT_SPECIAL_ZDC

PROVEN: Test hardcoded bag and BB1-family collision shapes.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1c33c.

Returns-1 for none and type-specific sprite codes1/6/9 for hits. bagDy=-1 disables that frame. BB1 distinguishes Rayman from fist by input height==cached Rayman height, not a passed object type.

### function:BOX_IN_COLL_ZONES

STRONG: Test a box against packed zones or sentinel-selected fallback shapes.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1c670.

Packed records first; bit2(4) zones only accept fist type94; bit1(2) attaches to a sprite layer. Flip mirrors sprite-local or object-origin rectangles. Result is sprite code or-1; fallback0xfe object-box hit returns1,0xff enumerates eligible layers. Absent sprite indices leave prior scratch rectangle intact.

### function:COLL_BOX_SPRITE

PROVEN: Test a box against the single layer named by hit_sprite.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1ca84.

Returns the stored layer id on successful sprite retrieval and overlap, otherwise-1. Layer0 is a valid hit.

### function:CHECK_BOX_COLLISION

PROVEN: Dispatch box collision by the object's collision-sprite selector.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1cb1c.

0xfd invokes special geometry;0xfe/0xff invoke packed/fallback zones; other bytes select one sprite. Failure sentinel-1 must not be tested as boolean.

### function:possible_sprite

PROVEN: Return one element of the type-specific support-layer list.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1cb9c.

Lists use255 sentinel and index supplied by caller. Known callers restrict object types; unsupported type/default has no initialized result in reconstruction and is not assigned an intended value.

### function:setToleranceDist

STRONG: Encode vertical distance and horizontal support tolerance.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1ce34.

Main x interval is inclusive[x,x+w-1]; margin8 normally,4 tiny. Inside returns abs(ray_bottom-y), margin only returns-abs(...)-1, outside returns10000. Negative is lateral-margin status, not below/above.

### function:SET_RAY_DIST_SLOPEY_PLAT

STRONG: Compute support distance for a slope platform using signed hit_points as slope.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1cf14.

Projects width with cos(pente[abs((s8)hit_points)]) amplitude512 and slope delta>>4. Inside returns absolute vertical distance; lateral margin negates without the -1 used by setToleranceDist. Invalid distance10000.

### function:SET_RAY_DIST_MULTISPR_CANTCHANGE

PROVEN: Select a candidate support layer and store its Rayman distance.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d0d4.

Reuses current layer until its tolerance gives10000; scans possible_sprite until abs(distance)<=7 or255. Some branches use signed ray_bottom-surface_y rather than the general absolute encoding. Do not assign one universal signed-distance convention.

### function:SET_RAY_DIST_PI

PROVEN: Compute support distance from fixed layer2 and its narrowed55-pixel span.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d334.

Temporarily clears flip to retrieve geometry, restores it, adds4 to x and offset_hy to y; stores follow_sprite2 on valid tolerance.

### function:SET_RAY_DIST_BAG

PROVEN: Compute a bag's support shape and launch Rayman at the terminal supporting frame.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d3ec.

Only sub3/6 with frame<12 expose45-pixel support. Supported sub6 frame11 detaches, selects2:0 and subtracts10 vertical speed.

### function:SET_RAY_DIST

PROVEN: Select the object's support-distance calculator.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d4d0.

Dispatches multisprite flag0x800, PI/BBL, bag3, slope type254; ordinary support uses follow_sprite and offset_hy, falling fruits widen by4. Stores10000 for unavailable support where specified.

### function:do_boum

PROVEN: Mark fist impact and stop its outbound movement.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d5cc.

Allocates impact effect, sets is_boum1. Outbound fist clears charge and object speed_x; returning fist keeps them.

### function:DO_POING_COLLISION

PROVEN: Test the fist against tiles and active object collision shapes.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d5fc.

Tile flag2 causes impact and follow release. Object candidates require display_prio, HP!=0, eta bit3; first valid collision calls per-type fist callback, resets gravity phases, sounds unchanged-HP impact conditionally, then stops scanning.

### function:DoPoingCollisionDefault

PROVEN: Empty default per-type fist collision callback.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d858.

Both parameters are unused.

### function:COLL_BOX_ALL_SPRITES

PROVEN: Return the first overlapping animation layer index.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d85c.

Iterates low14-bit layer count. Reconstruction's result is uninitialized if no layer hits; no failure sentinel is asserted for that path.

### function:COLL_RAY_PIC

STRONG: Test the sampled Rayman tiles for hurt tile type8.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1d914.

Uses cached ray.ray_dist tile index, support/air-motion checks and tiny/crouch-dependent upper samples. Some negative upper indices wrap by whole map rows; preserve the asymmetric sample pattern.

### function:COLL_RAY_BLK_MORTEL

PROVEN: Enter lethal-spike or cliff states from tile contact.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1da98.

Type24 at feet or cached upper sample selects3:32, dead_time1 and loss audio; feet branch adds y5. Type25 selects2:31 and locks vertical scroll to current ymap.

### function:RAY_KO

PROVEN: Enter stationary knockout state3:0.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1dbac.

Clears speeds and ray.config; called at collision-loop end when config!=0.

### function:RAY_HIT

STRONG: Apply optional HP damage and establish hurt knockback.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1dbdc.

hurt0 still applies knockback/state effects; hurt1 calls RAY_HURT then lifts Rayman out of water. Selects6:8 mounted,0:61 blocked crouch,otherwise2:8. Object-specific ejection overrides relative-speed sign; default bump2, fast5, saxo4. Resets support/gravity/charge and marks in_air_because_hit.

### function:standard_frontZone

STRONG: Extend an animation rectangle into the object's facing detection region.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1df34.

Facing0 shifts x left by detect_zone, facing1 advances x by half width; width becomes half original plus detect_zone. Pixel coordinates.

### function:SET_DETECT_ZONE_FLAG

STRONG: Update the object's consecutive-overlap detection phase.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1df88.

Type-specific world-pixel detection rectangle against cached Rayman box. Outside0, first inside1, next2, then increments through19 and wraps20 to2. Photograph exit also clears hurtbyfist.

### function:goToRay

PROVEN: Apply type-specific facing and script entry when a linked object is released.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1e738.

Plays204; some types force flip0, badguys choose labels2/3, clown label7, audio-start timer1; default faces Rayman.

### function:unleashMonsterHost

PROVEN: Activate objects detached from a generating-door link cycle.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1e998.

Starts at obj.link, skips already taken collectibles, initializes/alives/activates and calls goToRay, removing each node from link_init until self-successor termination. obj.link is not a universal next-link field.

### function:SHOW_COLLISIONS_ZONES

PROVEN: Traverse enabled collision candidates and call box checks while discarding the result.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1ea2c.

Requires type collision permission and eta bit5. No visible rendering effect is inferred from the name.

### function:DO_COLLISIONS

PROVEN: Dispatch Rayman contact callbacks and age the invulnerability counter.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1eae8.

Eta bit5 gates object candidates; ordinary path requires ray.config0, iframes-1, ray eta bit3 and type permission. Special type flag0x100000 bypasses normal timer gates. After callbacks, ray.config invokes RAY_KO and iframes>-1 decrements once;0 becomes-1.

### function:DoRaymanCollisionDefault

PROVEN: Apply special contact knockback while preserving ongoing invulnerability.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1ed74.

For type flag0x100000 and eta bit5, timer-1 damages then sets60 unless dead; all other timers perform hurt0 and restore old counter. Spike death3:32 excluded.

### function:DO_OBJ_COLLISIONS

STRONG: Apply selected falling-object contacts to active badguys and the Joe buoy.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1ee20.

Insets source animation rectangle (falling types use fixed10/20/-20/-25). Badguy contact creates support state0:4 and removes source; buoy contact changes scripts and horizontal speed by facing/sign.

## collision2.json

DOS GOG1.21 operations/consumers are primary; STRICT is independent of semantic confidence. PROVEN locals identify observed dataflow, not recovered historical C names. NEAR computational contracts use STRONG unless separately proved. Cross-build names only corroborate. Neutral unused or multipurpose storage is not renamed.

### function:test_allowed

PROVEN: Report entry into a different sampled tile and cache its linear index.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f1e0.

Signed16 map_index=((x+centre_x)>>4)+((y+centre_y)>>4)*width; equality compares bit pattern with u16 test_block_index. Repeated tile returns0, new tile stores and returns1.

### function:obj_jump

PROVEN: Start the two supported enemy-specific jumps.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f224.

Stonewoman requires eta bit0 ->2:3,label15,gravity phase5; blacktoon config bit1 ->2:5,label1. Both speed_y-3,y-12, retain horizontal speed. Label255 means no action.

### function:DO_MOVING_PLATFORM_COMMAND

PROVEN: Process direction-change markers and explicit moving-platform speed commands.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f2fc.

New marker tile plus on_block_chdir selects label99. GO_SPEED copies iframes_timer/follow_id to speed_x/y; these object fields are command parameters here, not timers/support ids.

### function:DoPTGRAPPINPoingCollision

PROVEN: Convert a fist hit on a grapple object into swinging attachment.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f378.

Requires grap capability, nondeath mode and vertical anchor distance<250; stores grapple id, angle and direction, enters7:0, then disables fist and its follow.

### function:DO_ONE_CMD_WAIT

PROVEN: Apply the wait command's state-specific stopping behaviour.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f44c.

Main1 stops both speeds and becomes0:0; idle fish0:0 clears only vertical speed.

### function:DO_ONE_CMD_LR_ATTENTE

PROVEN: Handle an idle object's left/right command by movement, jumping or attacks.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f488.

Substate exclusions choose walk state1:0; sub3/6 stop command reading and set speed_y-8; sub11/2/22 dispatch hunter or clown attacks.

### function:DO_ONE_CMD_UPDOWN

PROVEN: Apply up/down commands to fish and two platform types.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f594.

Fish/plain platform speed+-2; interactive platform decrements nb_cmd and moves+-1 while positive, otherwise stops, saves init_y and enters WAIT.

### function:special_pour_liv

PROVEN: Disable command reading for badguy type0 in state1:11.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f63c.

No other type/state is changed; intended narrative spelling is not inferred.

### function:DO_ONE_CMD

PROVEN: Dispatch the object's current movement command.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f658.

WAIT,LEFT,RIGHT,UP,DOWN,SPEED20,NOP30. Main state captured before the preliminary helper; left/right set facing, then choose grounded/air/idle handlers. SPEED reuses iframes_timer/follow_id as axis speeds.

### function:DO_ROLL_EYES

PROVEN: Aim eye animation at Rayman and advance the killing-eye firing cycle.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f714.

Uses quadrant and N_anim[45*abs(dx)/abs(dy)] to derive36-direction frame; coincident point=-1 leaves frame. Killing-eye timers sequence0->3->1->2->0 with30/60/5/param delays; offscreen shot sets param40.

### function:DoKillingEyesPoingCollision

PROVEN: Decrement killing-eye HP and choose hurt or death state.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f978.

Remaining HP selects sub5,timer50; zero selects6. Sprite argument unused.

### function:YaUnBloc

STRONG: Sample hard tiles around a tall object's side or vertical edges.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1f9b8.

Direction0/1 samples10 rows at y+6+16*i, x anchor+-16;2 samples top two x positions;3 has asymmetric bottom-left y+154 and top-right y. Raw bit0(mask1), distinct from support bit1(mask2); no implicit symmetric correction.

### function:DO_BAG1_COMMAND

PROVEN: Run bag commands and emit a timed landing effect at frame14.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1fbf4.

Requires sub4, frame14, selected animation clock0 and screen bounds; temporarily y+86 for the effect.

### function:DoRaymanBag1Collision

PROVEN: Squash a grounded Rayman when scrolling is unlocked.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1fc64.

Requires !squashed, scroll_x/y both-1, main0/1; calls Ray_RayEcrase.

### function:MarCoince

PROVEN: Check three adjacent tiles at the maracas object's movement-leading boundary.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1fcb0.

Direction0/1 horizontal,2/3 vertical; uses signed remainder x/y%16 with thresholds<3 or>12 and block flag2, not a generic bounding-box collision.

### function:DO_MOVE_MARACAS_COMMAND

PROVEN: Move the maracas support from rider position and marker commands.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:1ffdc.

Main0:12 only; x speed thresholds10/15/18 pixels, asymmetric far cutoffs42/41; blocked upward motion increments param, resets after>240. Horizontal hard contacts stop corresponding speed.

### function:DO_FLASH_COMMAND

PROVEN: Integrate flash motion using signed1/128-pixel residuals.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:201a4.

init_x/y accumulate iframes_timer/follow_id, subtract/add128 while abs>=128 and move x/y by1. Frame=hit_points-1. These reused slots have type-specific meanings.

### function:DO_WLKNOT_COMMAND

PROVEN: Move a walking object and turn at a newly encountered direction-change tile.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20260.

WAIT/LEFT/RIGHT use SET_X_SPEED and ground motion; samples(offset_bx,offset_by-8), cached-tile guard, flips and chooses label1/2.

### function:ACTIVE_L_EAU

PROVEN: Initialize the linked visible water-strip objects.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20318.

Places first water at xmap-offset_bx, ymapmax+190; next strips at101-pixel offsets, param130, states1/2/3. Depends on contiguous initialized object records; no capacity proof is inferred.

### function:DO_EAU_QUI_MONTE

STRONG: Update world-specific rising water position and horizontal strip recycling.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20490.

World1 startup HP countdown, then phase2-gated upward speed until param40 unless activated. World3 uses stone/scroll logic; stone branch sub_speed+=14 and moves down once only when >16, subtracting16. Falls through to world4/5 wrap at x<-166 or>366 by505.

### function:allocateOtherPosts

PROVEN: Activate linked spike-post copies with phased animations and positions.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:2076c.

Follows link_init cycle; offsets x by32*i*(HP-2), param by-256*i and frame modulo source frame count. Uses original calc_obj_pos(source) call; do not silently change it to the copy.

### function:doHerseCommand

PROVEN: Initialize linked spike posts once, then apply sinusoidal vertical motion.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:2088c.

Type178/241 with link0 randomizes frame, sets param4096, allocates posts and link1; later sinYspeed(24,40) and x speed0.

### function:doBlackRaymanCommand

PROVEN: Replay the stored Rayman/fist history into Black Rayman objects.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20908.

Requires full ring; reads black cursor then advances/wraps101 entries. At cursor==ray write cursor-1 retires alive flags. Inactive fist validity suppresses copying stale coordinates. Death paths disappear or land within32 pixels before animation107.

### function:doBlKRayRaymanCollision

PROVEN: Set Rayman HP to zero on Black Rayman contact after history is full.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20be4.

Does not decrement HP or directly choose death mode; later damage/death handling interprets zero.

### function:DO_POELLE_COMMAND

PROVEN: Attach the pan object to Rayman or expose it as a support when unmounted.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20bf8.

Mounted saves horizontal motion clamped magnitude2..7 in param, updates tilt every9 calls (timer0 write8, then8 decrements), anchors via sprite top, restores saved capabilities on death/drowning. Unmounted follow_enabled1.

### function:DO_CORDE_COMMAND

STRONG: Test super-helicopter contact against the rope segments and damage them.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:20ea8.

Requires ray2:15; uses46x4 Rayman blade rectangle and4-wide rope with sub16..20 offsets/heights. HP countdown advances substate or breaks rope; stone param reset0 on contact, increments otherwise.

### function:DO_PAC_COMMAND

PROVEN: Update the stone mechanism's downward motion and rope-contact marker.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:21060.

Horizontal trigger writes HP10,state2:4. Vertical speed rises to16 on horloge8==0; then extra y at horloge8&&3 zero. param>1 clears iframes marker, otherwise1; param resets0.

### function:DO_CFUMEE_COMMAND

PROVEN: Follow Rayman's vertical position and retire rope smoke after its terminal frame.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:210f8.

y=ray.y-98; terminal frame and animation clock0 plus stone marker0 clear alive/active.

### function:DO_NOVA2_COMMAND

PROVEN: Delay a nova effect, then retire it at animation end.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:21190.

param255 disables update;positive decrements;0 resets frame, writes-1 and display priority2;-1 waits EOA then clears alive/active.

### function:doShipCommand

PROVEN: Lock the camera to the ship scene and release linked enemies in sequence.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:21208.

WAIT stops; SPEED reuses timer/follow fields; NOP30 runs linked-door release phases0..4 and50 half-clock delay, waits on last released object, selects type-specific release callbacks. No state label is inferred solely from function spelling.

### function:DO_DROP_COMMAND

PROVEN: Select droplet fall animation and retire its terminal splash.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:21400.

speed_y>1/2 selects2:1/2; abs(speed_x)>1 damps by facing except2:0; terminal2:3 clears active/alive.

### function:move_fruit_in_water

PROVEN: Let a supported floating fruit follow Rayman's horizontal direction until tile contact.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:214a4.

Supported0:13 becomes0:15; selected fruit type134 stops when Rayman has no support. Saves/restores facing around SET_X_SPEED, samples leading edge at y+16, stops on block flag2.

### function:DO_FALLING_OBJ_CMD

PROVEN: Convert falling fruits to water support and process object contacts.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:215f0.

Airborne centre tile10 selects0:14 and aligns y to16-grid+7-offset_by. Then floating motion and DO_OBJ_COLLISIONS(inset10); sub9 reinitializes.

### function:DoFallingObjPoingCollision

PROVEN: Apply fist-dependent horizontal impulse to a falling object.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:2167c.

Outbound fist substates1/3/5 use damage,8/10/12 use damage-2; impulse=5*value+10, signed by fist speed, then narrowed to16. Other cases leave reconstructed temporary uninitialized; do not fabricate an impulse for them.

### function:DO_BLKTOON_EYES_CMD

PROVEN: Preserve a blacktoon's horizontal motion across eta-induced direction reversal.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:2171c.

If old*new speed<0 after SET_X_SPEED, flips facing and restores prior speed.

### function:DO_RAY_POS_CMD

PROVEN: Activate and position Rayman after a byte countdown.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:21768.

timer255 disables;0 activates, optionally ray3:40 for marker main5, removes marker, aligns anchors and writes dead_time64. The final decrement changes0 to255, making activation one-shot.

## globals.json

Meaning confidence is separate from compiler layout and STRICT status. UNKNOWN retains neutral names. Raw source/displacement/external token hits are candidates, not type proofs.

### global:horloge

PROVEN: Shared byte phase-counter bank: clock index i has period i engine calls, with indices0/1 always zero after update.

Units: phase calls. DOS layout/address: 2:3f9f5.

Low-nibble animation clocks0..15 used here; whole bank25 bytes. EOA triggers at phase0.

25 bytes; animation uses0..15, horizontal speed1..4, and motion decay uses20. ticks=0 still advances these clocks.

### global:level

PROVEN: Loaded packed object-array pointer and signed16 object count.

Units: object array/count. DOS layout/address: 2:3cdf0.

Size6 in DOS; pointer32 and signed16 count.

Object id normally selects level.objects[id]; circular successors are maintained separately in link_init.

### global:flags

PROVEN: Per-object-type packed32-bit behaviour/movement flags.

Units: bitset by type. DOS layout/address: 2:257c.

262 entries, stride4; axis scale flags0x1000/0x2000.

0x1000/0x2000 select fractional movement axes;0x100 keeps linked groups active;0x40000000 disallows links. Read mutable runtime flags as well as initial DOS table values.

### global:actobj

PROVEN: Counted active-object id array with a following-1 sentinel.

Units: object ids/count. DOS layout/address: 2:3ef5c.

100 s16 slots and s16 count; trailing -1 sentinel managed by add_actobj.

build_active_table and basic add/remove helpers have distinct write/order rules. Count and sentinel are redundant representations, not interchangeable iteration contracts.

### global:num_world

PROVEN: Current one-based world/level selector used by engine exceptions, time tables and spawn rules.

Units: world/level index. DOS layout/address: 2:3f88c.

This pilot observes1/2/default; does not infer a complete enum.

World time-table row is32 entries; do not infer level-file capacity from that stride.

### global:block_flags

PROVEN: Per-tile-type byte classification flags used by collision and slope/water movement rules.

Units: bitset by tile type. DOS layout/address: 2:253c.

64 one-byte entries; bit1 support,bit4 rebound/side blocking,bit5 turn probe.

Moteur uses bit0,1,3,4,6 in distinct tests; no blanket fully-solid meaning is assigned to all of them.

### global:ray

PROVEN: Live Rayman object record, updated separately from the counted active-object loop.

Units: object record. DOS layout/address: 2:3e650.

Coordinates are world pixels; ray_dist is a map index in Rayman collision paths.

Uses pixel speeds in player helpers; templates/snapshots rms/raytmp preserve subsets and full copies across modes.

### global:xmap

PROVEN: Signed16 world x of the camera/map origin.

Units: pixels. DOS layout/address: 2:3f89e.

calc_obj_pos adds an additional8 to screen x.

Normal object screen x is world x-xmap+8. DO_SCROLL clamps it, including shake, to scroll_start_x/end_x.

### global:ymap

PROVEN: Signed16 world y of the camera/map origin.

Units: pixels. DOS layout/address: 2:3f8a6.

Signed16 storage.

Normal object screen y is world y-ymap. DO_SCROLL clamps it, including shake, to scroll_start_y/end_y.

### global:mp

PROVEN: Current map dimensions, tile count and packed six-byte tile array.

Units: tiles / pointer. DOS layout/address: 2:3e638.

Tile16x16 pixels; record stride6.

Tile size is16 pixels; moteur can reduce/override mp.height without reallocating map data.

### global:RayEvts

PROVEN: Packed Rayman ability/status bits, including two-bit force_run and reverse fields.

Units: bitfield states. DOS layout/address: 2:3f81a.

Tiny bit changes collision probe distance; avoid assuming every bit is a permanent unlock.

INIT_RAY_BEGIN clears only the explicitly listed bits; tiny/squashed/mounted initialization has separate paths.

### global:ray_last_ground_btyp

STRONG: Rayman ground/support classification used by airborne inertia and direction transitions.

Units: classification code. DOS layout/address: 2:3fa72.

Despite name, stores0/1 rather than tile type; tile25 and unhandled types retain it. No rename until all consumers are reviewed.

INIT_RAY writes1; basic ground helper preserves it for tile25 and updates it for other classes.

Pass3 refinement: Object landing sets1, slope type254 overrides0. determineRayAirInertia gives class0 a magnitude>256 inertia test and clears class1; other class values leave the selector unchanged. Do not universally interpret this byte as a raw tile-type id.

### global:ray_mode

PROVEN: Player update mode dispatcher:1 ordinary,2 mounted,3/4 death,5 breakout; nonpositive values use direct placement in DO_MOTEUR2.

Units: mode code. DOS layout/address: 2:3f868.

Value5 exempts WIZ from always-object removal in switchOff.

Mode0 is also used for an inactive transition; signed cheat modes must remain signed.

### global:poing

PROVEN: Fist control record, including the separate16-scaled vertical accumulator.

Units: packed control record. DOS layout/address: 2:3e770.

20-byte DOS descriptor; damage is unsigned byte.

MOVE_OBJECT special-cases type94 and uses poing.y_16; engine initialization clears active/charging without zeroing the entire record.

### global:finBosslevel

PROVEN: Packed persistent boss/help-event completion bits.

Units: bitfield state. DOS layout/address: 2:3f81e.

Twelve named bits cleared by init_finBossLevel; exact identity of each boss/help event needs progression writers.

Engine reads helped_joe_1 to enable firefly and helped_joe_2 to initialize Joe interaction state; session initialization calls init_finBossLevel.

### global:first_boss_meet

STRONG: Byte toggled by waiting-animation swapping and tested with boss hit_points/scroll lock during activation.

Units: boolean marker. DOS layout/address: 2:3fa71.

Arithmetic1-value; the story meaning suggested by name remains unproven.

in_action_zone combines it with boss hit_points and scrollLocked; DONE_MOTEUR_LEVEL conditionally calls Change_Wait_Anim. The name alone does not prove a story encounter phase.

### global:level_obj

PROVEN: Activation-candidate object-id list, initially containing non-always types and later live spawned always objects.

Units: object ids/count. DOS layout/address: 2:3cdf8.

Append and unordered swap-remove, unlike actobj sentinel list.

init_struct_level puts always types in the separate level_alw pool. Despite their names, add_alwobj/del_alwobj change level_obj, with append/unordered swap-remove. build_active_table scans it backwards to produce actobj.

### global:hand_btyp

PROVEN: Tile type at the central Rayman hand sample.

Units: tile-type id. DOS layout/address: 2:3fa7c.

calc_bhand_typ samples x+offset_bx and byte-narrowed y offset; recale_ray_on_liane may replace it with a side vine type12.

### global:hand_btypd

PROVEN: Tile type at the hand sample eight pixels to the right.

Units: tile-type id. DOS layout/address: 2:3fa80.

Central vine12 forces this byte15; otherwise samples anchor x+8. This sample is not always freshly read when centre is a vine.

### global:hand_btypg

PROVEN: Tile type at the hand sample eight pixels to the left.

Units: tile-type id. DOS layout/address: 2:3fa7f.

Central vine12 forces this byte15; otherwise samples anchor x-8.

### global:ray_on_poelle

PROVEN: Whether Rayman is attached to the pan support with its restricted capability set.

Units: boolean. DOS layout/address: 2:3fa54.

rayMayLandOnAnObject saves RayEvts, clears all capabilities except fist, writes1 and detaches ordinary support. DO_POELLE_COMMAND/death paths restore SauveRayEvts and clear0.

### global:saveRMjumpX

PROVEN: Low16 bits of Rayman world x captured on a successful jump.

Units: pixel coordinate modulo65536. DOS layout/address: 2:3f89c.

ray_jump writes current ray.x only after permission/release checks; DOS declaration is unsigned16. Consumers outside ray are retained in the symbol evidence.

### global:saveRMjumpY

PROVEN: Low16 bits of Rayman world y captured on a successful jump.

Units: pixel coordinate modulo65536. DOS layout/address: 2:3f89a.

ray_jump writes current ray.y; declaration and binding are retained, without importing the Android ordering.

### global:options_jeu

STRONG: Input callback table and packed game-option storage.

Units: record. DOS layout/address: 2:32f78.

Rayman calls four s16(void) callbacks at offsets0/4/8/12. Their runtime targets are assigned by input/menu setup. Remaining option fields retain separately documented meanings or UNKNOWN.

### global:ray_between_clic

STRONG: Super-helicopter retrigger window countdown.

Units: airborne update calls. DOS layout/address: 2:3f848.

RAY_HELICO writes20; RAY_IN_THE_AIR decrements every call unless -1. A later retrigger tests >=0. -1 is the disabled/expired sentinel;0 is still in the accepted window before the next decrement.

### global:ray_clic

UNKNOWN: Signed16 slot cleared when super-helicopter activation starts.

Units: unknown. DOS layout/address: 2:3f840.

Whole-source scan and whole-image fixups find the clear, with no computational reader or nonzero writer established. Android symbol size1 differs from this DOS16-bit slot; do not transfer its type or infer a click counter.

### global:ray_inertie

STRONG: Byte count of super-helicopter retriggers used for the paired impulse rule.

Units: retrigger count. DOS layout/address: 2:3fa5e.

Activation clears0; retriggers increment, compare>=2, then reset0 on impulse or1 on a rejected window. Android has size2 for the same spelling; DOS byte accesses decide.

### global:joy_done

PROVEN: Per-Rayman-update count/latch of handled directional inputs.

Units: byte count. DOS layout/address: 2:2994.

DO_RAYMAN/DO_RAY_ON_MS clear it. Horizontal handlers increment it; down/up are conditionally gated on zero in normal mode. Both horizontal inputs can increment it twice; it is not a strict boolean.

### global:animation_attente

PROVEN: Index of the next Rayman idle animation in a five-step cycle.

Units: cycle index. DOS layout/address: 2:3f76c.

RAY_RESPOND_TO_NOTHING selects59,64,62,63,65 for indices0..4 and wraps to0. Handled direction/airborne paths clear it.

### global:id_obj_grapped

PROVEN: Level-object id of the grapple used by Rayman swinging.

Units: object id. DOS layout/address: 2:3f842.

DoPTGRAPPINPoingCollision writes the hit object id; RAY_BALANCE/FIN_BALANCE index level.objects directly. Do not infer a universal null sentinel for all reads.

### global:ray_zdc_h

PROVEN: Cached Rayman collision-box world y coordinate, despite the h suffix.

Units: pixels. DOS layout/address: 2:3f862.

DO_RAYMAN and DO_RAY_ON_MS pass its address as GET_RAY_ZDC out_y. Collision queries use it as rectangle y; poing.c uses it as fist vertical origin. Keep the existing binding2:3f862.

### global:ray_zdc_w

PROVEN: Cached Rayman collision-box width span.

Units: pixels. DOS layout/address: 2:3f864.

GET_RAY_ZDC out_w at2:3f864; inclusive inter_box uses x+w as the boundary, not x+w-1.

### global:ray_zdc_x

PROVEN: Cached Rayman collision-box world x coordinate.

Units: pixels. DOS layout/address: 2:3f860.

GET_RAY_ZDC out_x at2:3f860; compared against object collision rectangles after movement.

### global:rayStack

PROVEN: 101-entry ring of Rayman and optional fist display snapshots for Black Rayman replay.

Units: 18-byte records. DOS layout/address: 2:3f026.

Writer cursor wraps0..100 and full flag latches1; consumer uses a separate cursor. Coordinates are signed16, scale unsigned8 even though live ray.scale is16 bits. No fixed milliseconds delay is claimed.

### global:zdc_tab

PROVEN: Table of packed8-byte object collision-zone records.

Units: zdc_t records. DOS layout/address: 2:2dae.

High5 bits of obj.zdc select count and low11 bits start. Zone flags2 mean sprite-local geometry,4 mean fist-only; unused flag bits retain no inferred meaning. Initial200 records are read from the oracle.

### global:ZDCPTR

UNKNOWN: Unsigned16 slot returned by get_ZDCPTR.

Units: unknown. DOS layout/address: 2:3f81c.

No live writer or interpretive consumer was established by the full-image direct-reference and source-token scans. The name alone does not establish pointer, allocator or zone-count semantics.

### global:bagDy

PROVEN: Per-animation-frame vertical offset for the special bag collision rectangle.

Units: pixels / sentinel. DOS layout/address: 2:4b9c.

BOX_HIT_SPECIAL_ZDC indexes by anim_frame; -1 disables a rectangle for that frame. Source references and oracle initial20 signed16 values are archived.

### global:bagH

PROVEN: Per-frame height of the special bag collision rectangle.

Units: pixels. DOS layout/address: 2:4bec.

BOX_HIT_SPECIAL_ZDC uses y+offset_by+bagDy[frame]-bagH[frame], height=bagH[frame]. The20-entry declaration is retained; no capacity analysis is added.

### global:bagW

PROVEN: Per-frame width of the special bag collision rectangle.

Units: pixels. DOS layout/address: 2:4bc4.

BOX_HIT_SPECIAL_ZDC centres x on object x+offset_bx using width>>1.

### global:pente

STRONG: Angle lookup used to project slope-platform width from absolute signed slope.

Units: 1/512-turn angle. DOS layout/address: 2:4a7c.

SET_RAY_DIST_SLOPEY_PLAT calls cosinus(pente[abs((s8)hit_points)]) and shifts product9. Retain exact lookup entries and signed index contract; do not replace with floating atan.

### global:poing_obj_id

PROVEN: Level-object id of the live fist object.

Units: object id. DOS layout/address: 2:3f88a.

DO_POING_COLLISION uses it for a type-selected impact sound owner; object initialization establishes the associated poing_obj pointer. Existing -1 initialization is not a blanket safe call domain.

### global:ray_speed_inv

UNKNOWN: Signed16 slot cleared when Rayman enters hit knockback.

Units: unknown. DOS layout/address: 2:3f83c.

Whole-source/image scans establish the RAY_HIT zero store but no computational reader/nonzero writer. Keep its spelling and width without inventing an inverse-speed meaning.

### global:N_anim

STRONG: Integer slope-ratio lookup for eye aiming direction.

Units: direction offsets. DOS layout/address: 2:4a9c.

DO_ROLL_EYES computes45*abs(dx)/abs(dy), uses indices<=255 and adds quadrant offsets0/9/18/27. Preserve exact table and integer/narrowing order; no continuous-angle equivalence is asserted.

### global:black_fist_obj_id

PROVEN: Level-object id of Black Rayman replay fist.

Units: object id. DOS layout/address: 2:3f8be.

doBlackRaymanCommand copies fist snapshot fields only when validity byte is set and clears alive on absent/terminated playback.

### global:black_pos_in_stack

PROVEN: Next history entry read by Black Rayman replay.

Units: ring index. DOS layout/address: 2:3fa8a.

Increments and wraps after100 only while full history is replayed. Equality with ray_pos_in_stack-1 triggers alive-flag termination; subtraction is promoted, so write cursor0 compares with-1 rather than100.

## moteur.json

Meaning confidence is separate from compiler layout and STRICT status. UNKNOWN retains neutral names. Raw source/displacement/external token hits are candidates, not type proofs.

### function:INIT_HORLOGES

PROVEN: Clear the 25-byte global phase-counter bank.

Units: see parameter/result contract. DOS layout/address: 1:45480.

Indices 0 and 1 subsequently remain zero; this does not reset map_time.

### function:horloges

PROVEN: Advance each clock once with period equal to its index, and add the byte argument to map_time.

Units: see parameter/result contract. DOS layout/address: 1:45498.

Each call increments horloge[i] once then clears it if >=i, for i=0..24. ticks does not scale the clock update; ticks=0 still advances all clocks.

### function:init_allowed_time

PROVEN: Fill six 32-entry world rows of allowed_time with bonus-stage time limits and the -2 sentinel.

Units: see parameter/result contract. DOS layout/address: 1:454ec.

World/level indexing is (world-1)*32+(level-1). -2 denotes no timed bonus stage; positive entries are multiplied by 60 by calc_left_time.

### function:fades

PROVEN: Start map-entry fade at map_time zero, start the death fade in a bounded dead_time interval, and advance an unfinished fade.

Units: see parameter/result contract. DOS layout/address: 1:45ac8.

The dead_time interval is inclusive 32..display_mode+33. fade==65 starts fade-out; bit 0x40 gates DO_FADE.

### function:frapsol

PROVEN: Return the indexed expsin shake sample divided by 128 using the signed16 shift helper.

Units: see parameter/result contract. DOS layout/address: 1:45b24.

ashr16 rounds toward zero except its signed16 minimum-value quirk. The table terminal 32640 becomes 255.

### function:DO_SCREEN_TREMBLE

PROVEN: Add one scaled expsin sample to vertical camera position while advancing the shake index.

Units: see parameter/result contract. DOS layout/address: 1:45b48.

A resulting sample 255 is a terminator: clear both index and displacement before adding. Index zero disables this shake.

### function:DO_SCREEN_TREMBLE3

PROVEN: Advance the third shake index and add its expsin2 sample to ymap until sample 255.

Units: see parameter/result contract. DOS layout/address: 1:45b90.

Unlike the first shake, samples are already pixel offsets. A 255 terminator resets the index and is not added.

### function:DO_SCREEN_TREMBLE2

PROVEN: Advance the second shake index and add its expsin2 sample to xmap until sample 255.

Units: see parameter/result contract. DOS layout/address: 1:45bcc.

The sentinel is positive 255 in a signed16 table; negative samples are real displacements.

### function:DO_SCROLL

PROVEN: Apply camera shake and requested pixel displacement, clamp xmap/ymap to scroll bounds, and recalculate Rayman screen position.

Units: see parameter/result contract. DOS layout/address: 1:45c08.

Vertical speed 255 means zero displacement here. On clamping, adjust the caller speed by the clamp correction and clear the corresponding dhspeed/dvspeed; level exceptions suppress motion before shake/clamp.

### function:allocateLandingSmoke

PROVEN: Activate at most two inactive type-208 objects at the input object foot anchor, using states 0/0 and 0/1.

Units: see parameter/result contract. DOS layout/address: 1:45d78.

Searches the whole level separately for each side. Sets alive/active, appends to the activation-candidate list via add_alwobj, clears activation delay/state and uses display priorities 7 and 2.

### function:instantSpeed

PROVEN: Convert a signed16 speed in sixteenths to this map_time phase integer displacement.

Units: see parameter/result contract. DOS layout/address: 1:45e60.

q=ashr16(speed,4), r=abs(speed)&15; return q+sgn(speed)*((wrap32(map_time*r)>>4)-(wrap32(map_time*r-r)>>4)). The two 32-bit shifts round down; the initial 16-bit helper rounds toward zero. All objects share map_time phase.

### function:SET_X_SPEED

PROVEN: Select state left/right speed on an animation-header-selected clock phase, otherwise store zero horizontal speed.

Units: see parameter/result contract. DOS layout/address: 1:45ee4.

Clock index=((layers_per_frame>>14)&3)+1. Multiply the signed8 state speed by this period on phase zero; halve for tiny Rayman via ashr16. The resulting speed still uses the object type axis representation.

### function:make_active2

PROVEN: Set active, the activation mark, and activation state zero unconditionally, optionally emitting a nova.

Units: see parameter/result contract. DOS layout/address: 1:45f9c.

Does not test alive. make_active provides the alive guard for linked members.

### function:make_active

PROVEN: Activate a living object and mark it processed, optionally emitting a nova.

Units: see parameter/result contract. DOS layout/address: 1:45fbc.

A dead object remains untouched; this distinction matters during circular-link traversal.

### function:in_action_zone

PROVEN: Classify a type-dependent activation region using screen coordinates, level bounds and optional previous-active expansion.

Units: see parameter/result contract. DOS layout/address: 1:45fe4.

Uses global ot as the type discriminator. Previous-active expands each ordinary region margin by 60 pixels. Special types bypass some ordinary bounds; boss acceptance sets IsBossThere and may change idle animation.

### function:kill_obj

PROVEN: Move an object to (-32000,-32000), set activation state 1 and clear alive.

Units: see parameter/result contract. DOS layout/address: 1:46310.

Does not clear is_active itself. Hidden-position sentinel and alive state are separate effects.

### function:SET_ACTIVE_FLAG

PROVEN: Update activation, reactivation delay and linked-group reinitialization from action-zone membership and prior state.

Units: see parameter/result contract. DOS layout/address: 1:46338.

0 is normally activated; 1 is inactive/dead-side bookkeeping; 2 is reinitialised waiting to leave the zone; 4 marks members waiting for the entire linked group to leave. Delay decrements only in-zone and only when its signed16 interpretation is positive.

### function:DO_PESANTEUR

PROVEN: Apply the current state high-nibble vertical-acceleration rule and return its rebound-compensation marker.

Units: see parameter/result contract. DOS layout/address: 1:465d0.

Modes 1/2 use zero phases of object counters 1/2; 3 selects a step toward raw speed +1, 4 toward raw speed -2, 5 subtracts on counter1 zero, 6 adds on horloge[2] zero, 10/11 use counter1 against counter2. Modes 0,7,8,9,12..15 do no generic acceleration. Tambour types override nonzero modes with config countdown/counter1 gating. Scale generic acceleration by16 for fractional-y types, while comparisons to +1/-2 remain raw; the tambour direct increment is not scaled.

### function:freezeAnim

PROVEN: Place the animation frame at a capped supplied offset, with a distinct reverse-playback formula.

Units: see parameter/result contract. DOS layout/address: 1:46734.

Let last=frames_count-1. Forward uses min(last,offset), reverse uses last-min(last,offset)+1, narrowed to u8. The reverse result can equal frames_count; do not replace with a symmetric clamp.

### function:DO_ANIM

PROVEN: Advance the selected clock-gated animation, apply reset requests, transition states at frame overflow, and update follow/changed bookkeeping.

Units: see parameter/result contract. DOS layout/address: 1:467ac.

Low-nibble period zero suppresses advancement. Reverse byte decrement underflows to 255 and triggers the end transition. Reset modes 1/2 can play a state sound whenever active, including mode1 with unchanged animation. Clear change_anim_mode after processing; follow deltas use saved prior frame and animation.

### function:prof_in_bloc

PROVEN: Return the object foot coordinate modulo the 16-pixel tile height.

Units: see parameter/result contract. DOS layout/address: 1:46a38.

Computes (obj.y+offset_by)&15, including low-bit behaviour for negative positions.

### function:do_boing

PROVEN: Settle a slow vertical bounce or apply tile-dependent rebound impulses on the period-2 clock.

Units: see parameter/result contract. DOS layout/address: 1:46a4c.

At speed_y<=1 stop and set the supplied state, snapping unless main state 2. Otherwise table cases alter speed_x and reflect/dampen speed_y; upward result selects substate zero.

### function:underSlope

PROVEN: Test bit 0 of the foot tile flags together with bit 6 of the upper tile flags.

Units: see parameter/result contract. DOS layout/address: 1:46bc4.

This is the exact two-sample classification, not a generic geometric slope test; btypes[0] and btypes[3] are distinct samples.

### function:DO_STONEBOMB_REBOND

PROVEN: Apply type-47/type-69 tile-dependent horizontal/vertical rebound speeds and reset gravity counters to 0 and 2.

Units: see parameter/result contract. DOS layout/address: 1:46bec.

Type47 starts (32,-4), type69 (48,-6); slope orientation changes horizontal sign/size. Callers establish these two types, including a temporary type swap in DO_THROWN_BOMB_REBOND.

### function:DO_THROWN_BOMB_REBOND

PROVEN: Handle thrown-bomb impact, state-specific explosion/type delegation, slope impulses, rebound and position correction.

Units: see parameter/result contract. DOS layout/address: 1:46cd8.

The positive rebound offset reflects speed_y only above 1, adding one when the gravity-return marker is nonzero; nonpositive offset sets -3. Reset counter1; preserve counter2 in modes10/11. Under-slope escape iterates upward until the sampled tile differs.

### function:DO_FRUIT_REBOND

PROVEN: Apply fruit/seed horizontal damping, tile impulses, vertical rebound and floor correction.

Units: see parameter/result contract. DOS layout/address: 1:46f00.

Seed type38 uses smaller damping/impulses. Positive rebound offset can stop with hit_points=2; nonpositive offset has type-dependent platform impulses. Reset counter1 and counter2 except modes10/11.

### function:Drop_Atter

PROVEN: Put a landed drop in state 2/3 and stop both speeds.

Units: see parameter/result contract. DOS layout/address: 1:4720c.

No position snapping is performed by this helper.

### function:BadGuyAtter

PROVEN: Snap a landed bad guy, choose its hurt/ordinary walking state, face Rayman and select the direction script label.

Units: see parameter/result contract. DOS layout/address: 1:47230.

Damage is hit_points!=init_hit_points; state flag0x40 selects hurt substate2 versus11. Ordinary landing reenables command reading.

### function:MiteAtter

PROVEN: For a descending mite, snap to floor, select script label2 and clear follow_id when it exceeds500.

Units: see parameter/result contract. DOS layout/address: 1:472c4.

The >500 test is retained as a type-specific use of follow_id; it is not an object-id range assertion.

### function:LidolPinkAtter

PROVEN: Bounce LidolPink in the gerbe/substate1 case, otherwise stop and snap applicable nonzero substates.

Units: see parameter/result contract. DOS layout/address: 1:472f8.

The global gerbe changes landing behaviour; no universal hit_points/param interpretation is inferred.

### function:stoneDogAtter

PROVEN: For stone-dog substate2, select script label4 and snap to the floor.

Units: see parameter/result contract. DOS layout/address: 1:47344.

Other substates are untouched.

### function:stoneDogBounces

PROVEN: Probe tiles two pixels either side of the foot and alter stone-dog bounce script/direction on a slope edge.

Units: see parameter/result contract. DOS layout/address: 1:4736c.

Uses integer division speed_x/16 for its position nudge; division rounds toward zero. This is not the map_time fractional distributor.

### function:Spider_Atter

PROVEN: Set the spider walking state 1/0 and choose script label1 or3 from facing.

Units: see parameter/result contract. DOS layout/address: 1:474b8.

No floor snap is issued here.

### function:NormalAtter

PROVEN: Restore the saved initial main/substate and snap the object to the floor.

Units: see parameter/result contract. DOS layout/address: 1:474f4.

Initial state bytes are state-table indices, not animation indices.

### function:OBJ_IN_THE_AIR

PROVEN: Update gravity counters, apply acceleration, handle upward tile blocking, dispatch type-specific landing, and cap vertical speed.

Units: see parameter/result contract. DOS layout/address: 1:47530.

Except modes10/11, advance counter1 modulo3 and counter2 modulo4 before DO_PESANTEUR. Generic caps are [-8,6] pixels or [-128,96] sixteenths. Counter/mode and landing ordering are observable; upward blocking samples the predicted top coordinate.

### function:test_fall_in_water

PROVEN: Apply type/flag-dependent water-entry splash, velocity stop and offscreen sinking effects.

Units: see parameter/result contract. DOS layout/address: 1:47fec.

Type52 clears alive directly. Other flagged types splash only while object flag0x40 is clear; die-in-water types clear active and move y to ymap+484. Rayman main state6 is exempt.

### function:MOVE_OBJECT

PROVEN: Integrate object speeds, optionally decay horizontal magnitude and turn at a wall, set movement bookkeeping and process water.

Units: see parameter/result contract. DOS layout/address: 1:48084.

Ordinary axes use instantSpeed only when their type bits0x1000/0x2000 are set. The fist path uses poing.y_16 and direct x speed. DOS has B20 uninitialised bookkeeping-bit production on its fist path; candidate assigns zero. This pass changes no behaviour.

### function:DO_RAY_IN_ZONE

PROVEN: Call the object-type Rayman-in-zone callback when OBJ_IN_ZONE accepts the object.

Units: see parameter/result contract. DOS layout/address: 1:48270.

Dispatch is ObjectsFonctions[type].rayman_in_zone, not a direct collision event.

### function:DoRaymanInZDDDefault

PROVEN: Default Rayman-in-detection-zone callback with no effects.

Units: see parameter/result contract. DOS layout/address: 1:4829c.

Parameter is unused; no stronger role or event is inferred from this empty body.

### function:DO_ONE_OBJECT

PROVEN: Run one object type update in tile/script/command/gravity/follow/platform/detection order.

Units: see parameter/result contract. DOS layout/address: 1:482a0.

Uses global ot throughout. Disabled script reading sets command GO_NOP=30. A non-follow-enabled object gets ray_dist=10000. main state2 gates airborne processing.

### function:build_active_table

PROVEN: Evaluate candidate level objects with linked activation marks and build the active-id list in reverse level_obj order.

Units: see parameter/result contract. DOS layout/address: 1:48388.

Clear field_1C for the candidate list, then evaluate only unmarked objects; linked activation marks suppress later reevaluation. Emit active ids with screen_x=x-xmap+8 and screen_y=y-ymap, then -1. The DOS capacity guard is after writing and tests added>100, so it is not a safe pre-write capacity check.

### function:Add_One_RAY_lives

PROVEN: Increment lives up to99 and restore Rayman health to four only when max_hitp is four, otherwise two.

Units: see parameter/result contract. DOS layout/address: 1:48644.

Health restoration still occurs when lives were already capped.

### function:DO_CLING_ANIMS

PROVEN: Advance the delayed extra-life and power pickup animations, retire them at EOA and award the life when allowed.

Units: see parameter/result contract. DOS layout/address: 1:48680.

-1 means no pickup object. A positive timer decrements while resetting anim_frame before DO_ANIM; death modes3/4 suppress the life award.

### function:DO_OBJECTS_ANIMS

PROVEN: Animate active-list entries except the recorded black Rayman/fist types204/205.

Units: see parameter/result contract. DOS layout/address: 1:48754.

Iteration uses num_active_objects, not the -1 sentinel; the loop preloads the next entry even on termination.

### function:DO_OBJECTS

PROVEN: Run cling/toon allocation and active-object dispatch with switch-off and hit-point-zero handling.

Units: see parameter/result contract. DOS layout/address: 1:487c8.

ot is set from each active-list object before DO_ONE_OBJECT. Type161 is excluded from ordinary update. Nonboss zero-health objects in eta flag8 regain one hit point after their update.

### function:MOVE_OBJECTS

PROVEN: Apply MOVE_OBJECT to each counted active-list entry.

Units: see parameter/result contract. DOS layout/address: 1:48908.

Uses the counted list and preloads the next id; it does not filter alive/active again.

### function:RECALE_ALL_OBJECTS

PROVEN: Recalculate active-object screen positions and attach Rayman vertically to the active followed sprite.

Units: see parameter/result contract. DOS layout/address: 1:48964.

If follow_id!=-1 and the target is active, use followed sprite y+offset_hy-ray.offset_by. Type254 adds a signed8 hit_points-dependent tilt with ashr16 shift4; preserve the intermediate signed16 narrowing.

### function:RayCoince

PROVEN: Probe a three-tile strip at the selected side of mounted Rayman when near the corresponding tile-grid boundary.

Units: see parameter/result contract. DOS layout/address: 1:48aac.

dir0/1/2/3 select left/right/up/down. Tests block_flags bit0 and returns a boolean; the probes use DOS-specific offsets58/70 and32.

### function:move_up_ray

PROVEN: Apply Rayman upward movement, stop at the scroll-start top bound and recalculate screen position.

Units: see parameter/result contract. DOS layout/address: 1:48de8.

Mounted mode may first stop on RayCoince(2). Top margin is40 for state flag0x40 or tiny, otherwise25; hitting the bound while following calls RAY_HIT. Always clear v_scroll_speed.

### function:move_down_ray

PROVEN: Apply downward movement and set vertical camera speed for the airborne screen-y threshold.

Units: see parameter/result contract. DOS layout/address: 1:48ec8.

Mounted RayCoince(3) can stop speed. Only main state2 updates the camera speed here; screen_y<100 or insufficient remaining scroll clears it.

### function:recale_ray_pos

PROVEN: Compute camera follow speeds from Rayman screen position, facing, inertia, motion and followed-object displacement.

Units: see parameter/result contract. DOS layout/address: 1:48f70.

scroll_x/y==-1 enable ordinary follow. Horizontal target errors are quartered via ashr16, dhspeed approaches the chosen target by one, and another divide4 contributes to h_scroll_speed. Vertical speed255 is a special state. Followed-object y displacement and follow_y are added only under ordinary vertical follow.

### function:RAY_TO_THE_RIGHT

PROVEN: Apply Rayman rightward movement and clamp to RIGHT_MAP_BORDER, with mounted collision handling.

Units: see parameter/result contract. DOS layout/address: 1:49418.

A right clamp clears horizontal speed and inertia; if grounded it also clears vertical speed. Recalculates screen position.

### function:RAY_TO_THE_LEFT

PROVEN: Apply Rayman leftward movement and ordinary-follow left-bound clamping, with mounted collision handling.

Units: see parameter/result contract. DOS layout/address: 1:4949c.

The left clamp clears inertia and optionally vertical speed, but does not explicitly clear speed_x or recalculate screen position here. Preserve this asymmetry.

### function:DO_FIXE

PROVEN: Derive HUD health slots and decimal lives/tings digits from live counters.

Units: see parameter/result contract. DOS layout/address: 1:49518.

Lives -1 displays00; tings are capped at99; health slot ordering is explicitly [1] for the first two points and [0] for excess.

### function:deactivate_ship_links

PROVEN: Clear alive on all other members of the ship circular link when the ship group marker is set.

Units: see parameter/result contract. DOS layout/address: 1:495e8.

bateau_obj_id==-1 means absent. The ship itself is excluded from traversal effects.

### function:linkListHoldsAGendoor

PROVEN: Search other members of the object circular link for type164.

Units: see parameter/result contract. DOS layout/address: 1:4965c.

Starts at link_init[obj.id] and stops before obj.id; a generating door does not find itself.

### function:correct_gendoor_link

PROVEN: Detach generating doors from their circular groups, retain their next id in obj.link and optionally unlink dead-door groups.

Units: see parameter/result contract. DOS layout/address: 1:496c0.

Active doors first clear alive on other traversed members. The loop tolerates a self-linked tail through link!=last_link; normal group traversal otherwise assumes a closed valid cycle.

### function:suppressFromLinkList

PROVEN: Splice one object out of a circular successor list and self-link the removed object.

Units: see parameter/result contract. DOS layout/address: 1:497fc.

Find the predecessor, point it to the removed successor, clear group flags for the removed object and any newly singleton predecessor. No -1 link terminator is used.

### function:correct_link

PROVEN: Remove forbidden or door-required-without-door link members and set remaining group markers.

Units: see parameter/result contract. DOS layout/address: 1:49888.

A group marker does not prove the group has a generating door: types without that requirement are marked too.

### function:INIT_RAY_BEGIN

PROVEN: Clear the listed Rayman ability/status bits and set initial HUD health/tings and continuation state.

Units: see parameter/result contract. DOS layout/address: 1:49900.

This is not a complete RayEvts zeroing: super_helico, magicseed and tiny are not cleared by this body.

### function:INIT_RAY

PROVEN: Reinitialize the live Rayman object, abilities and camera, optionally locating a type99 start marker.

Units: see parameter/result contract. DOS layout/address: 1:49948.

Initial absent follow/link/iframes values are -1; remoteRayXToReach=-32000. Tiny mode restores rms first. New-level argument clears super_helico; magicseed is retained only in the special level or demo/record contexts.

### function:is_icy_pente

PROVEN: Return whether the tile flags have both bit6 and bit3 set.

Units: see parameter/result contract. DOS layout/address: 1:49cf4.

This is an exact flag conjunction used to allow movement across icy slopes; no type-number renaming is inferred.

### function:STOPPE_RAY_EN_XY

PROVEN: Look ahead by twice horizontal speed through up to four vertical tile samples and cancel blocked Rayman horizontal movement.

Units: see parameter/result contract. DOS layout/address: 1:49d18.

Main state7 bypasses checks. Crouched/tiny states use fewer samples. Blocking clears speed_x, inertia and nb_cmd and resets both air-speed variants.

### function:DO_PLACE_RAY

PROVEN: Move Rayman from direct directional input using RAY_MODE_SPEED and feed those speeds to the camera.

Units: see parameter/result contract. DOS layout/address: 1:49e9c.

Horizontal cheat speed is full RAY_MODE_SPEED; vertical is shifted right1. Runs movement helpers and displays collision zones.

### function:DO_AUTO_SCROLL

PROVEN: Interpret type64 scroll-marker thresholds and hit_points modes to select forced camera movement.

Units: see parameter/result contract. DOS layout/address: 1:49fb8.

scroll_x==-1 means ordinary follow; nonzero signed sums encode forced x. scroll_y uses -1,1,2 and a transient -2; mode2 gives a period2 half-rate upward scroll. Consumed marker x/init_x or y=10000 changes future threshold tests; hit_points is a scroll command here, not health.

### function:INIT_MOTEUR

PROVEN: Reset level engine state, camera limits, weather/particle state, Rayman and objects, and restore the appropriate checkpoint branch.

Units: see parameter/result contract. DOS layout/address: 1:4a1e0.

map_time=0, left_time=-2, horloges reset, active list count0 and first id -1. Cave5/10 overrides map height/bounds. Initial camera limits differ for video mode; width uses305 rather than320 in the nonnormal path.

### function:INIT_MOTEUR_BEGIN

PROVEN: Initialize session-level limits, counters, Rayman defaults, demo selection and initial ordinary-follow camera modes.

Units: see parameter/result contract. DOS layout/address: 1:4a5b4.

Lives3, continues9, demo duration1800, left_time=-2, scroll_x/y=-1. Avoids one selected demo world/level pair.

### function:INIT_MOTEUR_WORLD

PROVEN: Clear new_world and set new_level for world entry.

Units: see parameter/result contract. DOS layout/address: 1:4a6d8.

These are transition flags, not world/level indices.

### function:INIT_MOTEUR_LEVEL

PROVEN: Prepare displayed bounds, checkpoint/weather setup, engine initialization and the initial active table for a level.

Units: see parameter/result contract. DOS layout/address: 1:4a6f4.

Only runs its main body while fin_du_jeu is clear. Forward the argument to byte new_lvl in INIT_MOTEUR. Save/restore decisions depend on bonus_map, departlevel and fin_continue; correct_gendoor_link is called with zero.

### function:restore_gendoor_link

PROVEN: Reconnect living generating doors or restore saved successor cycles for dead generating doors.

Units: see parameter/result contract. DOS layout/address: 1:4a7dc.

The saved successor passes through u8 next, unlike live s16 links. Preserve truncation; all shipped level ids in sampled links fit in a byte. This is not a generic 16-bit-list restoration.

### function:DONE_MOTEUR_LEVEL

PROVEN: Restore generating-door links, finish save state when applicable, restore standard Rayman at transitions and align the level memory arena.

Units: see parameter/result contract. DOS layout/address: 1:4a8e4.

Can stop demo playback when a fairy exists; restoration uses rms/raytmp whole-object snapshots.

### function:INIT_MOTEUR_DEAD

PROVEN: At dead_time zero rebuild the engine and corrected door links, then save the secondary checkpoint when eligible.

Units: see parameter/result contract. DOS layout/address: 1:4a9d8.

The save2 branch also runs on nonbonus/departlevel calls where dead_time is nonzero.

### function:INIT_RAY_ON_MS

PROVEN: On NewMs, swap in the mounted Rayman object while preserving health-related fields and enter main state6/mode2.

Units: see parameter/result contract. DOS layout/address: 1:4aa20.

Uses rayman_obj_id==-1 for absent mounted template. The fallback rayOnMsWasThere branch initializes without the swap.

### function:DO_MOTEUR

PROVEN: Run demo abort input, cheats, one clock/map tick, camera/previous-state snapshots, wind selection and ordinary object updates.

Units: see parameter/result contract. DOS layout/address: 1:4aae8.

Calls horloges(1) before calc_left_time, stores prior Rayman state and camera positions, restores followed-object display priority before updates. Joystick port201 bits4..7 participate in demo abort.

### function:DO_MOTEUR2

PROVEN: Run animation/player/movement/camera/active-list/HUD/weather/particle and tiny-mode projection work for the second engine phase.

Units: see parameter/result contract. DOS layout/address: 1:4ac70.

Order is DO_OBJECTS_ANIMS then player state, auto-scroll, MOVE_OBJECTS, camera-follow, DO_SCROLL, active rebuild, RECALE_ALL_OBJECTS. Dead Rayman still moves objects/rebuilds the list and decrements dead_time. Platform display priority is temporarily capped at3 until DO_MOTEUR restores it.

### function:RAY_REVERSE_COMMANDS

PROVEN: Toggle reverse-control state and visibility/priorities of its two optional star objects.

Units: see parameter/result contract. DOS layout/address: 1:4b0a4.

reverse is a two-bit field, but this helper writes0/1. Null star pointers are accepted.

### function:RAY_DEMIRAY

PROVEN: Toggle tiny Rayman by swapping object snapshots/templates and copying the live movement/state/health subset.

Units: see parameter/result contract. DOS layout/address: 1:4b154.

Copies the listed fields, not every byte; type is restored to23 and alive/active set. A blocked upper tile forces state0/15.

### function:Ray_RayEcrase

PROVEN: Toggle squashed Rayman when tiny and squashed agree, swapping templates and saving/restoring ability bits.

Units: see parameter/result contract. DOS layout/address: 1:4b378.

Enabling squash disables listed abilities and reverse state; preserves the listed live object fields on both branches. This is distinct from the tiny-only toggle.

### function:DO_MOTEUR_GELE

PROVEN: Update the allowed frozen-engine subset and rebuild the active table even in freeze mode3.

Units: see parameter/result contract. DOS layout/address: 1:4b668.

gele2 advances horloges(1), signpost and Rayman animation; other non3 states run medallion handling. Selected objects animate/switch off, weather can still advance. A freeze does not universally stop clocks or activation.

### global:Bloc_lim_H1

STRONG: Vertical viewport/block bounds copied to display bounds at level initialization.

Units: pixels. DOS layout/address: 2:3d0a4.

Other display/map readers establish the geometry; moteur only copies these values.

### global:Bloc_lim_H1_Aff

STRONG: Displayed viewport/block bounds snapshot initialized from the corresponding live bound.

Units: pixels. DOS layout/address: 2:3d088.

These are separate display-state globals; preserve the pairing/order in INIT_MOTEUR_LEVEL.

### global:Bloc_lim_H2

STRONG: Vertical viewport/block bounds copied to display bounds at level initialization.

Units: pixels. DOS layout/address: 2:3d0a0.

Other display/map readers establish the geometry; moteur only copies these values.

### global:Bloc_lim_H2_Aff

STRONG: Displayed viewport/block bounds snapshot initialized from the corresponding live bound.

Units: pixels. DOS layout/address: 2:3d084.

These are separate display-state globals; preserve the pairing/order in INIT_MOTEUR_LEVEL.

### global:Bloc_lim_W1

STRONG: Horizontal viewport/block limits used to derive per-frame Rayman camera-relative boundaries.

Units: pixels. DOS layout/address: 2:3d09c.

INIT_MOTEUR_LEVEL copies them into displayed bounds. ModeX uses separate fixed320 geometry.

### global:Bloc_lim_W1_Aff

STRONG: Displayed viewport/block bounds snapshot initialized from the corresponding live bound.

Units: pixels. DOS layout/address: 2:3d064.

These are separate display-state globals; preserve the pairing/order in INIT_MOTEUR_LEVEL.

### global:Bloc_lim_W2

STRONG: Horizontal viewport/block limits used to derive per-frame Rayman camera-relative boundaries.

Units: pixels. DOS layout/address: 2:3d098.

INIT_MOTEUR_LEVEL copies them into displayed bounds. ModeX uses separate fixed320 geometry.

### global:Bloc_lim_W2_Aff

STRONG: Displayed viewport/block bounds snapshot initialized from the corresponding live bound.

Units: pixels. DOS layout/address: 2:3d060.

These are separate display-state globals; preserve the pairing/order in INIT_MOTEUR_LEVEL.

### global:DemoRecordMap

PROVEN: Parallel recorded-demo level/world selectors indexed by NumDemo.

Units: world/level ids. DOS layout/address: 2:5e1c.

Session initialization avoids one Cave5/10 pair; no new table-capacity inference.

### global:DemoRecordWorld

PROVEN: Parallel recorded-demo level/world selectors indexed by NumDemo.

Units: world/level ids. DOS layout/address: 2:5e10.

Session initialization avoids one Cave5/10 pair; no new table-capacity inference.

### global:First_Hist

UNKNOWN: Session initialization writes1; menu code tests for2 and clears0 in a later branch. Intended role and a live writer of2 remain unresolved.

Units: unknown state. DOS layout/address: 2:3fa41.

Keep the current name; do not infer an intro/history phase model from spelling.

### global:First_Menu

UNKNOWN: Session initialization writes1; no typed source reader or direct DOS fixup consumer was found. Intended role remains unresolved.

Units: unknown. DOS layout/address: 2:3fa42.

Keep current name. Indirect aggregate accesses are not excluded, and no unreachable-data investigation is proposed.

### global:GameModeVideo

STRONG: Video-mode selectors read by camera-limit initialization and per-frame player bounds.

Units: mode code. DOS layout/address: 2:5e2d.

They are separate globals: GameModeVideo controls initialization, ModeVideoActuel controls per-frame border geometry. ModeX=1.

### global:IsBossThere

PROVEN: Latched boss-presence marker set when an accepted action-zone object has the boss type flag.

Units: boolean marker. DOS layout/address: 2:3fa8e.

in_action_zone may accept flagged bosses while already set. Other reset sites in object initialization are evidence dependencies.

### global:LEFT_MAP_BORDER

PROVEN: Camera-relative horizontal player-origin limits, respectively right and left.

Units: pixels. DOS layout/address: 2:3f756.

DO_MOTEUR derives them from camera position and video bounds before player movement. Left/right clamp behaviour is asymmetric.

### global:MapAvecPluieOuNeige

PROVEN: Whether weather particles/snow-sequence work should run for the current map.

Units: boolean marker. DOS layout/address: 2:3f802.

Engine level setup can clear it; weather initialization sets1. Enables both live and frozen weather update paths.

### global:ModeDemo

PROVEN: Demo-mode state used for magicseed retention and abort/transition handling.

Units: state code. DOS layout/address: 2:3fa36.

DO_MOTEUR treats3/4 as abort conditions and writes2 on abort; nonzero is not the complete state meaning.

### global:ModeVideoActuel

STRONG: Video-mode selectors read by camera-limit initialization and per-frame player bounds.

Units: mode code. DOS layout/address: 2:5e2c.

They are separate globals: GameModeVideo controls initialization, ModeVideoActuel controls per-frame border geometry. ModeX=1.

### global:NbTramesEcrase

PROVEN: Squashed Rayman shrink-delay countdown.

Units: engine calls. DOS layout/address: 2:3fa97.

Tiny/squashed projection path resets200 when stable, decrements before reducing scale by5.

### global:NewMs

STRONG: Pending switch to mounted Rayman mode.

Units: boolean request. DOS layout/address: 2:3fa86.

INIT_RAY_ON_MS consumes it after a successful template/fallback initialization.

### global:NumDemo

PROVEN: Current recorded-demo selection index.

Units: demo index. DOS layout/address: 2:3f76a.

Session chooses myRand(5), then may advance/wrap at6 for the excluded pair.

### global:NumScrollObj

PROVEN: Count of level type64 auto-scroll marker ids.

Units: objects. DOS layout/address: 2:3f87a.

INIT_OBJECTS populates scroll_obj_id and checks the50-entry threshold; DO_AUTO_SCROLL breaks early if a selected object is not type64.

### global:ObjectsFonctions

PROVEN: Object-type callback dispatch table with command/fist collision/Rayman collision/in-zone/turn slots.

Units: function pointers by object type. DOS layout/address: 2:35fc.

Moteur invokes command and rayman_in_zone; callers in other units prove the remaining slots.262 records are current declaration, not a new capacity inference.

### global:ProchainEclair

STRONG: Weather lightning scheduling control forced to1 during eligible frozen weather update.

Units: weather sequence state. DOS layout/address: 2:3f742.

Full countdown/selection meaning belongs to weather code; no timer unit inferred here.

### global:RAY_MODE_SPEED

PROVEN: Direct-placement cheat motion magnitude.

Units: pixels/call. DOS layout/address: 2:3fa73.

DO_PLACE_RAY uses full value horizontally and value>>1 vertically; level initialization sets16.

### global:RIGHT_MAP_BORDER

PROVEN: Camera-relative horizontal player-origin limits, respectively right and left.

Units: pixels. DOS layout/address: 2:3f75a.

DO_MOTEUR derives them from camera position and video bounds before player movement. Left/right clamp behaviour is asymmetric.

### global:RaymanDansUneMapDuJeu

STRONG: Marker that live Rayman is in a playable game map, enabling collision-tile initialization.

Units: boolean. DOS layout/address: 2:3fa98.

INIT_RAY otherwise clears all five btypes slots.

### global:RunTimeDemo

STRONG: Demo runtime allowance initialized to1800.

Units: demo update calls. DOS layout/address: 2:3f768.

The display/demo path consumes it separately from record.length and map_time.

### global:SauveRayEvts

PROVEN: Saved complete Rayman ability-bit record for squash restoration.

Units: bitfield snapshot. DOS layout/address: 2:3f818.

Ray_RayEcrase saves before disabling abilities and restores on leaving squash.

### global:SizeScreen

STRONG: Screen-size setting used with video mode to disable horizontal scrolling on narrow maps.

Units: setting code. DOS layout/address: 2:5e6a.

INIT_MOTEUR tests SizeScreen==0 or ModeX when mp.width<=20.

### global:TextDark2_Affiche

STRONG: Final-boss text-display state cleared at engine initialization.

Units: display state. DOS layout/address: 2:3fa89.

Other dark/display readers/writers are retained; no enum labels assigned.

### global:Vignet_To_Display

STRONG: Pending vignette/display selector cleared at engine initialization.

Units: display selector. DOS layout/address: 2:3fa44.

Full selection meanings belong to menu/display code.

### global:You_Win

STRONG: Victory/credits progression flag cleared for a new session.

Units: boolean. DOS layout/address: 2:3fa46.

Credits consumers determine the later progression; not equivalent to fin_du_jeu.

### global:allowed_time

PROVEN: Six32-entry world rows of timed-bonus limits; -2 denotes no timed bonus stage.

Units: nominal seconds or -2. DOS layout/address: 2:3b9c0.

init_allowed_time explicitly populates all192 entries; calc_left_time multiplies positive values by60.

### global:bateau_obj_id

PROVEN: Ship object id or-1 when absent.

Units: object id / -1. DOS layout/address: 2:3f808.

deactivate_ship_links clears alive on its other linked members only when the group marker is set.

### global:bonus_map

PROVEN: Whether the current level has a timed-bonus limit.

Units: boolean. DOS layout/address: 2:3f806.

INIT_MOTEUR_LEVEL derives it from get_allowed_time()!=-2 before checkpoint decisions.

### global:boss_mort

STRONG: Distinct boss-death, boss-finish and game-finish controls.

Units: sequence flags. DOS layout/address: 2:3fa78.

Engine initialization clears boss_mort/fin_boss; fin_du_jeu suppresses level initialization and changes dead_time setup. Keep the globals distinct.

### global:button_released

PROVEN: Rayman jump/helicopter press-release phase latch.

Units: phase code. DOS layout/address: 2:3fa7d.

Other ray.c readers/writers retain the input-release contract; this engine site forces1 then calls ray_jump.

Pass3 refinement: Normal grounded release writes1; jumping writes0; airborne unpressed even value increments to odd. Helicopter activation1->2, later release2->3 and pressed stop3->4. ray_jump tests odd parity, not equality with1; do not reduce to boolean.

### global:compteur_attente

STRONG: Idle/wait animation counter reset during Rayman initialization.

Units: update calls. DOS layout/address: 2:3f774.

Change_Wait_Anim and Rayman idle consumers establish the counter role.

### global:current_pal_id

STRONG: Current palette-selection state used by palette actualization.

Units: palette selector. DOS layout/address: 2:3fa82.

Checkpoint return writes pal_id_at_wizard+1 then actualize_palette(pal_id_at_wizard), preserving its force-refresh convention.

### global:dans_la_map_monde

PROVEN: Worldmap-context flag influencing horizontal camera-follow smoothing.

Units: boolean. DOS layout/address: 2:3fa53.

Limits |dhspeed| to4 here; engine/session initialization clears it.

### global:dead_time

PROVEN: Context-dependent byte death/drowning countdown shared with the fade/death lifecycle.

Units: handler calls / byte countdown. DOS layout/address: 2:3fa7e.

Normal initialization uses64, the dead-player branch decrements when nonzero, and INIT_MOTEUR_DEAD resets the engine at0.

Pass3 refinement: Drowning and spikes set1; RAY_DEAD decrements only2:9 or EOA-qualified3:22/32, then writes64 on retirement. DO_MORT_DE_RAY writes128.0 underflows255 if decremented; no milliseconds duration is inferred.

### global:decalage_en_cours

PROVEN: Rayman horizontal momentum accumulator, converted through two four-bit stages to pixel motion.

Units: 1/256 pixel before phase quantization. DOS layout/address: 2:3f866.

Not a direct pixel displacement in recale_ray_pos; its sign selects target, and collision/clamp paths reset it.

Pass3 refinement: ray_inertia_speed computes instantSpeed(ashr16(momentum,4)); instantSpeed performs the second4-bit conversion. RAY_SWIP passes selected eta speed<<4. Preserve signed16 intermediate wrap and decay after extracting pixel speed.

### global:departlevel

PROVEN: Entry/return branch marker used by checkpoint, bonus and level-start initialization.

Units: entry state. DOS layout/address: 2:3f8a0.

Zero selects saved wizard-return position/camera; INIT_MOTEUR_LEVEL subsequently restores flags and sets it1. It is not the current level number.

### global:dhspeed

PROVEN: Signed16 horizontal camera-follow smoothing accumulator in quarter-error units.

Units: camera error accumulator. DOS layout/address: 2:2520.

Approaches ashr16(screen_error,2) by one; ashr16(dhspeed,2) contributes to actual h_scroll_speed. Clamp to[-4,4] in worldmap mode; reset on x clamp.

### global:display_mode

STRONG: Display-mode control whose value adjusts the inclusive death-fade upper threshold.

Units: mode code. DOS layout/address: 2:3f7e8.

fades uses dead_time in[32,display_mode+33]. Other display consumers are retained as evidence; do not assume a boolean.

### global:dontdoit

STRONG: Worldmap end-sequence one-shot latch.

Units: boolean marker. DOS layout/address: 2:3fa43.

Worldmap checks !dontdoit at a selected level branch, sets1 after its action and resets elsewhere; engine session resets0.

### global:dvspeed

UNKNOWN: Signed16 vertical camera state slot cleared at engine initialization and on vertical camera clamp.

Units: unknown. DOS layout/address: 2:2522.

No nonzero writer or computational consumer appears in the whole-source scan; the name alone does not establish a vertical analogue of dhspeed.

### global:eau_obj_id

PROVEN: Water object id or-1 when absent.

Units: object id / -1. DOS layout/address: 2:3f882.

Auto-scroll mode3 reads its hit_points/param and can set its iframes_timer=2; these are type-specific state uses.

### global:expsin

PROVEN: Signed16 sampled vertical-shake table, divided by128 by frapsol.

Units: 1/128 pixel samples. DOS layout/address: 2:4c14.

The terminal32640 becomes positive255; this value is a control marker, not a displacement.

### global:expsin2

PROVEN: Signed16 unscaled screen-shake sample table shared by horizontal and vertical shake indices.

Units: pixels or255 terminator. DOS layout/address: 2:4c92.

A positive255 stops the sequence; negative samples shift the camera normally.

### global:fade

STRONG: Fade controller byte carrying phase and a bit0x40 completion/inactivity condition.

Units: fade state. DOS layout/address: 2:3fa87.

fades starts fade-out on65 and calls DO_FADE only while bit0x40 is clear; full palette-step rules belong to the display implementation.

### global:fee_obj_id

STRONG: Fairy object id or-1 when absent.

Units: object id / -1. DOS layout/address: 2:3f888.

Level teardown stops recorded playback when a fairy exists.

### global:fin_boss

STRONG: Distinct boss-death, boss-finish and game-finish controls.

Units: sequence flags. DOS layout/address: 2:3fa76.

Engine initialization clears boss_mort/fin_boss; fin_du_jeu suppresses level initialization and changes dead_time setup. Keep the globals distinct.

### global:fin_continue

STRONG: Continuation/entry flag gating engine reset, checkpoint writes and start-marker player hiding.

Units: boolean marker. DOS layout/address: 2:3fa5c.

Different initialization branches inspect it; preserve its branch role without reducing it to fin_du_jeu.

### global:fin_dark

STRONG: Final-boss sequence state cleared during Rayman initialization.

Units: sequence state. DOS layout/address: 2:3fa3c.

Full progression belongs to dark.c; no state labels invented here.

### global:fin_de_rayman

PROVEN: Main game/application exit flag controlling the outer PcMain loop.

Units: boolean. DOS layout/address: 2:3fa74.

PcMain loops while clear; menu/display exit branches set1 and INIT_MOTEUR_BEGIN clears it.

### global:fin_du_jeu

STRONG: Distinct boss-death, boss-finish and game-finish controls.

Units: sequence flags. DOS layout/address: 2:3fa7a.

Engine initialization clears boss_mort/fin_boss; fin_du_jeu suppresses level initialization and changes dead_time setup. Keep the globals distinct.

### global:gele

PROVEN: Engine freeze-mode control; value2 advances a restricted timed subset and3 skips that subset.

Units: mode code. DOS layout/address: 2:3fa62.

DO_MOTEUR_GELE always rebuilds the active table, including mode3. It is not a universal pause boolean.

### global:gerbe

STRONG: Global event marker that changes cage/LidolPink bounce and landing handling.

Units: boolean marker. DOS layout/address: 2:3fa70.

Set by two Rayman ground-impact paths and reset by engine/save initialization. Exact shared player action terminology remains unassigned.

### global:h_scroll_speed

PROVEN: Requested horizontal camera displacement for the current engine phase.

Units: pixels/call. DOS layout/address: 2:3f8b2.

Recomputed/augmented by player follow and auto-scroll; DO_SCROLL adjusts it by any clamp correction.

### global:helico_time

PROVEN: Helicopter duration/impulse countdown advanced by the airborne handler.

Units: airborne update calls. DOS layout/address: 2:3f898.

Reset by INIT_MOTEUR; full helico consumption belongs to ray.c.

Pass3 refinement: -1 disables decrement; ordinary activation writes50, super paired impulse70,0 is handled by RAY_HELICO before it can be decremented again. Other nonair paths do not necessarily advance it.

### global:id_Cling_1up

PROVEN: Current extra-life/power pickup animation object ids, respectively, or-1 when absent.

Units: object id / -1. DOS layout/address: 2:3f82c.

DO_CLING_ANIMS retires the id on clock-gated EOA; only the extra-life animation can award a life.

### global:id_Cling_Pow

PROVEN: Current extra-life/power pickup animation object ids, respectively, or-1 when absent.

Units: object id / -1. DOS layout/address: 2:3f846.

DO_CLING_ANIMS retires the id on clock-gated EOA; only the extra-life animation can award a life.

### global:in_air_because_hit

PROVEN: Marker that airborne gravity must not be suppressed by holding jump after a hit.

Units: boolean. DOS layout/address: 2:3fa8c.

INIT_RAY clears it; ray.c hit/air logic supplies its writers/readers.

Pass3 refinement: RAY_HIT writes1; ray_jump and tile landing clear0. RAY_IN_THE_AIR includes it in the OR condition calling DO_PESANTEUR. Support acquisition differs from the tile-landing clear path.

### global:in_pause

STRONG: Pause control cleared during engine initialization.

Units: boolean. DOS layout/address: 2:3fa52.

The frozen-engine gele modes are separate from this control.

### global:joe_exp_probleme

STRONG: Joe interaction progression latch initialized from helped_joe_2.

Units: boolean marker. DOS layout/address: 2:3f786.

Joe callback tests it before the initial interaction and sets it1; full dialogue terminology not inferred.

### global:jump_time

PROVEN: Number of airborne handler calls since the last jump/fall/hit reset.

Units: airborne update calls. DOS layout/address: 2:3f8a4.

Rayman jump processing supplies its nonzero progression.

Pass3 refinement: RAY_IN_THE_AIR preincrements; tests6 for departure animation changes,23 for extra speed_y increment, >=3 for landing eligibility, >8 for ledge catch, >10 vine catch and >12 to end held-jump gravity suppression. Not milliseconds or all engine ticks.

### global:left_time

PROVEN: Timed-bonus remaining tick counter, with-2 denoting no timed limit.

Units: nominal60-Hz ticks / -2. DOS layout/address: 2:3d8c4.

calc_left_time starts from60*allowed_time and subtracts map_time-120 after the entry delay; INIT_MOTEUR starts-2.

### global:lidol_source_obj

PROVEN: Object source for a deferred allocate_toons call.

Units: pointer. DOS layout/address: 2:3d8c8.

Read only while lidol_to_allocate is nonzero; engine uses count7 then clears the request.

### global:lidol_to_allocate

PROVEN: Pending deferred toon-allocation request flag.

Units: boolean marker. DOS layout/address: 2:3fa66.

DO_OBJECTS calls allocate_toons(lidol_source_obj,7) once and clears it; INIT_MOTEUR cancels it.

### global:life_becoz_wiz

STRONG: Marker that the current tings progression awarded an extra life.

Units: boolean. DOS layout/address: 2:3fa99.

Engine/session/death paths clear it; collection consumers establish the award role.

### global:link_init

PROVEN: Live circular-successor array indexed by object id; a singleton points to itself.

Units: object ids. DOS layout/address: 2:3ee58.

Traversal stops on returning to the starting id, not on-1. Door detach saves the successor in obj.link; save_state copies16-bit entries but restore_gendoor_link truncates one path to u8.

### global:loop_time

STRONG: Bonus/postbonus sequence counter with both negative preparation and positive display phases.

Units: sequence calls. DOS layout/address: 2:3f810.

Engine reset0; bonus code uses exact500 stop and input after50, not an unbounded elapsed hardware timer.

### global:main_mem_level

PROVEN: Level allocation arena with base pointer, bump offset and total capacity.

Units: arena record. DOS layout/address: 2:3d84c.

DONE_MOTEUR_LEVEL calls align_mem_desc, which resets bump offset to base&3. align is not an alignment constant.

### global:map_time

PROVEN: Signed32 global map-time counter, advanced by the byte horloges argument and also used as the common fractional movement phase.

Units: nominal map ticks. DOS layout/address: 2:3d8c0.

Not a monotonic hardware time source: initialization clears it, ray_inertia_speed can adjust odd phases, and weather warm-up calls horloges(0). Bonus arithmetic uses60 ticks per displayed second.

### global:nb_continue

STRONG: Remaining continues counter.

Units: continues. DOS layout/address: 2:3fa3b.

New session initializes9; keep separate from lives and fin_continue.

### global:nb_wiz_collected

PROVEN: Bonus tings count and pending transfer count consumed into the HUD after leaving the bonus map.

Units: tings. DOS layout/address: 2:3fa55.

DO_WIZ_AFTER_BONUS_MAP transfers one ting on horloge[4]==0 outside a bonus map. Reset on fresh level entry; distinct from status_bar.num_wiz and nb_wiz.

### global:new_level

PROVEN: Pending level/world transition flags.

Units: boolean flags. DOS layout/address: 2:3f880.

INIT_MOTEUR_BEGIN sets both; world entry clears new_world and sets new_level; level initialization clears new_level.

### global:new_world

PROVEN: Pending level/world transition flags.

Units: boolean flags. DOS layout/address: 2:3f892.

INIT_MOTEUR_BEGIN sets both; world entry clears new_world and sets new_level; level initialization clears new_level.

### global:no_ray_landing_smoke

STRONG: Flag suppressing the player landing-smoke path.

Units: boolean. DOS layout/address: 2:3fa79.

Reset on Rayman initialization; this does not suppress allocateLandingSmoke for all object types.

### global:num_level

PROVEN: Current one-based world/level selector used by engine exceptions, time tables and spawn rules.

Units: world/level index. DOS layout/address: 2:3f87c.

World time-table row is32 entries; do not infer level-file capacity from that stride.

### global:num_level_choice

STRONG: Selected world/level identifiers used by deferred entry/ability decisions.

Units: world/level index. DOS layout/address: 2:3f82e.

Distinct from current num_world/num_level; firefly grant checks selected Cave5/4.

### global:num_world_choice

STRONG: Selected world/level identifiers used by deferred entry/ability decisions.

Units: world/level index. DOS layout/address: 2:3f83a.

Distinct from current num_world/num_level; firefly grant checks selected Cave5/4.

### global:numero_palette_special

STRONG: Special weather palette selection state cleared during frozen weather update.

Units: palette state. DOS layout/address: 2:3f740.

Other display/weather consumers determine nonzero codes.

### global:oldPrio

PROVEN: Saved original display priority of Rayman followed object.

Units: display priority. DOS layout/address: 2:54da6.

DO_MOTEUR2 saves/caps at3; the next DO_MOTEUR restores before object update. It is not the previous frame global render priority.

### global:ot

PROVEN: Current object-type dispatch cursor shared by engine helper calls.

Units: object type id. DOS layout/address: 2:54da4.

SET_ACTIVE_FLAG sets it before zone handling; DO_OBJECTS sets it before DO_ONE_OBJECT. Some helpers use ot instead of rereading obj.type, so preserve call ordering.

### global:pal_id_at_wizard

STRONG: Saved palette selector for the wizard/bonus return branch.

Units: palette selector. DOS layout/address: 2:3fa84.

INIT_MOTEUR forces palette refresh using this selector.

### global:pierreAcorde_obj_id

STRONG: Object id of the rope/stone counterpart used by action-zone special cases.

Units: object id. DOS layout/address: 2:3f88e.

TYPE_CORDE and TYPE_PIERREACORDE paths access this id directly; absence/availability belongs to the loaded level invariant.

### global:pix_gerbe

PROVEN: Eight particle batches with word-clear aliases and per-batch signed16 active markers.

Units: particle records. DOS layout/address: 2:39a80.

Engine initialization clears each batch marker and all320 words of its item storage; per-particle update lives in effect handling.

### global:pixels_enfonce

STRONG: Skops floor/camera sinking progress in pixels, reset at engine initialization and at scripted thresholds.

Units: pixels. DOS layout/address: 2:3eef4.

skops increments and decrements mp.height each16 pixels; thresholds96/196 belong to its two enable states.

### global:poing_obj

PROVEN: Pointer to the live fist object.

Units: pointer. DOS layout/address: 2:3e6dc.

Mounted DO_MOTEUR2 offsets it by camera speeds in addition to ordinary fist movement.

### global:pos_stack

PROVEN: Ten-entry shift history of Rayman recent horizontal positions.

Units: pixels, narrowed s16. DOS layout/address: 2:3e5b8.

RAY_IN_THE_AIR shifts entries1..9 with memmove and writes ray.x at0; landing uses unequal adjacent positions to choose inertia. This is distinct from the rayStack ring.

### global:rayOnMsWasThere

UNKNOWN: Marker tested by the absent-mounted-template fallback in INIT_RAY_ON_MS.

Units: unknown. DOS layout/address: 2:3fa49.

Whole-source scan finds a clear in special.c and this read, but no live nonzero writer. Preserve observed branch without inventing a mount-history meaning.

### global:ray_Suphelico_bis

STRONG: Special super-helico state marker exempting one airborne substate from the ordinary airborne camera rule.

Units: boolean marker. DOS layout/address: 2:3fa75.

Used with main state2/substate15 in recale_ray_pos; other ray.c writers/readers supply the specialized helico state.

### global:ray_X_main_pos_init

STRONG: Saved Rayman world origin used on the wizard/bonus return branch.

Units: pixels, s16. DOS layout/address: 2:3f8aa.

INIT_MOTEUR restores them when departlevel is zero.

### global:ray_Y_main_pos_init

STRONG: Saved Rayman world origin used on the wizard/bonus return branch.

Units: pixels, s16. DOS layout/address: 2:3f8a2.

INIT_MOTEUR restores them when departlevel is zero.

### global:ray_in_fee_zone

STRONG: Rayman fairy-zone interaction marker.

Units: boolean. DOS layout/address: 2:3fa77.

Reset by INIT_RAY; fairy detection/interaction is outside this unit.

### global:ray_old_main_etat

PROVEN: Rayman state snapshots captured before the current DO_OBJECTS/DO_ANIM phase.

Units: state index. DOS layout/address: 2:3f8c4.

DO_ANIM uses them for iframe clamping at selected state exits; these are not object-generic previous state fields.

### global:ray_old_sub_etat

PROVEN: Rayman state snapshots captured before the current DO_OBJECTS/DO_ANIM phase.

Units: state index. DOS layout/address: 2:3f8d4.

DO_ANIM uses them for iframe clamping at selected state exits; these are not object-generic previous state fields.

### global:ray_pos_in_stack

PROVEN: Next Rayman replay-history ring write index.

Units: ring index. DOS layout/address: 2:3fa8b.

stackRay increments/wraps this index and sets ray_stack_is_full; unrelated to the ten-entry pos_stack shift history.

Pass3 refinement: stackRay writes then increments and wraps after100; full flag latches1. Black consumer cursor comparison uses promoted ray_pos_in_stack-1, preserving the cursor0 boundary quirk.

### global:ray_se_noie

STRONG: Rayman drowning-state marker.

Units: boolean marker. DOS layout/address: 2:3fa7b.

Cleared at initialization; water/player state consumers are the meaning evidence.

### global:ray_stack_is_full

PROVEN: Latch indicating the101-entry Rayman replay ring has wrapped.

Units: boolean. DOS layout/address: 2:3fa88.

Set by stackRay on wrap, reset during INIT_RAY and final-boss setup; not a marker for pos_stack.

Pass3 refinement: stackRay sets1 on first write wrap; doBlackRaymanCommand and Black collision use it. Initialization clears it; no claim of a fixed wall-clock delay.

### global:ray_wind_force

STRONG: Horizontal wind contribution selected for Rayman for the current engine phase.

Units: player horizontal force units. DOS layout/address: 2:3f870.

DO_MOTEUR copies weather_wind except in mountain world3, where it writes0; full force scale belongs to player inertia.

### global:ray_zdc_y

PROVEN: Cached Rayman collision-box height span, despite the y suffix.

Units: pixels. DOS layout/address: 2:3f876.

recale_ray_pos uses (ray_zdc_y>>1)-offset_by+114; collision code supplies the value, despite its y-oriented name.

Pass3 refinement: DO_RAYMAN/DO_RAY_ON_MS supply it as GET_RAY_ZDC out_h; y position is ray_zdc_h. Collision and fist callers prove the reversed legacy names. The existing camera half-height interpretation is confirmed.

### global:rayman_obj_id

STRONG: Mounted/alternate Rayman object template id or-1 when unavailable.

Units: object id / -1. DOS layout/address: 2:3f894.

INIT_RAY_ON_MS copies it into live ray; DONE_MOTEUR_LEVEL uses its presence during mode restoration.

### global:raytmp

PROVEN: Temporary full Rayman object snapshot used during object-template swaps.

Units: object record. DOS layout/address: 2:3ee5c.

Whole-record assignments are intentional; field-copy subsets subsequently restore selected live state.

### global:record

PROVEN: Global demo-input stream descriptor and recording/playback controls.

Units: record descriptor. DOS layout/address: 2:3af50.

INIT_RAY reads recording to retain seed power; level teardown may stop playback.

### global:reduced_rayman_id

STRONG: Level object-template id for tiny/squashed Rayman swaps.

Units: object id. DOS layout/address: 2:3f886.

RAY_DEMIRAY/Ray_RayEcrase copy that record and restore a live subset.

### global:remoteRayXToReach

STRONG: Remote horizontal target used by player scripted movement, with-32000 for no target.

Units: pixels / -32000. DOS layout/address: 2:3f86a.

INIT_RAY stores the sentinel; consumers in ray.c/objupdate2 distinguish it.

### global:rms

PROVEN: Saved alternate/full-size or mounted Rayman object snapshot used by template swaps.

Units: object record. DOS layout/address: 2:3edd4.

Role varies with tiny/squashed/mounted paths; do not rename it to a single fixed player form.

### global:rubis_list_calculated

STRONG: Whether Skops ruby-object setup/list preparation has run.

Units: boolean. DOS layout/address: 2:3f8ec.

Set by Skops setup paths, reset by INIT_MOTEUR; no inferred declaration capacity.

### global:save1

PROVEN: Primary checkpoint image used during normal entry and generating-door link restoration.

Units: checkpoint record. DOS layout/address: 2:3e88c.

save_obj_id==-1 means no checkpoint object; is_just_saved gates restore. Live links are16-bit, but one door path truncates them to a byte.

### global:save2

STRONG: Secondary checkpoint snapshot recorded by the death-reinitialization path.

Units: checkpoint record. DOS layout/address: 2:3eae8.

INIT_MOTEUR_DEAD writes it on eligible nonbonus/departlevel calls.

### global:save_save

STRONG: Saved primary-checkpoint valid marker for the wizard return path.

Units: boolean marker. DOS layout/address: 2:3fa85.

INIT_MOTEUR copies it into save1.is_just_saved when departlevel==0.

### global:screen_trembling

PROVEN: Index into the scaled vertical-shake table; zero disables its update.

Units: sample index. DOS layout/address: 2:3f852.

DO_SCREEN_TREMBLE increments then clears at scaled sample255. Nonzero also changes vertical camera-follow behaviour.

### global:screen_trembling2

PROVEN: Index into expsin2 for horizontal camera shake.

Units: sample index. DOS layout/address: 2:3f84e.

DO_SCROLL invokes its updater only while the signed16 index is positive; sample255 resets it.

### global:screen_trembling3

PROVEN: Index into expsin2 for the additional vertical camera shake.

Units: sample index. DOS layout/address: 2:3f84c.

DO_SCROLL invokes its updater only while the signed16 index is positive; sample255 resets it.

### global:scrollLocked

STRONG: Camera/boss-region lock marker read by activation and background/camera code.

Units: boolean. DOS layout/address: 2:3fa68.

Set by boss handlers, cleared by engine/object initialization; do not equate with both scroll axis bounds being equal.

### global:scroll_end_x

PROVEN: Current lower/upper horizontal camera-origin bound, respectively.

Units: pixels. DOS layout/address: 2:3f872.

DO_SCROLL clamps xmap; level/background/boss logic may make the two equal to lock x scrolling.

### global:scroll_end_y

PROVEN: Current lower/upper vertical camera-origin bound, respectively.

Units: pixels. DOS layout/address: 2:3f85e.

DO_SCROLL clamps ymap. Follow and upward player-bound code read these independently of ymapmax.

### global:scroll_obj_id

PROVEN: Level-object ids of auto-scroll markers, in level initialization order.

Units: object ids. DOS layout/address: 2:3e5d0.

DO_AUTO_SCROLL interprets object hit_points as mode0..3 and mutates marker coordinates to consume thresholds.

### global:scroll_start_x

PROVEN: Current lower/upper horizontal camera-origin bound, respectively.

Units: pixels. DOS layout/address: 2:3f86e.

DO_SCROLL clamps xmap; level/background/boss logic may make the two equal to lock x scrolling.

### global:scroll_start_y

PROVEN: Current lower/upper vertical camera-origin bound, respectively.

Units: pixels. DOS layout/address: 2:3f854.

DO_SCROLL clamps ymap. Follow and upward player-bound code read these independently of ymapmax.

### global:scroll_x

PROVEN: Horizontal auto-scroll control: -1 ordinary follow, positive forced right amount, values<-1 encode forced left amount+1.

Units: control code / pixels. DOS layout/address: 2:3f8d2.

DO_MOTEUR2 temporarily reuses this slot to save ray.speed_x in mounted auto-scroll; avoid imposing a single persistent enum type.

### global:scroll_y

PROVEN: Vertical scroll control: -1 ordinary follow,1 forced upward,2 alternating upward, transient-2 normalized back to-1.

Units: control code. DOS layout/address: 2:3f8ba.

DO_AUTO_SCROLL recomputes it from type64 marker hit_points modes; it is not a vertical position.

### global:sko_enfonce_enable

STRONG: Skops floor-sinking phase selector0/1/2.

Units: phase code. DOS layout/address: 2:3fa8f.

Phase1 targets96 pixels on period2; phase2 targets196 on period4. Reset to0 by engine initialization.

### global:sko_last_action

STRONG: Skops previous action choice used to alternate attack selections.

Units: action code. DOS layout/address: 2:3f8dc.

Skops compares4/5 and writes8 or the corresponding action; moteur resets0.

### global:sko_rayon_on

STRONG: Skops beam-related counter/control using0, timed countdowns and255 special state.

Units: updates / control state. DOS layout/address: 2:3f8de.

Skops writes60/120/255 and decrements in selected states; engine resets0. Not a boolean.

### global:special_ray_mov_win_x_left

PROVEN: Additional left/right offsets in the horizontal camera-follow target window.

Units: pixels. DOS layout/address: 2:3f8a8.

Targets are112-offset_bx-left and208-offset_bx+right. Session initialization clears both.

### global:special_ray_mov_win_x_right

PROVEN: Additional left/right offsets in the horizontal camera-follow target window.

Units: pixels. DOS layout/address: 2:3f8ae.

Targets are112-offset_bx-left and208-offset_bx+right. Session initialization clears both.

### global:star_ray_der

STRONG: Optional reverse-control star-effect objects behind/in front of Rayman, respectively.

Units: nullable object pointers. DOS layout/address: 2:3e6d8.

RAY_REVERSE_COMMANDS toggles alive/active and assigns explicit priorities3/1 when enabled,3/3 when disabled.

### global:star_ray_dev

STRONG: Optional reverse-control star-effect objects behind/in front of Rayman, respectively.

Units: nullable object pointers. DOS layout/address: 2:3e6d4.

RAY_REVERSE_COMMANDS toggles alive/active and assigns explicit priorities3/1 when enabled,3/3 when disabled.

### global:status_bar

PROVEN: HUD live counters and derived digit/health display selectors.

Units: HUD record. DOS layout/address: 2:3ef52.

Engine phases update health/lives/tings and call DO_FIXE to derive display slots.

### global:type_fndscrX

STRONG: Background-mode marker forcing horizontal camera bounds to the scroll-start position when nonzero.

Units: mode marker. DOS layout/address: 2:3fa24.

Set by background setup; INIT_MOTEUR collapses scroll_end_x to scroll_start_x.

### global:v_scroll_speed

PROVEN: Requested vertical camera displacement or special positive255 control state.

Units: pixels/call or255. DOS layout/address: 2:3f8b4.

DO_SCROLL interprets255 as zero; recale_ray_pos distinguishes255 before ordinary follow. Never store this as u8 or bool in the port.

### global:weather_wind

STRONG: Weather-provided horizontal wind state.

Units: wind units. DOS layout/address: 2:3f874.

INIT_MOTEUR resets it; DO_MOTEUR forwards it to ray_wind_force outside world3.

### global:xmap_old

PROVEN: Prior camera coordinate snapshot for weather displacement.

Units: pixels. DOS layout/address: 2:3f8b0.

DO_MOTEUR captures these before movement; do_flocons receives old and current camera coordinates.

### global:xmapmax

PROVEN: Maximum horizontal camera origin used by world-size and movement-bound tests.

Units: pixels. DOS layout/address: 2:3f890.

Nonnormal engine initialization computes max(0,16*mp.width-305), distinct from scroll_end_x which can be tightened.

### global:xmapsave

STRONG: Saved camera origin restored on wizard/bonus return.

Units: pixels. DOS layout/address: 2:3f8b6.

Distinct from per-frame xmap_old/ymap_old snapshots.

### global:ymap_old

PROVEN: Prior camera coordinate snapshot for weather displacement.

Units: pixels. DOS layout/address: 2:3f8ac.

DO_MOTEUR captures these before movement; do_flocons receives old and current camera coordinates.

### global:ymapmax

PROVEN: Maximum vertical camera origin used for world-size/initial placement.

Units: pixels. DOS layout/address: 2:3f884.

Nonnormal initialization computes max(0,16*mp.height-200); Cave5/10 overrides it to312.

### global:ymapsave

STRONG: Saved camera origin restored on wizard/bonus return.

Units: pixels. DOS layout/address: 2:3f8b8.

Distinct from per-frame xmap_old/ymap_old snapshots.

### global:zonediffx

PROVEN: Per-type horizontal/vertical action-zone expansion margins.

Units: pixels. DOS layout/address: 2:2996.

in_action_zone adds60 to local margins for a previously active object; special_flags_init changes table entries by world/level.

### global:zonediffy

PROVEN: Per-type horizontal/vertical action-zone expansion margins.

Units: pixels. DOS layout/address: 2:2ba2.

in_action_zone adds60 to local margins for a previously active object; special_flags_init changes table entries by world/level.

## ray.json

DOS GOG1.21 operations/consumers are primary; STRICT is independent of semantic confidence. PROVEN locals identify observed dataflow, not recovered historical C names. NEAR computational contracts use STRONG unless separately proved. Cross-build names only corroborate. Neutral unused or multipurpose storage is not renamed.

### function:allocateRayLandingSmoke

PROVEN: Allocate the landing explosion effect unless the suppression byte is set.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5bfd0.

A successful allocation is switched to0:1 and positioned; allocation failure is allowed.

### function:recale_ray_on_liane

PROVEN: Align Rayman's horizontal anchor to the centre of the detected vine tile.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5c004.

Chooses the centre/left/right hand sample; masks its pixel x by0xfff0 then adds8-offset_bx. The declared unused temporary has no semantic role.

### function:calc_bhand_typ

PROVEN: Sample the centre and adjacent hand tiles for vine attachment.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5c088.

Pixel anchor x=x+offset_bx; byte offset_hy+22+speed_y wraps before the tiny transform80-((80-offset)>>1). Side samples are x+-8. A centre vine12 forces both sides to15.

### function:IS_RAY_ON_LIANE

PROVEN: Attach airborne Rayman to a vine after the jump delay, or count down the reattachment delay.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5c138.

Requires jump_time>10 and force_run0; suppresses sampling while airborne timer!=0. Saves/restores speed_y for zero-speed sampling. Success sets4:1 and clears movement; leaving hurt2:8 gives90 invulnerability.

### function:rayMayLandOnAnObject

STRONG: Find a nearby active support surface and transfer Rayman onto it.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5c224.

Ordered active list; requires follow_enabled, excludes the supplied id, uses strict abs(ray_dist)<max(abs(support vertical motion+follow_y-ray.speed_y)+2,tiny?8:4), rejects overhead flag0x10. Writes the caller byte0 only on acquisition. Restores excluded support id if no replacement is found. Tile and object landing thresholds differ at200.

### function:set_air_speed

PROVEN: Rewrite one airborne state's left/right speed limits using the current horizontal motion.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5ca70.

Magnitude narrows to signed8 after abs(momentum)>>4, then min112; opposite-facing limit remains16. param_4 is the minimum limit. Preserve this narrowing order.

### function:Reset_air_speed

PROVEN: Rewrite the normal or rolling set of airborne state speed limits.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5caf8.

Rolling states17/18/19/3/5/4/32 use minimum48; normal0/1/2/24/11/12/13/3/5/4 use32. Edits shared eta records.

### function:determineRayAirInertia

STRONG: Select whether the next airborne motion retains horizontal inertia.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5cce0.

Wind!=0 sets nb_cmd1. With no support, ground class0 preserves inertia only above256 magnitude; ground class1 or any support clears it. Other classes leave the slot unchanged.

### function:ray_jump

STRONG: Initiate a permitted jump and detach Rayman from support.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5cd40.

Requires current eta bit0 and button_released odd. Initial vertical speed is -5 normally, -3 from vines, or current speed-2 from swing. Supporting follow_y and slope groups modify it; tiny halves then subtracts1. Selects2:0 or2:17, resets counters/support/charge, records low16 x/y.

### function:ray_inertia_speed

STRONG: Integrate and decay Rayman's horizontal momentum, then derive pixel motion.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5d020.

Byte coefficient0 copies prior speed directly. Otherwise combines prior*coefficient>>8, slope<<2, wind<<3; signed16 saturation targets and a normalized coefficient>>3 decay. pixel speed=instantSpeed(momentum>>4), so momentum has1/256 pixel scale before both stages. Decay occurs after pixel-speed extraction.

### function:RAY_SWIP

STRONG: Select horizontal inertia parameters from Rayman's state, support, surface and wind.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5d2e8.

Releases invalid/out-of-range support, writes departure states, obtains eta speed, reverses crawl variants, and calls ray_inertia_speed. Coefficients differ in world3; pan halves the chosen coefficient. Signed slope-platform hit_points drives acceleration.

### function:RAY_STOP

PROVEN: Switch locomotion or vine movement to its stationary counterpart.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5d724.

Locomotion0/3/4/5/7 ->0:0; forward crawl8/10 ->0:47; backward9/11 ->0:48. Clears actual speeds only if momentum0. Vine4 stops at sub0 unless charging.

### function:RAY_HELICO

STRONG: Handle airborne helicopter activation, stopping and super-helicopter retriggers.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5d7d4.

Eta bit2, release state1 and held jump activate2:3 for50 airborne calls, or super2:14 with timer-1. Release state3/held jump or timer0 handles stopping/retrigger. Super2:15 edits gravity high nibble and20-call phase; double retrigger can set70 and horizontal+-48.

### function:Make_Ray_Hang

STRONG: Snap Rayman to a tile ledge and enter hanging state5:0.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5db1c.

Uses16-pixel grid; y subtracts projected32 plus37 when tiny. x uses facing-dependent3/7 inset. Clears support, momentum, speeds, gravity and jump counter; interrupted hurt gets90 invulnerability.

### function:AIR

PROVEN: Test absence of the tile support flag(mask2) at a linear map-tile index.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5dc24.

The argument is a tile-array index, not pixels or x/y coordinates; returns !block_flags[tile_type].solid.

### function:MUR

PROVEN: Test the tile support flag(mask2) at a linear map-tile index.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5dc4c.

Returns the raw one-bit solid value; caller supplies a valid tile-array index.

### function:CAN_RAY_HANG_BLOC

PROVEN: Check the ledge's solid and empty tile pattern before snapping to it.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5dc78.

Requires hang capability and force_run0; x tile in1..width-2, airborne and jump_time>8. Checks adjacent wall and a vertical air pattern, with a shifted retry for full-size Rayman. Scaling changes the sampled vertical extent.

### function:RAY_TOMBE

PROVEN: Enter an airborne departure state without applying a fresh jump impulse.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5df28.

Selects pan2:26/28, hanging2:1, otherwise2:24 or2:32 by abs(speed_x)<=2; resets jump/helico/support/gravity/release/charge and recomputes inertia.

### function:RAY_RESPOND_TO_DOWN

PROVEN: Apply the down input according to Rayman's main state.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5e030.

Vines descend+1 or detach; selected stationary states enter3:6; moving may crouch/stop; ledge5 drops y+14 before falling; mounted6 accelerates downward up to3 on phase0.

### function:RAY_RESPOND_TO_UP

STRONG: Apply the up input according to Rayman's main state.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5e370.

Vines ascend-1, detach at empty tile with timer10, or stop at obstruction. Ground/ledge can request camera value255; mounted6 accelerates upward to-3 on phase0.

### function:RAY_RESPOND_TO_DIR

STRONG: Apply horizontal input, including facing, crouch/crawl transitions and mount motion.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5e548.

flip_x0 is left,1 right. Horizontal processing increments joy_done in supported main states. Airborne motion may clear momentum before RAY_SWIP; vines permit side jumping when release is odd; turning away from a ledge drops support.

### function:RAY_RESPOND_TO_NOTHING

PROVEN: Apply no-direction state transitions, idle animation cycling and motion decay.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5eb40.

Main0 increments idle counter for sub0/1/2 and cycles59,64,62,63,65 at500 then subtracts100. Main1 stops unless forced/turning; vine stops; air uses coefficient8 or0; mount speeds decay toward0 on phase0.

### function:RAY_RESPOND_TO_BUTTON4

PROVEN: Empty action callback in the original Rayman unit.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f0ac.

No observable operation; retain shared RET/source-order matching boundary.

### function:RAY_RESPOND_TO_BUTTON3

STRONG: Handle action-button run, seed and ground-action selection.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f0b0.

Standing eligible states enter3:16 if seed capability and unsupported; otherwise0:18 under the listed capability exclusions. Moving1:0 becomes1:3 when run is unlocked and force_run0.

### function:RAY_RESPOND_TO_FIRE0

PROVEN: Start fist preparation when the fist capability and availability permit it.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f1c8.

Requires !poing.is_active, RayEvts.poing and force_run0; eta bit1 is checked by RAY_PREPARE_FIST in poing.c.

### function:RAY_RESPOND_TO_FIRE1

PROVEN: Route the jump button to ray_jump for eligible main states.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f208.

fin_boss blocks it. Main0/2/5/7 call directly; main1 requires no down input or running sub3, otherwise clears button_released. ray_jump still checks eta permission and release parity.

### function:RAY_BALANCE_ANIM

PROVEN: Map a swing angle and facing to an animation frame.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f24c.

Angle0 is coerced to1, then (angle-1)>>3 selects one of two facing-dependent frame ranges. Angle units are1/512 turn in the swing callers.

### function:SET_RAY_BALANCE

PROVEN: Enter grapple swinging state7:0 and reset movement bookkeeping.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f29c.

Clears support, gravity, momentum, vertical speed and idle; button_released1. Interrupted hurt2:8 sets90 invulnerability.

### function:RAY_GOING_BALANCE

PROVEN: Move the grapple entry toward its swinging radius and angle.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f328.

Squared anchor distance threshold4512; stores angle in grapple follow_x, direction+-1 in link, radius in follow_y. Trigonometric amplitudes512. Preserve the contradictory terminal branch as observed, without inventing a reachable condition.

### function:RAY_BALANCE

STRONG: Advance grapple swinging and derive Rayman's movement from the grapple anchor.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f500.

Angle bounds128..384, radius converges95..100, endpoint pause5 calls, angle step=(abs_cosinus(angle)>>7)+1 with sign from link. Sets animation frame directly, adds grapple motion, then handles floor landing or wall release.

### function:RAY_FIN_BALANCE

PROVEN: Release the grapple into airborne state2:2.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f7d0.

Facing follows grapple angle<256; resets gravity, helico/link, and backs x out by the current horizontal displacement.

### function:RayTestBlocSH

STRONG: Stop super-helicopter motion against sampled hard tiles.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f870.

Samples leading x at16/32/48 pixels below offset_hy, using low-nibble x thresholds4/13; upward sample at+16 stops negative vertical speed.

### function:remoteControlRay

PROVEN: Move the scripted horizontal target state until within one pixel.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f95c.

Signed16 difference x-target; outside+-1 selects sub21 and facing, otherwise speed_x0, sub20, and snaps target to ray.x. Target sentinel is interpreted by DO_RAYMAN.

### function:STOPPE_RAY_CONTRE_PAROIS

PROVEN: Stop upward airborne movement at a blocking tile.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5f9e0.

Requires tile flag0x10, nondeath mode, excludes sub7/9, and speed_y<=0. Unless hurt8 selects sub1 and clears release; resets fall accumulator and vertical speed.

### function:RAY_IN_THE_AIR

PROVEN: Update airborne timers, jump gravity, helicopter, landing and attachment checks.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:5fa44.

Timers decrement unless -1; jump_time increments once. Gravity3/4 phases run unless mode10/11; holding jump suppresses DO_PESANTEUR only through call12 unless hit/timer/sub2. Extra vertical increment at23. Fall accumulator sums speed_y unless -1. Landing and final speed clamp -10..4 depend on support acquisition.

### function:terminateFistWhenRayDies

PROVEN: Spawn a fist disappearance effect and disable an active fist.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:6008c.

Only acts if poing.is_active; calls DO_NOVA then switch_off_fist.

### function:snifRayIsDead

PROVEN: Consume a life and clear death-related rewards/capabilities.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:600a4.

Decrements signed lives, clears tings/super_helico, requests5 particles unless drowning or3:32, and disables fist. This function alone does not clamp lives.

### function:rayfallsinwater

PROVEN: Begin drowning state3:22 and its one-count death delay.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:6010c.

Clears speeds/momentum, sets iframes120, dead_time1 and drowning flag, allocates splash, disables fist and starts loss audio.

### function:RAY_DEAD

PROVEN: Check lethal bounds/water and retire or report the live Rayman object.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:601a8.

Returns active&&alive&&mode!=3/4. HP byte0 is passed through RAY_HIT to underflow255. Falling2:9 and terminal3:22/32 decrement dead_time; zero retires object, consumes life, uses64 delay and90 invulnerability. Preserve byte countdown underflow.

### function:RAY_HURT

PROVEN: Subtract a hit point and select death mode on unsigned underflow.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60408.

HP0 decrements to255. Only if iframes==-1 writes60 under a crouched blocking ceiling, otherwise120. HP255 selects mode3 or4 depending on prior mode2 and resets jump_time.

### function:RepousseRay

PROVEN: Push Rayman below the active stone mechanism and update its airborne state.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:604b0.

Uses pierreAcorde_obj_id; strict horizontal range(x-82,x+150), y floor+96, converted vertical motion when entering air. Stone alive clears once y>ymap+168.

### function:RayEstIlBloque

STRONG: Detect horizontal blocking by active objects marked as Rayman barriers.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60598.

Uses the leading cached box edge and five vertical samples at height/4 spacing; shapes shrink animation bounds and can use support-sprite top. Returns0/1. Cached ray_zdc_h is y and ray_zdc_y is height.

### function:stackRay

PROVEN: Append Rayman and optional fist display state to the101-entry replay ring.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:6089c.

Positions narrow to signed16; scale narrows to byte. Inactive fist coordinate slots are left unchanged but validity byte is0. Write cursor wraps after100 and sets full flag.

### function:RAY_SURF

STRONG: Select fast/slow ground slide animation transitions from horizontal speed.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60964.

Skips two Tibetan supports; eligible main0 enters0:41 at magnitude>3 unless sub40, or0:42 when sub40 slows below3. Eta bit0x40 blocks those transitions.

### function:DO_SURF_CHANGE_HAIRS

PROVEN: Choose the ground-slide hair animation according to motion and facing.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60a8c.

Only0:40; sign(speed_x) equal facing+-1 selects animation79, otherwise80.

### function:DO_PIEDS_RAYMAN

PROVEN: Empty original Rayman feet callback.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60ae4.

Parameter is unused; no invented feet behaviour is assigned.

### function:DO_MORT_DE_RAY

PROVEN: Advance the airborne hurt/death sequence and finish life consumption.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60ae8.

Forces iframes-1, executes airborne and directional motion, restores saved pan capabilities if needed. EOA or departure from2:8 retires Rayman, consumes life and sets delay128; animation/history still advance.

### function:TEST_FIN_FOLLOW

PROVEN: Detach a supported Rayman when tile contact blocks the carried motion.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60c20.

Checks horizontal tile, samples vertical tile before/after motion according to direction and crouch mode, clears follow_id on floor block and recalages or adds4 when rising against a nonfloor tile.

### function:RAY_FOLLOW

PROVEN: Add the supporting object's movement to Rayman's own pixel movement.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60d58.

Per-axis type flags convert sixteenths via instantSpeed; then add support follow_x/follow_y. Calls TEST_FIN_FOLLOW and detaches inactive support.

### function:RAY_RESPOND_TO_ALL_DIRS

PROVEN: Order direction responses with pan, reversed-input and forced-run rules.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60e14.

Normal order is right then left, down then up only while joy_done0. Both horizontal buttons can run both handlers. Reversed input swaps horizontal facing requests. Pan direction follows motion unless stationary. Idle counters reset on handled input.

### function:DO_RAY_ON_MS

PROVEN: Update mounted Rayman movement, fist inputs and collision dispatch.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:60ff8.

Main6 uses a four-call phase0..3; limits screen movement to the mounted viewport. Obtains cached box as(x,ray_zdc_h,w,ray_zdc_y). Suppresses collision dispatch on new_world/new_level.

### function:DO_RAYMAN

STRONG: Order ordinary Rayman input, movement, support, animation and collision work.

Units: see parameter/result and side-effect contract. DOS layout/address: 1:6133c.

Death check precedes tile hazards/input. Hurt2:8 and cliff2:31 bypass ordinary inputs. Scripted main3 has separate handling. Fist charge grows in sub12 and release throws. Swing skips DO_ANIM. Collisions follow motion; stackRay runs even when death check fails.

## types.json

Meaning confidence is separate from compiler layout and STRICT status. UNKNOWN retains neutral names. Raw source/displacement/external token hits are candidates, not type proofs.

### field:obj_t.sprites

PROVEN: Pointer to the object-specific sprites table.

Units: pointer. DOS layout/address: +0x0, 4 B.

DOS pointers are32 bits; eta is a two-level main/substate table.

Shipped initial sample: 11837 records, 34 values, 1..39. This does not bound runtime values.

### field:obj_t.animations

PROVEN: Pointer to the object-specific animations table.

Units: pointer. DOS layout/address: +0x4, 4 B.

DOS pointers are32 bits; eta is a two-level main/substate table.

Shipped initial sample: 11837 records, 34 values, 1..39. This does not bound runtime values.

### field:obj_t.img_buffer

STRONG: Object sprite pixel-atlas buffer.

Units: pointer. DOS layout/address: +0x8, 4 B.

Pointer32 in DOS; sprite offset_in_atlas indexes the pixel buffer.

Shipped initial sample: 11837 records, 34 values, 1..39. This does not bound runtime values.

### field:obj_t.eta

PROVEN: Pointer to the object-specific eta table.

Units: pointer. DOS layout/address: +0xc, 4 B.

DOS pointers are32 bits; eta is a two-level main/substate table.

Shipped initial sample: 11837 records, 33 values, 0..33. This does not bound runtime values.

### field:obj_t.cmds

STRONG: Object script stream/context member cmds.

Units: script dependent. DOS layout/address: +0x10, 4 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial sample: 11837 records, 8185 values, 0..9728522. This does not bound runtime values.

### field:obj_t.cmd_labels

STRONG: Object script stream/context member cmd_labels.

Units: script dependent. DOS layout/address: +0x14, 4 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial sample: 11837 records, 2203 values, 0..9728146. This does not bound runtime values.

### field:obj_t.cmd_contexts

STRONG: Object script stream/context member cmd_contexts.

Units: script dependent. DOS layout/address: +0x18, 4 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.field_1C

PROVEN: Overloaded 32-bit scratch mark: set by make_active/make_active2 and tested to suppress repeat linked activation; also set during save restoration for gate types179/242 and tested by INIT_OBJECTS.

Units: boolean mark. DOS layout/address: +0x1c, 4 B.

Cleared for all candidate objects at active-table rebuild and for all objects in INIT_MOTEUR. Not a permanent object property; retain the neutral name because restore and activation share storage.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.link_has_gendoor

STRONG: Marker enabling linked-group operations; normally set for eligible non-self successor groups, not a proof that a generating door exists.

Units: boolean marker. DOS layout/address: +0x20, 4 B.

correct_link sets it for nontrivial groups even without a door requirement. suppressFromLinkList clears removed/singleton markers; door detach/restore and save restoration maintain it. Keep existing spelling with this caveat.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.is_active

PROVEN: Object participation flag saved as the high bit of each two-bit snapshot entry.

Units: boolean. DOS layout/address: +0x24, 4 B.

s32 storage, snapshot preserves only the one-bit domain used here; distinct from flags.alive.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.x

PROVEN: Signed32 world pixel x origin in live object movement; loader also reuses object records as descriptors.

Units: pixels in live objects. DOS layout/address: +0x28, 4 B.

s32 storage; many consumers deliberately narrow to s16. switchOff writes -32000 sentinel. Loader wldobj descriptors reuse x as image byte length, so the type does not guarantee pixel units for every instance.

Ordinary movement adds integer dx after any speed conversion. Screen x=x-xmap+8, narrowed to s16. Some camera helpers first narrow world x to s16; keep those boundaries.

Shipped initial sample: 11837 records, 3721 values, -32000..10244. This does not bound runtime values.

### field:obj_t.y

PROVEN: Signed32 world pixel y origin in live object movement.

Units: pixels in live objects. DOS layout/address: +0x2c, 4 B.

s32 storage; many consumers deliberately narrow to s16. switchOff writes -32000 sentinel. Loader wldobj descriptors reuse x as image byte length, so the type does not guarantee pixel units for every instance.

Generic gravity changes speed_y, not a fractional y accumulator. Screen y=y-ymap is narrowed to s16. -32000 is one hidden-position sentinel; water sinking can set y=ymap+484.

Shipped initial sample: 11837 records, 1707 values, -32000..4849. This does not bound runtime values.

### field:obj_t.active_flag

PROVEN: Activation lifecycle code used by zone transitions: 0 activated/default,1 dead/outside,2 reinitialised awaiting departure,4 waiting for the linked group to leave.

Units: state code. DOS layout/address: +0x30, 4 B.

These codes are separate from alive and is_active. SET_ACTIVE_FLAG reads a signed16 snapshot of the signed32 slot on its outside-zone branch. REINIT_OBJECT sets2; kill_obj sets1.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.id

PROVEN: Object identifier stored in the active and always-object id lists.

Units: object index. DOS layout/address: +0x34, 2 B.

Signed16; do not confuse a list slot with an object id.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.screen_x

PROVEN: Stored screen-space origin coordinate screen_x after camera subtraction.

Units: pixels. DOS layout/address: +0x36, 2 B.

s16; screen_x includes +8 bias.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.screen_y

PROVEN: Stored screen-space origin coordinate screen_y after camera subtraction.

Units: pixels. DOS layout/address: +0x38, 2 B.

s16; screen_x includes +8 bias.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.sub_speed

UNKNOWN: Type-dependent signed16 scratch used for a water-motion residual and another type's horizontal distance.

Units: type-dependent pixels / sixteenths. DOS layout/address: +0x3a, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 7 source member-token matches are retained for review, including possible other types.

Pass3 refinement: DO_EAU_QUI_MONTE world3 stone branch adds14, moves water y+1 only if residual>16, then subtracts16. frame.c stores object-anchor minus Rayman-anchor distance. Do not rename the shared slot to a universal fractional speed.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.init_x

STRONG: Initial position component init_x used by reset and relative-displacement logic.

Units: pixels. DOS layout/address: +0x3c, 2 B.

DOS sinYspeed proves init_y reference position; initial-position naming also matches external object declarations.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.init_y

STRONG: Initial position component init_y used by reset and relative-displacement logic.

Units: pixels. DOS layout/address: +0x3e, 2 B.

DOS sinYspeed proves init_y reference position; initial-position naming also matches external object declarations.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.speed_x

PROVEN: Signed16 horizontal speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call.

Units: pixels/call or 1/16 pixels/call. DOS layout/address: +0x40, 2 B.

Type flags0x1000/0x2000 select instantSpeed. It distributes fractional motion using map_time rather than a per-object fractional coordinate accumulator. Fist has a special movement path.

Type flag0x1000 selects instantSpeed in ordinary MOVE_OBJECT. The fist special path always adds speed_x directly; SET_X_SPEED also time-gates and scales state speed by the animation-header period.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.speed_y

PROVEN: Signed16 vertical speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call.

Units: pixels/call or 1/16 pixels/call. DOS layout/address: +0x42, 2 B.

Type flags0x1000/0x2000 select instantSpeed. It distributes fractional motion using map_time rather than a per-object fractional coordinate accumulator. Fist has a special movement path.

Type flag0x2000 selects instantSpeed and16-scaled generic gravity. Generic fall clamps are[-8,6] or[-128,96]. Fist movement instead updates poing.y_16 from a16-scaled signed16 shift.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.nb_sprites

PROVEN: Count bounding the object sprite-table search.

Units: sprites. DOS layout/address: +0x44, 2 B.

s16 field read as u16 in this helper; its sign must not be silently propagated into the search bound.

Shipped initial sample: 11837 records, 51 values, 3..256. This does not bound runtime values.

### field:obj_t.cmd_offset

STRONG: Object script stream/context member cmd_offset.

Units: script dependent. DOS layout/address: +0x46, 2 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial sample: 11837 records, 3 values, 0..3. This does not bound runtime values.

### field:obj_t.nb_cmd

STRONG: Type-dependent script count or Rayman airborne-inertia selector.

Units: count / selector. DOS layout/address: +0x48, 2 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Pass3 refinement: Rayman sets1 when wind/nonordinary departure preserves inertia and0 on hit/wall or ordinary supported jump. Interactive platforms decrement this as remaining command updates. Keep the per-type interpretation.

Shipped initial sample: 11837 records, 17 values, 0..254. This does not bound runtime values.

### field:obj_t.follow_id

STRONG: Type-dependent support/object id or reused vertical command parameter.

Units: object id / type-dependent speed. DOS layout/address: +0x4a, 2 B.

-1 means no followed object for Rayman; MITE writes0 for its own behaviour.

Pass3 refinement: Rayman -1 means no support; rayMayLandOnAnObject acquires from active list, RAY_FOLLOW adds carrier motion, and jump/hit detach. Moving-platform GO_SPEED and flash motion interpret the same field as a vertical speed, so retain the shared spelling.

Shipped initial sample: 11837 records, 14 values, -32..32. This does not bound runtime values.

### field:obj_t.follow_y

STRONG: Type-dependent vertical follower displacement or grapple radius.

Units: pixels / type-dependent. DOS layout/address: +0x4c, 2 B.

No universal target_x/delta_x rename: ray swing also uses follow_x as an angle; preserve role per object/state.

Pass3 refinement: RAY_FOLLOW adds it after converting carrier speed_y as needed; object landing includes it in relative-speed tolerance. Grapple code stores swing radius95..100 here. Do not infer all follow_y values are speeds.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.follow_x

STRONG: Type-dependent horizontal follower displacement or grapple angle.

Units: pixels / 1/512-turn angle. DOS layout/address: +0x4e, 2 B.

No universal target_x/delta_x rename: ray swing also uses follow_x as an angle; preserve role per object/state.

Pass3 refinement: RAY_FOLLOW adds it directly to pixel movement. Grapple objects instead store angle here and use512-scaled trig products; live object type determines interpretation.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.param

PROVEN: Type-specific signed scratch parameter, including Rayman edge classification in calc_btyp.

Units: context dependent. DOS layout/address: +0x50, 2 B.

Rayman edge values -1/0/1 do not describe all object types.

Shipped initial sample: 11837 records, 3 values, 0..5. This does not bound runtime values.

### field:obj_t.link

UNKNOWN: Type-dependent signed16 scratch with proven Rayman fall, grapple-direction, door-id and initialization roles.

Units: type-dependent pixels/state/id. DOS layout/address: +0x52, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 208 source member-token matches are retained for review, including possible other types.

The live circular successor array is link_init, not obj.link. Do not rename this shared slot to next_link; the whole-source scan has many other type-specific writers/readers.

Pass3 refinement: Rayman RAY_IN_THE_AIR sums speed_y while link!=-1, resets at apex, and compares>=200 on tile landing (>200 on object landing) for smoke. -1 suppresses accumulation. Grapple objects use+-1 swing direction; doors hold detached id; spike posts use one-time initialization0/1. A single universal name remains unsupported.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.ray_dist

STRONG: Type-dependent Rayman tile index or support-distance code with several geometric conventions.

Units: linear tile index / pixels / code. DOS layout/address: +0x54, 2 B.

ray.ray_dist indexes mp.map; other objects use signed distance and1000/10000 sentinels. Keep the shared name.

Pass3 refinement: DO_RAYMAN/ON_MS cache feet tile index here. General support helper returns abs(vertical delta) inside[x,x+w-1], negative abs-1 in lateral margin,10000 unavailable. Slope platform margin uses negative abs without-1; multisprite paths can store signed vertical delta. Never globally interpret sign as vertical side.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.iframes_timer

STRONG: Type-dependent signed16 response timer or reused command/state storage; Rayman uses -1 for vulnerability.

Units: type-dependent calls/state/axis speed. DOS layout/address: +0x56, 2 B.

-1 is a significant inactive sentinel; do not turn into an unsigned countdown.

Pass3 refinement: Rayman dispatch accepts damage at-1; DO_COLLISIONS decrements nonnegative values after callbacks, including0->-1. RAY_HURT writes120 normally or60 under blocked crouch; hurt landing clamps to90. Moving commands and flashes reuse this field as horizontal speed; killing eyes use a firing delay. It is not universally an invulnerability timer.

Shipped initial sample: 11837 records, 14 values, -128..32. This does not bound runtime values.

### field:obj_t.test_block_index

PROVEN: Last row-major map tile index used by the changed-block test.

Units: tile index. DOS layout/address: +0x58, 2 B.

u16 storage compared through s16; same semantics visible in ufo test helper.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.scale

PROVEN: Projection-depth parameter whose denominator is signed_scale+256.

Units: projection depth units. DOS layout/address: +0x5a, 2 B.

u16 storage passed to s16 scale formals;0 is unity,256 gives half scale. Not a direct percentage or pixel size.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.zdc

PROVEN: Packed reference to an object collision-zone run.

Units: packed count/index. DOS layout/address: +0x5c, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 6 source member-token matches are retained for review, including possible other types.

Pass3 refinement: Unsigned16: high5 bits count (>>11), low11 start index (&0x7ff) in zdc_tab.0 selects hit_sprite fallback handling. get_zdc advances8-byte records without a bounds check. Initial serialized zeros do not describe the runtime type_zdc assignment.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.active_timer

PROVEN: Delay inhibiting reactivation, counted down while the living object is in its action zone.

Units: activation calls. DOS layout/address: +0x5e, 2 B.

REINIT_OBJECT uses0 for a linked-group marker and120 otherwise. The u16 field is decremented only when its signed16 interpretation is >0; values>=32768 therefore do not count down here.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.type

PROVEN: Unsigned object type discriminator indexing flags and selecting behaviour.

Units: object type id. DOS layout/address: +0x60, 2 B.

DOS u16; Android/PS1 layout widths must not replace it.

Shipped initial sample: 11837 records, 221 values, 0..257. This does not bound runtime values.

### field:obj_t.btypes

PROVEN: Five cached collision block types in centre,left,right,above,below order.

Units: block type id. DOS layout/address: +0x62, 5 B.

Five u8 entries, not a cross-shaped array of map indices; centre may be adjusted by bloc_floor.

### field:obj_t.unused_padding

UNKNOWN: Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x67, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.offset_bx

PROVEN: Horizontal object anchor offset used for collision probes and reflection centre.

Units: pixels. DOS layout/address: +0x68, 1 B.

u8; x reflection uses 2*offset_bx, not sprite half-width.

Shipped initial sample: 11837 records, 29 values, 0..192. This does not bound runtime values.

### field:obj_t.offset_by

PROVEN: Vertical object anchor offset used for foot/block probes and projection centre.

Units: pixels. DOS layout/address: +0x69, 1 B.

u8; object origin and collision anchor are different coordinates.

Shipped initial sample: 11837 records, 45 values, 0..248. This does not bound runtime values.

### field:obj_t.anim_index

PROVEN: Current animation selector or current frame selector (anim_index).

Units: index. DOS layout/address: +0x6a, 1 B.

u8 storage. anim_index255 is asserted in source by an empty macro; no DOS runtime guard is added.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.anim_frame

PROVEN: Current animation selector or current frame selector (anim_frame).

Units: index. DOS layout/address: +0x6b, 1 B.

u8 storage. anim_index255 is asserted in source by an empty macro; no DOS runtime guard is added.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.sub_etat

PROVEN: Object-type- and main-state-specific substate-table index.

Units: state index. DOS layout/address: +0x6c, 1 B.

main_etat indexes eta pointers; sub_etat indexes 8-byte eta entries.

Pass3 refinement: Rayman substates are named only as scoped(main,sub) pairs in ray_state_machine; identical sub numbers in other main states or object types have different meanings. Shipped eta transitions are additional data evidence, not a complete reachability proof.

Shipped initial sample: 11837 records, 49 values, 0..65. This does not bound runtime values.

### field:obj_t.main_etat

PROVEN: Object-type-specific main state-table index; Rayman's controller dispatches eight observed state families.

Units: state index. DOS layout/address: +0x6d, 1 B.

main_etat indexes eta pointers; sub_etat indexes 8-byte eta entries.

Pass3 refinement: Rayman0 is ground stationary/action family (can still slide),1 locomotion,2 airborne,3 mixed scripted/action/death,4 vine,5 ledge hanging,6 mounted,7 grapple swinging. These names do not transfer to other object types, and3 is not a universal death state.

Shipped initial sample: 11837 records, 11 values, 0..10. This does not bound runtime values.

### field:obj_t.init_sub_etat

STRONG: Reset value for init_sub_etat.

Units: state index or hit points. DOS layout/address: +0x6e, 1 B.

Initial fields are separate from current values; preserve byte widths.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.init_main_etat

STRONG: Reset value for init_main_etat.

Units: state index or hit points. DOS layout/address: +0x6f, 1 B.

Initial fields are separate from current values; preserve byte widths.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.cmd

STRONG: Current script command discriminator.

Units: opcode. DOS layout/address: +0x70, 1 B.

GO_LEFT=0, GO_RIGHT=1 in basic; not a facing bit.

Shipped initial sample: 11837 records, 10 values, 0..30. This does not bound runtime values.

### field:obj_t.gravity_value_1

STRONG: Type-dependent gravity phase byte.

Units: handler-call phase. DOS layout/address: +0x71, 1 B.

Both cleared together; usages differ by state and object, so no stronger shared rename.

OBJ_IN_THE_AIR increments before applying gravity. Rebound helpers reset it to0. Mode1/5 and tambour override gate on its zero phase; mode10/11 increment it inside DO_PESANTEUR.

Pass3 refinement: Ordinary RAY_IN_THE_AIR advances0..2 before DO_PESANTEUR unless gravity high nibble10/11; mounted path advances0..3. Modes10/11 increment internally. Jump/hit/ledge reset0. Other object types use their existing gravity contract.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.gravity_value_2

STRONG: Second type-dependent gravity phase byte or high-nibble10/11 period.

Units: handler-call phase/period. DOS layout/address: +0x72, 1 B.

Both cleared together; usages differ by state and object, so no stronger shared rename.

Mode2 accelerates on its zero phase. Rebounds usually reset it, except modes10/11. A threshold of0 in mode10/11 causes an acceleration on every call after the byte increment.

Pass3 refinement: Ordinary RAY_IN_THE_AIR advances0..3 unless modes10/11. Helicopter sets20 for the internally timed gravity modes. Jump resets0; type172 enemy jump seeds5.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.change_anim_mode

PROVEN: Animation transition mode set to1 by the state setters.

Units: mode. DOS layout/address: +0x73, 1 B.

Mode1 requests reset on animation change; do not reset anim_frame immediately in the setter.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.offset_hy

STRONG: Unsigned vertical upper-body/collision anchor offset.

Units: pixels. DOS layout/address: +0x74, 1 B.

DOS zero-extends; Rayverse signed declaration is contradicted by DOS.

Shipped initial sample: 11837 records, 34 values, 0..170. This does not bound runtime values.

### field:obj_t.follow_sprite

PROVEN: Selected animation layer for following, also overloaded as Blacktoon behaviour selector.

Units: layer index or subtype. DOS layout/address: +0x75, 1 B.

Not a sprite-table reference id; GET_SPRITE_POS takes an animation layer index.

Shipped initial sample: 11837 records, 9 values, 0..8. This does not bound runtime values.

### field:obj_t.hit_points

PROVEN: Unsigned hit-point byte in obj_hurt, with type-specific reuse including signed deformation angle input.

Units: hit points or type-specific state. DOS layout/address: +0x76, 1 B.

u8 storage; damage subtraction clamps at0. Type254 rendering and collision explicitly reinterpret it as s8 for slope/deformation; do not impose a universal nonnegative-health interpretation.

Shipped initial sample: 11837 records, 17 values, 0..255. This does not bound runtime values.

### field:obj_t.init_hit_points

STRONG: Reset value for init_hit_points.

Units: state index or hit points. DOS layout/address: +0x77, 1 B.

Initial fields are separate from current values; preserve byte widths.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.init_flag

UNKNOWN: Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x78, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 11 source member-token matches are retained for review, including possible other types.

Shipped initial sample: 11837 records, 8 values, 0..9. This does not bound runtime values.

### field:obj_t.hit_sprite

PROVEN: Collision layer selector with three special dispatch codes.

Units: layer id / sentinel. DOS layout/address: +0x79, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 10 source member-token matches are retained for review, including possible other types.

Pass3 refinement: 0..252 select one layer;0xfd chooses type-specific bag/BB1 geometry;0xfe chooses packed zones or object rectangle;0xff chooses packed zones or eligible-layer iteration. CHECK_BOX_COLLISION returns-1 for miss, so layer0 is a valid hit.

Shipped initial sample: 11837 records, 6 values, 0..255. This does not bound runtime values.

### field:obj_t.detect_zone

STRONG: Type-dependent size/range parameter used by the detection-zone computation.

Units: pixels or type-specific range. DOS layout/address: +0x7a, 1 B.

Do not equate this input parameter with detect_zone_flag, the computed classification.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.detect_zone_flag

PROVEN: Detection-zone classification tested by OBJ_IN_ZONE.

Units: classification. DOS layout/address: +0x7b, 1 B.

Exactly1 or >=1 depending on type; not safely interchangeable with bool.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.cmd_context_depth

STRONG: Object script stream/context member cmd_context_depth.

Units: script dependent. DOS layout/address: +0x7c, 1 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.config

PROVEN: Type-specific byte configuration tested for Blacktoon edge-turning.

Units: context dependent. DOS layout/address: +0x7d, 1 B.

Known local value2 here does not imply a shared enum.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.display_prio

STRONG: Object drawing-order bucket selector.

Units: priority bucket. DOS layout/address: +0x7e, 1 B.

Byte, separate from activity flags; exact bucket policy belongs with display-list generation.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.timer

UNKNOWN: Type-dependent byte countdown/state scratch with multiple independently established roles.

Units: type-dependent handler calls/state. DOS layout/address: +0x7f, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 159 source member-token matches are retained for review, including possible other types.

INIT_RAY clears it; DO_CLING_ANIMS uses a positive countdown before DO_ANIM. This does not establish one duration unit or timer meaning for all object types.

Pass3 refinement: Rayman uses10 calls to suppress vine reattachment; grapple objects use5-call endpoint pauses. DO_RAY_POS_CMD uses255 disabled and decrements0 to255 after activation. Ship uses50 half-clock counts; other objects keep their own contracts. No universal timer unit/name is assigned.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_t.anim_count

STRONG: Number of animations in the object descriptor.

Units: animations. DOS layout/address: +0x80, 1 B.

Byte count copied from loaded descriptor; serialized level values are not a bound on every runtime state index.

Shipped initial sample: 11837 records, 28 values, 0..135. This does not bound runtime values.

### field:obj_t.flags

PROVEN: Packed two-byte per-object flag record.

Units: bitset. DOS layout/address: +0x81, 2 B.

First byte begins at0x81; ninth named flag is in the next byte.

Shipped initial sample: 11837 records, 2 values, 0..32. This does not bound runtime values.

### field:obj_t.field_83

UNKNOWN: Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x83, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:eta_t.speed_x_right

STRONG: Signed state-selected horizontal speed for the speed_x_right facing.

Units: object speed units. DOS layout/address: +0x0, 1 B.

s8, sign extended; multiplication by animation clock divisor precedes tiny adjustment.

Shipped initial sample: 2639 records, 130 values, -128..124. This does not bound runtime values.

### field:eta_t.speed_x_left

STRONG: Signed state-selected horizontal speed for the speed_x_left facing.

Units: object speed units. DOS layout/address: +0x1, 1 B.

s8, sign extended; multiplication by animation clock divisor precedes tiny adjustment.

Shipped initial sample: 2639 records, 118 values, -126..126. This does not bound runtime values.

### field:eta_t.anim_index

PROVEN: Animation id selected by this main/substate entry.

Units: animation index. DOS layout/address: +0x2, 1 B.

Byte value; a state entry is8 bytes.

Shipped initial sample: 2639 records, 148 values, 0..255. This does not bound runtime values.

### field:eta_t.next_main_etat

PROVEN: State indices selected when an animation crosses its frame bounds.

Units: state index. DOS layout/address: +0x3, 1 B.

Prediction follows one transition; no recursive transition walk.

Shipped initial sample: 2639 records, 104 values, 0..255. This does not bound runtime values.

### field:eta_t.next_sub_etat

PROVEN: State indices selected when an animation crosses its frame bounds.

Units: state index. DOS layout/address: +0x4, 1 B.

Prediction follows one transition; no recursive transition walk.

Shipped initial sample: 2639 records, 143 values, 0..255. This does not bound runtime values.

### field:eta_t.anim_speed

PROVEN: Packed state timing/control byte: low nibble selects the animation clock, high nibble selects the airborne gravity/control mode.

Units: two selectors. DOS layout/address: +0x5, 1 B.

Low nibble0..15 is also used as a nominal period; high nibble has separate state-specific uses.

Low0 stops DO_ANIM advancement, while EOA still checks horloge[0]==0. Modes0,7,8,9,12..15 have no generic acceleration; the asset domain contains selectors not implemented by generic gravity.

Shipped initial sample: 2639 records, 120 values, 0..255. This does not bound runtime values.

### field:eta_t.anim_speed_bits

PROVEN: Overlapping nibble/bit alias container for the corresponding eta byte.

Units: bitset. DOS layout/address: +0x5, 1 B.

These containers have no independent storage or independent meaning; see individual bits and raw bytes.

### field:eta_t.anim_speed_bits.low

PROVEN: Low nibble alias of the animation timing byte.

Units: clock index. DOS layout/address: +0x5, 1 B.

4-bit domain0..15; aliases anim_speed, not additional storage.

Shipped initial sample: 2639 records, 16 values, 0..15. This does not bound runtime values.

### field:eta_t.anim_speed_bits.high

PROVEN: High-nibble gravity/control selector consumed by DO_PESANTEUR and specialized state logic.

Units: selector0..15. DOS layout/address: +0x5, 1 B.

Values10/11 select special gravity-counter handling; the nibble is mutated independently of animation timing. Keep the neutral high alias until all selector cases have a shared semantic vocabulary.

Full generic rules are in function:DO_PESANTEUR; modes10/11 change ownership of both gravity counters. Keep high because state-specific writers such as RAY_HELICO also manipulate it.

Shipped initial sample: 2639 records, 15 values, 0..15. This does not bound runtime values.

### field:eta_t.sound_index

STRONG: State-selected sound event index.

Units: sound id. DOS layout/address: +0x6, 1 B.

Byte; external field naming agrees with sound dispatch usage.

Shipped initial sample: 2639 records, 201 values, 0..255. This does not bound runtime values.

### field:eta_t.flags

PROVEN: State behaviour bitset, including reverse playback0x10 and Rayman rectangle selection0x40.

Units: bitset. DOS layout/address: +0x7, 1 B.

Do not describe all bits as animation flags; only observed roles are established here.

Shipped initial sample: 2639 records, 109 values, 0..255. This does not bound runtime values.

### field:eta_t.flags_bits

PROVEN: Overlapping nibble/bit alias container for the corresponding eta byte.

Units: bitset. DOS layout/address: +0x7, 1 B.

These containers have no independent storage or independent meaning; see individual bits and raw bytes.

### field:eta_t.flags_bits.bit0

STRONG: State permission bit tested by Rayman jump initiation and the stonewoman jump handler.

Units: permission bit. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Pass3 refinement: ray_jump additionally requires odd release latch; obj_jump type172 additionally selects its script/state. No universal actor-state name beyond these observed consumers is asserted.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit1

STRONG: State permission bit gating fist preparation in RAY_PREPARE_FIST.

Units: permission bit. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Pass3 refinement: RAY_RESPOND_TO_FIRE0 separately requires unlocked fist, no active fist and no forced run; poing.c tests bit1 of current Rayman eta before choosing charge or immediate airborne throw.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit2

STRONG: State permission bit gating helicopter activation.

Units: permission bit. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Pass3 refinement: RAY_HELICO tests bit2 together with release1/held jump, then capability and force_run. A standalone set bit does not prove helicopter is available.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit3

STRONG: State participation bit for damage/contact eligibility, with actor-dependent readers.

Units: state flag. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

This test is after DO_ONE_OBJECT and excludes boss-flagged types. Retain bit3 rather than inventing a universal health or invincibility name.

Pass3 refinement: DO_POING_COLLISION requires this bit on target; DO_COLLISIONS requires it on Rayman in ordinary hurt handling; spike hurt tests it too. DO_OBJECTS also uses it for the previous zero-HP repair rule. Do not replace all uses by an unconditional damage-enabled boolean.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit4

PROVEN: Reverse animation playback flag0x10, selecting decrement and last-frame initialization.

Units: boolean. DOS layout/address: +0x7, 1 B.

When set, the terminal animation frame is0.

DO_ANIM decrements the u8 frame and recognizes255 as underflow; EOA tests frame0 on the selected clock phase. Proposed field name anim_reverse is INVENTED, Android has no member symbols.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit5

STRONG: State gate for Rayman contact dispatch and collision-zone traversal.

Units: participation bit. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Pass3 refinement: DO_COLLISIONS and SHOW_COLLISIONS_ZONES test the target eta bit5; special contact callback also tests it. This does not gate every fist or object-to-object collision path.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit6

PROVEN: Context-dependent state bit selecting Rayman's low collision shape and participating in generating-door inset encoding.

Units: state flag. DOS layout/address: +0x7, 1 B.

This observed effect alone does not establish a universal state name such as crouching.

Pass3 refinement: For Rayman GET_RAY_ZDC returns(x+72,y+64,16,14) under bit6 and controller applies crouch/ceiling rules. For generating-door objects GET_OBJ_ZDC uses bit7+2*bit6 in inset=24-12*code. Preserve both contexts rather than naming this shared bit crouched globally.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:eta_t.flags_bits.bit7

STRONG: Low-weight bit of generating-door collision inset selection, paired with bit6.

Units: packed selector bit. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 1 source member-token matches are retained for review, including possible other types.

Pass3 refinement: GET_OBJ_ZDC type164 computes24-12*(bit7+2*bit6). Its use outside that shape selector remains unassigned. Existing UNKNOWN is refined only to this concrete reader contract.

Shipped initial sample: 2639 records, 2 values, 0..1. This does not bound runtime values.

### field:sprite_t.offset_in_atlas

STRONG: Pixel offset of this sprite in its object atlas buffer.

Units: bytes. DOS layout/address: +0x0, 4 B.

32-bit offset; retain asset layout separately from native pointers.

Shipped initial sample: 5512 records, 2382 values, 0..117792. This does not bound runtime values.

### field:sprite_t.id

PROVEN: Sprite reference id, with0 treated as an absent sprite in GET_SPRITE_POS.

Units: reference id. DOS layout/address: +0x4, 1 B.

Distinct from sprite-table index and animation layer index.

Shipped initial sample: 5512 records, 222 values, 0..222. This does not bound runtime values.

### field:sprite_t.outer_width

STRONG: Outer sprite rectangle dimension outer_width.

Units: pixels. DOS layout/address: +0x5, 1 B.

Outer width participates in mirroring; returned collision dimensions use inner widths instead.

Shipped initial sample: 5512 records, 21 values, 0..240. This does not bound runtime values.

### field:sprite_t.outer_height

STRONG: Outer sprite rectangle dimension outer_height.

Units: pixels. DOS layout/address: +0x6, 1 B.

Outer width participates in mirroring; returned collision dimensions use inner widths instead.

Shipped initial sample: 5512 records, 104 values, 0..145. This does not bound runtime values.

### field:sprite_t.inner_width

PROVEN: Returned inner sprite rectangle dimension inner_width.

Units: pixels. DOS layout/address: +0x7, 1 B.

Byte dimensions promoted before output stores.

Shipped initial sample: 5512 records, 111 values, 0..235. This does not bound runtime values.

### field:sprite_t.inner_height

PROVEN: Returned inner sprite rectangle dimension inner_height.

Units: pixels. DOS layout/address: +0x8, 1 B.

Byte dimensions promoted before output stores.

Shipped initial sample: 5512 records, 104 values, 0..145. This does not bound runtime values.

### field:sprite_t.sprite_pos

PROVEN: Overlapping raw/nibble views of the sprite inset byte.

Units: bitset. DOS layout/address: +0x9, 1 B.

Aliases occupy one byte.

### field:sprite_t.sprite_pos.raw

PROVEN: Overlapping raw/nibble views of the sprite inset byte.

Units: bitset. DOS layout/address: +0x9, 1 B.

Aliases occupy one byte.

Shipped initial sample: 5512 records, 56 values, 0..240. This does not bound runtime values.

### field:sprite_t.sprite_pos.bits

PROVEN: Overlapping raw/nibble views of the sprite inset byte.

Units: bitset. DOS layout/address: +0x9, 1 B.

Aliases occupy one byte.

### field:sprite_t.sprite_pos.bits.x

PROVEN: Packed nonnegative sprite inset component sprite_pos.bits.x.

Units: pixels. DOS layout/address: +0x9, 1 B.

x low nibble,y high nibble;0..15 each. These are not signed 4-bit deltas.

Shipped initial sample: 5512 records, 10 values, 0..9. This does not bound runtime values.

### field:sprite_t.sprite_pos.bits.y

PROVEN: Packed nonnegative sprite inset component sprite_pos.bits.y.

Units: pixels. DOS layout/address: +0x9, 1 B.

x low nibble,y high nibble;0..15 each. These are not signed 4-bit deltas.

Shipped initial sample: 5512 records, 16 values, 0..15. This does not bound runtime values.

### field:sprite_t.color

PROVEN: Raw/bitfield view of the sprite color control byte.

Units: bitset. DOS layout/address: +0xa, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

Shipped initial sample: 5512 records, 27 values, 0..243. This does not bound runtime values.

### field:sprite_t.color_bits

PROVEN: Raw/bitfield view of the sprite color_bits control byte.

Units: bitset. DOS layout/address: +0xa, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

### field:sprite_t.color_bits.unused

UNKNOWN: Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0xa, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial sample: 5512 records, 3 values, 0..3. This does not bound runtime values.

### field:sprite_t.color_bits.palette_index

STRONG: High-nibble palette selection.

Units: palette index. DOS layout/address: +0xa, 1 B.

Low nibble remains neutral; do not infer unused means always zero.

Shipped initial sample: 5512 records, 16 values, 0..15. This does not bound runtime values.

### field:sprite_t.flags

PROVEN: Raw/bitfield view of the sprite flags control byte.

Units: bitset. DOS layout/address: +0xb, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

Shipped initial sample: 5512 records, 111 values, 0..251. This does not bound runtime values.

### field:sprite_t.flags_bits

PROVEN: Raw/bitfield view of the sprite flags_bits control byte.

Units: bitset. DOS layout/address: +0xb, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

### field:sprite_t.flags_bits.unused0

UNKNOWN: Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0xb, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

Shipped initial sample: 5512 records, 4 values, 0..3. This does not bound runtime values.

### field:sprite_t.flags_bits.flip_x

PROVEN: Sprite horizontal flip bit XORed with object flip to select the bounds reflection.

Units: boolean. DOS layout/address: +0xb, 1 B.

Bit2 of sprite flags byte; differs from layer.mirrored renderer input.

Shipped initial sample: 5512 records, 2 values, 0..1. This does not bound runtime values.

### field:sprite_t.flags_bits.unused3

UNKNOWN: Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0xb, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial sample: 5512 records, 31 values, 0..31. This does not bound runtime values.

### field:anim_t.layers

PROVEN: Pointer to frame-major animation layers.

Units: pointer. DOS layout/address: +0x0, 4 B.

Layer stride4; frame stride=(layers_per_frame&0x3fff)*4 bytes.

### field:anim_t.frames

PROVEN: Pointer to one bounding rectangle per animation frame.

Units: pointer. DOS layout/address: +0x4, 4 B.

Frame rectangle stride4 bytes.

### field:anim_t.layers_per_frame

PROVEN: Packed animation header: low14 bits are layer count per frame, top2 bits encode the horizontal-speed clock period minus one.

Units: layer count / period selector. DOS layout/address: +0x8, 2 B.

Low14 bits are layer count; high2 bits yield clock divisor/index1..4 when shifted and incremented. Do not rename this whole field to an unqualified layer_count.

SET_X_SPEED selects horloge[1..4] and multiplies state speed by the period on its zero phase. Shipped samples use top-bit selectors0 and1; this is not a runtime restriction.

Shipped initial sample: 1629 records, 27 values, 1..16400. This does not bound runtime values.

### field:anim_t.frames_count

PROVEN: Number of frames in this animation.

Units: frames. DOS layout/address: +0xa, 2 B.

u16 field; snapToSprite intentionally narrows this count to u8.

Shipped initial sample: 1629 records, 126 values, 1..216. This does not bound runtime values.

### field:anim_frame_t.x

PROVEN: Unsigned offset of the animation bounding rectangle (x).

Units: pixels. DOS layout/address: +0x0, 1 B.

u8; horizontal reflection includes width and object anchor.

Shipped initial sample: 44955 records, 213 values, 0..255. This does not bound runtime values.

### field:anim_frame_t.y

PROVEN: Unsigned offset of the animation bounding rectangle (y).

Units: pixels. DOS layout/address: +0x1, 1 B.

u8; horizontal reflection includes width and object anchor.

Shipped initial sample: 44955 records, 216 values, 0..255. This does not bound runtime values.

### field:anim_frame_t.width

PROVEN: Animation bounding rectangle dimension width.

Units: pixels. DOS layout/address: +0x2, 1 B.

u8, emitted through s16 output pointers.

Shipped initial sample: 44955 records, 256 values, 0..255. This does not bound runtime values.

### field:anim_frame_t.height

PROVEN: Animation bounding rectangle dimension height.

Units: pixels. DOS layout/address: +0x3, 1 B.

u8, emitted through s16 output pointers.

Shipped initial sample: 44955 records, 256 values, 0..255. This does not bound runtime values.

### field:anim_layer_t.mirrored

STRONG: Layer horizontal mirror selector used by the renderer.

Units: boolean-like byte. DOS layout/address: +0x0, 1 B.

Renderer XORs with object flip; bounds helper instead uses sprite.flags bit2.

Shipped initial sample: 382694 records, 2 values, 0..1. This does not bound runtime values.

### field:anim_layer_t.x

PROVEN: Unsigned object-relative layer offset component x.

Units: pixels. DOS layout/address: +0x1, 1 B.

u8 offsets are not signed.

Shipped initial sample: 382694 records, 256 values, 0..255. This does not bound runtime values.

### field:anim_layer_t.y

PROVEN: Unsigned object-relative layer offset component y.

Units: pixels. DOS layout/address: +0x2, 1 B.

u8 offsets are not signed.

Shipped initial sample: 382694 records, 256 values, 0..255. This does not bound runtime values.

### field:anim_layer_t.sprite_index

PROVEN: Index into the owning object sprite table for this layer.

Units: sprite index. DOS layout/address: +0x3, 1 B.

Index0 suppresses successful bounds output.

Shipped initial sample: 382694 records, 186 values, 0..220. This does not bound runtime values.

### field:obj_flags_t.hurtbyfist

STRONG: Fist-hit interaction flag.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit0; preserve exact read/write sites before defining a port API.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.command_test

STRONG: Stored result of the current script test.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit1; script pilot should enumerate writers.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.alive

PROVEN: Low saved object-state bit, cleared at timed animation completion.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit2 of obj+0x81; distinct from is_active.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.flip_x

PROVEN: Horizontal object reflection/facing flag.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit3 of obj+0x81; meanings of left/right in code depend on object conventions.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.read_commands

PROVEN: Per-object command mode used to select turn labels.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit4; do not merge with per-type read-command flags.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.follow_enabled

STRONG: Enable support/follow-distance processing for the object.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit5; context is object-to-Rayman platform following.

Shipped initial sample: 11837 records, 2 values, 0..1. This does not bound runtime values.

### field:obj_flags_t.flag_0x40

STRONG: Water-entry/splash suppression flag in the generic movement path; also cleared by many spawned-object initializers.

Units: boolean marker. DOS layout/address: +0x0, 1 B.

allocate_splash sets the bit, test_fall_in_water tests it before producing another splash and stopping velocity. Retain the neutral name until other typed consumers are assigned the same contract.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.anim_changed

STRONG: Flag used by animation update when the selected animation changes.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit7.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:obj_flags_t.flag_0x100

PROVEN: Ninth object flag written as movement-occurrence bookkeeping, with one original uninitialised producer.

Units: boolean bit. DOS layout/address: +0x1, 1 B.

Storage is obj+0x82 bit0. PROVEN observed write, not a claim that all reads mean has_moved. Do not rename.

Shipped initial sample: 11837 records, 1 values, 0..0. This does not bound runtime values.

### field:cmd_context_t.cmd_offset

STRONG: Saved script command-context component cmd_offset.

Units: script offset or repeat count. DOS layout/address: +0x0, 2 B.

Two signed16 fields; context stack details remain a separate script contract.

### field:cmd_context_t.count

STRONG: Saved script command-context component count.

Units: script offset or repeat count. DOS layout/address: +0x2, 2 B.

Two signed16 fields; context stack details remain a separate script contract.

### field:level_t.objects

PROVEN: Base pointer of the packed level object array.

Units: pointer. DOS layout/address: +0x0, 4 B.

Object stride0x84.

### field:level_t.nb_objects

PROVEN: Count bounding the level object array.

Units: objects. DOS layout/address: +0x4, 2 B.

Signed16 count; struct size6 in DOS, not Android natural size8.

Shipped initial sample: 82 records, 66 values, 27..254. This does not bound runtime values.

### field:level_obj_t.obj_ids

PROVEN: Object-id sequence pointer shared by the level_obj activation-candidate descriptor and the separate level_alw always-type pool descriptor.

Units: object ids. DOS layout/address: +0x0, 4 B.

Signed16 ids/count; swap-remove, no sentinel written by add.

init_struct_level initially puts non-always types in level_obj and always types in level_alw. Despite their names, add_alwobj/del_alwobj change level_obj, adding/removing live spawned always objects from activation candidates. build_active_table evaluates level_obj backwards. No sentinel is maintained by those helpers.

### field:level_obj_t.nb_objects

PROVEN: Signed16 counted length of the id sequence in either level_obj or level_alw.

Units: objects. DOS layout/address: +0x4, 2 B.

Signed16 ids/count; swap-remove, no sentinel written by add.

The descriptor type is shared by two distinct lists. Only level_obj is scanned by moteur; pool discovery uses level_alw elsewhere. The active output is the separate actobj descriptor with its own count and-1 sentinel.

### field:active_objects_t.objects

PROVEN: Counted active-object id array with an additional -1 terminator written at the next slot.

Units: object ids. DOS layout/address: +0x0, 200 B.

100 s16 slots plus s16 count; add writes next -1 sentinel without capacity check.

build_active_table writes active candidates in reverse level_obj order, then -1. DOS checks added>100 after storing, so the terminator/guard does not make100 active objects safe for this100-slot declaration. Do not infer actual maximum live count here.

### field:active_objects_t.num_active_objects

PROVEN: Active-object list member num_active_objects.

Units: object ids or count. DOS layout/address: +0xc8, 2 B.

100 s16 slots plus s16 count; add writes next -1 sentinel without capacity check.

### field:obj_type_flags_t.raw

PROVEN: Overlapping raw/byte/nibble view raw of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 4 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:obj_type_flags_t.flags0

PROVEN: Overlapping raw/byte/nibble view flags0 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:obj_type_flags_t.flags1

PROVEN: Overlapping raw/byte/nibble view flags1 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x1, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:obj_type_flags_t.flags1_lo

PROVEN: Overlapping raw/byte/nibble view flags1_lo of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x1, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:obj_type_flags_t.move_x

PROVEN: Select fractional speed conversion for the corresponding object movement axis.

Units: boolean. DOS layout/address: +0x1, 1 B.

Byte1 bits4/5 = raw0x1000/0x2000; set selects speed in sixteenths with map_time phase distribution. Unset selects integer pixels/tick.

### field:obj_type_flags_t.move_y

PROVEN: Select fractional speed conversion for the corresponding object movement axis.

Units: boolean. DOS layout/address: +0x1, 1 B.

Byte1 bits4/5 = raw0x1000/0x2000; set selects speed in sixteenths with map_time phase distribution. Unset selects integer pixels/tick.

### field:obj_type_flags_t.flags1_hi

PROVEN: Overlapping raw/byte/nibble view flags1_hi of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x1, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:obj_type_flags_t.flags2

PROVEN: Overlapping raw/byte/nibble view flags2 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x2, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:obj_type_flags_t.flags3

PROVEN: Overlapping raw/byte/nibble view flags3 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x3, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

### field:map_data_t.width

PROVEN: Map dimension width in16-pixel tiles.

Units: tiles. DOS layout/address: +0x0, 2 B.

Signed16 bounds; row-major index=y*width+x.

### field:map_data_t.height

PROVEN: Map dimension height in16-pixel tiles.

Units: tiles. DOS layout/address: +0x2, 2 B.

Signed16 bounds; row-major index=y*width+x.

### field:map_data_t.length

STRONG: Number of tile records used by full-map processing.

Units: tiles. DOS layout/address: +0x4, 4 B.

s32; do not infer unused allocation capacity.

### field:map_data_t.map

PROVEN: Pointer to row-major six-byte tile records.

Units: pointer. DOS layout/address: +0x8, 4 B.

Not a pointer to packed collision bytes.

### field:map_tile_t.texture_id

STRONG: Index of a visual16x16 tile texture.

Units: texture index. DOS layout/address: +0x0, 2 B.

u16; renderer sanitises >=1200 to0; shipped data domain measured separately.

### field:map_tile_t.tile_type

PROVEN: Collision/behaviour type byte returned by BTYP.

Units: block type id. DOS layout/address: +0x2, 1 B.

Independent of visual texture id;82 levels use26 distinct values0..31.

### field:map_tile_t.field_3

UNKNOWN: Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x3, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

### field:map_tile_t.transparency

PROVEN: Computed texture coverage classification:0 opaque,1 empty,2 mixed.

Units: classification. DOS layout/address: +0x4, 1 B.

Not a boolean alpha value. Runtime recomputes from pixels; texture0 has a forced-empty case.

### field:map_tile_t.field_5

UNKNOWN: Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x5, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

### field:block_flags_t.raw

PROVEN: Byte/bitfield views of block-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 1 B.

Table indexing uses collision type, not texture.

### field:block_flags_t.bits

PROVEN: Byte/bitfield views of block-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 1 B.

Table indexing uses collision type, not texture.

### field:block_flags_t.bits.unused0

UNKNOWN: Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved.

Units: tile-class bit. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

Do not confuse this bit with bit1 support: underSlope requires foot bit0 together with upper-sample bit6. The historical alias unused0 is retained.

### field:block_flags_t.bits.solid

PROVEN: Bit1 tested as the supporting/solid classification in block selection.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit1 is not sufficient to replace the slope and pass-through logic with a generic solid test.

### field:block_flags_t.bits.unused2

UNKNOWN: Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved.

Units: packed tile-class bits. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

A multi-bit neutral alias is not a single boolean property; keep the raw masks and do not rename/split the declaration to force a meaning.

### field:block_flags_t.bits.chdir

PROVEN: Bit5 of the probed block flags, returned by the turn-marker test.

Units: boolean. DOS layout/address: +0x0, 1 B.

Turn-marker interpretation also agrees with the original name; preserve lookup and probe offsets.

### field:block_flags_t.bits.unused1

STRONG: Two-bit slope classification/orientation view used during Rayman tile landing.

Units: packed slope bits. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

underSlope samples bit6 from btypes[3]; is_icy_pente tests bit6 and bit3 on one tile. Preserve these different sample relationships.

Pass3 refinement: Raw bit6(0x40) gates landing conversion of vertical speed to horizontal momentum; bit7(0x80) selects addition rather than subtraction. Oracle table sets bit7 on left slope types3/6/7/19/22/23 and clears it on right2/4/5/18/20/21. This refines the earlier unresolved bit7 role without renaming the packed view.

### field:rayevts_t.poing

STRONG: Enable the fist-throwing ability.

Units: boolean. DOS layout/address: +0x0, 1 B.

Can be removed temporarily by scripted events; not necessarily a permanent unlock.

### field:rayevts_t.hang

STRONG: Enable the ledge-hanging ability.

Units: boolean. DOS layout/address: +0x0, 1 B.

Disabled by forced-run mode at the relevant call sites.

### field:rayevts_t.helico

STRONG: Enable normal/enhanced helicopter response (helico).

Units: boolean. DOS layout/address: +0x0, 1 B.

The two bits are distinct; super_helico also affects button-release handling.

### field:rayevts_t.super_helico

STRONG: Enable normal/enhanced helicopter response (super_helico).

Units: boolean. DOS layout/address: +0x0, 1 B.

The two bits are distinct; super_helico also affects button-release handling.

### field:rayevts_t.handstand_dash

UNKNOWN: Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

### field:rayevts_t.handstand

UNKNOWN: Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

### field:rayevts_t.magicseed

STRONG: Enable the level-specific magic-seed ability.

Units: boolean. DOS layout/address: +0x0, 1 B.

PS1 calls the related field grain; DOS/Rayverse naming and seed call sites agree. Shipped seed-enabled level has exactly two seed objects (PORT_READINESS B19).

### field:rayevts_t.grap

STRONG: Enable grabbing/swinging through the grappling interaction.

Units: boolean. DOS layout/address: +0x0, 1 B.

PS1 same member name; exact interaction contracts remain in ray/collision units.

### field:rayevts_t.run

STRONG: Enable Rayman running behaviour.

Units: boolean. DOS layout/address: +0x1, 1 B.

Forced-run processing may set this bit; not solely a saved ability flag.

### field:rayevts_t.tiny

STRONG: Rayman small-size mode/power bit.

Units: boolean. DOS layout/address: +0x1, 1 B.

Byte1 bit1; selects4-pixel instead of8-pixel side probes for type23.

### field:rayevts_t.firefly

STRONG: Enable the firefly/light companion mode.

Units: boolean. DOS layout/address: +0x1, 1 B.

PS1 related field luciole; mode also controls clipping/level behaviour.

### field:rayevts_t.force_run

PROVEN: Two-bit forced-running lifecycle state.

Units: state code. DOS layout/address: +0x1, 1 B.

0 inactive,1 starts forced run then becomes2,3 requests release when grounded; directional response is constrained while nonzero. Preserve all four values.

### field:rayevts_t.reverse

PROVEN: Two-bit reversed-controls lifecycle state whose nonzero values swap horizontal input directions.

Units: state code. DOS layout/address: +0x1, 1 B.

Width is2 bits, not bool; do not discard script lifecycle values merely because input processing tests zero/nonzero.

### field:rayevts_t.squashed

STRONG: Reduced/squashed Rayman mode coupled to tiny and object/ability-state swapping.

Units: boolean. DOS layout/address: +0x1, 1 B.

PS1 related slot is named unused_death, which does not describe this DOS behaviour; DOS usage wins.

### field:finbosslevel_t.bzzit

STRONG: Progression flag for completion of the bzzit encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.moskito

STRONG: Progression flag for completion of the moskito encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.mr_sax

STRONG: Progression flag for completion of the mr_sax encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.mr_stone

STRONG: Progression flag for completion of the mr_stone encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.viking_mama

STRONG: Progression flag for completion of the viking_mama encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.space_mama

STRONG: Progression flag for completion of the space_mama encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.mr_skops

STRONG: Progression flag for completion of the mr_skops encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.mr_dark

STRONG: Progression flag for completion of the mr_dark encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

### field:finbosslevel_t.crazy_drummer

UNKNOWN: Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x1, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 2 source member-token matches are retained for review, including possible other types.

### field:finbosslevel_t.helped_joe_1

STRONG: Progression flag for the corresponding help event (helped_joe_1).

Units: boolean. DOS layout/address: +0x1, 1 B.

Flags are also reconstructed from selected world/level in cheat/start flows; distinguish from current scene visibility.

### field:finbosslevel_t.helped_joe_2

STRONG: Progression flag for the corresponding help event (helped_joe_2).

Units: boolean. DOS layout/address: +0x1, 1 B.

Flags are also reconstructed from selected world/level in cheat/start flows; distinguish from current scene visibility.

### field:finbosslevel_t.helped_musician

STRONG: Progression flag for the corresponding help event (helped_musician).

Units: boolean. DOS layout/address: +0x1, 1 B.

Flags are also reconstructed from selected world/level in cheat/start flows; distinguish from current scene visibility.

### field:poing_t.y_16

PROVEN: Fist signed32 vertical position accumulator scaled by16, separately maintained from integer obj.y.

Units: sixteenths of pixel. DOS layout/address: +0x0, 4 B.

s32; shifted right4 for displayed/world integer y; separate from ordinary obj.y.

MOVE_OBJECT adds ashl16(speed_y,4), then uses signed >>4 for obj.y. The shift is performed at16-bit width before promotion; preserve this distinction.

### field:poing_t.field_4

UNKNOWN: Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x4, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 15 source member-token matches are retained for review, including possible other types.

### field:poing_t.speed_x

PROVEN: Stored positive horizontal fist speed used to launch and constrain return speed.

Units: pixels/tick. DOS layout/address: +0x6, 2 B.

Launch choices5/8/11 with player-speed adjustment capped at16; direction applied separately in object speed_x.

### field:poing_t.charge

PROVEN: Fist charge counter before throwing, then scaled outbound budget decremented during flight.

Units: charge ticks then scaled budget. DOS layout/address: +0x8, 2 B.

Starts at5 (or32 airborne), caps at63 before throw; throw shifts by8 and reuses the same s16 field. No single-scale rename.

### field:poing_t.field_A

PROVEN: Temporary throw-range denominator then decrement applied to the scaled outbound charge budget.

Units: context-dependent budget units. DOS layout/address: +0xa, 2 B.

Computed from charge and speed, then overwritten by shifted_charge*speed/denominator. Meaning is computationally proven; retain neutral name because the field has successive roles.

### field:poing_t.sub_etat

STRONG: Saved/selected fist upgrade substate copied into fist objects and save data.

Units: substate selector. DOS layout/address: +0xc, 1 B.

Distinct from the current flight animation substate on poing_obj.

### field:poing_t.is_returning

PROVEN: Select the homing-return phase of fist flight.

Units: boolean-like byte. DOS layout/address: +0xd, 1 B.

Set when horizontal speed reaches zero; return path steers towards current Rayman rectangle.

### field:poing_t.is_active

PROVEN: Enable fist update/collision processing.

Units: boolean-like byte. DOS layout/address: +0xe, 1 B.

Separate byte from poing_obj is_active and flags.alive; consumers use both.

### field:poing_t.damage

PROVEN: Current fist damage applied by obj_hurt.

Units: hit points. DOS layout/address: +0xf, 1 B.

Unsigned byte.

### field:poing_t.is_charging

STRONG: Fist charge-preparation flag.

Units: boolean-like byte. DOS layout/address: +0x10, 1 B.

Set before charging, cleared when thrown.

### field:poing_t.is_boum

STRONG: Fist impact/explosion interaction state.

Units: boolean-like byte. DOS layout/address: +0x11, 1 B.

Suppresses ordinary collision handling and influences return transition; do not rename to a cosmetic-only effect flag.

### field:poing_t.field_12

UNKNOWN: Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x12, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

### field:poing_t.field_13

UNKNOWN: Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x13, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

### field:font_t.sprites

PROVEN: Base sprite table for the corresponding font bank.

Units: pointer. DOS layout/address: +0x0, 4 B.

alpha/alpha2/alpha_numbers are separate banks.

### field:font_t.img_buffer

STRONG: Font image buffer or sprite count (img_buffer) copied from loaded object descriptors.

Units: pointer or count. DOS layout/address: +0x4, 4 B.

DOS font descriptor size10, Android natural size12.

### field:font_t.nb_sprites

STRONG: Font image buffer or sprite count (nb_sprites) copied from loaded object descriptors.

Units: pointer or count. DOS layout/address: +0x8, 2 B.

DOS font descriptor size10, Android natural size12.

### field:display_item_t.text

PROVEN: Fixed200-byte text buffer interpreted by the game glyph decoder.

Units: encoded bytes. DOS layout/address: +0x0, 200 B.

Not UTF-8; / delimiter and ]ddd numeric escape have game-specific meaning.

### field:display_item_t.centered_x_pos

PROVEN: Computed text-box rectangle component centered_x_pos.

Units: screen pixels. DOS layout/address: +0xc8, 2 B.

Includes original +2/-2 padding conventions; s16 output.

### field:display_item_t.centered_y_pos

PROVEN: Computed text-box rectangle component centered_y_pos.

Units: screen pixels. DOS layout/address: +0xca, 2 B.

Includes original +2/-2 padding conventions; s16 output.

### field:display_item_t.width

PROVEN: Computed text-box rectangle component width.

Units: screen pixels. DOS layout/address: +0xcc, 2 B.

Includes original +2/-2 padding conventions; s16 output.

### field:display_item_t.height

PROVEN: Computed text-box rectangle component height.

Units: screen pixels. DOS layout/address: +0xce, 2 B.

Includes original +2/-2 padding conventions; s16 output.

### field:display_item_t.xpos

PROVEN: Text anchor coordinate xpos.

Units: screen pixels. DOS layout/address: +0xd0, 2 B.

Baseline/centering behaviour depends on slash markup.

### field:display_item_t.ypos

PROVEN: Text anchor coordinate ypos.

Units: screen pixels. DOS layout/address: +0xd2, 2 B.

Baseline/centering behaviour depends on slash markup.

### field:display_item_t.font_size

PROVEN: Font selector controlling bank and line metrics.

Units: font selector. DOS layout/address: +0xd4, 1 B.

Supported0/1/2; line heights36/23/15 respectively, no general size parameter.

### field:display_item_t.field_D5

PROVEN: Byte selecting the palette255 box branch instead of the ordinary palette254/no-box branch.

Units: boolean-like selector. DOS layout/address: +0xd5, 1 B.

Observed effect proven, broader intended UI meaning unknown; keep neutral name. All located direct source stores are0.

### field:display_item_t.is_fond

PROVEN: Box drawing/position mode passed to DISPLAY_BLACKBOX and used to offset text by +/-1.

Units: mode. DOS layout/address: +0xd6, 1 B.

Values1/2 select negative/positive one-pixel text offsets in field_D5 branch; keep existing name rather than invent background semantics.

### field:display_item_t.color

STRONG: Text palette/color selector passed to display_text.

Units: palette selector. DOS layout/address: +0xd7, 1 B.

s8 storage passed through u8 cast; preserve bit pattern.

### field:status_bar_t.lives

PROVEN: Signed life counter with -1 display sentinel and upper gameplay cap99.

Units: lives. DOS layout/address: +0x0, 2 B.

DO_FIXE displays00 for-1; Add_One_RAY_lives caps99 and restores health.

### field:status_bar_t.lives_digits

PROVEN: Tens then units decimal display digits of lives.

Units: digits. DOS layout/address: +0x2, 2 B.

DO_FIXE writes slot0=tens, slot1=units; both zero for lives==-1.

### field:status_bar_t.hp_sprites

PROVEN: Two HUD health-sprite selectors, slot1 for the first two health points and slot0 for extra health.

Units: sprite selector. DOS layout/address: +0x4, 2 B.

Do not reverse these slots; max_hitp==2 always leaves slot0 zero.

### field:status_bar_t.num_wiz

PROVEN: Collected tings count used by the HUD and bonus/checkpoint rules.

Units: tings. DOS layout/address: +0x6, 1 B.

DO_FIXE caps at99; INIT_MOTEUR preserves this byte across one checkpoint restore branch.

### field:status_bar_t.wiz_digits

PROVEN: Tens then units decimal display digits for the tings count.

Units: digits. DOS layout/address: +0x7, 2 B.

Derived after capping num_wiz at99.

### field:status_bar_t.max_hitp

PROVEN: HUD maximum health class, normally2 or4.

Units: health points. DOS layout/address: +0x9, 1 B.

Add_One_RAY_lives restores4 only for exactly4, otherwise2; do not treat arbitrary values as a direct refill amount.

### field:save_state_t.triggered_objects

PROVEN: Alive-bit snapshot for object types141,164,179,242 indexed by object id bit.

Units: object bitset. DOS layout/address: +0x0, 32 B.

Eight u32 words; shift is id&31 and index id>>5, distinct from basic saveobj two-bit packing.

### field:save_state_t.nb_floc

PROVEN: Saved eight weather-layer flake counts.

Units: flakes per layer. DOS layout/address: +0x20, 16 B.

saveGameState/restoreGameState copy corresponding nb_floc entries.

### field:save_state_t.vent_x

PROVEN: Saved VENT_X weather/wind value.

Units: weather units. DOS layout/address: +0x30, 2 B.

Signed16 copy; do not assign pixel scale from spelling.

### field:save_state_t.vent_y

PROVEN: Saved VENT_Y weather/wind value.

Units: weather units. DOS layout/address: +0x32, 2 B.

Signed16 copy; do not assign pixel scale from spelling.

### field:save_state_t.x_map

PROVEN: Saved horizontal camera origin.

Units: pixels. DOS layout/address: +0x34, 2 B.

Signed16 copy of xmap.

### field:save_state_t.y_map

PROVEN: Saved vertical camera origin.

Units: pixels. DOS layout/address: +0x36, 2 B.

Signed16 copy of ymap.

### field:save_state_t.ray_x_pos

PROVEN: Saved Rayman world x narrowed to signed16.

Units: pixels. DOS layout/address: +0x38, 2 B.

Live obj.x is signed32; checkpoint narrowing is explicit.

### field:save_state_t.ray_y_pos

PROVEN: Saved Rayman world y narrowed to signed16.

Units: pixels. DOS layout/address: +0x3a, 2 B.

Live obj.y is signed32; checkpoint narrowing is explicit.

### field:save_state_t.ray_screen_x

PROVEN: Saved Rayman screen x.

Units: pixels. DOS layout/address: +0x3c, 2 B.

Direct copy of ray.screen_x.

### field:save_state_t.ray_screen_y

PROVEN: Saved Rayman screen y.

Units: pixels. DOS layout/address: +0x3e, 2 B.

Direct copy of ray.screen_y.

### field:save_state_t.ray_flip_x

PROVEN: Saved Rayman facing flag widened to u16.

Units: boolean. DOS layout/address: +0x40, 2 B.

Restored into the one-bit facing field.

### field:save_state_t.save_obj_x_pos

PROVEN: Saved checkpoint-object world x narrowed to signed16.

Units: pixels. DOS layout/address: +0x42, 2 B.

Only meaningful when save_obj_id!=-1.

### field:save_state_t.save_obj_y_pos

PROVEN: Saved checkpoint-object world y narrowed to signed16.

Units: pixels. DOS layout/address: +0x44, 2 B.

Only meaningful when save_obj_id!=-1.

### field:save_state_t.save_obj_id

PROVEN: Checkpoint-object id, with-1 when saveGameState receives no object.

Units: object id / -1. DOS layout/address: +0x46, 2 B.

Other checkpoint-object fields are not refreshed in the null-object branch.

### field:save_state_t.link_init

PROVEN: Snapshot of live circular successor ids, stored as u16.

Units: object ids. DOS layout/address: +0x48, 512 B.

Copied for level.nb_objects entries. restore_gendoor_link uses a u8 intermediate in the dead-door branch.

### field:save_state_t.save_obj_detect_zone_flag

PROVEN: Saved checkpoint-object detection-zone classification.

Units: classification. DOS layout/address: +0x248, 1 B.

Only meaningful when save_obj_id!=-1.

### field:save_state_t.save_obj_flag_1

PROVEN: Saved checkpoint-object hurtbyfist bit, despite the neutral numbered spelling.

Units: boolean. DOS layout/address: +0x249, 1 B.

saveGameState copies flags.hurtbyfist and restoreGameState writes it back.

### field:save_state_t.ray_coll_btype

PROVEN: Saved five Rayman tile-collision sample types.

Units: tile types. DOS layout/address: +0x24a, 5 B.

Copies ray.btypes[0..4] in order.

### field:save_state_t.ray_anim_index

PROVEN: Saved Rayman animation selector.

Units: animation index. DOS layout/address: +0x24f, 1 B.

Direct byte copy.

### field:save_state_t.ray_anim_frame

PROVEN: Saved Rayman animation frame selector.

Units: frame index. DOS layout/address: +0x250, 1 B.

Direct byte copy.

### field:save_state_t.ray_main_etat

PROVEN: Saved Rayman main state.

Units: state index. DOS layout/address: +0x251, 1 B.

Direct byte copy.

### field:save_state_t.ray_sub_etat

PROVEN: Saved Rayman substate.

Units: state index. DOS layout/address: +0x252, 1 B.

Direct byte copy.

### field:save_state_t.poing_sub_etat

PROVEN: Saved fist power/substate selection.

Units: fist state. DOS layout/address: +0x253, 1 B.

Restore maps selected values and writes poing_obj.init_sub_etat as well as poing.sub_etat.

### field:save_state_t.rayevts_reverse

PROVEN: Saved two-bit RayEvts.reverse value widened to a byte.

Units: control state. DOS layout/address: +0x254, 1 B.

INIT_RAY and checkpoint restore inspect this field; preserve values beyond boolean storage.

### field:save_state_t.rayevts_super_helico

PROVEN: Saved RayEvts.super_helico bit.

Units: boolean. DOS layout/address: +0x255, 1 B.

Restored independently of some other abilities.

### field:save_state_t.rayevts_poing

PROVEN: Saved RayEvts.poing ability bit.

Units: boolean. DOS layout/address: +0x256, 1 B.

Restored only in its checkpoint branch.

### field:save_state_t.current_pal_id

PROVEN: Saved palette selector.

Units: palette selector. DOS layout/address: +0x257, 1 B.

Restoration forces current_pal_id to this value+1 before actualization.

### field:save_state_t.dead_time

PROVEN: Saved entry/death sequence counter.

Units: sequence calls. DOS layout/address: +0x258, 1 B.

Restored into the byte global.

### field:save_state_t.is_just_saved

PROVEN: Checkpoint-valid marker set after a save image is filled.

Units: boolean. DOS layout/address: +0x259, 1 B.

initGameSave/doneGameSave clear the primary marker; restoreGameState is gated by this value.

### field:save_state_t.status_bar_tings

PROVEN: Saved HUD tings count.

Units: tings. DOS layout/address: +0x25a, 1 B.

One engine-entry branch preserves the live count across restoration.

### field:save_state_t.field_25b

UNKNOWN: Trailing checkpoint byte with no typed reader/writer found.

Units: unknown. DOS layout/address: +0x25b, 1 B.

Record extent is604 bytes; no padding or persistence meaning is inferred.

### field:record_t.current_offset

STRONG: Current byte cursor into the recorded demo stream.

Units: bytes. DOS layout/address: +0x0, 4 B.

Corroborated by typed input.c consumers; not advanced in moteur.

### field:record_t.repeat_length

STRONG: Length of the current repeated demo-input run.

Units: input updates. DOS layout/address: +0x4, 4 B.

input.c run decoding supplies the interpretation; moteur only reads is_recording/is_playing.

### field:record_t.repeat_index

STRONG: Progress within the current repeated demo-input run.

Units: input updates. DOS layout/address: +0x8, 4 B.

Preserve the original input decoder comparison/order.

### field:record_t.length

STRONG: Recorded demo stream length used by input playback.

Units: bytes. DOS layout/address: +0xc, 4 B.

This is storage/decoder extent, not RunTimeDemo.

### field:record_t.data

STRONG: Demo stream byte buffer pointer.

Units: pointer. DOS layout/address: +0x10, 4 B.

Owned by the loader/input path; moteur does not allocate or free it.

### field:record_t.is_recording

PROVEN: Nonzero while demo input recording is enabled.

Units: boolean. DOS layout/address: +0x14, 1 B.

INIT_RAY uses it to retain magicseed outside the special level.

### field:record_t.is_playing

PROVEN: Nonzero while recorded demo input is being played.

Units: boolean. DOS layout/address: +0x15, 1 B.

DONE_MOTEUR_LEVEL clears it when fee_obj_id!=-1.

### field:record_t.is_finished

PROVEN: Request to stop demo playback at the current repeat-run boundary.

Units: boolean. DOS layout/address: +0x16, 1 B.

Cheat handling sets1; DoDemo stops when repeat_index reaches repeat_length and this flag is1. It is not only an after-playback status.

### field:record_t.unknown_17

UNKNOWN: Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x17, 1 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:mem_t.align

PROVEN: Used-byte bump offset relative to arena data, despite the alignment-oriented field name.

Units: bytes. DOS layout/address: +0x0, 4 B.

block_malloc returns data+align and advances align. align_mem_desc resets it to (u32)data&3 rather than aligning the current cursor; teardown therefore resets the arena.

### field:mem_t.data

PROVEN: Arena base allocation pointer, with the live bump offset stored separately in align.

Units: pointer. DOS layout/address: +0x4, 4 B.

init_memory assigns malloc result; block_malloc adds align; free_mem clears the pointer. Names are retained.

### field:mem_t.capacity

PROVEN: Total arena byte capacity against which the bump offset is checked.

Units: bytes. DOS layout/address: +0x8, 4 B.

block_malloc compares align+adjusted_size with capacity and leaves capacity unchanged. Temporary-block descriptors with the same member token are different types.

### field:pix_gerbe_item_t.x_pos

UNKNOWN: Declared pix_gerbe_item_t.x_pos storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x0, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:pix_gerbe_item_t.y_pos

UNKNOWN: Declared pix_gerbe_item_t.y_pos storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x2, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:pix_gerbe_item_t.speed_x

UNKNOWN: Declared pix_gerbe_item_t.speed_x storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x4, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:pix_gerbe_item_t.speed_y

UNKNOWN: Declared pix_gerbe_item_t.speed_y storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x6, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:pix_gerbe_item_t.y_accel

UNKNOWN: Declared pix_gerbe_item_t.y_accel storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x8, 1 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:pix_gerbe_item_t.unk_1

UNKNOWN: Declared pix_gerbe_item_t.unk_1 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x9, 1 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:pix_gerbe_t.items

PROVEN: 64-item particle storage view.

Units: aggregate storage. DOS layout/address: +0x0, 640 B.

INIT_MOTEUR zeros the entire item storage via words and clears is_active; views alias exactly, not separate arrays.

### field:pix_gerbe_t.words

PROVEN: 320-word alias of the same640-byte particle item storage.

Units: aggregate storage. DOS layout/address: +0x0, 640 B.

INIT_MOTEUR zeros the entire item storage via words and clears is_active; views alias exactly, not separate arrays.

### field:pix_gerbe_t.is_active

PROVEN: Signed16 active marker for a particle batch.

Units: active marker. DOS layout/address: +0x280, 2 B.

INIT_MOTEUR zeros the entire item storage via words and clears is_active; views alias exactly, not separate arrays.

### field:obj_procs_t.command

PROVEN: Object command/update callback.

Units: callback pointer. DOS layout/address: +0x0, 4 B.

DO_ONE_OBJECT dispatches this slot through global ot.

### field:obj_procs_t.poing_collision

PROVEN: Object fist-collision callback receiving a signed16 sprite selector.

Units: callback pointer. DOS layout/address: +0x4, 4 B.

Collision1 callers dispatch through this slot.

### field:obj_procs_t.rayman_collision

PROVEN: Object Rayman-collision callback.

Units: callback pointer. DOS layout/address: +0x8, 4 B.

Collision1 callers dispatch through this slot.

### field:obj_procs_t.rayman_in_zone

PROVEN: Object Rayman-in-detection-zone callback.

Units: callback pointer. DOS layout/address: +0xc, 4 B.

DO_RAY_IN_ZONE dispatches only after OBJ_IN_ZONE accepts.

### field:obj_procs_t.u_turn

STRONG: Fifth callback slot, initialized with turn-handler function pointers.

Units: callback pointer. DOS layout/address: +0x10, 4 B.

DOS initial table has261 ObjectUTurnDefault targets and one blkUTurn target here. No direct DOS call reference to this slot was found; makeUturn does not dispatch it.

### field:flocon_t.field_0

UNKNOWN: Declared flocon_t.field_0 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x0, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:flocon_t.field_2

UNKNOWN: Declared flocon_t.field_2 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x2, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:flocon_t.field_4

UNKNOWN: Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x4, 2 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:flocon_t.field_6

UNKNOWN: Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x6, 1 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:flocon_t.field_7

UNKNOWN: Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it.

Units: unknown. DOS layout/address: +0x7, 1 B.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

### field:zdc_t.x_pos

PROVEN: Collision-zone horizontal offset in the selected coordinate frame.

Units: pixels. DOS layout/address: +0x0, 2 B.

If flags&2, relative to the referenced sprite box; otherwise relative to object x. Flip mirrors around sprite width or twice the object x+offset_bx anchor.

### field:zdc_t.y_pos

PROVEN: Collision-zone vertical offset in the selected coordinate frame.

Units: pixels. DOS layout/address: +0x2, 2 B.

Added to sprite y for flags&2 or to object y otherwise. Vertical mirroring is not performed by BOX_IN_COLL_ZONES.

### field:zdc_t.width

PROVEN: Unsigned horizontal span of this collision rectangle.

Units: pixels. DOS layout/address: +0x4, 1 B.

Passed unchanged to inclusive inter_box; right boundary is x+width. Also used when reflecting horizontal origin.

### field:zdc_t.height

PROVEN: Unsigned vertical span of this collision rectangle.

Units: pixels. DOS layout/address: +0x5, 1 B.

Passed unchanged to inclusive inter_box; bottom boundary is y+height.

### field:zdc_t.flags

PROVEN: Collision-zone mode flags.

Units: bit mask. DOS layout/address: +0x6, 1 B.

Mask2 selects sprite-local coordinates and mask4 restricts to fist type94. Other bits have no assigned meaning; initial-domain bits are archived separately and do not prove behavioural relevance.

### field:zdc_t.sprite

PROVEN: Animation-layer anchor index and collision-result code.

Units: layer index / result code. DOS layout/address: +0x7, 1 B.

For sprite-local zones indexes current frame's layers and GET_SPRITE_POS. Returned as the hit code even for object-relative zones. This is a layer ordinal, not the sprite-table sprite_index byte.

### field:options_jeu_t.test_fire1

STRONG: Current jump input-test callback.

Units: s16(void) function pointer. DOS layout/address: +0x0, 4 B.

Rayman calls this member indirectly and tests the signed16 return for nonzero. Binding to physical keys/buttons is runtime configuration, not encoded by the member name alone.

### field:options_jeu_t.test_fire0

STRONG: Current fist input-test callback.

Units: s16(void) function pointer. DOS layout/address: +0x4, 4 B.

Rayman calls this member indirectly and tests the signed16 return for nonzero. Binding to physical keys/buttons is runtime configuration, not encoded by the member name alone.

### field:options_jeu_t.test_button4

STRONG: Current empty fourth-action input-test callback.

Units: s16(void) function pointer. DOS layout/address: +0x8, 4 B.

Rayman calls this member indirectly and tests the signed16 return for nonzero. Binding to physical keys/buttons is runtime configuration, not encoded by the member name alone.

### field:options_jeu_t.test_button3

STRONG: Current action/run/seed input-test callback.

Units: s16(void) function pointer. DOS layout/address: +0xc, 4 B.

Rayman calls this member indirectly and tests the signed16 return for nonzero. Binding to physical keys/buttons is runtime configuration, not encoded by the member name alone.

### field:options_jeu_t.jump

PROVEN: Physical/logical button selector for test_fire1.

Units: button selector. DOS layout/address: +0x10, 2 B.

POINTEUR_BOUTONS_OPTIONS_BIS dispatches0..3 to but0pressed..but3pressed; other values leave the callback unchanged. This is a configurable button selector, not a raw key scancode.

### field:options_jeu_t.fist

PROVEN: Physical/logical button selector for test_fire0.

Units: button selector. DOS layout/address: +0x12, 2 B.

POINTEUR_BOUTONS_OPTIONS_BIS dispatches0..3 to but0pressed..but3pressed; other values leave the callback unchanged. This is a configurable button selector, not a raw key scancode.

### field:options_jeu_t.field_14

PROVEN: Physical/logical button selector for test_button4.

Units: button selector. DOS layout/address: +0x14, 2 B.

POINTEUR_BOUTONS_OPTIONS_BIS dispatches0..3 to but0pressed..but3pressed; other values leave the callback unchanged. This is a configurable button selector, not a raw key scancode.

### field:options_jeu_t.action

PROVEN: Physical/logical button selector for test_button3.

Units: button selector. DOS layout/address: +0x16, 2 B.

POINTEUR_BOUTONS_OPTIONS_BIS dispatches0..3 to but0pressed..but3pressed; other values leave the callback unchanged. This is a configurable button selector, not a raw key scancode.

### field:options_jeu_t.music_enabled

UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope.

Units: unknown. DOS layout/address: +0x18, 2 B.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

### field:options_jeu_t.sound_volume

UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope.

Units: unknown. DOS layout/address: +0x1a, 2 B.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

### field:options_jeu_t.is_stereo

UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope.

Units: unknown. DOS layout/address: +0x1c, 2 B.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

### field:options_jeu_t.unknown_1e

UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope.

Units: unknown. DOS layout/address: +0x1e, 2 B.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

### field:ray_stack_t.x_pos

PROVEN: Snapshot of Rayman x_pos.

Units: pixels (signed16). DOS layout/address: +0x0, 2 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.y_pos

PROVEN: Snapshot of Rayman y_pos.

Units: pixels (signed16). DOS layout/address: +0x2, 2 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.poing_x_pos

PROVEN: Snapshot of fist x_pos.

Units: pixels (signed16). DOS layout/address: +0x4, 2 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.poing_y_pos

PROVEN: Snapshot of fist y_pos.

Units: pixels (signed16). DOS layout/address: +0x6, 2 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.main_etat

PROVEN: Snapshot of Rayman main_etat.

Units: state/frame/index byte. DOS layout/address: +0x8, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.sub_etat

PROVEN: Snapshot of Rayman sub_etat.

Units: state/frame/index byte. DOS layout/address: +0x9, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.anim_index

PROVEN: Snapshot of Rayman anim_index.

Units: state/frame/index byte. DOS layout/address: +0xa, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.anim_frame

PROVEN: Snapshot of Rayman anim_frame.

Units: state/frame/index byte. DOS layout/address: +0xb, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.flip_x

PROVEN: Snapshot of Rayman flip_x.

Units: state/frame/index byte. DOS layout/address: +0xc, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.poing_anim_index

PROVEN: Snapshot of fist anim_index.

Units: state/frame/index byte. DOS layout/address: +0xd, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.poing_anim_frame

PROVEN: Snapshot of fist anim_frame.

Units: state/frame/index byte. DOS layout/address: +0xe, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.poing_flip_x

PROVEN: Snapshot of fist flip_x.

Units: state/frame/index byte. DOS layout/address: +0xf, 1 B.

stackRay writes and doBlackRaymanCommand restores the corresponding live member. Fist-only fields are updated only when the validity byte is nonzero. Positions narrow from live signed32 to signed16.

### field:ray_stack_t.poing_is_active

PROVEN: Validity byte for the optional fist snapshot.

Units: boolean. DOS layout/address: +0x10, 1 B.

Writer copies poing.is_active. Consumer must ignore stale fist coordinates/animation fields when0.

### field:ray_stack_t.scale

PROVEN: Low8 bits of Rayman scale at the history sample.

Units: scale modulo256. DOS layout/address: +0x11, 1 B.

Writer narrows live u16 scale to byte; replay zero-extends it to live u16. Tiny scale256 therefore records0. Preserve this lossy history representation.

