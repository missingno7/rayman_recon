/*
 * Bat enemy (GOG v1.21 object 1:0fa40-1:100b0). File name invented (Rayverse bat.c). Names: Rayverse/Android.
 * Uses the shared src/include/obj.h layout.
 * Worker candidate: eight STRICT functions and contiguous _TEXT layout.
 * DO_BAT_COMMAND remains NEAR; see NOTES.md and final_match.txt.
 */
#include "obj.h"

extern obj_t ray;
extern u8 horloge[25];
extern s16 bat_nb_frap;
extern u8 N_anim[256];
extern s16 ray_zdc_x;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern s16 ray_zdc_y;
extern u8 byte_CFA14;

void DO_REDEYE_FIRE(s16 x, s16 y, s16 direction);
u8 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void set_sub_etat(obj_t *obj, u8 sub_etat);
s16 test_allowed(obj_t *obj, s16 center_x, s16 center_y);
s16 on_block_chdir(obj_t *obj, s16 center_x, s16 center_y);
s16 myRand(s16 max);

#define GO_SPEED 20
#define Abs(value) ((s32)(value) >= 0 ? (s32)(value) : -(s32)(value))
#include "inline_helpers.h"

void DO_BAT_FLASH(s16 in_x, s16 in_y)
{
    s16 fire_par_3;
    s16 unk_1;
    s16 diff_x;
    s16 diff_y;
    u8 x_gt_0;
    u8 y_gt_0;

    diff_x = ray_zdc_x + (ray_zdc_w >> 1) - in_x;
    diff_y = ray_zdc_h + (ray_zdc_y >> 1) - in_y;
    x_gt_0 = diff_x > 0;
    y_gt_0 = diff_y > 0;

    if (!x_gt_0)
        diff_x = -diff_x;
    if (!y_gt_0)
        diff_y = -diff_y;

    if (diff_x > 200) {
        diff_x = ashr16(diff_x, 1);
        diff_y = ashr16(diff_y, 1);
    }

    if (diff_y == 0) {
        if (x_gt_0)
            fire_par_3 = 27;
        else if (diff_x != 0)
            fire_par_3 = 9;
        else
            fire_par_3 = -1;
    } else {
        unk_1 = diff_x * 45 / diff_y;
        if (x_gt_0) {
            if (y_gt_0) {
                if (unk_1 > 255)
                    fire_par_3 = 27;
                else
                    fire_par_3 = 27 + N_anim[unk_1];
            } else {
                if (unk_1 > 255)
                    fire_par_3 = 0;
                else
                    fire_par_3 = N_anim[unk_1];
            }
        } else if (y_gt_0) {
            if (unk_1 > 255)
                fire_par_3 = 18;
            else
                fire_par_3 = 18 + N_anim[unk_1];
        } else {
            if (unk_1 > 255)
                fire_par_3 = 9;
            else
                fire_par_3 = 9 + N_anim[unk_1];
        }
    }

    DO_REDEYE_FIRE(in_x, in_y, fire_par_3);
}

void DO_BAT_LEFT_FLASH(obj_t *obj)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;

    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

void DO_BAT_RIGHT_FLASH(obj_t *obj)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;

    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

void DO_BAT_LEFT_RIGHT_FLASH(obj_t *obj)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;

    GET_SPRITE_POS(obj, 15, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

s16 bat_dir(obj_t *obj)
{
    return (ray.x + ray.offset_bx - obj->x - obj->offset_bx) > 0;
}

void DO_BAT_COMMAND(obj_t *obj)
{
    s16 offs_by;
    s16 offs_bx;
    s16 obj_sub_etat;
    s32 speed;
    u8 horl_0;

    obj_sub_etat = obj->sub_etat;
    /* This conditional form preserves the DOS switch-table destinations. */
    horl_0 = 1;
    if (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0x0f] != 0)
        horl_0 = 0;

    switch (obj_sub_etat) {
    default:
        break;
    case 2:
        if (obj->anim_frame == 14 && horl_0)
            DO_BAT_LEFT_RIGHT_FLASH(obj);
        break;
    case 5:
        if (obj->anim_frame == 22) {
            if (myRand(1) == 0)
                skipToLabel(obj, 2, 1);
            else
                skipToLabel(obj, 3, 1);
            return;
        }
        break;
    case 3:
    case 4:
        if (obj->anim_frame == 19 && horl_0) {
            bat_nb_frap++;
            if (obj_sub_etat == 4)
                DO_BAT_RIGHT_FLASH(obj);
            if (obj_sub_etat == 3)
                DO_BAT_LEFT_FLASH(obj);
        }
        break;
    case 6:
    case 7:
        if (obj->anim_frame == 20) {
            switch (bat_nb_frap) {
            case 1:
                set_sub_etat(obj, 11);
                break;
            case 3:
                set_sub_etat(obj, 11);
                bat_nb_frap = 0;
                break;
            default:
                if (obj_sub_etat == 6)
                    skipToLabel(obj, 3, 1);
                else
                    skipToLabel(obj, 2, 1);
                break;
            }
        }
        break;
    case 8:
        if (bat_dir(obj))
            skipToLabel(obj, 4, 1);
        else
            skipToLabel(obj, 5, 1);
        break;
    case 11:
        if (bat_dir(obj))
            skipToLabel(obj, 6, 1);
        else
            skipToLabel(obj, 7, 1);
        break;
    case 9:
    case 10:
    case 12:
    case 13:
        if (obj->cmd == GO_SPEED) {
            switch (obj_sub_etat) {
            case 10:
            case 13:
                speed = obj->iframes_timer;
                obj->speed_x = Abs(speed);
                /* DOS stores the positive magnitude before negating it. */
                obj->speed_x = -obj->speed_x;
                break;
            case 12:
            case 9:
                speed = obj->iframes_timer;
                obj->speed_x = Abs(speed);
                break;
            }
            obj->speed_y = obj->follow_id;
        }

        offs_bx = obj->offset_bx;
        offs_by = obj->offset_by;
        if (test_allowed(obj, offs_bx, offs_by) &&
            on_block_chdir(obj, offs_bx, offs_by)) {
            switch (obj_sub_etat) {
            case 10:
                obj->sub_etat = 9;
                break;
            case 9:
                obj->sub_etat = 10;
                break;
            case 13:
                obj->sub_etat = 12;
                break;
            case 12:
                obj->sub_etat = 13;
                break;
            }
        }
        break;
    }
}

void BAT_ray_in_zone(obj_t *obj)
{
    if (obj->sub_etat == 0 && byte_CFA14 == 0) {
        bat_nb_frap = 0;
        obj->flags.flip_x = 0;
        skipToLabel(obj, 1, 1);
    }
}

void DO_BAT_POING_COLLISION(obj_t *bat_obj, s16 sprite)
{
}

s32 bat_get_eject_sens(obj_t *obj)
{
    ray.iframes_timer = 40;
    return bat_dir(obj) ? 1 : -1;
}
