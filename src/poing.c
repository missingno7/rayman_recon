/*
 * Rayman's fist (GOG v1.21 object 1:56670-1:57100). File name invented (Rayverse poing.c). Names: Rayverse/Android.
 * Worker candidate: measured statuses and remaining differences are in NOTES.md.
 */
#include "obj.h"
#include "game.h"
#include "inline_helpers.h"

extern obj_t ray;
extern obj_t *poing_obj;
extern poing_t poing;
extern level_t level;
extern rayevts_t RayEvts;
extern u8 horloge[25];
extern s16 decalage_en_cours;
extern s16 ray_zdc_x;
extern s16 ray_zdc_y;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern s16 xmap;
extern s16 ymap;
extern s16 helico_time;
extern u8 button_released;
extern s16 compteur_attente;
extern s16 poing_obj_id;

void set_sub_etat(obj_t *obj, u8 sub_etat);
void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
void del_alwobj(s16 obj_index);
void add_alwobj(obj_t *obj);
void RAY_STOP(void);
void DO_POING_COLLISION(void);
u8 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void GET_ANIM_POS(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
void calc_obj_pos(obj_t *obj);

void fist_U_turn(u8 a1);
void CALC_FIST_POS(void);
void RAY_THROW_FIST(void);
void RAY_PREPARE_FIST(void);
void RAY_GROW_FIST(void);
void fin_poing_follow(u8 a1);
void POING_FOLLOW(obj_t *obj);
void alter_fist_speed(obj_t *obj);
void switch_off_fist(void);
void DO_POING(obj_t *obj);
void allocatePoingBoum(void);

#define Abs(value) ((value) >= 0 ? (value) : -(value))
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])
#define false 0
#define true 1
#define TYPE_11_BOUM 11

void fist_U_turn(u8 a1)
{
    switch (poing_obj->sub_etat) {
    case 1:
        set_sub_etat(poing_obj, 2);
        break;
    case 3:
        set_sub_etat(poing_obj, 4);
        break;
    case 5:
        set_sub_etat(poing_obj, 6);
        break;
    case 8:
        set_sub_etat(poing_obj, 9);
        break;
    case 10:
        set_sub_etat(poing_obj, 11);
        break;
    case 12:
        set_sub_etat(poing_obj, 13);
        break;
    }

    if (a1)
        poing_obj->anim_frame = 7;
}

void CALC_FIST_POS(void)
{
    s32 screen_y;

    screen_y = poing.y_16;
    screen_y >>= 4;
    screen_y = (s16)screen_y;
    screen_y -= ymap;
    poing_obj->screen_y = screen_y;
    poing_obj->screen_x = poing_obj->x - xmap + 8;
}

void RAY_THROW_FIST(void)
{
    s32 unk_1;
    s16 unk_2;
    eta_t *eta_row;

    if (ray.main_etat == 2 && (ray.sub_etat == 25 || ray.sub_etat == 26))
        set_sub_etat(&ray, 29);
    else if (ray.main_etat == 2 && (ray.sub_etat == 27 || ray.sub_etat == 28))
        set_sub_etat(&ray, 30);
    else if (!(ray.main_etat == 2 && (ray.sub_etat == 17 || ray.sub_etat == 18 || ray.sub_etat == 19)))
        set_sub_etat(&ray, 13);

    if (ray.main_etat == 0) {
        if (ray.flags.flip_x)
            decalage_en_cours -= poing.charge * 1024 / 64;
        else
            decalage_en_cours += poing.charge * 1024 / 64;
    }
    poing.is_charging = false;
    poing.is_boum = false;
    poing.is_returning = false;
    poing_obj->flags.flip_x = ray.flags.flip_x;
    poing_obj->flags.alive = 1;
    poing_obj->is_active = 1;
    poing.is_active = true;
    del_alwobj(poing_obj_id);
    add_alwobj(poing_obj);
    poing_obj->speed_y = 0;
    poing_obj->anim_frame = 0;
    poing_obj->follow_id = -1;
    set_main_and_sub_etat(poing_obj, poing_obj->init_main_etat, poing_obj->init_sub_etat);
    eta_row = poing_obj->eta[poing_obj->main_etat];
    poing_obj->anim_index = eta_row[poing_obj->sub_etat].anim_index;
    poing_obj->x = ray_zdc_x + (ray_zdc_w >> 1) - poing_obj->offset_bx;
    poing_obj->x += (ray.flags.flip_x ? -5 : 5);
    poing.damage = ashr16(poing.charge, 4) + 1;

    switch (poing_obj->init_sub_etat) {
    case 12:
        poing.damage += 2;
    case 5:
        poing.speed_x = 11;
        break;
    case 10:
        poing.damage += 2;
    case 3:
        poing.speed_x = 8;
        break;
    case 8:
        poing.damage += 2;
    case 1:
        poing.speed_x = 5;
        break;
    }

    if ((ray.flags.flip_x && decalage_en_cours > 0) ||
        (!ray.flags.flip_x && decalage_en_cours < 0))
    {
        unk_1 = max32(Abs((s32)ray.speed_x) - 5, 0);
        poing.speed_x += unk_1;
    }

    poing.speed_x = min32(poing.speed_x, 16);

    if (poing_obj->flags.flip_x) {
        poing_obj->speed_x = poing.speed_x;
        poing_obj->x += 11;
    } else {
        poing_obj->speed_x = -poing.speed_x;
        poing_obj->x -= 11;
    }
    poing.field_A = (poing.charge * 130 / 64) + 40 - poing.speed_x * 3;
    unk_2 = ashl16(poing.charge, 8);
    poing.field_4 = unk_2;
    poing.charge = unk_2;
    poing.field_A = unk_2 * poing.speed_x / poing.field_A;
    poing_obj->y = ray_zdc_h + (ray_zdc_y >> 1) - ((poing_obj->offset_by + poing_obj->offset_hy) >> 1);
    poing.y_16 = ashl16(poing_obj->y, 4);
    CALC_FIST_POS();
}

void RAY_PREPARE_FIST(void)
{
    if (get_eta(&ray)->flags & 2) {
        if (ray.main_etat == 2) {
            if (ray.speed_y > 0)
                ray.eta[2][13].anim_speed_bits.high = ray.eta[2][2].anim_speed_bits.high;
            else
                ray.eta[2][13].anim_speed_bits.high = ray.eta[2][0].anim_speed_bits.high;
            poing.charge = 32;
            RAY_THROW_FIST();
            helico_time = -1;
            if (RayEvts.super_helico)
                button_released = 0;
        } else {
            RAY_STOP();
            set_sub_etat(&ray, 11);
            poing.is_charging = 1;
            compteur_attente = 0;
            poing.charge = 5;
        }
    }
}

void RAY_GROW_FIST(void)
{
    ++poing.charge;
    if (poing.charge >= 64)
        poing.charge = 63;
}

void fin_poing_follow(u8 a1)
{
    if (poing_obj->follow_id != -1) {
        obj_t *unk_obj = level.objects + poing_obj->follow_id;
        unk_obj->speed_x = 0;
        unk_obj->speed_y = 0;
        poing_obj->follow_id = -1;
        if (a1 || !(poing_obj->flags.alive)) {
            s32 ray_y;
            s32 unk_y;

            unk_obj->x = ray.x + ray.offset_bx - unk_obj->offset_bx;
            /* DOS 1:56c66/1:56c6c: these bound names have reversed vertical roles. */
            unk_obj->y = ray_zdc_h + (ray_zdc_y >> 1) - 5 -
                ((unk_obj->offset_by + unk_obj->offset_hy) >> 1);
        } else if (!a1) {
            unk_obj->x = unk_obj->init_x;
            unk_obj->y = unk_obj->init_y;
        }
    }
}

void POING_FOLLOW(obj_t *obj)
{
    obj_t *follow_obj = &level.objects[obj->follow_id];

    follow_obj->speed_x = obj->speed_x;
    follow_obj->speed_y = obj->speed_y;
    if (!(ray.iframes_timer == -1 && follow_obj->is_active))
        fin_poing_follow(0);
}

void alter_fist_speed(obj_t *obj)
{
    u8 accel_x = 0;
    s32 abs_spd_x;

    abs_spd_x = obj->speed_x;
    if (abs_spd_x < 0)
        abs_spd_x = -abs_spd_x;
    if (abs_spd_x >= 11)
        accel_x = 2;
    else if (Abs((s32)obj->speed_x) >= 8)
        accel_x = 1;
    else if (horloge[2] != 0)
        accel_x = 1;
    if (obj->flags.flip_x)
        obj->speed_x -= accel_x;
    else
        obj->speed_x += accel_x;
}

void switch_off_fist(void)
{
    poing.is_active = 0;
    poing.is_boum = 0;
    poing.charge = 5;
    poing.is_returning = 0;
    poing_obj->flags.alive = 0;
    poing_obj->is_active = 0;
    poing_obj->sub_etat = poing_obj->init_sub_etat;
    fin_poing_follow(0);
}

void DO_POING(obj_t *obj)
{
    s32 ray_diff_x;
    s32 ray_diff_y;
    s16 unk_x_1;
    s32 unk_y_1;
    s32 unk_1;

    if (poing.is_active) {
        if (obj->speed_x != 0) {
            CALC_FIST_POS();
            obj->y = poing.y_16 >> 4;
            if (poing.charge > 0) {
                if (poing.charge < poing.field_A)
                    poing.charge = 0;
                else
                    poing.charge -= poing.field_A;

                if (poing.charge == 0)
                    fist_U_turn(false);
            } else if (poing.is_returning == 0) {
                alter_fist_speed(obj);
            } else if (poing.is_returning == 1) {
                s32 return_speed = poing.speed_x;

                if (Abs((s32)obj->speed_x) < return_speed)
                    alter_fist_speed(obj);

                ray_diff_x = ray_zdc_x + (ray_zdc_w >> 1) - obj->offset_bx;
                if (obj->flags.flip_x)
                    ray_diff_x += 11;
                else
                    ray_diff_x -= 11;

                if (ray.main_etat == 4) {
                    if (obj->flags.flip_x)
                        ray_diff_x += 11;
                    else
                        ray_diff_x -= 11;
                }

                ray_diff_y =
                    (ray_zdc_h + (ray_zdc_y >> 1) -
                     ((obj->offset_by + obj->offset_hy) >> 1)) * 16;

                if ((obj->flags.flip_x && ray_diff_x >= obj->x) ||
                    (!obj->flags.flip_x && ray_diff_x <= obj->x))
                    switch_off_fist();
                else {
                    unk_x_1 = ray_diff_x - obj->x;
                    unk_y_1 = ray_diff_y - poing.y_16;
                    if (unk_x_1 != 0)
                        unk_y_1 = unk_y_1 / unk_x_1;
                    obj->speed_y = unk_y_1 * obj->speed_x;

                    {
                        unk_1 = obj->speed_y + poing.y_16;
                        if (inrange(unk_1, min32(poing.y_16, ray_diff_y), max32(poing.y_16, ray_diff_y)))
                            obj->speed_y >>= 4;
                        else
                            obj->speed_y = (ray_diff_y - poing.y_16) >> 4;
                    }
                }
            }

            if (!poing.is_boum && poing.is_active)
                DO_POING_COLLISION();
        } else {
            if (poing.is_boum && !poing.is_returning) {
                fist_U_turn(true);
                obj->anim_index = obj->eta[obj->main_etat][obj->sub_etat].anim_index;
            }
            poing.is_returning = true;
            if (obj->flags.flip_x)
                obj->speed_x--;
            else
                obj->speed_x++;
        }

        if (obj->follow_id != -1)
            POING_FOLLOW(obj);
    }
}

void allocatePoingBoum(void)
{
    obj_t *obj;
    s16 i;
    s16 nb_objs;
    s16 anim_x, anim_y, anim_w, anim_h;
    s16 poing_x, poing_y, poing_w, poing_h;

    i = 0;
    obj = level.objects + i;
    nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (obj->type == TYPE_11_BOUM && !obj->is_active) {
            add_alwobj(obj);
            GET_SPRITE_POS(poing_obj, 0, &poing_x, &poing_y, &poing_w, &poing_h);
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            obj->anim_frame = 0;
            obj->x = poing_x - obj->offset_bx;
            if (poing_obj->speed_x > 0)
                obj->x += poing_w;
            obj->y = poing_y + (poing_h >> 1) - ((obj->offset_by + obj->offset_hy) >> 1);
            calc_obj_pos(obj);
            obj->flags.alive = 1;
            obj->is_active = 1;
            break;
        }
        ++obj;
        ++i;
    }
}
