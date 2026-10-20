/*
 * UFO / moving platforms (GOG v1.21 object 1:6e760-1:6ec80). File name invented (Rayverse ufo.c; Rayverse has
 * stubs here, bodies adapted from the PS1 decomp). Status per function: config/units.json.
 */
/* GOG v1.21 UFO object, 1:6e760..1:6ec80. PS1 C bodies adapted to DOS layout. */
#include "obj.h"

#include "map.h"

extern map_data_t mp;
extern level_t level;
extern active_objects_t actobj;
extern s16 *link_init;

void set_main_and_sub_etat(obj_t *obj, u8 main_etat, u8 sub_etat);
void set_sub_etat(obj_t *obj, u8 sub_etat);
void PlaySnd(s16 sound, s16 obj_id);
s16 test_block_chdir(obj_t *obj, s16 *out_index, s16 off_x, s16 off_y);

#define TYPE_INDICATOR 0xA3
#define Abs(value) ((value) >= 0 ? (value) : -(value))

s16 test_block_chdir(obj_t *obj, s16 *out_unk, s16 off_x, s16 off_y)
{
    u8 res = 0;
    s16 obj_x = (obj->x + off_x) >> 4;
    s16 obj_y = (obj->y + off_y) >> 4;

    *out_unk = obj_x + obj_y * mp.width;
    if (*out_unk != obj->test_block_index)
    {
        obj->test_block_index = *out_unk;
        if (
            obj_x >= 0 && (mp.width - 1 >= obj_x) &&
            obj_y >= 0 && (mp.height - 1 >= obj_y)
        )
            res = mp.map[*out_unk].tile_type == 1;
    }
    return res;
}

void DO_MOVING_WITH_INDICATOR_COMMAND(obj_t *ufo_idc_obj)
{
    s16 sp10;
    obj_t *var_a3;
    s16 spd_x;
    s32 var_a0_2;
    s16 spd_y;
    s16 var_a2_2;
    s32 var_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 var_a0_1;
    s16 var_a2;
    s32 var_a3_2;
    s32 var_a3_3;
    s32 var_v0_7;
    u8 temp_s1;
    u8 temp_v1;

    ufo_idc_obj->flags.flip_x = 0;
    if (
        (ufo_idc_obj->main_etat == 0) && (ufo_idc_obj->sub_etat == 0x0E) &&
        ((test_block_chdir(ufo_idc_obj, &sp10, ufo_idc_obj->offset_bx, ufo_idc_obj->offset_by)) != 0)
    )
    {
        var_a2 = 0;
        var_a3 = &level.objects[actobj.objects[var_a2]];
        while ((var_a2 < actobj.num_active_objects) != 0)
        {
            if (var_a3->type == 0xA3)
            {
                if (
                    (s16) (((var_a3->x + var_a3->offset_bx) >> 4) +
                    (((var_a3->y + var_a3->offset_by) >> 4) * mp.width))
                    == sp10
                )
                {
                    var_v1 = ufo_idc_obj->speed_y;
                    spd_x = ufo_idc_obj->speed_x;
                    spd_y = var_v1;
                    var_a0_2 = Abs(spd_x);
                    var_v1 = Abs(var_v1);
                    if (var_a0_2 < var_v1)
                    {
                        var_a0_2 = var_v1;
                    }
                    switch (var_a3->sub_etat)
                    {
                    case 8:
                        if (spd_x == 0)
                        {
                            if (spd_y > 0)
                            {
                                spd_y = -var_a0_2;
                            }
                            else if (spd_y < 0)
                            {
                                spd_y = var_a0_2;
                            }
                        }
                        else if (spd_y == 0)
                        {
                            if (spd_x > 0)
                            {
                                spd_x = -var_a0_2;
                            }
                            else if (spd_x < 0)
                            {
                                spd_x = var_a0_2;
                            }
                        }
                        else
                        {
                            spd_x = -spd_x;
                            spd_y = -spd_y;
                        }
                        break;
                    case 9:
                    case 10:
                        var_a3_2 = -1;
                        if (var_a3->sub_etat == 9)
                        {
                            var_a3_2 = 1;
                        }
                        if (spd_y == 0)
                        {
                            if (spd_x > 0)
                            {
                                spd_x = var_a0_2 + var_a3_2;
                                if (spd_x > 4)
                                {
                                    spd_x = 4;
                                }
                                if (spd_x < 2)
                                {
                                    spd_x = 2;
                                }
                            }
                            else
                            {
                                if (spd_x < 0)
                                {
                                    spd_x = var_a3_2 + -var_a0_2;
                                    if (spd_x >= -1)
                                    {
                                        spd_x = -2;
                                    }
                                    if (spd_x < -4)
                                    {
                                        spd_x = -4;
                                    }
                                }
                            }
                        }
                        if (spd_x != 0)
                        {
                            spd_y = 0;
                        }
                        break;
                    case 11:
                    case 12:
                        var_a3_2 = -1;
                        if (var_a3->sub_etat == 0x0B)
                        {
                            var_a3_2 = 1;
                        }
                        if (spd_x == 0)
                        {
                            if (spd_y > 0)
                            {
                                spd_y = var_a0_2 + var_a3_2;
                                if (spd_y > 4)
                                {
                                    spd_y = 4;
                                }
                                if (spd_y < 2)
                                {
                                    spd_y = 2;
                                }
                            }
                            else
                            {
                                if (spd_y < 0)
                                {
                                    spd_y = var_a3_2 + -var_a0_2;
                                    if (spd_y > -2)
                                    {
                                        spd_y = -2;
                                    }
                                    if (spd_y < -4)
                                    {
                                        spd_y = -4;
                                    }
                                }
                            }
                        }
                        if (spd_y != 0)
                        {
                            spd_x = 0;
                        }
                        break;
                    case 4:
                        if ((spd_x) != 0)
                        {
                            if (spd_y == 0)
                            {
                            case 3: /* ??? */
                                spd_x = 0;
                                spd_y = -var_a0_2;
                            }
                        }
                        else
                        {
                            spd_x = var_a0_2;
                            spd_y = 0;
                        }
                        break;
                    case 5:
                        if (spd_x == 0)
                        {
                            spd_x = var_a0_2;
                            spd_y = 0;
                        }
                        else if (spd_y == 0)
                        {
                            spd_x = 0;
                            spd_y = var_a0_2;
                        }
                        break;
                    case 0:
                        spd_x = var_a0_2;
                        spd_y = 0;
                        break;
                    case 6:
                        if (spd_x != 0)
                        {
                            if (spd_y == 0)
                            {
                                spd_x = 0;
                                spd_y = -var_a0_2;
                            }
                        }
                        else
                        {
                            spd_x = -var_a0_2;
                            spd_y = 0;
                        }
                        break;

                    case 7:
                        if (spd_x == 0)
                        {
                            spd_x = -var_a0_2;
                            spd_y = 0;
                            break;
                        }
                        if (spd_y == 0)
                        {
                            spd_x = 0;
                            spd_y = var_a0_2;
                        }
                        break;
                    case 2:
                        spd_x = -var_a0_2;
                        spd_y = 0;
                        break;
                    case 1:
                        spd_x = 0;
                        spd_y = var_a0_2;
                        break;
                    }
                    ufo_idc_obj->speed_x = spd_x;
                    ufo_idc_obj->speed_y = spd_y;
                    return;
                }
            }
            var_a2 = var_a2 + 1;
            var_a3 = &level.objects[actobj.objects[var_a2]];
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
