/* Candidate for obj1:50350..50ad0. The switch table begins the _TEXT extent. */
#include "effect_types.h"
#include <string.h>

extern u32 dword_E4DA8;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(u32 size);
extern void JumelleFreeArray(void **p);
extern void effect_reset_geometry(void);
#pragma aux effect_reset_geometry modify exact []
extern void effect_copy_band(u8 *source, u8 *destination, u32 width);
extern void sinus_cosinus(s16 angle, s16 *sinus_result, s16 *cosinus_result);
extern void draw_effect_segment(u8 *destination, u8 *points);
#pragma aux draw_effect_segment parm [eax] [esi] modify exact []

#define effect_data() ((effect3d_t *)dword_E4DA8)

void effect_activate(s32 mode_arg, s16 speed)
{
    effect3d_t *state = effect_data();
    u8 mode = (u8)mode_arg;
    s32 amount = speed;
    s32 magnitude;
    u8 *vertex_bytes;
    u8 *matrix_bytes;

    effect_reset_geometry();
    if (amount < 0) magnitude = -amount; else magnitude = amount;
    if (magnitude < 1) magnitude = 1;
    if (magnitude > 15) magnitude = 15;
    state->speed = (s16)(speed < 0 ? -magnitude : magnitude);
    state->mode = mode < 1 ? 1 : (mode > 15 ? 15 : mode);

    vertex_bytes = (u8 *)state->vertices;
    matrix_bytes = (u8 *)state->transformed;
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
    *(u32 *)(matrix_bytes + 0x58) = 0;
    *(u32 *)(matrix_bytes + 0x44) = 0;
    *(u32 *)(matrix_bytes + 0x30) = 0;
    *(u32 *)(matrix_bytes + 0x1c) = 0;
    *(u32 *)(matrix_bytes + 0x08) = 0;

    vertex_bytes[0x26] = 0;
    vertex_bytes[0x2a] = 0x9f;
    vertex_bytes[0x2b] = 0;
    vertex_bytes[0x29] = 0xc7;
    vertex_bytes[0x24] = vertex_bytes[0x26];
    vertex_bytes[0x28] = vertex_bytes[0x2a];
    vertex_bytes[0x25] = vertex_bytes[0x2b];
    vertex_bytes[0x27] = vertex_bytes[0x29];
    *(u32 *)(vertex_bytes + 0) = (u32)state->screen_a;
    vertex_bytes[0x2c + 0x26] = 0;
    *(u32 *)(vertex_bytes + 0x2c) = (u32)state->screen_b;
    vertex_bytes[0x2c + 0x2a] = 0x9f;
    vertex_bytes[0x2c + 0x2b] = 0;
    vertex_bytes[0x2c + 0x29] = 0xc7;
    vertex_bytes[0x2c + 0x24] = vertex_bytes[0x2c + 0x26];
    vertex_bytes[0x2c + 0x28] = vertex_bytes[0x2c + 0x2a];
    vertex_bytes[0x2c + 0x25] = vertex_bytes[0x2c + 0x2b];
    vertex_bytes[0x2c + 0x27] = vertex_bytes[0x2c + 0x29];

    state->position = 0;
    state->position_limit = 0x200;
    state->angle_z = 0;
    state->field_10 = 0;
    state->step_z = 0;
    state->field_18 = 0xa0;
    state->angle_x = state->angle_z;
    state->angle_y = state->angle_z;
    state->field_0e = state->field_10;
    state->field_0c = state->field_10;
    state->step_y = state->step_z;
    state->step_x = state->step_z;
    state->field_1a = 100;
    state->field_1c = 0;
    state->step_x = (s16)(speed < 0 ? -magnitude : magnitude);

    switch (state->mode) {
    case 1: state->step_x = (s16)magnitude; break;
    case 2: state->step_y = (s16)(2 * magnitude); break;
    case 3: state->step_z = (s16)magnitude; break;
    case 4: state->step_y = (s16)magnitude; state->step_x = (s16)(2 * magnitude); break;
    case 5: state->step_z = (s16)magnitude; state->step_y = (s16)(2 * magnitude); break;
    case 6: state->step_z = (s16)magnitude; state->step_x = (s16)(2 * magnitude); break;
    case 7: state->step_z = (s16)magnitude; state->step_y = (s16)(3 * magnitude); state->step_x = (s16)(2 * magnitude); break;
    case 8:
        state->field_0c = 0x200; state->field_1a = 201; state->field_1c = 1;
        state->step_x = (s16)-speed; if (speed <= 0) state->field_0c = 0x180; break;
    case 9:
        state->field_1a = 201; state->field_1c = 1; state->step_x = speed;
        if (speed <= 0) state->field_0c = 0x80; break;
    case 10:
        state->field_1a = (s16)-2; state->field_1c = 1; state->step_x = speed;
        if (speed <= 0) state->field_0c = 0x80; break;
    case 11:
        state->field_0c = 0x200; state->field_1a = (s16)-2; state->field_1c = 1;
        state->step_x = (s16)-speed; if (speed <= 0) state->field_0c = 0x180; break;
    case 12:
        state->field_18 = (s16)-2; state->field_1c = 1; state->step_y = speed;
        if (speed <= 0) state->angle_y = 0x80; break;
    case 13:
        state->angle_y = 0x200; state->field_18 = (s16)-2; state->field_1c = 1;
        state->step_y = (s16)-speed; if (speed <= 0) state->angle_y = 0x180; break;
    case 14:
        state->angle_y = 0x200; state->field_18 = 0x141; state->field_1c = 1;
        state->step_y = (s16)-speed; if (speed <= 0) state->angle_y = 0x180; break;
    case 15:
        state->field_18 = 0x141; state->field_1c = 1; state->step_y = speed;
        if (speed <= 0) state->angle_y = 0x80; break;
    default: break;
    }
    if (state->field_1c == 1)
        state->position_limit += 0x80;
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
        state = (effect3d_t *)sub_6FBB4(sizeof(effect3d_t));
        dword_E4DA8 = (u32)state;
        if (state != 0) {
            state->source = source;
            state->destination = destination;
            state->vertices = (u32 *)sub_6FBB4(0x58);
            if (state->vertices != 0) {
                state->transformed = (u32 *)sub_6FBB4(0x78);
                if (state->transformed != 0) {
                    state->screen_a = EffetBufferNormal;
                    state->screen_b = EffetBufferNormal + 0xc800;
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

s16 effect_helper_507f0(void)
{
    effect3d_t *state = effect_data();
    effect3d_point_t *output = (effect3d_point_t *)state->transformed;
    s32 sine_x, cosine_x, sine_y, cosine_y, sine_z, cosine_z;
    s32 cx, cy, cz;
    s32 index;
    s32 extent;

    if (state->field_1c == 0) {
        extent = state->speed >= 0 ? state->position : state->position_limit - state->position;
        extent = (extent << 2) + 0x200;
    } else {
        extent = 0x200;
    }
    sinus_cosinus(state->angle_x, (s16 *)&sine_x, (s16 *)&cosine_x);
    sinus_cosinus(state->angle_y, (s16 *)&sine_y, (s16 *)&cosine_y);
    sinus_cosinus(state->angle_z, (s16 *)&sine_z, (s16 *)&cosine_z);
    cx = (s32)state->field_18;
    cy = (s32)state->field_18;
    cz = (s32)state->field_1a;

    for (index = 0; index < 6; ++index) {
        s32 x = (s32)state->vertices[index * 3] - state->field_18;
        s32 y = (s32)state->vertices[index * 3 + 1] - state->field_18;
        s32 z = (s32)state->vertices[index * 3 + 2] - state->field_1a;
        s32 temp;
        s32 divisor;

        temp = (x * cosine_x - y * sine_x) >> 9;
        y = y * cosine_x + x * sine_x;
        x = temp;
        temp = z * sine_y + x * cosine_y;
        z = (z * cosine_y - x * sine_y) >> 9;
        x = temp;
        y >>= 9;
        temp = z * sine_z + y * cosine_z;
        z = (z * cosine_z - y * sine_z) >> 9;
        y = temp;
        divisor = z + cz;
        if (divisor == 0) divisor = 1;
        output[index].screen_x = cx + (x * 0x200) / divisor;
        output[index].screen_y = cy + (y * 0x200) / divisor;
    }

    state->angle_x += state->step_x;
    state->angle_y += state->step_y;
    state->angle_z += state->step_z;
    if (state->vertices != 0 && output != 0) {
        u32 *v = state->vertices;
        v[1] = (u32)output[0].screen_x;
        v[2] = (u32)output[0].screen_y;
        v[3] = (u32)output[1].screen_x;
        v[4] = (u32)output[1].screen_y;
        v[8] = (u32)output[2].screen_x;
        v[9] = (u32)output[2].screen_y;
        v[10] = (u32)output[3].screen_x;
        v[11] = (u32)output[3].screen_y;
        v[14] = (u32)output[4].screen_x;
        v[15] = (u32)output[4].screen_y;
        v[16] = (u32)output[5].screen_x;
        v[17] = (u32)output[5].screen_y;
    }
    if (state->speed < 0) {
        s32 step = -state->speed;
        state->position = (u16)(state->position + step);
        if (state->position > state->position_limit)
            state->position = 0;
    } else {
        state->position = (u16)(state->position + state->speed);
        if (state->position > state->position_limit)
            state->position = 0;
    }
    return (s16)state->position;
}

void effect_helper_50aa8(void)
{
    effect3d_t *state = effect_data();
    draw_effect_segment(state->destination, (u8 *)state->vertices);
    draw_effect_segment(state->destination, (u8 *)state->vertices + 0x2c);
}
