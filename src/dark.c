/*
 * Mr Dark boss (GOG v1.21 object 1:22a70-1:240f0). File name invented (Rayverse dark.c). Names: Rayverse/Android.
 * Worker candidate: 18/22 STRICT; four functions remain NEAR_MATCH; contiguous _TEXT.
 * Detailed evidence and negative results: NOTES.md in this directory.
 */
#include "obj.h"
#include "game.h"
#include "inline_helpers.h"
extern obj_type_flags_t flags[262];
#include <stdlib.h>
#define ABS_B(x) ((s32)(x) >= 0 ? (s32)(x) : -(s32)(x))

#define TYPE_DARK_SORT 215
#define TYPE_VITRAIL 247
typedef struct vitraux_info_t {
    s16 x_pos;
    s16 y_pos;
    u8 obj_id;
    u8 index;
    u8 field_6;
    u8 mode;
    u16 value;
} vitraux_info_t;

extern obj_t ray;
extern obj_t *poing_obj;
extern level_t level;
extern rayevts_t RayEvts;
extern finbosslevel_t finBosslevel;
extern u8 horloge[25];

extern s16 xmapmax;
extern s16 ymap;
extern s16 poing_obj_id;
extern s16 black_ray_obj_id;
extern s16 corde_dark_obj_id;
extern s16 corde_x;
extern s16 corde_y_bas;
extern s16 corde_y_haut;
extern s16 moskitomama_droite_obj_id;
extern s16 moskitomama_gauche_obj_id;
extern s16 moskitosaxo_obj_id;
extern s16 stosko_obj_id;
extern s16 dark_rayon_dx;
extern s16 dark_rayon_dy;
extern s16 final_pass_n;
extern s16 final_pass_a;
extern s16 final_pass_f;
extern s16 final_pass_x; /* DOS signed -60 coordinate; see NOTES.md. */
extern u8 dark_phase;
extern s16 scroll_start_x;
extern s16 scroll_start_y;
extern s16 scroll_end_x;
extern s16 scroll_end_y;
extern u8 scrollLocked;
extern u8 fin_boss;
extern u8 TextDark2_Affiche;
extern u8 ray_stack_is_full;
extern u8 black_pos_in_stack;
extern u8 ray_pos_in_stack;
extern u8 dark_attaque;
extern u8 type_dark_attaque;
extern u8 byte_DF74C;
extern s16 oscille[29];
extern u8 dark_sequence[3];
extern u8 vitrail_clignotement[4][5];
extern vitraux_info_t VitrauxInfos[5];

s16 firstFloorBelow(obj_t *obj);
u8 EOA(obj_t *obj);
void set_sub_etat(obj_t *obj, u8 sub_etat);
void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
s16 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void add_alwobj(obj_t *obj);
void DO_NOVA(obj_t *obj);
void DO_NOVA2(obj_t *obj);
void RAY_REVERSE_COMMANDS(void);
void RAY_DEMIRAY(void);
void allocateSTOSKO(void);
void allocateMOSKITOMAMA(void);
void allocateMOSKITOSAXO(void);
void AllocateDarkPhase2(obj_t *obj);
void allume_vitraux(u8 *param_1);
void PlaceDarkPhase1et2(obj_t *obj);
void dark_attaque_suivante(void);
void init_corde(obj_t *obj);
void corde_en_bas(void);
void corde_en_haut(u8 show_fist);
void goto_phase1(obj_t *obj);
void goto_phase2(obj_t *obj);
void goto_phase3(obj_t *obj);
void goto_phase5(obj_t *obj);
void DARK_phase1(obj_t *obj);
void DARK_phase3(obj_t *obj);
void allocate_DARK_SORT(s16 x, s16 y, s16 sub_etat, s16 iframes);

void PlaceDarkPhase1et2(obj_t *obj)
{
    obj->y = obj->init_y = firstFloorBelow(obj) - obj->offset_by;
    obj->x = obj->init_x = xmapmax + 144;
}

void dark_attaque_suivante(void)
{
    dark_attaque++;
    if (dark_attaque == 3)
        type_dark_attaque = 3;
    else
        type_dark_attaque = dark_sequence[dark_attaque];
}

void init_vitraux(void)
{
    u8 unk_1 = 0;
    s16 i = 0;
    obj_t *cur_obj = level.objects;
    s16 nb_objs = level.nb_objects;

    if (nb_objs > 0) {
        do {
            if (unk_1 >= 5)
                return;
            if (cur_obj->type == TYPE_VITRAIL) {
                VitrauxInfos[unk_1].obj_id = (u8)i;
                VitrauxInfos[unk_1].x_pos = cur_obj->x + cur_obj->offset_bx;
                VitrauxInfos[unk_1].y_pos = cur_obj->y + cur_obj->offset_by;
                VitrauxInfos[unk_1].index = unk_1;
                VitrauxInfos[unk_1].field_6 = 0;
                VitrauxInfos[unk_1].mode = 0;
                VitrauxInfos[unk_1].value = 0;
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                cur_obj->param = unk_1;
                unk_1++;
            }
            i++;
            cur_obj++;
        } while (i < nb_objs);
    }
}

u8 poing_face_obj(obj_t *obj)
{
    s32 diff_x = (obj->x + obj->offset_bx) - (poing_obj->x + poing_obj->offset_bx);
    u8 ahead = diff_x > 0;
    u8 facing = 0;
    if (ahead == poing_obj->flags.flip_x)
        facing = 1;
    return facing;
}

void DARK_phase1(obj_t *mr_drk_obj)
{
    s16 diff_x;
    u32 dist;
    obj_t *crd_drk_obj;
    s16 diff_y;
    u8 unk_1;
    if (corde_dark_obj_id != -1 && mr_drk_obj->main_etat == 0) {
        crd_drk_obj = &level.objects[corde_dark_obj_id];
        if (poing_obj_id != -1) {
            set_main_and_sub_etat(poing_obj, 5, 64);
            poing_obj->x = crd_drk_obj->x + crd_drk_obj->offset_bx - poing_obj->offset_bx;
            poing_obj->y = crd_drk_obj->y + crd_drk_obj->offset_by - poing_obj->offset_by - 5;
            if (!poing_obj->is_active)
                add_alwobj(poing_obj);
            poing_obj->flags.alive = 1;
            poing_obj->is_active = 1;
        }

        switch (mr_drk_obj->sub_etat) {
        case 40:
            if (EOA(mr_drk_obj))
                goto_phase2(mr_drk_obj);
            break;
        case 27:
            crd_drk_obj->y -= 8;
            if (crd_drk_obj->y < corde_y_haut)
                crd_drk_obj->y = corde_y_haut;
            if (poing_obj_id != -1)
                poing_obj->y = crd_drk_obj->y + crd_drk_obj->offset_by - poing_obj->offset_by - 5;
            if (EOA(mr_drk_obj)) {
                if (--crd_drk_obj->param == 0)
                    set_main_and_sub_etat(mr_drk_obj, 0, 40);
                else {
                    crd_drk_obj->iframes_timer = 0;
                    set_main_and_sub_etat(mr_drk_obj, 0, 28);
                }
            }
            break;
        case 29:
            crd_drk_obj->y += 6;
            if (crd_drk_obj->y > corde_y_bas)
                crd_drk_obj->y = corde_y_bas;
            if (poing_obj_id != -1)
                poing_obj->y = crd_drk_obj->y + crd_drk_obj->offset_by - poing_obj->offset_by - 5;
            if (EOA(mr_drk_obj)) {
                crd_drk_obj->iframes_timer = 0;
                set_main_and_sub_etat(mr_drk_obj, 0, 26);
            } else if (ray.main_etat == 2) {
                set_main_and_sub_etat(mr_drk_obj, 0, 27);
            }
            break;
        case 26:
        case 28:
            diff_x = ((s16)ray.x + ray.offset_bx) - (corde_x + crd_drk_obj->offset_bx);
            diff_y = ((s16)ray.y + ray.offset_by) - (corde_y_bas + crd_drk_obj->offset_by);
            dist = diff_x * diff_x + diff_y * diff_y;
            unk_1 = ABS_B(diff_x) < 40 || dist < 3000;
            switch (mr_drk_obj->sub_etat) {
            case 26:
                if (unk_1 || (ray.main_etat == 2 && ray.sub_etat == 0) ||
                    ((ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40) &&
                    ABS_B(diff_y) < 30 && dist < 3000))
                    set_main_and_sub_etat(mr_drk_obj, 0, 27);
                else if (crd_drk_obj->iframes_timer < 30 - 1) {
                    crd_drk_obj->y = corde_y_bas - oscille[crd_drk_obj->iframes_timer];
                    if (poing_obj_id != -1)
                        poing_obj->y = crd_drk_obj->y + crd_drk_obj->offset_by - poing_obj->offset_by - 5;
                    crd_drk_obj->iframes_timer++;
                }
                break;
            case 28:
                if (!(unk_1 || ray.main_etat == 2))
                    set_main_and_sub_etat(mr_drk_obj, 0, 29);
                else if (crd_drk_obj->iframes_timer < 30 - 1) {
                    crd_drk_obj->y = corde_y_haut + oscille[crd_drk_obj->iframes_timer];
                    if (poing_obj_id != -1)
                        poing_obj->y = crd_drk_obj->y + crd_drk_obj->offset_by - poing_obj->offset_by - 5;
                    crd_drk_obj->iframes_timer++;
                }
                break;
            }
            break;
        }
    }
}

void DARK_phase3(obj_t *mr_drk_obj)
{
    obj_t *boss_obj;

    switch (type_dark_attaque) {
    case 0:
        if (stosko_obj_id != -1) {
            boss_obj = &level.objects[stosko_obj_id];
            if (boss_obj->param == 0) {
                boss_obj->param = 2;
                allume_vitraux(vitrail_clignotement[0]);
                flags[boss_obj->type].flags0 |= 0x80;
                scrollLocked = 1;
                allocateSTOSKO();
            } else if (boss_obj->param == 1 && !boss_obj->is_active) {
                flags[boss_obj->type].flags0 &= 0x7f;
                scrollLocked = 0;
                boss_obj->flags.alive = 0;
                boss_obj->is_active = 0;
                dark_attaque_suivante();
            }
        }
        break;
    case 1:
        if (moskitomama_droite_obj_id != -1) {
            boss_obj = &level.objects[moskitomama_droite_obj_id];
            if (boss_obj->param == 0) {
                boss_obj->param = 2;
                if (moskitomama_gauche_obj_id != -1)
                    level.objects[moskitomama_gauche_obj_id].param = 2;
                allume_vitraux(vitrail_clignotement[1]);
                flags[boss_obj->type].flags0 |= 0x80;
                scrollLocked = 1;
                allocateMOSKITOMAMA();
            } else if (boss_obj->param == 1 && !boss_obj->is_active) {
                flags[boss_obj->type].flags0 &= 0x7f;
                scrollLocked = 0;
                boss_obj->flags.alive = 0;
                boss_obj->is_active = 0;
                dark_attaque_suivante();
            }
        }
        break;
    case 2:
        if (moskitosaxo_obj_id != -1) {
            boss_obj = &level.objects[moskitosaxo_obj_id];
            if (boss_obj->param == 0) {
                boss_obj->param = 2;
                allume_vitraux(vitrail_clignotement[2]);
                flags[boss_obj->type].flags0 |= 0x80;
                scrollLocked = 1;
                allocateMOSKITOSAXO();
            } else if (boss_obj->param == 1 && !boss_obj->is_active) {
                flags[boss_obj->type].flags0 &= 0x7f;
                scrollLocked = 0;
                boss_obj->flags.alive = 0;
                boss_obj->is_active = 0;
                dark_attaque_suivante();
            }
        }
        break;
    case 3:
        allume_vitraux(vitrail_clignotement[3]);
        goto_phase5(mr_drk_obj);
        break;
    }
}

void DO_DARK_COMMAND(obj_t *mr_drk_obj)
{
    s16 dark_x;
    s16 dark_y;
    s16 dark_w;
    s16 dark_h;
    s16 sort_x;
    s16 sort_y;

    if (mr_drk_obj->main_etat == 0) {
        if (mr_drk_obj->sub_etat == 39) {
            switch (dark_phase) {
            case 0:
            case 1:
                goto_phase1(mr_drk_obj);
                break;
            case 2:
                goto_phase2(mr_drk_obj);
                break;
            case 3:
                goto_phase3(mr_drk_obj);
                break;
            case 5:
                goto_phase5(mr_drk_obj);
                break;
            }
        }
    }
    if (mr_drk_obj->main_etat == 0 && mr_drk_obj->sub_etat == 6) {
        if (mr_drk_obj->anim_frame == 4)
            mr_drk_obj->speed_y = -4;
        else if (dark_phase == 4) {
            if (EOA(mr_drk_obj))
                goto_phase5(mr_drk_obj);
        } else if (EOA(mr_drk_obj)) {
            mr_drk_obj->is_active = 0;
            mr_drk_obj->flags.alive = 0;
        }
    } else if (mr_drk_obj->main_etat == 0 && (mr_drk_obj->sub_etat == 8 || mr_drk_obj->sub_etat == 10 ||
        mr_drk_obj->sub_etat == 12 || mr_drk_obj->sub_etat == 14 ||
        mr_drk_obj->sub_etat == 22 || mr_drk_obj->sub_etat == 23 ||
        mr_drk_obj->sub_etat == 24) && mr_drk_obj->anim_frame == 22) {
        GET_SPRITE_POS(mr_drk_obj, 3, &dark_x, &dark_y, &dark_w, &dark_h);
        sort_x = dark_x + (dark_w >> 1) - 96;
        sort_y = dark_y + (dark_h >> 1) - 128;
        dark_rayon_dx = -5;
        dark_rayon_dy = 0;
        switch (mr_drk_obj->sub_etat) {
        case 8:
            allocate_DARK_SORT(sort_x, sort_y, 0, 1);
            break;
        case 10:
            allocate_DARK_SORT(sort_x, sort_y, 1, 1);
            break;
        case 12:
            allocate_DARK_SORT(sort_x, sort_y, 2, 1);
            break;
        case 14:
            allocate_DARK_SORT(sort_x, sort_y, 3, 1);
            break;
        case 22:
            byte_DF74C = 2;
            allume_vitraux(vitrail_clignotement[0]);
            allocate_DARK_SORT(sort_x, sort_y, 18, 3);
            allocate_DARK_SORT(sort_x, sort_y, 18, 5);
            break;
        case 23:
            byte_DF74C = 2;
            allume_vitraux(vitrail_clignotement[1]);
            allocate_DARK_SORT(sort_x, sort_y, 19, 1);
            allocate_DARK_SORT(sort_x, sort_y, 19, 4);
            break;
        case 24:
            byte_DF74C = 2;
            allume_vitraux(vitrail_clignotement[2]);
            allocate_DARK_SORT(sort_x, sort_y, 20, 1);
            allocate_DARK_SORT(sort_x, sort_y, 20, 2);
            break;
        }
    } else if (mr_drk_obj->main_etat == 0 && mr_drk_obj->sub_etat == 15) {
        if (mr_drk_obj->anim_frame == 0) {
            if (horloge[mr_drk_obj->eta[mr_drk_obj->main_etat][mr_drk_obj->sub_etat].anim_speed & 0x0f] == 0) {
                DO_NOVA2(mr_drk_obj);
                mr_drk_obj->flags.flip_x = 1;
                mr_drk_obj->flags.alive = 1;
                mr_drk_obj->is_active = 1;
            }
        } else if (mr_drk_obj->anim_frame == 22) {
            GET_SPRITE_POS(mr_drk_obj, 3, &dark_x, &dark_y, &dark_w, &dark_h);
            sort_x = dark_x + (dark_w >> 1) - 96;
            sort_y = dark_y + (dark_h >> 1) - 128;
            dark_rayon_dx = -2;
            dark_rayon_dy = 0;
            allocate_DARK_SORT(sort_x, sort_y, 4, 1);
        }
    } else if (mr_drk_obj->main_etat == 0 && mr_drk_obj->sub_etat == 21) {
        DARK_phase3(mr_drk_obj);
    } else if (mr_drk_obj->main_etat == 0 && (mr_drk_obj->sub_etat == 26 || mr_drk_obj->sub_etat == 27 ||
        mr_drk_obj->sub_etat == 28 || mr_drk_obj->sub_etat == 29 ||
        mr_drk_obj->sub_etat == 40)) {
        DARK_phase1(mr_drk_obj);
    }
}

void DO_DARK_POING_COLLISION(obj_t *obj, s16 sprite)
{
    if (obj->main_etat == 0 && (obj->sub_etat == 16 || obj->sub_etat == 17) && obj->display_prio != 0) {
        if (obj->hit_points != 0)
            obj->hit_points--;
        if (obj->hit_points == 0)
            set_main_and_sub_etat(obj, 0, 6);
    }
}

void DO_DARK_REACT(obj_t *obj)
{
    if (obj->main_etat == 0) {
        if (obj->sub_etat == 7)
            set_sub_etat(obj, 8);
        if (obj->sub_etat == 9)
            set_sub_etat(obj, 10);
        if (obj->sub_etat == 11)
            set_sub_etat(obj, 12);
        if (obj->sub_etat == 13)
            set_sub_etat(obj, 14);
    }
}

void DO_DARK_SORT_COMMAND(obj_t *ds_obj)
{
    obj_t *br_obj;
    s16 diff_x;
    s16 diff_y;
    s16 dist;

    if (ds_obj->hit_points != 0) {
        if (ds_obj->sub_etat == 5) {
            if (fin_boss == 1)
                ds_obj->hit_points = 0;
        } else if (ds_obj->sub_etat == 4) {
            br_obj = &level.objects[black_ray_obj_id];
            if (ds_obj->iframes_timer == 1) {
                ds_obj->iframes_timer = 0;
                br_obj->flags.flip_x = 0;
                br_obj->hit_points = 0xfe;
                set_main_and_sub_etat(br_obj, 0, 0);
                br_obj->x = 0;
                br_obj->y = 0;
                br_obj->init_x = ray.x;
                br_obj->init_y = ray.y;

                br_obj->flags.alive = 1;
                br_obj->is_active = 1;
                add_alwobj(br_obj);
                br_obj->display_prio = 0;
                ray_stack_is_full = 0;
                ray_pos_in_stack = 0;
                black_pos_in_stack = 0;
            }
            diff_x = (br_obj->init_x + br_obj->offset_bx) - ((s16)ds_obj->x + ds_obj->offset_bx);
            diff_y = (br_obj->init_y + br_obj->offset_by) - ((s16)ds_obj->y + ds_obj->offset_by);
            if (ray_stack_is_full == 1 && black_pos_in_stack == 25)
                ds_obj->hit_points = 0;
        } else {
            ds_obj->iframes_timer = 0;
            diff_x = ((s16)ray.x + ray.offset_bx) - ((s16)ds_obj->x + ds_obj->offset_bx);
            if (ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
                diff_y = ((s16)ray.y + ray.offset_by) - ((s16)ds_obj->y + ds_obj->offset_by);
            else
                diff_y = (ray.y + ray.offset_hy + ((ray.offset_by - ray.offset_hy) >> 1)) -
                (ds_obj->y + ds_obj->offset_by);
        }

        dist = ABS_B(diff_x) + ABS_B(diff_y);
        if (dist > 0) {
            diff_x = ashl16(diff_x, 4) / dist;
            diff_y = ashl16(diff_y, 4) / dist;
        }
        if (horloge[2] == 0) {
            dark_rayon_dx += sgn(diff_x - dark_rayon_dx);
            dark_rayon_dy += sgn(diff_y - dark_rayon_dy);
            allocate_DARK_SORT((s16)ds_obj->x + dark_rayon_dx, (s16)ds_obj->y + dark_rayon_dy,
                ds_obj->sub_etat, ds_obj->iframes_timer);
            ds_obj->hit_points = 0;
        }
    }
}

void DO_DARK_SORT_COLLISION(obj_t *obj)
{
    if (obj->hit_points != 0 && !(obj->main_etat == 0 && obj->sub_etat == 4)) {
        obj->hit_points = 0;
        switch (obj->sub_etat) {
        case 0:
            RayEvts.force_run = 1;
            break;
        case 1:
            DO_NOVA(&ray);
            RayEvts.reverse = 0;
            RAY_REVERSE_COMMANDS();
            break;
        case 2:
            DO_NOVA(&ray);
            RAY_DEMIRAY();
            break;
        case 3:
            DO_NOVA(&ray);
            RayEvts.poing = 0;
            break;
        }
    }
}

void allocate_DARK_SORT(s16 x, s16 y, s16 sub_etat, s16 iframes)
{
    s16 oldest_frame;
    obj_t *cur_obj = &level.objects[0];
    s16 scan_or_found = 0;
    s16 selected_or_index = 0;
    s16 nb_objs = level.nb_objects;

    do {
        if (cur_obj->type == TYPE_DARK_SORT && !cur_obj->is_active)
            scan_or_found = 1;
        cur_obj++;
        selected_or_index++;
    } while (scan_or_found == 0 && selected_or_index < nb_objs);
    cur_obj--;
    if (scan_or_found != 0) {
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
        add_alwobj(cur_obj);
        cur_obj->x = x;
        cur_obj->y = y;
        cur_obj->hit_points = 1;
        cur_obj->iframes_timer = iframes;
        cur_obj->param = 0;
        set_main_and_sub_etat(cur_obj, 0, sub_etat);
        cur_obj->anim_frame = 0;
    } else {
        oldest_frame = 0;
        cur_obj = &level.objects[0];
        scan_or_found = selected_or_index = 0;
        do {
            if (cur_obj->type == TYPE_DARK_SORT && cur_obj->anim_frame > oldest_frame) {
                selected_or_index = scan_or_found;
                oldest_frame = cur_obj->anim_frame;
            }
            cur_obj++;
            scan_or_found++;
        } while (scan_or_found < nb_objs);
        cur_obj = &level.objects[selected_or_index];
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
        add_alwobj(cur_obj);
        cur_obj->x = x;
        cur_obj->y = y;
        cur_obj->hit_points = 1;
        cur_obj->iframes_timer = iframes;
        cur_obj->param = 0;
        set_main_and_sub_etat(cur_obj, 0, sub_etat);
        cur_obj->anim_frame = 0;
    }
}

void DoAnnuleDarkSortRaymanCollision(obj_t *obj)
{
    if (RayEvts.force_run != 0 || RayEvts.reverse != 0 || RayEvts.tiny) {
        DO_NOVA(&ray);
        if (RayEvts.force_run != 0)
            RayEvts.force_run = 3;
        if (RayEvts.reverse != 0)
            RAY_REVERSE_COMMANDS();
        if (RayEvts.tiny)
            RAY_DEMIRAY();
        obj->flags.alive = 0;
        obj->is_active = 0;
    }
}

void corde_en_bas(void)
{
    obj_t *obj;
    if (corde_dark_obj_id != -1) {
        obj = &level.objects[corde_dark_obj_id];
        obj->init_x = obj->x = corde_x;
        obj->init_y = obj->y = corde_y_bas;
    }
}

void corde_en_haut(u8 show_fist)
{
    obj_t *corde_dark_obj;

    if (corde_dark_obj_id != -1) {
        corde_dark_obj = &level.objects[corde_dark_obj_id];
        corde_dark_obj->x = corde_x;
        corde_dark_obj->y = corde_y_haut;
        if (show_fist) {
            set_main_and_sub_etat(poing_obj, 5, 64);
            poing_obj->x = corde_dark_obj->x + corde_dark_obj->offset_bx - poing_obj->offset_bx;
            poing_obj->y = corde_dark_obj->y + corde_dark_obj->offset_by - poing_obj->offset_by - 5;
            if (!poing_obj->is_active)
                add_alwobj(poing_obj);
            poing_obj->flags.alive = 1;
            poing_obj->is_active = 1;
        }
    }
}

void init_corde(obj_t *obj)
{
    obj_t *corde;
    corde_y_bas = obj->y;
    corde_y_haut = ymap - 184;
    corde_x = 0;
    if (corde_dark_obj_id != -1) {
        corde = &level.objects[corde_dark_obj_id];
        corde_y_haut = (s16)obj->y + obj->offset_by - 184 - corde->offset_by + 36;
    }
}

void goto_phase1(obj_t *mr_drk_obj)
{
    obj_t *corde_dark_obj;

    dark_phase = 1;
    scroll_end_x = 0;
    scroll_start_x = 0;
    scroll_end_y = ymap;
    scroll_start_y = ymap;
    flags[mr_drk_obj->type].flags0 &= 0x7f;
    mr_drk_obj->param = 0;
    scrollLocked = 0;
    RayEvts.poing = 0;
    PlaceDarkPhase1et2(mr_drk_obj);
    init_corde(mr_drk_obj);
    if (corde_dark_obj_id != -1) {
        corde_dark_obj = &level.objects[corde_dark_obj_id];
        corde_dark_obj->param = 3;
        corde_dark_obj->iframes_timer = 29;
    }
    corde_en_bas();
    set_main_and_sub_etat(mr_drk_obj, 0, 26);
}

void goto_phase2(obj_t *mr_drk_obj)
{
    dark_phase = 2;
    scroll_end_x = 0;
    scroll_start_x = 0;
    scroll_end_y = ymap;
    scroll_start_y = ymap;
    flags[mr_drk_obj->type].flags0 &= 0x7f;
    scrollLocked = 0;
    PlaceDarkPhase1et2(mr_drk_obj);
    init_corde(mr_drk_obj);
    corde_en_haut(1);
    mr_drk_obj->flags.alive = 1;
    mr_drk_obj->is_active = 1;
    mr_drk_obj->param = 0;
    AllocateDarkPhase2(mr_drk_obj);
}

void goto_phase3(obj_t *mr_drk_obj)
{
    dark_phase = 3;
    scroll_end_x = 0;
    scroll_start_x = 0;
    scroll_end_y = ymap;
    scroll_start_y = ymap;
    flags[mr_drk_obj->type].flags0 &= 0x7f;
    scrollLocked = 0;
    PlaceDarkPhase1et2(mr_drk_obj);
    init_corde(mr_drk_obj);
    corde_en_haut(0);
    mr_drk_obj->init_x = mr_drk_obj->x = 0;
    mr_drk_obj->is_active = 1;
    mr_drk_obj->speed_x = 0;
    mr_drk_obj->speed_y = 0;
    mr_drk_obj->init_y = mr_drk_obj->y = ymap - mr_drk_obj->offset_by - 20;
    mr_drk_obj->flags.alive = 1;
    mr_drk_obj->param = 0;
    set_main_and_sub_etat(mr_drk_obj, 0, 21);
    dark_attaque = 0;
    type_dark_attaque = dark_sequence[0];
}

void goto_phase5(obj_t *mr_drk_obj)
{
    dark_phase = 5;
    scroll_end_x = 0;
    scroll_start_x = 0;
    scroll_end_y = ymap;
    scroll_start_y = ymap;
    flags[mr_drk_obj->type].flags0 &= 0x7f;
    scrollLocked = 0;
    PlaceDarkPhase1et2(mr_drk_obj);
    init_corde(mr_drk_obj);
    corde_en_haut(0);
    if (RayEvts.tiny) {
        DO_NOVA(&ray);
        RAY_DEMIRAY();
    }
    {
        s32 target_y = ymap;
        mr_drk_obj->init_x = mr_drk_obj->x = 0;
        mr_drk_obj->param = 0;
        mr_drk_obj->flags.alive = 0;
        final_pass_x = -60;
        final_pass_a = 0;
        final_pass_f = 0;
        mr_drk_obj->init_y = mr_drk_obj->y = target_y - mr_drk_obj->offset_by - 20;
        mr_drk_obj->is_active = 0;
        final_pass_n = 0;
    }
    fin_boss = 1;
    finBosslevel.mr_dark = 1;
    TextDark2_Affiche = 1;
}

void DO_VITRAIL_COMMAND(obj_t *obj)
{
    u8 index = (u8)obj->param;

    if (index < 5) {
        if (VitrauxInfos[index].mode != 0) {
            obj->display_prio = 4;
            VitrauxInfos[index].value--;
            if (VitrauxInfos[index].value == 0) {
                VitrauxInfos[index].value = 100;
                VitrauxInfos[index].mode--;
                if (VitrauxInfos[index].mode == 0)
                    obj->display_prio = VitrauxInfos[index].mode;
            }
        } else {
            obj->display_prio = 0;
        }
    } else {
        obj->display_prio = 0;
    }
}

void allume_vitraux(u8 *param_1)
{
    u8 i;

    i = 0;
    while (i < 5) {
        if (VitrauxInfos[i].mode == 0) {
            VitrauxInfos[i].mode = param_1[i];
            VitrauxInfos[i].value = 100;
            level.objects[(u8)VitrauxInfos[i].obj_id].anim_frame = 0;
        }
        i++;
    }
}

