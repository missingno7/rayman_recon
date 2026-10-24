# Remaining UNKNOWN after the moteur pass

These are current observations with unresolved intended meanings. Names remain unchanged. This view includes the carried shared-type unknowns from the pilot; it does not claim they are all newly investigated in moteur. HYPOTHESIS count is zero.

Four moteur globals remain UNKNOWN. Eleven local particle/snow declaration members have layout facts only; the snow typedef has no field access in moteur and was not investigated as unreachable gameplay data. Other entries belong to shared types or the pilot unused local.

## local:makeUturn:foll_spr:1

Owner: `basic.json`. Level: UNKNOWN.

Declared byte temporary with no uses in this reconstruction.

Declaration: `u8 $`. Units: unknown.

Preserve declared width, signed arithmetic and narrowing at assignment/call boundaries.

## global:First_Hist

Owner: `moteur.json`. Level: UNKNOWN.

Session initialization writes1; menu code tests for2 and clears0 in a later branch. Intended role and a live writer of2 remain unresolved.

Declaration: `u8 $`. Units: unknown state.

DOS binding: `2:3fa41`. Full-image direct references, minus2/minus3 widened-load candidates and source uses are in `evidence/symbols.json`.

Keep the current name; do not infer an intro/history phase model from spelling.

## global:First_Menu

Owner: `moteur.json`. Level: UNKNOWN.

Session initialization writes1; no typed source reader or direct DOS fixup consumer was found. Intended role remains unresolved.

Declaration: `u8 $`. Units: unknown.

DOS binding: `2:3fa42`. Full-image direct references, minus2/minus3 widened-load candidates and source uses are in `evidence/symbols.json`.

Keep current name. Indirect aggregate accesses are not excluded, and no unreachable-data investigation is proposed.

## global:dvspeed

Owner: `moteur.json`. Level: UNKNOWN.

Signed16 vertical camera state slot cleared at engine initialization and on vertical camera clamp.

Declaration: `s16 $`. Units: unknown.

DOS binding: `2:2522`. Full-image direct references, minus2/minus3 widened-load candidates and source uses are in `evidence/symbols.json`.

No nonzero writer or computational consumer appears in the whole-source scan; the name alone does not establish a vertical analogue of dhspeed.

## global:rayOnMsWasThere

Owner: `moteur.json`. Level: UNKNOWN.

Marker tested by the absent-mounted-template fallback in INIT_RAY_ON_MS.

Declaration: `u8 $`. Units: unknown.

DOS binding: `2:3fa49`. Full-image direct references, minus2/minus3 widened-load candidates and source uses are in `evidence/symbols.json`.

Whole-source scan finds a clear in special.c and this read, but no live nonzero writer. Preserve observed branch without inventing a mount-history meaning.

## field:obj_t.sub_speed

Owner: `types.json`. Level: UNKNOWN.

Neutral obj_t.sub_speed storage/view; its intended meaning is not established by this pilot.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x3a, 2 byte(s), signed.

Shipped initial sample: 11837 records, 1 values, range 0..0; not a runtime bound.

7 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/collision2.c:790`: `obj->sub_speed += 14;`
- `src/collision2.c:791`: `if (obj->sub_speed > 16) {`
- `src/collision2.c:792`: `obj->sub_speed -= 16;`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 7 source member-token matches are retained for review, including possible other types.

## field:obj_t.link

Owner: `types.json`. Level: UNKNOWN.

Type-dependent signed16 storage: generating doors save their detached successor id here; live Rayman initialization uses-1, and template swaps copy the field. A universal meaning across object types remains unresolved.

Declaration: `s16 $`. Units: type-dependent state / object id.

DOS/compiled declaration layout: +0x52, 2 byte(s), signed.

Shipped initial sample: 11837 records, 1 values, range 0..0; not a runtime bound.

208 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/araignee.c:661`: `obj->link = 0;`
- `src/araignee.c:695`: `obj->link = 0;`
- `src/araignee.c:715`: `if ((obj->main_etat == 0) && (obj->sub_etat == 3) && (obj->link == 0))`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 208 source member-token matches are retained for review, including possible other types.

The live circular successor array is link_init, not obj.link. Do not rename this shared slot to next_link; the whole-source scan has many other type-specific writers/readers.

## field:obj_t.zdc

Owner: `types.json`. Level: UNKNOWN.

Neutral obj_t.zdc storage/view; its intended meaning is not established by this pilot.

Declaration: `u16 $`. Units: unknown.

DOS/compiled declaration layout: +0x5c, 2 byte(s), unsigned.

Shipped initial sample: 11837 records, 1 values, range 0..0; not a runtime bound.

6 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/collision1.c:101`: `return obj->zdc >> 11;`
- `src/collision1.c:106`: `return obj->zdc & 0x7FF;`
- `src/collision1.c:680`: `if (obj->zdc != 0)`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 6 source member-token matches are retained for review, including possible other types.

## field:obj_t.unused_padding

Owner: `types.json`. Level: UNKNOWN.

Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x67, 1 byte(s), unsigned.

Shipped initial sample: 11837 records, 1 values, range 0..0; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:obj_t.init_flag

Owner: `types.json`. Level: UNKNOWN.

Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x78, 1 byte(s), unsigned.

Shipped initial sample: 11837 records, 8 values, range 0..9; not a runtime bound.

11 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/cage.c:87`: `obj->init_flag = 7;`
- `src/objinit.c:1088`: `obj->init_flag = 7;`
- `src/objinit.c:1093`: `obj->init_flag = 7;`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 11 source member-token matches are retained for review, including possible other types.

## field:obj_t.hit_sprite

Owner: `types.json`. Level: UNKNOWN.

Neutral obj_t.hit_sprite storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x79, 1 byte(s), unsigned.

Shipped initial sample: 11837 records, 6 values, range 0..255; not a runtime bound.

10 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/collision1.c:739`: `if (obj->hit_sprite == 0xFE)`
- `src/collision1.c:750`: `else if (obj->hit_sprite == 0xFF)`
- `src/collision1.c:778`: `succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 10 source member-token matches are retained for review, including possible other types.

## field:obj_t.timer

Owner: `types.json`. Level: UNKNOWN.

Type-dependent byte storage; the cling pickup animation path decrements it as a delay while holding frame zero. Its other object-specific roles remain unresolved.

Declaration: `u8 $`. Units: type-dependent updates/state.

DOS/compiled declaration layout: +0x7f, 1 byte(s), unsigned.

Shipped initial sample: 11837 records, 1 values, range 0..0; not a runtime bound.

159 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/affiche.c:521`: `if (c_1up->timer == 0) {`
- `src/affiche.c:532`: `if (c_pow->timer == 0) {`
- `src/affiche.c:1210`: `star->timer++;`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 159 source member-token matches are retained for review, including possible other types.

INIT_RAY clears it; DO_CLING_ANIMS uses a positive countdown before DO_ANIM. This does not establish one duration unit or timer meaning for all object types.

## field:obj_t.field_83

Owner: `types.json`. Level: UNKNOWN.

Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x83, 1 byte(s), unsigned.

Shipped initial sample: 11837 records, 1 values, range 0..0; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:eta_t.flags_bits.bit0

Owner: `types.json`. Level: UNKNOWN.

Neutral eta_t.flags_bits.bit0 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

Bit width 1, mask0x01.

Shipped initial sample: 2639 records, 2 values, range 0..1; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:eta_t.flags_bits.bit1

Owner: `types.json`. Level: UNKNOWN.

Neutral eta_t.flags_bits.bit1 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

Bit width 1, mask0x02.

Shipped initial sample: 2639 records, 2 values, range 0..1; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:eta_t.flags_bits.bit2

Owner: `types.json`. Level: UNKNOWN.

Neutral eta_t.flags_bits.bit2 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

Bit width 1, mask0x04.

Shipped initial sample: 2639 records, 2 values, range 0..1; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:eta_t.flags_bits.bit3

Owner: `types.json`. Level: UNKNOWN.

Observed state flag0x08: DO_OBJECTS increments a nonboss object hit_points from0 to1 after its update when this bit is set. A broader intended state name is unresolved.

Declaration: `u8 $ : 1`. Units: state flag.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

Bit width 1, mask0x08.

Shipped initial sample: 2639 records, 2 values, range 0..1; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

This test is after DO_ONE_OBJECT and excludes boss-flagged types. Retain bit3 rather than inventing a universal health or invincibility name.

## field:eta_t.flags_bits.bit5

Owner: `types.json`. Level: UNKNOWN.

Neutral eta_t.flags_bits.bit5 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

Bit width 1, mask0x20.

Shipped initial sample: 2639 records, 2 values, range 0..1; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:eta_t.flags_bits.bit7

Owner: `types.json`. Level: UNKNOWN.

Neutral eta_t.flags_bits.bit7 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

Bit width 1, mask0x80.

Shipped initial sample: 2639 records, 2 values, range 0..1; not a runtime bound.

1 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/collision1.c:475`: `inset = 24 - 12 * (get_eta(obj)->flags_bits.bit7 + 2 * get_eta(obj)->flags_bits.bit6);`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 1 source member-token matches are retained for review, including possible other types.

## field:sprite_t.color_bits.unused

Owner: `types.json`. Level: UNKNOWN.

Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 4`. Units: unknown.

DOS/compiled declaration layout: +0xa, 1 byte(s), unsigned.

Bit width 4, mask0x0f.

Shipped initial sample: 5512 records, 3 values, range 0..3; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:sprite_t.flags_bits.unused0

Owner: `types.json`. Level: UNKNOWN.

Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 2`. Units: unknown.

DOS/compiled declaration layout: +0xb, 1 byte(s), unsigned.

Bit width 2, mask0x03.

Shipped initial sample: 5512 records, 4 values, range 0..3; not a runtime bound.

5 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/mite.c:264`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0) {`
- `src/mite.c:478`: `if (block_flags[obj->btypes[3]].bits.unused0)`
- `src/mite.c:481`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0)`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

## field:sprite_t.flags_bits.unused3

Owner: `types.json`. Level: UNKNOWN.

Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 5`. Units: unknown.

DOS/compiled declaration layout: +0xb, 1 byte(s), unsigned.

Bit width 5, mask0xf8.

Shipped initial sample: 5512 records, 31 values, range 0..31; not a runtime bound.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:map_tile_t.field_3

Owner: `types.json`. Level: UNKNOWN.

Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x3, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:map_tile_t.field_5

Owner: `types.json`. Level: UNKNOWN.

Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x5, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:block_flags_t.bits.unused0

Owner: `types.json`. Level: UNKNOWN.

Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved.

Declaration: `u8 $ : 1`. Units: tile-class bit.

DOS/compiled declaration layout: +0x0, 1 byte(s), unsigned.

Bit width 1, mask0x01.

5 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/mite.c:264`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0) {`
- `src/mite.c:478`: `if (block_flags[obj->btypes[3]].bits.unused0)`
- `src/mite.c:481`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0)`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 5 source member-token matches are retained for review, including possible other types.

Do not confuse this bit with bit1 support: underSlope requires foot bit0 together with upper-sample bit6. The historical alias unused0 is retained.

## field:block_flags_t.bits.unused2

Owner: `types.json`. Level: UNKNOWN.

Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved.

Declaration: `u8 $ : 3`. Units: packed tile-class bits.

DOS/compiled declaration layout: +0x0, 1 byte(s), unsigned.

Bit width 3, mask0x1c.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

A multi-bit neutral alias is not a single boolean property; keep the raw masks and do not rename/split the declaration to force a meaning.

## field:block_flags_t.bits.unused1

Owner: `types.json`. Level: UNKNOWN.

Two-bit view of raw bits6..7; moteur consumes bit6 in slope predicates while the role of bit7 remains unresolved.

Declaration: `u8 $ : 2`. Units: packed tile-class bits.

DOS/compiled declaration layout: +0x0, 1 byte(s), unsigned.

Bit width 2, mask0xc0.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

underSlope samples bit6 from btypes[3]; is_icy_pente tests bit6 and bit3 on one tile. Preserve these different sample relationships.

## field:rayevts_t.handstand_dash

Owner: `types.json`. Level: UNKNOWN.

Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x0, 1 byte(s), unsigned.

Bit width 1, mask0x10.

3 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/display.c:984`: `RayEvts.handstand_dash = 0;`
- `src/moteur.c:3156`: `RayEvts.handstand_dash = 0;`
- `src/moteur.c:4045`: `RayEvts.handstand_dash = 0;`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:rayevts_t.handstand

Owner: `types.json`. Level: UNKNOWN.

Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x0, 1 byte(s), unsigned.

Bit width 1, mask0x20.

3 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/display.c:985`: `RayEvts.handstand = 0;`
- `src/moteur.c:3157`: `RayEvts.handstand = 0;`
- `src/moteur.c:4046`: `RayEvts.handstand = 0;`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 3 source member-token matches are retained for review, including possible other types.

## field:finbosslevel_t.crazy_drummer

Owner: `types.json`. Level: UNKNOWN.

Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $ : 1`. Units: unknown.

DOS/compiled declaration layout: +0x1, 1 byte(s), unsigned.

Bit width 1, mask0x01.

2 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/basic.c:1568`: `finBosslevel.crazy_drummer = 0;`
- `src/special.c:155`: `if (num_level == 5 && (finBosslevel.crazy_drummer) != 0)`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 2 source member-token matches are retained for review, including possible other types.

## field:poing_t.field_4

Owner: `types.json`. Level: UNKNOWN.

Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x4, 2 byte(s), signed.

15 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/neige.c:836`: `flocon_tab[flc_i].field_4 = scale;`
- `src/poing.c:166`: `poing.field_4 = unk_2;`
- `src/skops.c:176`: `Bande[i].field_9 = Def_Bande[num_world - 1][Num_Fond][i].field_4;`

Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. 15 source member-token matches are retained for review, including possible other types.

## field:poing_t.field_12

Owner: `types.json`. Level: UNKNOWN.

Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x12, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:poing_t.field_13

Owner: `types.json`. Level: UNKNOWN.

Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x13, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Retain current neutral name. Layout, source-token candidates and basic typed accesses are observations only. No direct named source member access found; whole-record loads/copies can still read/write this storage.

## field:save_state_t.field_25b

Owner: `types.json`. Level: UNKNOWN.

Trailing checkpoint byte with no typed reader/writer found.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x25b, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Record extent is604 bytes; no padding or persistence meaning is inferred.

## field:record_t.unknown_17

Owner: `types.json`. Level: UNKNOWN.

Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x17, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.x_pos

Owner: `types.json`. Level: UNKNOWN.

Declared pix_gerbe_item_t.x_pos storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x0, 2 byte(s), signed.

31 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/affiche.c:961`: `fplotNormalETX(draw_buffer, cur_item->x_pos >> 6,`
- `src/collision1.c:707`: `unk_x += unk_w - cur_zdc->width - cur_zdc->x_pos;`
- `src/collision1.c:709`: `unk_x += cur_zdc->x_pos;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.y_pos

Owner: `types.json`. Level: UNKNOWN.

Declared pix_gerbe_item_t.y_pos storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x2, 2 byte(s), signed.

32 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/affiche.c:955`: `if (unk_1 >= 128 && cur_item->y_pos > (Bloc_lim_H1_Aff << 6)) {`
- `src/affiche.c:962`: `cur_item->y_pos >> 6, color);`
- `src/collision1.c:711`: `unk_y += cur_zdc->y_pos;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.speed_x

Owner: `types.json`. Level: UNKNOWN.

Declared pix_gerbe_item_t.speed_x storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x4, 2 byte(s), signed.

743 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/action.c:55`: `obj->speed_x = (obj->speed_x * (s16)(0xFF - unk_x) + (unk_4 * unk_x)) / 255;`
- `src/action.c:66`: `obj->speed_x = 0;`
- `src/action.c:72`: `obj->speed_x = 0;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.speed_y

Owner: `types.json`. Level: UNKNOWN.

Declared pix_gerbe_item_t.speed_y storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x6, 2 byte(s), signed.

725 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/action.c:61`: `obj->speed_y = (obj->speed_y * (s16)(0xFF - unk_y) + (unk_5 * unk_y)) / 255;`
- `src/action.c:67`: `obj->speed_y = 0;`
- `src/action.c:73`: `obj->speed_y = 0;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.y_accel

Owner: `types.json`. Level: UNKNOWN.

Declared pix_gerbe_item_t.y_accel storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x8, 1 byte(s), unsigned.

1 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/special.c:1336`: `cur_item->speed_y += cur_item->y_accel;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:pix_gerbe_item_t.unk_1

Owner: `types.json`. Level: UNKNOWN.

Declared pix_gerbe_item_t.unk_1 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x9, 1 byte(s), unsigned.

3 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/affiche.c:954`: `unk_1 = cur_item->unk_1;`
- `src/special.c:1326`: `if (cur_item->unk_1 >= 128) {`
- `src/special.c:1331`: `cur_item->unk_1 = 0;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_0

Owner: `types.json`. Level: UNKNOWN.

Declared flocon_t.field_0 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x0, 2 byte(s), signed.

10 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/affiche.c:880`: `x = ((multiplier * entry->field_0) >> 8) + center_x;`
- `src/affiche.c:919`: `((multiplier * entry->field_0) >> 8) + center_x,`
- `src/affiche.c:925`: `((multiplier * entry->field_0) >> 8) + center_x,`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_2

Owner: `types.json`. Level: UNKNOWN.

Declared flocon_t.field_2 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x2, 2 byte(s), signed.

11 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/affiche.c:879`: `y = ((entry->field_2 * multiplier) >> 8) + center_y;`
- `src/affiche.c:920`: `((entry->field_2 * multiplier) >> 8) + center_y);`
- `src/affiche.c:926`: `((entry->field_2 * multiplier) >> 8) + center_y);`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_4

Owner: `types.json`. Level: UNKNOWN.

Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `s16 $`. Units: unknown.

DOS/compiled declaration layout: +0x4, 2 byte(s), signed.

15 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/neige.c:836`: `flocon_tab[flc_i].field_4 = scale;`
- `src/poing.c:166`: `poing.field_4 = unk_2;`
- `src/skops.c:176`: `Bande[i].field_9 = Def_Bande[num_world - 1][Num_Fond][i].field_4;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_6

Owner: `types.json`. Level: UNKNOWN.

Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x6, 1 byte(s), unsigned.

33 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.

- `src/dark.c:127`: `VitrauxInfos[unk_1].field_6 = 0;`
- `src/skops.c:177`: `Bande[i].field_4 = Def_Bande[num_world - 1][Num_Fond][i].field_6;`
- `src/skops.c:178`: `Bande[i].field_6 = 0;`

Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

## field:flocon_t.field_7

Owner: `types.json`. Level: UNKNOWN.

Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it.

Declaration: `u8 $`. Units: unknown.

DOS/compiled declaration layout: +0x7, 1 byte(s), unsigned.

0 raw source member-token candidates; exact typed moteur accesses and DOS sites are in the registry. Token matches can belong to other types.


Layout is measured; no per-particle/snow field meaning is inferred from this unused declaration. Retain its existing name.

