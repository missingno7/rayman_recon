/* Private type additions for this BSS reconstruction only.
 * Include after src/bss/bt.h. Rayverse names unless marked INVENTED;
 * DOS widths/strides and Android object sizes take precedence.
 */
typedef struct couteaux_info_t {
    s16 x_pos, y_pos, field2_0x4, field3_0x6, field4_0x8, field5_0xa, field6_0xc;
    u8 field7_0xe, active, field9_0x10, id;
    u8 field_tail_0x12[2]; /* INVENTED unknown bytes; DOS stride is 0x14. */
} couteaux_info_t;

typedef struct voice_t {
    s16 obj, vol, pan, snd, field_8, field_A;
} voice_t;

typedef struct pile_snd_t {
    s16 obj, snd, prog, tone, note, vol, field_C, pan;
    s32 end_time;
    s16 field_14, field_16;
} pile_snd_t;

typedef struct sax_attack_entry_t {
    u8 next_note, time;
    s16 end;
} sax_attack_entry_t;

typedef struct sax_data_t {
    s16 x_pos, y_pos, note_box_coll_x, note_box_coll_y, sprite2_x, sprite2_y;
    u8 coup, saved_hp, field8_0xe, field9_0xf;
    s16 field10_0x10;
} sax_data_t;

typedef struct bande_t {
    s16 length, offset, field_4, field_6;
    u8 field_8, field_9;
    u8 *source_buffer_pos, *draw_buffer_pos;
    s16 field_14;
} bande_t;

typedef struct def_sprite_t {
    s16 x, y, bande_index;
    u16 field_6; /* INVENTED reserved word required by the DOS 8-byte stride. */
} def_sprite_t;

typedef struct dos_vector_t {
    u8 offset[4];
    u8 selector[2];
} dos_vector_t; /* INVENTED 32:16 DPMI vector record, six bytes per DOS entry. */
