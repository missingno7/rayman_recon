/*
 * Mite enemies (GOG v1.21 object 1:42d30-1:43bc0). File name invented (Rayverse mite.c). Names: Rayverse/Android.
 * Worker candidate: measured statuses and remaining differences are in NOTES.md.
 */
#include "obj.h"
#include "map.h"
#include "inline_helpers.h"

/* Rayverse layout; Android symbol reports 20 bytes, DOS accesses confirm fields at +0x0e and +0x10. */
typedef struct poing_t {
    s32 y_16;
    u16 field_4;
    u16 speed_x;
    u16 charge;
    u16 field_A;
    u8 sub_etat;
    u8 is_returning;
    u8 is_active;
    u8 damage;
    u8 is_charging;
    u8 is_boum;
    u8 field_12;
    u8 field_13;
} poing_t;

#define true 1
#define false 0
#define GO_LEFT 0
#define GO_RIGHT 1
#define BLOCK_FULLY_SOLID 0
#define BLOCK_SOLID 1
#define MAX_2(x, y) if (x < y) x = y;
#define MIN_2(x, y) if (x > y) x = y
#define ABS(x) ((s32)(x) >= 0 ? (s32)(x) : -(s32)(x))

extern obj_t ray;
extern obj_t *poing_obj;
extern poing_t poing;
extern s16 ymap;
extern block_flags_t block_flags[64];

u8 EOA(obj_t *obj);
void SET_X_SPEED(obj_t *obj);
void CALC_MOV_ON_BLOC(obj_t *obj);
s32 set_sub_etat(obj_t *obj, u8 sub_etat);
void set_main_etat(obj_t *obj, u8 etat);
void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void obj_hurt(obj_t *obj);
u8 calc_typ_travd(obj_t *obj, u8 a2);
s16 calc_obj_dir(obj_t *obj);
s16 on_block_chdir(obj_t *obj, s16 center_x, s16 center_y);
void makeUturn(obj_t *obj);
u8 BTYP(s16 tile_x, s16 tile_y);
void DO_MITE2_ESQUIVE(obj_t *obj);
void calc_esquive_poing(obj_t *obj, s16 *diff_x, s16 *diff_y, s16 *unk);
//52D30
void mite_esquive_poing(obj_t* mit_obj, s16* out_diff_x) {
    /* 4A85C 8016F05C -O2 -msoft-float */
    s16 unk_x_1; s16 unk_y_1;
    s16 unk_y_2;

    if (!poing_obj->is_active) {
        unk_x_1 = ray.x + ray.offset_bx;
        unk_y_1 = ray.y + ((ray.offset_by + ray.offset_hy) >> 1) - 10;
        poing_obj->speed_x = 0;
        poing_obj->speed_y = 0;
    } else {
        unk_x_1 = poing_obj->x + poing_obj->offset_bx;
        unk_y_1 = poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1) - 3;
    }
    unk_y_2 = mit_obj->y + ((mit_obj->offset_hy + mit_obj->offset_by) >> 1) - unk_y_1;
    *out_diff_x = unk_x_1 - mit_obj->x - mit_obj->offset_bx;

    if (unk_y_2 < 0) {
        if (poing_obj->speed_y > 0) {
            if (unk_y_2 > -30)
                mit_obj->speed_y += 2;
            else if (unk_y_2 > -48)
                mit_obj->speed_y += 1;
        } else {
            if (unk_y_2 > -30)
                mit_obj->speed_y -= 2;
            else if (unk_y_2 > -48)
                mit_obj->speed_y -= 1;
        }
    } else {
        if (poing_obj->speed_y >= 0) {
            if (unk_y_2 < 30)
                mit_obj->speed_y += 2;
            else if (unk_y_2 < 48)
                mit_obj->speed_y += 1;
        } else {
            if (unk_y_2 < 30)
                mit_obj->speed_y -= 2;
            else if (unk_y_2 < 48)
                mit_obj->speed_y -= 1;
        }
    }
}

//52E64
u8 HAS_MIT_JUMP(obj_t* obj) {
    /* 4AA1C 8016F21C -O2 -msoft-float */
    u8 res = false;

    if (obj->main_etat != 2) {
        if (obj->eta[obj->main_etat][obj->sub_etat].flags & 1)
            skipToLabel(obj, 1, true);
    }
    else
    {
        if (obj->main_etat == 2 && obj->sub_etat == 1 && obj->anim_frame == 0)
        {
            obj->speed_y = -5;
            obj->gravity_value_1 = 0;
            obj->gravity_value_2 = 0;
            obj->y -= 14;
        }
        else
            res = true;
    }

    return res;
}

//52ED0
u8 ray_over_mit(obj_t* obj, s16 a2) {
    /* 4AACC 8016F2CC -O2 -msoft-float */
    return (!(obj->flags.flip_x) && a2 > 0) || (obj->flags.flip_x && a2 < 0);
}

//52EF4
void fix_mit_Xspeed(obj_t* obj, s16 a2) {
    /* 4AB0C 8016F30C -O2 -msoft-float */
    s16 spd_x;
    eta_t* eta;
    s32 unk_1;

    if (obj->main_etat == 2 || obj->main_etat == 1) {
        if (ray_over_mit(obj, a2)) {
            if (obj->main_etat == 2 && obj->speed_y < -2 && (obj->sub_etat != 0 ? true : false))
                obj->speed_y++;
            else
                HAS_MIT_JUMP(obj);

            if (!obj->flags.flip_x)
                spd_x = 4;
            else
                spd_x = -4;
            obj->speed_x = spd_x;
        } else {
            obj->speed_x = -ray.speed_x;
            if (obj->x < obj->init_x && obj->flags.flip_x) {
                if (obj->speed_x < 0)
                    obj->speed_x = 0;
                else
                    obj->speed_x *= 2;
            } else if ((obj->x > obj->init_x) && !obj->flags.flip_x) {
                if (obj->speed_x > 0)
                    obj->speed_x = 0;
                else
                    obj->speed_x *= 2;
            }
        }

        if (obj->main_etat == 1) {
            obj->eta[obj->main_etat][obj->sub_etat].flags_bits.bit4 = (
                    (!(obj->flags.flip_x) && obj->speed_x > 0) ||
                    (obj->flags.flip_x && obj->speed_x < 0)) ? true : false;
        }

        if (obj->speed_x > 4)
            obj->speed_x = 4;
        else if (obj->speed_x < -4)
            obj->speed_x = -4;
    } else {
        SET_X_SPEED(obj);
    }
}

//53068
void DO_MIT_ATTAK(obj_t *obj)
{
    if (obj->main_etat == 0 && obj->sub_etat == 10) {
        if (obj->follow_id > 50)
            obj->follow_id -= 50;
    } else if (HAS_MIT_JUMP(obj)) {
        if ((obj->sub_etat != 1 ? true : false) && obj->follow_id == 500) {
            obj->config = 140;
            obj->follow_x = obj->x;
            ++obj->follow_id;
            obj->speed_y = 0;
            set_sub_etat(obj, 3);
        } else if (obj->sub_etat == 3) {
            if (obj->gravity_value_2 != 0 && obj->speed_y >= 0)
                obj->speed_y = obj->speed_y - 1;
        } else if (obj->sub_etat == 4) {
            s16 distance;
            s16 ray_distance;
            s32 abs_distance;
            s32 abs_ray_distance;
            s16 diff_y;

            abs_distance = ABS(obj->x - obj->follow_x);
            distance = abs_distance;
            abs_ray_distance = ABS(ray.x + ray.offset_bx - obj->follow_x - obj->offset_bx - 10);
            ray_distance = abs_ray_distance;
            if (distance > obj->config || (distance > ray_distance && obj->config == 140)) {
                if (obj->config == 140 && ray_distance < 140)
                    obj->config = ray_distance;
                if (obj->gravity_value_2 == 1) {
                    if (!obj->flags.flip_x)
                        ++obj->speed_x;
                    else
                        --obj->speed_x;
                }
                if (obj->gravity_value_1 == 1 && obj->speed_y > -2)
                    --obj->speed_y;
            } else {
                diff_y = obj->offset_by + (s16)obj->y - (s16)ray.y - ray.offset_by;
                if ((obj->speed_x <= 0 && !obj->flags.flip_x) ||
                    (obj->speed_x >= 0 && obj->flags.flip_x)) {
                    if (!obj->flags.flip_x)
                        obj->speed_x = -4;
                    else
                        obj->speed_x = 4;
                    if (diff_y > 10)
                        obj->speed_y = -1;
                    else if (diff_y < -20)
                        obj->speed_y = 2;
                    else if (diff_y < -10)
                        obj->speed_y = 1;
                    else
                        obj->speed_y = 0;
                } else {
                    if (diff_y > -10 && ray.main_etat == 0)
                        obj->speed_y = 0;
                    else
                        obj->speed_y = 1;
                    if ((distance < 8 && !obj->flags.flip_x) ||
                        (distance > 8 && obj->flags.flip_x)) {
                        --obj->speed_x;
                        MAX_2(obj->speed_x, -4);
                    } else {
                        ++obj->speed_x;
                        MIN_2(obj->speed_x, 4);
                    }
                    if (obj->speed_x == 0 && distance < 8) {
                        obj->follow_id = 0;
                        set_sub_etat(obj, 0);
                    }
                }
            }
            if (block_flags[calc_typ_travd(obj, false)].bits.unused0) {
                if (obj->speed_x > 0)
                    obj->speed_x = -1;
                else
                    obj->speed_x = 1;
            }
        }
    }
}

//53320
u8 IS_MIT_PAF(obj_t* obj) {
    /* 4B154 8016F954 -O2 -msoft-float */
    s16 spd_x;
    u8 res = false;

    if (obj->main_etat == 0 && obj->sub_etat == 3)
    {
        obj->speed_y = -4;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        res = true;
    }
    else if (
            (obj->main_etat == 1 && obj->sub_etat == 2) ||
            (obj->main_etat == 0 && (obj->sub_etat == 2 || obj->sub_etat == 4)) ||
            (obj->main_etat == 2 && obj->sub_etat == 2)
            )
    {
        res = true;
        if (EOA(obj))
            obj->follow_id = 0;
        else
        {
            if (obj->main_etat != 2)
                SET_X_SPEED(obj);
            else
            {
                obj->speed_y = 0;
                if (obj->follow_id >= 500)
                {
                    s32 unk_x_3;
                    unk_x_3 = obj->x - obj->follow_x;
                    unk_x_3 = ABS(unk_x_3);

                    if ((s16)unk_x_3 < 8) {
                        if (!obj->flags.flip_x)
                            obj->speed_x--;
                        else
                            obj->speed_x++;
                    } else {
                        if (!obj->flags.flip_x)
                            spd_x = 4;
                        else
                            spd_x = -4;
                        obj->speed_x = spd_x;
                    }
                }
            }
        }
    }

    return res;
}

//5342C
void DO_MIT_COMMAND(obj_t* obj) {
    /* 4B3A0 8016FBA0 -O2 -msoft-float */
    s16 diff_x_1;
    s16 diff_x_2;
    s16 diff_y_1;
    s16 esquive_x;
    u8 prev_flip_x;
    s32 diff_y_base;
    s32 main_index = obj->main_etat;
    eta_t *eta_row = obj->eta[main_index];
    eta_t* obj_eta = &eta_row[obj->sub_etat];

    if (!IS_MIT_PAF(obj)) {
        if (obj->follow_id >= 500) {
            DO_MIT_ATTAK(obj);
            return;
        }

        if (!(obj->main_etat == 0 && obj->sub_etat == 7)) {
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else if (obj->cmd == GO_RIGHT)
                obj->flags.flip_x = 1;

            if (obj->detect_zone_flag != 0)
            {
                diff_x_1 = ((s16)ray.x + ray.offset_bx) - (s16)obj->x - obj->offset_bx;
                diff_x_2 = ABS(diff_x_1) >> 1;
                diff_y_base = obj->offset_by + (s16)obj->y - (s16)ray.y;
                diff_y_1 = diff_y_base - ray.offset_by;
                obj->follow_id++;
                if (ray_over_mit(obj, diff_x_1))
                {
                    if (obj->main_etat == 0 && obj->sub_etat == 10)
                        set_sub_etat(obj, 12);
                    else
                        HAS_MIT_JUMP(obj);
                }
                else if (poing.is_active || poing.is_charging)
                {
                    mite_esquive_poing(obj, &esquive_x);
                    /* TODO: ??? readable ??? */
                    if (
                            !(obj->flags.flip_x) ?
                            (diff_x_1 < -10 && (-diff_x_2 < esquive_x || poing_obj->speed_x >= 0)) :
                            (diff_x_1 > 10 && (diff_x_2 > esquive_x || poing_obj->speed_x <= 0))
                            )
                    {
                        if (obj->speed_y < 0)
                            HAS_MIT_JUMP(obj);
                        else if (obj_eta->flags & 4)
                            set_main_and_sub_etat(obj, 0, 9);
                    }
                    else
                    {
                        if (obj->main_etat == 0 && obj->sub_etat == 10)
                            set_sub_etat(obj, 12);
                    }

                    MAX_2(obj->speed_y, -3);
                    MIN_2(obj->speed_y, 3);
                }
                else
                {
                    switch (ray.main_etat)
                    {
                        case 0:
                        case 3:
                            if (
                                    obj->main_etat == 1 ||
                                    (obj->main_etat == 0 && obj->sub_etat == 10)
                                    )
                                set_main_and_sub_etat(obj, 0, 8);

                            obj->speed_x = 0;
                            break;
                        case 1:
                            if (obj->main_etat != 2 &&
                                !(obj->main_etat == 0 && obj->sub_etat == 9) &&
                                !(obj->main_etat == 0 && obj->sub_etat == 10) &&
                                !(obj->main_etat == 0 && obj->sub_etat == 11))
                                set_main_and_sub_etat(obj, 1, 0);
                            break;
                        case 2:
                            if (HAS_MIT_JUMP(obj))
                            {
                                if (diff_y_1 > 0)
                                {
                                    if (obj->gravity_value_1 == 1)
                                    {
                                        obj->speed_y--;
                                        MAX_2(obj->speed_y, -3);
                                    }
                                }
                                else
                                MIN_2(obj->speed_y, 1);
                            }
                            break;
                    }
                }

                if (
                        diff_x_2 < 40 && diff_y_1 < 10 &&
                        obj->link == 0 && (obj_eta->flags & 2)
                        )
                {
                    if (obj->main_etat == 1)
                        set_main_etat(obj, 0);

                    set_sub_etat(obj, 11);
                    obj->link = 1;
                    obj->timer = 0;
                }

                if (obj->link == 1)
                {
                    obj->timer++;
                    if (obj->timer > 40)
                        obj->link = 0;
                }

                if (obj->y + obj->offset_hy < ymap - 50)
                    obj->speed_y += 2;

                fix_mit_Xspeed(obj, diff_x_1);
            }
            else
            {
                obj->speed_x = 0;
                if (obj->main_etat != 2)
                {
                    if (obj->main_etat == 1)
                    {
                        set_main_and_sub_etat(obj, 0, 8);
                        skipToLabel(obj, 7, true);
                    }

                    prev_flip_x = obj->flags.flip_x;
                    calc_obj_dir(obj);
                    if (obj->flags.flip_x != prev_flip_x)
                        skipToLabel(obj, 3, true);
                }
            }

            if (block_flags[obj->btypes[3]].bits.unused0)
                obj->speed_y = 0;

            if (block_flags[calc_typ_travd(obj, false)].bits.unused0)
                obj->speed_x = 0;

            if (obj->main_etat != 2)
                CALC_MOV_ON_BLOC(obj);
        }
    }
}

//53838
void DoMitePoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == 1) {
        obj_hurt(obj);
        if (obj->hit_points != 0) {
            skipToLabel(obj, 4, true);
        } else {
            set_main_and_sub_etat(obj, 0, 3);
            obj->flags.read_commands = 0;
        }
    }
}

//5387C
void DO_MITE2_COMMAND(obj_t* obj) {
    /* 4BA08 80170208 -O2 -msoft-float */
    s16 unk_1;

    if (obj->main_etat == 0 && obj->sub_etat == 3)
    {
        obj->speed_x = 0;
        obj->speed_y = -8;
        obj->flags.read_commands = 0;
    }
    else
    {
        DO_MITE2_ESQUIVE(obj);
        if (obj->follow_id != 0 && HAS_MIT_JUMP(obj))
        {
            if (obj->flags.flip_x)
                obj->speed_x = 2;
            else
                obj->speed_x = -2;
        }

        if (obj->main_etat != 2)
        {
            if (on_block_chdir(obj, obj->offset_bx, obj->offset_by - 16))
            {
                obj->follow_id = 502;
                obj->speed_x = 0;
            }
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
        }

        unk_1 = calc_typ_travd(obj, false);
        if (obj->main_etat == 2)
        {
            if (
                    block_flags[
                            BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_hy) >> 4)
                    ].bits.unused0 &&
                    obj->speed_y < 0
                    )
                obj->speed_y = 1;
        }
        else
        {
            if (!(block_flags[obj->btypes[4]].bits.solid))
            {
                set_main_and_sub_etat(obj, 2, 0);
                skipToLabel(obj, 7, true);
            }
        }

        if (block_flags[unk_1].bits.unused0)
        {
            if (obj->main_etat == 1)
                makeUturn(obj);
            else
            {
                obj->flags.flip_x = 1 - obj->flags.flip_x;
                obj->speed_x = -obj->speed_x;
            }
        }
    }
}

//53A04
void DoMite2PoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == 1) {
        if (poing_obj->speed_x > 0) {
            obj->flags.flip_x = 0;
        } else if (poing_obj->speed_x < 0) {
            obj->flags.flip_x = 1;
        }
        obj_hurt(obj);
        if (obj->hit_points != 0) {
            skipToLabel(obj, 4, true);
        } else {
            set_main_and_sub_etat(obj, 0, 3);
            obj->flags.read_commands = 0;
        }
    }
}

//53A68
void DO_MITE2_ESQUIVE(obj_t* mit2_obj) {
    /* 3C96C 8016116C -O2 -msoft-float */
    s16 unk_1;
    s16 diff_x;
    s16 diff_y;

    if (poing.is_active || poing.is_charging) {
        calc_esquive_poing(mit2_obj, &diff_x, &diff_y, &unk_1);
        if (ABS(diff_x) <= (s32)unk_1 && diff_y < 35 && mit2_obj->main_etat == 1 &&
                (
                        (!(mit2_obj->flags.flip_x) && diff_x < 0) ||
                        (mit2_obj->flags.flip_x && diff_x > 0)
                ) &&
                sgn(mit2_obj->speed_x) == -sgn(poing_obj->speed_x)
        ) {
            mit2_obj->speed_x = 0;
            mit2_obj->speed_y = 0;
            if (ray.main_etat != 2) {
                mit2_obj->follow_id = 501;
            } else {
                if (mit2_obj->eta[mit2_obj->main_etat][mit2_obj->sub_etat].flags & 4) {
                    set_main_and_sub_etat(mit2_obj, 0, 9);
                    skipToLabel(mit2_obj, 7, true);
                }
            }
        }
    } else {
        if (mit2_obj->main_etat == 0 && mit2_obj->sub_etat == 10) {
            mit2_obj->follow_id = 0;
            set_sub_etat(mit2_obj, 12);
            skipToLabel(mit2_obj, 8, true);
        }
    }
}


















