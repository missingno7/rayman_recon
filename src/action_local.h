#ifndef TU_ACTION_WORKER_TYPES_H
#define TU_ACTION_WORKER_TYPES_H

#include "rtypes.h"

typedef struct vec2b_t {
    u8 width;
    u8 height;
} vec2b_t;

typedef void (draw_func_t)(s32 x, s32 color, s32 y, vec2b_t size,
                           u8 *draw_buffer, u8 *image_data);
typedef void (fplot_func_t)(u8 *draw_buffer, s16 x, s16 y, u8 color);

typedef struct pix_gerbe_item_t {
    s16 x_pos;
    s16 y_pos;
    s16 speed_x;
    s16 speed_y;
    u8 y_accel;
    u8 unk_1;
} pix_gerbe_item_t;

typedef struct pix_gerbe_t {
    pix_gerbe_item_t items[64];
    s16 is_active;
} pix_gerbe_t;

typedef struct flocon_t {
    s16 field_0;
    s16 field_2;
    s16 field_4;
    u8 field_6;
    u8 field_7;
} flocon_t;

typedef struct deform_blit_t {
    s32 corner[8];
    s32 dest_width;
    s32 dest_height;
    s32 dest_offset;
    s32 source_width;
    s32 source_height;
} deform_blit_t;

typedef struct display_map_t {
    s16 x;
    s16 y;
    u8 width_in_tiles;
    u8 height_in_tiles;
    s16 tile_index;
} display_map_t;

typedef struct big_map_t {
    map_tile_t* map;
    u8** tile_texture_offsets;
    display_map_t* display_map;
    u8* map_blocks;
    u32 field_10;
    s16 field_14;
    s16 width;
    s16 height;
    s16 field_1A;
    s16 field_1C;
} big_map_t;

typedef struct init_data_state_t {
    s16 x;
    s16 y;
    s32 c4;
    s32 c8;
    s32 cc;
    s32 d0;
} init_data_state_t;

typedef struct grp_star_t {
    u8 timer;
    u8 length;
    u8 dist;
    u8 sprite_table_index;
} grp_star_t;

#endif
