/* objupdate.c, part 2 (1:512f0..1:53340). Names RAYVERSE/ANDROID.
 * Worker h_objupdate2: 48/51 STRICT, contiguous _TEXT; see NOTES.md.
 * Shared headers are unchanged. Descriptive local names and iabs are INVENTED. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"
#include <stdlib.h>

#define iabs(x) ((s32)(x) < 0 ? -(s32)(x) : (s32)(x))

#define GO_LEFT 0
#define GO_NOP 30
#define GO_RIGHT 1
#define GO_WAIT 2
#define MODE_3_MORT_DE_RAYMAN 3
#define MODE_4_MORT_DE_RAYMAN_ON_MS 4
#define TYPE_GRAINE 38
#define TYPE_0_BADGUY1 0
#define TYPE_9_BADGUY2 9
extern rayevts_t RayEvts;
extern u8 Vignet_To_Display;
extern s16 bonus_map;
extern s16 decalage_en_cours;
extern s16 departlevel;
extern u8 fin_boss;
extern finbosslevel_t finBosslevel;
extern s16 h_scroll_speed;
extern u8 horloge[25];
extern u16 fixontemp;
extern s16 id_Cling_1up;
extern s16 id_Cling_Pow;
extern s16 joe_exp_probleme;
extern s32 nb_wiz_save;
extern level_t level;
extern u8 life_becoz_wiz;
extern s16 loop_time;
extern s16 Mus_obj_id;
extern u8 nb_wiz;
extern u8 nb_wiz_collected;
extern s16 new_world;
extern u8 no_ray_landing_smoke;
extern s16 numero_palette_special;
extern s16 num_level;
extern s16 NumScrollObj;
extern s16 num_world;
extern poing_t poing;
extern obj_t *poing_obj;
extern s16 poing_obj_id;
extern u8 pal_id_at_wizard;
extern s16 ProchainEclair;
extern s16 prise_branchee;
extern rayevts_t SauveRayEvts;
extern obj_t ray;
extern s16 ray_mode;
extern s16 reduced_rayman_id;
extern s16 scroll_end_x;
extern s16 scroll_end_y;
extern s16 scroll_start_x;
extern s16 scroll_start_y;
extern u8 save_save;
extern save_state_t save1;
extern save_state_t save2;
extern status_bar_t status_bar;
extern u8 gele;
extern u8 ray_on_poelle;
extern u8 current_pal_id;
extern s16 vignet_joe_affichee;
extern s16 v_scroll_speed;
extern s16 xmap;
extern s16 xmap_old;
extern s16 ymap;
extern s16 ymapmax;
extern s16 ymap_old;
extern s16 ray_zdc_x;
extern s16 ray_zdc_y;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern active_objects_t actobj;
void Add_One_RAY_lives(void);
void CALC_MOV_ON_BLOC(obj_t* obj);
void ChangeLevel(void);
void DO_FADE_OUT(void);
void DISPLAY_GAME_VIGNET(void);
void DO_NOVA(obj_t* obj);
void DO_ONE_CMD(obj_t* obj);
u8 EOA(obj_t *obj);
void GET_ANIM_POS(obj_t* obj, s16* x, s16* y, s16* w, s16* h);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
s16 GetY(s16 x);  /* DOS caller DO_BOUEE_JOE_COMMAND needs s16 (PROVEN by match); src/special.c still defines s32 GetY (NEAR, OPEN) */
void INIT_FADE_OUT(void);
s16 NOVA_STATUS_BAR(void);
void PlaySnd(s16 snd, s16 obj_id);
void Reset_air_speed(u8 mode);
void RAY_DEMIRAY(void);
void allocateSupHelico(obj_t *obj);
void SET_X_SPEED(obj_t* obj);
void add_alwobj(obj_t* obj);
void calc_esquive_poing(obj_t* mit_obj, s16* out_diff_x, s16* out_diff_y, s16* out_unk);
s16 calc_obj_dir(obj_t* obj);
void calc_obj_pos(obj_t* obj);
void correct_gendoor_link(s16 value);
void fin_poing_follow(u8 value);
void fix_numlevel(obj_t *obj);
void restoreGameState(save_state_t *state);
void restore_gendoor_link(void);
void saveGameState(obj_t *obj, save_state_t *state);
void save_objects_flags(void);
void start_cd_gros_rayman(void);
u8 SelectButPressed(void);
extern u8 NBRE_SAVE;
void stop_all_snd(void);
void test_fin_cling(void);
s16 OBJ_IN_ZONE(obj_t *obj);
s16 inter_box(s32 x, s32 y, s32 w, s32 h, s16 x2, s16 y2, s32 w2, s32 h2);
void DO_INTERACT_PLAT(obj_t *obj);
void manage_snd_event(void);
void obj_hurt(obj_t* target);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
s32 set_sub_etat(obj_t* obj, u8 sub_etat);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void start_pix_gerbe(s16 x, s16 y);
void take_bonus(s16 obj_id);
void unleashMonsterHost(obj_t* obj);
void ObjectUTurnDefault(obj_t* obj);
void DO_WIZARD(obj_t *obj);
s16 get_next_bonus_level(u8 lvl);
void TEST_WIZARD(obj_t* obj);
void DO_TARZAN(obj_t* tz_obj);
void DoTarzanPoingCollision(obj_t* obj, s16 sprite);
void allocate_badguy(obj_t *obj, s16 which_enemy, s16 xspeed, s16 yspeed);
void DO_PTI_ESQUIVE(obj_t *bg3_obj);
void DoPrisePoingCollision(obj_t *obj, s16 sprite);
void DO_PETIT_COUTEAU_COMMAND(obj_t *obj);
void DoPetitCouteauPoingCollision(obj_t *obj, s16 sprite);
void DO_TIRE_BOUCHON_COMMAND(obj_t *obj);
void DoOneUpPoingCollision(obj_t* obj, s16 sprite);
void DoOneUpRaymanCollision(obj_t* obj);
void DoMorningStarPoingCollision(obj_t* obj, s16 sprite);
void DoGrapBonusRaymanCollision(obj_t* obj);
void DoPoingPowerupRaymanCollision(obj_t *obj);
void DoPowerupRaymanCollision(obj_t* obj);
void DoSuperHelicoRaymanCollision(obj_t* obj);
void DoGraineRaymanCollision(obj_t* obj);
void DoWizRaymanCollision(obj_t* obj);
void DoJaugeUpRaymanCollision(obj_t* obj);
void DoGeneratingDoorRaymanCollision(obj_t* obj);
void DO_REDUCTEUR(obj_t *obj);
void DoReducteurRaymanCollision(obj_t* obj);
void DoSignPostRaymanCollision(obj_t* obj);
void TEST_SIGNPOST(void);
void DoPancarteRaymanCollision(obj_t *obj);
void DO_MUSICIEN(obj_t *obj);
void DoMusicienRaymanInZDD(obj_t* obj);
void DO_TEN_COMMAND(obj_t *obj);
void DoGeneBadGuyPoingCollision(obj_t* obj, s16 sprite);
void DoGeneBadGuyRaymanZDD(obj_t* obj);
void DoChasseurPoingCollision(obj_t *obj, s16 sprite);
void DoChasseurRaymanZDD(obj_t* obj);
void DO_CHASSEUR_COMMAND(obj_t* obj);
void DoBadGuy23RaymanZDD(obj_t* obj);
void DoBadGuy23PoingCollision(obj_t *obj, s16 sprite);
void DoBadGuy1RaymanZDD(obj_t* obj);
void DoBadGuy1PoingCollision(obj_t *obj, s16 sprite);
void DO_CCL_COMMAND(obj_t* obj);
void DoCaisseClairePoingCollision(obj_t* obj, s16 sprite);
void DoCaisseClaireRaymanZDD(obj_t* obj);
void DoStalagRaymanZDD(obj_t* obj);
void DO_ENSEIGNE_COMMAND(obj_t *obj);
void DO_JOE_COMMAND(obj_t* obj);
void DoJoeRaymanZDD(obj_t* obj);
void DO_BOUEE_JOE_COMMAND(obj_t *obj);
void DO_PHOTOGRAPHE_CMD(obj_t *obj);
void DoAudioStartRaymanCollision(obj_t* obj);
void DO_SLOPEY_PLAT_CMD(obj_t *obj);

void ObjectUTurnDefault(obj_t* obj) {
    obj->flags.flip_x = !obj->flags.flip_x;
}

void DO_WIZARD(obj_t *obj)
{
    switch (obj->sub_etat) {
    case 1:
        if (status_bar.num_wiz >= 10) {
            obj->link = status_bar.num_wiz;
            set_sub_etat(obj, 2);
            set_main_and_sub_etat(&ray, 3, 9);
        } else {
            set_sub_etat(obj, 10);
            obj->flags.flip_x = 0;
        }
        break;
    case 3:
        set_main_and_sub_etat(&ray, 3, 10);
        if (horloge[2] == 0 && obj->link - status_bar.num_wiz < 10)
            status_bar.num_wiz--;
        break;
    case 5:
        if (ray.main_etat == 3 && ray.sub_etat != 11 && ray.sub_etat != 12)
            set_sub_etat(&ray, 11);

        if (obj->anim_frame >= obj->animations[obj->anim_index].frames_count - 1) {
            set_sub_etat(obj, 0);
            fix_numlevel(obj);
            save_objects_flags();
            ray.flags.hurtbyfist = 1;
            save_save = save1.is_just_saved;
            pal_id_at_wizard = current_pal_id;
            DO_FADE_OUT();
        }
        break;
    }
}

s16 get_next_bonus_level(u8 lvl)
{
    s16 res = 0;

    switch (num_world)
    {
    case 1:
        switch (lvl)
        {
        case 2:
            res = 21;
            break;
        case 4:
            res = 20;
            break;
        case 11:
            res = 18;
            break;
        case 12:
            res = 19;
            break;
        }
        break;
    case 2:
        switch (lvl)
        {
        case 4:
            res = 17;
            break;
        case 9:
            res = 18;
            break;
        }
        break;
    case 4:
        switch (lvl)
        {
        case 3:
            res = 12;
            break;
        case 9:
            res = 13;
            break;
        }
        break;
    case 5:
        switch (lvl)
        {
        case 2:
            res = 12;
            break;
        }
    case 6:
        break;
    }

    return res;
}

void TEST_WIZARD(obj_t* obj) {
    /* 375BC 8015BDBC -O2 -msoft-float */

    s16 obj_y; s16 obj_h; s16 obj_w; s16 obj_x;
    s32 x_diff_1;
    s16 x_diff_2;
    s16 x_diff_3;
    s32 y_diff_1;

    if (RayEvts.tiny || (!ray.flags.hurtbyfist && (!(ray.main_etat == 0 || ray.main_etat == 3) || nb_wiz_collected != 0))) {
        obj->detect_zone_flag = 0;
    }


    if (obj->sub_etat == 0) {
        if (ray.flags.hurtbyfist) {
            if (status_bar.num_wiz + nb_wiz_collected >= 10) {
                fixontemp = 300; // NOTE: aaded in PC version
                set_sub_etat(obj, 11);
                obj->timer = 200;
            }
            else {
                ray.flags.hurtbyfist = 0;
            }
        } else {
            if (obj->detect_zone_flag >= 1 && obj->anim_frame < 20) {
                if (status_bar.num_wiz >= 10) {
                    obj->anim_frame = 20;
                } else {
                    fixontemp = 150;
                    set_sub_etat(obj, 10);
                    obj->flags.flip_x = 0;
                }
            }
            // NOTE: small change here in PC version
            else if (obj->detect_zone_flag == 0 && obj->anim_frame == 19) {
                obj->anim_frame = 0;
                /* DOS +0x100 retests the assigned frame before clearing the flag. */
                if (obj->anim_frame == 0)
                    ray.flags.hurtbyfist = 0;
            }
        }
    } else if (obj->detect_zone_flag != 0) {
        if (ray.flags.hurtbyfist && obj->sub_etat == 11) {
            obj->timer--;
            if (obj->timer == 0) {
                ray.flags.hurtbyfist = 0;
                set_sub_etat(obj, 1);
            }
        } else if (obj->sub_etat == 1) {
            if ((ray.main_etat == 0 && (ray.sub_etat == 0 || ray.sub_etat == 1 || ray.sub_etat == 2)) ||
                 ray.main_etat == 1
            ) {
                GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);
                x_diff_1 = obj->x + obj->offset_bx - ray.x - ray.offset_bx;
                x_diff_2 = x_diff_1 - (obj_w >> 1);
                x_diff_3 = x_diff_1 + (obj_w >> 1);
                y_diff_1 = (ray.y + ray.offset_by) - obj_y - obj_h;
                if (iabs(y_diff_1) <= 8) {
                    if ((iabs(x_diff_2) <= 16 && ray.flags.flip_x) || (iabs(x_diff_3) <= 16 && !(ray.flags.flip_x))) {
                        if (iabs(x_diff_2) <= 16 && ray.flags.flip_x) {
                            obj->flags.flip_x = 0;
                            ray.x = obj->x + obj->offset_bx - ray.offset_bx - (obj_w >> 1) - 12;
                        } else if (iabs(x_diff_3) <= 16 && !(ray.flags.flip_x)) {
                            obj->flags.flip_x = 1;
                            ray.x = obj->x + obj->offset_bx - ray.offset_bx + (obj_w >> 1) + 12;
                        }
                        ray.y = ((ray.y + ray.offset_by) >> 4 << 4) - ray.offset_by;
                        ray.speed_x = 0;
                        decalage_en_cours = 0;
                        ray.speed_y = 0;
                        ray.link = 0;
                        DO_WIZARD(obj);
                    }
                }
            }
        } else {
            DO_WIZARD(obj);
        }
    } else if (!(obj->sub_etat == 10 || obj->detect_zone_flag != 0 || ray.main_etat == 3 || obj->sub_etat == 1)) {
        set_sub_etat(obj, 1);
    }
}

void DO_TARZAN(obj_t* tz_obj) {
    /* 398E8 8015E0E8 -O2 -msoft-float */

    s16 i;
    obj_t* cur_obj;
    s16 nb_objs;

    DO_ONE_CMD(tz_obj);
    if (tz_obj->main_etat == 0 && tz_obj->sub_etat == 4)
    {
        Vignet_To_Display = 1;
        i = 0;
        cur_obj = &level.objects[i];
        nb_objs = level.nb_objects;
        while (i < nb_objs) {
            if ((cur_obj->type == TYPE_GRAINE) && !cur_obj->is_active) {
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                add_alwobj(cur_obj);
                set_main_and_sub_etat(cur_obj, 2, 2);
                cur_obj->x = tz_obj->x + tz_obj->offset_bx + cur_obj->offset_bx;
                cur_obj->y = tz_obj->y + 50;
                cur_obj->speed_y = -2;
                cur_obj->speed_x = 1;
                calc_obj_pos(cur_obj);
                set_sub_etat(tz_obj, 3);
                tz_obj->speed_y = -8;
                break;
            }
            cur_obj++;
            i++;
        }
    }
}

void DoTarzanPoingCollision(obj_t* obj, s16 sprite) {
    set_main_and_sub_etat(obj, 0, 1);
    obj->cmd = GO_LEFT;
}

void allocate_badguy(obj_t *tentacle_obj, s16 which_enemy, s16 xspeed, s16 yspeed)
{
    s16 event_index = 0;
    obj_t *spawned = level.objects;
    s16 nb_objects = level.nb_objects;

    while (event_index < nb_objects) {
        if (((spawned->type == TYPE_0_BADGUY1 && which_enemy == 1) ||
             (spawned->type == TYPE_9_BADGUY2 && which_enemy == 2)) &&
            spawned->init_x <= 0 && spawned->is_active == 0) {
            spawned->flags.alive = 0;
            spawned->flags.flag_0x40 = 0;
            spawned->is_active = 1;
            spawned->flags.alive = 1;
            spawned->active_timer = 0;
            set_main_and_sub_etat(spawned, 2, 2);
            spawned->active_flag = 1;
            spawned->x = tentacle_obj->offset_bx + (tentacle_obj->x - spawned->offset_bx);
            spawned->y = tentacle_obj->y - (spawned->offset_by >> 2);
            calc_obj_dir(spawned);
            spawned->speed_y = yspeed;
            if (spawned->flags.flip_x) {
                spawned->speed_x = xspeed;
            } else {
                spawned->speed_x = xspeed;
                spawned->speed_x = -spawned->speed_x;
            }
            calc_obj_pos(spawned);
            spawned->flags.read_commands = 0;
            break;
        }
        spawned++;
        event_index++;
    }
}

void DO_PTI_ESQUIVE(obj_t *bg3_obj)
{
    s16 unk_1; s16 diff_y; s16 diff_x;

    DO_ONE_CMD(bg3_obj);
    if (poing.is_active || poing.is_charging)
    {
        calc_esquive_poing(bg3_obj, &diff_x, &diff_y, &unk_1);
        if (iabs(diff_x) <= unk_1 && diff_y < 15)
        {
            if (
                (
                    ((bg3_obj->main_etat == 0 && bg3_obj->sub_etat == 0) || (bg3_obj->main_etat == 0 && bg3_obj->sub_etat == 2)) ||
                    bg3_obj->main_etat == 1
                ) && (poing_obj->flags.flip_x) != (bg3_obj->flags.flip_x)
            )
            {
                bg3_obj->speed_x = 0;
                bg3_obj->speed_y = 0;
                set_main_and_sub_etat(bg3_obj, 0, 15);
                bg3_obj->flags.read_commands = 0;

                skipToLabel(bg3_obj, bg3_obj->flags.flip_x ? 3 : 2, 1);
            }
        } else if (bg3_obj->main_etat == 0 && bg3_obj->sub_etat == 16) {
            set_sub_etat(bg3_obj, 17);
        }
    } else if (bg3_obj->main_etat == 0 && bg3_obj->sub_etat == 16) {
        set_sub_etat(bg3_obj, 17);
    }
}

void DoPrisePoingCollision(obj_t *obj, s16 sprite)
{
    if (poing_obj->screen_x + 30 > obj->screen_x &&
        obj->main_etat == 0 && obj->sub_etat == 0) {
        skipToLabel(obj, 2, 1);
        prise_branchee = 1;
        finBosslevel.helped_joe_2 = 1;
        obj->init_sub_etat = 2;
    }
}

void DO_PETIT_COUTEAU_COMMAND(obj_t *obj)
{
    if (obj->hit_points <= 2)
        obj->anim_frame = obj->hit_points - 1;
    else
        obj->anim_frame = obj->hit_points - 3;
}

void DoPetitCouteauPoingCollision(obj_t *obj, s16 sprite)
{
    if (obj->hit_points == 2 || obj->hit_points == 4)
        obj->hit_points--;
}

void DO_TIRE_BOUCHON_COMMAND(obj_t *obj)
{
    DO_ONE_CMD(obj);
    obj->speed_x = 0;
    obj->speed_y = 0;
}

void DoOneUpPoingCollision(obj_t* obj, s16 sprite) {
    if (RayEvts.grap && ray_mode != MODE_3_MORT_DE_RAYMAN) {
        PlaySnd(194, obj->id);
        poing_obj->follow_id = obj->id;
    }
}

void DoOneUpRaymanCollision(obj_t* obj) {
    DO_NOVA(obj);
    PlaySnd(194, obj->id);
    if (id_Cling_1up != -1) {
        Add_One_RAY_lives();
        level.objects[id_Cling_1up].flags.alive = 0;
    }
    fixontemp = 300;
    id_Cling_1up = NOVA_STATUS_BAR();
    if (id_Cling_1up == -1) {
        Add_One_RAY_lives();
    }
    obj->flags.alive = 0;
    take_bonus(obj->id);
}

void DoMorningStarPoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == 4 && obj->sub_etat == 5) {
        if (poing_obj->speed_x >= 0) {
            if (poing_obj->speed_x > 0)
                set_sub_etat(obj, 9);
            else if (poing_obj->flags.flip_x)
                set_sub_etat(obj, 9);
            else
                set_sub_etat(obj, 6);
        } else {
            set_sub_etat(obj, 6);
        }
    }
}

void DoGrapBonusRaymanCollision(obj_t* obj) {
    DO_NOVA(obj);
    RayEvts.grap = 1;
    obj->flags.alive = 0;
}

void DoPoingPowerupRaymanCollision(obj_t *obj)
{
    if (obj->sub_etat == 7) {
        if (!RayEvts.poing) {
            poing_obj->init_sub_etat = 8;
        } else {
            switch (poing_obj->init_sub_etat) {
            case 1: poing_obj->init_sub_etat = 8; break;
            case 3: poing_obj->init_sub_etat = 10; break;
            case 5: poing_obj->init_sub_etat = 12; break;
            }
        }
    } else if (obj->sub_etat == 14) {
        if (!RayEvts.poing) {
            poing_obj->init_sub_etat = 1;
        } else {
            switch (poing_obj->init_sub_etat) {
            case 1: poing_obj->init_sub_etat = 3; break;
            case 3: poing_obj->init_sub_etat = 5; break;
            case 5: poing_obj->init_sub_etat = 5; break;
            case 8: poing_obj->init_sub_etat = 10; break;
            case 10: poing_obj->init_sub_etat = 12; break;
            case 12: poing_obj->init_sub_etat = 12; break;
            }
        }
    }
    DO_NOVA(obj);
    obj->flags.alive = 0;
    RayEvts.poing = 1;
    poing.sub_etat = poing_obj->init_sub_etat;
    save1.poing_sub_etat = poing.sub_etat;
    poing_obj->sub_etat = poing_obj->init_sub_etat;
    PlaySnd(11, obj->id);
}

void DoPowerupRaymanCollision(obj_t* obj) {
    DO_NOVA(obj);
    ray.hit_points += obj->hit_points;
    if (ray.hit_points > status_bar.max_hitp) {
        ray.hit_points = status_bar.max_hitp;
    }
    obj->flags.alive = 0;
    PlaySnd(8, obj->id);

}

void DoSuperHelicoRaymanCollision(obj_t* obj) {
    PlaySnd(213, obj->id);
    DO_NOVA(obj);
    RayEvts.super_helico = 1;
    obj->flags.alive = 0;
}

void DoGraineRaymanCollision(obj_t* obj) {
    RayEvts.magicseed = 1;
    obj->is_active = 0;
    obj->flags.alive = 0;
    PlaySnd(10, obj->id);
}

void DoWizRaymanCollision(obj_t* obj) {
    PlaySnd(14, obj->id);
    set_sub_etat(obj, 23);
    if (bonus_map) {
        --nb_wiz;
        ++nb_wiz_collected;
        status_bar.num_wiz =  nb_wiz_collected;
        if (nb_wiz == 0) {
            loop_time = -32;
            INIT_FADE_OUT();
        }
    } else {
        ++status_bar.num_wiz;
    }
    if (status_bar.num_wiz > 99) {
        DO_NOVA(obj);
        if (bonus_map) {
            id_Cling_1up = -1;
        } else {
            id_Cling_1up = NOVA_STATUS_BAR();
        }
        if (id_Cling_1up == -1) {
            Add_One_RAY_lives();
        }
        life_becoz_wiz = 1;
        status_bar.num_wiz -= 100;
    }
    start_pix_gerbe(obj->screen_x + 10, obj->screen_y + 10);
}

void DoJaugeUpRaymanCollision(obj_t* obj) {
    DO_NOVA(obj);
    if (id_Cling_Pow != -1) {
        ray.hit_points = 4;
        status_bar.max_hitp = 4;
        level.objects[id_Cling_Pow].flags.alive = 0;
    }
    fixontemp = 300;
    id_Cling_Pow = NOVA_STATUS_BAR();
    if (id_Cling_Pow == -1 ) {
        ray.hit_points = 4;
        status_bar.max_hitp = 4;
    }
    obj->flags.alive = 0;
    PlaySnd(12, obj->id);
    if (ray_mode != MODE_3_MORT_DE_RAYMAN && ray_mode != MODE_4_MORT_DE_RAYMAN_ON_MS) {
        ray.hit_points = 4;
        status_bar.max_hitp = 4;
    }
}

void DoGeneratingDoorRaymanCollision(obj_t* obj) {
    obj->is_active = 0;
    obj->flags.alive = 0;
    unleashMonsterHost(obj);
}

void DO_REDUCTEUR(obj_t *obj)
{
    if (obj->hit_points < obj->init_hit_points) {
        --obj->hit_points;
        if (obj->hit_points < 1)
            obj->hit_points = obj->init_hit_points;
    }
}

void DoReducteurRaymanCollision(obj_t* obj) {
    if (reduced_rayman_id != -1) {
        if (obj->hit_points == obj->init_hit_points) {
            DO_NOVA(obj);
            obj->speed_x = 0;
            obj->speed_y = 0;
            --obj->hit_points;
            RAY_DEMIRAY();
        }
    }
}

void DoSignPostRaymanCollision(obj_t* obj) {
    TEST_SIGNPOST();
    life_becoz_wiz = 0;
}

void TEST_SIGNPOST(void)
{
    s32 sub_not_8;

    if (ray.main_etat == 2 && (sub_not_8 = ray.sub_etat != 8)) {
        if (ray.speed_x != 0 && decalage_en_cours == 0)
            ray.x -= ray.speed_x;

        no_ray_landing_smoke = 1;
        ray.speed_x = 0;
        decalage_en_cours = 0;
        if (ray.speed_y < 0)
            ray.speed_y = 0;
    } else if ((ray.main_etat == 0 || ray.main_etat == 1) && ray.iframes_timer == -1 && gele == 0) {
        stop_all_snd();
        if (fin_boss) {
            set_main_and_sub_etat(&ray, 3, 7);
        } else {
            set_main_and_sub_etat(&ray, 3, 23);
            ray.anim_frame = 0;
        }
        if (poing.is_active) {
            poing_obj->flags.alive = 0;
            poing.is_active = 0;
            DO_NOVA(poing_obj);
            fin_poing_follow(0);
        }
        test_fin_cling();
        if (ray_on_poelle == 1) {
            RayEvts = SauveRayEvts;
            decalage_en_cours = 0;
            Reset_air_speed(0);
            Reset_air_speed(1);
            ray_on_poelle = 0;
        }
        if (id_Cling_1up != -1) {
            id_Cling_1up = -1;
            Add_One_RAY_lives();
            level.objects[id_Cling_1up].flags.alive = 0;
        }
        if (id_Cling_Pow != -1) {
            id_Cling_Pow = -1;
            ray.hit_points = 4;
            status_bar.max_hitp = 4;
            level.objects[id_Cling_Pow].flags.alive = 0;
        }
        decalage_en_cours = 0;
        ray.speed_x = 0;
        if (!fin_boss && !ray_on_poelle && num_world != 6)
            gele = 2;
        start_cd_gros_rayman();
    } else if ((ray.main_etat == 3 && ray.sub_etat == 23) || (ray.main_etat == 3 && ray.sub_etat == 7)) {
        if ((EOA(&ray) && !(fin_boss && num_world == 6)) ||
            (fin_boss && num_world == 6 && (SelectButPressed() || (NBRE_SAVE != 0 && EOA(&ray))))) {
            DO_FADE_OUT();
            ChangeLevel();
            gele = 0;
        } else {
            ray.speed_x = 0;
            ray.speed_y = 0;
            decalage_en_cours = 0;
            NumScrollObj = 0;
            if (ray.follow_id != -1) {
                level.objects[ray.follow_id].speed_y = 0;
                level.objects[ray.follow_id].speed_x = 0;
                level.objects[ray.follow_id].gravity_value_1 = 0;
                level.objects[ray.follow_id].gravity_value_2 = 0;
            }
        }
    } else if (ray.main_etat == 6) {
        if (ray.sub_etat != 14) {
            set_sub_etat(&ray, 14);
            ray.anim_frame = 0;
            start_cd_gros_rayman();
        }
        gele = 2;
        decalage_en_cours = 0;
        ray.speed_x = 0;
        ray.speed_y = 0;
        if (ray.sub_etat == 14 &&
            EOA(&ray)) {
            ChangeLevel();
            gele = 0;
        }
    }
    if (gele != 0) {
        h_scroll_speed = 0;
        xmap = xmap_old;
        v_scroll_speed = 0;
        ymap = ymap_old;
    }
}

void DoPancarteRaymanCollision(obj_t *obj)
{
    if (bonus_map) {
        status_bar.num_wiz = nb_wiz_save;
        nb_wiz_save = 0;
        departlevel = 0;
        fix_numlevel(&ray);
        stop_all_snd();
        DO_FADE_OUT();
    } else if ((ray.main_etat != 2 || ray.sub_etat != 8) &&
                ray.hit_points != 255 && gele == 0) {
        stop_all_snd();
        DO_FADE_OUT();
        restoreGameState(&save2);
        new_world = 1;
        if (life_becoz_wiz) {
            --status_bar.lives;
            if (status_bar.lives == -1)
                status_bar.lives = 0;
            life_becoz_wiz = 0;
        }
    }
}

void DO_MUSICIEN(obj_t *obj)
{
    if (obj->main_etat == 0 && obj->sub_etat == 4 &&
        EOA(obj) &&
        !finBosslevel.helped_musician) {
        finBosslevel.helped_musician = 1;
        allocateSupHelico(&level.objects[Mus_obj_id]);
        Vignet_To_Display = 1;
    }
}

void DoMusicienRaymanInZDD(obj_t* obj) {
    if (obj->sub_etat == 1) {
        set_sub_etat(obj, 5);
    }
}

void DO_TEN_COMMAND(obj_t *obj)
{
    u8 i;
    s16 unk_1;
    u8 unk_2;

    if (obj->cmd == GO_LEFT)
        obj->flags.flip_x = 0;
    else if (obj->cmd == GO_RIGHT)
        obj->flags.flip_x = 1;

    if (obj->main_etat == 0 && obj->sub_etat == 11)
    {
        if (obj->anim_frame >= 5 && obj->timer == 0)
        {
            obj->timer++;
            if (++obj->config > 2)
                obj->config = 1;

            if (obj->hit_points != 1)
                allocate_badguy(obj, obj->config, 1, -1);
            else
            {
                allocate_badguy(obj, 2U, 1, -2);
                allocate_badguy(obj, 1, 2, -1);
            }
        }
        else if (obj->anim_frame < 2)
            obj->timer = 0;
    }
    else if (obj->main_etat == 0 && obj->sub_etat == 21)
    {
        if (obj->anim_frame >= 5 && obj->timer == 0)
        {
            unk_1 = -4;
            i = 0;
            obj->timer++;
            while (i < 4)
            {
                unk_2 = (i % 2) + 1;
                allocate_badguy(obj, 3 - unk_2, unk_1, -unk_2);

                unk_1++;
                if (unk_1 == -2)
                    unk_1 = 3;
                i++;
            }
        }
        else if (obj->anim_frame < 2)
            obj->timer = 0;
    }
    else if (obj->main_etat == 0 && obj->sub_etat == 3)
    {
        obj->timer++;
        if (obj->config != 3 && obj->timer >= 253) {
            DO_INTERACT_PLAT(obj);
            obj->config = 3;
        }

        obj->speed_x = 0;
        obj->speed_y = 0;
        if (obj->anim_frame > 12)
            obj->anim_frame = 3;
        else if (obj->anim_frame <= 1)
            obj->timer = 0;
    }
    else if (obj->main_etat == 1)
    {
        if (obj->link != 0)
            obj->timer++;

        if (obj->timer >= 254)
            skipToLabel(obj, 2, 1);

        SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
    }
}

void DoGeneBadGuyPoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == 0 && !(obj->main_etat == 0 && obj->sub_etat == 1)) {
        poing.damage = 1;
        obj_hurt(obj);
        if (obj->hit_points != 0)
            skipToLabel(obj, 3, 1);
        else
            skipToLabel(obj, 4, 1);
    }
    else {
        skipToLabel(obj, 2, 1);
    }
}

void DoGeneBadGuyRaymanZDD(obj_t* obj) {
    obj->link = 1;
}

void DoChasseurPoingCollision(obj_t *obj, s16 sprite)
{
    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 1) {
        obj_hurt(obj);
        if (obj->hit_points != 0) {
            obj->speed_x = 0;
            obj->speed_y = 0;
            set_main_and_sub_etat(obj, 0, 1);
        } else {
            set_main_and_sub_etat(obj, 0, 3);
        }
        if (!obj->flags.flip_x)
            obj->cmd = GO_LEFT;
        else
            obj->cmd = GO_RIGHT;
    }
}

void DoChasseurRaymanZDD(obj_t* obj) {
    if (obj->main_etat == 0) {
        if (obj->sub_etat == 0 || obj->sub_etat == 2) {
            obj->speed_x = 0;
            obj->speed_y = 0;
            set_main_and_sub_etat(obj, 0, 2);
            obj->flags.read_commands = 0;
            calc_obj_dir(obj);
            if (obj->flags.flip_x)
                obj->cmd = GO_RIGHT;
            else
                obj->cmd = GO_LEFT;
        }
    }
}

void DO_CHASSEUR_COMMAND(obj_t* obj) {
    DO_ONE_CMD(obj);
    if (obj->detect_zone_flag == 0 && obj->main_etat == 0 && obj->sub_etat == 21) {
        if (obj->anim_frame >= obj->animations[obj->anim_index].frames_count - 1) {
            set_main_and_sub_etat(obj, 0, 0);
        }
    }
}

void DoBadGuy23PoingCollision(obj_t *obj, s16 sprite)
{
    if (sprite == 255 || (obj->eta[obj->main_etat][obj->sub_etat].flags & 1)) {
        obj_hurt(obj);
        if (obj->hit_points == 0) {
            set_main_and_sub_etat(obj, 0, 3);
            skipToLabel(obj, 2, 1);
            obj->y -= 2;
            obj->flags.read_commands = 0;
        } else {
            if (poing_obj->flags.flip_x) {
                if (poing_obj->speed_x > -1)
                    skipToLabel(obj, 3, 1);
                else if (poing_obj->speed_x < -1)
                    skipToLabel(obj, 2, 1);
            } else {
                if (poing_obj->speed_x > 1)
                    skipToLabel(obj, 3, 1);
                else if (poing_obj->speed_x < 1)
                    skipToLabel(obj, 2, 1);
            }
            obj->speed_x = 0;
            obj->y -= 2;
            obj->speed_y = -4;
            set_main_and_sub_etat(obj, 2, 2);
            PlaySnd(28, obj->id);
        }
    }
}

void DoBadGuy23RaymanZDD(obj_t* obj) {
    if (obj->main_etat == 1 && obj->sub_etat == 11) {
        set_sub_etat(obj, 14);
    } else if ((obj->main_etat == 0 && obj->sub_etat == 0) || (obj->main_etat == 1 && obj->sub_etat == 0)) {
        set_main_and_sub_etat(obj, 1, 11);
        obj->flags.read_commands = 0;
        skipToLabel(obj, obj->flags.flip_x ? 3 : 2, 1);
    }
}

void DoBadGuy1PoingCollision(obj_t *obj, s16 sprite)
{
    obj_hurt(obj);
    if (poing_obj->speed_x > 0)
        skipToLabel(obj, 3, 1);
    else if (poing_obj->speed_x < 0)
        skipToLabel(obj, 2, 1);
    obj->speed_x = 0;
    obj->y -= 2;
    if (obj->hit_points != 0) {
        obj->speed_y = -4;
        set_main_and_sub_etat(obj, 2, (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40) ? 10 : 2);
        PlaySnd(28, obj->id);
    } else {
        obj->speed_y = -8;
        set_main_and_sub_etat(obj, 0, (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40) ? 6 : 3);
    }
}

void DoBadGuy1RaymanZDD(obj_t* obj) {
    if (ray.flags.flip_x != obj->flags.flip_x) {
        if ((obj->main_etat == 1 && obj->sub_etat == 0) || (obj->main_etat == 0 && obj->sub_etat == 0)) {
            if (ray.main_etat == 0 && ray.sub_etat == 18 /* stick out tongue */) {
                obj->speed_x = 0;
                obj->speed_y = 0;
                set_main_and_sub_etat(obj, 0, 2);
                if (obj->flags.flip_x)
                    skipToLabel(obj, 8, 1);
                else
                    skipToLabel(obj, 7, 1);
            }
        }
    }
}

void DO_CCL_COMMAND(obj_t* obj) {
    /* 4D6EC 80171EEC -O2 */
    switch (obj->cmd) {
        case GO_WAIT:
        case GO_NOP:
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
            if (obj->main_etat == 0 && obj->sub_etat == 2) {
                if (obj->flags.flip_x != obj->hit_points) {
                    obj->flags.flip_x = (obj->hit_points & 1) ? 1 : 0;
                }
            }
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT) {
                obj->flags.flip_x = 0;
            } else {
                obj->flags.flip_x = 1;
            }
            SET_X_SPEED(obj);
            CALC_MOV_ON_BLOC(obj);
            break;
    }
}

void DoCaisseClairePoingCollision(obj_t* obj, s16 sprite) {
    if (!(obj->main_etat == 0 && obj->sub_etat == 2)) {
        set_main_and_sub_etat(obj, 1, 1);
        if (!obj->flags.flip_x) {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 3, 1);
        } else {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 2, 1);
        }
    }
}

void DoCaisseClaireRaymanZDD(obj_t* obj) {
    if (obj->main_etat == 0 && obj->sub_etat == 0) {
        set_main_and_sub_etat(obj, 1, 0);
        if (obj->flags.flip_x) {
            skipToLabel(obj, 3, 1);
        } else {
            skipToLabel(obj, 2, 1);
        }
    }
}

void DoStalagRaymanZDD(obj_t* obj) {
    if (obj->sub_etat == 4) {
        skipToLabel(obj, 99, 1);
    }
}

void DO_ENSEIGNE_COMMAND(obj_t *obj)
{
    DO_ONE_CMD(obj);
    if (prise_branchee)
    {
        if (obj->main_etat == 0 && obj->sub_etat == 0)
            skipToLabel(obj, 1, 1);

        if (
            obj->main_etat == 0 && obj->sub_etat == 1 &&
            obj->anim_frame >= obj->animations[1].frames_count - 1
        )
            skipToLabel(obj, 2, 1);

        if (
            obj->main_etat == 0 && obj->sub_etat == 2 &&
            obj->anim_frame >= obj->animations[2].frames_count - 1
        )
            skipToLabel(obj, 1, 1);
    }
    else
        skipToLabel(obj, 0, 1);
}

void DO_JOE_COMMAND(obj_t* obj) {
    // NOTE: this has some differences compared to the PS1 version
    if (num_level == 3) {
        scroll_end_y = ymapmax;
        scroll_start_y = ymapmax;
        ymap = ymapmax;
        scroll_end_x = 0;
        scroll_start_x = 0;
        xmap = 0;
    }

    if (joe_exp_probleme) {
        if (obj->main_etat == 0 && obj->sub_etat == 10) {
            vignet_joe_affichee = 1;
        }
        if (obj->main_etat == 0 && obj->sub_etat == 1) {
            if (vignet_joe_affichee) {
                vignet_joe_affichee = 0;
                finBosslevel.helped_joe_1 = 1;
                obj->timer = 0;
            }
            ++obj->timer;
            if (obj->timer == 100) {
                DISPLAY_GAME_VIGNET();
            } else if (obj->timer == 101) {
                ChangeLevel();
                RayEvts.firefly = 1;
            }
        }
    }

    if (num_level == 8) {
        if (prise_branchee) {
            if (obj->main_etat == 0 && obj->sub_etat == 1)
                skipToLabel(obj, 3, 1);
        } else {
            obj->flags.alive = 0;
        }
    }
}

void DoJoeRaymanZDD(obj_t* obj) {
    if (obj->main_etat == 0 && obj->sub_etat == 1 && !joe_exp_probleme) {
        vignet_joe_affichee = 0;
        skipToLabel(obj, 2, 1);
        joe_exp_probleme = 1;
    }
    if (obj->main_etat == 0 && obj->sub_etat == 2) {
        skipToLabel(obj, 4, 1);
    }
}

void DO_BOUEE_JOE_COMMAND(obj_t *obj)
{
    s16 ground_y = GetY(obj->x + obj->offset_bx - 12);

    obj->y = (ground_y - 43) + obj->iframes_timer;

    if (ray.follow_id == obj->id)
        obj->param = 13;
    else
        obj->param = 0;

    if (obj->iframes_timer < obj->param)
        obj->iframes_timer++;
    else if (obj->iframes_timer > obj->param)
        obj->iframes_timer--;
}

void DO_PHOTOGRAPHE_CMD(obj_t *obj)
{
    u8 anim_spd;
    s16 px, py;

    DO_ONE_CMD(obj);
    anim_spd = obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0x0f;
    if (obj->anim_index == 4 && obj->anim_frame == 10 && horloge[anim_spd] == 0)
        PlaySnd(40, obj->id);

    if (obj->sub_etat == 1) {
        if (++obj->timer == 0)
            obj->timer = 1;
    } else if (obj->timer != 0) {
        ProchainEclair = 1;
        numero_palette_special = 0;
        ray.is_active = 1;
        restore_gendoor_link();
        saveGameState(obj, &save1);
        correct_gendoor_link(1);
        obj->timer = 0;
    } else if (!RayEvts.tiny && !obj->flags.hurtbyfist && OBJ_IN_ZONE(obj) &&
               ray.follow_id == -1 && !decalage_en_cours &&
               ray.main_etat == 0 && ray.sub_etat == 0) {
        px = obj->x + 42;
        py = obj->y + 48;
        if (inter_box(px, py, 5, 15,
                      ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y)) {
            obj->flags.hurtbyfist = 1;
            ray.speed_x = 0;
            ray.is_active = 0;
            skipToLabel(obj, 0, 1);
        }
    }
}

void DoAudioStartRaymanCollision(obj_t* obj) {
    manage_snd_event();
}

void DO_SLOPEY_PLAT_CMD(obj_t *obj)
{
    s16 sprite_x;
    s16 sprite_h;
    s16 sprite_y;
    s16 sprite_w;
    s16 sign;
    s16 dist;
    sign = (s16)sgn((s8)obj->hit_points);
    if (ray.follow_id != obj->id) {
        if (horloge[10] == 0 && obj->hit_points != 0)
            obj->hit_points -= sign;
    } else if (horloge[8] == 0) {
        GET_SPRITE_POS(obj, obj->follow_sprite, &sprite_x, &sprite_y, &sprite_w, &sprite_h);
        sprite_y += obj->offset_hy;
        dist = ray.x + ray.offset_bx - (sprite_x + (sprite_w >> 1));
        dist = ashr16(dist, 3);
        obj->hit_points -= dist;
        if (iabs((s8)obj->hit_points) >= 16)
            obj->hit_points = sign * 15;
    }
}
