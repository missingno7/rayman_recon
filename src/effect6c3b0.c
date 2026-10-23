#include "inline_helpers.h"
/* DOS-derived PC-only radial distortion, 1:6c3b0..1:6ca90.
 * Function/state names are INVENTED (inherited from w2_gap6).
 * NEAR candidate: see NOTES.md for remaining byte/layout differences. */
#include <stdlib.h>
#include <string.h>
#include "rtypes.h"

#include <math.h>
typedef struct effect_state_6c_t {
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
} effect_state_6c_t;

extern effect_state_6c_t *effect_state_6c;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(u32 size);
extern void JumelleFreeArray(void **array);
extern void effect_helper_6c3b0(s16 x, s16 y, s16 velocity, u16 step);
extern void effect_cleanup_6c7a0(void);

void effect_helper_6c3b0(s16 x, s16 y, s16 velocity, u16 step)
{
    s32 random_value;
    double cosine;
    double coordinate_x;
    double amplitude;
    double sine;
    double transformed_y;
    double angle;
    double coordinate_y;
    double transformed_x;
    double distance;
    effect_state_6c_t *state;
    s32 radius;
    double scale;
    u8 *map;

    state = effect_state_6c;
    map = state->map;

    state->frame = 0;
    state->step = max32(1, step);
    {
        s16 adjusted;
        if (velocity >= 0)
            adjusted = max32(1, velocity);
        else
            adjusted = -max32(1, -velocity);
        state->velocity = adjusted;
    }

    if (x != -1)
        state->x = x;
    if (y != -1)
        state->y = y;

    state->x = max32(state->radius, min32(state->x, 319 - state->radius));
    state->y = max32(state->radius, min32(state->y, 199 - state->radius));
    random_value = rand() & 7;
    state->velocity_x = random_value < 4 ? state->velocity : (s16)-state->velocity;
    random_value = rand() & 7;
    state->velocity_y = random_value < 4 ? state->velocity : (s16)-state->velocity;

    if (x == -1 || y == -1)
        return;

    radius = state->radius;
    {
        double factor = 100.0 / (radius * 50);
        scale = factor * factor;
    }
    for (coordinate_y = -radius; coordinate_y <= state->radius; coordinate_y += 1.0) {
        double square_y = coordinate_y * coordinate_y;
        for (coordinate_x = -state->radius; coordinate_x <= state->radius; coordinate_x += 1.0) {
            distance = sqrt(coordinate_x * coordinate_x + square_y);
            amplitude = distance * exp(-scale * distance * distance);
            if (coordinate_x == 0.0 && coordinate_y == 0.0) {
                transformed_x = coordinate_x;
                transformed_y = coordinate_y;
            } else {
                angle = atan2(coordinate_y, coordinate_x);
                cosine = cos(angle);
                sine = sin(angle);
                transformed_x = distance * cosine - amplitude * sine;
                transformed_y = distance * sine + amplitude * cosine;
            }
            map[0] = (s32)(transformed_x - coordinate_x);
            map[1] = (s32)(transformed_y - coordinate_y);
            map += 2;
        }
    }
}

void effect_cleanup_6c7a0(void)
{
    effect_state_6c_t *state = effect_state_6c;
    if (state != 0) {
        if (state->map != 0)
            JumelleFreeArray((void **)&state->map);
        if (effect_state_6c != 0)
            JumelleFreeArray((void **)&effect_state_6c);
    }
}

u8 effect_helper_6c7cc(u16 x, u16 y, u16 radius, s16 speed,
                       u16 step, u8 *screen, u8 *output)
{
    effect_state_6c_t *state;
    u8 success;
    s32 bytes;

    success = 0;
    if (screen != 0 && output != 0) {
        state = (effect_state_6c_t *)sub_6FBB4(0x20);
        effect_state_6c = state;
        if (state != 0) {
            state->radius = max32(8, min32(radius, 48));
            state->width = (u16)(state->radius * 2 + 1);
            state->height = (u16)(state->radius * 2 + 1);
            copy(screen, EffetBufferNormal, 0xfa00);
            state->screen = EffetBufferNormal;
            state->output = output;
            bytes = (s32)state->height * state->width * 2;
            state->map = (u8 *)sub_6FBB4(bytes);
            if (state->map != 0) {
                success = 1;
            }
        }
    }
    if (success)
        effect_helper_6c3b0(x, y, speed, step);
    else
        effect_cleanup_6c7a0();
    return success;
}

s16 effect_helper_6c8d4(void)
{
    effect_state_6c_t *state = effect_state_6c;
    s32 delta;
    state->x = state->velocity_x + state->x;
    state->y = state->velocity_y + state->y;
    if (state->x + state->radius >= 320) {
        state->x = 319 - state->radius;
        state->velocity_x = -state->velocity_x;
    }
    if (state->x - state->radius < 0) {
        state->x = state->radius;
        state->velocity_x = -state->velocity_x;
    }
    if (state->y + state->radius >= 200) {
        state->y = 199 - state->radius;
        state->velocity_y = -state->velocity_y;
    }
    if (state->y - state->radius < 0) {
        state->y = state->radius;
        state->velocity_y = -state->velocity_y;
    }
    delta = state->velocity;
    delta = delta >= 0 ? delta : -delta;
    state->frame += delta;
    if (state->frame > state->step) state->frame = 0;
    return state->frame;
}

void effect_helper_6c9bc(void)
{
    effect_state_6c_t *state = effect_state_6c;
    u8 *source;
    u8 *destination;
    u8 *map = state->map;
    s32 x;
    s32 y;
    s32 stride;

    copy(state->screen, state->output, 0xfa00);
    x = state->x - state->radius;
    y = state->y - state->radius;
    x = x + y * 64 + y * 256;
    source = state->screen + x;
    destination = state->output + x;
    stride = 320 - state->width;
    for (y = 0; y < state->height; ++y) {
        for (x = 0; x < state->width; ++x) {
            s32 dy = (s8)map[1];
            s32 dx = (s8)map[0];
            s32 offset = dx + dy * 320;
            *destination++ = source[offset];
            source++;
            map += 2;
        }
        source += stride;
        destination += stride;
    }
}
