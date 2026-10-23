/*
 * UFO / moving platforms (GOG v1.21 object 1:6e760-1:6ec80). File name invented (Rayverse ufo.c; Rayverse has
 * stubs here, bodies adapted from the PS1 decomp). Worker candidate: see NOTES.md.
 */
#include "obj.h"

#include "map.h"
#include "inline_helpers.h"

extern map_data_t mp;
extern level_t level;
extern active_objects_t actobj;
extern s16 *link_init;

void set_main_and_sub_etat(obj_t *obj, u8 main_etat, u8 sub_etat);
void set_sub_etat(obj_t *obj, u8 sub_etat);
void PlaySnd(s16 sound, s16 obj_id);
s16 test_block_chdir(obj_t *obj, s16 *out_index, s16 off_x, s16 off_y);

#define TYPE_INDICATOR 0xA3
#define Abs(value) ((s32)(value) >= 0 ? (s32)(value) : -(s32)(value))

s16 test_block_chdir(obj_t *obj, s16 *out_unk, s16 off_x, s16 off_y)
{
    s16 res = 0;
    s16 obj_x = (obj->x + off_x) >> 4;
    s16 obj_y = (obj->y + off_y) >> 4;
    *out_unk = obj_y * mp.width + obj_x;
    if (*out_unk != (s16)obj->test_block_index)
    {
        obj->test_block_index = *out_unk;
        if (inrange(obj_x, 0, mp.width - 1) &&
            inrange(obj_y, 0, mp.height - 1))
            res = mp.map[*out_unk].tile_type == 1;
    }
    return res;
}

void DO_MOVING_WITH_INDICATOR_COMMAND(obj_t *ufo_idc_obj)
{
    s16 active_index;
    s16 block_index;
    obj_t *indicator;
    s16 speed_y;
    s16 speed_x;
    /* Only the final maximum is short. Keep the Abs inputs/results promoted:
     * DOS 1:6e8ed..6e912; the short result is retained in a dword stack slot. */
    s16 maximum_speed;

    ufo_idc_obj->flags.flip_x = 0;
    if (
        (ufo_idc_obj->main_etat == 0) && (ufo_idc_obj->sub_etat == 0x0E) &&
        ((test_block_chdir(ufo_idc_obj, &block_index, ufo_idc_obj->offset_bx, ufo_idc_obj->offset_by)) != 0)
    )
    {
        active_index = 0;
        indicator = &level.objects[actobj.objects[active_index]];
        for (; active_index < actobj.num_active_objects;
             ++active_index, indicator = &level.objects[actobj.objects[active_index]])
        {
            if (indicator->type != TYPE_INDICATOR)
                continue;
            if ((s16)((mp.width * ((indicator->offset_by + indicator->y) >> 4)) +
                ((indicator->x + indicator->offset_bx) >> 4)) != block_index)
                continue;
            speed_y = ufo_idc_obj->speed_y;
            speed_x = ufo_idc_obj->speed_x;
            {
                s32 abs_y = Abs((s32)speed_y);
                maximum_speed = max32(Abs((s32)speed_x), abs_y);
            }
            switch (indicator->sub_etat)
            {
            case 8:
                if (speed_x == 0)
                {
                    if (speed_y > 0)
                    {
                        speed_y = -maximum_speed;
                    }
                    else if (speed_y < 0)
                    {
                        speed_y = maximum_speed;
                    }
                }
                else if (speed_y == 0)
                {
                    if (speed_x > 0)
                    {
                        speed_x = -maximum_speed;
                    }
                    else if (speed_x < 0)
                    {
                        speed_x = maximum_speed;
                    }
                }
                else
                {
                    speed_x = -speed_x;
                    {
                        /* COMPILER_STEERED (permuter pilot, minimized): with active_index declared first, this
                         * temporary gives the DOS conflict order (EDX at 1:6eb5b); each change alone gives 363 diffs. */
                        s16 negated_y = -speed_y;
                        speed_y = negated_y;
                    }
                }
                break;
            case 9:
            case 10:
            {
                s32 delta;
                if (indicator->sub_etat == 9)
                {
                    delta = 1;
                }
                else
                {
                    delta = -1;
                }
                if (speed_y == 0)
                {
                    if (speed_x > 0)
                    {
                        speed_x = maximum_speed + delta;
                        if (speed_x > 4)
                        {
                            speed_x = 4;
                        }
                        if (speed_x < 2)
                        {
                            speed_x = 2;
                        }
                    }
                    else
                    {
                        if (speed_x < 0)
                        {
                            speed_x = delta - maximum_speed;
                            if (speed_x > -2)
                            {
                                speed_x = -2;
                            }
                            if (speed_x < -4)
                            {
                                speed_x = -4;
                            }
                        }
                    }
                }
                if (speed_x != 0)
                {
                    speed_y = 0;
                }
                break;
            }
            case 11:
            case 12:
            {
                s32 delta;
                if (indicator->sub_etat == 0x0B)
                {
                    delta = 1;
                }
                else
                {
                    delta = -1;
                }
                if (speed_x == 0)
                {
                    if (speed_y > 0)
                    {
                        speed_y = maximum_speed + delta;
                        if (speed_y > 4)
                        {
                            speed_y = 4;
                        }
                        if (speed_y < 2)
                        {
                            speed_y = 2;
                        }
                    }
                    else
                    {
                        if (speed_y < 0)
                        {
                            speed_y = delta - maximum_speed;
                            if (speed_y > -2)
                            {
                                speed_y = -2;
                            }
                            if (speed_y < -4)
                            {
                                speed_y = -4;
                            }
                        }
                    }
                }
                if (speed_y != 0)
                {
                    speed_x = 0;
                }
                break;
            }
            case 4:
                if (speed_x == 0)
                {
                    speed_x = maximum_speed;
                    speed_y = 0;
                }
                else if (speed_y == 0)
                {
                    speed_x = 0;
                    speed_y = -maximum_speed;
                }
                break;
            case 5:
                if (speed_x == 0)
                {
                    speed_x = maximum_speed;
                    speed_y = 0;
                }
                else if (speed_y == 0)
                {
                    speed_x = 0;
                    speed_y = maximum_speed;
                }
                break;
            case 0:
                speed_x = maximum_speed;
                speed_y = 0;
                break;
            case 6:
                if (speed_x == 0)
                {
                    speed_x = -maximum_speed;
                    speed_y = 0;
                }
                else if (speed_y == 0)
                {
                    speed_x = 0;
                    speed_y = -maximum_speed;
                }
                break;

            case 3:
                speed_x = 0;
                speed_y = -maximum_speed;
                break;
            case 7:
                if (speed_x == 0)
                {
                    speed_x = -maximum_speed;
                    speed_y = 0;
                    break;
                }
                if (speed_y == 0)
                {
                    speed_x = 0;
                    speed_y = maximum_speed;
                }
                break;
            case 2:
                speed_x = -maximum_speed;
                speed_y = 0;
                break;
            case 1:
                speed_x = 0;
                speed_y = maximum_speed;
                break;
            }
            ufo_idc_obj->speed_x = speed_x;
            ufo_idc_obj->speed_y = speed_y;
            return;
        }
    }
}

void DO_IDC_COMMAND(obj_t *idc_obj)
{
    switch (idc_obj->sub_etat)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        idc_obj->display_prio = 4;
        idc_obj->flags.flip_x = 0;
        break;
    default:
        idc_obj->display_prio = 0;
        break;
    }
}

void DO_LEV_POING_COLLISION(obj_t *lev_obj, s16 sprite)
{
    obj_t *idc_obj;

    if (lev_obj->main_etat == 5 && lev_obj->sub_etat == 53)
    {
        PlaySnd(235, lev_obj->id);
        set_main_and_sub_etat(lev_obj, 5, 54);
        idc_obj = &level.objects[link_init[lev_obj->id]];
        if (idc_obj->type == TYPE_INDICATOR)
        {
            switch (idc_obj->sub_etat)
            {
            case 0:
                set_sub_etat(idc_obj, 1);
                return;
            case 1:
                set_sub_etat(idc_obj, 2);
                return;
            case 2:
                set_sub_etat(idc_obj, 3);
                return;
            case 3:
                set_sub_etat(idc_obj, 0);
                break;
            }
        }
    }
}

void START_UFO(obj_t *ufo_idc_obj)
{
    if (ufo_idc_obj->main_etat == 0 && ufo_idc_obj->sub_etat == 13)
    {
        set_main_and_sub_etat(ufo_idc_obj, 0, 14);
        ufo_idc_obj->speed_x = 2;
        ufo_idc_obj->speed_y = 0;
    }
}
