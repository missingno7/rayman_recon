#include "inline_helpers.h"
/* DOS v1.21 1:633e0..1:63b80, partial worker candidate.
 * Public/state/field names are INVENTED and retain the canonical bindings.
 * DOS evidence and rejected hypotheses: NOTES.md. No shared headers changed.
 */
#include <string.h>
#include <math.h>
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#include "rtypes.h"

typedef struct effect_state {
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
} effect_state_t;

extern effect_state_t *effect_state_55760;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(s32 size);
extern void JumelleFreeArray(void **array);
/* 1:75b4a implements acos; retain the existing INVENTED binding name. */
extern double effect_math_curve(double value);

void effect_update_table_633e0(s16 amount)
{
    effect_state_t *state;
    s32 magnitude;
    s16 y;

    state = effect_state_55760;
    state->pos_x = 0;
    if (amount >= 0) {
        s32 requested = amount;
        magnitude = 1;
        if (requested > magnitude)
            magnitude = requested;
        magnitude = -magnitude;
    } else {
        s32 requested = -(s32)amount;
        magnitude = 1;
        if (requested > magnitude)
            magnitude = requested;
    }
    state->delta = magnitude;
    state->mode = state->delta > 0 ? 0 : 3;
    state->initialized = 0;

    for (y = -state->width; y < state->width; ++y) {
        s32 width = state->width;
        double scale = (double)width;
        state->table[width + y] = (s32)(effect_math_curve((double)y / scale) * scale);
    }
}

void effect_cleanup_634c4(void)
{
    effect_state_t *state = effect_state_55760;
    if (state != 0) {
        if (state->table != 0)
            JumelleFreeArray((void **)&state->table);
        if (effect_state_55760 != 0)
            JumelleFreeArray((void **)&effect_state_55760);
    }
}

u8 effect_helper_634f0(u16 width, s16 amount, u8 *source, u8 *destination)
{
    effect_state_t *state;
    u8 success;

    success = 0;
    if (source != 0 && destination != 0) {
        state = (effect_state_t *)sub_6FBB4(0x30);
        effect_state_55760 = state;
        if (state != 0) {
            state->width = max32(8, min32(width, 40));
            state->span = state->width * 2;
            state->period1 = (u16)((double)state->width * 3.14159265359);
            state->period2 = (u16)(3.14159265359 * (double)state->width * 0.5);
            copy(source, EffetBufferNormal, 64000);
            state->source = EffetBufferNormal;
            state->destination = destination;
            state->table = (s32 *)sub_6FBB4((u32)state->span * 4);
            if (state->table != 0)
                success = 1;
        }
    }

    if (success != 0)
        effect_update_table_633e0(amount);
    else
        effect_cleanup_634c4();
    return success;
}

s16 effect_helper_63614(void)
{
    effect_state_t *state;
    double difference;

    state = effect_state_55760;
    switch (state->mode) {
    case 0:
        if (state->initialized == 0) {
            if (state->delta > 0) {
                state->field_04 = 319;
                state->field_14 = state->field_04;
                state->field_08 = 319 - state->period2;
            } else {
                state->field_08 = state->field_14;
                state->field_04 = 319;
            }
            state->initialized = 1;
        }
        state->field_0c = (s32)(sin((double)(state->field_04 - state->field_14) / (double)state->width) * (double)state->width);
        state->field_10 = state->field_14 - state->period2;
        break;
    case 1:
        if (state->initialized == 0) {
            if (state->delta > 0) {
                state->field_04 = state->field_14;
                state->field_08 = state->width;
            } else {
                state->field_08 = state->field_14;
                state->field_04 = 319 - state->period2;
            }
            state->initialized = 1;
        }
        difference = (double)(state->field_04 - state->field_14);
        if (difference <= (double)state->period1) {
            state->field_0c = (s32)(cos(difference / (double)state->width) * (double)state->width);
        } else {
            state->field_0c = -(s32)state->width;
        }
        state->field_10 = 319 - (state->field_04 - state->field_14);
        break;
    case 2:
        if (state->initialized == 0) {
            if (state->delta > 0) {
                state->field_04 = state->field_14;
                state->field_08 = -(s32)state->width;
            } else {
                state->field_04 = (s32)state->width - 1;
                state->field_14 = -(s32)state->width;
                state->field_08 = state->field_14;
            }
            state->initialized = 1;
        }
        state->field_0c = -state->field_14;
        state->field_10 = state->field_14 + state->period2;
        break;
    }

    {
        s32 step = state->delta;
        if (step < 0)
            step = -step;
        state->pos_x = step + state->pos_x;
    }
    if (state->delta > 0) {
        if (state->mode == 4)
            state->pos_x = 0;
    } else if ((s8)state->mode == -1) {
        state->pos_x = 0;
    }
    return state->pos_x;
}

void effect_helper_63838(void)
{
    effect_state_t *state;
    u8 *source_row;
    u8 *destination_row;
    s32 x;
    s32 source_x;

    state = effect_state_55760;
    if (state->mode == 3) {
        u32 *clear_pos = (u32 *)state->destination;
        u32 count = 16000;
        for (; count; --count)
            *clear_pos++ = 0;
        {
            u8 next_mode;
            if (state->delta > 0)
                next_mode = state->mode + 1;
            else
                next_mode = state->mode - 1;
            state->initialized = 0;
            state->mode = next_mode;
        }
    } else {
        source_row = state->source;
        destination_row = state->destination;
        switch (state->mode) {
        case 0:
        {
            s32 y;
            if (state->delta > 0 && state->initialized == 1) {
                memcpy(destination_row, source_row, 64000);
                state->initialized = 2;
            }
            for (y = 0; y < 200; ++y) {
                for (x = 0; x < state->field_0c; ++x) {
                    source_x = state->field_10 + state->table[state->width - x];
                    destination_row[state->field_14 + x] = source_row[source_x];
                }
                if (state->delta > 0) {
                    for (x = state->field_0c; x < ABS((s32)state->delta) + state->field_0c; ++x) {
                        if (state->field_14 + x < 320)
                            destination_row[state->field_14 + x] = 0;
                    }
                } else {
                    for (x = -1; x < ABS((s32)state->delta) - 1; ++x)
                        destination_row[state->field_14 - x] = source_row[state->field_14 - x];
                }
                source_row += 320;
                destination_row += 320;
            }
            break;
        }
        case 1:
        {
            s32 y;
            for (y = 0; y < 200; ++y) {
                for (x = state->field_0c; x < state->width; ++x) {
                    source_x = MIN(319, state->field_10 + state->table[state->width + x]);
                    if (source_row[source_x] == 0) {
                        if (x <= 0)
                            source_x = state->field_14 + x;
                        else
                            source_x = state->field_10 - state->table[state->width + x] - 1;
                    }
                    destination_row[state->field_14 + x] = source_row[source_x];
                }
                if (state->delta > 0) {
                    for (x = state->width; x < ABS((s32)state->delta) + state->width; ++x)
                        destination_row[state->field_14 + x] = 0;
                } else if (state->width < state->field_14) {
                    for (x = -state->width - ABS((s32)state->delta); x < state->field_0c; ++x)
                        destination_row[state->field_14 + x] = source_row[state->field_14 + x];
                }
                source_row += 320;
                destination_row += 320;
            }
            break;
        }
        case 2:
        {
            s32 y;
            for (y = 0; y < 200; ++y) {
                for (x = state->field_0c; x < state->width; ++x) {
                    s32 destination_x;
                    source_x = state->field_10 + state->table[state->width + x];
                    destination_x = state->field_14 + x;
                    if (source_row[source_x] == 0) {
                        if (x < 0)
                            source_x = state->field_14 + x;
                        else
                            source_x = state->field_10 - state->table[state->width + x];
                    }
                    destination_row[destination_x] = source_row[source_x];
                }
                if (state->delta > 0) {
                    for (x = state->width; x < ABS((s32)state->delta) + state->width; ++x)
                        destination_row[state->field_14 + x] = 0;
                }
                source_row += 320;
                destination_row += 320;
            }
            break;
        }
        }
        state->field_14 -= state->delta;
        if (state->delta > 0) {
            if (state->field_14 < state->field_08) {
                state->mode = state->mode + 1;
                state->initialized = 0;
            }
        } else if (state->field_14 > state->field_04) {
            state->mode = state->mode - 1;
            state->initialized = 0;
        }
    }
}
