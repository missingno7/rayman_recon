# Remaining unknown meanings

These entries retain their current names. The lists below report observations, not guesses about an intended role. 
Source member-token results can include other types with the same member name; consult typed basic accesses and DOS dataflow before assigning meaning.

## local:makeUturn:foll_spr:1

Declared byte temporary with no uses in this reconstruction.

Declared as an unused byte local in makeUturn; no value or observable use is established. No rename proposed.

## field:obj_t.field_1C

Neutral obj_t.field_1C storage/view; its intended meaning is not established by this pilot.

C declaration: `u32 $`; offset `0x1c`, storage 4 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

7 named source member-token matches; first examples:

- `src/moteur.c:1148`: `obj->field_1C = 1;`
- `src/moteur.c:1158`: `obj->field_1C = 1;`
- `src/moteur.c:2518`: `obj->field_1C = 0;`
- `src/moteur.c:2527`: `if (obj->field_1C == 0) {`
- `src/moteur.c:3480`: `obj->field_1C = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.link_has_gendoor

Neutral obj_t.link_has_gendoor storage/view; its intended meaning is not established by this pilot.

C declaration: `u32 $`; offset `0x20`, storage 4 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

15 named source member-token matches; first examples:

- `src/collision2.c:1219`: `if (obj->link_has_gendoor) {`
- `src/moteur.c:3032`: `if (bateau_obj->link_has_gendoor) {`
- `src/moteur.c:3089`: `level.objects[last_link].link_has_gendoor = 0;`
- `src/moteur.c:3093`: `obj->link_has_gendoor = 0;`
- `src/moteur.c:3127`: `level.objects[id].link_has_gendoor = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.active_flag

Neutral obj_t.active_flag storage/view; its intended meaning is not established by this pilot.

C declaration: `s32 $`; offset `0x30`, storage 4 byte(s), signed.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

18 named source member-token matches; first examples:

- `src/breakout.c:509`: `bonus->active_flag = 0;`
- `src/fee.c:389`: `if (png_or_fee->active_flag == 2 && !png_or_fee->is_active) {`
- `src/fee.c:391`: `png_or_fee->active_flag = 0;`
- `src/moteur.c:1080`: `cur_obj->active_flag = ACTIVE_ALIVE;`
- `src/moteur.c:1149`: `obj->active_flag = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.sub_speed

Neutral obj_t.sub_speed storage/view; its intended meaning is not established by this pilot.

C declaration: `s16 $`; offset `0x3a`, storage 2 byte(s), signed.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

7 named source member-token matches; first examples:

- `src/collision2.c:790`: `obj->sub_speed += 14;`
- `src/collision2.c:791`: `if (obj->sub_speed > 16) {`
- `src/collision2.c:792`: `obj->sub_speed -= 16;`
- `src/frame.c:1669`: `obj->sub_speed = (obj->offset_bx + obj->x) - (ray.x + ray.offset_bx);`
- `src/frame.c:1670`: `if (Abs(obj->sub_speed) > obj->detect_zone + 16)`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.link

Neutral obj_t.link storage/view; its intended meaning is not established by this pilot.

C declaration: `s16 $`; offset `0x52`, storage 2 byte(s), signed.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

208 named source member-token matches; first examples:

- `src/araignee.c:661`: `obj->link = 0;`
- `src/araignee.c:695`: `obj->link = 0;`
- `src/araignee.c:715`: `if ((obj->main_etat == 0) && (obj->sub_etat == 3) && (obj->link == 0))`
- `src/araignee.c:718`: `obj->link = 1;`
- `src/araignee.c:795`: `if (obj->main_etat == 1 && obj->sub_etat == 0 && obj->link == 0) {`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.zdc

Neutral obj_t.zdc storage/view; its intended meaning is not established by this pilot.

C declaration: `u16 $`; offset `0x5c`, storage 2 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

6 named source member-token matches; first examples:

- `src/collision1.c:101`: `return obj->zdc >> 11;`
- `src/collision1.c:106`: `return obj->zdc & 0x7FF;`
- `src/collision1.c:680`: `if (obj->zdc != 0)`
- `src/frame.c:1398`: `obj->zdc = type_zdc[obj->type];`
- `src/meredenis.c:346`: `obj->zdc = type_zdc[obj->type];`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.active_timer

Neutral obj_t.active_timer storage/view; its intended meaning is not established by this pilot.

C declaration: `u16 $`; offset `0x5e`, storage 2 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

12 named source member-token matches; first examples:

- `src/breakout.c:511`: `bonus->active_timer = 0;`
- `src/fee.c:366`: `png_or_fee->active_timer = 0;`
- `src/fee.c:390`: `png_or_fee->active_timer = 0;`
- `src/moteur.c:1079`: `cur_obj->active_timer = 0;`
- `src/moteur.c:1281`: `if (obj->active_timer == 0 &&`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.unused_padding

Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x67`, storage 1 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.init_flag

Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x78`, storage 1 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..9; 8 distinct. This does not bound runtime writes.

11 named source member-token matches; first examples:

- `src/cage.c:87`: `obj->init_flag = 7;`
- `src/objinit.c:1088`: `obj->init_flag = 7;`
- `src/objinit.c:1093`: `obj->init_flag = 7;`
- `src/objinit.c:1158`: `obj->init_flag = 7;`
- `src/objinit.c:1161`: `obj->init_flag = 7;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.hit_sprite

Neutral obj_t.hit_sprite storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x79`, storage 1 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..255; 6 distinct. This does not bound runtime writes.

10 named source member-token matches; first examples:

- `src/collision1.c:739`: `if (obj->hit_sprite == 0xFE)`
- `src/collision1.c:750`: `else if (obj->hit_sprite == 0xFF)`
- `src/collision1.c:778`: `succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);`
- `src/collision1.c:780`: `return succ ? obj->hit_sprite : -1;`
- `src/collision1.c:785`: `s16 hit_sprite = obj->hit_sprite;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.timer

Neutral obj_t.timer storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x7f`, storage 1 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

159 named source member-token matches; first examples:

- `src/affiche.c:521`: `if (c_1up->timer == 0) {`
- `src/affiche.c:532`: `if (c_pow->timer == 0) {`
- `src/affiche.c:1210`: `star->timer++;`
- `src/affiche.c:1211`: `if (star->timer > star->length) {`
- `src/affiche.c:1213`: `star->timer = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_t.field_83

Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x83`, storage 1 byte(s), unsigned.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:eta_t.flags_bits.bit0

Neutral eta_t.flags_bits.bit0 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x7`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x01`.

Serialized initial values: 2639 records; range 0..1; 2 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:eta_t.flags_bits.bit1

Neutral eta_t.flags_bits.bit1 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x7`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x02`.

Serialized initial values: 2639 records; range 0..1; 2 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:eta_t.flags_bits.bit2

Neutral eta_t.flags_bits.bit2 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x7`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x04`.

Serialized initial values: 2639 records; range 0..1; 2 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:eta_t.flags_bits.bit3

Neutral eta_t.flags_bits.bit3 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x7`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x08`.

Serialized initial values: 2639 records; range 0..1; 2 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:eta_t.flags_bits.bit5

Neutral eta_t.flags_bits.bit5 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x7`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x20`.

Serialized initial values: 2639 records; range 0..1; 2 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:eta_t.flags_bits.bit7

Neutral eta_t.flags_bits.bit7 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x7`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x80`.

Serialized initial values: 2639 records; range 0..1; 2 distinct. This does not bound runtime writes.

1 named source member-token matches; first examples:

- `src/collision1.c:475`: `inset = 24 - 12 * (get_eta(obj)->flags_bits.bit7 + 2 * get_eta(obj)->flags_bits.bit6);`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:sprite_t.color_bits.unused

Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 4`; offset `0xa`, storage 1 byte(s), unsigned.

Bit width 4; mask `0x0f`.

Serialized initial values: 5512 records; range 0..3; 3 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:sprite_t.flags_bits.unused0

Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 2`; offset `0xb`, storage 1 byte(s), unsigned.

Bit width 2; mask `0x03`.

Serialized initial values: 5512 records; range 0..3; 4 distinct. This does not bound runtime writes.

5 named source member-token matches; first examples:

- `src/mite.c:264`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0) {`
- `src/mite.c:478`: `if (block_flags[obj->btypes[3]].bits.unused0)`
- `src/mite.c:481`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0)`
- `src/mite.c:542`: `].bits.unused0 &&`
- `src/mite.c:556`: `if (block_flags[unk_1].bits.unused0)`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:sprite_t.flags_bits.unused3

Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 5`; offset `0xb`, storage 1 byte(s), unsigned.

Bit width 5; mask `0xf8`.

Serialized initial values: 5512 records; range 0..31; 31 distinct. This does not bound runtime writes.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:obj_flags_t.flag_0x40

Neutral obj_flags_t.flag_0x40 storage/view; its intended meaning is not established by this pilot.

C declaration: `unsigned char $ : 1`; offset `0x0`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x40`.

Serialized initial values: 11837 records; range 0..0; 1 distinct. This does not bound runtime writes.

21 named source member-token matches; first examples:

- `src/araignee.c:756`: `cur_obj->flags.flag_0x40 = 0;`
- `src/bbmont.c:391`: `cur->flags.flag_0x40 = 0;`
- `src/blacktoon.c:65`: `cur_obj->flags.flag_0x40 = 0;`
- `src/cage.c:244`: `obj->flags.flag_0x40 = 0;`
- `src/clown.c:260`: `cur_obj->flags.flag_0x40 = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:map_tile_t.field_3

Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x3`, storage 1 byte(s), unsigned.

Shipped map-byte domains: +3: 241 distinct (0..255).

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:map_tile_t.field_5

Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x5`, storage 1 byte(s), unsigned.

Shipped map-byte domains: +5: 240 distinct (0..255).

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:block_flags_t.bits.unused0

Neutral block_flags_t.bits.unused0 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x0`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x01`.

5 named source member-token matches; first examples:

- `src/mite.c:264`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0) {`
- `src/mite.c:478`: `if (block_flags[obj->btypes[3]].bits.unused0)`
- `src/mite.c:481`: `if (block_flags[calc_typ_travd(obj, false)].bits.unused0)`
- `src/mite.c:542`: `].bits.unused0 &&`
- `src/mite.c:556`: `if (block_flags[unk_1].bits.unused0)`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:block_flags_t.bits.unused2

Neutral block_flags_t.bits.unused2 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 3`; offset `0x0`, storage 1 byte(s), unsigned.

Bit width 3; mask `0x1c`.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:block_flags_t.bits.unused1

Neutral block_flags_t.bits.unused1 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 2`; offset `0x0`, storage 1 byte(s), unsigned.

Bit width 2; mask `0xc0`.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:rayevts_t.handstand_dash

Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x0`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x10`.

3 named source member-token matches; first examples:

- `src/display.c:984`: `RayEvts.handstand_dash = 0;`
- `src/moteur.c:3156`: `RayEvts.handstand_dash = 0;`
- `src/moteur.c:4045`: `RayEvts.handstand_dash = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:rayevts_t.handstand

Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x0`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x20`.

3 named source member-token matches; first examples:

- `src/display.c:985`: `RayEvts.handstand = 0;`
- `src/moteur.c:3157`: `RayEvts.handstand = 0;`
- `src/moteur.c:4046`: `RayEvts.handstand = 0;`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:finbosslevel_t.crazy_drummer

Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $ : 1`; offset `0x1`, storage 1 byte(s), unsigned.

Bit width 1; mask `0x01`.

2 named source member-token matches; first examples:

- `src/basic.c:1615`: `finBosslevel.crazy_drummer = 0;`
- `src/special.c:155`: `if (num_level == 5 && (finBosslevel.crazy_drummer) != 0)`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:poing_t.field_4

Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot.

C declaration: `s16 $`; offset `0x4`, storage 2 byte(s), signed.

15 named source member-token matches; first examples:

- `src/neige.c:836`: `flocon_tab[flc_i].field_4 = scale;`
- `src/poing.c:166`: `poing.field_4 = unk_2;`
- `src/skops.c:176`: `Bande[i].field_9 = Def_Bande[num_world - 1][Num_Fond][i].field_4;`
- `src/skops.c:177`: `Bande[i].field_4 = Def_Bande[num_world - 1][Num_Fond][i].field_6;`
- `src/skops.c:450`: `} else if (Bande[i].field_4 == 0) {`

All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:poing_t.field_12

Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x12`, storage 1 byte(s), unsigned.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

## field:poing_t.field_13

Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot.

C declaration: `u8 $`; offset `0x13`, storage 1 byte(s), unsigned.

0 named source member-token matches; first examples:

No direct named member access found. Whole-record loads/copies remain possible.


All matches and displacement candidates remain in the evidence files referenced by this registry record.

