/* DOS 1:6ec80..1:6f200. Semantic effect names are INVENTED.
 * Partial matching candidate; see NOTES.md for evidence and rejected shapes. */
#include "rtypes.h"
#include "inline_helpers.h"

typedef struct effect_vertex_6e_t {
    u16 x;
    u8 y;
    u8 field_3;
    u16 next_x;
    u8 next_y;
    u8 field_7;
    u32 surface;
    s32 projected[8];
    u8 rectangle[8];
} effect_vertex_6e_t;

typedef struct effect_state_6e_t {
    u8 columns;
    u8 rows;
    u16 vertex_count;
    u16 current;
    u16 speed_fraction;
    s16 speed;
    u16 field_a;
    void *source;
    u8 *sprite;
    u8 *surface_a;
    u8 *surface_b;
    effect_vertex_6e_t *vertices;
    u8 history[0x4b];
    u8 ramp[0x4b];
    u16 history_position;
    u8 history_length;
    u8 tail[3];
} effect_state_6e_t;

extern effect_state_6e_t *effect_state_6e;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(s32 size);
extern void JumelleFreeArray(void **array);
extern void effect_reset_geometry(void);
extern void effect_copy_band(u8 *source, u8 *destination, u32 width);
extern s16 sinus(s16 angle);
extern void draw_effect_segment(u8 *sprite, u8 *points);
extern void effect_cleanup_6ee28(void);

/* DOS routine at 1:00010 uses PUSHAD/POPAD and has no register inputs. */
#pragma aux effect_reset_geometry modify exact []
/* DOS routine at 1:00045 reads EAX and ESI and uses PUSHAD/POPAD. */
#pragma aux draw_effect_segment parm [eax] [esi] modify exact []

/* COMPILER_STEERED: max32 speed clamps follow DOS 1:6ece1..6ed03.
 * The fraction is assigned once; surface writes share DOS 1:6ed5e. */
void effect_helper_6ec80(s16 speed, u16 step)
{
    effect_state_6e_t *state;
    effect_vertex_6e_t *vertex;
    u8 step_y;
    u8 step_x;
    u8 row;
    u8 column;
    u8 angle;
    u8 index;
    u8 y;
    u16 x;
    u32 surface;

    state = effect_state_6e;
    step_x = (u8)(320 / state->columns);
    step_y = (u8)(200 / state->rows);
    vertex = state->vertices;
    effect_reset_geometry();
    state->current = 0;
    state->speed_fraction = step > 1 ? step : 1;
    state->speed = speed >= 0 ?
        max32(1, speed) : -max32(1, -speed);

    row = 0;
    do {
        column = 0;
        do {
            x = step_x * column;
            y = row * step_y;
            vertex->x = vertex->next_x = x;
            vertex->y = vertex->next_y = y;
            if (x < 160) {
                surface = (u32)state->surface_a;
            } else {
                surface = (u32)state->surface_b;
                x -= 160;
            }
            vertex->surface = surface;
            vertex->rectangle[0] = vertex->rectangle[2] = (u8)x;
            vertex->rectangle[4] = vertex->rectangle[6] = (u8)(x + step_x - 1);
            vertex->rectangle[1] = vertex->rectangle[7] = y;
            vertex->rectangle[3] = vertex->rectangle[5] = (u8)(y + step_y - 1);
            ++vertex;
            ++column;
        } while (column <= state->columns);
        ++row;
    } while (row <= state->rows);

    index = 0;
    angle = 0;
    do {
        state->history[index] = 0;
        state->ramp[index] = (u8)(s32)(((double)sinus((s16)(angle * 2)) * -40.0) * (1.0 / 512.0));
        angle = (u8)(angle + 10);
        ++index;
    } while (index < 0x4b);
    state->history_position = 0;
    state->history_length = 0x4b;
}

void effect_cleanup_6ee28(void)
{
    effect_state_6e_t *state;
    state = effect_state_6e;
    if (state != 0) {
        if (state->vertices != 0)
            JumelleFreeArray((void **)&state->vertices);
        if (effect_state_6e != 0)
            JumelleFreeArray((void **)&effect_state_6e);
    }
}

u8 effect_helper_6ee54(u16 columns, u16 rows, s16 speed, u16 step,
                       void *source, u8 *sprite)
{
    effect_state_6e_t *state;
    effect_state_6e_t *allocated;
    u8 success;

    success = 0;
    if (source != 0 && sprite != 0) {
        allocated = (effect_state_6e_t *)sub_6FBB4(0xbc);
        effect_state_6e = allocated;
        if (allocated != 0) {
            state = allocated;
            allocated->columns = (u8)max32(2, columns);
            allocated->rows = (u8)max32(1, rows);
            allocated->vertex_count = (columns + 1) * (rows + 1);
            allocated->source = source;
            allocated->sprite = sprite;
            allocated->vertices = (effect_vertex_6e_t *)sub_6FBB4(allocated->vertex_count * sizeof(effect_vertex_6e_t));
            if (allocated->vertices != 0) {
                allocated->surface_a = EffetBufferNormal;
                allocated->surface_b = EffetBufferNormal + 0xc800;
                success = 1;
            }
        }
    }
    if (success != 0) {
        effect_copy_band((u8 *)state->source, state->surface_a, 0xa0);
        effect_copy_band((u8 *)state->source + 0xa0, state->surface_b, 0xa0);
        effect_helper_6ec80(speed, step);
    } else {
        effect_cleanup_6ee28();
    }
    return success;
}

#define EFFECT_ABS(value) ((value) < 0 ? -(value) : (value))

s16 effect_helper_6ef5c(void)
{
    effect_state_6e_t *state;
    u8 column;
    u8 iteration;
    u8 row;
    u16 current;
    u8 wave;
    u8 i;

    state = effect_state_6e;
    for (iteration = 0; iteration < EFFECT_ABS((s32)state->speed); ++iteration) {
        state->history_position += 2;
        for (row = 1; row < state->rows; ++row) {
            u16 vertex_index;
            effect_vertex_6e_t *vertex;
            vertex_index = row * (state->columns + 1) + 1;
            vertex = vertex_index + state->vertices;
            for (column = 1; column < state->columns; ++column) {
                s32 dx;
                s32 dy;
                s32 radius_index;
                s32 divisor;
                dx = (s32)vertex->x - 160;
                dy = (s32)vertex->y - 100;
                radius_index = (dx * dx + dy * dy) >> 9;
                divisor = (s32)((s8 *)state->history)[radius_index] + 512;
                dx = (dx * 512) / divisor + 160;
                dy = (dy * 512) / divisor + 100;
                vertex->next_y = dy;
                vertex->next_x = dx;
                ++vertex;
            }
        }
        if (state->history_length != 0) {
            wave = --state->history_length;
            wave = state->ramp[wave];
        } else {
            wave = state->history[0x4a];
        }
        if (state->history_position > 0x258 && state->history[0] == 0)
            wave = 0;
        for (i = 0x4a; i > 0; --i)
            state->history[i] = state->history[i - 1];
        state->history[0] = wave;
    }
    current = state->current + EFFECT_ABS((s32)state->speed);
    state->current = current;
    if (current > state->speed_fraction)
        state->current = 0;
    return state->current;
}

void effect_helper_6f0e0(void)
{
    effect_state_6e_t *state;
    effect_vertex_6e_t *vertices;
    effect_vertex_6e_t *vertex;
    u16 index;
    u8 row;
    u8 column;

    state = effect_state_6e;
    index = 0;
    row = 0;
    vertex = state->vertices;
    vertices = vertex;
    for (; row < state->rows; ++row) {
        for (column = 0; column < state->columns; ++column) {
            vertex->projected[0] = vertices[index].next_x;
            vertex->projected[1] = vertices[index].next_y;
            vertex->projected[2] = vertices[index + state->columns + 1].next_x;
            vertex->projected[3] = vertices[index + state->columns + 1].next_y;
            vertex->projected[4] = vertices[index + state->columns + 2].next_x - 1;
            vertex->projected[5] = vertices[index + state->columns + 2].next_y;
            vertex->projected[6] = vertices[index + 1].next_x - 1;
            vertex->projected[7] = vertices[index + 1].next_y;
            draw_effect_segment(state->sprite, (u8 *)&vertex->surface);
            ++index;
            ++vertex;
        }
        ++index;
        ++vertex;
    }
}
