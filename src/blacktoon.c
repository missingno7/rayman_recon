/*
 * Black toon enemy (GOG v1.21 object 1:13030-1:13800). File name invented (Rayverse blacktoon.c).
 * Names: Rayverse/Android. Status per function: config/units.json.
 */
#include "obj.h"
extern level_obj_t level_alw;
extern level_t level;
extern obj_t ray;
extern obj_t *poing_obj;
extern u8 block_flags[64];
extern s16 ray_zdc_x;
extern s16 ray_zdc_y;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;

#define TYPE_130_BLACKTOON_EYES 130
#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_WAIT 2
#define GO_SPEED 20
#define GO_NOP 30
#define true 1
#define false 0

void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
void calc_obj_pos(obj_t *obj);
void calc_obj_dir(obj_t *obj);
void add_alwobj(obj_t *obj);
s16 get_center_x(obj_t *obj);
s16 get_center_y(obj_t *obj);
s16 on_block_chdir(obj_t *obj, s16 center_x, s16 center_y);
s16 test_allowed(obj_t *obj, s16 center_x, s16 center_y);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void SET_X_SPEED(obj_t *obj);
u8 calc_typ_travd(obj_t *obj, u8 a2);
void CALC_MOV_ON_BLOC(obj_t *obj);
s32 sinYspeed(obj_t *obj, s16 a2, s16 a3, s16 *a4);
void obj_hurt(obj_t *obj);
void GET_ANIM_POS(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
s16 inter_box(s32 x1, s32 y1, s32 w1, s32 h1, s16 x2, s16 y2, s32 w2, s32 h2);

void allocateBlacktoonEyes(obj_t* obj) {
    s16 i;
    obj_t* cur_obj = level.objects + level_alw.obj_ids[0];

    for (i = 0; i < level_alw.nb_objects; ++i,
         cur_obj = level.objects + level_alw.obj_ids[i]) {
        if (cur_obj->type == TYPE_130_BLACKTOON_EYES && !cur_obj->is_active) {
            cur_obj->anim_frame = 0;
            cur_obj->x = obj->x;
            cur_obj->y = obj->y;
            cur_obj->speed_y = 0;
            cur_obj->speed_x = cur_obj->speed_y;
            set_main_and_sub_etat(cur_obj, cur_obj->init_main_etat, cur_obj->init_sub_etat);
            calc_obj_pos(cur_obj);
            calc_obj_dir(cur_obj);
            cur_obj->cmd_offset = -1;
            cur_obj->nb_cmd = 0;
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->flags.flag_0x40 = 0;
            break;
        }
    }
}

void DO_BLK_SPEED_COMMAND(obj_t* obj) {
    s16 cen_x; s16 cen_y;

    if (obj->config & 4) {
        cen_x = get_center_x(obj);
        cen_y = get_center_y(obj);
        if (on_block_chdir(obj, cen_x, cen_y) && test_allowed(obj, cen_x, cen_y))
            skipToLabel(obj, 99, true);

        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
        if (obj->speed_x > 0)
            obj->flags.flip_x = 1;
        else if (obj->speed_x < 0)
            obj->flags.flip_x = 0;
    }
    if (obj->config & 8) {
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
    }
}

void blkUTurn(obj_t* obj) {
    u8 label = 0xff;

    if (obj->cmd == GO_LEFT) {
        obj->flags.flip_x = 1;
        label = 3;
    } else if (obj->cmd == GO_RIGHT) {
        obj->flags.flip_x = 0;
        label = 2;
    }

    if (label != 0xff)
        skipToLabel(obj, label, 1);
}

void DO_BLK_LR_COMMAND(obj_t* obj) {
    s16 cen_x; s16 cen_y;

    if ((obj->main_etat == 0) && (obj->sub_etat == 4)) {
        SET_X_SPEED(obj);
        obj->speed_x = -obj->speed_x;
    } else {
        if (obj->cmd == GO_LEFT)
            obj->flags.flip_x = 0;
        else
            obj->flags.flip_x = 1;

        if (obj->config & 2) {
            SET_X_SPEED(obj);
            if (block_flags[calc_typ_travd(obj, false)] & 1) {
                if (obj->cmd == GO_LEFT)
                    skipToLabel(obj, 3, true);
                else
                    skipToLabel(obj, 2, true);
            }
            CALC_MOV_ON_BLOC(obj);
        }
        if (obj->config & 1) {
            obj->speed_y = sinYspeed(obj, 48, 40, &obj->follow_id);
            cen_x = get_center_x(obj);
            cen_y = get_center_y(obj);
            if (on_block_chdir(obj, cen_x, cen_y) && test_allowed(obj, cen_x, cen_y))
            {
                obj->x -= obj->speed_x;
                if (obj->cmd == GO_LEFT) {
                    skipToLabel(obj, 3, true);
                    obj->flags.flip_x = 1;
                } else {
                    skipToLabel(obj, 2, true);
                    obj->flags.flip_x = 0;
                }
            }
            SET_X_SPEED(obj);
        }
        if (obj->config & 0x10)
        {
            SET_X_SPEED(obj);
            obj->speed_y = 0;
        }
    }
}

void DO_BLK_NOP_COMMAND(obj_t* obj) {
    s32 condition;

    if (obj->config & 0x10 &&
        ((obj->main_etat == 0 && obj->sub_etat == 2) || (obj->main_etat == 2 && obj->sub_etat == 3))
    ) {
        if (obj->timer == 255) {
            condition = (obj->param >= obj->y + obj->speed_y) && obj->y >= obj->param;
            if (condition || obj->speed_y == 0) {
                obj->timer = 60;
            }
        } else if (obj->timer == 0) {
            set_main_and_sub_etat(obj, 2, 0);
            if (obj->flags.flip_x)
                skipToLabel(obj, 3, true);
            else
                skipToLabel(obj, 2, true);

            obj->speed_y = 0;
        } else {
            obj->speed_y = 0;
            obj->timer--;
        }
    }

    if ((obj->follow_sprite == 4) && (
            (obj->main_etat == 0 && obj->sub_etat == 2) || (obj->main_etat == 2 && obj->sub_etat == 3))
    ) {
        if (obj->detect_zone_flag != 0) {
            condition = (obj->param >= obj->y + obj->speed_y) && obj->y >= obj->param;
            if (condition) {
                set_main_and_sub_etat(obj, 0, 3);
                obj->speed_y = 0;
            }
        } else {
            set_main_and_sub_etat(obj, 2, 1);
        }
    }

    if (obj->config & 2 && obj->speed_x != 0 && block_flags[calc_typ_travd(obj, false)] & 1) {
        if (obj->cmd == GO_LEFT)
            skipToLabel(obj, 3, true);
        else
            skipToLabel(obj, 2, true);
    }
}

void DO_BLKTOON_COMMAND(obj_t* obj) {
    switch (obj->cmd) {
        case GO_SPEED:
            DO_BLK_SPEED_COMMAND(obj);
            break;
        case GO_LEFT:
        case GO_RIGHT:
            DO_BLK_LR_COMMAND(obj);
            break;
        case GO_NOP:
            DO_BLK_NOP_COMMAND(obj);
        case GO_WAIT:
        default:
            break;
    }
}

void DoBlackToonPoingCollision(obj_t* obj, s16 sprite) {
    obj_hurt(obj);
    if (obj->hit_points == 0) {
        set_main_and_sub_etat(obj, 0, 4);
        obj->speed_y = 0;
        if (poing_obj->speed_x >= 0) {
            if (poing_obj->speed_x > 0) {
                obj->flags.flip_x = 0;
            } else if (poing_obj->flags.flip_x) {
                obj->flags.flip_x = 0;
            } else {
                obj->flags.flip_x = 1;
            }
        } else {
            obj->flags.flip_x = 1;
        }
        allocateBlacktoonEyes(obj);
        if (obj->flags.flip_x) {
            skipToLabel(obj, 2, 1);
        } else {
            skipToLabel(obj, 3, 1);
        }
    }

}

void DoBlackToonRaymanZDD(obj_t* obj) {
    s32 ray_state_ok;

    switch (obj->follow_sprite) {
        case 7:
            if (obj->main_etat == 1 && obj->sub_etat == 1) {
                ray_state_ok = ray.main_etat != 2;
                if (ray_state_ok)
                    skipToLabel(obj, 5, true);
            }
            break;
        case 4:
            if (ray.main_etat != 2) {
                s16 blkt_x, blkt_y, blkt_w, blkt_h;
                GET_ANIM_POS(obj, &blkt_x, &blkt_y, &blkt_w, &blkt_h);
                blkt_y -= obj->detect_zone + blkt_h;
                blkt_x -= blkt_w >> 1;
                blkt_w += blkt_w;
                blkt_h += obj->detect_zone;
                if (obj->main_etat == 1 && obj->sub_etat == 0
                        && inter_box(blkt_x, blkt_y, blkt_w, blkt_h, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y)
                ) {
                    calc_obj_dir(obj);
                    obj->param = ray_zdc_y - obj->offset_hy;
                    if (obj->param < obj->y - 16) {
                        skipToLabel(obj, 4, true);
                        obj->gravity_value_1 = 0;
                        obj->gravity_value_2 = 6;
                        obj->y--;
                    }
                }
            }
            break;
        case 3:
            if (obj->main_etat == 0 && obj->sub_etat == 0) {
                ray_state_ok = ray.main_etat != 2;
                if (ray_state_ok) {
                calc_obj_dir(obj);
                obj->param = ray.y + ray.offset_hy - obj->offset_hy;
                if (obj->param < obj->y - 16) {
                    skipToLabel(obj, 4, true);
                    obj->gravity_value_2 = 6;
                    obj->gravity_value_1 = 0;
                    obj->timer = 255;
                    obj->y--;
                }
                }
            }
            break;
        case 2:
            if (obj->main_etat == 0 && obj->sub_etat == 0) {
                ray_state_ok = ray.main_etat != 2;
                if (ray_state_ok) {
                calc_obj_dir(obj);
                skipToLabel(obj, 4, true);
                obj->gravity_value_1 = 0;
                obj->gravity_value_2 = 6;
                obj->y--;
                }
            }
            break;
    }
}
