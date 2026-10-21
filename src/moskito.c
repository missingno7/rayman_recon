/*
 * Moskito boss (GOG v1.21 object 1:43bc0-1:45480). File name invented (Rayverse moskito.c). Names: Rayverse/Android.
 * Worker candidate: per-function evidence and unresolved differences are in NOTES.md.
 */
#include <stddef.h>
#include "obj.h"
#include "game.h"
#include "inline_helpers.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define true 1
#define false 0
#define Abs(value) ((value) >= 0 ? (value) : -(value))
#define MIN_2(x, y) if (x > y) x = y;
#define MAX_2(x, y) if (x < y) x = y;

typedef enum ObjType {
    TYPE_51_MST_FRUIT1 = 51,
    TYPE_52_MST_FRUIT2 = 52,
    TYPE_MST_FRUIT1 = 51,
    TYPE_MST_FRUIT2 = 52,
    TYPE_MST_COPAIN = 171
} ObjType;

/* These three one-byte states form the MOSKITO object's natural _BSS block. */
u8 curAct;
u8 fistAvoided;
u8 mstMustLeaveScreenToProceed;

extern s16 ray_zdc_x, ray_zdc_y, ray_zdc_h, ray_zdc_w;
extern s16 scroll_start_x, scroll_end_x, scroll_start_y, scroll_end_y;
extern s16 bossScrollStartX, bossScrollEndX, bossScrollStartY, bossScrollEndY;
extern s16 h_scroll_speed, v_scroll_speed, xmap, ymap;
extern u8 scrollLocked;
extern u8 *moskitoActionSequences[10];
extern u8 bossEncounter, currentBossAction, currentBossActionIsOver;
extern u8 saveBossEncounter, saveCurrentBossAction, bossReachingAccuracyX, bossReachingAccuracyY;
extern u8 bossReachingTimer, bossSafeTimer, fin_boss;
extern s16 MsAnimIndex;
extern s16 remoteRayXToReach, bossXToReach, bossYToReach, floorLine, mst_scroll_obj_id;
extern s32 Bloc_lim_H1;
extern s32 alternateBossSpeedFactor, bossSpeedFactor;
extern s16 scroll_x, ray_mode;
extern u8 boss_mort;
extern u8 ModeDemo, horloge[25];
extern obj_t ray;
extern level_t level;
extern poing_t poing;
extern obj_t *poing_obj;
extern record_t record;
extern finbosslevel_t finBosslevel;

void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
s32 vblToEOA(obj_t *obj, u8 a2);
void ChangeLevel(void);
s16 calc_obj_dir(obj_t *obj);
void setBossReachingSpeeds(obj_t *obj, u8 timer, u8 accuracy_x, u8 accuracy_y);
void add_alwobj(obj_t *obj);
u8 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void calc_obj_pos(obj_t *obj);
void obj_hurt(obj_t *obj);
s16 testActionEnd(obj_t *obj);
//53BC0
void getIdealStingCoords(obj_t* obj, s16* out_x, s16* out_y) {
    s16 calc_x;
    s16 calc_y;
    calc_x = ray_zdc_x + (ray_zdc_w >> 1);
    /* DOS 1:43bde..43bf4: halve ray_zdc_y, then add ray_zdc_h. */
    calc_y = ray_zdc_h + (ray_zdc_y >> 1);
    *out_x = calc_x - obj->offset_bx;
    if (obj->flags.flip_x)
        *out_x -= 48;
    else
        *out_x += 48;
    *out_y = calc_y - 112;
}

//53C2C
u8 closeEnoughToSting(obj_t* obj, u16 min_x, u16 min_y) {
    s16 sting_x; s16 sting_y;

    getIdealStingCoords(obj, &sting_x, &sting_y);
    sting_x = Abs((s16)(sting_x - obj->x));
    sting_y = Abs((s16)(sting_y - obj->y));
    if (sting_x < min_x && sting_y < min_y)
        return true;
    else
        return false;
}

//53CAC
u8 moskitoCanAttak(obj_t* obj) {
    if (!scrollLocked) {
        scrollLocked =
            inrange(xmap, bossScrollStartX - h_scroll_speed, bossScrollEndX + h_scroll_speed) &&
            inrange(ymap, bossScrollStartY - v_scroll_speed, bossScrollEndY + v_scroll_speed);
        if (scrollLocked) {
            scroll_end_x = xmap;
            scroll_start_x = xmap;
            scroll_start_y = bossScrollStartY;
            scroll_end_y = bossScrollEndY;
        }
    }
    else {
        if (scroll_start_y < ymap)
            scroll_start_y = ymap;
        if (obj->timer != 0)
            --obj->timer;
        if (obj->scale != 0)
            --obj->scale;
    }
    if (scrollLocked && obj->timer == 0)
        return true;
    return false;
}

//53DC4
s16 setMoskitoAtScrollBorder(obj_t* obj, u8 a2) {
    /* 6FCCC 801944CC -O2 -msoft-float */
    s16 res;

    switch (a2) {
        case 0:
            obj->x = scroll_start_x - obj->offset_bx;
            res = -1;
            obj->flags.flip_x = false;
            break;
        case 1:
            obj->x = scroll_end_x + SCREEN_WIDTH - obj->offset_bx;
            res = 1;
            obj->flags.flip_x = true;
            break;
        default:
            res = obj->x + obj->offset_bx - ((scroll_start_x + scroll_end_x + SCREEN_WIDTH) / 2);
            if (res > 0) {
                obj->x = scroll_end_x + SCREEN_WIDTH - obj->offset_bx;
                obj->flags.flip_x = false;
            } else {
                obj->x = scroll_start_x - obj->offset_bx;
                obj->flags.flip_x = true;
            }
            break;
    }
    return res;
}

//53F04
void prepareNewMoskitoAttack(obj_t* mst_obj) {
    /* 6FDC8 801945C8 -O2 -msoft-float */
    obj_t *cur_obj;
    s16 i;
    u8 prev_flip_x;
    s16 unk_2;
    s16 nb_obj;
    s32 midpoint_sum;

    midpoint_sum = scroll_start_x;
    unk_2 = (scroll_end_x + midpoint_sum + SCREEN_WIDTH) / 2;
    while (currentBossActionIsOver)
    {
        curAct = moskitoActionSequences[bossEncounter][currentBossAction++];
        alternateBossSpeedFactor = 0;
        switch (curAct) {
            default: break;
            case 3: // end arrival sequence
                bossEncounter++;
            case 2:
                currentBossAction = 0;
                currentBossActionIsOver = true;
                break;
            case 5:
                bossEncounter = saveBossEncounter;
                currentBossAction = saveCurrentBossAction;
                curAct = moskitoActionSequences[bossEncounter][currentBossAction];
                if (curAct == 26 || curAct == 14)
                {
                    while (curAct == 26 || curAct == 14)
                        curAct = moskitoActionSequences[bossEncounter][++currentBossAction];
                }
                /* DOS 1:4401d..4402b: advance a resumed side attack. */
                else if (curAct == 20)
                    currentBossAction++;
                else if (curAct == 18 || curAct == 19 || (curAct == 21))
                {
                    if (curAct == 21)
                        currentBossAction++;

                    currentBossAction++;
                    curAct = moskitoActionSequences[bossEncounter][currentBossAction];
                    while (curAct == 21 || curAct == 20)
                    {
                        if (curAct == 21)
                            currentBossAction++;

                        currentBossAction++;
                        curAct = moskitoActionSequences[bossEncounter][currentBossAction];
                    }
                }
                currentBossActionIsOver = true;
                saveBossEncounter = 255;
                mstMustLeaveScreenToProceed = false;
                break;
            case 12:
            {
                u8 curAct_tmp_1;
                curAct_tmp_1 = moskitoActionSequences[bossEncounter][currentBossAction++];
                currentBossAction = 0;
                if (curAct_tmp_1 == bossEncounter)
                    curAct_tmp_1++;
                bossEncounter = curAct_tmp_1;

                currentBossActionIsOver = false;
                break;
            }
            case 25: // end of dying sequence
                if (mst_obj->init_hit_points == 5) {
                    ChangeLevel();
                    finBosslevel.bzzit = true;
                } else {
                    remoteRayXToReach = -32000;
                    finBosslevel.moskito = true;
                    set_main_and_sub_etat(&ray, 3, 7); // NOTE: changed in PC version (is sub etat 52 in PS1 version)
                    mst_obj->nb_cmd = vblToEOA(&ray, 1) + 1;
                    fin_boss = true;
                }
                currentBossActionIsOver = false;
                break;
            case 22: // dying sequence: fall on the floor
                if (mst_obj->init_hit_points == 12) {
                    set_main_and_sub_etat(mst_obj, 0, 19);
                } else {
                    i = 0;
                    cur_obj = &level.objects[i];
                    nb_obj = level.nb_objects;
                    while (i < nb_obj) {
                        if (cur_obj->type == TYPE_MST_COPAIN) {
                            mst_obj->sprites = cur_obj->sprites;
                            mst_obj->animations = cur_obj->animations;
                            mst_obj->img_buffer = cur_obj->img_buffer;
                            mst_obj->eta = cur_obj->eta;
                            mst_obj->nb_sprites = cur_obj->nb_sprites;
                            mst_obj->main_etat = cur_obj->main_etat;
                            mst_obj->sub_etat = cur_obj->sub_etat;
                            mst_obj->anim_index = cur_obj->anim_index;
                            mst_obj->anim_frame = 0;
                            mst_obj->offset_by += 4;
                            mst_obj->flags.flip_x = false;

                            ray.is_active = false;
                            break;
                        }
                        i++;
                        cur_obj++;
                    }
                }
                skipToLabel(mst_obj, 1, true);
                currentBossActionIsOver = false;
                mst_obj->nb_cmd += 128;
                break;
            case 24: // dying sequence: fall down from above the screen
                if (mst_obj->init_hit_points == 12)
                    mst_obj->offset_by -= 16; // NOTE: changed in PC version

                mst_obj->flags.flip_x = false;
                bossXToReach = scroll_start_x + SCREEN_WIDTH / 2 - mst_obj->offset_bx;
                mst_obj->x = bossXToReach;
                bossYToReach = floorLine - mst_obj->offset_by;
                set_main_and_sub_etat(mst_obj, 2, 1);
                mst_obj->gravity_value_2 = 2;
                skipToLabel(mst_obj, 2, true); // NOTE: added in PC version
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0;
                bossReachingTimer = 1;
                currentBossActionIsOver = false;
                break;
            case 17: // recover after hit
                bossXToReach = scroll_start_x + SCREEN_WIDTH / 2 - mst_obj->offset_bx;
                bossYToReach = floorLine - 230 + (s16)Bloc_lim_H1;
                set_main_and_sub_etat(mst_obj, 0, 6);
                skipToLabel(mst_obj, 2, true);
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                currentBossActionIsOver = false;
                break;
            case 16: // hit
                set_main_and_sub_etat(mst_obj, 0, 5);
                skipToLabel(mst_obj, 1, true);
                currentBossActionIsOver = false;
                break;
            case 6:
                set_main_and_sub_etat(mst_obj, 0, 13);
                skipToLabel(mst_obj, 1, true);
                currentBossActionIsOver = false;
                break;
            case 26: // at top of screen (after recovering from hit), turn around
                if (mst_obj->main_etat == 0 && mst_obj->sub_etat == 20)
                {
                    calc_obj_dir(mst_obj);
                    set_main_and_sub_etat(mst_obj, 0, 0); // NOTE: different from PS1 version? (sub etat 21 is invalid)
                    skipToLabel(mst_obj, 1, true);
                    currentBossActionIsOver = false;
                }
                else
                {
                    s32 flip_snapshot;
                    /* DOS 1:443d5..443f8: wide snapshot; case 13 saves a byte.
                     * DOS 1:44437..44459: the animation count has a separate lifetime. */
                    flip_snapshot = mst_obj->flags.flip_x;
                    calc_obj_dir(mst_obj);
                    if ((mst_obj->flags.flip_x) != flip_snapshot)
                    {
                        s32 anim_frames;
                        mst_obj->flags.flip_x = flip_snapshot;
                        set_main_and_sub_etat(mst_obj, 0, 0); // NOTE: different from PS1 version? (sub etat 21 is invalid)
                        anim_frames = vblToEOA(mst_obj, 1);
                        set_main_and_sub_etat(mst_obj, 0, 1);
                        skipToLabel(mst_obj, 1, true);
                        mst_obj->nb_cmd = anim_frames + mst_obj->nb_cmd;
                        currentBossActionIsOver = false;
                    }
                }
                break;
            case 23: // dying sequence: move up
                remoteRayXToReach = scroll_start_x + 160 - mst_obj->offset_bx - 48;
                if (ray.x & 1) /* ??? */
                    remoteRayXToReach |= 1;
                else
                    remoteRayXToReach &= ~1;
                goto block_52;
            case 11: // decide whether to fly up out of the screen after recovering from a hit (?)
                bossSafeTimer = 0;
                mst_obj->display_prio = 3;
                if (mstMustLeaveScreenToProceed == 0)
                {
                    currentBossActionIsOver = true;
                    break;
                }
                mstMustLeaveScreenToProceed = 0;
                curAct = 4; // fly up
                /* fall through */
            case 4: // fly straight up, out of the screen
            block_52:
                bossXToReach = mst_obj->x;
                bossYToReach = scroll_start_y + (s16)Bloc_lim_H1 - 128; // NOTE: changed in PC version
                if (curAct == 4)
                    set_main_and_sub_etat(mst_obj, 0, 8);
                else
                    set_main_and_sub_etat(mst_obj, 0, 6);

                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                skipToLabel(mst_obj, 2, true);
                currentBossActionIsOver = false;
                break;
            case 15: // end recover after hit (reached top of the screen)
                set_main_and_sub_etat(mst_obj, 0, 20);
                skipToLabel(mst_obj, 1, true);
                currentBossActionIsOver = false;
                break;
            case 14: // sting
                set_main_and_sub_etat(mst_obj, 0, 15);
                skipToLabel(mst_obj, 1, true);
                currentBossActionIsOver = false;
                break;
            case 10: // fly down from the top of the screen toward Rayman (basic flying)
                calc_obj_dir(mst_obj);
                setMoskitoAtScrollBorder(mst_obj, !mst_obj->flags.flip_x);
                mst_obj->y = scroll_start_y - 128;
                if (mst_obj->flags.flip_x)
                {
                    mst_obj->x -= 48;
                    mst_obj->flags.flip_x = false;
                }
                else
                {
                    mst_obj->x += 48;
                    mst_obj->flags.flip_x = true;
                }
            case 13: // basic fly around
                fistAvoided = false;
                prev_flip_x = mst_obj->flags.flip_x;
                calc_obj_dir(mst_obj);
                getIdealStingCoords(mst_obj, &bossXToReach, &bossYToReach);
                set_main_and_sub_etat(mst_obj, 0, 8);
                bossReachingAccuracyX = 16;
                bossReachingAccuracyY = 16;
                bossReachingTimer = 2;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                if (curAct == 13)
                    mst_obj->speed_y = -64;
                else
                    mst_obj->speed_x = 0;

                mst_obj->flags.flip_x = prev_flip_x;
                skipToLabel(mst_obj, 2, true);
                currentBossActionIsOver = false;
                break;
            case 7: // fly in carrying small spiky fruit
                set_main_and_sub_etat(mst_obj, 0, 12);
            case 8: // fly in carrying medium spiky fruit
                if (curAct == 8)
                    set_main_and_sub_etat(mst_obj, 0, 11);

                if (setMoskitoAtScrollBorder(mst_obj, 2) > 0)
                {
                    mst_obj->flags.flip_x = false;
                    mst_obj->x += 48;
                    bossXToReach = scroll_start_x - mst_obj->offset_bx - 48;
                }
                else
                {
                    mst_obj->flags.flip_x = true;
                    mst_obj->x -= 48;
                    bossXToReach = scroll_end_x + SCREEN_WIDTH - mst_obj->offset_bx + 48;
                }
                mst_obj->y = floorLine - 230;
                bossYToReach = mst_obj->y;
                alternateBossSpeedFactor = moskitoActionSequences[bossEncounter][currentBossAction++] << 10;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                skipToLabel(mst_obj, 2, true);
                currentBossActionIsOver = false;
                break;
            case 9: // fly in carrying huge spiky fruit
                set_main_and_sub_etat(mst_obj, 0, 9);
                if (setMoskitoAtScrollBorder(mst_obj, 2) > 0)
                {
                    mst_obj->flags.flip_x = false;
                    mst_obj->x += 80;
                    bossXToReach = scroll_start_x - mst_obj->offset_bx - 80;
                }
                else
                {
                    mst_obj->flags.flip_x = true;
                    mst_obj->x -= 96;
                    bossXToReach = scroll_end_x + SCREEN_WIDTH - mst_obj->offset_bx + 64;
                }
                mst_obj->y = floorLine - 180;
                bossYToReach = mst_obj->y;
                alternateBossSpeedFactor = moskitoActionSequences[bossEncounter][currentBossAction++] << 10;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                skipToLabel(mst_obj, 2, true);
                currentBossActionIsOver = false;
                break;
            case 18: // prepare to fly in fast from the side (low)
            case 19: // prepare to fly in fast from the side (high)
                mst_obj->y = floorLine - 72;
                if (curAct == 19)
                    mst_obj->y -= 48;

                if (setMoskitoAtScrollBorder(mst_obj, 2) > 0)
                {
                    mst_obj->flags.flip_x = false;
                    mst_obj->x += 68;
                    bossXToReach = mst_obj->x - 24;
                }
                else
                {
                    mst_obj->flags.flip_x = true;
                    mst_obj->x -= 76;
                    bossXToReach = mst_obj->x + 16;
                }

                set_main_and_sub_etat(mst_obj, 0, 14);
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                bossYToReach = mst_obj->y;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                skipToLabel(mst_obj, 2, true);
                currentBossActionIsOver = false;
                break;
            case 21: // wiggle/wait, preparing to fly in fast from the side
                set_main_and_sub_etat(mst_obj, 0, 14);
                skipToLabel(mst_obj, 1, true);
                mst_obj->nb_cmd *= moskitoActionSequences[bossEncounter][currentBossAction++];
                currentBossActionIsOver = false;
                break;
            case 20: // fly in fast from the side (woosh)
                if (mst_obj->x + mst_obj->offset_bx > unk_2)
                {
                    mst_obj->flags.flip_x = false;
                    bossXToReach = scroll_start_x - mst_obj->offset_bx - 56;
                }
                else
                {
                    mst_obj->flags.flip_x = true;
                    bossXToReach = scroll_end_x + SCREEN_WIDTH - mst_obj->offset_bx + 48;
                }

                bossYToReach = mst_obj->y;
                set_main_and_sub_etat(mst_obj, 0, 4);
                skipToLabel(mst_obj, 2, true);
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(mst_obj, 1, 0xFF, 0xFF);
                currentBossActionIsOver = false;
                break;
        }
    }
}

//54AEC
obj_t* allocateMoskitoFruit(obj_t* mst2_obj) {
    /* 70C64 80195464 -O2 -msoft-float */
    u8 spr_ind;
    u8 type_check;
    s16 i;
    obj_t* cur_obj;
    s16 nb_objs;
    s16 unk_h; s16 unk_w; s16 unk_y; s16 unk_x;
    obj_t* res = NULL;

    switch (mst2_obj->sub_etat)
    {
        case 12:
            spr_ind = 3;
            type_check = TYPE_51_MST_FRUIT1;
            break;
        case 11:
            spr_ind = 3;
            type_check = TYPE_52_MST_FRUIT2;
            break;
    }

    i = 0;
    cur_obj = &level.objects[i];
    nb_objs = level.nb_objects;
    while (i < (nb_objs)) {
        if (cur_obj->type == type_check && !cur_obj->is_active) {
            cur_obj->flags.alive = true;
            cur_obj->is_active = true;
            add_alwobj(cur_obj);
            GET_SPRITE_POS(mst2_obj, spr_ind, &unk_x, &unk_y, &unk_w, &unk_h);
            cur_obj->x = unk_x - cur_obj->offset_bx + (unk_w >> 1);
            cur_obj->y = unk_y - cur_obj->offset_hy;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            switch (type_check)
            {
                case TYPE_MST_FRUIT1:
                    cur_obj->param = 1;
                    cur_obj->speed_x = 0;
                    break;
                case TYPE_MST_FRUIT2:
                    cur_obj->param = 3;
                    cur_obj->speed_x = mst2_obj->speed_x;
                    break;
                default: break;
            }
            cur_obj->speed_y = 0;
            cur_obj->anim_index = cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].anim_index;
            cur_obj->anim_frame = 0;
            calc_obj_pos(cur_obj);
            cur_obj->gravity_value_1 = 0;
            cur_obj->flags.alive = true;
            cur_obj->is_active = true;
            cur_obj->flags.flip_x = mst2_obj->flags.flip_x;
            cur_obj->flags.flag_0x40 = false;
            res = cur_obj;
            break;
        }
        i++;
        cur_obj++;
    }

    return res;
}

//54C9C
void moskitoDropFruitOnRay(obj_t* obj) {
    /* DOS frame: height +0, width +4, x_2 +8, cen_y +12, y_1 +16. */
    s16 x_1, width;
    s16 y_1, cen_x, cen_y;
    s16 distance, x_2, height;
    x_1 = obj->offset_bx + obj->x;
    x_2 = x_1;
    switch (obj->sub_etat) {
    case 12:
        distance = ((obj->speed_x - ray.speed_x) * 2) / 16;
        x_1 -= distance / 2;
        break;
    case 11:
        distance = (obj->speed_x * 120) / 16;
        break;
    }
    width = Abs((s32)distance);
    if (distance < 0) x_1 += distance;
    y_1 = obj->offset_by + obj->y;
    height = scroll_end_y - scroll_start_y + SCREEN_HEIGHT;
    cen_x = ray_zdc_x + (ray_zdc_w >> 1);
    cen_y = ray_zdc_h + (ray_zdc_y >> 1);
    /* DOS 1:44d44: [scroll_end_x - 2] >> 16 is the signed short upper bound. */
    if (inrange(x_2, scroll_start_x, scroll_end_x + 304) &&
        cen_x >= x_1 && cen_y >= y_1 && cen_x <= x_1 + width && cen_y <= y_1 + height) {
        allocateMoskitoFruit(obj);
        set_main_and_sub_etat(obj, 0, 16);
        bossYToReach -= 64;
    }
}

//54E1C
void doMoskitoCommand(obj_t* obj) {
    /* 71030 80195830 -O2 -msoft-float */
    if (moskitoCanAttak(obj)) {
        if (obj->scale != 0) {
            obj->scale--; // Added in PC/mobile versions
        }
        if (bossSafeTimer != 0)
            bossSafeTimer--;
        if (!(obj->flags.hurtbyfist))
        {
            switch (obj->cmd)
            {
            case 2:
                if (MsAnimIndex == 14 && obj->anim_index == 0x0F)
                    obj->flags.flip_x = !obj->flags.flip_x;
                bossXToReach = -32000;
                bossYToReach = -32000;
                obj->speed_x = 0;
                obj->speed_y = 0;
                break;
            default:
                switch (curAct)
                {
                    case 10:
                    case 13:
                        calc_obj_dir(obj);
                        getIdealStingCoords(obj, &bossXToReach, &bossYToReach);
                        if (closeEnoughToSting(obj, 24, 24))
                            currentBossActionIsOver = true;
                        /* fall through */
                    case 4:
                        if (
                                obj->hit_points < 2 &&
                                poing.is_active &&
                                !fistAvoided &&
                                ModeDemo == 0 &&
                                !record.is_recording
                                )
                        {
                            fistAvoided = true;
                            if (
                                    obj->y + (obj->offset_by + obj->offset_hy) / 2 >
                                    poing_obj->y + (poing_obj->offset_by + poing_obj->offset_hy) / 2
                                    )
                                obj->speed_y += 80;
                            else
                                obj->speed_y -= 80;

                            if ((obj->offset_bx + obj->x) > (poing_obj->offset_bx + poing_obj->x))
                                obj->speed_x += 80;
                            else
                                obj->speed_x -= 80;
                        }
                        break;
                }
                setBossReachingSpeeds(obj, bossReachingTimer, bossReachingAccuracyX, bossReachingAccuracyY);
                if (obj->main_etat == 0 && (obj->sub_etat == 12 || obj->sub_etat == 11))
                    moskitoDropFruitOnRay(obj);
            }
            testActionEnd(obj);
        }
        prepareNewMoskitoAttack(obj);
    }
    obj->flags.hurtbyfist = 0;
    MsAnimIndex = obj->anim_index;
}

//55054
u8 tellNextMoskitoAction(void) {
    /* 71330 80195B30 -O2 -msoft-float */
    u8 *actions;

    if (bossEncounter != 8)
        actions = &moskitoActionSequences[bossEncounter][currentBossAction];
    else
        actions = &moskitoActionSequences[saveBossEncounter][saveCurrentBossAction];

    switch (actions[0])
    {
        case 2:
            return moskitoActionSequences[bossEncounter][0];
        case 3:
            return moskitoActionSequences[bossEncounter + 1][0];
        case 5:
            return moskitoActionSequences[saveBossEncounter][0];
        case 12:
            return moskitoActionSequences[actions[1]][0];
        default:
            return actions[0];
    }
}

//550D8
void changeMoskitoPhase(obj_t* obj) {
    /* 7144C 80195C4C -O2 -msoft-float */
    u8 *enc;
    u8 *act;
    u8 enc_prev;
    u8 hp;

    if (bossEncounter == 8)
    {
        enc = &saveBossEncounter;
        act = &saveCurrentBossAction;
    }
    else
    {
        enc = &bossEncounter;
        act = &currentBossAction;
    }

    enc_prev = *enc;
    if (obj->init_hit_points == 12)
    {
        
        if (obj->hit_points > 9)
            *enc = 3;
        else if (obj->hit_points > 7)
            *enc = 5;
        else if (obj->hit_points > 4)
            *enc = 6;
        else if (obj->hit_points > 0)
            *enc = 7;
        else
        {
            *enc = 9;
            bossSafeTimer = 0;
        }

        if (enc_prev != *enc)
            *act = 0;
    }
}

//55144
void doMoskitoHit(obj_t* obj, s16 sprite) {
    /* 7151C 80195D1C -O2 -msoft-float */
    u8 act_next;
    s32 dividend;
    s32 divisor;

    if (bossSafeTimer == 0)
    {
        poing.damage = 1;
        obj_hurt(obj);
        bossSafeTimer = 0xFF;
        obj->flags.hurtbyfist = true;
        changeMoskitoPhase(obj);
        act_next = tellNextMoskitoAction();
        if (
                act_next >= 7 &&
                (act_next <= 10 || (act_next >= 18 && act_next <= 21))
                )
            mstMustLeaveScreenToProceed = true;

        /* DOS 1:4519d..451ad: the byte HP test feeds the promoted denominator.
         * Direct field expressions reproduce the allocation; an hp local changes it. */
        if (obj->hit_points != 0)
        {
            divisor = obj->hit_points;
            divisor += obj->init_hit_points;
            dividend = obj->init_hit_points * 0xc000 / 12;
            bossSpeedFactor = (dividend / divisor) + 0x4000;

            if (bossEncounter != 8)
            {
                saveBossEncounter = bossEncounter;
                saveCurrentBossAction = currentBossAction;
                bossEncounter = 8;
            }
        }
        else
        {
            bossSpeedFactor = 0x4000;
            bossEncounter = 9;
            boss_mort = true;
        }
        currentBossAction = 0;
        currentBossActionIsOver = true;
    }
}

//55230
void DO_MST_SCROLL_COMMAND(obj_t* obj) {
    /* 4D354 80171B54 -O2 -msoft-float */
    s16 unk_x; s16 unk_y;

    if (obj->hit_points == 0)
    {
        switch (obj->timer)
        {
            case 0:
                obj->follow_id = ray.x - 50;
                obj->iframes_timer = ray.y - 80;
                unk_x = 1;
                unk_y = 1;
                break;
            case 1:
                if (ray_mode <= 0)
                    obj->timer = 2;
                obj->follow_id = xmap + obj->param - 100;
                obj->iframes_timer = ymap + obj->link - 10;
                unk_x = h_scroll_speed + 1;
                unk_y = 2;
                break;
            case 2:
                obj->param = 500;
                obj->follow_id = xmap + 1000;
                obj->iframes_timer = ray.y - 125;
                unk_y = 1;
                unk_x = 3;
                break;
        }

        if (!(scroll_x == -1 || obj->timer == 2))
            obj->timer = 1;

        if (horloge[5] != 0)
        {
            if (obj->follow_id > obj->x)
            {
                obj->speed_x++;
                MIN_2(obj->speed_x, unk_x);

                if (obj->x + obj->speed_x > obj->follow_id)
                    obj->speed_x = unk_x - 1;
            }
            else
            {
                obj->speed_x--;
                MAX_2(obj->speed_x, -1);
            }

            if (obj->iframes_timer > obj->y + obj->speed_y)
            {
                obj->speed_y++;
                MIN_2(obj->speed_y, unk_y);
            }
            else if (obj->iframes_timer < obj->y + obj->speed_y - unk_y)
            {
                obj->speed_y--;
                MAX_2(obj->speed_y, -unk_y);
            }
            else
                obj->speed_y = 0;
        }
    }
}

//553E0
void DO_MST_CHANGE_COMMAND(obj_t* obj) {
    /* 4D5AC 80171DAC -O2 -msoft-float */
    /* ugh */
    if (level.objects[mst_scroll_obj_id].param != 500)
    {
        switch (obj->hit_points)
        {
            case 1:
                level.objects[mst_scroll_obj_id].param = 100;
                return;
            case 2:
                level.objects[mst_scroll_obj_id].param = 0;
                return;
            case 3:
                level.objects[mst_scroll_obj_id].param = -80;
                return;
            case 4:
                level.objects[mst_scroll_obj_id].link = -100;
                return;
            case 5:
                level.objects[mst_scroll_obj_id].link = 0;
                break;
        }
    }
}

//5544C
void DoMstShakyFruitRaymanZDD(obj_t* obj) {
    if (!(obj->main_etat == 2 || obj->sub_etat == 1)) {
        obj->param = 1;
        skipToLabel(obj, 0, true);
        obj->gravity_value_2 = 4;
    }
}


