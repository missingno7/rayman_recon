/* effect.c part 2 (1:28a30..1:29210; sub_* = Rayverse placeholder names, PC-only). Worker g_effect2; partial candidate, see NOTES.md. */
#include "obj.h"
#include "game.h"
#include "rtypes.h"
#include "inline_helpers.h"

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
    u16 draw_order;
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
/* 1:00045 pushad/popad; input EAX destination, ESI projected item. */
extern void draw_effect_segment(u8 *destination, u8 *points);
#pragma aux draw_effect_segment parm [eax] [esi] modify exact []
/* DOS 1:28b8d..28b94: y_step is stored before this call.
 * Keep the default call contract: PUSHAD/POPAD does not prove that the
 * original C declaration advertised EAX preservation. */
extern void effect_reset_geometry(void);
extern void effect_copy_band(u8 *source, u8 *destination, u32 width);
extern void sinus_cosinus(s16 angle, s16 *sinus_result, s16 *cosinus_result);
extern u8 *EffetBufferNormal;
extern void sub_38B58(s16 horizontal_offset, u16 vertical_speed);
void sub_38A30(void)
{
    effect_state_t *state;
    effect_item_t *items;
    s16 *order;
    s16 current_key;
    s16 largest_key;
    s32 tmp;
    u16 i;
    u16 j;
    u16 largest;
    u32 allocation_size;

    state = effect_data;
    allocation_size = state->count * 2;
    items = state->items;
    order = (s16 *)sub_6FBB4(allocation_size);

    if (order != 0) {
        for (i = 0; i < state->count; ++i)
            order[i] = items[i].sort_key;
        for (i = 0; i < state->count; ++i) {
            largest_key = (s16)0x8000;
            for (j = i; j < state->count; ++j) {
                current_key = order[j];
                if (largest_key < current_key) {
                    largest_key = current_key;
                    largest = j;
                }
            }
            if (largest != i) {
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
}

void sub_38B58(s16 horizontal_offset, u16 vertical_speed)
{
    effect_state_t *state = effect_data;
    effect_item_t *item;
    u8 y_step;
    u8 x_step;
    u16 i;
    s16 speed;

    item = state->items;
    x_step = (u8)(320 / state->columns);
    y_step = (u8)(200 / state->rows);
    effect_reset_geometry();
    state->frame = 0;
    state->frame_limit = max32(1, vertical_speed);
    if (horizontal_offset >= 0)
        speed = max32(1, horizontal_offset);
    else
        speed = -max32(1, -horizontal_offset);
    state->speed = speed;

    for (i = 0; i < state->count; ++i) {
        u16 y;
        u16 x;

        x = (i % state->columns) * x_step;

        item->x0 = item->x1 = x;
        item->x2 = item->x3 = x + x_step - 1;
        y = (i / state->columns) * y_step;
        /* DOS 1:28c44..28c67: top corners 0/3, bottom corners 1/2. */
        item->y0 = item->y3 = y;
        item->y1 = item->y2 = y + y_step - 1;
        item->width0 = item->width1 = item->width2 = item->width3 = 0x200;
        if (x < 160) {
            item->screen = state->screen_a;
        } else {
            item->screen = state->screen_b;
            x -= 160;
        }
        item->rectangle[0] = item->rectangle[2] = (u8)x;
        item->rectangle[4] = item->rectangle[6] = (u8)(x + x_step - 1);
        item->rectangle[1] = item->rectangle[7] = (u8)y;
        item->rectangle[3] = item->rectangle[5] = (u8)(y + y_step - 1);
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
    effect_state_t *state = effect_data;
    if (state != 0) {
        if (state->items != 0) JumelleFreeArray((void **)&state->items);
        if (effect_data != 0) JumelleFreeArray((void **)&effect_data);
    }
}

u8 sub_38DCC(u16 columns, u16 rows, s16 horizontal_offset, u16 vertical_speed,
             u8 *source, u8 *destination)
{
  effect_state_t *state;
  u8 success = 0;
  if ((source != 0) && (destination != 0))
  {
    effect_data = (effect_state_t *) sub_6FBB4(sizeof(effect_state_t));
    state = effect_data;
    if (state)
    {
      state->columns = max32(2, columns);
      state->rows = max32(1, rows);
      state->count = state->columns * state->rows;
      state->source = source;
      state->destination = destination;
      state->items = (effect_item_t *) sub_6FBB4(state->count * (sizeof(effect_item_t)));
      if (state->items)
      {
        state->screen_a = EffetBufferNormal;
        state->screen_b = EffetBufferNormal + 0xc800;
        {
          u8 perm_value_0;
          u8 *perm_address_0;
          perm_address_0 = &perm_value_0;
          *perm_address_0 = 1;
          {
            u8 perm_value_1;
            u8 *perm_address_1;
            perm_address_1 = &perm_value_1;
            *perm_address_1 = perm_value_0;
            {
              u8 perm_value_2;
              u8 *perm_address_2;
              perm_address_2 = &perm_value_2;
              *perm_address_2 = perm_value_1;
              success = (u8) perm_value_2;
            }
          }
        }
      }
    }
  }
  if (success)
  {
    effect_copy_band(state->source, state->screen_a, 0xa0);
    effect_copy_band(state->source + 0xa0, state->screen_b, 0xa0);
    sub_38B58(horizontal_offset, vertical_speed);
  }
  else
  {
    sub_38DA0();
  }
  return success;
}

s16 sub_38ED4(void)
{
    effect_state_t *state = effect_data;
    effect_item_t *item = state->items;
    u16 item_index;
    s32 amount = state->speed;
    u8 step;

    if (amount < 0) amount = -amount;
    step = (u8)amount;
    for (item_index = 0; item_index < state->count; ++item_index) {
        s32 center_x = (item->x0 + item->x2) >> 1;
        s32 center_y = (item->y0 + item->y2) >> 1;
        s32 center_z = (item->width0 + item->width2) >> 1;
        u8 vertex = 0;
        s32 output = 0;
        do {
            u8 repeat;
            s32 x, y, z;
            s32 temp;
            s32 divisor;
            s16 sinus_value;
            s16 cosinus_value;
            for (repeat = 0; repeat < step; ++repeat) {
                ((s32 *)item)[vertex * 3 + 0] += item->random_x;
                ((s32 *)item)[vertex * 3 + 1] += item->random_y;
                if (((s32 *)item)[vertex * 3 + 2] < 50) item->sort_key = 0;
                ((s32 *)item)[vertex * 3 + 2] += item->sort_key;
            }
            x = ((s32 *)item)[vertex * 3 + 0] - center_x;
            y = ((s32 *)item)[vertex * 3 + 1] - center_y;
            z = ((s32 *)item)[vertex * 3 + 2] - center_z;
            sinus_cosinus((s16)(item->color_b * 2), &sinus_value, &cosinus_value);
            temp = (x * cosinus_value - y * sinus_value) >> 9;
            y = y * cosinus_value + x * sinus_value;
            x = temp;
            sinus_cosinus((s16)(item->color_g * 2), &sinus_value, &cosinus_value);
            temp = z * sinus_value + x * cosinus_value;
            z = (z * cosinus_value - x * sinus_value) >> 9;
            x = temp;
            sinus_cosinus((s16)(item->color_r * 2), &sinus_value, &cosinus_value);
            y >>= 9;
            temp = z * sinus_value + y * cosinus_value;
            z = (z * cosinus_value - y * sinus_value) >> 9;
            y = temp;
            divisor = z + center_z;
            item->vertices[output] = center_x + x / divisor;
            item->vertices[output + 1] = center_y + y / divisor;
            ++vertex;
            output += 2;
        } while (vertex < 4);
        item->color_r += step * item->color_step_r;
        item->color_g += step * item->color_step_g;
        item->color_b += step * item->color_step_b;
        ++item;
    }
    if (state->frame > 20 && state->frame % 5 == 0) {
        item = state->items;
        for (item_index = 0; item_index < state->count; ++item_index) {
            item->random_y += step;
            ++item;
        }
    }
    state->frame += step;
    if (state->frame > state->frame_limit) state->frame = 0;
    return state->frame;
}

void sub_391C0(void)
{
    effect_state_t *state = effect_data;
    effect_item_t *items = state->items;
    effect_item_t *item = items;
    s32 i;

    for (i = 0; i < state->count; ++i) {
        draw_effect_segment(state->destination,
                            (u8 *)(items + (item++)->draw_order) + 0x3c);
    }
}

