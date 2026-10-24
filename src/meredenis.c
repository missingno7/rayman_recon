#include "inline_helpers.h"
/*
 * Viking Mama boss (GOG v1.21 object 1:40b60-1:42d30). File name invented (Rayverse meredenis.c).
 * Names: Rayverse/Android. Worker results and remaining differences: NOTES.md.
 * Accepted baseline is recorded separately in config/units.json.
 */
#include "obj.h"
#include "game.h"
#include <stdlib.h>
#include <string.h>
#include "meredenis_local.h"

#define PC 1
#define SCREEN_WIDTH 320
#define true 1
#define false 0
#define TYPE_98_SPACE_MAMA 98
#define TYPE_112_WASHING_MACHINE 112
#define TYPE_192_SMA_GRAND_LASER 192
#define TYPE_193_SMA_BOMB 193
#define TYPE_194_SMA_BOMB_CHIP 194
#define TYPE_203_SMA_WEAPON 203
#define TYPE_213_SPACE_MAMA2 213
#define TYPE_244_SMA_PETIT_LASER 244
#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_WAIT 2
#define Abs(value) ((value) >= 0 ? (value) : -(value))

/* DOS stores the TYPE_203 weapon index at 2:54d90 (mereDenis_weapon_id) and the TYPE_112 washing-machine index
 * at 2:54d8c (machine_obj_id); Rayverse had these two names swapped. */
//50B60
void findMereDenisWeapon(void) {
    s16 i;
    s16 nb_objs;
    obj_t *cur;

    mereDenis_weapon_id = -1;
    machine_obj_id = -1;
    cur = level.objects;
    nb_objs = level.nb_objects;
    i = 0;
    if (nb_objs > 0) {
        do {
            if (cur->type == TYPE_203_SMA_WEAPON) {
                mereDenis_weapon_id = i;
            } else if (cur->type == TYPE_112_WASHING_MACHINE) {
                machine_obj_id = i;
            }
            ++cur;
            ++i;
        } while (i < nb_objs);
    }
}


//50BB8
u8 mereDenisCanAttak(obj_t *obj) {
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
    } else {
        if (obj->timer && !horloge[2]) obj->timer--;
        if (scroll_start_y < ymap) scroll_start_y = ymap;
    }
    return scrollLocked && !obj->timer;
}

//50CC8
s16 setMereDenisAtScrollBorder(obj_t *obj, u8 flip_x) {
    s16 delta;
    switch (flip_x) {
    case 0:
        obj->x = scroll_start_x - obj->offset_bx;
        delta = -1;
        obj->flags.flip_x = 1;
        break;
    case 1:
        obj->x = scroll_end_x + 304 - obj->offset_bx;
        delta = 1;
        obj->flags.flip_x = 0;
        break;
    default:
        delta = obj->x + obj->offset_bx - (scroll_start_x + scroll_end_x + 320) / 2;
        if (delta > 0) {
            obj->x = scroll_end_x + 304 - obj->offset_bx;
            obj->flags.flip_x = 0;
        } else {
            obj->x = scroll_start_x - obj->offset_bx;
            obj->flags.flip_x = 1;
        }
        break;
    }
    return delta;
}

//50DA4
void setCirclePointToReach(void) {
    bossXToReach = circle_x + (sinus(circle_index) * 100) / 512;
    bossYToReach = circle_y + (cosinus(circle_index) * 60) / 512;
    circle_index += 4;
    if (circle_index > 512) circle_index = -32000;
}

//50E5C
void allocateMereDenisBombChips(obj_t* obj) {
    obj_t *cur;
    s16 j;
    s16 nb_objs;
    u8 i;
    u8 unk_1;
    u8 sub_etat_2;
    u8 sub_etat_1;

    for (i = 0; i <= 2; i += 2) {
        cur = level.objects;
        j = 0;
        nb_objs = level.nb_objects;
        while (j < nb_objs) {
            unk_1 = 0;
            switch (obj->sub_etat) {
                case 1:
                    sub_etat_1 = 0;
                    sub_etat_2 = 1;
                    break;
                case 3:
                case 5:
                    if (obj->flags.flip_x) {
                        sub_etat_1 = 4;
                        sub_etat_2 = 5;
                    } else {
                        sub_etat_1 = 2;
                        sub_etat_2 = 3;
                    }
                    break;
                case 7:
                    if (obj->flags.flip_x)
                        unk_1 = 1;
                    sub_etat_1 = 6;
                    sub_etat_2 = 7;
                    break;
            }

                if (cur->type == TYPE_194_SMA_BOMB_CHIP &&
                    !cur->is_active &&
                    (cur->sub_etat == sub_etat_1 || cur->sub_etat == sub_etat_2))
                {
                    cur->flags.flip_x = obj->flags.flip_x;
                    cur->is_active = 1;
                    cur->flags.alive = 1;
                    add_alwobj(cur);
                    cur->x = obj->x;
                    cur->y = obj->y;
                    set_main_and_sub_etat(cur, 1, (i != 0 ? sub_etat_2 : sub_etat_1));
                    calc_obj_pos(cur);
                    skipToLabel(cur, (unk_1 ? i == 0 : i != 0), true);
                    break;
                }
                ++cur;
                ++j;
        }
    }

    allocateExplosion(obj);
    PlaySnd(220, obj->id);
    obj->is_active = 0;
    obj->flags.alive = 0;
}

//51010
void mereDenisExplodeBombs(void) {
    s16 v;
    s16 t;
    for (t = 0; t < 8; t++) {
        for (v = 0; v < 8; v++) {
            u32 id = droppedBombIds[t][v];
            if (id != (u32)-1) {
                level.objects[id].timer = 70;
                droppedBombIds[t][v] = -1;
            }
        }
    }
}

//5108C
void mereDenisDropBomb(obj_t *smama_obj)
{
    s16 ray_x;
    s16 smama_x_1; s16 smama_x_2;
    obj_t *cur_obj;
    s16 j; u8 i;
    s16 nb_objs;
    u8 new_sub_etat;
    s16 new_flip_x;

    if (currentBombSequence == 0xfe && lastDroppedBombXCenterPos == -32000)
    {
        ray_x = ray.x + ray.offset_bx;
        smama_x_1 = smama_obj->offset_bx + smama_obj->x;
        if (inrange(ray_x, smama_x_1 - 8, smama_x_1 + 8))
        {
            cur_obj = level.objects; 
            j = 0;
            nb_objs = level.nb_objects;
            while (j < nb_objs)
            {
                if (cur_obj->type == TYPE_193_SMA_BOMB && cur_obj->is_active == 0)
                {
                    cur_obj->flags.flip_x = 0;
                    cur_obj->sub_etat = 0;
                    cur_obj->x = smama_x_1 - cur_obj->offset_bx;
#ifdef PC
                    cur_obj->y = smama_obj->y + smama_obj->offset_by - cur_obj->offset_by - 3 * ((Bloc_lim_H2 - Bloc_lim_H1) / 18);
#else
                    cur_obj->y = smama_obj->y + smama_obj->offset_by - cur_obj->offset_by - 3 * 11;
#endif
                    cur_obj->is_active = 1;
                    cur_obj->flags.alive = 1;
                    add_alwobj(cur_obj);
                    cur_obj->timer = 50;
                    break;
                }
                cur_obj++;
                j++;
            }
            currentBombSequence = 0xff;
            lastDroppedBombXCenterPos = smama_x_1;
        }
    }
    else
    {
        for (i = 0; i < sizeof(lastDroppedBombIdInSequence); i++)
        {
            if (lastDroppedBombIdInSequence[i] == 7) {
                currentBombSequence = 0xff;
                lastDroppedBombIdInSequence[i] = 0xff;
                lastDroppedBombXCenterPos = -32000;
            }
            else
            {
                smama_x_2 = smama_obj->x + smama_obj->offset_bx;
                if (i == 0) {
#ifdef PC
                    if (Abs(smama_x_2 - lastDroppedBombXCenterPos) < (Bloc_lim_W2 - Bloc_lim_W1) / 9) {
                        return;
                    }
#else
                    if (abs(smama_x_2 - lastDroppedBombXCenterPos) < 0x25) {
                        return;
                    }
#endif
                }
                lastDroppedBombIdInSequence[i]++;
                switch (bombSequences[currentBombSequence][i][lastDroppedBombIdInSequence[i]])
                {
                case 0:
                case 4:
                    new_sub_etat = 0;
                    new_flip_x = 0;
                    break;
                case 1:
                case 5:
                    new_sub_etat = 2;
                    new_flip_x = smama_obj->flags.flip_x;
                    break;
                case 2:
                    new_sub_etat = 6;
                    new_flip_x = smama_obj->flags.flip_x;
                    break;
                case 6:
                    new_sub_etat = 6;
                    new_flip_x = !smama_obj->flags.flip_x;
                    break;
                case 3:
                case 7:
                    new_sub_etat = 4;
                    new_flip_x = !smama_obj->flags.flip_x;
                    break;
                case 8:
                    droppedBombIds[i][lastDroppedBombIdInSequence[i]] = -1;
                    smama_x_2 = 0;
                    break;
                default:
                    break;
                }

                if (smama_x_2 != 0)
                {
                    cur_obj = level.objects;
                    j = 0;
                    nb_objs = level.nb_objects;
                    while (j < nb_objs)
                    {
                        if (cur_obj->type == TYPE_193_SMA_BOMB && cur_obj->is_active == 0)
                        {
                            cur_obj->sub_etat = new_sub_etat;
                            cur_obj->flags.flip_x = new_flip_x;
                            droppedBombIds[i][lastDroppedBombIdInSequence[i]] = j;
                            cur_obj->anim_frame = 0;
                            cur_obj->x = smama_x_2 - cur_obj->offset_bx;
#ifdef PC
                            cur_obj->y = (smama_obj->y + smama_obj->offset_by - cur_obj->offset_by) + (i - 6) * ((Bloc_lim_H2 - Bloc_lim_H1) / 18);
#else
                            cur_obj->y = (smama_obj->y + smama_obj->offset_by - cur_obj->offset_by) + (i - 6) * 11;
#endif
                            cur_obj->is_active = 1;
                            cur_obj->flags.alive = 1;
                            add_alwobj(cur_obj);
                            break;
                        }
                        cur_obj++;
                        j++;
                    }
                    if (j >= nb_objs)
                        droppedBombIds[i][lastDroppedBombIdInSequence[i]] = -1;
                }
                lastDroppedBombXCenterPos = smama_obj->x + smama_obj->offset_bx;
            }
        }
    }
}



//51470
void swapWeaponAnimState(obj_t* obj, u8 new_sub_etat) {
        set_main_and_sub_etat(obj, 0, new_sub_etat);
        obj->anim_frame = 0;
        obj->anim_index = obj->eta[0][new_sub_etat].anim_index;
    
}

//5149C
void swapMereDenisCollZones(obj_t* obj, u8 smama2) {
    if (smama2)
        obj->type = TYPE_213_SPACE_MAMA2;
    else
        obj->type = TYPE_98_SPACE_MAMA;
    obj->zdc = type_zdc[obj->type];
}

//515AC

void prepareNewMereDenisAttack(obj_t* smama_obj) {
    u8 cur_act = 0xFF;
    obj_t* wep_obj = &level.objects[mereDenis_weapon_id];
    obj_t* mach_obj = &level.objects[machine_obj_id];
    u8 old_flip_x;
    s16 i;
    eta_t *state_row;

    if (circle_index == -32000) 
    {
        while (currentBossActionIsOver) 
        {
            cur_act = mereDenisActionSequences[bossEncounter][currentBossAction++];
            switch (cur_act) {
            case 0:
                bossEncounter++;
            case 1:
                currentBossAction = 0;
                currentBossActionIsOver = 1;
                break;

            case 2:
                bossEncounter = saveBossEncounter;
                currentBossAction = saveCurrentBossAction;
                if (currentBossAction == 0xFF)
                    currentBossAction++;
                currentBossActionIsOver = 1;
                saveBossEncounter = 0xFF;
                break;

            case 46:
                remoteRayXToReach = -32000;
                set_main_and_sub_etat(&ray, 3, 7);
                fin_boss = true;
                finBosslevel.space_mama = true;
                smama_obj->nb_cmd = vblToEOA(&ray, 1) - 3;
                currentBossActionIsOver = 0;
                break;

            case 42:
                set_main_and_sub_etat(smama_obj, 0, 3);
                remoteRayXToReach = ray.x;
                skipToLabel(smama_obj, 0, true);
                smama_obj->nb_cmd = 0;
                currentBossActionIsOver = 1;
                break;

            case 44:
                {
                    switch (mach_obj->sub_etat) {
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                        set_main_and_sub_etat(smama_obj, 0, 16);
                        swapWeaponAnimState(wep_obj, 43);
                        set_sub_etat(mach_obj, mach_obj->sub_etat + 1);
                        skipToLabel(smama_obj, 0, true);
                        mach_obj->x = smama_obj->x;
                        mach_obj->y = smama_obj->y;
                        mach_obj->anim_frame = 0;
                        currentBossActionIsOver = 0;
                        break;

                    case 42:
                        wep_obj->flags.alive = 0;
                        bossEncounter = 6;
                        currentBossAction = 0;
                        currentBossActionIsOver = 1;
                        
                        break;
                    }
                }
                laserSourceSprNumInAnim = -1;
                break;

            case 40:
                    set_main_and_sub_etat(smama_obj, 0, 2);
                    bossYToReach = smama_obj->y;
                    if (poing_obj->speed_x != 0) {
                        skipToLabel(smama_obj, 0, true);
                        state_row = smama_obj->eta[0];
                        bossXToReach = state_row[2].speed_x_right * smama_obj->nb_cmd / 16;
                        if (poing_obj->speed_x < 0)
                            bossXToReach = smama_obj->x + bossXToReach;
                        else if (poing_obj->speed_x > 0)
                            bossXToReach = smama_obj->x - bossXToReach;

                        bossReachingAccuracyX = 0xFF;
                        bossReachingAccuracyY = 0xFF;
                        bossReachingTimer = 1;
                        setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                        skipToLabel(smama_obj, 1, true);
                    } else {
                        skipToLabel(smama_obj, 0, true);
                    }

                currentBossActionIsOver = 0;
                break;

            case 5:
                fix_numlevel(smama_obj);
                new_level = true;
                finBosslevel.space_mama = true;
                currentBossActionIsOver = 0;
                break;

            case 41:
                old_flip_x = smama_obj->flags.flip_x;
                calc_obj_dir(smama_obj);
                if (smama_obj->flags.flip_x != old_flip_x) {
                    set_main_and_sub_etat(smama_obj, 0, 4);
                    skipToLabel(smama_obj, 0, true);
                    currentBossActionIsOver = 0;
                } else {
                    currentBossActionIsOver = 1;
                }
                break;

            case 7:
                stepsForward = 0;
                currentPhaseHitCounter = 0;
                smama_obj->display_prio = 3;
                swapMereDenisCollZones(smama_obj, 1);
                set_main_and_sub_etat(smama_obj, 0, 44);
                setMereDenisAtScrollBorder(smama_obj, 0);
                smama_obj->flags.flip_x = 0;
                smama_obj->x = smama_obj->x - 144;
                smama_obj->y = (floorLine - smama_obj->offset_by) - 32;
                bossXToReach = (scroll_start_x - smama_obj->offset_bx) + 50;
                bossYToReach = smama_obj->y;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 8:
                swapMereDenisCollZones(smama_obj, 1);
                circle_index = 4;
                circle_x = smama_obj->x;
                circle_y = smama_obj->y - 60;
                setCirclePointToReach();
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 12:
                swapMereDenisCollZones(smama_obj, 1);
                bossXToReach = smama_obj->x + 110;
                bossYToReach = (floorLine - smama_obj->offset_by) - 124;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 38:
                swapMereDenisCollZones(smama_obj, 1);
                set_main_and_sub_etat(smama_obj, 2, 1);
                bossYToReach = floorLine - smama_obj->offset_by;
                bossXToReach = smama_obj->x;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 13:
                swapMereDenisCollZones(smama_obj, 1);
                set_main_and_sub_etat(smama_obj, 0, 10);
                wep_obj->x = smama_obj->x;
                wep_obj->y = smama_obj->y;
                mach_obj->x = smama_obj->x;
                mach_obj->y = smama_obj->y;
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 14:
                swapMereDenisCollZones(smama_obj, 0);
                {
                    set_main_and_sub_etat(mach_obj, 0, 37);
                    mach_obj->flags.alive = 1;
#ifdef PC
                    add_alwobj(mach_obj);
#endif
                    calc_obj_pos(mach_obj);
                }
                currentBossActionIsOver = 1;
                break;

            case 36: 
                swapMereDenisCollZones(smama_obj, 1);
                {
                    if (!mach_obj->flags.alive)
                        add_alwobj(mach_obj);
                    mach_obj->flags.alive = 1;
                }
                smama_obj->display_prio = 6;
                {
                    if (!wep_obj->flags.alive)
                        add_alwobj(wep_obj);
                    wep_obj->flags.alive = 1;
                }
                set_main_and_sub_etat(smama_obj, 0, 19);
                swapWeaponAnimState(wep_obj, 36);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 15:
                swapMereDenisCollZones(smama_obj, 0);
                if ((s16)myRand(1))
                    set_main_and_sub_etat(smama_obj, 0, 0);
                else
                    set_main_and_sub_etat(smama_obj, 0, 9);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 16:
                swapMereDenisCollZones(smama_obj, 0);
                stepsForward++;
                set_main_and_sub_etat(smama_obj, 1, 1);
                skipToLabel(smama_obj, 0, true);
                bossYToReach = smama_obj->y;
                state_row = smama_obj->eta[1];
                bossXToReach = smama_obj->nb_cmd * state_row[1].speed_x_right / 16;
                if (smama_obj->flags.flip_x)
                    bossXToReach = smama_obj->x + bossXToReach;
                else
                    bossXToReach = smama_obj->x - bossXToReach;
                bossReachingTimer = 1;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 26:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 5);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 18:
                swapMereDenisCollZones(smama_obj, 0);
                laserSourceSprNumInAnim = 5;
                currentLaserSize = 4;
                set_main_and_sub_etat(smama_obj, 0, 7);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 34:
                switch (smama_obj->sub_etat) {
                case 34:
                    set_sub_etat(smama_obj, 31);

                        swapWeaponAnimState(wep_obj, 33);
                    break;
                case 21:
                case 31:
                case 19:
                    set_sub_etat(smama_obj, 24);

                        swapWeaponAnimState(wep_obj, 25);
                    break;
                }
                goto block_123;

            case 35:
                switch (smama_obj->sub_etat) {
                case 22:
                    set_sub_etat(smama_obj, 23);

                        swapWeaponAnimState(wep_obj, 26);
                    break;
                case 21:
                case 23:
                case 19:
                    set_sub_etat(smama_obj, 30);

                        swapWeaponAnimState(wep_obj, 32);
                    break;
                }
block_123:
                swapMereDenisCollZones(smama_obj, 1);

                    wep_obj->flags.alive = 1;
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 32:
            case 33:
            case 43:
                swapMereDenisCollZones(smama_obj, 1);
                laserSourceSprNumInAnim = 8;
                currentLaserSize = mereDenisActionSequences[bossEncounter][currentBossAction++];
                switch (cur_act) {
                case 33:
                    set_main_and_sub_etat(smama_obj, 0, 22);

                        swapWeaponAnimState(wep_obj, 18);
                    break;
                case 32:
                    set_main_and_sub_etat(smama_obj, 0, 21);

                        swapWeaponAnimState(wep_obj, 17);
                    break;
                case 43:
                    set_main_and_sub_etat(smama_obj, 0, 34);

                        swapWeaponAnimState(wep_obj, 35);
                    break;
                }

                    wep_obj->flags.alive = 1;
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 19:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 8);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 21:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 11);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 39:
                swapMereDenisCollZones(smama_obj, 0);
                mach_obj->flags.alive = 1;
#ifdef PC
                add_alwobj(mach_obj);
#endif
                smama_obj->display_prio = 3;
                mach_obj->speed_y = -6;
                set_main_and_sub_etat(mach_obj, 2, 1);
                mach_obj->speed_x = mach_obj->eta[mach_obj->main_etat][mach_obj->sub_etat].speed_x_right;
                mach_obj->speed_x = -mach_obj->speed_x;
                set_main_and_sub_etat(smama_obj, 0, 14);
                smama_obj->x += 100;
                goto block_90;

            case 22:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 13);
block_90:
                bossXToReach = smama_obj->x;
                bossYToReach = scroll_end_y - smama_obj->offset_by - 16;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                if (cur_act == 22) {
                    bossReachingAccuracyX = 32;
                    bossReachingAccuracyY = 32;
                    bossReachingTimer = 2;
                    if (smama_obj->flags.flip_x)
                        bossXToReach += 80;
                    else
                        bossXToReach -= 80;
                }
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                break;

            case 30:
                smama_obj->flags.flip_x = !smama_obj->flags.flip_x;
                goto block_98;
            case 25:
                smama_obj->flags.flip_x = 1;
                goto block_98;
            case 23:
            case 45:
block_98:
                mere_denis_wait_time = 60;
                swapMereDenisCollZones(smama_obj, false);
                setMereDenisAtScrollBorder(smama_obj, smama_obj->flags.flip_x);
                if (cur_act == 45)
                {
                    {
                        /* COMPILER_STEERED: full-width sum changes instruction
                         * lengths before the six late switch targets.
                         * Local name INVENTED. DOS divergence starts 1:4217c. */
                        s32 center = ray.x + ray.offset_bx;
                        bossXToReach = center - smama_obj->offset_bx;
                    }
                    smama_obj->x = bossXToReach;
                }
                else if (cur_act == 25)
                {
                    bossXToReach = level.objects[machine_obj_id].x + 100;
                    smama_obj->x = bossXToReach + 80;
                }
                else /* cur_act == 30 || cur_act == 23 */
                {
                   if (smama_obj->flags.flip_x)
                    {
                        bossXToReach = smama_obj->x + 48;
                        smama_obj->x = bossXToReach + 80;
                    }
                    else
                    {
                        bossXToReach = smama_obj->x - 160;
                        /* DOS 1:42213 retains this redundant fallback predicate. */
                        if (cur_act == 25)
                            bossXToReach += 112;
                        smama_obj->x = bossXToReach - 80;
                    }
                }
                smama_obj->y = scroll_end_y - smama_obj->offset_by - 16;
                bossYToReach = smama_obj->y;
                bossReachingAccuracyX = 24;
                bossReachingAccuracyY = 24;
                bossReachingTimer = 1;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                bossYToReach = floorLine - smama_obj->offset_by;
                set_main_and_sub_etat(smama_obj, 0, 14);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = false;
                break;
            case 24:
                calc_obj_dir(smama_obj);
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 1);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 37:
                swapMereDenisCollZones(smama_obj, 1);

                    wep_obj->flags.alive = 0;

                    mach_obj->flags.alive = 0;
                set_main_and_sub_etat(smama_obj, 0, 6);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 31:
                swapMereDenisCollZones(smama_obj, 1);
                smama_obj->x -= 100;

                    mach_obj->flags.alive = 0;
                set_main_and_sub_etat(smama_obj, 0, 20);
                skipToLabel(smama_obj, 0, true);
                currentBossActionIsOver = 0;
                break;

            case 27:
                swapMereDenisCollZones(smama_obj, 0);
                set_main_and_sub_etat(smama_obj, 0, 15);
                bossXToReach = smama_obj->x;
                bossYToReach = scroll_end_y - smama_obj->offset_by - 16;
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                currentBossActionIsOver = 0;
                currentBombSequence = 0xFF;
                break;

            case 28:
                swapMereDenisCollZones(smama_obj, 0);
                setMereDenisAtScrollBorder(smama_obj, !smama_obj->flags.flip_x);
#ifdef PC
                if (smama_obj->flags.flip_x) {
                    smama_obj->x -= 144 - Bloc_lim_H1;
                    bossXToReach = scroll_end_x + SCREEN_WIDTH - smama_obj->offset_bx + 144;
                } else {
                    smama_obj->x += 136;
                    bossXToReach = scroll_start_x - smama_obj->offset_bx - 144;
                }
#else
                if (smama_obj->flags.flip_x) {
                    smama_obj->x -= 144;
                    bossXToReach = scroll_end_x + SCREEN_WIDTH - smama_obj->offset_bx + 144;
                } else {
                    smama_obj->x += 144;
                    bossXToReach = scroll_start_x - smama_obj->offset_bx - 144;
                }
#endif
                smama_obj->y = floorLine - smama_obj->offset_by - 32 - mereDenisActionSequences[bossEncounter][currentBossAction++];
                bossYToReach = smama_obj->y;
                set_main_and_sub_etat(smama_obj, 0, 15);
                bossReachingAccuracyX = 0xFF;
                bossReachingAccuracyY = 0xFF;
                bossReachingTimer = 1;
                setBossReachingSpeeds(smama_obj, 1, 0xFF, 0xFF);
                skipToLabel(smama_obj, 1, true);
                timerBeforeFirstBomb = 41;
                currentBombSequence = mereDenisActionSequences[bossEncounter][currentBossAction++];

                if (currentBombSequence == 0xFF) {
                    do {
                        do {
                            currentBombSequence = myRand(6);
                        } while (currentBombSequence == 1);
                    } while (currentBombSequence == 6);
                }

                for (i = 0; i < (s16)sizeof(lastDroppedBombIdInSequence); i++)
                    lastDroppedBombIdInSequence[i] = 0xFF;

                lastDroppedBombXCenterPos = -32000;
                currentBossActionIsOver = 0;
                break;

            case 29:
                swapMereDenisCollZones(smama_obj, 0);
                smama_obj->flags.flip_x = !smama_obj->flags.flip_x;
                set_main_and_sub_etat(smama_obj, 0, 0);
                skipToLabel(smama_obj, 0, true);
                mereDenisExplodeBombs();
                currentBossActionIsOver = 0;
                break;
            }
        }
    }
}

//525FC
void snapLaserToWeapon(obj_t *laser_obj, u8 param_2)
{
    obj_t *unk_obj = &level.objects[laser_obj->follow_id];
    s16 x; s16 y; s16 w; s16 h;
    u8 sub_etat;
    eta_t *eta;

    GET_SPRITE_POS(unk_obj, laser_obj->link, &x, &y, &w, &h);
    if (param_2)
    {
        eta = &unk_obj->eta[unk_obj->main_etat][unk_obj->sub_etat];
        switch (unk_obj->sub_etat)
        {
        case 17:
            eta->anim_index = 47;
            break;
        case 35:
            eta->anim_index = 48;
            break;
        case 18:
            eta->anim_index = 46;
            break;
        }
        unk_obj->anim_index = eta->anim_index;
        laser_obj->x = x - laser_obj->offset_bx;
        if (unk_obj->flags.flip_x)
        {
            laser_obj->cmd = GO_RIGHT; //for some reason it's inverted compared to the PS1 decomp? 
            laser_obj->x += w - 5;
        }
        else
        {
            laser_obj->cmd = GO_LEFT;
            laser_obj->x += 5;
        }
    }
    laser_obj->speed_y = 0;
    laser_obj->y = y + (h >> 1) - ((laser_obj->offset_by + laser_obj->offset_hy) / 2) + 2;
}

//52718
void allocateSpaceMamaLaser(obj_t *smama_obj) {
    obj_t *cur_obj;
    s16 i;
    s16 nb_objs;
    u8 sub_etat;
    u8 obj_type;
    switch (currentLaserSize) {
    case 1: sub_etat = 3; obj_type = TYPE_192_SMA_GRAND_LASER; break;
    case 2: sub_etat = 4; obj_type = TYPE_192_SMA_GRAND_LASER; break;
    case 3: sub_etat = 2; obj_type = TYPE_192_SMA_GRAND_LASER; break;
    case 4: sub_etat = 4; obj_type = TYPE_244_SMA_PETIT_LASER; break;
    }
    cur_obj = level.objects;
    nb_objs = level.nb_objects;
    for (i = 0; i < nb_objs; ++i, ++cur_obj) {
        if (cur_obj->type == obj_type && cur_obj->is_active == 0) {
            if (smama_obj->type == TYPE_213_SPACE_MAMA2)
                smama_obj = &level.objects[mereDenis_weapon_id];
            cur_obj->follow_id = smama_obj->id;
            cur_obj->link = laserSourceSprNumInAnim;
            cur_obj->flags.read_commands = 0;
            cur_obj->flags.flip_x = smama_obj->flags.flip_x;
            cur_obj->anim_frame = 0;
            cur_obj->sub_etat = sub_etat;
            smama_obj->follow_id = cur_obj->id;
            snapLaserToWeapon(cur_obj, 1);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            calc_obj_pos(cur_obj);
            break;
        }
    }
    laserSourceSprNumInAnim = -1;
    currentLaserSize = 0;
}

//52870
void doMereDenisCommand(obj_t* obj) {
    if (mere_denis_wait_time != 0) {
        mere_denis_wait_time--;
    } else { 
        if (mereDenisCanAttak(obj)) { 
            if (bossSafeTimer != 0) bossSafeTimer--;
            
            if (!(obj->flags.hurtbyfist)) { 
                 switch (obj->cmd) {
                 case GO_WAIT:
                     if (laserSourceSprNumInAnim != 0xFF) {
                         allocateSpaceMamaLaser(obj);
                     }
                     obj->speed_x = 0;
                     obj->speed_y = 0;
                     bossXToReach = -32000;
                     bossYToReach = -32000;
                     circle_index = -32000;
                    break;
                default:
                    laserSourceSprNumInAnim = -1;
                    if (circle_index != -32000) {
                        setCirclePointToReach();
                    } else if (timerBeforeFirstBomb == 0 && currentBombSequence != 0xFF &&
                               obj->main_etat == 0 && obj->sub_etat == 15) {
                        mereDenisDropBomb(obj);
                    } else if (timerBeforeFirstBomb != 0) {
                        timerBeforeFirstBomb--;
                    }
                    setBossReachingSpeeds(obj, bossReachingTimer, bossReachingAccuracyX, bossReachingAccuracyY);
                    break;
                }
                testActionEnd(obj);
            }
            prepareNewMereDenisAttack(obj);
        }
    }
    obj->flags.hurtbyfist = 0; 
}

//529A8
void changeMereDenisPhase(void) {
   u8 old_encounter;
    u8 *encounter;
    u8 *action;

    if (bossEncounter == 8 || bossEncounter == 9)
    {
        encounter = &saveBossEncounter;
        action = &saveCurrentBossAction;
    }
    else
    {
        encounter = &bossEncounter;
        action = &currentBossAction;
    }
    old_encounter = *encounter;

    switch (*encounter)
    {
    case 1:
        if (currentPhaseHitCounter >= 2 && stepsForward > 2)
        {
            *encounter = 2;
            stepsForward = 0;
        }
        break;
    case 2:
        if (currentPhaseHitCounter >= 3)
            *encounter = 3;
        break;
    case 3:
    case 4:
    case 5:
        break;
    }

    if (old_encounter != *encounter)
    {
        *action = 0;
        currentPhaseHitCounter = 0;
    }

}

//52A28
void fitSaveCurrentAction(void) {
    u8 again;
    do {
        again = 0;
        switch (mereDenisActionSequences[bossEncounter][currentBossAction]) {
        case 1: currentBossAction = 0; again = 1; break;
        case 0: currentBossAction = 0; bossEncounter++; again = 1; break;
        case 33:
        case 43: currentBossAction++;
        case 15: case 18: case 19: case 24: case 26: case 34: case 35: case 36:
            currentBossAction++; again = 1; break;
        case 28: currentBossAction += 2;
        case 29: again = 1; break;
        case 22:
            if (currentBossAction) { currentBossAction--; again = 1; }
            break;
        }
    } while (again);
    saveCurrentBossAction = currentBossAction;
    saveBossEncounter = bossEncounter;
}

//52AF4
void doMereDenisHit(obj_t* obj, s16 sprite) {
    if ((obj->eta[obj->main_etat][obj->sub_etat].flags & 1) && bossSafeTimer == 0) {
        s16 unk_1;
        switch (obj->main_etat * 0x100 + obj->sub_etat) {
            case 0x013:
            case 0x015:
            case 0x016:
            case 0x017:
            case 0x018:
            case 0x01e:
            case 0x01f:
            case 0x022:
                unk_1 = 255;
                break;
            case 0x002:
            case 0x006:
            case 0x00a:
            case 0x00b:
            case 0x00d:
            case 0x00f:
            case 0x025:
            case 0x02c:
                unk_1 = -1;
                break;
            default:
                unk_1 = 1;
                break;
            }
        if (unk_1 == sprite)
        {
            if (sprite == 1)
                {
                    poing.damage = 1;
                    obj_hurt(obj);
                    currentPhaseHitCounter++;
                    changeMereDenisPhase(); 
                }
            if (obj->hit_points != 0)
            {
                if (!(bossEncounter == 8 || bossEncounter == 9))
                {
                    fitSaveCurrentAction();
                    if (obj->type == TYPE_98_SPACE_MAMA)
                        bossEncounter = 8;
                    else
                        bossEncounter = 9;
                }
            }
            else {
             // PC version does triggerAction(15, 1);
                bossEncounter = 10;
                boss_mort = 1; // PC
            }

            currentBossAction = 0;
            bossSafeTimer = 0xFF;
            currentBossActionIsOver = true;
            obj->flags.hurtbyfist = 1;
        
        }


    }
    
}

//52C54
void mereDenisBigLaserCommand(obj_t *laser_obj)
{
    u8 max_frame;
    obj_t *unk_obj;
    eta_t *unk_eta;

    switch (laser_obj->sub_etat)
    {
    case 3:
    case 5:
        max_frame = 0;
        break;
    case 4:
        max_frame = 7;
        break;
    case 2:
        max_frame = 14;
        break;
    }

    if (laser_obj->anim_frame <= max_frame)
        snapLaserToWeapon(laser_obj, false);
    else
    {
        unk_obj = &level.objects[laser_obj->follow_id];
        if (unk_obj->follow_id == laser_obj->id) // field 4A? 
        {
            unk_eta = &unk_obj->eta[unk_obj->main_etat][unk_obj->sub_etat];
            switch (unk_obj->anim_index)
            {
            case 46:
                unk_eta->anim_index = 1;
                break;
            case 47:
                unk_eta->anim_index = 0;
                break;
            case 48:
                unk_eta->anim_index = 28;
                break;
            }
        }
    }
    SET_X_SPEED(laser_obj);
}


//52D10
void mereDenisBombCommand(obj_t* obj) {
    if (obj->timer != 0) {
        obj->timer--;
        if (obj->timer == 0) {
            allocateMereDenisBombChips(obj);
        }
    }
}
