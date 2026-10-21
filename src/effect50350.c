/* Candidate for obj1:50350..50ad0. The switch table begins the _TEXT extent. */
#include "effect_types.h"
#include "inline_helpers.h"
#include <string.h>

extern effect3d_t *dword_E4DA8;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(s32 size);
extern void JumelleFreeArray(void **p);
extern void effect_reset_geometry(void);
extern void effect_copy_band(u8 *source, u8 *destination, u32 width);
extern void sinus_cosinus(s16 angle, s16 *cosinus_result, s16 *sinus_result);
extern void draw_effect_segment(u8 *destination, u8 *points);
#pragma aux draw_effect_segment parm [eax] [esi] modify exact []

#define effect_data() (dword_E4DA8)

void effect_activate(u8 mode, s16 speed)
{
    effect3d_t *state = effect_data();

    s32 magnitude;
    s32 step;
    effect_segment_t *segment;
    u8 *matrix_bytes;

    matrix_bytes = (u8 *)state->transformed;
    effect_reset_geometry();
    state->speed = speed >= 0 ? -max32(1, speed) : max32(1, -speed);
    state->mode = (u8)min32(max32(1, mode), 15);

    *(u32 *)(matrix_bytes + 0x14) = 0;
    *(u32 *)(matrix_bytes + 0x54) = 0;
    *(u32 *)(matrix_bytes + 0x3c) = 0x13f;
    *(u32 *)(matrix_bytes + 0x68) = 0xc7;
    *(u32 *)(matrix_bytes + 0x64) = 0xa0;
    *(u32 *)(matrix_bytes + 0x6c) = 0;
    *(u32 *)(matrix_bytes + 0x00) = *(u32 *)(matrix_bytes + 0x14);
    *(u32 *)(matrix_bytes + 0x40) = *(u32 *)(matrix_bytes + 0x54);
    *(u32 *)(matrix_bytes + 0x04) = *(u32 *)(matrix_bytes + 0x54);
    *(u32 *)(matrix_bytes + 0x28) = *(u32 *)(matrix_bytes + 0x3c);
    *(u32 *)(matrix_bytes + 0x2c) = *(u32 *)(matrix_bytes + 0x68);
    *(u32 *)(matrix_bytes + 0x18) = *(u32 *)(matrix_bytes + 0x68);
    *(u32 *)(matrix_bytes + 0x50) = *(u32 *)(matrix_bytes + 0x64);
    *(u32 *)(matrix_bytes + 0x58) = *(u32 *)(matrix_bytes + 0x6c);
    *(u32 *)(matrix_bytes + 0x44) = *(u32 *)(matrix_bytes + 0x6c);
    *(u32 *)(matrix_bytes + 0x30) = *(u32 *)(matrix_bytes + 0x6c);
    *(u32 *)(matrix_bytes + 0x1c) = *(u32 *)(matrix_bytes + 0x6c);
    *(u32 *)(matrix_bytes + 0x08) = *(u32 *)(matrix_bytes + 0x6c);

    segment = state->vertices;
    segment->rectangle[2] = 0;
    segment->rectangle[6] = 0x9f;
    segment->rectangle[7] = 0;
    segment->rectangle[5] = 0xc7;
    segment->screen = state->screen_a;
    segment->rectangle[0] = segment->rectangle[2];
    segment->rectangle[4] = segment->rectangle[6];
    segment->rectangle[1] = segment->rectangle[7];
    segment->rectangle[3] = segment->rectangle[5];
    segment = state->vertices + 1;
    segment->rectangle[2] = 0;
    segment->screen = state->screen_b;
    segment->rectangle[0] = segment->rectangle[2];
    segment->rectangle[6] = 0x9f;
    segment->rectangle[7] = 0;
    segment->rectangle[5] = 0xc7;
    segment->rectangle[4] = segment->rectangle[6];
    segment->rectangle[1] = segment->rectangle[7];
    segment->rectangle[3] = segment->rectangle[5];

    state->position = 0;
    state->position_limit = 0x200;
    state->angle_z = 0;
    state->field_10 = 0;
    state->step_z = 0;
    state->field_18 = 0xa0;
    state->angle_y = state->angle_z;
    state->angle_x = state->angle_z;
    state->field_0e = state->field_10;
    state->field_0c = state->field_10;
    state->step_y = state->step_z;
    state->step_x = state->step_z;
    state->field_1a = 100;
    state->field_1c = 0;
    magnitude = (s32)state->speed;
    if (magnitude < 0) magnitude = -magnitude;

    step = magnitude;
    switch (state->mode) {
    case 3: state->step_z = (s16)step; break;
    case 4: state->step_y = (s16)step; step *= 2; state->step_x = (s16)step; break;
    case 5: state->step_z = (s16)step;
    case 2: step *= 2; state->step_y = (s16)step; break;
    case 6: state->step_z = (s16)step; step *= 2; state->step_x = (s16)step; break;
    case 7: state->step_z = (s16)step; state->step_y = (s16)(3 * step); step *= 2;
    case 1: state->step_x = (s16)step; break;
    case 8:
        state->angle_x = 0x200; state->field_1a = 201; state->field_1c = 1;
        state->step_x = state->speed; state->step_x = -state->step_x; if (state->speed <= 0) state->angle_x = 0x180; break;
    case 9:
        state->field_1a = 201; state->field_1c = 1; state->step_x = state->speed;
        if (state->speed <= 0) state->angle_x = 0x80; break;
    case 10:
        state->field_1a = (s16)-2; state->field_1c = 1; state->step_x = state->speed;
        if (state->speed <= 0) state->angle_x = 0x80; break;
    case 11:
        state->angle_x = 0x200; state->field_1a = (s16)-2; state->field_1c = 1;
        state->step_x = state->speed; state->step_x = -state->step_x; if (state->speed <= 0) state->angle_x = 0x180; break;
    case 12:
        state->field_18 = (s16)-2; state->field_1c = 1; state->step_y = state->speed;
        if (state->speed <= 0) state->angle_y = 0x80; break;
    case 13:
        state->angle_y = 0x200; state->field_18 = (s16)-2; state->field_1c = 1;
        state->step_y = state->speed; state->step_y = -state->step_y; if (state->speed <= 0) state->angle_y = 0x180; break;
    case 14:
        state->angle_y = 0x200; state->field_18 = 0x141; state->field_1c = 1;
        state->step_y = state->speed; state->step_y = -state->step_y; if (state->speed <= 0) state->angle_y = 0x180; break;
    case 15:
        state->field_18 = 0x141; state->field_1c = 1; state->step_y = state->speed;
        if (state->speed <= 0) state->angle_y = 0x80; break;
    default: break;
    }
    if (state->field_1c == 1)
        state->position_limit = magnitude + 0x80;
}

void effect_cleanup_506fc(void)
{
    effect3d_t *state = effect_data();
    if (state == 0)
        return;
    if (state->vertices != 0)
        JumelleFreeArray((void **)&state->vertices);
    if (state->transformed != 0)
        JumelleFreeArray((void **)&state->transformed);
    if (dword_E4DA8 != 0)
        JumelleFreeArray((void **)&dword_E4DA8);
}

u8 effect_helper_50738(u8 mode, s16 speed, u8 *source, u8 *destination)
{
    effect3d_t *state;
    u8 okay = 0;

    if (source != 0 && destination != 0) {
        dword_E4DA8 = (effect3d_t *)sub_6FBB4(sizeof(effect3d_t));
        if (effect_data() != 0) {
            state = effect_data();
            state->source = source;
            state->destination = destination;
            state->vertices = (effect_segment_t *)sub_6FBB4(0x58);
            if (state->vertices != 0) {
                state->transformed = (effect3d_point_t *)sub_6FBB4(0x78);
                if (state->transformed != 0) {
                    u8 *second_screen = EffetBufferNormal + 0xc800;
                    state->screen_a = EffetBufferNormal;
                    state->screen_b = second_screen;
                    okay = 1;
                }
            }
        }
    }
    if (okay != 0) {
        effect_copy_band(state->source, state->screen_a, 0xa0);
        effect_copy_band(state->source + 0xa0, state->screen_b, 0xa0);
        effect_activate(mode, speed);
    } else {
        effect_cleanup_506fc();
    }
    return okay;
}

u16 effect_helper_507f0(void)
{
    effect3d_t *state = effect_data();
    effect3d_point_t *point = state->transformed;
    /* Neutral coefficient names: EDX receives first_result, EBX second_result.
     * DOS caller dataflow at 1:5083b..508b4 establishes their use below. */
    /* DOS stack outputs: first +0x24, second +0x28 (1:5083b..50894). */
    s16 second_result, first_result;
    s16 extent;
    /* DOS spill slots: cofirst_z +10, first_x +14, cofirst_y +18,
     * first_y +1c, first_z +20; cofirst_x remains in EBP. */
    s32 first_z, cofirst_x, cofirst_z, first_x, cofirst_y, first_y;
    u8 index;

    if (state->field_1c == 0) {
        if (state->speed >= 0) extent = state->position;
        else extent = state->position_limit - state->position;
        extent = (extent << 2) + 0x200;
    } else extent = 0x200;
    sinus_cosinus(state->angle_x, &first_result, &second_result);
    first_x = first_result; cofirst_x = second_result;
    sinus_cosinus(state->angle_y, &first_result, &second_result);
    first_y = first_result; cofirst_y = second_result;
    sinus_cosinus(state->angle_z, &first_result, &second_result);
    first_z = first_result; cofirst_z = second_result;
    for (index = 0; index < 6; ++index, ++point) {
        s32 x = point->x - state->field_18;
        s32 y = point->y - state->field_1a;
        s32 z = point->z;
        s32 temp;
        if (state->angle_z != 0) {
            temp = cofirst_z * x - first_z * y;
            y = (x * first_z + y * cofirst_z) >> 9;
            x = temp >> 9;
        }
        if (state->angle_y != 0) {
            temp = first_y * z + cofirst_y * x;
            z = (z * cofirst_y - x * first_y) >> 9;
            x = temp >> 9;
        }
        if (state->angle_x != 0) {
            temp = first_x * z + y * cofirst_x;
            z = z * cofirst_x - y * first_x;
            y = temp >> 9;
            z >>= 9;
        }
        /* The rotated depth becomes the projection denominator (1:50995). */
        z += extent;
        point->screen_x = state->field_18 + (x << 9) / z;
        point->screen_y = (y << 9) / z + state->field_1a;
    }
    state->angle_x += state->step_x;
    state->angle_y += state->step_y;
    state->angle_z += state->step_z;
    {
        effect3d_point_t *output = state->transformed;
        effect_segment_t *segment = state->vertices;
        segment->points[0] = output[0].screen_x;
        segment->points[1] = output[0].screen_y;
        segment->points[2] = output[1].screen_x;
        segment->points[3] = output[1].screen_y;
        segment->points[4] = output[5].screen_x;
        segment->points[5] = output[5].screen_y;
        segment->points[6] = output[4].screen_x;
        segment->points[7] = output[4].screen_y;
        segment = state->vertices + 1;
        segment->points[0] = output[4].screen_x;
        segment->points[1] = output[4].screen_y;
        segment->points[2] = output[5].screen_x;
        segment->points[3] = output[5].screen_y;
        segment->points[4] = output[2].screen_x;
        segment->points[5] = output[2].screen_y;
        segment->points[6] = output[3].screen_x;
        segment->points[7] = output[3].screen_y;
    }
    {
        s32 step = state->speed;
        if (step < 0) step = -step;
        state->position += step;
    }
    if (state->position > state->position_limit) state->position = 0;
    return state->position;
}

void effect_helper_50aa8(void)
{
    effect3d_t *state = effect_data();
    effect_segment_t *points = state->vertices;
    draw_effect_segment(state->destination, (u8 *)points);
    ++points;
    draw_effect_segment(state->destination, (u8 *)points);
}
