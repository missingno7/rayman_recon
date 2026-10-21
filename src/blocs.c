/* blocs.c (1:14e90..1:15e60). Names RAYVERSE (blocs1_* helpers from Rayverse); seeded by tools/seed.py, worker tu_blocs. finish pass fin_blocs; see config/units.json. Watcom places switch jump tables in _TEXT before the function that uses them. */
#include "obj.h"
#include "game.h"
#include "inline_helpers.h"
extern obj_type_flags_t flags[262];
#include "map.h"

#define BTYP_CHDIR 1
#define BTYP_CLIFF 25
#define BTYP_HURT 8
#define BTYP_LIANE 12
#define BTYP_NONE 0
#define BTYP_RESSORT 9
#define BTYP_SLIPPERY 30
#define BTYP_SLIPPERY_LEFT1_30 22
#define BTYP_SLIPPERY_LEFT2_30 23
#define BTYP_SLIPPERY_LEFT_45 19
#define BTYP_SLIPPERY_RIGHT1_30 20
#define BTYP_SLIPPERY_RIGHT2_30 21
#define BTYP_SLIPPERY_RIGHT_45 18
#define BTYP_SOLID 15
#define BTYP_SOLID_LEFT1_30 6
#define BTYP_SOLID_LEFT2_30 7
#define BTYP_SOLID_LEFT_45 3
#define BTYP_SOLID_PASSTHROUGH 14
#define BTYP_SOLID_RIGHT1_30 4
#define BTYP_SOLID_RIGHT2_30 5
#define BTYP_SOLID_RIGHT_45 2
#define BTYP_SPIKES 24
#define BTYP_WATER 10
#define TYPE_BADGUY1 0
#define TYPE_BADGUY2 9
#define TYPE_BADGUY3 165
#define TYPE_LIDOLPINK 3
#define TYPE_MITE 100
#define TYPE_RAYMAN 23
#define TYPE_SPIDER 65
#define TYPE_STONEDOG 40
#define TYPE_STONEDOG2 122
#define TYPE_STONEWOMAN 172
extern u8 block_flags[64];
extern u8 hand_btyp;
extern u8 hand_btypd;
extern u8 hand_btypg;
extern s16 jump_time;
extern obj_t ray;
extern rayevts_t RayEvts;
extern u8 ray_last_ground_btyp;
extern u8 ray_on_poelle;
extern s16 ray_wind_force;
typedef s32 (*calcbloc_func_t)(s32, s32);
extern calcbloc_func_t calcblocrecal[64];
extern calcbloc_func_t calcbloc1[64];
extern calcbloc_func_t calcbloc2[64];
extern calcbloc_func_t calcblocfloor[64];
extern u8 button_released;
#define TYPE_23_RAYMAN TYPE_RAYMAN
#define flags1_0x10_move_x (1UL << 12)
#define flags2_1_check_tile_type (1UL << 16)
#define flags2_8_can_jump (1UL << 19)
#define Abs(value) ((s32)(value) >= 0 ? (s32)(value) : -(s32)(value))
u8 BTYP(s16 tile_x, s16 tile_y);
u8 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void IS_RAY_ON_LIANE(void);
void Reset_air_speed(u8 is_rolling_speed);
void ray_jump(void);
void PlaySnd(s16 snd, s16 obj_id);
void allocatePaillette(obj_t *obj);
void obj_jump(obj_t *obj);
s16 instantSpeed(s16 speed);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void set_main_etat(obj_t* obj, u8 etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
u16 MURDUR(s16 x, s16 y);
s16 dist_to_bloc_floor(s16 btyp, s16 x, s32 y);
 s16 bloc_floor(s16 btype, s16 x, s16 y);
u16 MURDUR(s16 x, s16 y)
{
    return BTYP(x >> 4, y >> 4) == BTYP_SOLID;
}

s16 dist_to_bloc_floor(s16 btyp, s16 x, s32 y) {
    s32 result = calcblocfloor[btyp](x, (s16)y) - y;
    return result;
}

s16 bloc_floor(s16 btype, s16 x, s16 y) {
    if (dist_to_bloc_floor(btype, x, y) <= 0) {
        return btype;
    } else {
        return 0;
    }
}

u8 calc_typ_trav(obj_t* obj, u8 a2) {
    /* 6E710 80192F10 -O2 -msoft-float */
    s16 temp_v0_0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1_0;
    s16 var_a0_2;
    s32 temp_a0;
    s32 temp_s0;
    s32 var_a0_0;
    s32 var_s0_1;
    s32 var_s0_2;
    s32 var_s2;
    u8 var_s3_2;
    s32 var_v0_1;
    s32 var_v0_2;
    s16 temp_s1;
    u8 temp_v1_2;
    u8 var_s3_1;
    s16 test_3;

    var_s3_1 = a2;
    var_v0_1 = 0;
    var_s0_1 = 0;
    if ((obj->type == TYPE_23_RAYMAN) && RayEvts.tiny) {
        var_s3_1 = ashr16(a2, 1);
        if (var_s3_1 == 0 && !(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)) {
            var_s3_1 = 1;
        }
    }
    temp_v1_0 = obj->offset_bx + (u16) obj->x;
    temp_v0_0 = obj->offset_by + (u16) obj->y;
    var_v0_2 = 0 << 0x10;
    switch (BTYP((s32) temp_v1_0 >> 4, (s32) temp_v0_0 >> 4))
    {
        case BTYP_NONE:
        case BTYP_CHDIR:
        case BTYP_HURT:
        case BTYP_WATER:
        case BTYP_SPIKES:
        case BTYP_CLIFF:
            var_s0_1 += 8 - (u16) obj->speed_y;
            break;
        case BTYP_SOLID_RIGHT_45:
        case BTYP_SLIPPERY_RIGHT_45:
            if (obj->speed_x >= 0)
            {
                var_s0_1 += 0x11;
            }
            break;
        case BTYP_SOLID_LEFT_45:
        case BTYP_SLIPPERY_LEFT_45:
            if (obj->speed_x <= 0)
            {
                var_s0_1 += 0x11;
            }
            break;
        case BTYP_SOLID_RIGHT1_30:
        case BTYP_SOLID_RIGHT2_30:
        case BTYP_SLIPPERY_RIGHT1_30:
        case BTYP_SLIPPERY_RIGHT2_30:
            if (obj->speed_x >= 0)
            {
                var_s0_1 += 0x11;
            }
            else
            {
                var_s0_1 += 2;
            }
            break;
        case BTYP_SOLID_LEFT1_30:
        case BTYP_SOLID_LEFT2_30:
        case BTYP_SLIPPERY_LEFT1_30:
        case BTYP_SLIPPERY_LEFT2_30:
            if (obj->speed_x <= 0)
            {
                var_s0_1 += 0x11;
            }
            else
            {
                var_s0_1 += 2;
            }
            break;
        case BTYP_RESSORT:
        case BTYP_SOLID_PASSTHROUGH:
        case BTYP_SOLID:
        case BTYP_SLIPPERY:
            temp_v1_2 = obj->btypes[0];
            if ((s32) temp_v1_2 >= 2)
            {
                if ((s32) temp_v1_2 >= 4)
                {
                    if ((s32) temp_v1_2 <= 0x13)
                    {
                        if ((s32) temp_v1_2 >= 0x12)
                        {
                            var_s0_1 += 8;
                        }
                    }
                }
                else
                {
                    var_s0_1 += 8;
                }
            }
            switch (BTYP(temp_v1_0 >> 4, (temp_v0_0 - 0x10) >> 4))
            {
                case BTYP_NONE:
                case BTYP_CHDIR:
                case BTYP_HURT:
                case BTYP_WATER:
                case BTYP_SPIKES:
                case BTYP_CLIFF:
                    var_s0_1 += 9;
                    break;
                case BTYP_SOLID_RIGHT_45:
                case BTYP_SLIPPERY_RIGHT_45:
                    if (obj->speed_x >= 0)
                    {
                        var_s0_1 += 0x19;
                    }
                    break;
                case BTYP_SOLID_LEFT_45:
                case BTYP_SLIPPERY_LEFT_45:
                    if (obj->speed_x <= 0)
                    {
                        var_s0_1 += 0x19;
                    }
                    break;
                case BTYP_SOLID_RIGHT1_30:
                case BTYP_SOLID_RIGHT2_30:
                case BTYP_SLIPPERY_RIGHT1_30:
                case BTYP_SLIPPERY_RIGHT2_30:
                    if (obj->speed_x >= 0)
                    {
                        var_s0_1 += 0x11;
                    }
                    else
                    {
                        var_s0_1 += 2;
                    }
                    break;
                case BTYP_SOLID_LEFT1_30:
                case BTYP_SOLID_LEFT2_30:
                case BTYP_SLIPPERY_LEFT1_30:
                case BTYP_SLIPPERY_LEFT2_30:
                    if (obj->speed_x <= 0)
                    {
                        var_s0_1 += 0x11;
                    }
                    else
                    {
                        var_s0_1 += 2;
                    }
                    break;
                case BTYP_LIANE:
                    var_s0_1 += 1;
                    break;
                default:
                    var_s0_1 += 8;
                    break;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 16:
        case 17:
        case 26:
        case 27:
        case 28:
        case 29:
        default:
            /* on android default case has 0 */
            break;
    }
    temp_s0 = (s16) var_s0_1;
    var_s3_1 = var_s3_1 - ashr16(temp_s0, 4U);
    if ((var_s3_1) > 0x80U)
    {
        var_s3_1 = 0;
    }
    ashr16(temp_v1_0, 4U);
    ashr16(temp_v0_0 - temp_s0, 4U);
    var_s0_2 = 0;
    var_s2 = 0;
    while (var_s3_1 >= var_s0_2) {
        if ((MURDUR(temp_v1_0, (temp_v0_0 - temp_s0) - ((var_s3_1 - var_s0_2) * 0x10))) != 0)
        {
            var_s2 += 1;
        }
        var_s0_2 += 1;
    }

    if (var_s2 & 0xFF)
    {
        var_v0_1 = 0x0000000F;
    }
    return var_v0_1;
}

u8 calc_typ_travd(obj_t* obj, u8 a2) {
    s16 x_saved;
    s16 spd_x_sgn = sgn(obj->speed_x);
    s16 x_offs_1;
    s32 unk_1;
    s32 x_offs_2;
    s16 foll_x; s16 foll_y; s16 foll_w; s16 foll_h;
    u8 unk_2;
    s32 res;

    if (spd_x_sgn != 0)
    {
        x_offs_1 = 14;
        if (obj->type == TYPE_RAYMAN)
        {
            unk_1 = 4;
            if (RayEvts.tiny)
            {
                x_offs_1 = 8;
                unk_1 = 2;
            }

            if (ray.param != -1)
                x_offs_1 -= unk_1;
        }
        x_offs_1 *= spd_x_sgn;
        if (a2)
            x_offs_1 = -x_offs_1;
        x_saved = obj->x;
        obj->x += x_offs_1;
        if (!a2)
        {
            x_offs_2 = obj->speed_x;
            if (flags[obj->type].raw & flags1_0x10_move_x)
                x_offs_2 = ashr16(x_offs_2, 4);
            obj->x -= x_offs_2;
        }

        if (obj->flags.follow_enabled)
            GET_SPRITE_POS(obj, obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h);
        else
            foll_y = 0;

        unk_2 = ashr16(-(foll_y + obj->offset_hy) + obj->offset_by, 4);
        if (obj->type == TYPE_RAYMAN && ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
            unk_2 = ashr16(unk_2, 1);
        if (unk_2 != 0)
            unk_2--;
        res = calc_typ_trav(obj, unk_2);
        obj->x = x_saved;
    }
    else
        res = 0;

    return res;
}

void TEST_FIN_BLOC(obj_t* obj) {
    eta_t **eta;
    /* 6EC34 80193434 -O2 -msoft-float */
    if (!(block_flags[obj->btypes[0]] & 2))
    {
        switch (obj->type)
        {
            case TYPE_RAYMAN:
                    IS_RAY_ON_LIANE();
                    if (
                            !(hand_btyp == BTYP_LIANE || hand_btypd == BTYP_LIANE || hand_btypg == BTYP_LIANE ||
                              ray.main_etat == 5)
                            )
                    {
                        eta = ray.eta;
                        if (eta[ray.main_etat][ray.sub_etat].flags & 0x40)
                        {
                            ray.speed_y = 0;
                            ray.y += 16;
                            ray.screen_y += 16;
                        }

                        if (ray_on_poelle)
                        {
                            if (ray.main_etat == 0 && ray.sub_etat == 40)
                                set_main_and_sub_etat(obj, 2, 26);
                            else
                                set_main_and_sub_etat(obj, 2, 28);
                        }
                        else
                        {
                            if (ray.main_etat == 1 && (ray.sub_etat == 9 || ray.sub_etat == 11)) {
                                ray.flags.flip_x = !ray.flags.flip_x;
                            }
                            if (Abs(ray.speed_x) <= 2)
                            {
                                Reset_air_speed(0);
                                if (!(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40))
                                    set_main_and_sub_etat(obj, 2, 24);
                                else
                                    set_main_and_sub_etat(obj, 2, 33);
                            }
                            else
                            {
                                Reset_air_speed(1);
                                set_main_and_sub_etat(obj, 2, 32);
                            }
                        }
                        jump_time = 0;
                        if (ray_last_ground_btyp == 0 || ray_wind_force != 0)
                            ray.nb_cmd = 1;
                        else
                            ray.nb_cmd = 0;
                    }
                break;
            case TYPE_LIDOLPINK:
                set_main_and_sub_etat(obj, 2, 2);
                skipToLabel(obj, 2, 1);
                break;
            case TYPE_BADGUY1:
                set_main_etat(obj, 2);
                if (obj->sub_etat != 2)
                    set_sub_etat(obj, 2);
                else
                    set_sub_etat(obj, 10);
                break;
            case TYPE_BADGUY2:
            case TYPE_BADGUY3:
                set_main_and_sub_etat(obj, 2, 2);
                break;
            case TYPE_STONEDOG:
            case TYPE_STONEDOG2:
                skipToLabel(obj, 2, 1);
                obj->gravity_value_2 = 7;
                obj->speed_y = 0;
                break;
            case TYPE_MITE:
                skipToLabel(obj, 0, 1);
                break;
            case TYPE_SPIDER:
                obj->cmd_context_depth = -1;
                set_main_and_sub_etat(obj, 2, 0);
                skipToLabel(obj, 8, 1);
                obj->speed_x = 0;
                obj->speed_y = 0;
                break;
            case TYPE_STONEWOMAN:
                set_main_and_sub_etat(obj, 2, 4);
                skipToLabel(obj, 15, 0);
                break;
        }
        obj->link = 0;
        obj->gravity_value_1 = 0;
        if (!(obj->type == TYPE_STONEDOG || obj->type == TYPE_STONEDOG2))
            obj->gravity_value_2 = 0;
    }
}

u8 TEST_IS_ON_RESSORT_BLOC(obj_t* obj) {
    u8 result = 0;
    if (obj->btypes[0] == BTYP_RESSORT && obj->speed_y >= 0) {
        if (obj->type == TYPE_23_RAYMAN) {
            result = 1;
        } else if (flags[obj->type].raw & flags2_8_can_jump) {
            result = 1;
        }
    }
    return result;
}

u8 IS_ON_RESSORT_BLOC(obj_t* obj) {
    u8 result = 0;
    u8 btype = obj->btypes[0];
    if (btype == BTYP_RESSORT && obj->speed_y >= 0) {
        if (obj->type == TYPE_23_RAYMAN) {
            result = 1;
            button_released = result;
            set_main_and_sub_etat(&ray, 0, 0);
            ray_jump();
            PlaySnd(249, -1);
            ray.speed_y -= 3;
            jump_time = 12;
            allocatePaillette(&ray);
        } else if ((flags[obj->type].raw & flags2_8_can_jump) != 0) {
            result = 1;
            obj_jump(obj);
        }
    }
    return result;
}

void CALC_MOV_ON_BLOC(obj_t* obj) {
    s16 temp_s0_1;
    s16 temp_s0_3;
    s32 temp_s1_1;
    s16 temp_s1_2;
    u8 temp_v0_1;
    s16 temp_v0_2;
    s16 temp_v1_6;
    s32 var_s1;
    s16 var_v0_2;
    s32 temp_a0;
    s32 temp_s0_2;
    s32 temp_s4;
    s32 x_in_tile;
    s32 var_s2_1;
    s32 var_s2_2;
    s32 y_in_tile;
    s32 speed_x;
    s32 var_v0;
    u32 temp_v1_4;
    u32 temp_v1_5;
    u8 temp_v1_2;
    u8 temp_v1_3;

    /*var_s2_1 = saved_reg_s2;*/
    temp_a0 = obj->type * 4;
    if ((flags[obj->type].raw & flags2_1_check_tile_type) || obj->type == TYPE_23_RAYMAN) {
        if ((flags[obj->type].raw & flags1_0x10_move_x) && obj->type != TYPE_23_RAYMAN) {
            speed_x = instantSpeed(obj->speed_x);
        } else {
            speed_x = obj->speed_x;
        }
        x_in_tile = speed_x + ((obj->offset_bx + obj->x) & 0xF);
        y_in_tile = (obj->offset_by + obj->y) & 0xF;
        if ((obj->type == TYPE_23_RAYMAN) && (ray.param != -1)) {
            if (speed_x > 0)
            {
                x_in_tile = 15;
            }
            else if (speed_x < 0)
            {
                x_in_tile = 0;
            }
            temp_v1_2 = obj->btypes[0];
            var_s2_1 = calcbloc1[obj->btypes[0]](x_in_tile, y_in_tile);
            y_in_tile = var_s2_1;
        }
        else
        {
            var_s2_1 = calcbloc1[obj->btypes[0]](x_in_tile, y_in_tile);

        }
        var_s1 = var_s2_1 - y_in_tile;
        x_in_tile = speed_x + (obj->x + obj->offset_bx);
        temp_s1_1 = var_s1 + (obj->y + obj->offset_by);
        temp_v0_1 = BTYP(x_in_tile >> 4, temp_s1_1 >> 4);
        x_in_tile = x_in_tile & 0xF;
        y_in_tile = temp_s1_1 & 0xF;
        switch (temp_v0_1)
        {
            case 0:
            case 1:
            case 8:
            case 10:
            case 24:
            case 25:
                temp_s1_2 = speed_x + (obj->x + obj->offset_bx);
                temp_s0_3 = var_s1 + (obj->y + obj->offset_by) + 16;
                temp_v0_2 = calcblocfloor[BTYP(temp_s1_2 >> 4, temp_s0_3 >> 4)](temp_s1_2 & 0xF, temp_s0_3 | ~0xF) - (temp_s0_3 | ~0xF);
                if (temp_v0_2 <= 2)
                    var_s2_1 = y_in_tile + temp_v0_2;
                else
                {
                    case 12:
                        var_s2_1 = y_in_tile;
                }
                break;
            case 2:
            case 18:
                var_s2_1 = 15 - x_in_tile;
                break;
            case 3:
            case 19:
                var_s2_1 = x_in_tile;
                break;
            case 4:
            case 20:
                var_s2_1 = 15 - (x_in_tile >> 1);
                break;
            case 5:
            case 21:
                var_s2_1 = 7 - (x_in_tile >> 1);
                break;
            case 6:
            case 22:
                var_s2_1 = ((x_in_tile) >> 1);
                break;
            case 7:
            case 23:
                var_s2_1 = ((x_in_tile) >> 1) + 8;
                break;
            case 9:
            case 14:
            case 15:
            case 30:
                temp_v1_5 = BTYP((obj->x + obj->offset_bx + speed_x) >> 4, (obj->y + obj->offset_by + var_s1 - 16) >> 4);
                var_s2_2 = calcbloc2[temp_v1_5](x_in_tile, y_in_tile);
                var_s2_1 = (var_s2_2 - 16);
                var_s2_1 = y_in_tile + var_s2_1;
                break;
        }
        if (!IS_ON_RESSORT_BLOC(obj)) {
            if (obj->type == TYPE_23_RAYMAN) {
                if ((ray.follow_id == -1 && ray.main_etat != 2) && Abs(obj->speed_y) > 2 && Abs(obj->speed_y) < 16 &&
                    !(block_flags[temp_v0_1] & 2))
                {
                    s16 saved_speed_y = obj->speed_y;
                    set_main_and_sub_etat(&ray, 0, 0);
                    button_released = 1;
                    ray_jump();
                    obj->speed_y = saved_speed_y;
                }
                else
                {
                    if (
                            ((ray.main_etat != 3) || (((ray.sub_etat != 32)) &&
//                                                     (ray.sub_etat != 0x26) && //NOTE: this subetat only in PS1
                                                     ((ray.sub_etat != 22))))
                            )
                    {
                        obj->speed_y = (var_s1 + var_s2_1) - y_in_tile;
                        TEST_FIN_BLOC(obj);
                    }
                }
            }
            else {
                obj->speed_y = (var_s1 + var_s2_1) - y_in_tile;
                TEST_FIN_BLOC(obj);
            }
        }
    }
}

void recale_position(obj_t *obj) {
    s16 full_x = obj->offset_bx + (u16)obj->x;
    s16 full_y = obj->offset_by + (u16)obj->y;
    s16 y = full_y & 0xfff0;
    s16 x = full_x & 15;
    s16 z = calcblocrecal[BTYP(full_x >> 4, full_y >> 4)](x, 0);
    if (obj->type != TYPE_23_RAYMAN)
        obj->y = y + z - obj->offset_by;
    else
        obj->speed_y = y + z - full_y;
}

s16 blocs1_empty(s32 x, s32 y) {
    return y + 1;
}

s32 blocs1_right_45(s32 x, s32 y) {
    return 15 - x;
}

s32 blocs1_left_45(s32 x, s32 y) {
    return x;
}

s32 blocs1_right1_30(s16 x, s32 y) {
    return 15 - (x >> 1);
}

s32 blocs1_right2_30(s16 x, s32 y) {
    return 7 - (x >> 1);
}

s32 blocs1_left1_30(s16 x, s32 y) {
    return x >> 1;
}

s32 blocs1_left2_30(s16 x, s32 y) {
    return (x >> 1) + 8;
}

s32 blocs3_empty(s32 x, s32 y) {
    return y;
}

s32 blocs1_liane(s32 x, s32 y) {
    return y;
}

s32 blocs4_empty(s32 x, s32 y) {
    return 0;
}

s32 blocs1_hor(s32 x, s32 y) {
    return 0;
}

s32 blocs2_empty(s32 x, s32 y) {
    return 16;
}

void init_calcbloc_func(void) {
    u8 i;
    for (i = 0; i < 64; ++i) {
        switch(i) {
            case 0:
            {
                calcbloc1[i] = blocs1_empty;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            } break;

            case 2:
            case 18: {
                calcbloc1[i] = blocs1_right_45;
                calcbloc2[i] = blocs1_right_45;
                calcblocfloor[i] = blocs1_right_45;
                calcblocrecal[i] = blocs1_right_45;
            } break;
            case 3:
            case 19: {
                calcbloc1[i] = blocs1_left_45;
                calcbloc2[i] = blocs1_left_45;
                calcblocfloor[i] = blocs1_left_45;
                calcblocrecal[i] = blocs1_left_45;
            } break;
            case 4:
            case 20: {
                calcbloc1[i] = blocs1_right1_30;
                calcbloc2[i] = blocs1_right1_30;
                calcblocfloor[i] = blocs1_right1_30;
                calcblocrecal[i] = blocs1_right1_30;
            } break;
            case 5:
            case 21: {
                calcbloc1[i] = blocs1_right2_30;
                calcbloc2[i] = blocs1_right2_30;
                calcblocfloor[i] = blocs1_right2_30;
                calcblocrecal[i] = blocs1_right2_30;
            } break;
            case 6:
            case 22: {
                calcbloc1[i] = blocs1_left1_30;
                calcbloc2[i] = blocs1_left1_30;
                calcblocfloor[i] = blocs1_left1_30;
                calcblocrecal[i] = blocs1_left1_30;
            } break;
            case 7:
            case 23: {
                calcbloc1[i] = blocs1_left2_30;
                calcbloc2[i] = blocs1_left2_30;
                calcblocfloor[i] = blocs1_left2_30;
                calcblocrecal[i] = blocs1_left2_30;
            } break;
            case 12: {
                calcbloc1[i] = blocs1_liane;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            } break;
            case 9:
            case 14:
            case 15:
            case 30: {
                calcbloc1[i] = blocs1_hor;
                calcbloc2[i] = blocs1_hor;
                calcblocfloor[i] = blocs1_hor;
                calcblocrecal[i] = blocs1_hor;
            } break;
            default: {
                calcbloc1[i] = blocs1_empty;
                calcbloc2[i] = blocs2_empty;
                calcblocfloor[i] = blocs3_empty;
                calcblocrecal[i] = blocs4_empty;
            } break;

        }
    }
}

