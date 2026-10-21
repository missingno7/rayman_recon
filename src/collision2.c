/*
 * Collision engine, second object (GOG v1.21 1:1f1e0-1:21840). File name invented (Rayverse collision.c).
 * Names: Rayverse/Android. Uses the shared ORIGINAL_INLINE ashr16 helper at its two DOS sites.
 * Status per function: config/units.json.
 */
#include "collision2_local.h"

//2F1E0
s16 test_allowed(obj_t* obj, s16 center_x, s16 center_y) {
    /* 487A4 8016CFA4 -O2 -msoft-float */
    s16 res = false;
    s16 map_index =
            ((center_x + obj->x) >> 4) +
            ((center_y + obj->y) >> 4) * mp.width;
    if (map_index != (s16)obj->test_block_index) {
        obj->test_block_index = map_index;
        res = true;
    }
    return res;
}

//2F224
void obj_jump(obj_t* obj) {
    /* 4880C 8016D00C -O2 -msoft-float */
    s16 spd_y;
    s16 gravity_value_2 = 0;
    s16 spd_x = obj->speed_x;
    u8 label = 255;

    switch (obj->type) {
        case TYPE_172_STONEWOMAN:
            if (get_eta(obj)->flags & 1) {
                label = 15;
                set_main_and_sub_etat(obj, 2, 3);
                spd_y = -3;
                gravity_value_2 = 5;
            }
            break;
        case TYPE_123_BLACKTOON1:
            if (obj->config & 2) {
                label = 1;
                set_main_and_sub_etat(obj, 2, 5);
                spd_y = -3;
            }
            break;
    }

    if (label != 255) {
        skipToLabel(obj, label, true);
        obj->speed_x = spd_x;
        obj->speed_y = spd_y;
        obj->gravity_value_1 = 0;
        obj->gravity_value_2 = gravity_value_2;
        obj->y -= 12;
        calc_btyp(obj);
    }
}

//2F2FC
void DO_MOVING_PLATFORM_COMMAND(obj_t* obj) {
    /* 49320 8016DB20 -O2 */
    s16 y_cen;
    s16 x_cen;
    s16 chdir;

    x_cen = get_center_x(obj);
    y_cen = get_center_y(obj);
    chdir = on_block_chdir(obj, x_cen, y_cen);
    /* TODO: remove cast once sure of test_allowed() return */
    if (chdir && (s16) test_allowed(obj, x_cen, y_cen))
        skipToLabel(obj, 99, true);

    obj->flags.flip_x = 0;
    if (obj->cmd == GO_SPEED) {
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
    }
}

//2F378
void DoPTGRAPPINPoingCollision(obj_t* obj, s16 sprite) {
    s32 angle;
    extern s16 ANGLE_RAYMAN(obj_t *);
    if (RayEvts.grap && ray_mode != MODE_3_MORT_DE_RAYMAN) {
        if (Abs((ray.offset_by + ray.y) - obj->y - obj->offset_by) < 250) {
            SET_RAY_BALANCE();
            id_obj_grapped = obj->id;
            angle = ANGLE_RAYMAN(obj);
            obj->follow_x = angle;
            if (angle > 256 || (angle == 256 && !ray.flags.flip_x))
                obj->link = -1;
            else
                obj->link = 1;

            if (obj->main_etat == 0 && obj->sub_etat == 0) {
                skipToLabel(obj, 0, true);
            }

            poing.is_active = false;
            poing.is_returning = false;
            poing_obj->is_active = 0;
            poing_obj->flags.alive = 0;
            fin_poing_follow(false);
        }
    }
}

//2F44C
void DO_ONE_CMD_WAIT(obj_t* obj) {
    if (obj->main_etat == 1) {
        obj->speed_y = 0;
        obj->speed_x = 0;
        set_main_and_sub_etat(obj, 0, 0);
    } else if (obj->type == TYPE_10_FISH && obj->main_etat == 0 && obj->sub_etat == 0) {
        obj->speed_y = 0;
    }
}

//2F488
void DO_ONE_CMD_LR_ATTENTE(obj_t* obj) {
    /* 4956C 8016DD6C -O2 */
    s16 sub_etat = obj->sub_etat;
    s16 type = obj->type;

    if (!(sub_etat == 1 || sub_etat == 2 || sub_etat == 11 || sub_etat == 22 ||
          sub_etat == 3 || sub_etat == 9 || sub_etat == 4 || sub_etat == 5 ||
          sub_etat == 6 || sub_etat == 15 || sub_etat == 16 || sub_etat == 17)
    ) {
        if (!(type == TYPE_CHASSEUR1 || type == TYPE_CHASSEUR2))
            set_main_and_sub_etat(obj, 1, 0);

        SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
    }
    else if (sub_etat == 3 || sub_etat == 6) {
        obj->flags.read_commands = 0;
        obj->speed_y = -8;
    } else if (sub_etat == 11 || sub_etat == 2 || sub_etat == 22) {
        switch (type) {
            case TYPE_CHASSEUR1:
            case TYPE_CHASSEUR2:
                DO_TIR(obj);
                break;
            case TYPE_BIG_CLOWN:
                DO_BIG_CLOWN_ATTAK(obj);
                break;
            case TYPE_WAT_CLOWN:
                DO_WAT_CLOWN_ATTAK(obj);
                break;
        }
    }
}

//2F594
void DO_ONE_CMD_UPDOWN(obj_t* obj) {
    switch (obj->type) {
    case TYPE_10_FISH:
        if (obj->main_etat == 0 && obj->sub_etat == 0) {
            if (obj->cmd == GO_UP)
                obj->speed_y = -2;
            else
                obj->speed_y = 2;
        }
        break;
    case TYPE_1_PLATFORM:
        if (obj->cmd == GO_UP)
            obj->speed_y = -2;
        else if (obj->cmd == GO_DOWN)
            obj->speed_y = 2;
        break;
    case TYPE_24_INTERACTPLT:
        if (obj->cmd != GO_WAIT) {
            --obj->nb_cmd;
            if (obj->nb_cmd > 0) {
                if (obj->cmd == GO_UP)
                    obj->speed_y = -1;
                else if (obj->cmd == GO_DOWN)
                    obj->speed_y = 1;
            } else {
                obj->speed_y = 0;
                obj->nb_cmd = 0;
                obj->init_y = obj->y;
                obj->cmd = GO_WAIT;
            }
        }
        break;
    }
}

//2F63C
void special_pour_liv(obj_t* event) {
    if (event->type == TYPE_0_BADGUY1 && event->main_etat == 1 && event->sub_etat == 11) {
        event->flags.read_commands = false;
    }
}

//2F658
void DO_ONE_CMD(obj_t* obj) {
    /* 49844 8016E044 -O2 */
    s16 etat = obj->main_etat;
    special_pour_liv(obj);
    switch (obj->cmd) {
        case GO_WAIT:
            DO_ONE_CMD_WAIT(obj);
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT) {
                obj->flags.flip_x = 0;
            } else {
                obj->flags.flip_x = 1;
            }

            if (etat == 1) {
                SET_X_SPEED(obj);
                CALC_MOV_ON_BLOC(obj);
            }
            else if (etat == 2) {
                SET_X_SPEED(obj);
            } else if (etat == 0) {
                DO_ONE_CMD_LR_ATTENTE(obj);
            }
            break;
        case GO_UP:
        case GO_DOWN:
            DO_ONE_CMD_UPDOWN(obj);
            break;
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            break;
        case GO_NOP:
            break;
    }
}

//2F714
void DO_ROLL_EYES(obj_t* obj) {
    /* 4BC40 80170440 -O2 -msoft-float */
    s16 diff_x, diff_y;
    s16 screen_x, screen_y;
    s16 unk_x_1, unk_y_1;
    s16 fire_par_3;
    u8 y_gt_0, x_gt_0;
    extern void DO_REDEYE_FIRE(s16, s16, s16);

    if (obj->type == TYPE_145_KILLING_EYES) {
        switch (obj->sub_etat) {
        case 0:
            if (--obj->iframes_timer == 0) { obj->sub_etat = 3; obj->iframes_timer = 30; }
            break;
        case 2:
            if (--obj->iframes_timer == 0) { obj->sub_etat = 0; obj->iframes_timer = obj->param; obj->param = 100; }
            break;
        case 1:
            if (--obj->iframes_timer == 0) { obj->sub_etat = 2; obj->iframes_timer = 5; }
            break;
        case 5:
            if (--obj->iframes_timer == 0) { obj->sub_etat = 0; obj->iframes_timer = obj->param; obj->param = 100; }
            break;
        }
        obj->change_anim_mode = ANIMMODE_NONE;
    }
    diff_x = ray.x - obj->x;
    diff_y = ray.y - obj->y;
    unk_x_1 = diff_x + 0x20;
    unk_y_1 = diff_y + 0x19;
    if (obj->type == TYPE_145_KILLING_EYES)
    {
        unk_x_1 += 20;
        unk_y_1 += 10;
    }
    x_gt_0 = unk_x_1 > 0;
    y_gt_0 = (unk_y_1 > 0);
    if (x_gt_0 == 0)
    {
        unk_x_1 = -unk_x_1;
    }
    if (y_gt_0 == 0)
    {
        unk_y_1 = -unk_y_1;
    }
    if (unk_x_1 > 200)
    {
        unk_x_1 = ashr16(unk_x_1, 1);
        unk_y_1 = ashr16(unk_y_1, 1);
    }

    if (unk_y_1 == 0)
    {
        if (x_gt_0)
        {
            fire_par_3 = 0x1B;
        }
        else
        {
            if (unk_x_1 != 0)
            {
                fire_par_3 = 9;
            }
            else
                fire_par_3 = -1;
        }
    }
    else
    {
        fire_par_3 = unk_x_1 * 0x2D / unk_y_1;
        if (x_gt_0)
        {
            if (y_gt_0)
            {
                if (fire_par_3 > 255)
                {
                    fire_par_3 = 0x1B;
                }
                else
                {
                    fire_par_3 = N_anim[fire_par_3] + 0x1B;
                }
            }
            else
            {
                if (fire_par_3 > 255)
                {
                    fire_par_3 = 0;
                }
                else
                {
                    fire_par_3 = N_anim[fire_par_3];
                }
            }
        }
        else if (y_gt_0)
        {
            if (fire_par_3 > 255)
            {
                fire_par_3 = 0x12;
            }
            else
            {
                fire_par_3 = N_anim[fire_par_3] + 0x12;
            }
        }
        else
        {
            if (fire_par_3 > 255)
            {
                fire_par_3 = 9;
            }
            else
            {
                fire_par_3 = N_anim[fire_par_3] + 9;
            }
        }
    }
    if (obj->sub_etat == 3)
    {
        if ((--obj->iframes_timer) == 0)
        {
            obj->sub_etat = 1;
            screen_x = obj->screen_x;
            obj->iframes_timer = 60;
            screen_y = obj->screen_y;
            if (
                    (screen_x > -52) && (screen_y > -36) &&
                    (screen_x < 0x131) && (screen_y < 195)
                    )
            {
                DO_REDEYE_FIRE(obj->x, obj->y, fire_par_3);
            }
            else {
                obj->param = 40;
            }
        }
    }
    if ((fire_par_3) != -1)
    {
        obj->anim_frame = fire_par_3 - 1;
    }
}

//2F978
void DoKillingEyesPoingCollision(obj_t* obj, s16 sprite) {
    --obj->hit_points;
    if (obj->hit_points != 0) {
        set_sub_etat(obj, 5);
        obj->iframes_timer = 50;
    } else {
        set_sub_etat(obj, 6);
    }
}

//2F9B8
s32 YaUnBloc(obj_t *obj, s16 dir) {
    s16 i;
    s16 unk_y;
    s32 res = false;

    switch (dir) {
        case 2:
            if (block_flags[mp.map[((obj->x + obj->offset_bx - 8) >> 4) + (obj->y >> 4) * mp.width].tile_type] & 1)
                res = true;
            if (block_flags[mp.map[((obj->x + obj->offset_bx + 7) >> 4) + mp.width * (obj->y >> 4)].tile_type] & 1)
                res = true;
            break;
        case 3:
            if (block_flags[mp.map[((obj->x + obj->offset_bx - 8) >> 4) + ((obj->y + 154) >> 4) * mp.width].tile_type] & 1)
                res = true;
            if (block_flags[mp.map[((obj->x + obj->offset_bx + 7) >> 4) + mp.width * (obj->y >> 4)].tile_type] & 1)
                res = true;
            break;
        case 0:
            unk_y = 6;
            i = 0;
            while (i < 10) {
                if (block_flags[mp.map[((obj->x + obj->offset_bx - 16) >> 4) + mp.width * ((obj->y + unk_y) >> 4)].tile_type] & 1)
                    res = true;
                unk_y += 16;
                i++;
            }
            break;
        case 1:
            unk_y = 6;
            i = 0;
            while (i < 10) {
                if (block_flags[mp.map[((obj->x + obj->offset_bx + 16) >> 4) + mp.width * ((obj->y + unk_y) >> 4)].tile_type] & 1)
                    res = true;
                unk_y += 16;
                i++;
            }
            break;
    }
    return res;
}

//2FBF4
void DO_BAG1_COMMAND(obj_t* obj) {
    /* 4C2C8 80170AC8 -O2 */
    DO_ONE_CMD(obj);
    if (obj->sub_etat == 4 && obj->anim_frame == 14 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0 &&
            obj->screen_x > -160 && obj->screen_x < 250 &&
            obj->screen_y < SCREEN_HEIGHT - 100
    ) {
        obj->y += 86;
        allocateLandingSmoke(obj);
        obj->y -= 86;
    }
}

//2FC64
void DoRaymanBag1Collision(obj_t* obj) {
    if (!RayEvts.squashed && scroll_x == -1 && scroll_y == -1 && (ray.main_etat == 0 || ray.main_etat == 1)) {
        Ray_RayEcrase(); //TODO
    }
}

//2FCB0
s16 MarCoince(obj_t* obj, s16 dir) {
    /* 4CB88 80171388 -O2 -msoft-float */
    s32 map_ind;
    s32 pos_to_check;
    s16 res = false;
    s16 y;
    s16 x;
    x = obj->x + obj->offset_bx - 25;
    y = obj->y + obj->offset_hy;

    switch (dir)
    {
        case 2:
            pos_to_check = y % 16;
            if (pos_to_check < 3)
            {
                map_ind = (x >> 4) + mp.width * (y >> 4);
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
            }
            break;
        case 3:
            pos_to_check = y % 16;
            if (pos_to_check > 12)
            {
                map_ind = (x >> 4) + mp.width * ((y + 16 * 3) >> 4);
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[++map_ind].tile_type] & 2)
                    res = true;
            }
            break;
        case 0:
            pos_to_check = x % 16;
            if (pos_to_check < 3)
            {
                map_ind = ((x >> 4) + mp.width * ((y + 16) >> 4)) - 1;
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
            }
            break;
        case 1:
            pos_to_check = x % 16;
            if (pos_to_check > 12)
            {
                map_ind = ((x >> 4) + mp.width * ((y + 16) >> 4)) + 3;
                if (block_flags[mp.map[map_ind].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
                if (block_flags[mp.map[map_ind += mp.width].tile_type] & 2)
                    res = true;
            }
            break;
    }

    return res;
}

//2FFDC
void DO_MOVE_MARACAS_COMMAND(obj_t* obj) {
    /* 4CF4C 8017174C -O2 */
    s16 cen_x;
    s16 cen_y;
    s16 speed_x;
    s16 diff_x;
    s16 diff_x_inv;
    u8 diff_x_less;

    if (obj->main_etat == 0 && obj->sub_etat == 0x0C)
    {
        cen_x = get_center_x(obj);
        cen_y = get_center_y(obj);
        if ((s16) on_block_chdir(obj, cen_x, cen_y) && (s16) test_allowed(obj, cen_x, cen_y))
            skipToLabel(obj, 99, true);

        obj->flags.flip_x = 0;
        if (obj->cmd == 0x14)
            obj->speed_y = obj->follow_id;

        speed_x = 0;
        if (ray.follow_id == obj->id)
        {
            diff_x = (ray.offset_bx + ray.x) - (obj->offset_bx + obj->x);
            if (diff_x > 0)
            {
                if (diff_x > 10)
                    speed_x = 1;
                if (diff_x > 15)
                    speed_x += 1;
                if (diff_x > 18)
                    speed_x += 1;
                if (diff_x > 42) speed_x = 0;
            }
            else
            {
                diff_x_inv = -diff_x;
                if (diff_x_inv > 10)
                    speed_x = -1;
                if (diff_x_inv > 15)
                    speed_x -= 1;
                if (diff_x_inv > 18)
                    speed_x -= 1;
                if (diff_x_inv > 41) speed_x = 0;
            }

        }
        obj->speed_x = speed_x;
        if (obj->speed_y < 0)
        {
            if (MarCoince(obj, 2))
            {
                obj->speed_y = 0;
                obj->param++;
                if (obj->param > 240)
                {
                    obj->y -= 80;
                    allocateExplosion(obj);
                    PlaySnd(0x73, obj->id);
                    obj->x = obj->init_x;
                    obj->y = obj->init_y;
                    set_main_and_sub_etat(obj, 0, 4);
                    obj->speed_x = 0;
                    obj->speed_y = 0;
                    DO_NOVA(obj);
                }
            }
            else
                obj->param = 0;
        }
        if (obj->speed_x > 0)
        {
            if (MarCoince(obj, 1))
                obj->speed_x = 0;
        }
        else if (obj->speed_x < 0)
        {
            if (MarCoince(obj, 0))
                obj->speed_x = 0;
        }
    }
}

//301A4
void DO_FLASH_COMMAND(obj_t* obj) {
    /* 4D1F4 801719F4 -O2 -msoft-float */
    obj->init_x += obj->iframes_timer;
    while (Abs((s32)obj->init_x) >= 128) {
        if (obj->iframes_timer > 0) {
            obj->x++;
            obj->init_x -= 128;
        } else {
            obj->x--;
            obj->init_x += 128;
        }
    }

    obj->init_y = obj->init_y + obj->follow_id;
    while (Abs((s32)obj->init_y) >= 128) {
        if (obj->follow_id > 0) {
            obj->y++;
            obj->init_y -= 128;
        } else {
            obj->y--;
            obj->init_y += 128;
        }
    }

    calc_obj_pos(obj);
    obj->anim_frame = obj->hit_points - 1;
}

//30260
void DO_WLKNOT_COMMAND(obj_t* obj) {
    /* 4D7DC 80171FDC -O2 */
    s16 bx; s16 by;

    switch (obj->cmd) {
        case GO_WAIT:
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = false;
            else
                obj->flags.flip_x = true;
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
            break;
    }

    if (obj->speed_x != 0) {
        bx = obj->offset_bx;
        by = obj->offset_by - 8;
        if ((s16) test_allowed(obj, bx, by) && (s16) on_block_chdir(obj, bx, by)) {
            if (!(obj->flags.flip_x)) {
                obj->flags.flip_x = true;
                skipToLabel(obj, 2, true);
            } else {
                obj->flags.flip_x = false;
                skipToLabel(obj, 1, true);
            }
        }
    }
}

//30318
void ACTIVE_L_EAU(obj_t* eau_obj) {
    /* 4D908 80172108 -O2 -msoft-float */
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;

    eau_obj->y = ymapmax + (SCREEN_HEIGHT - 10);
    eau_obj->x = xmap - eau_obj->offset_bx;
    eau_obj->param = 130;
    i = 0;
    cur_obj = level.objects;
    nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (cur_obj->type == TYPE_157_EAU && !cur_obj->is_active) {
            cur_obj->flags.flip_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->speed_x = 0;
            cur_obj->x = eau_obj->x + 101 * 1;
            cur_obj->y = eau_obj->y;
            calc_obj_pos(cur_obj);
            cur_obj->is_active = 1;
    cur_obj->param = 130;
    cur_obj->flags.alive = 1;
            cur_obj->sub_etat = 1;
            break;
        }
        cur_obj++;
        i++;
    }
    cur_obj++;
    cur_obj->flags.flip_x = 0;
    cur_obj->speed_y = 0;
    cur_obj->speed_x = 0;
    cur_obj->x = eau_obj->x + 101 * 2;
    cur_obj->y = eau_obj->y;
    calc_obj_pos(cur_obj);
    cur_obj->is_active = 1;
    cur_obj->param = 130;
    cur_obj->flags.alive = 1;
    cur_obj->sub_etat = 2;

    cur_obj++;
    cur_obj->flags.flip_x = 0;
    cur_obj->speed_y = 0;
    cur_obj->speed_x = 0;
    cur_obj->x = eau_obj->x + 101 * 3;
    cur_obj->y = eau_obj->y;
    calc_obj_pos(cur_obj);
    cur_obj->is_active = 1;
    cur_obj->param = 130;
    cur_obj->flags.alive = 1;
    cur_obj->sub_etat = 3;
}

//30490
void DO_EAU_QUI_MONTE(obj_t* obj) {
    s32 stone_id;

    if ((ray.flags.alive) && !(ray.main_etat == 3 && ray.sub_etat == 23)) {
        switch (num_world)
        {
            case 1:
                switch (obj->hit_points) {
                case 1:
                    obj->hit_points--;
                    ACTIVE_L_EAU(obj);
                    break;
                case 0:
                    if (obj->param > 40 || level.objects[eau_obj_id].iframes_timer == 1) {
                        if (horloge[2] != 0 && ray.flags.alive && obj->y > (SCREEN_HEIGHT - 80)) {
                            obj->speed_y = -1;
                        } else {
                            obj->speed_y = 0;
                        }
                        {
                            s16 countdown = obj->param;
                            if (countdown > 0)
                                obj->param = countdown - 1;
                        }
                    } else {
                        obj->speed_y = 0;
                    }
                    break;
                default:
                    obj->hit_points--;
                    break;
                }
                break;
            case 3:
                stone_id = pierreAcorde_obj_id;
                if (stone_id == -1) {
                    if (scroll_y != -1) {
                        obj->init_y = ymap + (SCREEN_HEIGHT - 80);
                        if (obj->y - 100 > obj->init_y) {
                            obj->y -= 3;
                        }
                        --obj->y;
                        if (obj->y > obj->init_y && horloge[2] != 0) {
                            obj->y -= 1; // NOTE: changed from 2 to 1 in PC version
                        }
                        obj->iframes_timer = 1;
                    } else {
                        if (obj->iframes_timer == 1) {
                            s16 ray_bottom = ray.y + ray.offset_by;
                            obj->init_y = ray_bottom - 30;
                            obj->iframes_timer = 0;
                        }
                        if (obj->init_y < obj->y) {
                            obj->y--;
                        }
                    }
                } else if ((level.objects[stone_id].hit_points > 0) && (obj->y < (ymapmax + (SCREEN_HEIGHT - 80)))) {
                    obj->sub_speed += 14;
                    if (obj->sub_speed > 16) {
                        obj->sub_speed -= 16;
                        ++obj->y;
                    }
                }
                scroll_end_y = obj->y - 120;
            case 4:
            case 5:
                if (obj->screen_x < -166) {
                    obj->x += 505;
                    if (++obj->sub_etat > 3)
                        obj->sub_etat = 0;
                    if (num_level == 8 && num_world == 5) {
                        obj->init_y -= num_world;
                        obj->y = obj->init_y;
                    }
                } else if (obj->screen_x > 366) {
                    obj->x -= 505;
                    if (--obj->sub_etat > 3) obj->sub_etat = 3;
                    if (num_level == 8 && num_world == 5) {
                        obj->init_y += num_world;
                        obj->y = obj->init_y;
                    }
                }
                if (num_level == 8 && num_world == 5)
                    obj->y = ymapmax + Bloc_lim_H2 - 80;
                break;
        }
    } else {
        obj->speed_y = 0;
        obj->speed_x = 0;
    }
    if (obj->y < (ymap + Bloc_lim_H2 - 80)) {
        obj->y = ymap + Bloc_lim_H2 - 80;
    }
}

//3076C
void allocateOtherPosts(obj_t* her_bh_obj) {
    obj_t *cur_obj;
    s16 cur_id = link_init[her_bh_obj->id];
    u8 her_bh_frames = her_bh_obj->animations[get_eta(her_bh_obj)->anim_index].frames_count;
    s16 i = 0;

    while (cur_id != her_bh_obj->id) {
        i++;
        cur_obj = &level.objects[cur_id];
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
        cur_obj->anim_frame = (her_bh_obj->anim_frame + i) % her_bh_frames;
        cur_obj->init_x = her_bh_obj->init_x + i * 32 * (her_bh_obj->hit_points - 2);
        cur_obj->init_y = her_bh_obj->init_y;
        cur_obj->param = her_bh_obj->param - i * 256;
        cur_obj->x = cur_obj->init_x;
        cur_obj->y = cur_obj->init_y;
        calc_obj_pos(her_bh_obj);
        cur_id = link_init[cur_id];
    }
}

//3088C
void doHerseCommand(obj_t* obj) {
    /* 4EF0C 8017370C -O2 -msoft-float */
    if ((obj->type == TYPE_178_HERSE_BAS || obj->type == TYPE_HERSE_HAUT) && obj->link == 0) {
        obj->anim_frame = myRand((s16)(obj->animations[obj->anim_index].frames_count - 1));
        obj->param = 0x1000;
        allocateOtherPosts(obj);
        obj->link = 1;
    } else {
        obj->speed_y = sinYspeed(obj, 24, 40, &obj->param);
        obj->speed_x = 0;
    }
}

//30908
void doBlackRaymanCommand(obj_t* br_obj) {
    extern s16 firstFloorBelow(obj_t *);
    s32 scale;
    obj_t *fist_obj;
    ray_stack_t *br_stack;

    if (br_obj->main_etat == 0 && br_obj->sub_etat == 54) {
        if (EOA(br_obj)) {
            br_obj->is_active = 0;
            br_obj->flags.alive = 0;
        } else if (br_obj->anim_frame == 0) {
            DO_NOVA(br_obj);
        }
        DO_ANIM(br_obj);
        return;
    }

    if (
        (ray.main_etat == 3 && ray.sub_etat == 23) ||
        (ray_mode == MODE_3_MORT_DE_RAYMAN && !(ray.main_etat == 2 && ray.sub_etat == 31))
    ) {
        if (firstFloorBelow(br_obj) - br_obj->offset_by - br_obj->y <= 32) {
            br_obj->y = firstFloorBelow(br_obj) - br_obj->offset_by;
            set_main_and_sub_etat(br_obj, 0, 54);
            br_obj->anim_index = 107;
            br_obj->anim_frame = 0;
        } else {
            br_obj->is_active = 0;
            br_obj->flags.alive = 0;
        }
        br_obj->speed_x = 0;
        br_obj->speed_y = 0;
        DO_NOVA(br_obj);
        fist_obj = &level.objects[black_fist_obj_id];
        fist_obj->is_active = 0;
        fist_obj->flags.alive = 0;
        return;
    }

    if (ray_stack_is_full) {
        if (black_pos_in_stack == ray_pos_in_stack - 1) {
            fist_obj = &level.objects[black_fist_obj_id];
            br_obj->flags.alive = 0;
            fist_obj->flags.alive = 0;
            DO_NOVA(br_obj);
            if (fist_obj->is_active) {
                DO_NOVA(fist_obj);
            }
        } else {
            br_stack = &rayStack[black_pos_in_stack];
            br_obj->x = br_stack->x_pos;
            br_obj->y = br_stack->y_pos;
            br_obj->main_etat = br_stack->main_etat;
            br_obj->sub_etat = br_stack->sub_etat;
            br_obj->anim_index = br_stack->anim_index;
            br_obj->anim_frame = br_stack->anim_frame;
            br_obj->flags.flip_x = br_stack->flip_x;
            scale = br_stack->scale;
            br_obj->scale = scale;
            br_obj->speed_x = 0;
            br_obj->speed_y = 0;
            if (br_obj->display_prio == 0) {
                DO_NOVA(br_obj);
            }

            br_obj->display_prio = 4;
            fist_obj = &level.objects[black_fist_obj_id];
            if (br_stack->poing_is_active) {
                fist_obj->display_prio = 4;
                fist_obj->flags.alive = 1;
                fist_obj->x = br_stack->poing_x_pos;
                fist_obj->y = br_stack->poing_y_pos;
                fist_obj->anim_index = br_stack->poing_anim_index;
                fist_obj->anim_frame = br_stack->poing_anim_frame;
                fist_obj->flags.flip_x = br_stack->poing_flip_x;
                fist_obj->speed_x = 0;
                fist_obj->speed_y = 0;
            } else {
                fist_obj->flags.alive = 0;
            }

            black_pos_in_stack++;
            if (black_pos_in_stack > LEN(rayStack) - 1) {
                black_pos_in_stack = 0;
            }
        }
    } else {
        br_obj->display_prio = 0;
        level.objects[black_fist_obj_id].flags.alive = 0;
    }
}

//30BE4
void doBlKRayRaymanCollision(obj_t* obj) {
    if (ray_stack_is_full == 1) {
        ray.hit_points = 0;
    }
}

//30BF8
void DO_POELLE_COMMAND(obj_t* po_obj) {
    /* 4F818 80174018 -O2 -msoft-float */
    extern u8 RAY_DEAD(void);
    s32 abs_ray_spd_x;
    s16 po_y; s16 po_x; s16 po_h; s16 po_w;
    s32 sub_etat;

    po_obj->flags.flip_x = 1;
    if (ray_on_poelle == true) {
        po_obj->flags.follow_enabled = 0;
        if ((ray.speed_x > 0 && ray.flags.flip_x) || (ray.speed_x < 0 && !ray.flags.flip_x)) {
            if (Abs((s32)ray.speed_x) < 2)
                po_obj->param = ray.speed_x > 0 ? 2 : -2;
            else if (Abs((s32)ray.speed_x) >= 7)
                po_obj->param = ray.speed_x > 0 ? 7 : -7;
            else
                po_obj->param = ray.speed_x;
        }
        if (po_obj->iframes_timer == 0)
        {
            if (Abs((s32)ray.speed_y) <= 0) /* ray.speed_y == 0 */
            {
                if (Abs((s32)ray.speed_x) > 2)
                    po_obj->anim_frame = 1;
            }
            else if (ray.speed_y > 0)
            {
                if (ray.speed_x > 0)
                    po_obj->anim_frame = 0;
                else if (ray.speed_x < 0)
                    po_obj->anim_frame = 2;
                else
                    po_obj->anim_frame = 1;
            }
            else if (ray.speed_y < 0)
            {
                if (ray.speed_x > 0)
                    po_obj->anim_frame = 2;
                else if (ray.speed_x < 0)
                    po_obj->anim_frame = 0;
                else
                    po_obj->anim_frame = 1;
            }

            po_obj->iframes_timer = 8;
        }
        else
            po_obj->iframes_timer--;

        GET_SPRITE_POS(po_obj, 0, &po_x, &po_y, &po_w, &po_h);
        po_y = (po_y + po_obj->offset_hy) - (po_obj->y + po_obj->offset_by);
        po_obj->x = (ray.offset_bx + ray.x) - po_obj->offset_bx;
        po_obj->y = (ray.offset_by + ray.y) - po_obj->offset_by - po_y;
        if (((ray.main_etat == 3 && ray.sub_etat == 23) || (ray.main_etat == 3 && ray.sub_etat == 22)) || !RAY_DEAD()) {
            RayEvts = SauveRayEvts;
            ray_on_poelle = false;
        }
    } else {
        po_obj->flags.follow_enabled = 1;
        po_obj->iframes_timer = 0;
        po_obj->anim_frame = 1;
        if (ray.main_etat == 3 && ray.sub_etat == 22)
            po_obj->speed_x = po_obj->param;
        else if (
                (ray.main_etat == 2 && ray.sub_etat == 1) ||
                (ray.main_etat == 2 && ray.sub_etat == 2) ||
                (ray.main_etat == 2 && ray.sub_etat == 6)
                )
            ray.speed_x = 0;
        else
            po_obj->param = 0;
    }
}

//30EA8
void DO_CORDE_COMMAND(obj_t* obj) {
    /* 4FB58 80174358 -O2 -msoft-float */
    extern s16 inter_box(s32, s32, s32, s32, s16, s16, s32, s32);
    extern void DO_FUMEE_CORDE(s16, s16);
    s32 pa_id;
    s16 bVar2;
    s16 obj_x_pos;
    s16 obj_y_pos;
    s16 ray_x_pos;
    s16 sVar4;
    s16 ray_y_pos;
    u8 obj_y_offs;
    u8 obj_h;
    s16 inter;
    u8 hp;

    pa_id = pierreAcorde_obj_id;
    if (pa_id != -1)
    {
        if (obj->main_etat == 0)
        {
            if (obj->x < level.objects[pa_id].x)
                obj->y = level.objects[pa_id].y + 112;
            else
                obj->y = level.objects[pa_id].y + 120;
        }

        if (ray.main_etat == 2 && ray.sub_etat == 15)
        {
            /* TODO: ... */

            obj_x_pos = obj->x;
            obj_y_pos = obj->y;
            bVar2 = ray.x;
            ray_x_pos = bVar2;
            ray_y_pos = ray.y;
            bVar2 = true;
            switch (obj->sub_etat)
            {
                case 16:
                    obj_y_offs = 48;
                    obj_h = 24;
                    break;
                case 17:
                    obj_y_offs = 48;
                    obj_h = 32;
                    break;
                case 18:
                case 19:
                    obj_y_offs = 56;
                    obj_h = 40;
                    break;
                case 20:
                    obj_y_offs = 56;
                    obj_h = 64;
                    break;
                default:
                    bVar2 = false;
                    break;
            }
            if (bVar2 && inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h)) {
                if (--obj->hit_points < 1)
                {
                    obj->hit_points = obj->init_hit_points;
                    if (obj->sub_etat < 19)
                        set_sub_etat(obj, obj->sub_etat + 1);
                    else
                        DO_CORDE_CASSE(obj);
                }

                if (level.objects[pierreAcorde_obj_id].iframes_timer == 0)
                    DO_FUMEE_CORDE(obj_x_pos + 128, ray_y_pos + 30);

                level.objects[pierreAcorde_obj_id].param = 0;
                return;
            }
        }
        level.objects[pierreAcorde_obj_id].param++;
    }
}

//31060
void DO_PAC_COMMAND(obj_t* obj) {
    /* 4FDF4 801745F4 -O2 -msoft-float */
    if (obj->hit_points == 0) {
        if (obj->x < (ray.x + 47) && (obj->x + 186) > ray.x) {
            obj->hit_points = 10;
            set_main_and_sub_etat(obj, 2, 4);
        }
    }

    if (obj->hit_points > 1) {
        if (obj->speed_y < 16) {
            if (horloge[8] == 0)
                obj->speed_y++;
        } else if (horloge[8] == 0 && horloge[3] == 0) { // NOTE: changed from the PS1 to the PC version
            obj->y++;
        }
    } else {
        obj->speed_y = 0;
    }

    if (obj->param > 1) {
        obj->iframes_timer = 0;
    } else {
        obj->iframes_timer = 1;
    }
    obj->param = 0;
}

//310F8
void DO_CFUMEE_COMMAND(obj_t* obj) {
    /* 4FEF8 801746F8 -O2 -msoft-float */
    obj->y = ray.y + -98;
    if (
            obj->anim_frame == obj->animations[obj->anim_index].frames_count - 1 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
            level.objects[pierreAcorde_obj_id].iframes_timer == 0
            )
    {
        obj->flags.alive = 0;
        obj->is_active = 0;
    }
}

//31190
void DO_NOVA2_COMMAND(obj_t* obj) {
    if (obj->param != 255) {
        if (obj->param == -1) {
            if (EOA(obj)) {
                obj->flags.alive = 0;
                obj->is_active = 0;
            }
        } else {
            if (obj->param == 0) {
                obj->anim_frame = 0;
                obj->param = -1;
                obj->display_prio = 2;
            } else {
                --obj->param;
            }
        }
    }
}

//31208
void doShipCommand(obj_t* obj) {
    // NOTE(Falcury): PS1 non-matching function - needs checking!
    obj_t *temp_s0_2;
    s32 temp_a0_1;
    u8 var_s2;
    u8 temp_v1;
    u16 var_s3;
    s16 test_3;

    scroll_end_y = ymapmax;
    scroll_start_y = ymapmax;
    ymap = ymapmax;
    if (GameModeVideo == MODE_NORMAL) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }

    temp_v1 = obj->cmd;
    switch (temp_v1)
    {
        case 2:
            obj->speed_x = obj->speed_y = 0;
            return;
        case 20:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            return;
        case 30:
            if (obj->link_has_gendoor) {
                temp_a0_1 = obj->param;
                var_s2 = 1;
                if (temp_a0_1 != -1)
                {
                    if (level.objects[temp_a0_1].is_active)
                        var_s2 = 0;
                }
                if ((var_s2) && (obj->timer != 0) && (horloge[2] != 0))
                {
                    obj->timer--;
                }
                do
                {
                    var_s2 = 0;
                    if (obj->timer == 0)
                    {
                        switch (obj->link)
                        {
                            case 0:
                                obj->link = 2;
                            case 4:
                                var_s3 = obj->type;
                                break;
                            case 1:
                                obj->link = 3;
                                var_s3 = TYPE_77_PIRATE_GUETTEUR;
                                break;
                            case 2:
                                obj->link = 1;
                                var_s3 = TYPE_72_PIRATE_NGAWE;
                                var_s2 = 1;
                                break;
                            case 3:
                                obj->link = 4;
                                var_s3 = TYPE_187_MAMA_PIRATE;
                                break;
                        }
                        if (var_s2 == 0)
                        {
                            obj->timer = 50;
                        }
                        if (var_s3 != obj->type)
                        {
                            skipOneCommand(obj);
                            obj->nb_cmd = 0;
                            test_3 = obj->id;
                            do
                            {
                                test_3 = link_init[test_3];
                                temp_s0_2 = &level.objects[test_3];
                                if (var_s3 == temp_s0_2->type)
                                {
                                    break;
                                }
                            } while (temp_s0_2->type != obj->type);

                            if (temp_s0_2->type != obj->type)
                            {
                                switch (temp_s0_2->type) {
                                    case TYPE_77_PIRATE_GUETTEUR:
                                        guetteurFollowsShip(temp_s0_2);
                                        break;
                                    case TYPE_72_PIRATE_NGAWE:
                                        ngaweTriesToGrabShip(temp_s0_2);
                                        break;
                                    case TYPE_187_MAMA_PIRATE:
                                        pmamaFollowsShip(temp_s0_2);
                                        PMA_SORT_DU_CANON(temp_s0_2);
                                        break;
                                }
                                suppressFromLinkList(temp_s0_2);
                                MOVE_OBJECT(temp_s0_2);
                                calc_obj_pos(temp_s0_2);
                                temp_s0_2->flags.alive = 1;
                                obj->param = temp_s0_2->id;
                            }
                        }
                    }
                } while (var_s2 != 0);
            }
            return;
    }
}

//31400
void DO_DROP_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->speed_y > 2)
        set_main_and_sub_etat(obj, 2, 2);
    else if (obj->speed_y > 1)
        set_main_and_sub_etat(obj, 2, 1);

    if (Abs((s32)obj->speed_x) > 1 && !(obj->main_etat == 2 && obj->sub_etat == 0)) {
        if (obj->flags.flip_x)
            obj->speed_x--;
        else if (!obj->flags.flip_x)
            obj->speed_x++;
    }

    if (obj->main_etat == 2 && obj->sub_etat == 3) {
        if (EOA(obj)) {
            obj->is_active = 0;
            obj->flags.alive = 0;
        }
    }
}

//314A4
void move_fruit_in_water(obj_t* obj) {
    extern u8 BTYP(s16, s16);
    s16 btyp;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s32 x2;

    u8 obj_flip_x = obj->flags.flip_x;
    {
        if (obj->main_etat == 0 && obj->sub_etat == 13 && ray.follow_id == obj->id)
            set_main_and_sub_etat(obj, 0, 15);

        if (obj->main_etat == 0 && obj->sub_etat == 15)
        {
            if (obj->type == TYPE_FALLING_OBJ2 && ray.follow_id == -1)
            {
                set_sub_etat(obj, 13);
                obj->speed_x = 0;
            }
            else if (ray.follow_id == obj->id)
            {
                obj->flags.flip_x = ray.flags.flip_x;
                SET_X_SPEED(obj);
                obj->flags.flip_x = obj_flip_x;
            }

            if (obj->speed_x != 0)
            {
                GET_ANIM_POS(obj, &x, &y, &w, &h);
                x2 = x;
                if (obj->speed_x > -1)
                    x2 += w;
                btyp = BTYP((x + (obj->speed_x < 0 ? 0 : w)) >> 4, (y + 16) >> 4);
                if (block_flags[btyp] & 2) {
                    set_main_and_sub_etat(obj, 0, 13);
                    obj->speed_x = 0;
                }
            }
        }
    }
}

//315F0
void DO_FALLING_OBJ_CMD(obj_t* obj) {
    /* 50A30 80175230 -O2 */
    if (obj->main_etat == 2 && obj->btypes[0] == BTYP_WATER) {
        set_main_and_sub_etat(obj, 0, 14);
        obj->y = ((obj->offset_by + obj->y) & 0xfff0) - obj->offset_by + 7;
    }
    move_fruit_in_water(obj);
    DO_OBJ_COLLISIONS(obj, 10);
    if (obj->sub_etat == 9)
        REINIT_OBJECT(obj);
}

//3167C
void DoFallingObjPoingCollision(obj_t* obj, s16 sprite) {
    s32 fall_x_accel;
    switch (obj->type)
    {
        case TYPE_FALLING_OBJ:
        case TYPE_FALLING_OBJ2:
        case TYPE_FALLING_OBJ3:
            PlaySnd(44, obj->id);
            break;
        case TYPE_FALLING_YING:
        case TYPE_FALLING_YING_OUYE:
            PlaySnd(139, obj->id);
            break;
    }

    switch (poing.sub_etat)
    {
        case 1:
        case 3:
        case 5:
            fall_x_accel = poing.damage;
            break;
        case 8:
        case 10:
        case 12:
            fall_x_accel = poing.damage - 2;
        case 2:
        case 4:
        case 6:
        case 7:
        case 9:
        case 11:
            break;
    }
    fall_x_accel = fall_x_accel * 5 + 10;
    if (poing_obj->speed_x < 0)
        fall_x_accel = -fall_x_accel;

    make_my_fruit_go_down(obj, (s16)fall_x_accel);
}

//3171C
void DO_BLKTOON_EYES_CMD(obj_t* obj) {
    /* 50C60 80175460 -O2 -msoft-float */
    s16 prev_spd_x = obj->speed_x;

    SET_X_SPEED(obj);
    if (prev_spd_x * obj->speed_x < 0) {
        obj->flags.flip_x = !obj->flags.flip_x;
        obj->speed_x = prev_spd_x;
    }
}

//31768
void DO_RAY_POS_CMD(obj_t *obj) {
    if (obj->timer != 255) {
        if (obj->timer == 0) {
            switch (obj->main_etat) {
                case 0:
                    ray.flags.alive = ray.is_active = 1;
                    obj->flags.alive = 0;
                    break;
                case 5:
                    ray.flags.alive = ray.is_active = 1;
                    set_main_and_sub_etat(&ray, 3, 40);
                    break;
            }
            obj->flags.alive = 0;
            del_alwobj(obj->id);
            ray.x = obj->x + obj->offset_bx - ray.offset_bx;
            ray.y = obj->offset_by + obj->y - ray.offset_by;
            dead_time = 64;
        }
        --obj->timer;
    }
}
