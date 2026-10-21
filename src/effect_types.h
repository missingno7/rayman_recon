#ifndef W2_GAP4_EFFECT_TYPES_H
#define W2_GAP4_EFFECT_TYPES_H

#include "rtypes.h"

/* DOS record at obj2:50d14. Offsets follow direct accesses in 1:345b0..1:348a4. */
typedef struct effect_span {
    s16 source_x;
    s16 source_y;
    s16 destination_x;
    s16 destination_y;
    s16 width;
} effect_span_t;

typedef struct effect_rows {
    u8 count;
    u8 reserved_01;
    u16 position;
    u16 position_limit;
    s16 direction;
    u8 mode;
    u8 reserved_09;
    u8 reserved_0a[2];
    effect_span_t *entries;
    u8 *screen;
    u8 *destination;
} effect_rows_t;

/* Each transformed record is five dwords (DOS accesses at +0c/+10, then advances 0x14). */
typedef struct effect3d_point {
    s32 x;
    s32 y;
    s32 z;
    s32 screen_x;
    s32 screen_y;
} effect3d_point_t;

/* INVENTED: two 0x2c-byte draw records, DOS 1:5044e..504ae, 1:50a0a..50a73. */
typedef struct effect_segment {
    u8 *screen;
    s32 points[8];
    u8 rectangle[8];
} effect_segment_t;

/* DOS record at obj2:54da8. All offsets below are explicit in effect_activate and helpers. */
typedef struct effect3d {
    u16 position;
    u16 position_limit;
    s16 speed;
    s16 angle_x;
    s16 angle_y;
    s16 angle_z;
    s16 field_0c;
    s16 field_0e;
    s16 field_10;
    s16 step_x;
    s16 step_y;
    s16 step_z;
    s16 field_18;
    s16 field_1a;
    u8 field_1c;
    u8 mode;
    u8 reserved_1e[2];
    u8 *source;
    u8 *destination;
    u8 *screen_a;
    u8 *screen_b;
    effect_segment_t *vertices;
    effect3d_point_t *transformed;
} effect3d_t;


#endif
