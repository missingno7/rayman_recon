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
    u16 center_x;
    u16 center_y;
    s16 velocity_x;
    s16 velocity_y;
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
extern void draw_effect_segment(u8 *destination, u8 *item);
#pragma aux draw_effect_segment parm [eax] [esi] modify exact []
extern void sinus_cosinus(s16 angle, s16 *sinus_result, s16 *cosinus_result);

#define EFFECT_MAX(a,b) ((s32)(a) < (s32)(b) ? (s32)(b) : (s32)(a))

void effect_helper_15e60(u16 radius, s16 speed, u16 frame_limit)
{
    effect5_state_t *state;
    effect5_point_t *point;
    effect5_item_t *item;
    double latitude_step;
    double longitude_step;
    double latitude;
    double longitude;
    double scaled_sin;
    double scaled_cos;
    u8 row;
    u8 column;
    u16 cell;
    u8 width_step;
    u8 height_step;
    u16 x;
    u16 y;
    s32 clamped;

    state = effect5_state;
    effect_reset_geometry();
    state->frame = 0;
    clamped = 1;
    if (frame_limit > clamped) clamped = frame_limit;
    state->frame_limit = clamped;
    if (speed >= 0) {
        clamped = 1;
        if (speed > clamped) clamped = speed;
    } else {
        clamped = 1;
        if (-speed > clamped) clamped = -speed;
        clamped = -clamped;
    }
    state->speed = clamped;
    clamped = radius;
    if (clamped > 70) clamped = 70;
    {
        s32 minimum = 40;
        if (minimum < clamped) minimum = clamped;
        state->radius = minimum;
    }
    latitude_step = 3.141592654 / state->rows;
    longitude_step = (2 * 3.141592654) / state->columns;
    point = state->points;
    latitude = -3.141592654 / 2;
    for (row = 0; row <= state->rows; row++) {
        scaled_sin = state->radius * sin(latitude);
        scaled_cos = state->radius * cos(latitude);
        longitude = 0;
        for (column = 0; column <= state->columns; column++) {
            point->x = -scaled_cos * sin(longitude);
            point->y = scaled_sin;
            point->z = -scaled_cos * cos(longitude);
            point++;
            longitude += longitude_step;
        }
        latitude += latitude_step;
    }
    item = state->items;
    for (cell = 0; cell < state->count; cell++) {
        s32 vertex;
        vertex = cell + cell / state->columns;
        item->point[0] = vertex;
        item->point[1] = vertex + 1;
        item->point[2] = vertex + state->columns + 2;
        item->point[3] = vertex + state->columns + 1;
        item++;
    }
    width_step = effect5_width / state->columns;
    height_step = effect5_height / state->rows;
    item = state->items;
    for (row = 0; row < state->rows; row++) {
        for (column = 0; column < state->columns; column++) {
            x = column * width_step;
            y = row * height_step;
            if (effect5_width <= 256)
                item->screen = state->screen_a;
            else if (x < 160)
                item->screen = state->screen_a;
            else {
                item->screen = state->screen_b;
                x -= 160;
            }
            item->rectangle[0] = item->rectangle[2] = x;
            item->rectangle[1] = item->rectangle[7] = y;
            item->rectangle[3] = item->rectangle[5] = y + height_step - 1;
            item->rectangle[4] = item->rectangle[6] = x + width_step - 1;
            item++;
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
    effect5_y_offset = 0;
    effect5_x_offset = 0x20;
    success = 0;

    if (source != 0 && destination != 0) {
        effect5_state = (effect5_state_t *)sub_6FBB4(0x3c);
        state = effect5_state;
        if (state != 0) {
            state->columns = EFFECT_MAX(2, columns);
            state->rows = EFFECT_MAX(1, rows);
            state->count = (u16)(columns * rows);
            state->point_count = (u16)((columns + 1) * (rows + 1));
            state->source = source;
            state->destination = destination;
            state->items = (effect5_item_t *)sub_6FBB4(state->count * 52);
            if (state->items != 0) {
                state->points = (effect5_point_t *)sub_6FBB4(state->point_count * 10);
                if (state->points != 0) {
                    state->screen_a = EffetBufferNormal;
                    if (effect5_width <= 0x100)
                        state->screen_b = 0;
                    else
                        state->screen_b = EffetBufferNormal + 0xc800;
                    success = 1;
                }
            }
        }
    }

    if (success != 0) {
        if (effect5_width <= 0x100) {
            effect_copy_band(state->source + effect5_y_offset * 320 + effect5_x_offset,
                             state->screen_a, effect5_width);
        } else {
            effect_copy_band(state->source, state->screen_a, 0xa0);
            effect_copy_band(state->source + 0xa0, state->screen_b, 0xa0);
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
    s32 frame;
    s32 magnitude;
    s32 step;
    s32 target_x, target_y;
    s32 projected_x, projected_y;
    s32 denominator;
    s32 x, y, z;
    s32 sine_x, cosine_x, sine_y, cosine_y;
    s16 cosine, sine;
    u8 width_step, height_step;
    u8 row, column;

    state = effect5_state;
    point = state->points;
    if (state->mode == 1) {
        if (state->speed >= 0)
            frame = state->frame;
        else
            frame = state->frame_limit - state->frame;
        width_step = effect5_width / state->columns;
        height_step = effect5_height / state->rows;
        target_y = effect5_y_offset - 100;
        for (row = 0; row <= state->rows; row++) {
            target_x = effect5_x_offset - 160;
            for (column = 0; column <= state->columns; column++) {
                denominator = point->z + 512;
                projected_x = (point->x << 9) / denominator;
                projected_y = (point->y << 9) / denominator;
                point->u = projected_x + 160 + (target_x - projected_x) * frame / state->frame_limit;
                point->v = projected_y + 100 + (target_y - projected_y) * frame / state->frame_limit;
                point++;
                target_x += width_step;
            }
            target_y += height_step;
        }
        magnitude = state->speed;
        if (magnitude < 0) magnitude = -magnitude;
        state->frame = state->frame + magnitude;
        if (state->frame > state->frame_limit) {
            if (state->speed < 0)
                state->mode = 2;
            else
                state->frame = 0;
        }
        return state->frame;
    } else {
        if (state->mode == 2) {
            state->center_x = 160;
            state->center_y = 100;
            state->velocity_x = 8;
            state->velocity_y = 10;
            state->phase_x = 0;
            state->phase_y = 0;
            state->flags_36 = 0;
            state->angle_x = 0;
            state->angle_y = 0;
            state->mode = 0;
            state->frame = 0;
            state->frame_limit = 1000;
            state->flags_37 = 0;
            state->flags_38 = 0;
        }
        if (state->flags_36 != 0) {
            magnitude = state->speed;
            if (magnitude < 0) magnitude = -magnitude;
            state->angle_y = magnitude * 4 + state->angle_y;
        }
        state->velocity_y++;
        if (state->center_x > 320 - (state->radius + 4) && state->phase_y == 0) {
            state->velocity_x = -state->velocity_x + 1;
            state->phase_y = 1;
            if (state->flags_38 != 0)
                state->flags_37 = 1;
            state->flags_38 = 1;
        } else if (state->center_x < state->radius + 4 && state->phase_y == 0) {
            state->velocity_x = -state->velocity_x - 1;
            state->phase_y = 1;
        } else if (state->center_x > state->radius + 4 || state->center_x < 320 - (state->radius + 4))
            state->phase_y = 0;
        if (state->center_y > 200 - (state->radius + 3) && state->phase_x == 0 && state->flags_37 == 0) {
            state->phase_x = 1;
            state->velocity_y = -state->velocity_y + 2;
            state->flags_36 = 1;
        } else if (state->center_y <= 200 - (state->radius + 3))
            state->phase_x = 0;
        else if (state->center_y >= 200 + state->radius + 3)
            state->frame = state->frame_limit;
        step = state->velocity_x / 4;
        magnitude = state->speed;
        if (magnitude < 0) magnitude = -magnitude;
        state->center_x = step * magnitude + state->center_x;
        step = state->velocity_y / 4;
        magnitude = state->speed;
        if (magnitude < 0) magnitude = -magnitude;
        state->center_y = step * magnitude + state->center_y;
        sinus_cosinus(state->angle_x, &sine, &cosine);
        sine_x = sine;
        cosine_x = cosine;
        sinus_cosinus(state->angle_y, &sine, &cosine);
        sine_y = sine;
        cosine_y = cosine;
        for (row = 0; row <= state->rows; row++) {
            for (column = 0; column <= state->columns; column++) {
                y = sine_y * point->x + cosine_y * point->y;
                x = (point->x * cosine_y - point->y * sine_y) >> 9;
                z = -point->z;
                projected_x = sine_x * z + cosine_x * x;
                denominator = z * cosine_x - x * sine_x;
                denominator >>= 9;
                denominator += 512;
                point->u = projected_x / denominator + state->center_x;
                point->v = y / denominator + state->center_y;
                point++;
            }
        }
        magnitude = state->speed;
        if (magnitude < 0) magnitude = -magnitude;
        state->angle_x = magnitude * 2 + state->angle_x;
        magnitude = state->speed;
        if (magnitude < 0) magnitude = -magnitude;
        state->frame = state->frame + magnitude;
        if (state->frame > state->frame_limit)
            state->frame = 0;
    }
    return state->frame;
}

void effect_helper_168c8(void)
{
    effect5_state_t *state;
    effect5_point_t *points;
    effect5_item_t *item;
    s32 u0, u1, u2, u3;
    s32 v0, v1, v2, v3;
    u16 i;

    state = effect5_state;
    points = state->points;
    item = state->items;
    for (i = 0; i < state->count; i++) {
        u0 = points[item->point[0]].u;
        v0 = points[item->point[0]].v;
        u3 = points[item->point[3]].u;
        v3 = points[item->point[3]].v;
        u2 = points[item->point[2]].u - 1;
        v2 = points[item->point[2]].v;
        u1 = points[item->point[1]].u - 1;
        v1 = points[item->point[1]].v;
        if ((u0 - u2) * (v3 - v1) < (v0 - v2) * (u3 - u1)) {
            item->projected[3] = v3;
            item->projected[7] = v1;
            item->projected[0] = u0;
            item->projected[4] = u2;
            item->projected[1] = v0;
            item->projected[5] = v2;
            item->projected[2] = u3;
            item->projected[6] = u1;
            draw_effect_segment(state->destination, (u8 *)&item->screen);
        }
        item++;
    }
}
