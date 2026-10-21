/* effect.c part 2 (1:28a30..1:29210; sub_* = Rayverse placeholder names, PC-only). Worker tu_effect; all functions written, none STRICT yet. */
#include "obj.h"
#include "game.h"
#include "rtypes.h"

typedef struct effect_item {
    s32 x0;
    s32 y0;
    s32 width0;
    s32 x1;
    s32 y1;
    s32 width1;
    s32 x2;
    s32 y2;
    s32 width2;
    s32 x3;
    s32 y3;
    s32 width3;
    s16 random_x;
    s16 random_y;
    s16 sort_key;
    u8 color_r;
    u8 color_g;
    u8 color_b;
    u8 color_step_r;
    u8 color_step_g;
    u8 color_step_b;
    u8 *screen;
    s32 vertices[8];
    u8 rectangle[8];
    s16 draw_order;
    u8 reserved_6a[2];
} effect_item_t;

typedef struct effect_state {
    u8 columns;
    u8 rows;
    u16 count;
    u16 frame;
    u16 frame_limit;
    s16 speed;
    u8 reserved_0a[2];
    u8 *source;
    u8 *destination;
    u8 *screen_a;
    u8 *screen_b;
    effect_item_t *items;
} effect_state_t;

extern effect_state_t *effect_data;
extern void *sub_6FBB4(u32 size);
extern void JumelleFreeArray(void **p);
extern void draw_effect_segment(u8 *destination, effect_item_t *item, u8 *points);
#pragma aux draw_effect_segment parm [eax] [edx] [esi] modify []
extern void effect_reset_geometry(void);
#pragma aux effect_reset_geometry modify []
extern void effect_copy_band(u8 *source, u8 *destination, u32 width);
extern void sinus_cosinus(s16 angle, s16 *sinus_result, s16 *cosinus_result);
extern u8 *EffetBufferNormal;
extern void sub_38B58(s16 horizontal_offset, u16 vertical_speed);
extern void sub_38A30(void);

void sub_38A30(void)
{
    effect_state_t *state = effect_data;
    effect_item_t *items;
    s16 *order;
    u32 allocation_size;
    s32 i;

    allocation_size = (u32)state->count * 2;
    items = state->items;
    order = (s16 *)sub_6FBB4(allocation_size);
    if (order == 0)
        return;

    for (i = 0; i < state->count; ++i)
        order[i] = items[i].sort_key;

    for (i = 0; i < state->count; ++i) {
        s32 j;
        s32 largest = i;
        s16 largest_key = (s16)0x8000;

        for (j = i; j < state->count; ++j) {
            if (largest_key < order[j]) {
                largest_key = order[j];
                largest = j;
            }
        }
        if (largest != i) {
            s16 tmp;
            tmp = items[i].draw_order;
            items[i].draw_order = items[largest].draw_order;
            items[largest].draw_order = tmp;
            tmp = order[i];
            order[i] = order[largest];
            order[largest] = tmp;
        }
    }
    JumelleFreeArray((void **)&order);
}

void sub_38B58(s16 horizontal_offset, u16 vertical_speed)
{
    effect_state_t *state = effect_data;
    effect_item_t *item = state->items;
    u8 x_step = (u8)(320 / state->columns);
    u8 y_step = (u8)(200 / state->rows);
    u16 i;

    state->frame = 0;
    state->frame_limit = vertical_speed < 1 ? 1 : vertical_speed;
    state->speed = horizontal_offset < 0 ?
                   (horizontal_offset > -1 ? -1 : horizontal_offset) :
                   (horizontal_offset < 1 ? 1 : horizontal_offset);
    effect_reset_geometry();

    for (i = 0; i < state->count; ++i) {
        s32 x = (i % state->columns) * x_step;
        s32 y = (i / state->columns) * y_step;
        s32 x_end = x + x_step - 1;
        s32 y_end = y + y_step - 1;

        item->x0 = x;
        item->x1 = x;
        item->x2 = x_end;
        item->x3 = x_end;
        item->y0 = y;
        item->y1 = y_end;
        item->y2 = y;
        item->y3 = y_end;
        item->width0 = item->width1 = item->width2 = item->width3 = 0x200;
        if (x < 160) {
            item->screen = state->screen_a;
            item->rectangle[0] = (u8)x;
            item->rectangle[2] = (u8)x;
            item->rectangle[4] = (u8)(x + x_step - 1);
            item->rectangle[6] = (u8)(x + x_step - 1);
        } else {
            item->screen = state->screen_b;
            item->rectangle[0] = (u8)(x - 160);
            item->rectangle[2] = (u8)(x - 160);
            item->rectangle[4] = (u8)(x - 160 + x_step - 1);
            item->rectangle[6] = (u8)(x - 160 + x_step - 1);
        }
        item->rectangle[1] = (u8)y;
        item->rectangle[3] = (u8)(y + y_step - 1);
        item->rectangle[5] = (u8)y;
        item->rectangle[7] = (u8)(y + y_step - 1);
        if (i % 10 < 5)
            item->random_x = (s16)(rand() % 3);
        else
            item->random_x = (s16)(rand() % 3 - 2);
        item->random_y = (s16)(rand() % 8 - 6);
        item->sort_key = (s16)(rand() % 5 - 2);
        item->color_step_r = (u8)(rand() % 5 + 2);
        item->color_step_g = (u8)(rand() % 5 + 2);
        item->color_step_b = (u8)(rand() % 5 + 2);
        item->color_r = (u8)(item->color_step_r * 5);
        item->color_g = (u8)(item->color_step_g * 5);
        item->color_b = (u8)(item->color_step_b * 5);
        item->draw_order = (s16)i;
        ++item;
    }
    sub_38A30();
}

void sub_38DA0(void)
{
    if (effect_data != 0 && effect_data->items != 0)
        JumelleFreeArray((void **)&effect_data->items);
    if (effect_data != 0)
        JumelleFreeArray((void **)&effect_data);
}

u8 sub_38DCC(s32 columns, s32 rows, s32 horizontal_offset, s32 vertical_speed,
             u8 *source, u8 *destination)
{
    effect_state_t *state;
    u32 count;

    if (source == 0 || destination == 0)
        return 0;
    state = (effect_state_t *)sub_6FBB4(0x20);
    effect_data = state;
    if (state == 0)
        return 0;
    state->columns = (u8)((u16)columns < 2 ? 2 : (u16)columns);
    state->rows = (u8)((u16)rows < 1 ? 1 : (u16)rows);
    state->count = (u16)(state->columns * state->rows);
    state->source = source;
    state->destination = destination;
    count = (u32)state->count * 0x6c;
    state->items = (effect_item_t *)sub_6FBB4(count);
    if (state->items == 0) {
        sub_38DA0();
        return 0;
    }
    state->screen_a = EffetBufferNormal;
    state->screen_b = EffetBufferNormal + 0xc800;
    effect_copy_band(source, state->screen_a, 0xa0);
    effect_copy_band(source + 0xa0, state->screen_b, 0xa0);
    sub_38B58((s16)(columns >> 16), (u16)vertical_speed);
    return 1;
}

s16 sub_38ED4(void)
{
    effect_state_t *state = effect_data;
    effect_item_t *item = state->items;
    s32 item_index;
    s32 amount = state->speed;
    u8 step;

    if (amount < 0)
        amount = -amount;
    step = (u8)amount;

    for (item_index = 0; item_index < state->count; ++item_index) {
        s32 *coord = (s32 *)&item->x0;
        s32 center_x = (coord[0] + coord[6]) >> 1;
        s32 center_y = (coord[1] + coord[7]) >> 1;
        s32 center_z = (coord[2] + coord[8]) >> 1;
        u8 vertex;

        for (vertex = 0; vertex < step; ++vertex) {
            coord[vertex * 3] += item->random_x;
            coord[vertex * 3 + 1] += item->random_y;
            coord[vertex * 3 + 2] += item->sort_key;
            if (coord[vertex * 3 + 2] < 0x32)
                item->sort_key = 0;
        }

        for (vertex = 0; vertex < step; ++vertex) {
            s32 x = coord[vertex * 3] - center_x;
            s32 y = coord[vertex * 3 + 1] - center_y;
            s32 z = coord[vertex * 3 + 2] - center_z;
            s32 temp;
            s32 divisor;
            s16 sinus_value;
            s16 cosinus_value;

            sinus_cosinus((s16)(item->color_r * 2),
                          &sinus_value, &cosinus_value);
            temp = (x * cosinus_value - y * sinus_value) >> 9;
            y = (y * cosinus_value + x * sinus_value) >> 9;
            x = temp;

            sinus_cosinus((s16)(item->color_g * 2),
                          &sinus_value, &cosinus_value);
            temp = (x * cosinus_value + z * sinus_value) >> 9;
            z = (z * cosinus_value - x * sinus_value) >> 9;
            x = temp;

            sinus_cosinus((s16)(item->color_b * 2),
                          &sinus_value, &cosinus_value);
            temp = (y * cosinus_value + z * sinus_value) >> 9;
            z = (z * cosinus_value - y * sinus_value) >> 9;
            y = temp;
            divisor = z + center_z;
            item->vertices[vertex * 2] = center_x + x / divisor;
            item->vertices[vertex * 2 + 1] = center_y + y / divisor;
        }

        item->color_r = (u8)(item->color_r + (u8)(step * item->color_step_r));
        item->color_g = (u8)(item->color_g + (u8)(step * item->color_step_g));
        item->color_b = (u8)(item->color_b + (u8)(step * item->color_step_b));
        ++item;
    }

    if (state->frame > 20 && state->frame % 5 == 0) {
        item = state->items;
        for (item_index = 0; item_index < state->count; ++item_index) {
            item->random_y += (s16)step;
            ++item;
        }
    }

    state->frame = (u16)(state->frame + step);
    if (state->frame > state->frame_limit)
        state->frame = 0;
    return (s16)state->frame;
}

void sub_391C0(void)
{
    register effect_state_t *state = effect_data;
    register effect_item_t *items = state->items;
    register effect_item_t *item = items;
    register s32 i;

    for (i = 0; i < state->count; ++i) {
        u16 order = item->draw_order;
        item += 1;
        draw_effect_segment(state->destination, item,
                            (u8 *)(items + order) + 0x3c);
    }
}

