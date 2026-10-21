/* Worker candidate for the unnamed DOS effect block 1:15e60..1:16a20.
 * All semantic names here are INVENTED; the oracle controls every address and byte. */
#include "rtypes.h"
#include <math.h>

typedef struct effect5_point {
    s16 x;
    s16 y;
    s16 z;
    s16 u;
    s16 v;
} effect5_point_t;

typedef struct effect5_item {
    u16 point[4];
    u8 *screen;
    s32 projected[8];
    u8 rectangle[8];
} effect5_item_t;

typedef struct effect5_state {
    u8 columns;
    u8 rows;
    u16 count;
    u16 point_count;
    u16 frame;
    u16 frame_limit;
    s16 speed;
    u8 radius;
    u8 mode;
    u16 reserved_0e;
    u8 *source;
    u8 *destination;
    u8 *screen_a;
    u8 *screen_b;
    effect5_point_t *points;
    effect5_item_t *items;
    s16 center_x;
    s16 center_y;
    s16 radius_x;
    s16 radius_y;
    s16 angle_x;
    s16 angle_y;
    u8 phase_x;
    u8 phase_y;
    u8 flags_36;
    u8 flags_37;
    u8 flags_38;
    u8 reserved_39[3];
} effect5_state_t;

extern effect5_state_t *effect5_state;
extern s16 effect5_width;
extern s16 effect5_height;
extern s16 effect5_x_offset;
extern s16 effect5_y_offset;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(s32 size);
extern void JumelleFreeArray(void **p);
extern void effect_copy_band(u8 *source, u8 *destination, u32 width);
extern void effect_reset_geometry(void);
#pragma aux effect_reset_geometry modify exact []
extern void draw_effect_segment(u8 *destination, u8 *item);
#pragma aux draw_effect_segment parm [eax] [esi] modify exact []
extern void sinus_cosinus(s16 angle, s16 *sinus_result, s16 *cosinus_result);

void effect_helper_15e60(u16 radius, s16 speed, u16 frame_limit)
{
    effect5_state_t *state;
    double latitude_step;
    double longitude_step;
    double latitude;
    double longitude;
    double radius_value;
    s16 row;
    s16 column;
    u16 point_index;
    u16 cell_index;
    u16 width_step;
    u16 height_step;

    state = effect5_state;
    effect_reset_geometry();

    state->frame = 0;
    state->frame_limit = frame_limit < 1 ? 1 : frame_limit;
    if (speed >= 0) {
        if (speed < 1)
            speed = 1;
    } else if (speed > -1) {
        speed = -1;
    }
    state->speed = speed;
    if (radius > 70)
        radius = 70;
    if (radius < 40)
        radius = 40;
    state->radius = (u8)radius;

    latitude_step = 3.14159265358979323846 / state->rows;
    longitude_step = (2.0 * 3.14159265358979323846) / state->columns;
    radius_value = state->radius;
    point_index = 0;
    for (row = 0; row <= state->rows; ++row) {
        latitude = -1.57079632679489661923 + row * latitude_step;
        for (column = 0; column <= state->columns; ++column) {
            longitude = column * longitude_step;
            state->points[point_index].x = (s16)(radius_value * cos(latitude) * cos(longitude));
            state->points[point_index].y = (s16)(radius_value * cos(latitude) * sin(longitude));
            state->points[point_index].z = (s16)(radius_value * sin(latitude));
            ++point_index;
        }
    }

    width_step = (u16)(effect5_width / state->columns);
    height_step = (u16)(effect5_height / state->rows);
    cell_index = 0;
    for (row = 0; row < state->rows; ++row) {
        for (column = 0; column < state->columns; ++column) {
            effect5_item_t *item = &state->items[cell_index];
            u16 x;
            u16 y;
            u16 right;
            u16 bottom;
            u16 vertex;

            vertex = (u16)(row * (state->columns + 1) + column);
            item->point[0] = vertex;
            item->point[1] = vertex + 1;
            item->point[2] = vertex + state->columns + 2;
            item->point[3] = vertex + state->columns + 1;

            x = (u16)(column * width_step);
            y = (u16)(row * height_step);
            right = (u16)(x + width_step - 1);
            bottom = (u16)(y + height_step - 1);
            if (effect5_width > 256 && x >= 160) {
                item->screen = state->screen_b;
                x -= 160;
                right -= 160;
            } else {
                item->screen = state->screen_a;
            }
            item->rectangle[0] = (u8)x;
            item->rectangle[1] = (u8)y;
            item->rectangle[2] = (u8)x;
            item->rectangle[3] = (u8)bottom;
            item->rectangle[4] = (u8)right;
            item->rectangle[5] = (u8)bottom;
            item->rectangle[6] = (u8)right;
            item->rectangle[7] = (u8)y;
            ++cell_index;
        }
    }
    state->mode = 1;
}

void effect_cleanup_16200(void)
{
    effect5_state_t *state;

    state = effect5_state;
    if (state != 0) {
        if (state->points != 0)
            JumelleFreeArray((void **)&state->points);
        if (state->items != 0)
            JumelleFreeArray((void **)&state->items);
        if (effect5_state != 0)
            JumelleFreeArray((void **)&effect5_state);
    }
}

u8 effect_helper_1623c(u16 columns, u16 rows, u16 radius, s16 speed,
                       u16 frame_limit, u8 *source, u8 *destination)
{
    effect5_state_t *state;
    u8 success;

    effect5_width = 0x100;
    effect5_height = 0xc8;
    effect5_x_offset = 0x20;
    effect5_y_offset = 0;
    success = 0;

    if (source != 0 && destination != 0) {
        effect5_state = (effect5_state_t *)sub_6FBB4(0x3c);
        state = effect5_state;
        if (state != 0) {
            state->columns = columns < 2 ? 2 : (u8)columns;
            state->rows = rows < 1 ? 1 : (u8)rows;
            state->count = (u16)(columns * rows);
            state->point_count = (u16)((columns + 1) * (rows + 1));
            state->source = source;
            state->destination = destination;
            state->screen_a = EffetBufferNormal;
            state->screen_b = effect5_width > 0x100 ? EffetBufferNormal + 0xc800 : 0;
            state->points = (effect5_point_t *)sub_6FBB4(state->point_count * 10);
            if (state->points != 0) {
                state->items = (effect5_item_t *)sub_6FBB4(state->count * 52);
                if (state->items != 0)
                    success = 1;
            }
        }
    }

    if (success != 0) {
        if (effect5_width <= 0x100) {
            effect_copy_band(source + effect5_y_offset * 320 + effect5_x_offset,
                             state->screen_a, effect5_width);
        } else {
            effect_copy_band(source, state->screen_a, 0xa0);
            effect_copy_band(source + 0xa0, state->screen_b, 0xa0);
        }
        effect_helper_15e60(radius, speed, frame_limit);
    } else {
        effect_cleanup_16200();
    }
    return success;
}

s16 effect_helper_16404(void)
{
    effect5_state_t *state;
    effect5_point_t *point;
    effect5_item_t *item;
    s16 frame_step;
    s16 x_sin;
    s16 x_cos;
    s16 y_sin;
    s16 y_cos;
    s16 i;

    state = effect5_state;
    if (state->mode == 1) {
        frame_step = state->speed;
        if (frame_step < 0)
            frame_step = -frame_step;
        point = state->points;
        for (i = 0; i < state->point_count; ++i) {
            point->x = (s16)(point->x + frame_step);
            point->y = (s16)(point->y + state->speed);
            ++point;
        }
        state->frame = (u16)(state->frame + frame_step);
        if (state->frame > state->frame_limit) {
            if (state->speed < 0)
                state->mode = 2;
            else
                state->frame = 0;
        }
    }

    if (state->mode == 2) {
        state->center_x = 160;
        state->center_y = 100;
        state->radius_x = 8;
        state->radius_y = 10;
        state->angle_x = 0;
        state->angle_y = 0;
        state->phase_x = 0;
        state->phase_y = 0;
        state->flags_36 = 0;
        state->flags_37 = 0;
        state->flags_38 = 0;
        state->frame = 0;
        state->frame_limit = 1000;
        state->mode = 0;
    }

    sinus_cosinus(state->angle_x, &x_sin, &x_cos);
    sinus_cosinus(state->angle_y, &y_sin, &y_cos);
    item = state->items;
    for (i = 0; i < state->count; ++i) {
        s16 corner;
        for (corner = 0; corner < 4; ++corner) {
            point = &state->points[item->point[corner]];
            item->projected[corner * 2] = state->center_x + point->x * x_cos / 512;
            item->projected[corner * 2 + 1] = state->center_y + point->y * y_cos / 512;
        }
        ++item;
    }
    return (s16)state->frame;
}

void effect_helper_168c8(void)
{
    effect5_state_t *state;
    effect5_item_t *item;
    effect5_point_t *p0;
    effect5_point_t *p1;
    effect5_point_t *p2;
    effect5_point_t *p3;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 u2;
    s32 v2;
    s32 u3;
    s32 v3;
    s32 i;

    state = effect5_state;
    item = state->items;
    for (i = 0; i < state->count; ++i) {
        s32 area;

        p0 = &state->points[item->point[0]];
        p1 = &state->points[item->point[1]];
        p2 = &state->points[item->point[2]];
        p3 = &state->points[item->point[3]];
        u0 = p0->u;
        v0 = p0->v;
        u1 = p1->u - 1;
        v1 = p1->v;
        u2 = p2->u - 1;
        v2 = p2->v;
        u3 = p3->u;
        v3 = p3->v;
        area = (u0 - u3) * (v2 - v1) - (v0 - v3) * (u2 - u1);
        if (area < 0) {
            item->projected[0] = u0;
            item->projected[1] = v0;
            item->projected[2] = u1;
            item->projected[3] = v1;
            item->projected[4] = u2;
            item->projected[5] = v2;
            item->projected[6] = u3;
            item->projected[7] = v3;
            draw_effect_segment(state->destination, (u8 *)&item->screen);
        }
        ++item;
    }
}
