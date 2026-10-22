#ifndef INIT_DATA_STATE_H
#define INIT_DATA_STATE_H
#include "rtypes.h"
/* DOS 2:321c0..321d4; the renderer reads these same fields. Names INVENTED. */
typedef struct init_data_state_t {
    s16 x;
    s16 y;
    s32 draw_origin;   /* +4: draw-buffer-relative origin (DOS 1:06fd4 store, 1:2c3e8 read) */
    s32 scroll_x;      /* +8 */
    s32 scroll_y;      /* +0xc */
    s32 source_origin; /* +0x10: PLAN0BIT-relative source (DOS 1:07075 store, 1:2c403 read) */
} init_data_state_t;
#endif
