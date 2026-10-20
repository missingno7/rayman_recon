typedef struct vec2b_t {
    u8 x;
    u8 y;
} vec2b_t;

typedef struct bande_t {
    s16 length;
    s16 offset;
    s16 field_4;
    s16 field_6;
    u8 field_8;
    u8 field_9;
    u8 _pad_0a[2];
    u8 *source_buffer_pos;
    u8 *draw_buffer_pos;
    s16 field_14;
    u8 _pad_16[2];
} bande_t;

typedef struct def_bande_t {
    s16 length;
    u8 field_2;
    u8 _pad_3;
    u8 field_4;
    u8 _pad_5;
    u8 field_6;
    u8 _pad_7;
} def_bande_t;

typedef struct def_sprite_t {
    s16 x;
    s16 y;
    s16 bande_index;
    s16 _pad_6;
} def_sprite_t;
