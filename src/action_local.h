#ifndef TU_ACTION_WORKER_TYPES_H
#define TU_ACTION_WORKER_TYPES_H

#include "rtypes.h"
#include "map.h"

typedef struct vec2b_t {
    u8 width;
    u8 height;
} vec2b_t;

typedef void (draw_func_t)(s32 x, s32 color, s32 y, u32 size,
                           u8 *draw_buffer, u8 *image_data);
/* DOS assembly sprite entries 1:063e6..06d50 use these six registers. */
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]
/* DOS fplot_X 1:02b86 preserves ECX/EDX/EDI, modifies EAX/EBX. */
typedef void (fplot_func_t)(u8 *draw_buffer, s32 x, s32 y, s32 color);
#pragma aux fplot_func_t parm [edi] [eax] [ebx] [esi] modify exact [eax ebx]

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

/* DOS descriptor 1:0baec..0bb5f; drawing assembler reads stride at +0x20. */
typedef struct deform_blit_t {
    s32 x_step_1;
    s32 row_step_1;
    s32 dx_1;
    s32 dy_1;
    s32 x_step_2;
    s32 row_step_2;
    s32 dx_2;
    s32 dy_2;
    s32 dest_width;
    s32 dest_height;
    s32 dest_offset;
    s32 source_width;
    s32 source_height;
} deform_blit_t;

#include "init_data_state.h"

typedef struct grp_star_t {
    u8 timer;
    u8 length;
    u8 dist;
    u8 sprite_table_index;
} grp_star_t;

#endif
