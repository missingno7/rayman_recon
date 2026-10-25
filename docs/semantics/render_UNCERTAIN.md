# Rendering — remaining UNKNOWN / HYPOTHESIS

This is the current pass-5 view. Earlier uncertainty files are historical pass snapshots; the refined entries listed in RENDER_REPORT.md are now owned at their existing IDs in types.json/globals.json. Layout certainty and observed operations do not prove a universal intended label. No entry below is renamed.

60 uncertain entities occur in these three units, their directly used globals, or the complete referenced shared types. This includes unused members of those complete types; it does not mean there are that many unexplained rendering instructions. The entire 12-registry collection has 140 UNKNOWN and 0 HYPOTHESIS records.

## field:credit_t.cmd

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Credit control byte consumed by credits logic outside the display loop.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x9; 1 byte(s); unsigned.

DISPLAY_TXT_CREDITS does not read it; recorded layout9. No command enum/intended meaning is assigned from the name alone.

## field:credit_t.unknown_b

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Credit record trailing byte not read by the display loop.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0xb; 1 byte(s); unsigned.

Observed12-byte record includes this byte at+11; no purpose inferred from padding/capacity.

## field:finbosslevel_t.crazy_drummer

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS member offset +0x1; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 2 source member-token matches are retained for review, including possible other types.

## field:flocon_t.field_4

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS member offset +0x4; 2 byte(s); signed.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_6

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x6; 1 byte(s); unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_7

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x7; 1 byte(s); unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:map_tile_t.field_3

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Raw serialized map byte+3 with no intended meaning assigned.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x3; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Pass4: All6-byte map records include this byte. Loader reads it as part of the record, without decoding or assigning a semantic property.

## field:map_tile_t.field_5

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Raw serialized map byte+5 with no intended meaning assigned.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x5; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

Pass4: Included in raw6-byte LEV map records; no additional meaning inferred from neighboring transparency/type fields.

## field:obj_t.field_83

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x83; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:obj_t.init_flag

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Serialized byte with initializer writes of7; no intended common meaning or typed reader is established.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x78; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 11 source member-token matches are retained for review, including possible other types.

Pass4: 11, 837 serialized instances span values0, 1, 2, 3, 4, 5, 7, 9; many INIT_OBJECTS branches and cage collision write7. Whole-binary displacement candidates require typed-base proof. A draw priority/respawn-delay interpretation is not justified.

## field:obj_t.link

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Multipurpose signed 16-bit scratch; initialization includes paired-object links and unrelated timers/parameters.

Declaration: `s16 $`. Units: type dependent.

DOS member offset +0x52; 2 byte(s); signed.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 208 source member-token matches are retained for review, including possible other types.

The live circular successor array is link_init, not obj.link. Do not rename this shared slot to next_link; the whole-source scan has many other type-specific writers/readers.

Pass3 refinement: Rayman RAY_IN_THE_AIR sums speed_y while link!=-1, resets at apex, and compares>=200 on tile landing (>200 on object landing) for smoke. -1 suppresses accumulation. Grapple objects use+-1 swing direction; doors hold detached id; spike posts use one-time initialization0/1. A single universal name remains unsupported.

Pass4: first_obj_init usually clears0; type26 sets20; type173 clears0 whileparam=-1; type231/232 get each other's ids after the scan. This is not the separate link_init successor array.

## field:obj_t.sub_speed

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Type-dependent signed 16-bit scratch used for a water-motion residual and another type's horizontal distance.

Declaration: `s16 $`. Units: type-dependent pixels / sixteenths.

DOS member offset +0x3a; 2 byte(s); signed.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 7 source member-token matches are retained for review, including possible other types.

Pass3 refinement: DO_EAU_QUI_MONTE world3 stone branch adds14, moves water y+1 only if residual>16, then subtracts16. frame.c stores object-anchor minus Rayman-anchor distance. Do not rename the shared slot to a universal fractional speed.

## field:obj_t.timer

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Multipurpose byte countdown/state scratch with common reset0 and actor-specific defaults.

Declaration: `u8 $`. Units: type dependent.

DOS member offset +0x7f; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 159 source member-token matches are retained for review, including possible other types.

INIT_RAY clears it; DO_CLING_ANIMS uses a positive countdown before DO_ANIM. This does not establish one duration unit or timer meaning for all object types.

Pass3 refinement: Rayman uses10 calls to suppress vine reattachment; grapple objects use5-call endpoint pauses. DO_RAY_POS_CMD uses255 disabled and decrements0 to255 after activation. Ship uses50 half-clock counts; other objects keep their own contracts. No universal timer unit/name is assigned.

Pass4: obj_init resets0, bosses get100 or180; INIT_OBJECTS type99 state5 gets30. Earlier Rayman/collision roles remain valid; no universal timing or meaning rename.

## field:obj_t.unused_padding

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x67; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:options_jeu_t.is_stereo

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned 16-bit option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS member offset +0x1c; 2 byte(s); unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:options_jeu_t.music_enabled

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned 16-bit option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS member offset +0x18; 2 byte(s); unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:options_jeu_t.sound_volume

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned 16-bit option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS member offset +0x1a; 2 byte(s); unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:options_jeu_t.unknown_1e

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Unsigned 16-bit option slot outside the Rayman/collision field-access scope.

Declaration: `u16 $`. Units: unknown.

DOS member offset +0x1e; 2 byte(s); unsigned.

Layout and source declaration measured; no new meaning or rename assigned by this pass. Do not infer meaning from spelling alone; input/menu semantics are deferred.

## field:poing_t.field_12

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x12; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:poing_t.field_13

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x13; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:poing_t.field_4

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

Declaration: `s16 $`. Units: unknown.

DOS member offset +0x4; 2 byte(s); signed.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 15 source member-token matches are retained for review, including possible other types.

## field:rayevts_t.handstand

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS member offset +0x0; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:rayevts_t.handstand_dash

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS member offset +0x0; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:record_t.unknown_17

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x17; 1 byte(s); unsigned.

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:sprite_t.color_bits.unused

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 4`. Units: unknown.

DOS member offset +0xa; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:sprite_t.flags_bits.unused0

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 2`. Units: unknown.

DOS member offset +0xb; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

## field:sprite_t.flags_bits.unused3

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 5`. Units: unknown.

DOS member offset +0xb; 1 byte(s); unsigned.

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:world_info_t.color

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.color storage; no intended meaning is assigned from its spelling alone.

Declaration: `s8 $`. Units: unknown.

DOS member offset +0xc; 1 byte(s); signed.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.field_D

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.field_D storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0xd; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.field_E

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.field_E storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0xe; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.field_F

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.field_F storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0xf; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.index_down

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_down storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x5; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.index_left

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_left storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x6; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.index_right

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_right storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x7; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.index_up

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.index_up storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x4; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.is_unlocked

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.is_unlocked storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 1`. Units: unknown.

DOS member offset +0x8; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.is_unlocking

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.is_unlocking storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 1`. Units: unknown.

DOS member offset +0x8; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.level

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.level storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0xb; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.state

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.state storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0x8; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.state_bit1

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.state_bit1 storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 1`. Units: unknown.

DOS member offset +0x8; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.state_rest

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.state_rest storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $ : 5`. Units: unknown.

DOS member offset +0x8; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.text

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.text storage; no intended meaning is assigned from its spelling alone.

Declaration: `char *$`. Units: unknown.

DOS member offset +0x10; 4 byte(s); not applicable (pointer).

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.world

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.world storage; no intended meaning is assigned from its spelling alone.

Declaration: `u8 $`. Units: unknown.

DOS member offset +0xa; 1 byte(s); unsigned.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.xpos

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.xpos storage; no intended meaning is assigned from its spelling alone.

Declaration: `s16 $`. Units: unknown.

DOS member offset +0x0; 2 byte(s); signed.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## field:world_info_t.ypos

Owner: `docs/semantics/types.json`. Level: **UNKNOWN**.

Declared world_info_t.ypos storage; no intended meaning is assigned from its spelling alone.

Declaration: `s16 $`. Units: unknown.

DOS member offset +0x2; 2 byte(s); signed.

Layout is measured. This member has no value-consuming typed access in the scoped units; whole-header copies/opaque callees are not an intended-meaning proof.

## global:let_shadow

Owner: `docs/semantics/globals.json`. Level: **UNKNOWN**.

Byte written by save/world-map rendering without a located value-consuming reader.

Declaration: `u8 $`. Units: unknown.

DOS binding: `2:51d0`.

Known stores0/1 and original name retained; the name does not prove a shadow-rendering effect in DOS. Whole-image exact/widened-reference search found only writers in audited functions.

## global:unk_ptr_3d840

Owner: `docs/semantics/globals.json`. Level: **UNKNOWN**.

Pointer used only as the dormant BIOS benchmark completion flag.

Declaration: `u8 *$`. Units: u8 pointer.

DOS binding: `2:3d840`.

Zero BSS; no initializer or live caller was found. Only sub_3B5E8 dereferences it. Native port may omit this diagnostic per docs/port/PLATFORM_OPEN.md; historical pointer ownership remains UNKNOWN.

## local:display_credits_prg:a1:1

Owner: `docs/semantics/display.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Read input/CD/DO_CREDITS, copy background/draw text. Save previous PROC_EXIT then clear; valid/select sets PROC_EXIT1 and display_Vignet7. Return either old or new exit. Incoming clock unused.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_deform_text:color:1

Owner: `docs/semantics/affiche.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s8 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Same font metrics/escapes as display_text; calc_num_let_spr adjusts the low glyph byte before sprite lookup. rotation is a512-unit turn; enx/eny are Q12 scales passed to DISPLAY_DEFORM_SPRITE. Color formal is not consumed by this function; transformed sprites use their own palette nibble.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_inter_anim:a1:1

Owner: `docs/semantics/display.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. horloges1, clear, read input, draw BEFORE DO_ANIM, test exit/buttons, then anim_func; return PROC_EXIT. Incoming scene clock is unused.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_time:col:1

Owner: `docs/semantics/affiche.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. time==-2 skips the entire function. Digits use time/60, time/600, time/6000 modulo10, so60 nominal ticks per displayed second. Prompt<=map_time100 disables Rayman and oscillates the byte color1..5 while horloge8<4; 120<map_time<=160 shows go. Negative values other than-2 are not a new valid digit domain.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_time:col_add:1

Owner: `docs/semantics/affiche.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. time==-2 skips the entire function. Digits use time/60, time/600, time/6000 modulo10, so60 nominal ticks per displayed second. Prompt<=map_time100 disables Rayman and oscillates the byte color1..5 while horloge8<4; 120<map_time<=160 shows go. Negative values other than-2 are not a new valid digit domain.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_time:nb_wiz_text:1

Owner: `docs/semantics/affiche.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `u8 $[8]`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. time==-2 skips the entire function. Digits use time/60, time/600, time/6000 modulo10, so60 nominal ticks per displayed second. Prompt<=map_time100 disables Rayman and oscillates the byte color1..5 while horloge8<4; 120<map_time<=160 shows go. Negative values other than-2 are not a new valid digit domain.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_time:time_div:1

Owner: `docs/semantics/affiche.json`. Level: **UNKNOWN**.

Unused local; no value-consuming role is established.

Declaration: `s32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. time==-2 skips the entire function. Digits use time/60, time/600, time/6000 modulo10, so60 nominal ticks per displayed second. Prompt<=map_time100 disables Rayman and oscillates the byte color1..5 while horloge8<4; 120<map_time<=160 shows go. Negative values other than-2 are not a new valid digit domain.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:display_vignet_prg:a1:1

Owner: `docs/semantics/display.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Read/draw; increment loop_nb_trames, at60 reset0 and decrement loop_timing. Accept input only when timing<=3; auto end at<=0. Next pair resets timing5; no later pair sets new_txt_fee10 and returnsdone. This is60 callback invocations per nominal second, not measured elapsed time.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:loader_anim_prg:a1:1

Owner: `docs/semantics/display.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Input can set PROC_EXIT; horloges1, CD, background, bigray draw, DO_LOADER_ANIM. Return PROC_EXIT&&nb_fade0. Incoming scene clock unused.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:select_level_prg:a1:1

Owner: `docs/semantics/display.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Draw current decimal level; horizontal input changes selection only inter_select0. Clamp1..{17, 16, 11, 11, 11, 4}[world-1]; repeat clock cycles0..3. Return any action button. Incoming clock unused; the private array initializer is part of object identity.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:selection_save_option_prg:a1:1

Owner: `docs/semantics/display.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `u32 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Separate button-release snapshots for command and selection; optional plasma rebuild. Actions1 or3 may run name entry then SaveGameOnDisk unless MENU_RETURN. Increment TempsDemo, clear action, and start demo at RunTimeDemo if no exit/game end. Return end/exit/demo/menu-return. Incoming clock unused.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:sub_3BE20:x:1

Owner: `docs/semantics/frame.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Formal x/y are unused. Context words are plane length, end pointer, length-1; fourth word unused/unknown. type_fndscrX forces mask1. P486 nonzero calls asm_037e4, else4 plane calls asm_03257; doubling mask wraps16->1 and dest phase1. Record semantics are in docs/port/ASM_SPEC.md.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## local:sub_3BE20:y:1

Owner: `docs/semantics/frame.json`. Level: **UNKNOWN**.

Unused parameter; no value-consuming role is established.

Declaration: `s16 $`. Units: unknown.

Preserve declared width, signed operations, cast/narrowing points and statement/declaration order. Formal x/y are unused. Context words are plane length, end pointer, length-1; fourth word unused/unknown. type_fndscrX forces mask1. P486 nonzero calls asm_037e4, else4 plane calls asm_03257; doubling mask wraps16->1 and dest phase1. Record semantics are in docs/port/ASM_SPEC.md.

The original declaration and exact uses are retained in `value_domain.observed_code_uses`; an unused argument is not assigned an invented purpose.

## Additional boundaries without an UNKNOWN entity rename

- `display_item_t.field_D5`: its fill/text-offset effect is PROVEN, but a broader shadow/opacity-style intended label is not. The neutral spelling stays.
- `obj_t.timer`, `link`, and `sub_speed` have actor-specific meanings. The lookout fuse/latch/distance uses are recorded; they do not establish universal actor-field names.
- `hasGuetteurABomb` leaves a return value undefined when the eta flag is absent; spawning has unsupported-substate paths with undefined originals. This pass changes no behavior and assigns no guessed fallback. The full caller-domain obligation is retained.
- The star callback accesses its slot before the `< 30` increment guard. This pass does not prove a maximum path-point count or alter capacity.
- The separate background-render local context has a fourth unaccessed word. Its purpose remains unknown; no padding/helper-state label is inferred.
- Frame-context words +0/+4/+8 are understood per phase. A single universal meaning for each storage word is not established.
- P486 branch effects are proven; its historical CPU-selection intent is not newly proved. Frame counts and nominal 60-tick displays do not establish one universal measured refresh rate.
- Existing original array-capacity questions (credits, PLANVIG metadata, language buffers and other unconsumed storage) were not investigated. Current declarations and ownership are unchanged.
- Historical BIOS/PMODE/W pointer translation and the orphan benchmark remain the existing platform omit/open decision, not a live rendering blocker.
- Direct C entity coverage delegates assembly clip/timer/private state to ASM_SPEC and projection helper state to the earlier basic registry. It is not an attempted recursive re-documentation of every scene callback.

Negative and rejected inferences are retained in `evidence/negative_results.json`; full-image displacement/token matches remain candidates unless typed dataflow establishes their base.
