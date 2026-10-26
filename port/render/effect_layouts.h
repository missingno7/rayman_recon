/*
 * Layouts of the screen effects' private state structures, for render/effect_capture.c, which reads them. Each is a
 * copy of the definition in its historical unit (named after it, port_ prefix); render/effect_layout_*.c compiles
 * the unit itself next to these copies and asserts that every field has the same offset and type size, so a change
 * of the original definition fails the build instead of being misread.
 */
#ifndef PORT_RENDER_EFFECT_LAYOUTS_H
#define PORT_RENDER_EFFECT_LAYOUTS_H

#include "rtypes.h"

/* src/effect633e0.c effect_column_state_t (its derivative: build/port/derived/effect633e0.c) */
typedef struct port_effect_column_state {
    s16 pos_x;
    s16 delta;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    u8 mode;
    u8 initialized;
    u16 field_1a;
    u16 width;
    u16 span;
    u16 period1;
    u16 period2;
    s32 *table;
    u8 *source;
    u8 *destination;
} port_effect_column_state_t;

/* src/effect6c3b0.c effect_state_6c_t (its derivative: build/port/derived/effect6c3b0.c) */
typedef struct port_effect_state_6c {
    u16 frame;
    u16 step;
    s16 x;
    s16 y;
    s16 velocity_x;
    s16 velocity_y;
    s16 velocity;
    u16 radius;
    u16 width;
    u16 height;
    u8 *map;
    u8 *screen;
    u8 *output;
} port_effect_state_6c_t;

#endif
