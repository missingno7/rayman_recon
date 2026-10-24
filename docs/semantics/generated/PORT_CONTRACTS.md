# Port-facing contracts for basic.c

Generated from semantics.json. Confidence applies to the stated, scoped meaning; it is separate from STRICT status. 
DOS offsets and packed widths describe the original ABI/assets. They do not require packed native runtime structs.

## function:EOA

PROVEN: Report the terminal animation frame only on a zero phase of its selected animation clock.

Units: see parameters/result contract. DOS layout/address: 1:0d074.

Reverse flag 0x10 makes frame 0 terminal; otherwise frames_count-1. This is a timed event, not a persistent last-frame predicate.

## function:save_objects_flags

PROVEN: Pack alive and is_active into two bits per level object.

Units: see parameters/result contract. DOS layout/address: 1:0d0ec.

16 objects per u32: entry i uses word i>>4 and shift2*(15-(i&15)); low bit within the pair is alive, high bit active. Entry pairs run from most-significant to least-significant bits.

## function:restore_objects_flags

PROVEN: Restore packed alive and active bits for every level object except type 58.

Units: see parameters/result contract. DOS layout/address: 1:0d15c.

Cage flags are deliberately left alone; do not apply the snapshot blindly to all objects.

## function:snapToSprite

PROVEN: Compute follower speeds towards a selected sprite of the other object at its predicted next animation frame.

Units: see parameters/result contract. DOS layout/address: 1:0d204.

Temporarily replaces then restores the target animation fields; moves follower coordinates in signed 16-pixel chunks; speed conversion is per axis and object type.

## function:findfirstObject

PROVEN: Return the first object of the requested type, or the pointer just past the level object array.

Units: see parameters/result contract. DOS layout/address: 1:0d4c4.

No NULL failure result; caller must distinguish one-past-end before dereferencing.

## function:findfirstInactiveObject

PROVEN: Return the first inactive object of the requested type, or the pointer just past the level object array.

Units: see parameters/result contract. DOS layout/address: 1:0d4f4.

Inactive means is_active==0, independent of alive.

## function:sinYspeed

PROVEN: Advance a 12-bit phase and return the sine target displacement minus current displacement from init_y.

Units: see parameters/result contract. DOS layout/address: 1:0d528.

phase=(phase+step)&4095; sinus(phase>>3)*amplitude arithmetic-shifted by 9, then subtract signed-16 y-init_y; return narrows to s16. Preserve the integer costab: its129 stored quarter-wave entries at2:06fe0 range from1 to512, including quantised endpoints.

## function:set_proj_center

PROVEN: Store the signed-16 projection centre coordinates.

Units: see parameters/result contract. DOS layout/address: 1:0d560.

Global mutable centre; GET_RAY_ZDC restores a screen-space centre after its world-space calculation.

## function:get_proj_dist

PROVEN: Return a dimension unchanged with zoom disabled, otherwise apply the integer projection factor.

Units: see parameters/result contract. DOS layout/address: 1:0d570.

factor=65536/(signed_scale+256), then factor*dimension/256 with division toward zero; invalid denominator is not guarded.

## function:get_proj_dist2

PROVEN: Scale a signed dimension by the integer projection factor without consulting zoom_mode.

Units: see parameters/result contract. DOS layout/address: 1:0d5b0.

Arithmetic >>8 differs from /256 for negative products; preserve both helpers.

## function:get_proj_x

PROVEN: Project a signed-16 horizontal coordinate about PROJ_CENTER_X.

Units: see parameters/result contract. DOS layout/address: 1:0d5d8.

Two integer divisions: first 65536/(scale+256), then product/256 toward zero; result narrows to s16.

## function:get_proj_y

PROVEN: Project a signed-16 vertical coordinate about PROJ_CENTER_Y.

Units: see parameters/result contract. DOS layout/address: 1:0d614.

Same rounding and narrowing as get_proj_x.

## function:set_zoom_mode

PROVEN: Store the byte controlling the optional projection branch of get_proj_dist.

Units: see parameters/result contract. DOS layout/address: 1:0d650.

Consumers test zero versus nonzero; this helper does not validate the byte.

## function:inverse_proj_x

PROVEN: Map a horizontal coordinate back through the quantised projection factor.

Units: see parameters/result contract. DOS layout/address: 1:0d658.

Inverse uses the already-truncated divisor; it need not undo forward projection exactly. DOS left shift has two-complement machine behaviour.

## function:inverse_proj_y

PROVEN: Map a vertical coordinate back through the quantised projection factor.

Units: see parameters/result contract. DOS layout/address: 1:0d690.

Same quantised divisor, signed division and narrowing as inverse_proj_x.

## function:vblToEOA

PROVEN: Compute a positive, signed-16-truncated duration from the state animation period, frame count, clock phase and multiplier.

Units: see parameters/result contract. DOS layout/address: 1:0d6c8.

max(1, s16(s16(period*frames_count)*multiplier-clock+1)); ignores current anim_frame, so its name must not imply an exact remaining-frame count.

## function:GET_ANIM_POS

PROVEN: Write the current animation-frame rectangle in world coordinates.

Units: see parameters/result contract. DOS layout/address: 1:0d738.

Horizontal flip reflects around x+offset_bx, using frame width; output pointers are s16.

## function:add_actobj

PROVEN: Append an object id to the active-object id array and write a following -1 sentinel.

Units: see parameters/result contract. DOS layout/address: 1:0d7d8.

No capacity check; count and ids are signed 16-bit; no duplicate suppression.

## function:set_sub_etat

PROVEN: Set the object substate and request animation reset if the selected animation changes.

Units: see parameters/result contract. DOS layout/address: 1:0d808.

Writes change_anim_mode=1 even if the numeric substate is unchanged.

## function:set_main_etat

PROVEN: Set the object main state and request animation reset if the selected animation changes.

Units: see parameters/result contract. DOS layout/address: 1:0d810.

State indices are bytes; preserve per-TU prototypes rather than consolidating ABI declarations.

## function:set_main_and_sub_etat

PROVEN: Set both state indices and request animation reset if the selected animation changes.

Units: see parameters/result contract. DOS layout/address: 1:0d818.

Mode byte is set to 1 before the state stores.

## function:get_center_x

PROVEN: Choose a type-dependent horizontal offset for the turn-marker block probe.

Units: see parameters/result contract. DOS layout/address: 1:0d824.

Not a general sprite centre: table cases include world and follow_sprite-dependent constants.

## function:get_center_y

PROVEN: Choose a type-dependent vertical offset for the turn-marker block probe.

Units: see parameters/result contract. DOS layout/address: 1:0d954.

Blacktoon follow_sprite outside 5..7 leaves the DOS return unassigned; do not silently invent a fallback.

## function:on_block_chdir

PROVEN: Test bit 5 of the block flags at the object position plus supplied probe offsets.

Units: see parameters/result contract. DOS layout/address: 1:0dac8.

Coordinates use arithmetic >>4, unlike calc_btyp_square negative-coordinate division.

## function:CALC_FOLLOW_SPRITE_SPEED

PROVEN: Store current-minus-next selected animation-layer offsets into follow_x and follow_y.

Units: see parameters/result contract. DOS layout/address: 1:0db04.

Stride is layers_per_frame&0x3fff; mirrored x uses the current sprite outer width for both frames.

## function:GET_SPRITE_POS

PROVEN: Write the inner sprite rectangle for a layer of the current object animation when its sprite index and reference id are nonzero.

Units: see parameters/result contract. DOS layout/address: 1:0dbb4.

Layer index is not a sprite-table index. Failure returns 0 and leaves outputs unchanged. X reflection uses object flip XOR sprite flag bit 2, not layer.mirrored.

## function:GET_RAY_ZDC

PROVEN: Write a state-dependent Rayman collision rectangle and optionally project it around the object anchor.

Units: see parameters/result contract. DOS layout/address: 1:0dcfc.

Special eta flag0x40 and states5/7 choose rectangles; nonzero scale applies projection regardless of zoom_mode and changes the global centre twice.

## function:GET_BB1_ZDCs

PROVEN: Derive two BB1 collision rectangles from selected sprite bounds and current animation height.

Units: see parameters/result contract. DOS layout/address: 1:0dec0.

Includes state-dependent layer selection, asymmetric margins, and a later layer-0 override.

## function:calc_obj_dir

PROVEN: Set object flip_x from the sign of Rayman anchor x minus object anchor x.

Units: see parameters/result contract. DOS layout/address: 1:0dfb8.

Equal anchors clear flip_x; it is not equivalent to retaining the previous facing.

## function:OBJ_IN_ZONE

PROVEN: Test the object detection-zone result with type-specific acceptance of values above one.

Units: see parameters/result contract. DOS layout/address: 1:0dff4.

Types0x9/0xa5/3/0x90 require exactly1; all other types accept >=1.

## function:calc_obj_pos

PROVEN: Convert object world coordinates to stored screen coordinates using the camera offsets.

Units: see parameters/result contract. DOS layout/address: 1:0e030.

screen_x=x-xmap+8, screen_y=y-ymap, both narrowed to s16; preserve the +8 bias.

## function:makeUturn

PROVEN: Apply type- and state-specific facing/script changes when an object turns.

Units: see parameters/result contract. DOS layout/address: 1:0e05c.

Label255 means no script jump; when a label is selected, undo this tick x movement, clear speed_x, then skipToLabel. MITE2 deliberately falls through to MITE.

## function:BTYP

PROVEN: Return the collision-type byte of an in-bounds tile, or zero outside the map.

Units: see parameters/result contract. DOS layout/address: 1:0e588.

Signed-16 input coordinates, inclusive range tests; DOS returns full zero-extended s32 in this TU.

## function:calc_btyp_square

PROVEN: Sample centre, left, right, above and below block types around an object anchor.

Units: see parameters/result contract. DOS layout/address: 1:0e608.

Tile size16; ashr16 divides negative positions toward zero. Side distance16 normally,8 for Rayman,4 for tiny Rayman. Airborne centre passes through bloc_floor.

## function:DO_OBJ_REBOND_EN_X

PROVEN: Negate horizontal speed when either side block has flag bit 4.

Units: see parameters/result contract. DOS layout/address: 1:0e76c.

No facing or state update; signed-16 speed storage retains the DOS narrowing behaviour.

## function:calc_btyp

PROVEN: Select the effective supporting block type and perform type-specific side-edge turning and Rayman edge bookkeeping.

Units: see parameters/result contract. DOS layout/address: 1:0e790.

Rayman param uses -1/0/1 here; ray_last_ground_btyp retains its previous value for unhandled types including25; Rayman ray_dist is a tile index in this path.

## function:init_obj_in_the_air

PROVEN: Clear the two byte gravity accumulators.

Units: see parameters/result contract. DOS layout/address: 1:0eaa8.

Does not itself set an airborne state or a vertical speed.

## function:make_my_fruit_go_down

PROVEN: Accelerate horizontal speed below magnitude32 and select the type-dependent falling state.

Units: see parameters/result contract. DOS layout/address: 1:0eab4.

Acceleration is conditional on state2; gravity bytes reset only when changing to the desired main/substate.

## function:switchOff

PROVEN: On the timed animation-end event, clear object alive/active flags and remove its always-object entry with exceptions.

Units: see parameters/result contract. DOS layout/address: 1:0eb18.

Breakout WIZ stays in level_obj; types170/75 are moved to -32000 on both axes.

## function:obj_hurt

PROVEN: Subtract current fist damage from target hit points with saturation at zero.

Units: see parameters/result contract. DOS layout/address: 1:0eb80.

Both damage and HP are unsigned bytes; equality also produces zero.

## function:Projectil_to_RM

PROVEN: Compute facing-constrained projectile speeds towards the stored follow_x/y target, then advance that stored target.

Units: see parameters/result contract. DOS layout/address: 1:0eba0.

Target comes from follow_x/y, not a fresh ray.x/y read. Horizontal-only branch initially chooses the opposite x sign; later facing correction may invert it. Increment target by twice the quotient/sign pair; preserve +16 and +2 special cases.

## function:del_actobj

PROVEN: Clear object activity, find its id in actobj, shift entries using the original j-1 destination, and decrement the count.

Units: see parameters/result contract. DOS layout/address: 1:0ee18.

The loop starts at i and writes objects[j-1], including i-1: unusual DOS behaviour. Keep this specified before designing a safe native container; no absent-id guard.

## function:calc_let_Width

PROVEN: Return the outer glyph width from the selected font or numeric sprite bank.

Units: see parameters/result contract. DOS layout/address: 1:0eeb8.

font_size1 adds41; glyph>=1000 selects alpha_numbers at glyph-1000. Unsupported font_size leaves sprite uninitialised in DOS.

## function:calc_let_Width2

PROVEN: Return glyph inner width plus its low-nibble x inset.

Units: see parameters/result contract. DOS layout/address: 1:0ef1c.

This advance measure differs from calc_let_Width; same bank selection and unsupported-size caveat.

## function:INIT_TEXT_TO_DISPLAY

PROVEN: Clear text and two drawing-control bytes for display slots1 through10.

Units: see parameters/result contract. DOS layout/address: 1:0ef80.

Slot0 is intentionally excluded; 11 physical slots are declared.

## function:deter_num_let

PROVEN: Decode one text byte or a bracket-prefixed three-character numeric sprite escape into a glyph number.

Units: see parameters/result contract. DOS layout/address: 1:0efb4.

Escape introducer is ] (not [); returns atoi(next3)+1000; unknown bytes return0. Extended byte mappings are explicit DOS encoding values.

## function:calc_num_let_spr

PROVEN: Add41 modulo256 to the pointed byte glyph number when font_size equals1.

Units: see parameters/result contract. DOS layout/address: 1:0f194.

The <1000 check is always true for u8; do not widen the pointed type to make the apparent check meaningful.

## function:calc_largmax_text

PROVEN: Accumulate glyph advances from the character after char_index until slash or NUL.

Units: see parameters/result contract. DOS layout/address: 1:0f1b0.

Loop increments before its first test; numeric escapes consume three additional bytes; slash ends a line.

## function:INIT_TXT_BOX

PROVEN: Compute display-box rectangle from text glyph advances and the original slash-delimited line accounting.

Units: see parameters/result contract. DOS layout/address: 1:0f21c.

200-byte local text copy; do not replace the original slash count/final-line logic with conventional line splitting. Empty text leaves rectangle unchanged.

## function:Deter_Option_Caract

PROVEN: Replace the selected text position with a..d for key0..3, optionally searching for the last dollar sign.

Units: see parameters/result contract. DOS layout/address: 1:0f4a0.

offset255 requests search and is also the no-position sentinel; the scan index is u8.

## function:SwapAB

PROVEN: Exchange the pointed signed-16 values.

Units: see parameters/result contract. DOS layout/address: 1:0f50c.

The C parameter and store widths are part of the contract.

## function:Bresenham

PROVEN: Visit spaced integer line points through a signed-16 callback using an endpoint fraction divided by128.

Units: see parameters/result contract. DOS layout/address: 1:0f520.

percent128 is full extent; strict error>0 branch and counter>gap condition matter. Preserve direction normalisation, initial counter and pre-increment quirks; this is not a drop-in standard Bresenham routine.

## function:init_finBossLevel

PROVEN: Clear the twelve named progression bits in finBosslevel.

Units: see parameters/result contract. DOS layout/address: 1:0f880.

Clears named bits only; the upper four bits of its second byte are not named or explicitly cleared here.

## function:Change_Wait_Anim

PROVEN: Swap the state0/0 and state0/56 animation ids, copy the latter into state3/20, and toggle first_boss_meet.

Units: see parameters/result contract. DOS layout/address: 1:0f8a4.

Other eta bytes stay unchanged; first_boss_meet uses arithmetic 1-value.

## function:add_alwobj

PROVEN: Append an object id to the level_obj array and increment its signed-16 count.

Units: see parameters/result contract. DOS layout/address: 1:0f8e8.

Unlike add_actobj, no following -1 sentinel is written.

## function:del_alwobj

PROVEN: Remove the first matching id from level_obj by replacing it with the last counted id.

Units: see parameters/result contract. DOS layout/address: 1:0f918.

Order is not preserved; missing ids leave count and entries unchanged.

## function:FatalError

PROVEN: Stop CD playback, enter text mode, print diagnostics, optionally wait for input, then exit with -1.

Units: see parameters/result contract. DOS layout/address: 1:0f988.

errno is a Watcom runtime accessor expression; its getter returns2:574fc in this DOS build. It has no game-data binding named errno; input_mode==1 suppresses the key wait.

## function:sprite_of_ref

PROVEN: Return the first sprite-table index whose reference id matches, or zero if absent.

Units: see parameters/result contract. DOS layout/address: 1:0fa00.

Search count is cast to u16 although stored as s16; result narrows to u8. Index0 and failure share a result.

## global:horloge

PROVEN: Global phase-counter bank indexed by animation period.

Units: ticks. DOS layout/address: 2:3f9f5.

Low-nibble animation clocks0..15 used here; whole bank25 bytes. EOA triggers at phase0.

## global:level

PROVEN: Current level packed object-array descriptor.

Units: objects. DOS layout/address: 2:3cdf0.

Size6 in DOS; pointer32 and signed16 count.

## global:saveobj

PROVEN: Packed snapshot of object alive/active flags.

Units: two-bit entries. DOS layout/address: 2:3ce08.

16 u32 words;16 object entries per word, most-significant pair first: shift2*(15-(i&15)). Alive is pair bit0 and active pair bit1. No extra capacity claim is inferred.

## global:flags

PROVEN: Per-object-type behaviour flag table.

Units: bitset. DOS layout/address: 2:257c.

262 entries, stride4; axis scale flags0x1000/0x2000.

## global:PROJ_CENTER_X

PROVEN: Global horizontal projection centre.

Units: pixels. DOS layout/address: 2:3f838.

s16; caller changes coordinate space deliberately.

## global:PROJ_CENTER_Y

PROVEN: Global vertical projection centre.

Units: pixels. DOS layout/address: 2:3f836.

s16.

## global:zoom_mode

PROVEN: Optional projection switch read by get_proj_dist.

Units: zero/nonzero. DOS layout/address: 2:3fa63.

Other projection helpers do not consult this byte.

## global:actobj

PROVEN: Current active-object id array and count.

Units: object ids. DOS layout/address: 2:3ef5c.

100 s16 slots and s16 count; trailing -1 sentinel managed by add_actobj.

## global:num_world

STRONG: Current world discriminator used by type-specific turn-probe offsets.

Units: world id. DOS layout/address: 2:3f88c.

This pilot observes1/2/default; does not infer a complete enum.

## global:block_flags

PROVEN: Flags indexed by collision tile type.

Units: bitset. DOS layout/address: 2:253c.

64 one-byte entries; bit1 support,bit4 rebound/side blocking,bit5 turn probe.

## global:ray

PROVEN: Live Rayman object record.

Units: obj_t. DOS layout/address: 2:3e650.

Coordinates are world pixels; ray_dist is a map index in Rayman collision paths.

## global:xmap

PROVEN: Horizontal camera/map origin subtracted from world coordinates.

Units: pixels. DOS layout/address: 2:3f89e.

calc_obj_pos adds an additional8 to screen x.

## global:ymap

PROVEN: Vertical camera/map origin subtracted from world coordinates.

Units: pixels. DOS layout/address: 2:3f8a6.

Signed16 storage.

## global:mp

PROVEN: Current map dimensions and tile-record descriptor.

Units: tiles. DOS layout/address: 2:3e638.

Tile16x16 pixels; record stride6.

## global:RayEvts

STRONG: Packed Rayman ability/mode state.

Units: bitset. DOS layout/address: 2:3f81a.

Tiny bit changes collision probe distance; avoid assuming every bit is a permanent unlock.

## global:ray_last_ground_btyp

PROVEN: Sticky classification of the last recognised supporting surface as ordinary versus slippery.

Units: boolean. DOS layout/address: 2:3fa72.

Despite name, stores0/1 rather than tile type; tile25 and unhandled types retain it. No rename until all consumers are reviewed.

## global:ray_mode

STRONG: Rayman/game mode discriminator.

Units: mode. DOS layout/address: 2:3f868.

Value5 exempts WIZ from always-object removal in switchOff.

## global:poing

STRONG: Rayman fist state including damage and fixed-point vertical accumulator.

Units: mixed. DOS layout/address: 2:3e770.

20-byte DOS descriptor; damage is unsigned byte.

## global:alpha

PROVEN: Sprite font bank selected by font_size2.

Units: font_t. DOS layout/address: 2:3e6e0.

Glyph table with outer/inner widths and pixel insets.

## global:alpha2

PROVEN: Sprite font bank selected by font_size0/1.

Units: font_t. DOS layout/address: 2:3edc8.

font_size1 offsets glyph index by41.

## global:alpha_numbers

PROVEN: Sprite bank selected by glyph numbers>=1000.

Units: font_t. DOS layout/address: 2:3e644.

Index is glyph-1000; numeric ]ddd escapes feed it.

## global:text_to_display

PROVEN: Array of eleven game text display descriptors.

Units: display_item_t[11]. DOS layout/address: 2:3c4a0.

Initialiser clears slots1..10, excluding0.

## global:finBosslevel

STRONG: Packed boss/progression flag record.

Units: bitset. DOS layout/address: 2:3f81e.

Twelve named bits cleared by init_finBossLevel; exact identity of each boss/help event needs progression writers.

## global:first_boss_meet

PROVEN: Byte toggled while swapping Rayman waiting-animation entries.

Units: 0/1 in intended domain. DOS layout/address: 2:3fa71.

Arithmetic1-value; the story meaning suggested by name remains unproven.

## global:level_obj

PROVEN: Always-object id list and count used by activation processing.

Units: object ids. DOS layout/address: 2:3cdf8.

Append and unordered swap-remove, unlike actobj sentinel list.

## global:errno

PROVEN: Watcom runtime error value obtained through __get_errno_ptr.

Units: runtime error code. DOS layout/address: 2:574fc.

Macro expression has no game-data binding named errno. FatalError calls1:704e8, whose getter returns fixed2:574fc, then reads32 bits.

## global:input_mode

PROVEN: Input-mode byte whose value1 suppresses FatalError key waiting.

Units: mode. DOS layout/address: 2:3fa51.

This effect does not establish a complete meaning for its other values.

## field:obj_t.sprites

PROVEN: Pointer to the object-specific sprites table.

Units: pointer. DOS layout/address: +0x0, 4 B.

DOS pointers are32 bits; eta is a two-level main/substate table.

Shipped initial-data sample: 11837 records, 34 distinct values, range 1..39. This is not the full runtime domain.

## field:obj_t.animations

PROVEN: Pointer to the object-specific animations table.

Units: pointer. DOS layout/address: +0x4, 4 B.

DOS pointers are32 bits; eta is a two-level main/substate table.

Shipped initial-data sample: 11837 records, 34 distinct values, range 1..39. This is not the full runtime domain.

## field:obj_t.img_buffer

STRONG: Object sprite pixel-atlas buffer.

Units: pointer. DOS layout/address: +0x8, 4 B.

Pointer32 in DOS; sprite offset_in_atlas indexes the pixel buffer.

Shipped initial-data sample: 11837 records, 34 distinct values, range 1..39. This is not the full runtime domain.

## field:obj_t.eta

PROVEN: Pointer to the object-specific eta table.

Units: pointer. DOS layout/address: +0xc, 4 B.

DOS pointers are32 bits; eta is a two-level main/substate table.

Shipped initial-data sample: 11837 records, 33 distinct values, range 0..33. This is not the full runtime domain.

## field:obj_t.cmds

STRONG: Object script stream/context member cmds.

Units: script dependent. DOS layout/address: +0x10, 4 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial-data sample: 11837 records, 8185 distinct values, range 0..9728522. This is not the full runtime domain.

## field:obj_t.cmd_labels

STRONG: Object script stream/context member cmd_labels.

Units: script dependent. DOS layout/address: +0x14, 4 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial-data sample: 11837 records, 2203 distinct values, range 0..9728146. This is not the full runtime domain.

## field:obj_t.cmd_contexts

STRONG: Object script stream/context member cmd_contexts.

Units: script dependent. DOS layout/address: +0x18, 4 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.field_1C

UNKNOWN: Neutral obj_t.field_1C storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x1c, 4 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 7 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.link_has_gendoor

UNKNOWN: Neutral obj_t.link_has_gendoor storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x20, 4 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 15 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.is_active

PROVEN: Object participation flag saved as the high bit of each two-bit snapshot entry.

Units: boolean. DOS layout/address: +0x24, 4 B.

s32 storage, snapshot preserves only the one-bit domain used here; distinct from flags.alive.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.x

PROVEN: World-space live-object origin coordinate on the x axis, with loader-descriptor reuse.

Units: pixels (live objects). DOS layout/address: +0x28, 4 B.

s32 storage; many consumers deliberately narrow to s16. switchOff writes -32000 sentinel. Loader wldobj descriptors reuse x as image byte length, so the type does not guarantee pixel units for every instance.

Shipped initial-data sample: 11837 records, 3721 distinct values, range -32000..10244. This is not the full runtime domain.

## field:obj_t.y

PROVEN: World-space live-object origin coordinate on the y axis, with loader-descriptor reuse.

Units: pixels (live objects). DOS layout/address: +0x2c, 4 B.

s32 storage; many consumers deliberately narrow to s16. switchOff writes -32000 sentinel. Loader wldobj descriptors reuse x as image byte length, so the type does not guarantee pixel units for every instance.

Shipped initial-data sample: 11837 records, 1707 distinct values, range -32000..4849. This is not the full runtime domain.

## field:obj_t.active_flag

UNKNOWN: Neutral obj_t.active_flag storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x30, 4 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 18 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.id

PROVEN: Object identifier stored in the active and always-object id lists.

Units: object index. DOS layout/address: +0x34, 2 B.

Signed16; do not confuse a list slot with an object id.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.screen_x

PROVEN: Stored screen-space origin coordinate screen_x after camera subtraction.

Units: pixels. DOS layout/address: +0x36, 2 B.

s16; screen_x includes +8 bias.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.screen_y

PROVEN: Stored screen-space origin coordinate screen_y after camera subtraction.

Units: pixels. DOS layout/address: +0x38, 2 B.

s16; screen_x includes +8 bias.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.sub_speed

UNKNOWN: Neutral obj_t.sub_speed storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x3a, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 7 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.init_x

STRONG: Initial position component init_x used by reset and relative-displacement logic.

Units: pixels. DOS layout/address: +0x3c, 2 B.

DOS sinYspeed proves init_y reference position; initial-position naming also matches external object declarations.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.init_y

STRONG: Initial position component init_y used by reset and relative-displacement logic.

Units: pixels. DOS layout/address: +0x3e, 2 B.

DOS sinYspeed proves init_y reference position; initial-position naming also matches external object declarations.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.speed_x

PROVEN: Signed per-axis movement quantity whose scale is selected by object-type flags.

Units: pixels/tick or 1/16 pixel/tick. DOS layout/address: +0x40, 2 B.

Type flags0x1000/0x2000 select instantSpeed. It distributes fractional motion using map_time rather than a per-object fractional coordinate accumulator. Fist has a special movement path.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.speed_y

PROVEN: Signed per-axis movement quantity whose scale is selected by object-type flags.

Units: pixels/tick or 1/16 pixel/tick. DOS layout/address: +0x42, 2 B.

Type flags0x1000/0x2000 select instantSpeed. It distributes fractional motion using map_time rather than a per-object fractional coordinate accumulator. Fist has a special movement path.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.nb_sprites

PROVEN: Count bounding the object sprite-table search.

Units: sprites. DOS layout/address: +0x44, 2 B.

s16 field read as u16 in this helper; its sign must not be silently propagated into the search bound.

Shipped initial-data sample: 11837 records, 51 distinct values, range 3..256. This is not the full runtime domain.

## field:obj_t.cmd_offset

STRONG: Object script stream/context member cmd_offset.

Units: script dependent. DOS layout/address: +0x46, 2 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial-data sample: 11837 records, 3 distinct values, range 0..3. This is not the full runtime domain.

## field:obj_t.nb_cmd

STRONG: Object script stream/context member nb_cmd.

Units: script dependent. DOS layout/address: +0x48, 2 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial-data sample: 11837 records, 17 distinct values, range 0..254. This is not the full runtime domain.

## field:obj_t.follow_id

STRONG: Identifier of an object being followed, with type-specific reuse.

Units: object id or scratch. DOS layout/address: +0x4a, 2 B.

-1 means no followed object for Rayman; MITE writes0 for its own behaviour.

Shipped initial-data sample: 11837 records, 14 distinct values, range -32..32. This is not the full runtime domain.

## field:obj_t.follow_y

PROVEN: Context-dependent scratch value: animation-layer displacement, projectile target coordinate, or other type-specific state.

Units: context dependent. DOS layout/address: +0x4c, 2 B.

No universal target_x/delta_x rename: ray swing also uses follow_x as an angle; preserve role per object/state.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.follow_x

PROVEN: Context-dependent scratch value: animation-layer displacement, projectile target coordinate, or other type-specific state.

Units: context dependent. DOS layout/address: +0x4e, 2 B.

No universal target_x/delta_x rename: ray swing also uses follow_x as an angle; preserve role per object/state.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.param

PROVEN: Type-specific signed scratch parameter, including Rayman edge classification in calc_btyp.

Units: context dependent. DOS layout/address: +0x50, 2 B.

Rayman edge values -1/0/1 do not describe all object types.

Shipped initial-data sample: 11837 records, 3 distinct values, range 0..5. This is not the full runtime domain.

## field:obj_t.link

UNKNOWN: Neutral obj_t.link storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x52, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 208 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.ray_dist

PROVEN: Context-dependent Rayman tile index or signed object-to-Rayman support distance.

Units: tile index or pixels. DOS layout/address: +0x54, 2 B.

ray.ray_dist indexes mp.map; other objects use signed distance and1000/10000 sentinels. Keep the shared name.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.iframes_timer

STRONG: Signed invulnerability/response timer.

Units: ticks. DOS layout/address: +0x56, 2 B.

-1 is a significant inactive sentinel; do not turn into an unsigned countdown.

Shipped initial-data sample: 11837 records, 14 distinct values, range -128..32. This is not the full runtime domain.

## field:obj_t.test_block_index

PROVEN: Last row-major map tile index used by the changed-block test.

Units: tile index. DOS layout/address: +0x58, 2 B.

u16 storage compared through s16; same semantics visible in ufo test helper.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.scale

PROVEN: Projection-depth parameter whose denominator is signed_scale+256.

Units: projection depth units. DOS layout/address: +0x5a, 2 B.

u16 storage passed to s16 scale formals;0 is unity,256 gives half scale. Not a direct percentage or pixel size.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.zdc

UNKNOWN: Neutral obj_t.zdc storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x5c, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 6 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.active_timer

UNKNOWN: Neutral obj_t.active_timer storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x5e, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 12 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.type

PROVEN: Unsigned object type discriminator indexing flags and selecting behaviour.

Units: object type id. DOS layout/address: +0x60, 2 B.

DOS u16; Android/PS1 layout widths must not replace it.

Shipped initial-data sample: 11837 records, 221 distinct values, range 0..257. This is not the full runtime domain.

## field:obj_t.btypes

PROVEN: Five cached collision block types in centre,left,right,above,below order.

Units: block type id. DOS layout/address: +0x62, 5 B.

Five u8 entries, not a cross-shaped array of map indices; centre may be adjusted by bloc_floor.

## field:obj_t.unused_padding

UNKNOWN: Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x67, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.offset_bx

PROVEN: Horizontal object anchor offset used for collision probes and reflection centre.

Units: pixels. DOS layout/address: +0x68, 1 B.

u8; x reflection uses 2*offset_bx, not sprite half-width.

Shipped initial-data sample: 11837 records, 29 distinct values, range 0..192. This is not the full runtime domain.

## field:obj_t.offset_by

PROVEN: Vertical object anchor offset used for foot/block probes and projection centre.

Units: pixels. DOS layout/address: +0x69, 1 B.

u8; object origin and collision anchor are different coordinates.

Shipped initial-data sample: 11837 records, 45 distinct values, range 0..248. This is not the full runtime domain.

## field:obj_t.anim_index

PROVEN: Current animation selector or current frame selector (anim_index).

Units: index. DOS layout/address: +0x6a, 1 B.

u8 storage. anim_index255 is asserted in source by an empty macro; no DOS runtime guard is added.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.anim_frame

PROVEN: Current animation selector or current frame selector (anim_frame).

Units: index. DOS layout/address: +0x6b, 1 B.

u8 storage. anim_index255 is asserted in source by an empty macro; no DOS runtime guard is added.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.sub_etat

PROVEN: Byte index into the main/substate animation-state table (sub_etat).

Units: state index. DOS layout/address: +0x6c, 1 B.

main_etat indexes eta pointers; sub_etat indexes 8-byte eta entries.

Shipped initial-data sample: 11837 records, 49 distinct values, range 0..65. This is not the full runtime domain.

## field:obj_t.main_etat

PROVEN: Byte index into the main/substate animation-state table (main_etat).

Units: state index. DOS layout/address: +0x6d, 1 B.

main_etat indexes eta pointers; sub_etat indexes 8-byte eta entries.

Shipped initial-data sample: 11837 records, 11 distinct values, range 0..10. This is not the full runtime domain.

## field:obj_t.init_sub_etat

STRONG: Reset value for init_sub_etat.

Units: state index or hit points. DOS layout/address: +0x6e, 1 B.

Initial fields are separate from current values; preserve byte widths.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.init_main_etat

STRONG: Reset value for init_main_etat.

Units: state index or hit points. DOS layout/address: +0x6f, 1 B.

Initial fields are separate from current values; preserve byte widths.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.cmd

STRONG: Current script command discriminator.

Units: opcode. DOS layout/address: +0x70, 1 B.

GO_LEFT=0, GO_RIGHT=1 in basic; not a facing bit.

Shipped initial-data sample: 11837 records, 10 distinct values, range 0..30. This is not the full runtime domain.

## field:obj_t.gravity_value_1

STRONG: Byte gravity integration state component gravity_value_1.

Units: context dependent. DOS layout/address: +0x71, 1 B.

Both cleared together; usages differ by state and object, so no stronger shared rename.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.gravity_value_2

STRONG: Byte gravity integration state component gravity_value_2.

Units: context dependent. DOS layout/address: +0x72, 1 B.

Both cleared together; usages differ by state and object, so no stronger shared rename.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.change_anim_mode

PROVEN: Animation transition mode set to1 by the state setters.

Units: mode. DOS layout/address: +0x73, 1 B.

Mode1 requests reset on animation change; do not reset anim_frame immediately in the setter.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.offset_hy

STRONG: Unsigned vertical upper-body/collision anchor offset.

Units: pixels. DOS layout/address: +0x74, 1 B.

DOS zero-extends; Rayverse signed declaration is contradicted by DOS.

Shipped initial-data sample: 11837 records, 34 distinct values, range 0..170. This is not the full runtime domain.

## field:obj_t.follow_sprite

PROVEN: Selected animation layer for following, also overloaded as Blacktoon behaviour selector.

Units: layer index or subtype. DOS layout/address: +0x75, 1 B.

Not a sprite-table reference id; GET_SPRITE_POS takes an animation layer index.

Shipped initial-data sample: 11837 records, 9 distinct values, range 0..8. This is not the full runtime domain.

## field:obj_t.hit_points

PROVEN: Unsigned hit-point byte in obj_hurt, with type-specific reuse including signed deformation angle input.

Units: hit points or type-specific state. DOS layout/address: +0x76, 1 B.

u8 storage; damage subtraction clamps at0. Type254 rendering and collision explicitly reinterpret it as s8 for slope/deformation; do not impose a universal nonnegative-health interpretation.

Shipped initial-data sample: 11837 records, 17 distinct values, range 0..255. This is not the full runtime domain.

## field:obj_t.init_hit_points

STRONG: Reset value for init_hit_points.

Units: state index or hit points. DOS layout/address: +0x77, 1 B.

Initial fields are separate from current values; preserve byte widths.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.init_flag

UNKNOWN: Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x78, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 11 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 8 distinct values, range 0..9. This is not the full runtime domain.

## field:obj_t.hit_sprite

UNKNOWN: Neutral obj_t.hit_sprite storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x79, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 10 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 6 distinct values, range 0..255. This is not the full runtime domain.

## field:obj_t.detect_zone

STRONG: Type-dependent size/range parameter used by the detection-zone computation.

Units: pixels or type-specific range. DOS layout/address: +0x7a, 1 B.

Do not equate this input parameter with detect_zone_flag, the computed classification.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.detect_zone_flag

PROVEN: Detection-zone classification tested by OBJ_IN_ZONE.

Units: classification. DOS layout/address: +0x7b, 1 B.

Exactly1 or >=1 depending on type; not safely interchangeable with bool.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.cmd_context_depth

STRONG: Object script stream/context member cmd_context_depth.

Units: script dependent. DOS layout/address: +0x7c, 1 B.

Keep widths and stack-depth sentinels; exact per-opcode interpretation belongs in the script pilot.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.config

PROVEN: Type-specific byte configuration tested for Blacktoon edge-turning.

Units: context dependent. DOS layout/address: +0x7d, 1 B.

Known local value2 here does not imply a shared enum.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.display_prio

STRONG: Object drawing-order bucket selector.

Units: priority bucket. DOS layout/address: +0x7e, 1 B.

Byte, separate from activity flags; exact bucket policy belongs with display-list generation.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.timer

UNKNOWN: Neutral obj_t.timer storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7f, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 159 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_t.anim_count

STRONG: Number of animations in the object descriptor.

Units: animations. DOS layout/address: +0x80, 1 B.

Byte count copied from loaded descriptor; serialized level values are not a bound on every runtime state index.

Shipped initial-data sample: 11837 records, 28 distinct values, range 0..135. This is not the full runtime domain.

## field:obj_t.flags

PROVEN: Packed two-byte per-object flag record.

Units: bitset. DOS layout/address: +0x81, 2 B.

First byte begins at0x81; ninth named flag is in the next byte.

Shipped initial-data sample: 11837 records, 2 distinct values, range 0..32. This is not the full runtime domain.

## field:obj_t.field_83

UNKNOWN: Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x83, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:eta_t.speed_x_right

STRONG: Signed state-selected horizontal speed for the speed_x_right facing.

Units: object speed units. DOS layout/address: +0x0, 1 B.

s8, sign extended; multiplication by animation clock divisor precedes tiny adjustment.

Shipped initial-data sample: 2639 records, 130 distinct values, range -128..124. This is not the full runtime domain.

## field:eta_t.speed_x_left

STRONG: Signed state-selected horizontal speed for the speed_x_left facing.

Units: object speed units. DOS layout/address: +0x1, 1 B.

s8, sign extended; multiplication by animation clock divisor precedes tiny adjustment.

Shipped initial-data sample: 2639 records, 118 distinct values, range -126..126. This is not the full runtime domain.

## field:eta_t.anim_index

PROVEN: Animation id selected by this main/substate entry.

Units: animation index. DOS layout/address: +0x2, 1 B.

Byte value; a state entry is8 bytes.

Shipped initial-data sample: 2639 records, 148 distinct values, range 0..255. This is not the full runtime domain.

## field:eta_t.next_main_etat

PROVEN: State indices selected when an animation crosses its frame bounds.

Units: state index. DOS layout/address: +0x3, 1 B.

Prediction follows one transition; no recursive transition walk.

Shipped initial-data sample: 2639 records, 104 distinct values, range 0..255. This is not the full runtime domain.

## field:eta_t.next_sub_etat

PROVEN: State indices selected when an animation crosses its frame bounds.

Units: state index. DOS layout/address: +0x4, 1 B.

Prediction follows one transition; no recursive transition walk.

Shipped initial-data sample: 2639 records, 143 distinct values, range 0..255. This is not the full runtime domain.

## field:eta_t.anim_speed

PROVEN: Packed animation timing/control byte whose low nibble selects the clock.

Units: clock index/control bits. DOS layout/address: +0x5, 1 B.

Low nibble0..15 is also used as a nominal period; high nibble has separate state-specific uses.

Shipped initial-data sample: 2639 records, 120 distinct values, range 0..255. This is not the full runtime domain.

## field:eta_t.anim_speed_bits

PROVEN: Overlapping nibble/bit alias container for the corresponding eta byte.

Units: bitset. DOS layout/address: +0x5, 1 B.

These containers have no independent storage or independent meaning; see individual bits and raw bytes.

## field:eta_t.anim_speed_bits.low

PROVEN: Low nibble alias of the animation timing byte.

Units: clock index. DOS layout/address: +0x5, 1 B.

4-bit domain0..15; aliases anim_speed, not additional storage.

Shipped initial-data sample: 2639 records, 16 distinct values, range 0..15. This is not the full runtime domain.

## field:eta_t.anim_speed_bits.high

PROVEN: High-nibble state control selector used by gravity/helicopter processing.

Units: control code. DOS layout/address: +0x5, 1 B.

Values10/11 select special gravity-counter handling; the nibble is mutated independently of animation timing. Keep the neutral high alias until all selector cases have a shared semantic vocabulary.

Shipped initial-data sample: 2639 records, 15 distinct values, range 0..15. This is not the full runtime domain.

## field:eta_t.sound_index

STRONG: State-selected sound event index.

Units: sound id. DOS layout/address: +0x6, 1 B.

Byte; external field naming agrees with sound dispatch usage.

Shipped initial-data sample: 2639 records, 201 distinct values, range 0..255. This is not the full runtime domain.

## field:eta_t.flags

PROVEN: State behaviour bitset, including reverse playback0x10 and Rayman rectangle selection0x40.

Units: bitset. DOS layout/address: +0x7, 1 B.

Do not describe all bits as animation flags; only observed roles are established here.

Shipped initial-data sample: 2639 records, 109 distinct values, range 0..255. This is not the full runtime domain.

## field:eta_t.flags_bits

PROVEN: Overlapping nibble/bit alias container for the corresponding eta byte.

Units: bitset. DOS layout/address: +0x7, 1 B.

These containers have no independent storage or independent meaning; see individual bits and raw bytes.

## field:eta_t.flags_bits.bit0

UNKNOWN: Neutral eta_t.flags_bits.bit0 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit1

UNKNOWN: Neutral eta_t.flags_bits.bit1 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit2

UNKNOWN: Neutral eta_t.flags_bits.bit2 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit3

UNKNOWN: Neutral eta_t.flags_bits.bit3 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit4

PROVEN: Alias for reverse animation playback flag0x10.

Units: boolean. DOS layout/address: +0x7, 1 B.

When set, the terminal animation frame is0.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit5

UNKNOWN: Neutral eta_t.flags_bits.bit5 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit6

PROVEN: Alias for flag0x40 selecting the small14-pixel Rayman collision rectangle in GET_RAY_ZDC.

Units: boolean. DOS layout/address: +0x7, 1 B.

This observed effect alone does not establish a universal state name such as crouching.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:eta_t.flags_bits.bit7

UNKNOWN: Neutral eta_t.flags_bits.bit7 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x7, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 1 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 2639 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:sprite_t.offset_in_atlas

STRONG: Pixel offset of this sprite in its object atlas buffer.

Units: bytes. DOS layout/address: +0x0, 4 B.

32-bit offset; retain asset layout separately from native pointers.

Shipped initial-data sample: 5512 records, 2382 distinct values, range 0..117792. This is not the full runtime domain.

## field:sprite_t.id

PROVEN: Sprite reference id, with0 treated as an absent sprite in GET_SPRITE_POS.

Units: reference id. DOS layout/address: +0x4, 1 B.

Distinct from sprite-table index and animation layer index.

Shipped initial-data sample: 5512 records, 222 distinct values, range 0..222. This is not the full runtime domain.

## field:sprite_t.outer_width

STRONG: Outer sprite rectangle dimension outer_width.

Units: pixels. DOS layout/address: +0x5, 1 B.

Outer width participates in mirroring; returned collision dimensions use inner widths instead.

Shipped initial-data sample: 5512 records, 21 distinct values, range 0..240. This is not the full runtime domain.

## field:sprite_t.outer_height

STRONG: Outer sprite rectangle dimension outer_height.

Units: pixels. DOS layout/address: +0x6, 1 B.

Outer width participates in mirroring; returned collision dimensions use inner widths instead.

Shipped initial-data sample: 5512 records, 104 distinct values, range 0..145. This is not the full runtime domain.

## field:sprite_t.inner_width

PROVEN: Returned inner sprite rectangle dimension inner_width.

Units: pixels. DOS layout/address: +0x7, 1 B.

Byte dimensions promoted before output stores.

Shipped initial-data sample: 5512 records, 111 distinct values, range 0..235. This is not the full runtime domain.

## field:sprite_t.inner_height

PROVEN: Returned inner sprite rectangle dimension inner_height.

Units: pixels. DOS layout/address: +0x8, 1 B.

Byte dimensions promoted before output stores.

Shipped initial-data sample: 5512 records, 104 distinct values, range 0..145. This is not the full runtime domain.

## field:sprite_t.sprite_pos

PROVEN: Overlapping raw/nibble views of the sprite inset byte.

Units: bitset. DOS layout/address: +0x9, 1 B.

Aliases occupy one byte.

## field:sprite_t.sprite_pos.raw

PROVEN: Overlapping raw/nibble views of the sprite inset byte.

Units: bitset. DOS layout/address: +0x9, 1 B.

Aliases occupy one byte.

Shipped initial-data sample: 5512 records, 56 distinct values, range 0..240. This is not the full runtime domain.

## field:sprite_t.sprite_pos.bits

PROVEN: Overlapping raw/nibble views of the sprite inset byte.

Units: bitset. DOS layout/address: +0x9, 1 B.

Aliases occupy one byte.

## field:sprite_t.sprite_pos.bits.x

PROVEN: Packed nonnegative sprite inset component sprite_pos.bits.x.

Units: pixels. DOS layout/address: +0x9, 1 B.

x low nibble,y high nibble;0..15 each. These are not signed 4-bit deltas.

Shipped initial-data sample: 5512 records, 10 distinct values, range 0..9. This is not the full runtime domain.

## field:sprite_t.sprite_pos.bits.y

PROVEN: Packed nonnegative sprite inset component sprite_pos.bits.y.

Units: pixels. DOS layout/address: +0x9, 1 B.

x low nibble,y high nibble;0..15 each. These are not signed 4-bit deltas.

Shipped initial-data sample: 5512 records, 16 distinct values, range 0..15. This is not the full runtime domain.

## field:sprite_t.color

PROVEN: Raw/bitfield view of the sprite color control byte.

Units: bitset. DOS layout/address: +0xa, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

Shipped initial-data sample: 5512 records, 27 distinct values, range 0..243. This is not the full runtime domain.

## field:sprite_t.color_bits

PROVEN: Raw/bitfield view of the sprite color_bits control byte.

Units: bitset. DOS layout/address: +0xa, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

## field:sprite_t.color_bits.unused

UNKNOWN: Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0xa, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 5512 records, 3 distinct values, range 0..3. This is not the full runtime domain.

## field:sprite_t.color_bits.palette_index

STRONG: High-nibble palette selection.

Units: palette index. DOS layout/address: +0xa, 1 B.

Low nibble remains neutral; do not infer unused means always zero.

Shipped initial-data sample: 5512 records, 16 distinct values, range 0..15. This is not the full runtime domain.

## field:sprite_t.flags

PROVEN: Raw/bitfield view of the sprite flags control byte.

Units: bitset. DOS layout/address: +0xb, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

Shipped initial-data sample: 5512 records, 111 distinct values, range 0..251. This is not the full runtime domain.

## field:sprite_t.flags_bits

PROVEN: Raw/bitfield view of the sprite flags_bits control byte.

Units: bitset. DOS layout/address: +0xb, 1 B.

Preserve overlap; full meaning of unnamed bits remains unknown.

## field:sprite_t.flags_bits.unused0

UNKNOWN: Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0xb, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 5512 records, 4 distinct values, range 0..3. This is not the full runtime domain.

## field:sprite_t.flags_bits.flip_x

PROVEN: Sprite horizontal flip bit XORed with object flip to select the bounds reflection.

Units: boolean. DOS layout/address: +0xb, 1 B.

Bit2 of sprite flags byte; differs from layer.mirrored renderer input.

Shipped initial-data sample: 5512 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:sprite_t.flags_bits.unused3

UNKNOWN: Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0xb, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Shipped initial-data sample: 5512 records, 31 distinct values, range 0..31. This is not the full runtime domain.

## field:anim_t.layers

PROVEN: Pointer to frame-major animation layers.

Units: pointer. DOS layout/address: +0x0, 4 B.

Layer stride4; frame stride=(layers_per_frame&0x3fff)*4 bytes.

## field:anim_t.frames

PROVEN: Pointer to one bounding rectangle per animation frame.

Units: pointer. DOS layout/address: +0x4, 4 B.

Frame rectangle stride4 bytes.

## field:anim_t.layers_per_frame

PROVEN: Packed layer-count and horizontal-speed clock selector.

Units: count and bitset. DOS layout/address: +0x8, 2 B.

Low14 bits are layer count; high2 bits yield clock divisor/index1..4 when shifted and incremented. Do not rename this whole field to an unqualified layer_count.

Shipped initial-data sample: 1629 records, 27 distinct values, range 1..16400. This is not the full runtime domain.

## field:anim_t.frames_count

PROVEN: Number of frames in this animation.

Units: frames. DOS layout/address: +0xa, 2 B.

u16 field; snapToSprite intentionally narrows this count to u8.

Shipped initial-data sample: 1629 records, 126 distinct values, range 1..216. This is not the full runtime domain.

## field:anim_frame_t.x

PROVEN: Unsigned offset of the animation bounding rectangle (x).

Units: pixels. DOS layout/address: +0x0, 1 B.

u8; horizontal reflection includes width and object anchor.

Shipped initial-data sample: 44955 records, 213 distinct values, range 0..255. This is not the full runtime domain.

## field:anim_frame_t.y

PROVEN: Unsigned offset of the animation bounding rectangle (y).

Units: pixels. DOS layout/address: +0x1, 1 B.

u8; horizontal reflection includes width and object anchor.

Shipped initial-data sample: 44955 records, 216 distinct values, range 0..255. This is not the full runtime domain.

## field:anim_frame_t.width

PROVEN: Animation bounding rectangle dimension width.

Units: pixels. DOS layout/address: +0x2, 1 B.

u8, emitted through s16 output pointers.

Shipped initial-data sample: 44955 records, 256 distinct values, range 0..255. This is not the full runtime domain.

## field:anim_frame_t.height

PROVEN: Animation bounding rectangle dimension height.

Units: pixels. DOS layout/address: +0x3, 1 B.

u8, emitted through s16 output pointers.

Shipped initial-data sample: 44955 records, 256 distinct values, range 0..255. This is not the full runtime domain.

## field:anim_layer_t.mirrored

STRONG: Layer horizontal mirror selector used by the renderer.

Units: boolean-like byte. DOS layout/address: +0x0, 1 B.

Renderer XORs with object flip; bounds helper instead uses sprite.flags bit2.

Shipped initial-data sample: 382694 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:anim_layer_t.x

PROVEN: Unsigned object-relative layer offset component x.

Units: pixels. DOS layout/address: +0x1, 1 B.

u8 offsets are not signed.

Shipped initial-data sample: 382694 records, 256 distinct values, range 0..255. This is not the full runtime domain.

## field:anim_layer_t.y

PROVEN: Unsigned object-relative layer offset component y.

Units: pixels. DOS layout/address: +0x2, 1 B.

u8 offsets are not signed.

Shipped initial-data sample: 382694 records, 256 distinct values, range 0..255. This is not the full runtime domain.

## field:anim_layer_t.sprite_index

PROVEN: Index into the owning object sprite table for this layer.

Units: sprite index. DOS layout/address: +0x3, 1 B.

Index0 suppresses successful bounds output.

Shipped initial-data sample: 382694 records, 186 distinct values, range 0..220. This is not the full runtime domain.

## field:obj_flags_t.hurtbyfist

STRONG: Fist-hit interaction flag.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit0; preserve exact read/write sites before defining a port API.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.command_test

STRONG: Stored result of the current script test.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit1; script pilot should enumerate writers.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.alive

PROVEN: Low saved object-state bit, cleared at timed animation completion.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit2 of obj+0x81; distinct from is_active.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.flip_x

PROVEN: Horizontal object reflection/facing flag.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit3 of obj+0x81; meanings of left/right in code depend on object conventions.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.read_commands

PROVEN: Per-object command mode used to select turn labels.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit4; do not merge with per-type read-command flags.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.follow_enabled

STRONG: Enable support/follow-distance processing for the object.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit5; context is object-to-Rayman platform following.

Shipped initial-data sample: 11837 records, 2 distinct values, range 0..1. This is not the full runtime domain.

## field:obj_flags_t.flag_0x40

UNKNOWN: Neutral obj_flags_t.flag_0x40 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 21 source member-token matches are retained for review, including possible other types.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.anim_changed

STRONG: Flag used by animation update when the selected animation changes.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit7.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:obj_flags_t.flag_0x100

PROVEN: Ninth object flag written as movement-occurrence bookkeeping, with one original uninitialised producer.

Units: boolean bit. DOS layout/address: +0x1, 1 B.

Storage is obj+0x82 bit0. PROVEN observed write, not a claim that all reads mean has_moved. Do not rename.

Shipped initial-data sample: 11837 records, 1 distinct values, range 0..0. This is not the full runtime domain.

## field:cmd_context_t.cmd_offset

STRONG: Saved script command-context component cmd_offset.

Units: script offset or repeat count. DOS layout/address: +0x0, 2 B.

Two signed16 fields; context stack details remain a separate script contract.

## field:cmd_context_t.count

STRONG: Saved script command-context component count.

Units: script offset or repeat count. DOS layout/address: +0x2, 2 B.

Two signed16 fields; context stack details remain a separate script contract.

## field:level_t.objects

PROVEN: Base pointer of the packed level object array.

Units: pointer. DOS layout/address: +0x0, 4 B.

Object stride0x84.

## field:level_t.nb_objects

PROVEN: Count bounding the level object array.

Units: objects. DOS layout/address: +0x4, 2 B.

Signed16 count; struct size6 in DOS, not Android natural size8.

Shipped initial-data sample: 82 records, 66 distinct values, range 27..254. This is not the full runtime domain.

## field:level_obj_t.obj_ids

PROVEN: Always-object list member obj_ids.

Units: object ids or count. DOS layout/address: +0x0, 4 B.

Signed16 ids/count; swap-remove, no sentinel written by add.

## field:level_obj_t.nb_objects

PROVEN: Always-object list member nb_objects.

Units: object ids or count. DOS layout/address: +0x4, 2 B.

Signed16 ids/count; swap-remove, no sentinel written by add.

## field:active_objects_t.objects

PROVEN: Active-object list member objects.

Units: object ids or count. DOS layout/address: +0x0, 200 B.

100 s16 slots plus s16 count; add writes next -1 sentinel without capacity check.

## field:active_objects_t.num_active_objects

PROVEN: Active-object list member num_active_objects.

Units: object ids or count. DOS layout/address: +0xc8, 2 B.

100 s16 slots plus s16 count; add writes next -1 sentinel without capacity check.

## field:obj_type_flags_t.raw

PROVEN: Overlapping raw/byte/nibble view raw of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 4 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:obj_type_flags_t.flags0

PROVEN: Overlapping raw/byte/nibble view flags0 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:obj_type_flags_t.flags1

PROVEN: Overlapping raw/byte/nibble view flags1 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x1, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:obj_type_flags_t.flags1_lo

PROVEN: Overlapping raw/byte/nibble view flags1_lo of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x1, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:obj_type_flags_t.move_x

PROVEN: Select fractional speed conversion for the corresponding object movement axis.

Units: boolean. DOS layout/address: +0x1, 1 B.

Byte1 bits4/5 = raw0x1000/0x2000; set selects speed in sixteenths with map_time phase distribution. Unset selects integer pixels/tick.

## field:obj_type_flags_t.move_y

PROVEN: Select fractional speed conversion for the corresponding object movement axis.

Units: boolean. DOS layout/address: +0x1, 1 B.

Byte1 bits4/5 = raw0x1000/0x2000; set selects speed in sixteenths with map_time phase distribution. Unset selects integer pixels/tick.

## field:obj_type_flags_t.flags1_hi

PROVEN: Overlapping raw/byte/nibble view flags1_hi of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x1, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:obj_type_flags_t.flags2

PROVEN: Overlapping raw/byte/nibble view flags2 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x2, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:obj_type_flags_t.flags3

PROVEN: Overlapping raw/byte/nibble view flags3 of per-type behaviour flags.

Units: bitset. DOS layout/address: +0x3, 1 B.

Do not interpret all flags as movement; table has262 u32 entries.

## field:map_data_t.width

PROVEN: Map dimension width in16-pixel tiles.

Units: tiles. DOS layout/address: +0x0, 2 B.

Signed16 bounds; row-major index=y*width+x.

## field:map_data_t.height

PROVEN: Map dimension height in16-pixel tiles.

Units: tiles. DOS layout/address: +0x2, 2 B.

Signed16 bounds; row-major index=y*width+x.

## field:map_data_t.length

STRONG: Number of tile records used by full-map processing.

Units: tiles. DOS layout/address: +0x4, 4 B.

s32; do not infer unused allocation capacity.

## field:map_data_t.map

PROVEN: Pointer to row-major six-byte tile records.

Units: pointer. DOS layout/address: +0x8, 4 B.

Not a pointer to packed collision bytes.

## field:map_tile_t.texture_id

STRONG: Index of a visual16x16 tile texture.

Units: texture index. DOS layout/address: +0x0, 2 B.

u16; renderer sanitises >=1200 to0; shipped data domain measured separately.

## field:map_tile_t.tile_type

PROVEN: Collision/behaviour type byte returned by BTYP.

Units: block type id. DOS layout/address: +0x2, 1 B.

Independent of visual texture id;82 levels use26 distinct values0..31.

## field:map_tile_t.field_3

UNKNOWN: Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x3, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:map_tile_t.transparency

PROVEN: Computed texture coverage classification:0 opaque,1 empty,2 mixed.

Units: classification. DOS layout/address: +0x4, 1 B.

Not a boolean alpha value. Runtime recomputes from pixels; texture0 has a forced-empty case.

## field:map_tile_t.field_5

UNKNOWN: Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x5, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:block_flags_t.raw

PROVEN: Byte/bitfield views of block-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 1 B.

Table indexing uses collision type, not texture.

## field:block_flags_t.bits

PROVEN: Byte/bitfield views of block-type behaviour flags.

Units: bitset. DOS layout/address: +0x0, 1 B.

Table indexing uses collision type, not texture.

## field:block_flags_t.bits.unused0

UNKNOWN: Neutral block_flags_t.bits.unused0 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

## field:block_flags_t.bits.solid

PROVEN: Bit1 tested as the supporting/solid classification in block selection.

Units: boolean. DOS layout/address: +0x0, 1 B.

Bit1 is not sufficient to replace the slope and pass-through logic with a generic solid test.

## field:block_flags_t.bits.unused2

UNKNOWN: Neutral block_flags_t.bits.unused2 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:block_flags_t.bits.chdir

PROVEN: Bit5 of the probed block flags, returned by the turn-marker test.

Units: boolean. DOS layout/address: +0x0, 1 B.

Turn-marker interpretation also agrees with the original name; preserve lookup and probe offsets.

## field:block_flags_t.bits.unused1

UNKNOWN: Neutral block_flags_t.bits.unused1 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:rayevts_t.poing

STRONG: Enable the fist-throwing ability.

Units: boolean. DOS layout/address: +0x0, 1 B.

Can be removed temporarily by scripted events; not necessarily a permanent unlock.

## field:rayevts_t.hang

STRONG: Enable the ledge-hanging ability.

Units: boolean. DOS layout/address: +0x0, 1 B.

Disabled by forced-run mode at the relevant call sites.

## field:rayevts_t.helico

STRONG: Enable normal/enhanced helicopter response (helico).

Units: boolean. DOS layout/address: +0x0, 1 B.

The two bits are distinct; super_helico also affects button-release handling.

## field:rayevts_t.super_helico

STRONG: Enable normal/enhanced helicopter response (super_helico).

Units: boolean. DOS layout/address: +0x0, 1 B.

The two bits are distinct; super_helico also affects button-release handling.

## field:rayevts_t.handstand_dash

UNKNOWN: Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:rayevts_t.handstand

UNKNOWN: Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x0, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:rayevts_t.magicseed

STRONG: Enable the level-specific magic-seed ability.

Units: boolean. DOS layout/address: +0x0, 1 B.

PS1 calls the related field grain; DOS/Rayverse naming and seed call sites agree. Shipped seed-enabled level has exactly two seed objects (PORT_READINESS B19).

## field:rayevts_t.grap

STRONG: Enable grabbing/swinging through the grappling interaction.

Units: boolean. DOS layout/address: +0x0, 1 B.

PS1 same member name; exact interaction contracts remain in ray/collision units.

## field:rayevts_t.run

STRONG: Enable Rayman running behaviour.

Units: boolean. DOS layout/address: +0x1, 1 B.

Forced-run processing may set this bit; not solely a saved ability flag.

## field:rayevts_t.tiny

STRONG: Rayman small-size mode/power bit.

Units: boolean. DOS layout/address: +0x1, 1 B.

Byte1 bit1; selects4-pixel instead of8-pixel side probes for type23.

## field:rayevts_t.firefly

STRONG: Enable the firefly/light companion mode.

Units: boolean. DOS layout/address: +0x1, 1 B.

PS1 related field luciole; mode also controls clipping/level behaviour.

## field:rayevts_t.force_run

PROVEN: Two-bit forced-running lifecycle state.

Units: state code. DOS layout/address: +0x1, 1 B.

0 inactive,1 starts forced run then becomes2,3 requests release when grounded; directional response is constrained while nonzero. Preserve all four values.

## field:rayevts_t.reverse

PROVEN: Two-bit reversed-controls lifecycle state whose nonzero values swap horizontal input directions.

Units: state code. DOS layout/address: +0x1, 1 B.

Width is2 bits, not bool; do not discard script lifecycle values merely because input processing tests zero/nonzero.

## field:rayevts_t.squashed

STRONG: Reduced/squashed Rayman mode coupled to tiny and object/ability-state swapping.

Units: boolean. DOS layout/address: +0x1, 1 B.

PS1 related slot is named unused_death, which does not describe this DOS behaviour; DOS usage wins.

## field:finbosslevel_t.bzzit

STRONG: Progression flag for completion of the bzzit encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.moskito

STRONG: Progression flag for completion of the moskito encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.mr_sax

STRONG: Progression flag for completion of the mr_sax encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.mr_stone

STRONG: Progression flag for completion of the mr_stone encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.viking_mama

STRONG: Progression flag for completion of the viking_mama encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.space_mama

STRONG: Progression flag for completion of the space_mama encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.mr_skops

STRONG: Progression flag for completion of the mr_skops encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.mr_dark

STRONG: Progression flag for completion of the mr_dark encounter.

Units: boolean. DOS layout/address: +0x0, 1 B.

Boss identity is corroborated by reference names and independently located boss-completion writers; bits also control later level/world behaviour.

## field:finbosslevel_t.crazy_drummer

UNKNOWN: Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x1, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 2 source member-token matches are retained for review, including possible other types.

## field:finbosslevel_t.helped_joe_1

STRONG: Progression flag for the corresponding help event (helped_joe_1).

Units: boolean. DOS layout/address: +0x1, 1 B.

Flags are also reconstructed from selected world/level in cheat/start flows; distinguish from current scene visibility.

## field:finbosslevel_t.helped_joe_2

STRONG: Progression flag for the corresponding help event (helped_joe_2).

Units: boolean. DOS layout/address: +0x1, 1 B.

Flags are also reconstructed from selected world/level in cheat/start flows; distinguish from current scene visibility.

## field:finbosslevel_t.helped_musician

STRONG: Progression flag for the corresponding help event (helped_musician).

Units: boolean. DOS layout/address: +0x1, 1 B.

Flags are also reconstructed from selected world/level in cheat/start flows; distinguish from current scene visibility.

## field:poing_t.y_16

PROVEN: Fist vertical position accumulator scaled by16.

Units: 1/16 pixel. DOS layout/address: +0x0, 4 B.

s32; shifted right4 for displayed/world integer y; separate from ordinary obj.y.

## field:poing_t.field_4

UNKNOWN: Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x4, 2 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 15 source member-token matches are retained for review, including possible other types.

## field:poing_t.speed_x

PROVEN: Stored positive horizontal fist speed used to launch and constrain return speed.

Units: pixels/tick. DOS layout/address: +0x6, 2 B.

Launch choices5/8/11 with player-speed adjustment capped at16; direction applied separately in object speed_x.

## field:poing_t.charge

PROVEN: Fist charge counter before throwing, then scaled outbound budget decremented during flight.

Units: charge ticks then scaled budget. DOS layout/address: +0x8, 2 B.

Starts at5 (or32 airborne), caps at63 before throw; throw shifts by8 and reuses the same s16 field. No single-scale rename.

## field:poing_t.field_A

PROVEN: Temporary throw-range denominator then decrement applied to the scaled outbound charge budget.

Units: context-dependent budget units. DOS layout/address: +0xa, 2 B.

Computed from charge and speed, then overwritten by shifted_charge*speed/denominator. Meaning is computationally proven; retain neutral name because the field has successive roles.

## field:poing_t.sub_etat

STRONG: Saved/selected fist upgrade substate copied into fist objects and save data.

Units: substate selector. DOS layout/address: +0xc, 1 B.

Distinct from the current flight animation substate on poing_obj.

## field:poing_t.is_returning

PROVEN: Select the homing-return phase of fist flight.

Units: boolean-like byte. DOS layout/address: +0xd, 1 B.

Set when horizontal speed reaches zero; return path steers towards current Rayman rectangle.

## field:poing_t.is_active

PROVEN: Enable fist update/collision processing.

Units: boolean-like byte. DOS layout/address: +0xe, 1 B.

Separate byte from poing_obj is_active and flags.alive; consumers use both.

## field:poing_t.damage

PROVEN: Current fist damage applied by obj_hurt.

Units: hit points. DOS layout/address: +0xf, 1 B.

Unsigned byte.

## field:poing_t.is_charging

STRONG: Fist charge-preparation flag.

Units: boolean-like byte. DOS layout/address: +0x10, 1 B.

Set before charging, cleared when thrown.

## field:poing_t.is_boum

STRONG: Fist impact/explosion interaction state.

Units: boolean-like byte. DOS layout/address: +0x11, 1 B.

Suppresses ordinary collision handling and influences return transition; do not rename to a cosmetic-only effect flag.

## field:poing_t.field_12

UNKNOWN: Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x12, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:poing_t.field_13

UNKNOWN: Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

Units: unknown. DOS layout/address: +0x13, 1 B.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:font_t.sprites

PROVEN: Base sprite table for the corresponding font bank.

Units: pointer. DOS layout/address: +0x0, 4 B.

alpha/alpha2/alpha_numbers are separate banks.

## field:font_t.img_buffer

STRONG: Font image buffer or sprite count (img_buffer) copied from loaded object descriptors.

Units: pointer or count. DOS layout/address: +0x4, 4 B.

DOS font descriptor size10, Android natural size12.

## field:font_t.nb_sprites

STRONG: Font image buffer or sprite count (nb_sprites) copied from loaded object descriptors.

Units: pointer or count. DOS layout/address: +0x8, 2 B.

DOS font descriptor size10, Android natural size12.

## field:display_item_t.text

PROVEN: Fixed200-byte text buffer interpreted by the game glyph decoder.

Units: encoded bytes. DOS layout/address: +0x0, 200 B.

Not UTF-8; / delimiter and ]ddd numeric escape have game-specific meaning.

## field:display_item_t.centered_x_pos

PROVEN: Computed text-box rectangle component centered_x_pos.

Units: screen pixels. DOS layout/address: +0xc8, 2 B.

Includes original +2/-2 padding conventions; s16 output.

## field:display_item_t.centered_y_pos

PROVEN: Computed text-box rectangle component centered_y_pos.

Units: screen pixels. DOS layout/address: +0xca, 2 B.

Includes original +2/-2 padding conventions; s16 output.

## field:display_item_t.width

PROVEN: Computed text-box rectangle component width.

Units: screen pixels. DOS layout/address: +0xcc, 2 B.

Includes original +2/-2 padding conventions; s16 output.

## field:display_item_t.height

PROVEN: Computed text-box rectangle component height.

Units: screen pixels. DOS layout/address: +0xce, 2 B.

Includes original +2/-2 padding conventions; s16 output.

## field:display_item_t.xpos

PROVEN: Text anchor coordinate xpos.

Units: screen pixels. DOS layout/address: +0xd0, 2 B.

Baseline/centering behaviour depends on slash markup.

## field:display_item_t.ypos

PROVEN: Text anchor coordinate ypos.

Units: screen pixels. DOS layout/address: +0xd2, 2 B.

Baseline/centering behaviour depends on slash markup.

## field:display_item_t.font_size

PROVEN: Font selector controlling bank and line metrics.

Units: font selector. DOS layout/address: +0xd4, 1 B.

Supported0/1/2; line heights36/23/15 respectively, no general size parameter.

## field:display_item_t.field_D5

PROVEN: Byte selecting the palette255 box branch instead of the ordinary palette254/no-box branch.

Units: boolean-like selector. DOS layout/address: +0xd5, 1 B.

Observed effect proven, broader intended UI meaning unknown; keep neutral name. All located direct source stores are0.

## field:display_item_t.is_fond

PROVEN: Box drawing/position mode passed to DISPLAY_BLACKBOX and used to offset text by +/-1.

Units: mode. DOS layout/address: +0xd6, 1 B.

Values1/2 select negative/positive one-pixel text offsets in field_D5 branch; keep existing name rather than invent background semantics.

## field:display_item_t.color

STRONG: Text palette/color selector passed to display_text.

Units: palette selector. DOS layout/address: +0xd7, 1 B.

s8 storage passed through u8 cast; preserve bit pattern.

