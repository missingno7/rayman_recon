/* Candidate for obj1:34580..34d90. The DOS switch table at 34580 belongs to the
 * first function; these are PC-only screen-wipe helpers (INVENTED names). */
#include "effect_types.h"
#include "inline_helpers.h"

extern effect_rows_t *dword_E0D14;
extern u8 *EffetBufferNormal;
extern void *sub_6FBB4(u32 size);
extern void JumelleFreeArray(void **p);

#define current_rows() (dword_E0D14)

void effect_prepare_rows(u8 mode, s16 direction)
{
    effect_rows_t *state = current_rows();
    effect_span_t *span = state->entries;
    s32 amount;
    u8 i;

    state->position = 0;
    if (direction >= 0) {
        amount = 1;
        amount = amount >= direction ? amount : direction;
    } else {
        amount = 1;
        amount = amount >= -direction ? amount : -direction;
        amount = -amount;
    }
    state->direction = amount;
    amount = 1;
    amount = amount >= mode ? amount : mode;
    amount = amount <= 12 ? amount : 12;
    state->mode = amount;
    for (i = 0; i < state->count; ++i, ++span) {
        span->width = 0;
        span->destination_y = i;
        span->destination_x = span->source_x = span->source_y = span->width;
    }
    switch (state->mode) {
    case 3:
    case 7:
    case 11:
        state->position_limit = 320;
        break;
    case 12:
        state->position_limit = 320;
        state->direction = -state->direction;
        break;
    case 9:
        state->position_limit = 100;
        break;
    case 1: case 2: case 4: case 5: case 6: case 8: case 10:
    default:
        state->position_limit = 200;
        break;
    }
}

void effect_cleanup_34688(void)
{
    effect_rows_t *state = current_rows();
    if (state == 0) return;
    if (state->entries != 0)
        JumelleFreeArray((void **)&state->entries);
    if (dword_E0D14 != 0)
        JumelleFreeArray((void **)&dword_E0D14);
}

u8 effect_helper_346b4(u8 mode, s16 direction, u8 *screen, void *destination)
{
    effect_rows_t *state;
    u8 okay = 0;

    if (screen != 0 && destination != 0) {
        dword_E0D14 = (effect_rows_t *)sub_6FBB4(sizeof(effect_rows_t));
        state = current_rows();
        if (state != 0) {
            state->count = 200;
            copy(EffetBufferNormal, screen, 0xfa00);
            state->screen = EffetBufferNormal;
            state->destination = (u8 *)destination;
            state->entries = (effect_span_t *)sub_6FBB4(state->count * sizeof(effect_span_t));
            if (state->entries != 0)
                okay = 1;
        }
    }
    if (okay != 0)
        effect_prepare_rows(mode, direction);
    else
        effect_cleanup_34688();
    return okay;
}

void effect_helper_34760(void)
{
    effect_span_t *span;
    effect_rows_t *state;
    s32 index;

    state = current_rows();
    index = 0;
    span = state->entries;
    for (; index < state->count; ++index, ++span) {
        s32 width = span->width;
        s32 destination_y = span->destination_y;
        s32 source_y = span->source_y;
        if (width > 0 && source_y >= 0 && source_y < 200 &&
                destination_y >= 0 && destination_y < 200) {
            s32 source_x = span->source_x;
            if (source_x < 0) {
                width += source_x;
                source_x = 0;
            }
            if (source_x + width > 320) width = 320 - source_x;
            if (width > 0) {
                s32 destination_x = span->destination_x;
                if (destination_x < 0) {
                    width += destination_x;
                    destination_x = 0;
                }
                if (destination_x + width > 320) width = 320 - destination_x;
                if (width > 0) {
                    u8 *source = state->screen + source_y * 320 + source_x;
                    u8 *destination = state->destination + destination_y * 320 + destination_x;
                    copy(destination, source, width);
                }
            }
        }
    }
}

u16 effect_helper_348a4(void)
{
    effect_rows_t *state = current_rows();
    effect_span_t *span = state->entries;
    s16 distance;
    s16 i;
    s16 j;

    if (state->direction >= 0) distance = state->position_limit - state->position;
    else distance = state->position;
    switch (state->mode) {
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        {
            s32 source_offset;
            s32 destination_x;
            s32 source_x;
            s32 width;
            if (state->mode == 1 || state->mode == 2 || state->mode == 8)
                source_offset = distance;
            else source_offset = 0;
            if (state->mode == 5 || state->mode == 4 || state->mode == 6)
                source_offset = -distance;
            if (state->mode == 3 || state->mode == 2 || state->mode == 4)
                destination_x = distance;
            else destination_x = 0;
            if (state->mode == 7 || state->mode == 8 || state->mode == 6)
                source_x = distance;
            else source_x = 0;
            width = (state->mode == 1 || state->mode == 5) ? 0 : -distance;
            width += 320;
            span = state->entries;
            for (i = 0; i < 200; ++i, ++span) {
                span->destination_x = destination_x;
                span->destination_y = i;
                span->source_x = source_x;
                span->width = width;
                span->source_y = source_offset + i;
            }
        }
        break;
    case 10:
        {
            s32 even_y = distance;
            s32 odd_y = -distance;
            for (i = 0, j = 1; i < 200; i += 2, j += 2) {
                span[i].destination_x = span[j].destination_x =
                    span[i].source_x = span[j].source_x = 0;
                span[i].destination_y = i;
                span[j].destination_y = j;
                span[i].source_y = even_y;
                span[j].source_y = odd_y;
                span[i].width = span[j].width = 320;
                even_y += 2;
                odd_y += 2;
            }
        }
        break;
    case 11:
        {
            s32 width = 320 - distance;
            for (i = 0, j = 1; i < 200; i += 2, j += 2) {
                span[i].destination_x = span[j].source_x = 0;
                span[i].destination_y = span[i].source_y = i;
                span[j].destination_x = span[i].source_x = distance;
                span[i].width = span[j].width = width;
                span[j].destination_y = span[j].source_y = j;
            }
        }
        break;
    case 9:
        for (i = 0; i < 200; ++i) span[i].width = 0;
        {
            s32 width = 320 - 2 * distance;
            s32 source_y = 199 - distance;
            for (i = 0, j = 199; i < 100 - distance; ++i, --j) {
                span[j].destination_x = span[j].source_x =
                    span[i].source_y = span[i].destination_x = span[i].source_x = i + distance;
                span[i].destination_y = i;
                span[j].destination_y = j;
                span[j].source_y = source_y;
                span[i].width = span[j].width = width;
                width -= 2;
                --source_y;
            }
        }
        effect_helper_34760();
        {
            s32 width = -distance;
            for (i = 0, j = 199; i < 100; ++i, --j) {
                span[i].destination_x = span[j].destination_x = 0;
                span[j].source_x = span[i].source_x = distance;
                span[i].destination_y = span[i].source_y = i;
                span[i].width = span[j].width = width;
                span[j].source_y = span[j].destination_y = j;
                ++width;
            }
        }
        effect_helper_34760();
        {
            s32 destination_x = 320 + distance;
            s32 source_x = 320;
            s32 destination_y = 200;
            for (i = 0, j = 199; i < 100; ++i, --j) {
                span[i].destination_x = span[j].destination_x = destination_x;
                span[i].destination_y = span[i].source_y = i;
                span[j].destination_y = j;
                span[i].source_x = span[j].source_x = source_x;
                span[j].source_y = destination_y;
                span[i].width = span[j].width = i - distance;
                --source_x;
                --destination_y;
                --destination_x;
            }
        }
        break;
    case 12:
        {
            s16 vertical = (200 - distance) >> 1;
            s32 horizontal = (320 - distance) >> 1;
            span = state->entries;
            for (i = 0; i < 200; ++i, ++span) {
                span->source_y = span->destination_y = i;
                span->source_x = span->destination_x = horizontal;
                if (i >= vertical && 200 - vertical >= i) span->width = distance;
                else span->width = 0;
            }
        }
        break;
    default:
        span = state->entries;
        for (i = 0; i < state->count; ++i, ++span) {
            span->destination_x = span->source_x = 0;
            span->destination_y = span->source_y = i;
            span->width = 320;
        }
        break;
    }
    {
        s32 step = (s32)state->direction;
        if (step < 0) step = -step;
        state->position += step;
    }
    if (state->position > state->position_limit) state->position = 0;
    return state->position;
}
