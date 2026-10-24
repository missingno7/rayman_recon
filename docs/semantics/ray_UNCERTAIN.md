# Remaining UNKNOWN after Rayman/collision pass3

75 entity meanings remain UNKNOWN across the seven registries; 144 default-table state pairs have no assigned action label. HYPOTHESIS count is0.

UNKNOWN means the intended role is not established. Layout, literal stores and observed readers can still be known. Existing spellings are retained. This consolidated view supersedes earlier uncertainty lists for the refined entities.

## local:makeUturn:foll_spr:1

Owner: `basic.json`. Carried from accepted passes1-2.

Declared byte temporary with no uses in this reconstruction.

Declaration: `u8 $`. Units: unknown.

Preserve declared width, signed arithmetic and narrowing at assignment/call boundaries.

## local:GET_OBJ_ZDC:bc_x_pos:1

Owner: `collision1.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:GET_OBJ_ZDC:new_x:1

Owner: `collision1.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:GET_OBJ_ZDC:unk_3:1

Owner: `collision1.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `u8 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:GET_OBJ_ZDC:new_y:1

Owner: `collision1.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:GET_SPRITE_ZDC:temp_h:1

Owner: `collision1.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DoPoingCollisionDefault:obj:1

Owner: `collision1.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `obj_t *$`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DoPoingCollisionDefault:sprite:1

Owner: `collision1.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DoPTGRAPPINPoingCollision:sprite:1

Owner: `collision2.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DoKillingEyesPoingCollision:sprite:1

Owner: `collision2.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DoRaymanBag1Collision:obj:1

Owner: `collision2.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `obj_t *$`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DO_MOVE_MARACAS_COMMAND:diff_x_less:1

Owner: `collision2.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `u8 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:doBlKRayRaymanCollision:obj:1

Owner: `collision2.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `obj_t *$`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DO_POELLE_COMMAND:abs_ray_spd_x:1

Owner: `collision2.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DO_CORDE_COMMAND:sVar4:1

Owner: `collision2.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DO_CORDE_COMMAND:hp:1

Owner: `collision2.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `u8 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DoFallingObjPoingCollision:sprite:1

Owner: `collision2.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## global:ray_clic

Owner: `globals.json`. Added in pass3.

Signed16 slot cleared when super-helicopter activation starts.

Declaration: `s16 $`. Units: unknown.

DOS binding: `2:3f840`.

Whole-source scan and whole-image fixups find the clear, with no computational reader or nonzero writer established. Android symbol size1 differs from this DOS16-bit slot; do not transfer its type or infer a click counter.

## global:ZDCPTR

Owner: `globals.json`. Added in pass3.

Unsigned16 slot returned by get_ZDCPTR.

Declaration: `u16 $`. Units: unknown.

DOS binding: `2:3f81c`.

No live writer or interpretive consumer was established by the full-image direct-reference and source-token scans. The name alone does not establish pointer, allocator or zone-count semantics.

## global:ray_speed_inv

Owner: `globals.json`. Added in pass3.

Signed16 slot cleared when Rayman enters hit knockback.

Declaration: `s16 $`. Units: unknown.

DOS binding: `2:3f83c`.

Whole-source/image scans establish the RAY_HIT zero store but no computational reader/nonzero writer. Keep its spelling and width without inventing an inverse-speed meaning.

## global:First_Hist

Owner: `moteur.json`. Carried from accepted passes1-2.

Session initialization writes1; menu code tests for2 and clears0 in a later branch. Intended role and a live writer of2 remain unresolved.

Declaration: `u8 $`. Units: unknown state.

DOS binding: `2:3fa41`.

Keep the current name; do not infer an intro/history phase model from spelling.

## global:First_Menu

Owner: `moteur.json`. Carried from accepted passes1-2.

Session initialization writes1; no typed source reader or direct DOS fixup consumer was found. Intended role remains unresolved.

Declaration: `u8 $`. Units: unknown.

DOS binding: `2:3fa42`.

Keep current name. Indirect aggregate accesses are not excluded, and no unreachable-data investigation is proposed.

## global:dvspeed

Owner: `moteur.json`. Carried from accepted passes1-2.

Signed16 vertical camera state slot cleared at engine initialization and on vertical camera clamp.

Declaration: `s16 $`. Units: unknown.

DOS binding: `2:2522`.

No nonzero writer or computational consumer appears in the whole-source scan; the name alone does not establish a vertical analogue of dhspeed.

## global:rayOnMsWasThere

Owner: `moteur.json`. Carried from accepted passes1-2.

Marker tested by the absent-mounted-template fallback in INIT_RAY_ON_MS.

Declaration: `u8 $`. Units: unknown.

DOS binding: `2:3fa49`.

Whole-source scan finds a clear in special.c and this read, but no live nonzero writer. Preserve observed branch without inventing a mount-history meaning.

## local:rayMayLandOnAnObject:var_a1_1:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_SWIP:var_v0_2:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_SWIP:var_v0_3:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_SWIP:var_v1:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_SWIP:temp_v1_2:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `u8 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:Make_Ray_Hang:unk_4:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_RESPOND_TO_DIR:sel_anim:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `anim_t *$`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_RESPOND_TO_DIR:unk_1:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_RESPOND_TO_NOTHING:unk_1:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_RESPOND_TO_NOTHING:sel_eta_2:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `eta_t *$`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_BALANCE:unk_3:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RAY_BALANCE:unk_4:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s16 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RayTestBlocSH:var_v0:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `s32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:RayTestBlocSH:var_v0_2:1

Owner: `ray.json`. Added in pass3.

Unused local declaration; no value-consuming role is established.

Declaration: `u32 $`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## local:DO_PIEDS_RAYMAN:obj:1

Owner: `ray.json`. Added in pass3.

Unused parameter declaration; no value-consuming role is established.

Declaration: `obj_t *$`. Units: declared C storage; see exact computation and function contract.

Preserve declared width, signed operations, narrowing points and current declaration/statement order. No source expression is changed.

## field:obj_t.sub_speed

Owner: `types.json`. Carried from accepted passes1-2.

Type-dependent signed16 scratch used for a water-motion residual and another type's horizontal distance.

Declaration: `s16 $`. Units: type-dependent pixels / sixteenths.

DOS offset+0x3a, 2 byte(s), signed.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 7 source member-token matches are retained for review, including possible other types.
Pass3 refinement: DO_EAU_QUI_MONTE world3 stone branch adds14, moves water y+1 only if residual>16, then subtracts16. frame.c stores object-anchor minus Rayman-anchor distance. Do not rename the shared slot to a universal fractional speed.

## field:obj_t.link

Owner: `types.json`. Carried from accepted passes1-2.

Type-dependent signed16 scratch with proven Rayman fall, grapple-direction, door-id and initialization roles.

Declaration: `s16 $`. Units: type-dependent pixels/state/id.

DOS offset+0x52, 2 byte(s), signed.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 208 source member-token matches are retained for review, including possible other types.
The live circular successor array is link_init, not obj.link. Do not rename this shared slot to next_link; the whole-source scan has many other type-specific writers/readers.
Pass3 refinement: Rayman RAY_IN_THE_AIR sums speed_y while link!=-1, resets at apex, and compares>=200 on tile landing (>200 on object landing) for smoke. -1 suppresses accumulation. Grapple objects use+-1 swing direction; doors hold detached id; spike posts use one-time initialization0/1. A single universal name remains unsupported.

## field:obj_t.unused_padding

Owner: `types.json`. Carried from accepted passes1-2.

Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x67, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:obj_t.init_flag

Owner: `types.json`. Carried from accepted passes1-2.

Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x78, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 11 source member-token matches are retained for review, including possible other types.

## field:obj_t.timer

Owner: `types.json`. Carried from accepted passes1-2.

Type-dependent byte countdown/state scratch with multiple independently established roles.

Declaration: `u8 $`. Units: type-dependent handler calls/state.

DOS offset+0x7f, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 159 source member-token matches are retained for review, including possible other types.
INIT_RAY clears it; DO_CLING_ANIMS uses a positive countdown before DO_ANIM. This does not establish one duration unit or timer meaning for all object types.
Pass3 refinement: Rayman uses10 calls to suppress vine reattachment; grapple objects use5-call endpoint pauses. DO_RAY_POS_CMD uses255 disabled and decrements0 to255 after activation. Ship uses50 half-clock counts; other objects keep their own contracts. No universal timer unit/name is assigned.

## field:obj_t.field_83

Owner: `types.json`. Carried from accepted passes1-2.

Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x83, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:sprite_t.color_bits.unused

Owner: `types.json`. Carried from accepted passes1-2.

Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 4`. Units: unknown.

DOS offset+0xa, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:sprite_t.flags_bits.unused0

Owner: `types.json`. Carried from accepted passes1-2.

Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 2`. Units: unknown.

DOS offset+0xb, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

## field:sprite_t.flags_bits.unused3

Owner: `types.json`. Carried from accepted passes1-2.

Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 5`. Units: unknown.

DOS offset+0xb, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:map_tile_t.field_3

Owner: `types.json`. Carried from accepted passes1-2.

Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x3, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:map_tile_t.field_5

Owner: `types.json`. Carried from accepted passes1-2.

Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x5, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:block_flags_t.bits.unused0

Owner: `types.json`. Carried from accepted passes1-2.

Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved.

Declaration: `u8 $ : 1`. Units: tile-class bit.

DOS offset+0x0, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.
Do not confuse this bit with bit1 support: underSlope requires foot bit0 together with upper-sample bit6. The historical alias unused0 is retained.

## field:block_flags_t.bits.unused2

Owner: `types.json`. Carried from accepted passes1-2.

Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved.

Declaration: `u8 $ : 3`. Units: packed tile-class bits.

DOS offset+0x0, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.
A multi-bit neutral alias is not a single boolean property; keep the raw masks and do not rename/split the declaration to force a meaning.

## field:rayevts_t.handstand_dash

Owner: `types.json`. Carried from accepted passes1-2.

Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS offset+0x0, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:rayevts_t.handstand

Owner: `types.json`. Carried from accepted passes1-2.

Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS offset+0x0, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:finbosslevel_t.crazy_drummer

Owner: `types.json`. Carried from accepted passes1-2.

Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS offset+0x1, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 2 source member-token matches are retained for review, including possible other types.

## field:poing_t.field_4

Owner: `types.json`. Carried from accepted passes1-2.

Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x4, 2 byte(s), signed.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 15 source member-token matches are retained for review, including possible other types.

## field:poing_t.field_12

Owner: `types.json`. Carried from accepted passes1-2.

Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x12, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:poing_t.field_13

Owner: `types.json`. Carried from accepted passes1-2.

Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x13, 1 byte(s), unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:save_state_t.field_25b

Owner: `types.json`. Carried from accepted passes1-2.

Trailing checkpoint byte with no typed reader/writer found.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x25b, 1 byte(s), unsigned.

Record extent is604 bytes; no padding or persistence meaning is inferred.

## field:record_t.unknown_17

Owner: `types.json`. Carried from accepted passes1-2.

Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x17, 1 byte(s), unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.x_pos

Owner: `types.json`. Carried from accepted passes1-2.

Declared pix_gerbe_item_t.x_pos storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x0, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.y_pos

Owner: `types.json`. Carried from accepted passes1-2.

Declared pix_gerbe_item_t.y_pos storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x2, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.speed_x

Owner: `types.json`. Carried from accepted passes1-2.

Declared pix_gerbe_item_t.speed_x storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x4, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.speed_y

Owner: `types.json`. Carried from accepted passes1-2.

Declared pix_gerbe_item_t.speed_y storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x6, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.y_accel

Owner: `types.json`. Carried from accepted passes1-2.

Declared pix_gerbe_item_t.y_accel storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x8, 1 byte(s), unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.unk_1

Owner: `types.json`. Carried from accepted passes1-2.

Declared pix_gerbe_item_t.unk_1 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x9, 1 byte(s), unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_0

Owner: `types.json`. Carried from accepted passes1-2.

Declared flocon_t.field_0 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x0, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_2

Owner: `types.json`. Carried from accepted passes1-2.

Declared flocon_t.field_2 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x2, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_4

Owner: `types.json`. Carried from accepted passes1-2.

Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS offset+0x4, 2 byte(s), signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_6

Owner: `types.json`. Carried from accepted passes1-2.

Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x6, 1 byte(s), unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_7

Owner: `types.json`. Carried from accepted passes1-2.

Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS offset+0x7, 1 byte(s), unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:options_jeu_t.music_enabled

Owner: `types.json`. Added in pass3.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS offset+0x18, 2 byte(s), unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:options_jeu_t.sound_volume

Owner: `types.json`. Added in pass3.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS offset+0x1a, 2 byte(s), unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:options_jeu_t.is_stereo

Owner: `types.json`. Added in pass3.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS offset+0x1c, 2 byte(s), unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:options_jeu_t.unknown_1e

Owner: `types.json`. Added in pass3.

Unsigned16 option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS offset+0x1e, 2 byte(s), unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## Unnamed state pairs

These rows are observations from the loader-proven default ALLFIX eta bank0. Table presence is not a reachability proof. Full speed, animation, flags and next-state records are in ray.json and evidence/shipped_ray_states.json. Mounted template replacement can supply another table.

| main:sub | Default next state | Animation | Timing/flags |
|---|---|---|---|
| 0:1 | 0:2 | 23 | 0x04 / 0x2b |
| 0:2 | 0:0 | 23 | 0x04 / 0x2b |
| 0:3 | 0:3 | 4 | 0x04 / 0x2b |
| 0:4 | 0:4 | 9 | 0x04 / 0x28 |
| 0:5 | 0:6 | 11 | 0x04 / 0x28 |
| 0:6 | 0:7 | 12 | 0x04 / 0x28 |
| 0:7 | 0:5 | 12 | 0x04 / 0x28 |
| 0:9 | 0:10 | 11 | 0x04 / 0x28 |
| 0:10 | 0:4 | 12 | 0x04 / 0x28 |
| 0:14 | 0:0 | 0 | 0x00 / 0x00 |
| 0:16 | 0:0 | 0 | 0x00 / 0x00 |
| 0:17 | 0:0 | 48 | 0x04 / 0x2b |
| 0:18 | 0:0 | 5 | 0x04 / 0x28 |
| 0:19 | 0:19 | 19 | 0x02 / 0x00 |
| 0:20 | 0:4 | 53 | 0x01 / 0x28 |
| 0:21 | 0:21 | 59 | 0x02 / 0x20 |
| 0:22 | 0:22 | 59 | 0x02 / 0x20 |
| 0:23 | 0:23 | 59 | 0x02 / 0x20 |
| 0:24 | 0:24 | 59 | 0x02 / 0x20 |
| 0:25 | 0:25 | 59 | 0x02 / 0x20 |
| 0:26 | 0:26 | 59 | 0x02 / 0x20 |
| 0:27 | 0:27 | 59 | 0x02 / 0x20 |
| 0:28 | 0:28 | 59 | 0x02 / 0x20 |
| 0:29 | 0:29 | 59 | 0x02 / 0x20 |
| 0:30 | 0:30 | 59 | 0x02 / 0x20 |
| 0:31 | 0:0 | 0 | 0x00 / 0x00 |
| 0:32 | 0:32 | 59 | 0x02 / 0x20 |
| 0:33 | 0:0 | 0 | 0x00 / 0x00 |
| 0:34 | 0:0 | 0 | 0x00 / 0x00 |
| 0:35 | 0:0 | 0 | 0x00 / 0x00 |
| 0:36 | 0:37 | 68 | 0x02 / 0x2b |
| 0:37 | 0:37 | 69 | 0x02 / 0x2b |
| 0:38 | 0:0 | 70 | 0x02 / 0x2b |
| 0:39 | 0:0 | 71 | 0x02 / 0x2b |
| 0:44 | 0:45 | 85 | 0x01 / 0x38 |
| 0:45 | 0:45 | 86 | 0x04 / 0x28 |
| 0:46 | 0:4 | 9 | 0x01 / 0x28 |
| 0:49 | 0:49 | 99 | 0x04 / 0x2b |
| 0:52 | 0:40 | 102 | 0x01 / 0x2b |
| 0:53 | 0:0 | 104 | 0x01 / 0x2b |
| 0:54 | 0:54 | 107 | 0x01 / 0x03 |
| 0:55 | 0:0 | 109 | 0x04 / 0x2b |
| 0:56 | 0:0 | 110 | 0x04 / 0x2b |
| 0:57 | 0:57 | 114 | 0x04 / 0x00 |
| 0:58 | 0:58 | 113 | 0x04 / 0x00 |
| 0:59 | 0:59 | 115 | 0x02 / 0x2b |
| 0:60 | 0:0 | 27 | 0x01 / 0x78 |
| 0:62 | 0:62 | 131 | 0x04 / 0x2b |
| 0:63 | 0:63 | 132 | 0x02 / 0x2b |
| 0:64 | 0:64 | 134 | 0x02 / 0x2b |
| 0:65 | 0:64 | 133 | 0x03 / 0x2b |
| 1:1 | 1:1 | 7 | 0x01 / 0x28 |
| 1:2 | 0:0 | 0 | 0x00 / 0x00 |
| 1:5 | 1:0 | 61 | 0x01 / 0x2b |
| 1:6 | 1:6 | 87 | 0x01 / 0x28 |
| 2:4 | 2:4 | 15 | 0x32 / 0x2a |
| 2:6 | 2:6 | 11 | 0x22 / 0x28 |
| 2:7 | 2:2 | 19 | 0x22 / 0x28 |
| 2:10 | 0:0 | 0 | 0x00 / 0x00 |
| 2:11 | 2:12 | 0 | 0x21 / 0x2c |
| 2:12 | 2:12 | 1 | 0x21 / 0x2c |
| 2:13 | 2:2 | 49 | 0x21 / 0x2c |
| 2:16 | 2:2 | 56 | 0x32 / 0x2a |
| 2:20 | 0:0 | 0 | 0x00 / 0x00 |
| 2:21 | 0:0 | 0 | 0x00 / 0x00 |
| 2:22 | 2:22 | 88 | 0x21 / 0x28 |
| 2:23 | 2:23 | 86 | 0x22 / 0x28 |
| 2:25 | 2:26 | 101 | 0x11 / 0x2a |
| 2:26 | 2:26 | 79 | 0x11 / 0x2a |
| 2:27 | 2:28 | 103 | 0x11 / 0x2a |
| 2:28 | 2:28 | 79 | 0x11 / 0x2a |
| 2:29 | 2:26 | 49 | 0x21 / 0x28 |
| 2:30 | 2:28 | 49 | 0x21 / 0x28 |
| 2:33 | 2:24 | 29 | 0x62 / 0x6b |
| 3:1 | 0:4 | 6 | 0x01 / 0x28 |
| 3:2 | 0:0 | 8 | 0x01 / 0x28 |
| 3:3 | 0:5 | 6 | 0x01 / 0x28 |
| 3:4 | 0:1 | 8 | 0x01 / 0x28 |
| 3:5 | 2:7 | 10 | 0x03 / 0x28 |
| 3:7 | 3:7 | 130 | 0x03 / 0x00 |
| 3:8 | 0:0 | 0 | 0x00 / 0x00 |
| 3:9 | 3:10 | 36 | 0x04 / 0x00 |
| 3:10 | 3:10 | 37 | 0x03 / 0x00 |
| 3:11 | 3:12 | 38 | 0x04 / 0x00 |
| 3:12 | 3:12 | 98 | 0x04 / 0x03 |
| 3:13 | 0:0 | 0 | 0x00 / 0x00 |
| 3:14 | 0:0 | 0 | 0x00 / 0x00 |
| 3:15 | 2:2 | 46 | 0x01 / 0x2b |
| 3:17 | 3:18 | 51 | 0x01 / 0x68 |
| 3:18 | 0:20 | 52 | 0x01 / 0x68 |
| 3:19 | 0:15 | 51 | 0x01 / 0x68 |
| 3:23 | 3:38 | 98 | 0x04 / 0x00 |
| 3:24 | 3:25 | 90 | 0x02 / 0x00 |
| 3:25 | 3:25 | 91 | 0x01 / 0x00 |
| 3:26 | 3:27 | 92 | 0x02 / 0x00 |
| 3:27 | 3:27 | 93 | 0x02 / 0x00 |
| 3:28 | 3:23 | 97 | 0x02 / 0x00 |
| 3:29 | 3:30 | 95 | 0x02 / 0x00 |
| 3:30 | 3:30 | 96 | 0x01 / 0x00 |
| 3:31 | 3:29 | 94 | 0x02 / 0x00 |
| 3:33 | 3:33 | 106 | 0x00 / 0x00 |
| 3:34 | 3:34 | 22 | 0x04 / 0x00 |
| 3:35 | 3:36 | 65 | 0x02 / 0x00 |
| 3:36 | 3:36 | 66 | 0x02 / 0x00 |
| 3:37 | 3:20 | 67 | 0x02 / 0x00 |
| 3:38 | 0:0 | 111 | 0x04 / 0x00 |
| 3:39 | 3:39 | 116 | 0x03 / 0x20 |
| 3:40 | 0:0 | 118 | 0x02 / 0x00 |
| 3:41 | 0:0 | 119 | 0x03 / 0x20 |
| 3:42 | 0:0 | 120 | 0x03 / 0x20 |
| 3:43 | 3:44 | 121 | 0x03 / 0x20 |
| 3:44 | 0:0 | 122 | 0x03 / 0x20 |
| 3:45 | 3:46 | 123 | 0x03 / 0x20 |
| 3:46 | 0:0 | 124 | 0x03 / 0x20 |
| 3:47 | 3:48 | 125 | 0x03 / 0x20 |
| 3:48 | 3:49 | 126 | 0x03 / 0x20 |
| 3:49 | 0:0 | 127 | 0x03 / 0x20 |
| 3:50 | 0:4 | 50 | 0x01 / 0x28 |
| 3:51 | 0:0 | 128 | 0x03 / 0x00 |
| 3:52 | 0:0 | 129 | 0x03 / 0x00 |
| 4:4 | 0:0 | 0 | 0x00 / 0x00 |
| 4:5 | 0:0 | 0 | 0x00 / 0x00 |
| 4:6 | 0:0 | 0 | 0x00 / 0x00 |
| 4:7 | 0:0 | 0 | 0x00 / 0x00 |
| 4:8 | 0:0 | 0 | 0x00 / 0x00 |
| 4:9 | 0:0 | 0 | 0x00 / 0x00 |
| 4:10 | 0:0 | 0 | 0x00 / 0x00 |
| 4:11 | 4:12 | 33 | 0x01 / 0x28 |
| 4:12 | 4:12 | 34 | 0x01 / 0x28 |
| 4:13 | 4:1 | 35 | 0x01 / 0x28 |
| 5:1 | 5:1 | 39 | 0x05 / 0x2b |
| 5:2 | 0:0 | 0 | 0x00 / 0x00 |
| 5:3 | 0:0 | 0 | 0x00 / 0x00 |
| 5:4 | 0:0 | 0 | 0x00 / 0x00 |
| 5:5 | 0:0 | 0 | 0x00 / 0x00 |
| 5:6 | 0:0 | 0 | 0x00 / 0x00 |
| 5:7 | 0:0 | 0 | 0x00 / 0x00 |
| 5:8 | 0:0 | 0 | 0x00 / 0x00 |
| 5:9 | 0:0 | 0 | 0x00 / 0x00 |
| 5:10 | 0:0 | 0 | 0x00 / 0x00 |
| 5:11 | 5:12 | 62 | 0x01 / 0x28 |
| 5:12 | 5:12 | 63 | 0x01 / 0x28 |
| 5:13 | 5:1 | 64 | 0x01 / 0x29 |
| 6:0 | 0:0 | 0 | 0x00 / 0x00 |
