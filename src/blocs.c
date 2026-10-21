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
/* Provisional DOS table-1 return width; see NOTES.md. Not yet a STRICT proof. */
typedef s16 (*calcbloc1_func_t)(s32, s32);
extern calcbloc_func_t calcblocrecal[64];
extern calcbloc1_func_t calcbloc1[64];
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

u8 calc_typ_trav(obj_t* obj, u8 height) {
    /* Local names INVENTED. DOS extent 1:14f60..151f8, STRICT. */
    s16 y;
    s16 x;
    s16 inset;
    s32 step;
    u8 hits;
    s32 result;
    u8 btype;

    result = 0;
    inset = 0;
    if ((obj->type == TYPE_23_RAYMAN) && RayEvts.tiny) {
        height = ashr16(height, 1);
        if (!(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)) {
            height = height ? height : 1;
        }
    }
    y = obj->offset_by + (u16) obj->y;
    x = obj->offset_bx + (u16) obj->x;
    switch (BTYP((s32) x >> 4, (s32) y >> 4))
    {
        case BTYP_NONE:
        case BTYP_CHDIR:
        case BTYP_HURT:
        case BTYP_WATER:
        case BTYP_SPIKES:
        case BTYP_CLIFF:
            inset += 8 - obj->speed_y;
            break;
        case BTYP_SOLID_RIGHT_45:
        case BTYP_SLIPPERY_RIGHT_45:
            if (obj->speed_x >= 0)
            {
                inset += 0x11;
            }
            break;
        case BTYP_SOLID_LEFT_45:
        case BTYP_SLIPPERY_LEFT_45:
            if (obj->speed_x <= 0)
            {
                inset += 0x11;
            }
            break;
        case BTYP_SOLID_RIGHT1_30:
        case BTYP_SOLID_RIGHT2_30:
        case BTYP_SLIPPERY_RIGHT1_30:
        case BTYP_SLIPPERY_RIGHT2_30:
            if (obj->speed_x >= 0)
            {
                inset += 0x11;
            }
            else
            {
                inset += 2;
            }
            break;
        case BTYP_SOLID_LEFT1_30:
        case BTYP_SOLID_LEFT2_30:
        case BTYP_SLIPPERY_LEFT1_30:
        case BTYP_SLIPPERY_LEFT2_30:
            if (obj->speed_x <= 0)
            {
                inset += 0x11;
            }
            else
            {
                inset += 2;
            }
            break;
        case BTYP_RESSORT:
        case BTYP_SOLID_PASSTHROUGH:
        case BTYP_SOLID:
        case BTYP_SLIPPERY:
            btype = obj->btypes[0];
            if (btype >= 2 && (btype <= 3 || (btype >= 18 && btype <= 19)))
                inset += 8;
            switch (BTYP(x >> 4, (y - 0x10) >> 4))
            {
                case BTYP_NONE:
                case BTYP_CHDIR:
                case BTYP_HURT:
                case BTYP_WATER:
                case BTYP_SPIKES:
                case BTYP_CLIFF:
                    inset += 9;
                    break;
                case BTYP_SOLID_RIGHT_45:
                case BTYP_SLIPPERY_RIGHT_45:
                    if (obj->speed_x >= 0)
                    {
                        inset += 0x19;
                    }
                    break;
                case BTYP_SOLID_LEFT_45:
                case BTYP_SLIPPERY_LEFT_45:
                    if (obj->speed_x <= 0)
                    {
                        inset += 0x19;
                    }
                    break;
                case BTYP_SOLID_RIGHT1_30:
                case BTYP_SOLID_RIGHT2_30:
                case BTYP_SLIPPERY_RIGHT1_30:
                case BTYP_SLIPPERY_RIGHT2_30:
                    if (obj->speed_x >= 0)
                    {
                        inset += 0x11;
                    }
                    else
                    {
                        inset += 2;
                    }
                    break;
                case BTYP_SOLID_LEFT1_30:
                case BTYP_SOLID_LEFT2_30:
                case BTYP_SLIPPERY_LEFT1_30:
                case BTYP_SLIPPERY_LEFT2_30:
                    if (obj->speed_x <= 0)
                    {
                        inset += 0x11;
                    }
                    else
                    {
                        inset += 2;
                    }
                    break;
                case BTYP_LIANE:
                    inset += 1;
                    break;
                default:
                    inset += 8;
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
    height = height - ashr16(inset, 4U);
    if ((height) > 0x80U)
    {
        height = 0;
    }
    /* Original discarded helper calls: DOS 1:15183 and 1:1519c. */
    ashr16(x, 4U);
    ashr16(y - inset, 4U);
    step = 0;
    hits = 0;
    for (; step <= height; step++) {
        if ((MURDUR(x, (y - inset) - ((height - step) * 0x10))) != 0)
        {
            hits += 1;
        }
    }

    if (hits)
    {
        result = 0x0000000F;
    }
    return result;
}

u8 calc_typ_travd(obj_t* obj, u8 backwards) {
    /* Local names INVENTED. DOS 1:151f8..15388; word sprite outputs. */
    eta_t **eta;
    s16 x_saved;
    s16 direction = sgn(obj->speed_x);
    s16 x_offset;
    u8 follow_inset;
    s16 x_speed;
    s16 sprite_y; s16 sprite_x; s16 sprite_w; s16 sprite_h;
    u8 height;
    u8 result;

    if (direction != 0)
    {
        if (obj->type == TYPE_RAYMAN)
        {
            follow_inset = 4;
            if (RayEvts.tiny)
            {
                x_offset = 8;
                follow_inset = 2;
            }
            else
                x_offset = 14;

            if (ray.param != -1)
                x_offset -= follow_inset;
        }
        else
            x_offset = 14;
        if (direction < 0)
            x_offset = -x_offset;
        if (backwards)
            x_offset = -x_offset;
        x_saved = obj->x;
        obj->x += x_offset;
        if (!backwards)
        {
            x_speed = obj->speed_x;
            if (flags[obj->type].raw & flags1_0x10_move_x)
                x_speed = ashr16(x_speed, 4);
            obj->x -= x_speed;
        }

        if (obj->flags.follow_enabled)
            GET_SPRITE_POS(obj, obj->follow_sprite, &sprite_x, &sprite_y, &sprite_w, &sprite_h);
        else
            sprite_y = 0;

        height = ashr16(-(sprite_y + obj->offset_hy) + obj->offset_by, 4);
        if (obj->type == TYPE_RAYMAN) {
            eta = ray.eta;
            if (eta[ray.main_etat][ray.sub_etat].flags & 0x40)
                height = ashr16(height, 1);
        }
        if (height != 0)
            height--;
        result = calc_typ_trav(obj, height);
        obj->x = x_saved;
    }
    else
        result = 0;

    return result;
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

void CALC_MOV_ON_BLOC(obj_t *obj) {
    /* INVENTED locals. DOS 1:15820..15b30; exact extent, still NEAR_MATCH. */
    s16 speed_x, delta, full_x, full_y, x, y, height, next_y, next_x;
    s32 dist;
    u8 btyp, floor_type;
    s16 floor_x, floor_y;
    if ((flags[obj->type].raw & flags2_1_check_tile_type) || obj->type == TYPE_23_RAYMAN) {
        if ((flags[obj->type].raw & flags1_0x10_move_x) && obj->type != TYPE_23_RAYMAN)
            speed_x = instantSpeed(obj->speed_x);
        else speed_x = obj->speed_x;
        full_x = obj->offset_bx + (u16)obj->x;
        full_y = obj->offset_by + (u16)obj->y;
        x = (full_x & 15) + speed_x;
        y = full_y & 15;
        if (obj->type == TYPE_23_RAYMAN && ray.param != -1) {
            if (speed_x > 0) x = 15;
            else if (speed_x < 0) x = 0;
            height = calcbloc1[obj->btypes[0]](x, y);
            y = height;
        } else height = calcbloc1[obj->btypes[0]](x, y);
        delta = height - y;
        x = full_x + speed_x;
        next_y = full_y + delta;
        btyp = BTYP(x >> 4, next_y >> 4);
        x &= 15;
        y = next_y & 15;
        switch (btyp) {
        case 9: case 14: case 15: case 30:
            height = calcbloc2[BTYP((full_x + speed_x) >> 4, (full_y + delta - 16) >> 4)](x, y) + (y - 16);
            break;
        case 2: case 18: height = 15 - x; break;
        case 3: case 19: height = x; break;
        case 4: case 20: height = 15 - (x >> 1); break;
        case 5: case 21: height = 7 - (x >> 1); break;
        case 6: case 22: height = x >> 1; break;
        case 7: case 23: height = (x >> 1) + 8; break;
        case 0:
        default:
            next_x = full_x + speed_x;
            next_y = full_y + delta + 16;
            floor_x = next_x & 15;
            floor_y = (next_y & 15) - 16;
            floor_type = BTYP(next_x >> 4, next_y >> 4);
            dist = calcblocfloor[floor_type](floor_x, floor_y) - floor_y;
            if ((s16)dist > 2)
                height = y;
            else
                height = y + dist;
            break;
        case 12:
            height = y;
            break;
        }
        if (!IS_ON_RESSORT_BLOC(obj)) {
            switch (obj->type) {
            case TYPE_23_RAYMAN:
                if ((ray.follow_id == -1 && !(ray.main_etat == 2)) && Abs(obj->speed_y) > 2 && Abs(obj->speed_y) < 16 && !(block_flags[btyp] & 2)) {
                    s16 saved_speed_y = obj->speed_y;
                    set_main_and_sub_etat(&ray, 0, 0);
                    button_released = 1;
                    ray_jump();
                    obj->speed_y = saved_speed_y;
                } else if (!(ray.main_etat == 3 && ray.sub_etat == 32) && !(ray.main_etat == 3 && ray.sub_etat == 22)) {
                    obj->speed_y = delta + height - y;
                    TEST_FIN_BLOC(obj);
                }
                break;
            default:
                obj->speed_y = delta + height - y;
                TEST_FIN_BLOC(obj);
                break;
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

/* DOS 1:15bf0 is shared by both names. The forward wrapper folds naturally
 * into the following implementation; both public names remain in the object. */
s32 blocs3_empty(s32 x, s32 y);
s32 blocs1_liane(s32 x, s32 y) {
    return blocs3_empty(x, y);
}

s32 blocs3_empty(s32 x, s32 y) {
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

