/* hybrid.c: split at original object boundary 1:2f040. Names RAYVERSE/ANDROID/PS1. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>
#ifndef FLG
#define FLG(x) (1 << x)
#endif
#ifndef ABS
#define ABS(x) (x >= 0 ? (x) : -(x))
#endif
#define GO_LEFT 0
#define GO_NOP 30
#define GO_RIGHT 1
#define GO_SPEED 20
#define GO_WAIT 2
#define OBJ_ACTIVE 11
#define SCREEN_WIDTH 320
#define TYPE_DARK2_PINK_FLY 108
#define TYPE_DARK_PHASE2 32
#define TYPE_DARK2_SORT 33
#define TYPE_FIRE_LEFT 209
#define TYPE_FIRE_RIGHT 210
#define TYPE_BOUM 11
#define TYPE_HYB_BBF2_D 231
#define TYPE_HYB_BBF2_G 232
#define TYPE_HYB_BBF2_LAS 233
#define TYPE_STOSKO_PINCE 224

extern u8 BBF2DEsk;
extern u8 BBF2GEsk;
extern obj_t *TirBBF2D;
extern obj_t *TirBBF2G;
extern s16 NiveauSol;
extern s16 PosArXToon1;
extern s16 PosArXToon2;
extern s16 PosArYToon1;
extern s16 PosArYToon2;
extern rayevts_t RayEvts;
extern u8 TextDark2_Affiche;
extern u8 ToonJustGivePoing;
extern s16 XText;
extern s16 YPosBBF2D;
extern s16 YPosBBF2G;
extern obj_t *dark_obj;
extern s16 phase_dark2;
extern s16 dark2_rayon_dx_1;
extern s16 dark2_rayon_dx_2;
extern s16 dark2_rayon_dy_1;
extern s16 dark2_rayon_dy_2;
extern s16 PosXSin1;
extern s16 PosYSin1;
extern s16 PosYSin2;
extern s16 PosXSin2;
extern s16 VitesseYText;
extern u8 num_dark2_phrase;
extern u8 sens_sinus_1;
extern u8 sens_sinus_2;
extern u8 sinus_actif;
extern s16 YText;
extern u8 bossReachingAccuracyX;
extern u8 bossReachingAccuracyY;
extern u8 bossReachingTimer;
extern s32 bossSpeedFactor;
extern s16 bossXToReach;
extern s16 bossYToReach;
extern s16 flamme_droite_id;
extern s16 flamme_gauche_id;
extern u8 flammes_actives;
extern u8 let_shadow;
extern finbosslevel_t finBosslevel;
extern s16 final_pass_n, final_pass_a, final_pass_f, final_pass_x;
extern u8 NBRE_SAVE, gele;
extern s16 expsin[63];
extern active_objects_t actobj;
extern level_t level;
extern s16 moskitomama_droite_obj_id;
extern s16 moskitomama_gauche_obj_id;
extern s16 moskitosaxo_obj_id;
extern obj_t *poing_obj;
extern obj_t ray;
extern poing_t poing;
extern s16 remoteRayXToReach;
extern s16 scroll_start_x;
extern s16 ray_mode;
extern u8 scrollLocked;
extern s16 scroll_start_x;
extern s16 scroll_end_x;
extern block_flags_t block_flags[64];
extern map_data_t mp;
extern u8 horloge[25];
extern s16 stosko_obj_id;
extern s16 temps_text;
extern char txt_dark2[100];
extern char *language_txt[300];
extern s16 xmap;
extern s16 xmapmax;
extern s16 ymap;
void DO_NOVA(obj_t* obj);
void DO_ONE_CMD(obj_t* obj);
void RAY_DEMIRAY(void);
s16 calc_obj_dir(obj_t* obj);
void calc_obj_pos(obj_t* obj);
void obj_hurt(obj_t *obj);
void RAY_HIT(u8 hurt, obj_t *obj);
void display_text_sin(const char* text, s16 x, s16 y, s16 phase, u8 font_size, u8 color);
s16 firstFloorBelow(obj_t* obj);
void setBossReachingSpeeds(obj_t* obj, u8 timer, u8 accuracyX, u8 accuracyY);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void add_alwobj(obj_t *obj);
u8 EOA(obj_t *obj);
void SET_X_SPEED(obj_t *obj);
s32 set_sub_etat(obj_t *obj, u8 sub_etat);
void goto_phase3(obj_t *obj);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
zdc_t *get_zdc(obj_t *obj, s16 index);
s16 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
s16 inter_box(s32 x1, s32 y1, s32 w1, s32 h1,
    s16 x2, s16 y2, s32 w2, s32 h2);
void allocateSTOSKO(void);
void allocateMOSKITOMAMA(void);
void allocateMOSKITOSAXO(void);
void doMOSAMScommand(obj_t *obj);
void DoMOSAMSPoingCollision(obj_t *obj, s16 sprite);
void allocateStoskoClaw(obj_t *stsk_obj);
void doSTOSKOcommand(obj_t *stsk_obj);
void DoSTOSKOPoingCollision(obj_t *obj, s16 sprite);
void DO_HYB_BBF2_POING_COLLISION(obj_t *obj, s16 sprite);
s16 OBJ_IN_COL_ZDC(obj_t *obj1, obj_t *obj2);
void allocate_DARK2_SORT(s16 x, s16 y, s16 sub_etat, s16 param_4);
void doBBF2command(obj_t *bbf2_obj);
void AllocateTirBBF2(obj_t *bbf2_obj);
void DoHybBBF2LasRaymanCollision(obj_t *obj);
void AllocateDarkPhase2(obj_t *obj);
void DO_DARK2_AFFICHE_TEXT(void);
void DoFlammeCommand(obj_t *obj);
void DoFlammeRaymanCollision(obj_t *obj);
void AllocateFlammes(s16 param_1);
void AllocateToons(void);
void DO_DARK2_TOONS_COMMAND(obj_t *d2fly_obj);
void ToonDonnePoing(obj_t *d2fly_obj);

void allocateSTOSKO(void)
{
    obj_t *obj = &level.objects[stosko_obj_id];

    obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
    obj->is_active = 1;
    obj->x = 160;
    obj->y = ymap - 50;
    obj->init_y = firstFloorBelow(obj) - obj->offset_by;
    obj->y = obj->init_y;
    obj->x = xmapmax + 304;
    obj->init_x = xmap + 320 - obj->offset_bx - 60;
    obj->timer = 50;
    obj->flags.flip_x = 0;
    RayEvts.poing = 1;
    poing_obj->init_sub_etat = 8;
}

void allocateMOSKITOMAMA(void)
{
    obj_t *boss_obj;

    /* DOS 1:2f126 loads NiveauSol at 2:50c20 through the -2 short-load addend. */
    YPosBBF2D =
        NiveauSol - (ray.offset_by - ray.offset_hy) + 90 -
        ((ray.x + 8 + ray.offset_bx - xmap) >> 1);
    boss_obj = &level.objects[moskitomama_droite_obj_id];
    boss_obj->x = 160;
    boss_obj->flags.flip_x = 0;
    boss_obj->init_x = boss_obj->x;
    boss_obj->init_y = boss_obj->y = ymap;
    NiveauSol = firstFloorBelow(boss_obj);
    boss_obj->speed_x = 0;
    boss_obj->speed_y = 0;
    boss_obj->init_x = boss_obj->x = 272 - boss_obj->offset_bx;
    boss_obj->init_y = boss_obj->y = ymap - 200;
    boss_obj->is_active = 1;
    boss_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
    BBF2DEsk = 0;

    YPosBBF2G =
        NiveauSol - (ray.offset_by - ray.offset_hy) - 70 +
        ((ray.x + 8 + ray.offset_bx - xmap) >> 1);
    boss_obj = &level.objects[moskitomama_gauche_obj_id];
    boss_obj->speed_x = 0;
    boss_obj->speed_y = 0;
    boss_obj->is_active = 1;
    boss_obj->init_y = boss_obj->y = ymap - 200;
    boss_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
    boss_obj->flags.flip_x = 1;
    boss_obj->init_x = boss_obj->x = 32 - boss_obj->offset_bx;
    BBF2GEsk = 0;

    RayEvts.poing = 1;
    poing_obj->init_sub_etat = 8;
}

/* allocateMOSKITOSAXO: no seed source */

void allocateMOSKITOSAXO(void)
{
    obj_t *obj = &level.objects[moskitosaxo_obj_id];

    if (!RayEvts.tiny)
    {
        DO_NOVA(&ray);
        RAY_DEMIRAY();
    }
    RayEvts.run = 1;
    obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
    obj->init_x = obj->x = xmap + 160 - obj->offset_bx;
    obj->init_y = obj->y = ymap - obj->offset_by;
    obj->is_active = 1;
    RayEvts.poing = 1;
    poing_obj->init_sub_etat = 8;
}

void doMOSAMScommand(obj_t *obj)
{
    scrollLocked = 1;
    switch (obj->cmd)
    {
    case GO_LEFT:
    case GO_RIGHT:
        obj->speed_x = 0;
        obj->speed_y = -8;
        scrollLocked = 0;
        break;
    case GO_WAIT:
        if (!(obj->main_etat == 0 && obj->sub_etat == 2))
            calc_obj_dir(obj);
        bossXToReach = -32000;
        bossYToReach = -32000;
        obj->speed_x = obj->speed_y = 0;
        break;
    case GO_SPEED:
        calc_obj_dir(obj);
        bossXToReach = ray.x + ray.offset_bx - obj->offset_bx;
        bossYToReach = firstFloorBelow(&ray) - obj->offset_by + 16;
        obj->speed_x = obj->iframes_timer;
        obj->speed_y = obj->follow_id;
        bossSpeedFactor =
            (((bossXToReach - obj->x) << 18) / 55) / obj->eta[obj->main_etat][obj->sub_etat].speed_x_right
        ;
        bossSpeedFactor = labs(bossSpeedFactor);
        bossReachingAccuracyX = -1; /* not sure if best represented as -1 or 0xff */
        bossReachingTimer = 1;
        obj->gravity_value_1 = 0;
        bossReachingAccuracyY = 0;
        obj->gravity_value_2 = 0;
        break;
    case GO_NOP:
        setBossReachingSpeeds(obj, bossReachingTimer, bossReachingAccuracyX, bossReachingAccuracyY);
        break;
    }
}

void DoMOSAMSPoingCollision(obj_t *obj, s16 sprite)
{
    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 1)
    {
        if (sprite == 3 || sprite == 6)
        {
            poing.damage = 1;
            obj_hurt(obj);
            if (obj->hit_points != 0)
                skipToLabel(obj, 4, 1);
            else
            {
                skipToLabel(obj, 2, 1);
                obj->param = 1;
            }
        }
    }
}

void allocateStoskoClaw(obj_t *stsk_obj)
{
    obj_t *cur_obj;
    s16 i;
    s16 nb_objs;

    stsk_obj->link = -1;
    cur_obj = level.objects;
    i = 0;
    nb_objs = level.nb_objects;
    while (i < nb_objs)
    {
        if (cur_obj->type == TYPE_STOSKO_PINCE && !(cur_obj->is_active))
        {
            cur_obj->x = stsk_obj->x;
            cur_obj->y = stsk_obj->y;
            cur_obj->speed_x = 0;
            calc_obj_pos(cur_obj);
            cur_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->cmd_offset = -1;
            cur_obj->nb_cmd = 0;
            stsk_obj->link = i;
            break;
        }
        cur_obj++;
        i++;
    }
}

void doSTOSKOcommand(obj_t *stsk_obj)
{
    s32 diff_y;
    obj_t *unk_obj;

    scrollLocked = 1;
    switch (stsk_obj->main_etat * 0x100 + stsk_obj->sub_etat)
    {
    case 0x100:
        if (stsk_obj->x <= stsk_obj->init_x)
        {
            set_main_and_sub_etat(stsk_obj, 0, 0);
            set_main_and_sub_etat(&ray, 0, 0);
            remoteRayXToReach = -32000;
        }
        else
        {
            remoteRayXToReach = scroll_start_x;
            if (ray.main_etat == 3 && ray.sub_etat == 20)
                ray.flags.flip_x = 1;
        }
        SET_X_SPEED(stsk_obj);
        break;
    case 0x8:
        stsk_obj->speed_x = 0;
        stsk_obj->speed_y = -8;
        break;
    case 0x0:
        if (poing.is_active || ray.main_etat == 2)
            set_sub_etat(stsk_obj, 5);
        else
        {
            if (stsk_obj->timer != 0)
                stsk_obj->timer--;
            else
                set_sub_etat(stsk_obj, 1);
        }
        break;
    case 0x1:
        if (poing.is_active)
        {
            diff_y = (s16)(ray.y + ray.offset_by - stsk_obj->y - stsk_obj->offset_by);
            if (
                ABS(diff_y) < 48 ||
                stsk_obj->anim_frame > 9
            )
            {
                set_sub_etat(stsk_obj, 10);
                stsk_obj->anim_index = stsk_obj->eta[0][10].anim_index;
            }
            else
                set_sub_etat(stsk_obj, 5);
        }
        else
        {
            if (EOA(stsk_obj))
                allocateStoskoClaw(stsk_obj);
        }
        break;
    case 0x4:
        stsk_obj->timer = 50;
    case 0x2:
        unk_obj = &level.objects[stsk_obj->link];
        if (unk_obj->speed_x > 0 && unk_obj->x >= stsk_obj->x)
        {
            stsk_obj->link = -1;
            unk_obj->flags.alive = 0;
            set_sub_etat(stsk_obj, 3);
        }
        break;
    case 0xD:
        stsk_obj->timer = 50;
        break;
    case 0x3:
    case 0x7:
    case 0xC:
        stsk_obj->timer = 50;
        break;
    case 0xB:
        if (!poing.is_active)
            set_sub_etat(stsk_obj, 12);
        break;
    case 0x6:
        if (!(ray.main_etat == 2 || poing.is_active))
            set_sub_etat(stsk_obj, 7);
        break;
    case 0x5:
    case 0x9:
        break;
    }
}

void DoSTOSKOPoingCollision(obj_t *obj, s16 sprite)
{
    if (
        sprite == 6 && !poing.is_returning &&
        (obj->eta[obj->main_etat][obj->sub_etat].flags & 1)
    )
    {
        poing.damage = 1;
        obj_hurt(obj);
        if (obj->hit_points != 0)
        {
            if (obj->sub_etat == 2)
                set_sub_etat(obj, 4);
            else
                set_sub_etat(obj, 13);
        }
        else
        {
            if (obj->sub_etat == 2)
            {
                level.objects[obj->link].flags.alive = 0;
                DO_NOVA(&level.objects[obj->link]);
            }
            set_sub_etat(obj, 8);
            obj->param = 1;
        }
    }
}

void doBBF2command(obj_t *bbf2_obj)
{
    s16 spr_x; s16 spr_y_1; s16 spr_w; s16 spr_h;
    s16 spr_y_2;
    s16 bbf2_y_1;
    s16 bbf2_y_3;
    s16 var_v1;
    u16 unk_2;

    if (!(bbf2_obj->main_etat == 0 && bbf2_obj->sub_etat == 2))
    {
        if (bbf2_obj->type == TYPE_HYB_BBF2_D)
        {
            if (BBF2DEsk == 0)
            {
                YPosBBF2D =
                    NiveauSol - (ray.offset_by - ray.offset_hy) + 90 -
                    (((ray.x + 8) + ray.offset_bx - xmap) >> 1);
            }
            else
            {
                if (BBF2DEsk == 100)
                {
                    bbf2_y_1 = bbf2_obj->y + ((bbf2_obj->offset_by + bbf2_obj->offset_hy) >> 1);
                    GET_SPRITE_POS(TirBBF2G, 8, &spr_x, &spr_y_1, &spr_w, &spr_h);
                    if (
                        (spr_y_1 + spr_h) < bbf2_y_1 &&
                        ((spr_y_1 + spr_h) > bbf2_obj->y + bbf2_obj->offset_hy - 0xA)
                    )
                        YPosBBF2D += 50;
                    else if (
                        spr_y_1 > bbf2_y_1 &&
                        (spr_y_1 < (bbf2_obj->y + bbf2_obj->offset_by + 0xA))
                    )
                        YPosBBF2D -= 50;
                    else if (
                        (spr_y_1 > ((bbf2_obj->y + bbf2_obj->offset_hy) - 0xA)) &&
                        ((spr_y_1 + spr_h) < (bbf2_obj->y + bbf2_obj->offset_by + 0xA))
                    )
                        YPosBBF2D -= 80;
                    else
                    {
                                YPosBBF2D =
                            NiveauSol - (ray.offset_by - ray.offset_hy) + 90 -
                            (((ray.x + 8) + ray.offset_bx - xmap) >> 1);
                    }
                }
                BBF2DEsk--;
            }
            unk_2 = YPosBBF2D - (bbf2_obj->y + 150);
        }
        else if (bbf2_obj->type == TYPE_HYB_BBF2_G)
        {
            if (BBF2GEsk == 0)
            {
                YPosBBF2G =
                    NiveauSol - (ray.offset_by - ray.offset_hy) - 70 +
                    (((ray.x + 8) + ray.offset_bx - xmap) >> 1);
            }
            else
            {
                if (BBF2GEsk == 100)
                {
                    bbf2_y_1 = bbf2_obj->y + ((bbf2_obj->offset_by + bbf2_obj->offset_hy) >> 1);
                    GET_SPRITE_POS(TirBBF2D, 8, &spr_x, &spr_y_1, &spr_w, &spr_h);
                    if (
                        (spr_y_1 + spr_h) < bbf2_y_1 &&
                        (spr_y_1 + spr_h) > bbf2_obj->y + bbf2_obj->offset_hy - 0xA
                    )
                        YPosBBF2G += 50;
                    else if (
                        spr_y_1 > bbf2_y_1 &&
                        spr_y_1 < bbf2_obj->y + bbf2_obj->offset_by + 0xA
                    )
                        YPosBBF2G -= 50;
                    else if (
                        spr_y_1 > bbf2_obj->y + bbf2_obj->offset_hy - 0xA &&
                        spr_y_1 + spr_h < bbf2_obj->y + bbf2_obj->offset_by + 0xA
                    )
                        YPosBBF2G -= 80;
                    else
                    {
                                YPosBBF2G =
                            NiveauSol - (ray.offset_by - ray.offset_hy) - 70 +
                            (((ray.x + 8) + ray.offset_bx - xmap) >> 1);
                    }
                }
                BBF2GEsk--;
            }
            unk_2 = YPosBBF2G - (bbf2_obj->y + 150);
        }

        if (
            ((bbf2_obj->y > ymap + 50) && (s16)unk_2 > 0) ||
            ((bbf2_obj->y < ymap - 150) && (s16)unk_2 < 0)
        )
        {
            unk_2 = 0;
            bbf2_obj->speed_y = 0;
        }

        var_v1 = (s16)unk_2;
        if (var_v1 != 0)
        {
            s32 speed_step = sgn(((s16)unk_2) - bbf2_obj->speed_y) +
                bbf2_obj->speed_y;
            bbf2_obj->speed_y = max32(-48, min32(48, speed_step));
        }
    }

    GET_SPRITE_POS(&ray, 5, &spr_x, &spr_y_2, &spr_w, &spr_h);
    spr_y_2 += 15;
    bbf2_y_3 = bbf2_obj->y + 150;
    if (
        bbf2_obj->sub_etat == 0 &&
        spr_y_2 > bbf2_y_3 - 15 && spr_y_2 < bbf2_y_3 + 15
    )
        set_sub_etat(bbf2_obj, 5);
    else if (
        (bbf2_obj->sub_etat == 6 || bbf2_obj->sub_etat == 4) &&
        (spr_y_2 < bbf2_y_3 - 20 || spr_y_2 > bbf2_y_3 + 20)
    )
        set_sub_etat(bbf2_obj, 7);
    else if (
        (bbf2_obj->sub_etat == 5 || bbf2_obj->sub_etat == 6) &&
        spr_y_2 > bbf2_y_3 - 7 && spr_y_2 < bbf2_y_3 + 7
    )
        set_sub_etat(bbf2_obj, 3);
    else if (bbf2_obj->sub_etat == 3 && bbf2_obj->anim_frame == 3)
    {
        AllocateTirBBF2(bbf2_obj);
        if (bbf2_obj->type == TYPE_HYB_BBF2_D)
            BBF2GEsk = 100;
        else
            BBF2DEsk = 100;
    }
}

void DO_HYB_BBF2_POING_COLLISION(obj_t *obj, s16 sprite)
{
    /* TODO: ??? switch instead didn't work */
    if (obj->sub_etat == 2)
    {}
    else if (obj->sub_etat == 1)
    {}
    else
    {
        if (level.objects[obj->link].sub_etat != 1)
        {
            obj->hit_points--;
            level.objects[obj->link].hit_points = obj->hit_points;
            if (obj->hit_points != 0)
                set_main_and_sub_etat(obj, 0, 1);
            else
            {
                set_main_and_sub_etat(obj, 0, 2);
                obj->speed_y = -128;
                obj->speed_x = 0;
                obj->flags.read_commands = 0;
                obj->param = 1;

                set_main_and_sub_etat(&level.objects[obj->link], 0, 2);
                level.objects[obj->link].speed_y = -128;
                level.objects[obj->link].speed_x = 0;
                level.objects[obj->link].flags.read_commands = 0;
                level.objects[obj->link].param = 1;
            }
        }
    }
}

void AllocateTirBBF2(obj_t *bbf2_obj)
{
    obj_t *cur_obj = &level.objects[0];
    s16 i = 0;
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs)
    {
        if (cur_obj->type == TYPE_HYB_BBF2_LAS && !cur_obj->is_active)
        {
            cur_obj->display_prio = 4;
            cur_obj->init_x = cur_obj->x = bbf2_obj->x;
            cur_obj->init_y = cur_obj->y = bbf2_obj->y;
            if (bbf2_obj->type == TYPE_HYB_BBF2_D)
            {
                cur_obj->speed_x = -5;
                TirBBF2D = cur_obj;
                cur_obj->flags.flip_x = 0;
            }
            else
            {
                cur_obj->speed_x = 5;
                TirBBF2G = cur_obj;
                cur_obj->flags.flip_x = 1;
            }
            cur_obj->speed_y = 0;
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
            add_alwobj(cur_obj);
            cur_obj->link = bbf2_obj->link;
            break;
        }
        cur_obj++;
        i++;
    }
}

void DO_HYB_BBF2_LAS(obj_t *las_obj)
{
    /* The type check and collision path independently form the owner address
     * at DOS 1:2ff67 and 1:2ff95. */
    obj_t *owner = &level.objects[las_obj->link];
    obj_t *unk_obj;
    obj_t *cur_obj;
    s16 i;
    s16 nb_objs;
    s16 j;
    /* Stack slots inferred from DOS 1:2ffeb..30014, not an order search. */
    s16 las_y, las_x, las_w, cur_y;
    s16 cur_h, las_h, cur_w, cur_x;

    if (owner->type == TYPE_HYB_BBF2_D ||
        owner->type == TYPE_HYB_BBF2_G)
    {
        unk_obj = &level.objects[las_obj->link];
        if (OBJ_IN_COL_ZDC(las_obj, unk_obj))
        {
            i = 0;
            cur_obj = &level.objects[i];
            nb_objs = level.nb_objects;
            while (i < nb_objs)
            {
                if (cur_obj->type == TYPE_BOUM && !cur_obj->is_active)
                {
                    GET_ANIM_POS(las_obj, &las_x, &las_y, &las_w, &las_h);
                    GET_ANIM_POS(cur_obj, &cur_x, &cur_y, &cur_w, &cur_h);
                    cur_obj->anim_frame = 0;
                    cur_obj->x = las_x - cur_obj->offset_bx;
                    if (las_obj->speed_x > 0)
                        cur_obj->x += las_w;
                    cur_obj->y = las_y + (las_h >> 1) -
                        ((cur_obj->offset_by + cur_obj->offset_hy) >> 1);
                    calc_obj_pos(cur_obj);
                    cur_obj->flags.alive = 1;
                    cur_obj->is_active = 1;
                    add_alwobj(cur_obj);
                    break;
                }
                cur_obj++;
                i++;
            }
            DO_HYB_BBF2_POING_COLLISION(unk_obj, 0);
            las_obj->is_active = 0;
            las_obj->flags.alive = 0;
        }
    }

    if (las_obj->speed_y == 0)
    {
        j = 0;
        unk_obj = &level.objects[actobj.objects[j]];
        while (j < actobj.num_active_objects)
        {
            if (unk_obj->type == TYPE_HYB_BBF2_LAS &&
                unk_obj->id != las_obj->id && OBJ_IN_COL_ZDC(las_obj, unk_obj))
            {
                unk_obj->speed_x = 0;
                las_obj->speed_x = 0;
                unk_obj->speed_y = 3;
                las_obj->speed_y = 3;
                las_obj->link = unk_obj->id;
                unk_obj->link = las_obj->id;
                break;
            }
            j++;
            unk_obj = &level.objects[actobj.objects[j]];
        }
    }
}

void DoHybBBF2LasRaymanCollision(obj_t *obj)
{
    obj->is_active = 0;
    obj->flags.alive = 0;
}

s16 OBJ_IN_COL_ZDC(obj_t *obj1, obj_t *obj2)
{
    /* Declaration order reproduces the DOS pointer spills and coordinate slots
     * at 1:3018c..1:301e4; keep the independent sprite outputs distinct. */
    zdc_t *zdc1;
    zdc_t *zdc2;
    s16 w1; s16 w2; s16 h;
    s16 x1; s16 y1; s16 x2;
    s16 y2;
    zdc1 = get_zdc(obj1, 0);
    zdc2 = get_zdc(obj2, 0);

    GET_SPRITE_POS(obj1, zdc1->sprite, &x1, &y1, &w1, &h);
    GET_SPRITE_POS(obj2, zdc2->sprite, &x2, &y2, &w2, &h);

    if (!obj1->flags.flip_x)
        x1 += zdc1->x_pos;
    else
        x1 = (x1 + w1) - (zdc1->x_pos + zdc1->width);

    if (!obj2->flags.flip_x)
        x2 += zdc2->x_pos;
    else
        x2 = (x2 + w2) - (zdc2->width + zdc2->x_pos);
    y1 += zdc1->y_pos;
    y2 += zdc2->y_pos;

    return inter_box(x1, y1, zdc1->width, zdc1->height,
        x2, y2, zdc2->width, zdc2->height);
}

/* AllocateDarkPhase2: no seed source */

void AllocateDarkPhase2(obj_t *mr_drk_obj)
{
    obj_t *cur_obj = &level.objects[0];
    s16 i = 0;
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs)
    {
        if (cur_obj->type == TYPE_DARK_PHASE2 && !cur_obj->is_active)
        {
            cur_obj->init_x = cur_obj->x = mr_drk_obj->x;
            cur_obj->init_y = cur_obj->y = mr_drk_obj->y;
            cur_obj->speed_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
            cur_obj->is_active = 1;
            set_main_and_sub_etat(cur_obj, 0, 38);

            dark_obj = mr_drk_obj;
            phase_dark2 = 0;
            mr_drk_obj->flags.alive = 0;
            mr_drk_obj->is_active = 0;
            break;
        }
        cur_obj++;
        i++;
    }
}

void DO_DARK2_AFFICHE_TEXT(void)
{
    s16 delta;
    u8 shift = 7;
    if (TextDark2_Affiche)
    {
        let_shadow = 1;
        if (finBosslevel.mr_dark)
        {
            if (NBRE_SAVE == 0)
            {
                if (final_pass_f == 0)
                {
                    if (final_pass_x > 66)
                    {
                        final_pass_a = final_pass_x;
                        final_pass_f = 1;
                    }
                    else if (final_pass_x > 40)
                        final_pass_x += 4;
                    else
                        final_pass_x += 2;
                }
                else
                {
                    delta = ashr16(expsin[final_pass_n], shift);
                    if (delta != 255)
                    {
                        final_pass_x += delta;
                        final_pass_n++;
                    }
                }
            }
        }
        else
        {
            display_text_sin(txt_dark2, XText, YText, temps_text, 2, 0);
            if (!gele)
                temps_text += 3;
        }
    }
}

void DO_DARK_PHASE2_COMMAND(obj_t *dp2_obj)
{
    s16 dp2_x, dp2_y, dp2_w, dp2_h;
    s16 ds2_x, ds2_y;

    if (dp2_obj->main_etat == 0 && dp2_obj->sub_etat == 38)
    {
        RayEvts.poing = 0;
        sinus_actif = 0;
        flammes_actives = 0;
        phase_dark2 = 0;
        TextDark2_Affiche = 0;
        scroll_end_x = 0;
        scroll_start_x = 0;
        ToonJustGivePoing = 0;
        dark_obj->flags.alive = 0;
        dark_obj->is_active = 0;
    }

    switch (phase_dark2)
    {
    case 0:
        if (sinus_actif == 0)
        {
            sinus_actif = 1;
            set_sub_etat(dp2_obj, 30);
        }
        else if (sinus_actif == 2)
            phase_dark2 = 1;
        break;
    case 1:
        if (flammes_actives == 0)
        {
            flammes_actives = 1;
            set_sub_etat(dp2_obj, 31);
        }
        else if (flammes_actives == 2 &&
            level.objects[flamme_gauche_id].cmd == GO_NOP &&
            level.objects[flamme_droite_id].cmd == GO_NOP)
        {
            skipToLabel(&level.objects[flamme_droite_id], 2, 1);
            skipToLabel(&level.objects[flamme_gauche_id], 2, 1);
            phase_dark2 = 2;
        }
        break;
    case 2:
        if (level.objects[flamme_gauche_id].cmd == GO_NOP &&
            level.objects[flamme_droite_id].cmd == GO_NOP)
        {
            phase_dark2 = 3;
            sinus_actif = 0;
        }
        break;
    case 3:
        if (sinus_actif == 0)
        {
            sinus_actif = 1;
            set_sub_etat(dp2_obj, 30);
        }
        else if (sinus_actif == 2)
            phase_dark2 = 4;
        break;
    case 4:
        skipToLabel(&level.objects[flamme_droite_id], 3, 1);
        skipToLabel(&level.objects[flamme_gauche_id], 3, 1);
        phase_dark2 = 5;
        break;
    case 5:
        if (level.objects[flamme_gauche_id].cmd == GO_NOP &&
            level.objects[flamme_droite_id].cmd == GO_NOP)
        {
            if (sinus_actif == 0)
            {
                sinus_actif = 1;
                set_sub_etat(dp2_obj, 30);
            }
            else if (sinus_actif == 2)
            {
                phase_dark2 = 6;
                sinus_actif = 0;
            }
        }
        else
            sinus_actif = 0;
        break;
    case 6:
        if (sinus_actif == 0)
        {
            skipToLabel(&level.objects[flamme_droite_id], 2, 1);
            skipToLabel(&level.objects[flamme_gauche_id], 2, 1);
            sinus_actif = 1;
            set_sub_etat(dp2_obj, 30);
        }
        else if (sinus_actif == 2 &&
            level.objects[flamme_gauche_id].cmd == GO_NOP &&
            level.objects[flamme_droite_id].cmd == GO_NOP)
        {
            phase_dark2 = 7;
            sinus_actif = 0;
        }
        break;
    case 7:
        if (dp2_obj->sub_etat != 33)
        {
            set_sub_etat(dp2_obj, 33);
            num_dark2_phrase = 0;
            TextDark2_Affiche = 1;
        }
        else if (num_dark2_phrase == 2)
        {
            set_sub_etat(dp2_obj, 32);
            phase_dark2 = 8;
            TextDark2_Affiche = 0;
        }
        break;
    case 8:
        skipToLabel(&level.objects[flamme_gauche_id], 4, 1);
        phase_dark2 = 9;
        PosArXToon1 = -1;
        break;
    case 9:
        if (RayEvts.poing)
            phase_dark2 = 10;
        else if (PosArXToon1 == -1 && level.objects[flamme_gauche_id].speed_x == 4)
        {
            AllocateToons();
            PosArXToon1 = poing_obj->x + 64;
            PosArYToon1 = poing_obj->y + 50;
            PosArXToon2 = poing_obj->x + 48;
            PosArYToon2 = poing_obj->y + 50;
        }
        break;
    case 10:
        set_main_and_sub_etat(&level.objects[flamme_gauche_id], 0, 4);
        set_main_and_sub_etat(&level.objects[flamme_droite_id], 0, 4);
        phase_dark2 = 11;
        break;
    case 11:
        /* 1:30a5b reads map_tile_t +2 (tile_type), not texture bits. */
        if (ToonJustGivePoing == 1 &&
            block_flags[mp.map[ray.ray_dist].tile_type].bits.solid &&
            !(ray.main_etat == 3 && ray.sub_etat == 23))
        {
            ToonJustGivePoing = 2;
            set_main_and_sub_etat(&ray, 3, 23);
            ray.speed_x = 0;
            ray.speed_y = 0;
        }
        if (!(level.objects[flamme_gauche_id].is_active ||
            (dp2_obj->main_etat == 0 && dp2_obj->sub_etat == 34) ||
            (dp2_obj->main_etat == 2 && dp2_obj->sub_etat == 1) ||
            (ray.main_etat == 3 && ray.sub_etat == 23) || ToonJustGivePoing != 2))
        {
            ToonJustGivePoing = 0;
            set_main_and_sub_etat(dp2_obj, 0, 34);
        }
        else if (dp2_obj->anim_frame == 4 &&
            dp2_obj->main_etat == 0 && dp2_obj->sub_etat == 34)
        {
            dp2_obj->speed_y = -4;
            phase_dark2 = 12;
            goto_phase3(dark_obj);
        }
        break;
    }

    if (dp2_obj->main_etat == 0)
    {
        switch (dp2_obj->sub_etat)
        {
        case 31:
            if (dp2_obj->anim_frame == 22)
            {
                GET_SPRITE_POS(dp2_obj, 3, &dp2_x, &dp2_y, &dp2_w, &dp2_h);
                dark2_rayon_dx_2 = 0;
                dark2_rayon_dx_1 = 0;
                dark2_rayon_dy_2 = -15;
                dark2_rayon_dy_1 = -15;
                ds2_x = dp2_x + (dp2_w >> 1) - 96;
                ds2_y = dp2_y + (dp2_h >> 1) - 128;
                allocate_DARK2_SORT(ds2_x, ds2_y, 37, 1);
                allocate_DARK2_SORT(ds2_x, ds2_y, 37, 0);
            }
            break;
        case 30:
            if (dp2_obj->anim_frame == 22)
            {
                GET_SPRITE_POS(dp2_obj, 3, &dp2_x, &dp2_y, &dp2_w, &dp2_h);
                dark2_rayon_dx_2 = -16;
                dark2_rayon_dx_1 = -16;
                dark2_rayon_dy_1 = 76;
                dark2_rayon_dy_2 = -76;
                sens_sinus_1 = 0;
                sens_sinus_2 = 1;
                PosXSin1 = PosXSin2 = dp2_x + (dp2_w >> 1) - 96;
                PosYSin1 = PosYSin2 = dp2_y + (dp2_h >> 1) - 128;
                allocate_DARK2_SORT(PosXSin1, PosYSin1, 35, 1);
                allocate_DARK2_SORT(PosXSin2, PosYSin2, 35, 0);
            }
            break;
        case 33:
            if (num_dark2_phrase == 0)
            {
                strcpy(txt_dark2, language_txt[95]);
                XText = SCREEN_WIDTH / 2;
                YText = -10;
                VitesseYText = 40;
                temps_text = 0;
                num_dark2_phrase = 1;
            }
            else if (YText < -20)
            {
                TextDark2_Affiche = 0;
                temps_text = 0;
                XText = SCREEN_WIDTH / 2;
                YText = -10;
                VitesseYText = 40;
                num_dark2_phrase++;
            }
            else
            {
                YText += ashr16(VitesseYText, 4);
                if (VitesseYText != 0)
                {
                    if (horloge[4] == 0)
                        VitesseYText--;
                }
                else if (horloge[6] == 0)
                    VitesseYText--;
            }
            break;
        }
    }
}

void DO_DARK2_SORT_COMMAND(obj_t *ds2_obj)
{
    s16 unk_1;
    s16 unk_y_2;
    s16 unk_x_2;

    if (ds2_obj->hit_points != 0)
    {
        if (ds2_obj->sub_etat == 35 || ds2_obj->sub_etat == 36)
        {
            if (-ds2_obj->offset_bx > ds2_obj->x)
            {
                sinus_actif = 2;
                ds2_obj->hit_points = 0;
            }
            else if (ds2_obj->iframes_timer != 0)
            {
                if (sens_sinus_1 != 0)
                {
                    dark2_rayon_dy_1 += 3;
                    if (dark2_rayon_dy_1 > 76)
                        sens_sinus_1 = 0;
                }
                else
                {
                    dark2_rayon_dy_1 -= 3;
                    if (dark2_rayon_dy_1 < -76)
                        sens_sinus_1 = 1;
                }
                PosXSin1 += ashr16(dark2_rayon_dx_1, 4);
                PosYSin1 += ashr16(dark2_rayon_dy_1, 4);
                if (ds2_obj->anim_frame == 1)
                {
                    allocate_DARK2_SORT(PosXSin1, PosYSin1,
                        ds2_obj->sub_etat, ds2_obj->iframes_timer);
                    ds2_obj->hit_points = 0;
                }
            }
            else
            {
                if (sens_sinus_2 != 0)
                {
                    dark2_rayon_dy_2 += 3;
                    if (dark2_rayon_dy_2 > 76)
                        sens_sinus_2 = 0;
                }
                else
                {
                    dark2_rayon_dy_2 -= 3;
                    if (dark2_rayon_dy_2 < -76)
                        sens_sinus_2 = 1;
                }
                PosXSin2 += ashr16(dark2_rayon_dx_2, 4);
                PosYSin2 += ashr16(dark2_rayon_dy_2, 4);
                if (ds2_obj->anim_frame == 1)
                {
                    allocate_DARK2_SORT(PosXSin2, PosYSin2,
                        ds2_obj->sub_etat, ds2_obj->iframes_timer);
                    ds2_obj->hit_points = 0;
                }
            }
        }
        else if (ds2_obj->sub_etat == 37)
        {
            if (ds2_obj->iframes_timer != 0)
                unk_x_2 = 240 - (ds2_obj->x + ds2_obj->offset_bx);
            else
                unk_x_2 = 5 - (ds2_obj->x + ds2_obj->offset_bx);
            {
                /* DOS 1:3107f: the center is accumulated at short width. */
                s16 center_y = ds2_obj->offset_by;
                s32 abs_x, abs_y;
                center_y += ds2_obj->y;
                unk_y_2 = firstFloorBelow(ds2_obj) - center_y;
                abs_x = unk_x_2;
                if (abs_x < 0)
                    abs_x = -abs_x;
                abs_y = unk_y_2;
                if (abs_y < 0)
                    abs_y = -abs_y;
                unk_1 = abs_x + abs_y;
            }

            if (unk_1 < 10)
            {
                ds2_obj->hit_points = 0;
                AllocateFlammes(ds2_obj->iframes_timer);
            }
            else
            {
                if (unk_1 > 0)
                {
                    unk_x_2 = ashl16(unk_x_2, 4) / unk_1;
                    unk_y_2 = ashl16(unk_y_2, 4) / unk_1;
                }

                if (ds2_obj->anim_frame == 1)
                {
                    if (ds2_obj->iframes_timer != 0)
                    {
                        dark2_rayon_dx_1 += sgn(unk_x_2 - dark2_rayon_dx_1);
                        dark2_rayon_dy_1 += sgn(unk_y_2 - dark2_rayon_dy_1);
                        allocate_DARK2_SORT(ds2_obj->x + dark2_rayon_dx_1,
                            ds2_obj->y + dark2_rayon_dy_1,
                            ds2_obj->sub_etat, ds2_obj->iframes_timer);
                        ds2_obj->hit_points = 0;
                    }
                    else
                    {
                        dark2_rayon_dx_2 += sgn(unk_x_2 - dark2_rayon_dx_2);
                        dark2_rayon_dy_2 += sgn(unk_y_2 - dark2_rayon_dy_2);
                        allocate_DARK2_SORT(ds2_obj->x + dark2_rayon_dx_2,
                            ds2_obj->y + dark2_rayon_dy_2,
                            ds2_obj->sub_etat, ds2_obj->iframes_timer);
                        ds2_obj->hit_points = 0;
                    }
                }
            }
        }
    }
}

void allocate_DARK2_SORT(s16 param_1, s16 param_2, s16 param_3, s16 param_4)
{
    obj_t *first;
    obj_t *cur_obj = level.objects;
    s16 found = 0;
    s16 i = 0;
    s16 nb_objs = level.nb_objects;

    do
    {
        if (cur_obj->type == TYPE_DARK2_SORT && !cur_obj->is_active)
            found = 1;
        cur_obj++;
        i++;
    } while (!found && i < nb_objs);
    cur_obj--;
    if (found)
    {
        first = cur_obj;
        found = 0;
        do
        {
            if (cur_obj->type == TYPE_DARK2_SORT && cur_obj->is_active == 1)
                found = 1;
            cur_obj++;
            i++;
        } while (!found && i < nb_objs);
        cur_obj--;

        if (!found)
        {
            found = 1;
            cur_obj = first;
        }
        else
        {
            found = 0;
            do
            {
                if (cur_obj->type == TYPE_DARK2_SORT && !cur_obj->is_active)
                    found = 1;
                cur_obj++;
                i++;
            } while (!found && i < nb_objs);
            cur_obj--;
            if (!found)
            {
                found = 1;
                cur_obj = first;
            }
        }

    }
    if (found)
    {
        cur_obj->flags.alive = 1;
        if (!cur_obj->is_active)
            add_alwobj(cur_obj);
        cur_obj->is_active = 1;
        cur_obj->hit_points = 1;
        cur_obj->x = param_1;
        cur_obj->y = param_2;
        cur_obj->iframes_timer = param_4;
        cur_obj->param = 0;
        set_main_and_sub_etat(cur_obj, 0, (u8)param_3);
    }
}

void DoFlammeCommand(obj_t *obj)
{
    if (obj->main_etat == 0 && obj->sub_etat == 5)
    {
        obj->is_active = 0;
        obj->flags.alive = 0;
    }
    else
        DO_ONE_CMD(obj);
}

void DoFlammeRaymanCollision(obj_t *obj)
{
    s16 ray_iframes = ray.iframes_timer;
    if (ray_iframes < 10)
    {
        if (ray_iframes == -1)
        {
            RAY_HIT(1, obj);
            if (ray_mode != 3)
                ray.iframes_timer = 10;
        }
        else
        {
            RAY_HIT(0, obj);
            ray.iframes_timer = ray_iframes;
        }
    }
    else
    {
        RAY_HIT(1, obj);
        if (ray_mode != 3)
            ray.iframes_timer = 10;
    }
}

void AllocateFlammes(s16 param_1)
{
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs)
    {
        if (
            param_1 == 0 &&
            cur_obj->type == TYPE_FIRE_LEFT && !(cur_obj->is_active)
        )
        {
            cur_obj->display_prio = 4;
            cur_obj->init_x =
            cur_obj->x = 5 - cur_obj->offset_bx;
            cur_obj->init_y =
            cur_obj->y = 100;
            cur_obj->init_y =
            cur_obj->y = firstFloorBelow(cur_obj) - cur_obj->offset_by;
            cur_obj->speed_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            flamme_gauche_id = cur_obj->id;
            flammes_actives = 2;
            break;
        }
        if (
            param_1 == 1 &&
            cur_obj->type == TYPE_FIRE_RIGHT && !(cur_obj->is_active)
        )
        {
            cur_obj->display_prio = 4;
            cur_obj->init_x =
            cur_obj->x = 240 - cur_obj->offset_bx;
            cur_obj->init_y =
            cur_obj->y = 100;
            cur_obj->init_y =
            cur_obj->y = firstFloorBelow(cur_obj) - cur_obj->offset_by;
            cur_obj->speed_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            flamme_droite_id = cur_obj->id;
            flammes_actives = 2;
            break;
        }
        cur_obj++;
        i++;
    }
}

void AllocateToons(void)
{
    s16 unk_1 = -1;
    s16 unk_2 = 0;
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs)
    {
        if (
            unk_2 == 0 &&
            cur_obj->type == TYPE_DARK2_PINK_FLY && !(cur_obj->is_active)
        )
        {
            cur_obj->display_prio = 4;
            cur_obj->init_x =
            cur_obj->x =
                SCREEN_WIDTH;
            cur_obj->init_y = 
            cur_obj->y =
                ymap + 100;
            cur_obj->speed_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->hit_points = 0;
            cur_obj->param = 0;

            unk_1 = i;
            unk_2 = 1;
        }
        else if (
            unk_2 == 1 &&
            cur_obj->type == TYPE_DARK2_PINK_FLY && !(cur_obj->is_active)
        )
        {
            cur_obj->display_prio = 4;
            cur_obj->init_x = 
            cur_obj->x =
                -20;
            cur_obj->init_y = 
            cur_obj->y =
                ymap + 100;
            cur_obj->speed_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->flags.alive = 1 | FLG(OBJ_ACTIVE);
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->hit_points = 1;
            cur_obj->param = 0;

            cur_obj->link = unk_1;
            level.objects[unk_1].link = i;
            break;
        }
        i++;
        cur_obj++;
    }
}

void DO_DARK2_TOONS_COMMAND(obj_t *d2fly_obj)
{
    s16 diff_x; s16 diff_y;
    s32 ray_x; s16 ray_y;
    obj_t *poing_obj_2;
    s16 dist;
    obj_t *unk_obj;
    s32 speed_y_step;
    s32 speed_x_step;

    if (d2fly_obj->hit_points == 0)
    {
        diff_x = PosArXToon1 - (d2fly_obj->x + d2fly_obj->offset_bx);
        diff_y = PosArYToon1 - (d2fly_obj->y + d2fly_obj->offset_by);
    }
    else
    {
        diff_x = PosArXToon2 - (d2fly_obj->x + d2fly_obj->offset_bx);
        diff_y = PosArYToon2 - (d2fly_obj->y + d2fly_obj->offset_by);
    }

    /* DOS re-forms the linked address after updating the fist position. */
    if (d2fly_obj->param != 0 &&
        level.objects[d2fly_obj->link].param != 0)
    {
        if (d2fly_obj->is_active && level.objects[d2fly_obj->link].is_active)
        {
            poing_obj->x = ((d2fly_obj->x + d2fly_obj->offset_bx +
                level.objects[d2fly_obj->link].x + level.objects[d2fly_obj->link].offset_bx) >> 1) - poing_obj->offset_bx;
            poing_obj->y = ((d2fly_obj->y + d2fly_obj->offset_by +
                level.objects[d2fly_obj->link].y + level.objects[d2fly_obj->link].offset_by) >> 1) - poing_obj->offset_by;
        }
        else if (d2fly_obj->is_active)
        {
            poing_obj->x = d2fly_obj->x + d2fly_obj->offset_bx - poing_obj->offset_bx;
            poing_obj->y = d2fly_obj->y + d2fly_obj->offset_by - poing_obj->offset_by;
        }
        else if (level.objects[d2fly_obj->link].is_active)
        {
            poing_obj->x = level.objects[d2fly_obj->link].offset_bx + level.objects[d2fly_obj->link].x - poing_obj->offset_bx;
            poing_obj->y = level.objects[d2fly_obj->link].offset_by + level.objects[d2fly_obj->link].y - poing_obj->offset_by;
        }

        /* DOS 1:317f2..3182d keeps the horizontal center at full width. */
        ray_x = ray.x + ray.offset_bx;
        PosArXToon1 = (d2fly_obj->offset_bx >> 1) + ray_x;
        ray_y = ray.y + (ray.offset_by >> 1);
        PosArYToon1 = ray_y;
        PosArXToon2 = ray_x - (d2fly_obj->offset_bx >> 1);
        PosArYToon2 = ray_y;
    }
    else
    {
        if (!RayEvts.poing)
        {
            poing_obj_2 = poing_obj;
            PosArXToon1 = poing_obj_2->x + 64;
            PosArYToon1 = poing_obj_2->y + 50;
            PosArXToon2 = poing_obj_2->x + 48;
            PosArYToon2 = poing_obj_2->y + 50;
        }
    }

    {
        s32 abs_x = diff_x;
        s32 abs_y;
        if (abs_x < 0)
            abs_x = -abs_x;
        abs_y = diff_y;
        if (abs_y < 0)
            abs_y = -abs_y;
        dist = abs_x + abs_y;
    }
    if (dist < 2)
    {
        if (d2fly_obj->param == 0)
        {
            d2fly_obj->param = 1;
            unk_obj = &level.objects[d2fly_obj->link];
            if (!(unk_obj->is_active))
                unk_obj->param = 1;
        }
    }
    if (dist > 0)
    {
        diff_x = ashl16(diff_x, 5) / dist;
        diff_y = ashl16(diff_y, 5) / dist;
    }

    speed_y_step = sgn(diff_y - d2fly_obj->speed_y) + d2fly_obj->speed_y;
    d2fly_obj->speed_y = max32(-40, min32(40, speed_y_step));
    speed_x_step = sgn(diff_x - d2fly_obj->speed_x) + d2fly_obj->speed_x;
    d2fly_obj->speed_x = max32(-40, min32(40, speed_x_step));

    if (d2fly_obj->speed_x > 0 && !(d2fly_obj->flags.flip_x))
        d2fly_obj->flags.flip_x = 1;
    else if (d2fly_obj->speed_x < 0 && d2fly_obj->flags.flip_x)
        d2fly_obj->flags.flip_x = 0;

    if (d2fly_obj->y < ymap - 200)
    {
        d2fly_obj->flags.alive = 0;
        d2fly_obj->is_active = 0;
    }
}

void ToonDonnePoing(obj_t *d2fly_obj)
{
    if (d2fly_obj->param != 0 && level.objects[d2fly_obj->link].param != 0)
    {
        d2fly_obj->param = 0;
        RayEvts.poing = 1;
        level.objects[d2fly_obj->link].param = 0;
        PosArXToon1 = 10;
        PosArXToon2 = 300;
        PosArYToon2 = PosArYToon1 = ymap - 200;
        DO_NOVA(poing_obj);
        poing_obj->is_active = 0;
        poing_obj->flags.alive = 0;
        poing_obj->init_sub_etat = 8;
        ToonJustGivePoing = 1;
    }
}
