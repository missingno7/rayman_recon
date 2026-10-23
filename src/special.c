#include "type_island_abi.h"
#include "map.h"
/*
 * Special level objects (GOG v1.21 object 1:6a340-1:6c3b0). File name invented (Rayverse special.c).
 * Worker candidate: 22/30 STRICT, contiguous _TEXT; see NOTES.md and final.json.
 * Names: Rayverse/Android; new arithmetic local names are INVENTED.
 */
#include "obj.h"
#include "game.h"
#include "rtypes.h"
#include "map.h"
#include <stddef.h>
#include <stdlib.h>

/* Rayverse's bool/enum aliases are adapted to Watcom C; constants retain DOS values. */
typedef s32 bool;
#define true 1
#define false 0
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define Abs(value) abs(value)
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])

#define TYPE_WIZARD1 5
#define TYPE_19_SPLASH 19
#define TYPE_13_BALLE1 13
#define TYPE_14_CHASSEUR2 14
#define TYPE_15_BALLE2 15
#define TYPE_24_INTERACTPLT 24
#define TYPE_39_NEN_GRAINE 39
#define TYPE_75_PAILLETTE 75
#define TYPE_83_EXPLOSION 83
#define TYPE_133_SUPERHELICO 133
#define TYPE_MARACAS_BAS 139
#define TYPE_143_NOVA2 143
#define TYPE_168_CYMBAL1 168
#define TYPE_221_CFUMEE 221
#define TYPE_222_CORDEBAS 222
#define TYPE_FLASH 146
#define BTYP_WATER 10
#define GO_UP 3
#define GO_DOWN 4
#define GO_SPEED 20
#define ANIMMODE_RESET 2

typedef struct pix_gerbe_item_t {
    s16 x_pos;
    s16 y_pos;
    s16 speed_x;
    s16 speed_y;
    u8 y_accel;
    u8 unk_1;
} pix_gerbe_item_t;

typedef struct pix_gerbe_t {
    pix_gerbe_item_t items[64];
    s16 is_active;
} pix_gerbe_t;

extern s16 ray_X_main_pos_init, ray_Y_main_pos_init, xmapsave, ymapsave;
extern s16 num_level_choice, new_world, new_level;
extern s16 departlevel, num_world, num_level, xmap, ymap;
extern s16 h_scroll_speed, v_scroll_speed, rayman_obj_id;
extern s16 pierreAcorde_obj_id, eau_obj_id, id_Cling_1up, id_Cling_Pow;
extern u8 Vignet_To_Display, You_Win, fin_du_jeu, fin_dark, NewMs, rayOnMsWasThere;
extern u8 Nb_total_cages;
extern s16 ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h;
extern obj_t ray;
extern obj_t *poing_obj;
extern level_t level;
extern level_obj_t level_alw;
extern world_info_t t_world_info[24];
extern status_bar_t status_bar;
extern rayevts_t RayEvts;
extern finbosslevel_t finBosslevel;
extern map_data_t mp;
extern block_flags_t block_flags[64];
extern s32 dword_BD96C;
extern s32 Bloc_lim_W1_Aff, Bloc_lim_W2_Aff, Bloc_lim_H2_Aff;
extern u8 angletab[65];
extern pix_gerbe_t pix_gerbe[8];
extern s16 EauDy[14], DecEau[4];
extern s16 *link_init;

extern void DISPLAY_GAME_VIGNET(void);
extern void make_active(obj_t *obj, s16 flags);
extern void add_alwobj(obj_t *obj);
extern void calc_obj_pos(obj_t *obj);
extern void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
extern void set_sub_etat(obj_t *obj, u8 sub_etat);
extern void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
extern void PlaySnd(s16 sound, s16 obj_id);
extern void GET_ANIM_POS(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
extern obj_t *findfirstObject(u16 type);
extern void del_alwobj(s16 obj_index);
extern void Add_One_RAY_lives(void);
extern s16 myRand(s16 max);
extern obj_t *allocateNOVA(void);
extern void DO_NOVA(obj_t *obj);
extern void fix_numlevel(obj_t *obj);
extern obj_t *oldest_planted(void);
extern obj_t *allocateExplosion(obj_t *obj);
extern s16 allocate_gerbe(void);

//7A340
void new_level_init(void) {
    ray_X_main_pos_init = ray.x;
    ray_Y_main_pos_init = ray.y;
    xmapsave = xmap;
    ymapsave = ymap;
    departlevel = 1;
}

//7A474
void fix_numlevel(obj_t* obj) {
    s16 handledLevelChoice = false;

    if (departlevel)
    {
        new_level_init();
        if (obj->type != TYPE_WIZARD1)
    {
        switch(num_world)
        {
            // Jungle
            case 1:
                // Betila: fist
                if (num_level == 2 && (RayEvts.poing) != 0)
                    num_level_choice++;

                // Boss: Bzzit
                if (num_level == 5 && (finBosslevel.bzzit) != 0)
                    num_level_choice++;

                // Betilla: hang
                if (num_level == 7 && (RayEvts.hang) != 0)
                    num_level = 8;

                // Boss: Moskito chase
                if (num_level == 13 && (finBosslevel.moskito) != 0)
                    num_level_choice++;

                // Boss: Moskito
                if (num_level == 15 && (finBosslevel.moskito) != 0)
                    new_world = 1;

                // Betilla: grab
                if (num_level == 16 && (RayEvts.grap) != 0)
                    new_world = 1;
                break;

                // Music
            case 2:
                // Boss: Red Drummer (unused)
                if (num_level == 5 && (finBosslevel.crazy_drummer) != 0)
                    new_world = 1;

                // Boss: Sax encounter
                if (num_level == 9 && (finBosslevel.mr_sax) != 0)
                    new_world = 1;

                // Betilla: helico
                if (num_level == 10 && (RayEvts.helico) != 0)
                    num_level = 11;

                // Boss: Sax
                if (num_level == 14 && (finBosslevel.mr_sax) != 0)
                    new_world = 1;
                break;

                // Mountain
            case 3:
                // Boss: Stone chase
                if (num_level == 1 && (finBosslevel.mr_stone) != 0)
                    new_world = 1;

                // Boss: Stone
                if ((num_level == 9) && (finBosslevel.mr_stone) != 0)
                    new_world = 1;

                // Betilla: run
                if (num_level == 10 && (RayEvts.run) != 0)
                    num_level = 11;
                break;

                // Image
            case 4:
                // Boss: Pirate Mama
                if (num_level == 3 && (finBosslevel.viking_mama) != 0)
                    new_world = 1;

                // Boss: Space Mama
                if (num_level == 10 && (finBosslevel.space_mama) != 0)
                    new_world = 1;
                break;

                // Cave
            case 5:
                // Boss: Skops
                if (num_level == 9 && (finBosslevel.mr_skops) != 0)
                    new_world = 1;
                break;

                // Cake
            case 6:
                // Boss: Dark (unused)
                if (num_level == 3 && (finBosslevel.mr_dark) != 0)
                    new_world = 1;
                break;
        }
    }

    }
    switch(num_world)
    {
        // Jungle
        case 1:
            switch((u16)num_level)
            {
                // Bonus: Pink Plant Woods 2
                case 2:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 21;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End & bonus: Pink Plant Woods 4
                case 4:
                    // Bonus
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 20;
                        handledLevelChoice = 1;
                    }
                        // End
                    else
                    {
                        new_world = 1;

                        // Unlock Anguish Lagoon and save point
                        if ((t_world_info[1].state & 1) == 0)
                        {
                            t_world_info[1].state |= 4;
                            t_world_info[18].state |= 4;
                        }
                    }
                    break;

                    // End: Anguish Lagoon
                case 8:
                    new_world = 1;

                    // Unlock The Swamps of Forgetfulness
                    if ((t_world_info[2].state & 1) == 0)
                        t_world_info[2].state |= 4;

                    // Unlock Bongo Hills
                    if ((t_world_info[4].state & 1) == 0)
                        t_world_info[4].state |= 4;
                    break;

                    // End & bonus: The Swamps of Forgetfulness 3
                case 11:
                    // Bonus
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 18;
                        handledLevelChoice = 1;
                    }
                        // End
                    else
                    {
                        new_world = 1;

                        // Unlock Moskito's Nest
                        if ((t_world_info[3].state & 1) == 0)
                            t_world_info[3].state |= 4;
                    }
                    break;

                    // Bonus: Moskito's Nest 1
                case 12:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 19;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Moskito's Nest
                case 17:
                    new_world = 1;
                    break;

                    // End: Bonus 1
                case 18:
                    num_level_choice = 11;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 19:
                    num_level_choice = 12;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 3
                case 20:
                    num_level_choice = 4;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 4
                case 21:
                    num_level_choice = 2;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Music
        case 2:
            switch((u16)num_level)
            {
                // Bonus: Bongo Hills 4
                case 4:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 17;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Bongo Hills
                case 6:
                    new_world = 1;

                    // Unlock Allegro Presto and save point
                    if ((t_world_info[5].state & 1) == 0)
                    {
                        t_world_info[5].state |= 4;
                        t_world_info[19].state |= 4;
                    }
                    break;

                    // Bonus: Allegro Presto 3
                case 9:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 18;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Allegro Presto
                case 11:
                    new_world = 1;

                    // Unlock Gong Heights
                    if ((t_world_info[6].state & 1) == 0)
                        t_world_info[6].state |= 4;

                    // Unlock Twilight Gulch
                    if ((t_world_info[8].state & 1) == 0)
                        t_world_info[8].state |= 4;
                    break;

                    // End: Gong Heights
                case 13:
                    new_world = 1;

                    // Unlock Mr Sax's Hullaballoo and save point
                    if ((t_world_info[7].state & 1) == 0)
                    {
                        t_world_info[7].state |= 4;
                        t_world_info[20].state |= 4;
                    }
                    break;

                    // End: Mr Sax's Hullaballoo
                case 16:
                    new_world = 1;
                    break;

                    // End: Bonus 1
                case 17:
                    num_level_choice = 4;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 18:
                    num_level_choice = 9;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Mountain
        case 3:
            switch((u16)num_level)
            {
                // End: Twilight Gulch
                case 2:
                    new_world = 1;

                    // Unlock The Hard Rocks
                    if ((t_world_info[9].state & 1) == 0)
                        t_world_info[9].state |= 4;
                    break;

                    // Bonus: The Hard Rocks 2
                case 4:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 12;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: The Hard Rocks
                case 5:
                    new_world = 1;

                    // Unlock Mr Stone's Peaks and save point
                    if ((t_world_info[10].state & 1) == 0)
                    {
                        t_world_info[10].state |= 4;
                        t_world_info[21].state |= 4;
                    }
                    break;

                    // Bonus: Mr Stone's Peaks 4
                case 9:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 13;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Mr Stone's Peaks
                case 11:
                    new_world = 1;

                    // Unlock Eraser Plains
                    if ((t_world_info[11].state & 1) == 0)
                        t_world_info[11].state |= 4;
                    break;

                    // End: Bonus 1
                case 12:
                    num_level_choice = 4;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 13:
                    num_level_choice = 9;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Image
        case 4:
            switch((u16)num_level)
            {
                // Bonus: Eraser Plains 3
                case 3:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 12;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Eraser Plains
                case 4:
                    new_world = 1;

                    // Unlock Pencil Pentathlon
                    if ((t_world_info[12].state & 1) == 0)
                        t_world_info[12].state |= 4;
                    break;

                    // End: Pencil Pentathlon
                case 7:
                    new_world = 1;

                    // Unlock Space Mama's Crater
                    if ((t_world_info[13].state & 1) == 0)
                        t_world_info[13].state |= 4;
                    break;

                    // Bonus: Space Mama's Crater 2
                case 9:
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 13;
                        handledLevelChoice = 1;
                    }
                    break;

                    // End: Space Mama's Crater
                case 11:
                    new_world = 1;

                    // Unlock Crystal Palace and save point
                    if ((t_world_info[14].state & 1) == 0)
                    {
                        DISPLAY_GAME_VIGNET();
                        t_world_info[14].state |= 4;
                        t_world_info[22].state |= 4;
                    }
                    break;

                    // End: Bonus 1
                case 12:
                    num_level_choice = 3;
                    handledLevelChoice = 1;
                    break;

                    // End: Bonus 2
                case 13:
                    num_level_choice = 9;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Cave
        case 5:
            switch ((u16)num_level)
            {
                // End & bonus: Crystal Palace 2
                case 2:
                    // Bonus
                    if (obj->type == TYPE_WIZARD1)
                    {
                        num_level_choice = 12;
                        handledLevelChoice = 1;
                    }
                        // End
                    else
                    {
                        new_world = 1;

                        // Unlock Eat at Joe's
                        if ((t_world_info[15].state & 1) == 0) {
                            t_world_info[15].state |= 4;
                            t_world_info[22].state |= 4;
                        }
                    }
                    break;

                    // End: Eat at Joe's
                case 8:
                    new_world = 1;

                    // Unlock Mr Skops' Stalactites and save point
                    if ((t_world_info[16].state & 1) == 0)
                    {
                        t_world_info[16].state |= 4;
                        t_world_info[23].state |= 4;
                    }
                    break;

                    // End: Mr Skops' Stalactites
                case 11:
                    new_world = 1;

                    // Show cutscene
                    DISPLAY_GAME_VIGNET();
                    break;

                    // End: Bonus 1
                case 12:
                    num_level_choice = 2;
                    handledLevelChoice = 1;
                    break;
            }
            break;

            // Cake
        case 6:
            // End: Mr Dark's Dare
            switch (num_level) {
            case 4:
            {
                new_world = 1;
                You_Win = 1;
                fin_dark = 1;
                fin_du_jeu = 1;
                break;
            }
            }
            break;
    }

    // Unlock Mr Dark's Dare
    if ((t_world_info[17].state & 1) == 0 && Nb_total_cages >= 102)
        t_world_info[17].state |= 4;

    // If we're in the same world and the level choice has not already been
    // handled then we increment the level by 1 to go to the next map
    if (!new_world && !handledLevelChoice)
        num_level_choice++;

    new_level = 1;
}

//7AD20
void allocate_splash(obj_t* obj) {
    s16 i;
    s16 nb_objs;
    obj_t* cur_obj;
    i = 0;
    cur_obj = level.objects;
    nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (cur_obj->type == TYPE_19_SPLASH && !cur_obj->is_active) {
            cur_obj->flags.alive = 1;
            make_active(cur_obj, 0);
            if (cur_obj->is_active) {
                add_alwobj(cur_obj);
            }
            cur_obj->x = obj->x + obj->offset_bx - cur_obj->offset_bx;
            {
                s32 y;
                s32 offset_by;
                if (obj->btypes[0] == BTYP_WATER) {
                    offset_by = cur_obj->offset_by;
                    y = obj->offset_by + obj->y;
                } else if (!(((u8 *)block_flags)[obj->btypes[0]] & 2)) {
                    y = (mp.height - 1) * 16;
                    offset_by = cur_obj->offset_by;
                } else {
                    offset_by = cur_obj->offset_by;
                    y = (mp.height - 1) * 16 + 10;
                }
                cur_obj->y = y - offset_by;
            }
            calc_obj_pos(cur_obj);
            set_main_and_sub_etat(cur_obj, 0, 0);
            cur_obj->anim_frame = 0;
            obj->flags.flag_0x40 = 1;
            break;
        }
        ++i;
        ++cur_obj;
    }
}

//7AE1C
void DO_BALLE(obj_t* obj) {
    eta_t* eta = get_eta(obj);
    if (obj->type == TYPE_15_BALLE2) {
        eta->speed_x_left = -1;
        eta->speed_x_right = 1;
        eta->anim_speed &= 0xF;
        eta->anim_speed |= 0xA0; // TODO: change to bitfield?
    } else if (obj->type == TYPE_13_BALLE1) {
        eta->speed_x_left = -2;
        eta->speed_x_right = 2;
        eta->anim_speed = (eta->anim_speed & 0xF);
    }
}

//7AE78
void DO_TIR(obj_t* obj) {
    obj_t *cur_obj;
    s16 i, nb_objs;
    s16 distance;
    s32 speed_y; /* Extent-correct hypothesis: narrow only at the object-field store. */
    s32 x, numerator;

    if (obj->main_etat == 0 && obj->sub_etat == 11) {
        if (obj->link == 0) {
            cur_obj = level.objects;
            i = 0;
            nb_objs = level.nb_objects;
            while (i < nb_objs) {
                if (((cur_obj->type == TYPE_13_BALLE1) || (cur_obj->type == TYPE_15_BALLE2)) && !cur_obj->is_active) {
                    cur_obj->flags.flip_x = obj->flags.flip_x;
                    cur_obj->flags.alive = 1;
                    cur_obj->is_active = 1;
                    add_alwobj(cur_obj);
                    set_main_and_sub_etat(cur_obj, 2, 2);
                    cur_obj->anim_frame = 0;
                    cur_obj->anim_index = (u8) cur_obj->eta[2][2].anim_index;
                    x = obj->x;
                    x += obj->offset_bx;
                    x -= cur_obj->offset_bx;
                    x += cur_obj->flags.flip_x ? 64 : -64;
                    cur_obj->x = x;
                    cur_obj->y = obj->y;
                    cur_obj->init_x = cur_obj->x;
                    cur_obj->init_y = cur_obj->y;
                    calc_obj_pos(cur_obj);
                    PlaySnd(24, cur_obj->id);
                    if (cur_obj->flags.flip_x) {
                        skipToLabel(cur_obj, 3, 1);
                    } else {
                        skipToLabel(cur_obj, 1, 1);
                    }

                    if (obj->type == TYPE_14_CHASSEUR2) {
                        distance = ((obj->x - ray.x) < 0 ? -(obj->x - ray.x) : (obj->x - ray.x)) >> 5;
                        cur_obj->type = TYPE_15_BALLE2;
                        numerator = ray.y;
                        numerator -= obj->y;
                        numerator *= 2;

                        speed_y = numerator / ((obj->x - ray.x) < 0 ? -(obj->x - ray.x) : (obj->x - ray.x)) - (s16)distance;
                        cur_obj->speed_y = speed_y;
                        cur_obj->gravity_value_2 = 10;
                    } else {
                        cur_obj->type = TYPE_13_BALLE1;
                        cur_obj->speed_y = 0;
                    }
                    calc_obj_pos(cur_obj);
                    break;
                }
                cur_obj += 1;
                i = i + 1;
            }
            obj->link = 1;
            return;
        }
    } else if (obj->main_etat == 0 && obj->sub_etat == 2) {
        obj->link = 0;
    }
}

//7B060
void DO_INTERACT_PLAT(obj_t* obj) {
    s16 linked_id = link_init[obj->id];
    if (linked_id != obj->id) {
        obj_t* found_obj;
        s16 diff_y;
        found_obj = level.objects + linked_id;
        found_obj->type = TYPE_24_INTERACTPLT;
        found_obj->flags.read_commands = 0;

        diff_y = ((s16)found_obj->y + found_obj->offset_by) - (s16)obj->y - obj->offset_by;
        found_obj->cmd = diff_y <= 0 ? GO_DOWN : GO_UP;
        found_obj->nb_cmd = Abs(diff_y);
    }
}

//7B0E0
obj_t* oldest_planted(void) {
    /* 39A14 8015E214 -O2 -msoft-float */
    s16 ng_ids[2];
    s16 unk_1;
    u8 cnt_2 = 0;
    s16 cnt_1 = 0;
    obj_t* cur_obj = &level.objects[0];
    s16 nb_objs = level.nb_objects;

    while (cnt_1 < nb_objs) {
        if (cur_obj->type == TYPE_39_NEN_GRAINE) {
            ng_ids[cnt_2++] = cur_obj->id;
            if (!cur_obj->is_active) {
                cur_obj->iframes_timer = 10;
            } else {
                cur_obj->iframes_timer++;
            }

            if (cnt_2 >= LEN(ng_ids)) {
                break;
            }
        }
        cur_obj++;
        cnt_1++;
    }

    unk_1 = ng_ids[0];
    cur_obj = &level.objects[unk_1];
    cnt_1 = unk_1;
    cnt_2 = 0;
    while (cnt_2 < LEN(ng_ids)) {
        if (level.objects[cnt_1].iframes_timer > level.objects[unk_1].iframes_timer) {
            unk_1 = cnt_1;
            cur_obj = &level.objects[unk_1];
        }
        cnt_2++;
        cnt_1 = ng_ids[cnt_2];
    }

    cur_obj->iframes_timer = 1;
    return cur_obj;
}

//7B1A0
void DO_GROWING_PLATFORM(void) {
    obj_t* oldest_obj;
    u8 main_etat; u8 sub_etat;
    s32 frames_count;

    if (dword_BD96C != true)
        dword_BD96C = false;

    frames_count = ray.animations[ray.anim_index].frames_count;
    if (ray.anim_frame == frames_count - 1)
    {
        if (dword_BD96C == false)
        {
            oldest_obj = oldest_planted();
            if (oldest_obj->is_active) {
                DO_NOVA(oldest_obj);
            } else {
                add_alwobj(oldest_obj);
            }

            oldest_obj->flags.alive = 1;
            oldest_obj->is_active = 1;
            main_etat = 0;
            sub_etat = 38;
            set_main_and_sub_etat(oldest_obj, main_etat, sub_etat);
            oldest_obj->anim_frame = 0;
            oldest_obj->anim_index = oldest_obj->eta[0][38].anim_index;
            oldest_obj->x = (ray.x + ray.offset_bx) - oldest_obj->offset_bx;
            oldest_obj->y = (ray.y + ray.offset_by) - oldest_obj->offset_by;
            calc_obj_pos(oldest_obj);
            dword_BD96C = true;
        }
    }
    else
        dword_BD96C = false;
}

//7B29C
obj_t* allocateExplosion(obj_t* obj) {
    obj_t *result = NULL;
    {
        s16 i = 0;
        obj_t *cur_obj = level.objects;
        s16 nb_objs = level.nb_objects;
        while (i < nb_objs) {
        if (cur_obj->type == TYPE_83_EXPLOSION && !cur_obj->is_active) {
            set_main_and_sub_etat(cur_obj, 0, 0);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->x = obj->x + obj->offset_bx - cur_obj->offset_bx;
            cur_obj->y = obj->y + obj->offset_by - cur_obj->offset_by + 10;
            calc_obj_pos(cur_obj);
            cur_obj->anim_index = get_eta(cur_obj)->anim_index;
            cur_obj->anim_frame = 0;
            cur_obj->change_anim_mode = ANIMMODE_RESET;
            result = cur_obj;
            break;
        }
        ++cur_obj;
        ++i;
    }
    }
    return result;
}

//7B370
void BombExplosion(obj_t* obj) {
    obj_t* explosion_obj = allocateExplosion(obj);
    obj->flags.alive = 0;
    obj->is_active = 0;
}

//7B390
void MARACAS_GO(obj_t* obj) {
    /* 3AB5C 8015F35C -O2 */
    s16 i = 0;
    obj_t* cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if ((cur_obj->type == TYPE_MARACAS_BAS) && !cur_obj->is_active) {
            cur_obj->flags.flip_x = obj->flags.flip_x;
            cur_obj->speed_y = 0;
            cur_obj->speed_x = 0;
            cur_obj->x = obj->x;
            cur_obj->y = obj->y;
            calc_obj_pos(cur_obj);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->gravity_value_1 = 0;
            cur_obj->gravity_value_2 = 7;
            break;
        }
        cur_obj++;
        i++;
    }

    skipToLabel(obj, 99, true);
    if (obj->cmd == GO_SPEED) {
        obj->speed_x = cur_obj->iframes_timer;
        obj->speed_y = cur_obj->follow_id;
    }
    set_main_and_sub_etat(obj, 0, 12);
}

//7B470
s16 ANGLE_RAYMAN(obj_t* obj) {
    /* 3AC88 8015F488 -O2 -msoft-float */
    s16 x;
    s16 y;
    u8 y_gt_0;
    u8 x_gt_0;
    u8 tab_val;
    s16 res;

    x = (ray.x - obj->x - obj->offset_bx) + ray.offset_bx;
    y = (ray.y - obj->y - obj->offset_by) + ray.offset_by;
    x_gt_0 = x > 0;
    y_gt_0 = y > 0;
    if (!x_gt_0) {
        x = -x;
    }
    if (!y_gt_0) {
        y = -y;
    }

    if (x > 200) {
        x = x >> 1;
        y = y >> 1;
    }

    if (y == 0) {
        if (x_gt_0) {
            res = 384;
        } else {
            res = 128;
        }
    } else {
        if (y > x) {
            tab_val = angletab[(s16) ((x * 64) / y)];
        } else {
            tab_val = 128 - angletab[(s16) ((y * 64) / x)];
        }
        if (x_gt_0) {
            if (y_gt_0) {
                res = 384 - tab_val;
            } else {
                res = 384 + tab_val;
            }
        } else {
            if (y_gt_0) {
                res = 128 + tab_val;
            } else {
                res = 128 - tab_val;
            }
        }
    }
    return res;
}

//7B56C
obj_t* allocateNOVA(void) {
    obj_t *result = NULL;
    s16 i = 0;
    obj_t *obj = level.objects + level_alw.obj_ids[i];
    while (i < level_alw.nb_objects) {
        if (obj->type == TYPE_143_NOVA2 && !obj->is_active) {
            set_main_and_sub_etat(obj, 5, 20);
            obj->flags.alive = 1;
            add_alwobj(obj);
            obj->anim_index = get_eta(obj)->anim_index;
            obj->anim_frame = 0;
            result = obj;
            break;
        }
        ++i;
        obj = level.objects + level_alw.obj_ids[i];
    }
    return result;
}

//7B628
void DO_NOVA(obj_t* obj) {
    s16 obj_x, obj_y, obj_w, obj_h;
    s16 nova_x, nova_y, nova_w, nova_h;
    obj_t* nova = allocateNOVA();
    if (nova) {
        nova->x = obj->x;
        nova->y = obj->y;
        GET_ANIM_POS(nova, &nova_x, &nova_y, &nova_w, &nova_h);
        GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);
        nova->is_active = 1;
        nova->x += (obj_x + (obj_w >> 1)) - (nova_x + (nova_w >> 1));
        nova->y += (obj_y + (obj_h >> 1)) - (nova_y + (nova_h >> 1));
        nova->display_prio = 1;
        nova->param = 0;
        calc_obj_pos(nova);
    }
}

//7B708
void DO_NOVA2(obj_t* obj) {
    /* 3B034 8015F834 -O2 -msoft-float */
    s16 i;
    obj_t* nova_obj;
    s16 nova_w, in_y, in_x, in_w, in_h, nova_y, nova_h, nova_x;
    s16 new_x; s16 new_y;

    for (i = 0; i < 5; i++) {
        nova_obj = allocateNOVA();
        if (nova_obj)
        {
            nova_obj->x = obj->x;
            nova_obj->y = obj->y;
            GET_ANIM_POS(nova_obj, &nova_x, &nova_y, &nova_w, &nova_h);
            GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);
            new_x = in_x - nova_x - (nova_w >> 1);
            new_y = in_y - nova_y - (nova_h >> 1);
            nova_obj->is_active = 1;
            switch (i)
            {
                case 0:
                    break;
                case 1:
                    new_x += in_w;
                    break;
                case 2:
                    new_x += in_w;
                    new_y += in_h;
                    break;
                case 3:
                    new_y += in_h;
                    break;
                case 4:
                    new_x += in_w >> 1;
                    new_y += in_h >> 1;
                    break;
            }
            nova_obj->y += new_y;
            nova_obj->display_prio = 0;
            nova_obj->param = i * 5;
            nova_obj->x += new_x;
            calc_obj_pos(nova_obj);
        }
    }
}

//7B838
s16 NOVA_STATUS_BAR(void) {
    obj_t* obj = allocateNOVA();
    if (obj) {
    obj->display_prio = 0;
    obj->timer = 10;
    obj->param = 255;
    obj->is_active = 1;
    obj->x = xmap + obj->offset_bx;
    obj->y = ymap + obj->offset_by;
    return obj->id;
    }
    return -1;
}

//7B920
void DO_REDEYE_FIRE(s16 x, s16 y, s16 a3) {
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_FLASH && !(cur_obj->is_active)) {
            cur_obj->flags.flip_x = 0;
            switch (a3 % 36) {
                case 1:
                    cur_obj->iframes_timer = 252;
                    cur_obj->follow_id = -44;
                    break;
                case 2:
                    cur_obj->iframes_timer = 240;
                    cur_obj->follow_id = -87;
                    break;
                case 3:
                    cur_obj->iframes_timer = 221;
                    cur_obj->follow_id = -128;
                    break;
                case 4:
                    cur_obj->iframes_timer = 196;
                    cur_obj->follow_id = -164;
                    break;
                case 5:
                    cur_obj->iframes_timer = 164;
                    cur_obj->follow_id = -196;
                    break;
                case 6:
                    cur_obj->iframes_timer = 128;
                    cur_obj->follow_id = -221;
                    break;
                case 7:
                    cur_obj->iframes_timer = 87;
                    cur_obj->follow_id = -240;
                    break;
                case 8:
                    cur_obj->iframes_timer = 44;
                    cur_obj->follow_id = -252;
                    break;
                case 9:
                    cur_obj->iframes_timer = -252;
                    cur_obj->follow_id = -44;
                    break;
                case 10:
                    cur_obj->iframes_timer = -240;
                    cur_obj->follow_id = -87;
                    break;
                case 11:
                    cur_obj->iframes_timer = -221;
                    cur_obj->follow_id = -128;
                    break;
                case 12:
                    cur_obj->iframes_timer = -196;
                    cur_obj->follow_id = -164;
                    break;
                case 13:
                    cur_obj->iframes_timer = -164;
                    cur_obj->follow_id = -196;
                    break;
                case 14:
                    cur_obj->iframes_timer = -128;
                    cur_obj->follow_id = -221;
                    break;
                case 15:
                    cur_obj->iframes_timer = -87;
                    cur_obj->follow_id = -240;
                    break;
                case 16:
                    cur_obj->iframes_timer = -44;
                    cur_obj->follow_id = -252;
                    break;
                case 17:
                    cur_obj->iframes_timer = 0;
                    cur_obj->follow_id = -256;
                    break;
                case 18:
                    cur_obj->iframes_timer = -256;
                    cur_obj->follow_id = 0;
                    break;
                case 19:
                    cur_obj->iframes_timer = -252;
                    cur_obj->follow_id = 44;
                    break;
                case 20:
                    cur_obj->iframes_timer = -240;
                    cur_obj->follow_id = 87;
                    break;
                case 21:
                    cur_obj->iframes_timer = -221;
                    cur_obj->follow_id = 128;
                    break;
                case 22:
                    cur_obj->iframes_timer = -196;
                    cur_obj->follow_id = 164;
                    break;
                case 23:
                    cur_obj->iframes_timer = -164;
                    cur_obj->follow_id = 196;
                    break;
                case 24:
                    cur_obj->iframes_timer = -128;
                    cur_obj->follow_id = 221;
                    break;
                case 25:
                    cur_obj->iframes_timer = -87;
                    cur_obj->follow_id = 240;
                    break;
                case 26:
                    cur_obj->iframes_timer = -44;
                    cur_obj->follow_id = 252;
                    break;
                case 27:
                    cur_obj->iframes_timer = 252;
                    cur_obj->follow_id = 44;
                    break;
                case 28:
                    cur_obj->iframes_timer = 240;
                    cur_obj->follow_id = 87;
                    break;
                case 29:
                    cur_obj->iframes_timer = 221;
                    cur_obj->follow_id = 128;
                    break;
                case 30:
                    cur_obj->iframes_timer = 196;
                    cur_obj->follow_id = 164;
                    break;
                case 31:
                    cur_obj->iframes_timer = 164;
                    cur_obj->follow_id = 196;
                    break;
                case 32:
                    cur_obj->iframes_timer = 128;
                    cur_obj->follow_id = 221;
                    break;
                case 33:
                    cur_obj->iframes_timer = 87;
                    cur_obj->follow_id = 240;
                    break;
                case 34:
                    cur_obj->iframes_timer = 44;
                    cur_obj->follow_id = 252;
                    break;
                case 35:
                    cur_obj->iframes_timer = 0;
                    cur_obj->follow_id = 256;
                    break;
                case 0:
                default:
                    cur_obj->iframes_timer = 256;
                    cur_obj->follow_id = 0;
                    break;

            }
            cur_obj->hit_points = a3 % 36;
            cur_obj->x = x;
            cur_obj->y = y;
            cur_obj->speed_x = 0;
            cur_obj->speed_y = 0;
            cur_obj->init_x = cur_obj->iframes_timer;
            cur_obj->init_y = cur_obj->follow_id;
            calc_obj_pos(cur_obj);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            PlaySnd(205, i);
            break;
        }
        cur_obj++;
        i++;
    }
}

//7BC60
void calc_esquive_poing(obj_t* mit_obj, s16* out_diff_x, s16* out_diff_y, s16* out_unk) {
    /* 3C6E4 80160EE4 -O2 -msoft-float */
    s16 unk_x; s16 unk_y;

    if (!poing_obj->is_active) {
        unk_x = ray_zdc_x + (ray_zdc_w >> 1);
        unk_y = ray_zdc_h + (ray_zdc_y >> 1);
    } else {
        unk_x = (s16)poing_obj->x + poing_obj->offset_bx;
        /* Repeated sum reproduces DOS scheduling at 1:6bcb0..6bcb7. */
        unk_y = poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_by) >> 1);
    }

    *out_diff_x = unk_x - (s16)mit_obj->x - mit_obj->offset_bx;
    *out_diff_y = Abs(mit_obj->y + ((mit_obj->offset_hy + mit_obj->offset_by) >> 1) - unk_y);
    *out_unk = mit_obj->detect_zone * 4;
}

//7BD04
s16 allocate_gerbe(void) {
    /* 3CB5C 8016135C -O2 -msoft-float */
    s16 i;
    s16 res = -1;

    for (i = 0; i < (s16) LEN(pix_gerbe); i++) {
        if (!pix_gerbe[i].is_active) {
            pix_gerbe[i].is_active = true;
            res = i;
            break;
        }
    }

    return res;
}

//7BD40
void start_pix_gerbe(s16 x, s16 y) {
    /* 3CBE0 801613E0 -O2 -msoft-float */
    s16 *cur_data;
    s16 spd_x;
    s16 i;
    s16 spd_y;
    s16 grb;
    grb = allocate_gerbe();

    if (grb != -1) {
        cur_data = (s16 *) &pix_gerbe[grb]; /* struct access instead??? */
        for (i = 0; i < (s16) LEN(pix_gerbe[grb].items) - grb * (s16) LEN(pix_gerbe); i++) {
            *cur_data++ = x << 6;
            *cur_data++ = y << 6;
            spd_y = myRand((i << 2) + 128) - (256 + (i << 1));
            spd_x = myRand((i << 2) + 64) - (32 + (i << 1));
            *cur_data++ = spd_x;
            *cur_data++ = spd_y;
            ((u8 *) cur_data)[0] = myRand(8) + 8; /* y_accel */
            ((u8 *) cur_data)[1] = 128; /* unk_1 */
            cur_data++;
        }
    }
}

//7BE44
void do_pix_gerbes(void) {
    /* 3CD10 80161510 -O2 -msoft-float */
    s16 i;
    s16 new_active;
    pix_gerbe_item_t* cur_item;
    s16 j;
    s16 v_speed;
    s16 h_speed;
    h_speed = h_scroll_speed * 64;
    v_speed = v_scroll_speed * 64;

    i = 0;
    while (i < (s16) LEN(pix_gerbe)) {
        if (pix_gerbe[i].is_active == true) {
            new_active = false;
            cur_item = &pix_gerbe[i].items[0];
            j = 0;
            while (j < (s16) LEN(pix_gerbe[i].items)) {
                if (cur_item->unk_1 >= 128) {
                    cur_item->x_pos += cur_item->speed_x - h_speed;
                    cur_item->y_pos += cur_item->speed_y - v_speed;
                    // NOTE: this check was changed in the PC version
                    if (cur_item->x_pos > (Bloc_lim_W2_Aff << 6) || cur_item->x_pos < (Bloc_lim_W1_Aff << 6) || cur_item->y_pos >= (Bloc_lim_H2_Aff << 6)) {
                        cur_item->unk_1 = 0;
                    } else {
                        new_active = true;
                    }

                    cur_item->speed_y += cur_item->y_accel;
                }
                cur_item++;
                j++;
            }
            pix_gerbe[i].is_active = new_active;
        }
        i++;
    }
}

//7BF40
void START_2_PARTS_CYMBAL_ACTION(obj_t* obj) {
    /* 3CE78 80161678 -O2 */
    obj_t* linkedCymbal;
    s16 linked_id;

    skipToLabel(obj, 4, true);
    obj->link = 1;
    linked_id = link_init[obj->id];
    if (linked_id != obj->id) {
        linkedCymbal = &level.objects[linked_id];
        if (linkedCymbal->type == TYPE_168_CYMBAL1) {
            skipToLabel(linkedCymbal, 4, true);
            linkedCymbal->link = 1;
        }
    }
}

//7BFB0
void ChangeLevel(void) {
    fix_numlevel(&ray);
    new_level = 1;
}

//7BFCC
void init_moustique(void) {
    if (!fin_du_jeu) {
        NewMs = rayman_obj_id != -1;
        rayOnMsWasThere = 0;
    }
}

//7BFF4
void DO_CORDE_CASSE(obj_t* obj) {
    /* 3EB50 80163350 -O2 -msoft-float */
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;

    set_sub_etat(obj, 21);
    if (level.objects[pierreAcorde_obj_id].hit_points == 10) {
        level.objects[pierreAcorde_obj_id].hit_points = 5;
    } else {
        level.objects[pierreAcorde_obj_id].hit_points = 1;
        set_main_and_sub_etat(level.objects + pierreAcorde_obj_id, 0, 23);
    }

    i = 0;
    cur_obj = &level.objects[i];
    nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (cur_obj->type == TYPE_222_CORDEBAS && !(cur_obj->is_active)) {
            cur_obj->x = obj->x;
            cur_obj->y = obj->y;
            cur_obj->speed_y = 1;
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->gravity_value_2 = 4;
            break;
        }
        cur_obj++;
        i++;
    }
}

//7C0A4
void DO_FUMEE_CORDE(s16 x, s16 y) {
    /* 3EC84 80163484 -O2 -msoft-float */
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs)
    {
        if (cur_obj->type == TYPE_221_CFUMEE && !(cur_obj->is_active))
        {
            cur_obj->x = x - 142;
            cur_obj->y = y - 128;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->flags.alive = 1;
            break;
        }
        cur_obj++;
        i++;
    }
}

//7C10C
s32 GetY(s16 x) {
    /* 3ED10 80163510 -O2 -msoft-float */
    s16 unk_1;
    s32 unk_2;
    s16 unk_obj_id;
    s16 unk_3;
    obj_t* unk_obj;

    s32 result;
    s32 distance;
    if (eau_obj_id == -1) {
        result = 0;
    } else {

    unk_1 = (x + 50) / 101;
    unk_obj_id = eau_obj_id + unk_1 % 5;
    unk_2 = unk_1 * 101;
    distance = x;
    distance -= unk_2;
    if (distance < 0) distance = -distance;
    unk_3 = 51 - distance;
    unk_obj = &level.objects[unk_obj_id];

    result = unk_obj->y - EauDy[(s16)((unk_obj->anim_frame + DecEau[unk_obj->anim_index]) % 14)] * unk_3 / 50;
    }
    return result;
}

//7C1C4
void allocateSupHelico(obj_t* mus_obj) {
    {
        s16 i = 0;
        obj_t *cur_obj = level.objects;
        s16 nb_objs = level.nb_objects;
        while (i < nb_objs) {
        if (cur_obj->type == TYPE_133_SUPERHELICO && !cur_obj->is_active) {
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->x = mus_obj->x;
            cur_obj->y = mus_obj->y;
            set_main_and_sub_etat(cur_obj, 2, 18);
            cur_obj->screen_x = cur_obj->x - xmap;
            cur_obj->screen_y = cur_obj->y - ymap;
            cur_obj->speed_y = -2;
            cur_obj->speed_x = 1;
            calc_obj_pos(cur_obj);
            break;
        }
        ++cur_obj;
        ++i;
    }
    }
}

//7C274
void allocatePaillette(obj_t* obj) {
    obj_t *pailette_obj = NULL;
    s16 i = 0;
    obj_t *cur_obj = level.objects;
    s16 nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (cur_obj->type == TYPE_75_PAILLETTE && !cur_obj->is_active) {
            pailette_obj = cur_obj;
            break;
        }
        ++i;
        ++cur_obj;
    }
    if (!pailette_obj) {
        pailette_obj = findfirstObject(TYPE_75_PAILLETTE);
    } else {
        add_alwobj(pailette_obj);
    }
    pailette_obj->is_active = 1;
    pailette_obj->flags.alive = 1;
    pailette_obj->active_timer = 0;
    pailette_obj->flags.flag_0x40 = 0;
    pailette_obj->x = obj->x - 40;
    pailette_obj->y = obj->y - 20;
    pailette_obj->anim_index = 0;
    pailette_obj->anim_frame = 1;
    pailette_obj->main_etat = 0;
    pailette_obj->init_main_etat = 0;
    pailette_obj->init_sub_etat = 0;
    pailette_obj->sub_etat = 0;
    calc_obj_pos(pailette_obj);
}

//7C328
void test_fin_cling(void) {
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
}
