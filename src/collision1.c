/*
 * Collision engine, first object (GOG v1.21 1:1b2d0-1:1f1e0). File name invented (Rayverse collision.c).
 * Names: Rayverse/Android. Worker status: 24/37 STRICT; see NOTES.md.
 * Whole-object layout remains unresolved. No promotion is claimed.
 */
#include "obj.h"
#include "game.h"
extern obj_type_flags_t flags[262];
#include "map.h"
#include "collision1_local.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>

typedef struct obj_procs_t {
    void (*command)(obj_t *obj);
    void (*poing_collision)(obj_t *obj, s16 sprite);
    void (*rayman_collision)(obj_t *obj);
    void (*rayman_in_zone)(obj_t *obj);
    void (*u_turn)(obj_t *obj);
} obj_procs_t;
extern obj_procs_t ObjectsFonctions[262];
extern u8 dead_time;
extern s16 ray_speed_inv;
extern u8 in_air_because_hit;
extern s16 jump_time;

typedef int bool;
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])
#define true 1
#define false 0

void get_spi_zdc(obj_t* obj, s16* x, s16* y, s16* w, s16* h);
void start_cd_perdu(void);
void GET_ANIM_POS(obj_t* obj, s16* x, s16* y, s16* w, s16* h);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void GET_RAY_ZDC(obj_t* obj, s16* x, s16* y, s16* w, s16* h);
void GET_BB1_ZDCs(obj_t* obj, s16 *x_1, s16 *y_1, s16 *w_1, s16 *h_1, s16 *x_2, s16 *y_2, s16 *w_2, s16 *h_2);
void calc_obj_dir(obj_t* obj);
u8 BTYP(s32 tile_x, s32 tile_y);
s32 bat_get_eject_sens(obj_t* obj);
u8 calc_typ_trav(obj_t* obj, u8 a2);
void make_active(obj_t* obj, u8 do_nova);
void suppressFromLinkList(obj_t* obj);
void obj_init(obj_t* obj);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
s32 x_pos(u8 param_1, u8 nb_couteau);
s32 y_pos(s16 param_1, s16 param_2);
void get_cou_zdc(obj_t* obj, s16 *x, s16 *y, s16 *w, s16 *h);
s32 pma_get_eject_sens(obj_t* obj);
void fin_poing_follow(u8 a1);
void allocatePoingBoum(void);
void Reset_air_speed(u8 is_rolling_speed);
void RAY_TOMBE(void);
void RAY_HURT(void);
u8 bonus_taken(s16 obj_id);
s32 saxo2_get_eject_sens(void);
s32 sko_get_eject_sens(void);
void stop_all_snd(void);
void PlaySnd(s32 snd, s32 obj_id);

extern obj_t ray;
extern s16 ray_zdc_x;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern s16 ray_zdc_y;
extern s16 pente[16];
extern s16 num_world;
extern u8 block_flags[64];
extern zdc_t zdc_tab[200];
extern s16 bagDy[20];
extern s16 bagW[20];
extern s16 bagH[20];
extern obj_t* poing_obj;
extern s16 poing_obj_id;
extern s16 decalage_en_cours;
extern map_data_t mp;
extern level_t level;
extern active_objects_t actobj;
extern s16* link_init;
extern s16 ymap;
extern poing_t poing;
extern rayevts_t RayEvts;
extern s16 helico_time;
extern u8 button_released;
extern s16 ray_mode;
extern s16 scroll_start_y;
extern s16 scroll_end_y;
extern u8 fin_boss;
extern s16 new_world;
extern s16 new_level;
extern u8 boss_mort;
extern u16 ZDCPTR;

//2B2D0
s32 get_nb_zdc(obj_t* obj) {
    return obj->zdc >> 11;
}

//2B2E0
s16 get_zdc_index(obj_t* obj) {
    return obj->zdc & 0x7FF;
}

//2B2E8
zdc_t* get_zdc(obj_t* obj, s16 a2) {
    return zdc_tab + (get_zdc_index(obj) + a2);
}

//2B300
u16 get_ZDCPTR(void) {
    return ZDCPTR;
}

//2B308
s16 in_coll_sprite_list(obj_t* obj, s16 a2) {
    /* 1B2E0 8013FAE0 -O2 -msoft-float */
    u8 unk_1[12];
    s16 i;
    s16 res = false;

    switch (obj->type)
    {
        case TYPE_GENEBADGUY:
            unk_1[0] = 0;
            unk_1[1] = 1;
            unk_1[2] = 2;
            unk_1[3] = 0xFF;
            break;
        case TYPE_MITE:
        case TYPE_MITE2:
            unk_1[0] = 1;
            if (obj->sub_etat == 11)
            {
                if (obj->main_etat == 2)
                    unk_1[1] = 3;
                else
                {
                    if (obj->anim_frame > 15)
                        unk_1[1] = 2;
                    else
                        unk_1[1] = 0xFF;
                }
                unk_1[2] = 0xFF;
            }
            else
                unk_1[1] = 0xFF;
            break;
        default:
            unk_1[0] = 0;
            unk_1[1] = 0xFF;
            break;
    }

    i = 0;
    do
    {
        res = a2 == unk_1[i++];
    } while (unk_1[i] != 0xFF && res != true);

    return res;
}

//2B3B8
s16 box_inter_v_line(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    s16 res = 0;
    if (a1 < a5 && a2 > a5) {
        if (a3 > a6 && a3 < a7)
            res = 1;
        else if (a4 > a6 && a4 < a7)
            res = 1;
    }
    return res;
}

s16 box_inter_h_line(s16 a1, s16 a2, s16 a3, s16 a4, s16 a5, s16 a6, s16 a7) {
    s16 res = 0;
    if (a3 < a5 && a4 > a5) {
        if (a1 > a6 && a1 < a7)
            res = 1;
        else if (a2 > a6 && a2 < a7)
            res = 1;
    }
    return res;
}

//2B430
s16 inter_box(s32 x_1, s32 y_1, s32 w_1, s32 h_1, s16 x_2, s16 y_2, s32 w_2, s32 h_2) {
    s16 comp_x;
    s16 comp_y;
    s16 sum_w;
    s16 sum_h;

    comp_x = x_1 - w_2;
    comp_y = y_1 - h_2;
    sum_w = w_1 + w_2;
    sum_h = h_1 + h_2;
    if (comp_x <= x_2 && comp_y <= y_2 &&
        comp_x + sum_w >= x_2 && comp_y + sum_h >= y_2) return 1;
    return 0;
}

//2B48C
void GET_OBJ_ZDC(obj_t* obj, s16 *out_x, s16 *out_y, s16 *out_w, s16 *out_h) {
    s16 anim_x; s16 anim_y; s16 anim_w; s16 anim_h;
    s32 inset;
    s16 old_flip_x;
    s16 bc_x_pos;
    s16 new_x;
    u8 unk_3;
    s16 new_y;

    switch (obj->type)
    {
        case TYPE_STALAG:
            GET_SPRITE_POS(obj, 1, out_x, out_y, out_w, out_h);
            if (*out_w > *out_h)
            {
                *out_x += 4;
                *out_w -= 8;
                *out_y += 8;
                *out_h -= 12;
            }
            else
            {
                *out_x += (*out_w >> 1) - 3;
                *out_w = 6;
                *out_y += 16;
                *out_h -= 32;
            }
            break;
        case TYPE_BLACK_RAY:
            GET_RAY_ZDC(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_STONECHIP:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 13;
            *out_y = anim_y + 13;
            *out_w = 4;
            *out_h = 4;
            break;
        case TYPE_POWERUP:
        case TYPE_FALLING_OBJ:
        case TYPE_FALLING_OBJ2:
        case TYPE_FALLING_OBJ3:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 10;
            *out_y = anim_y + 10;
            *out_w = anim_w - 20;
            *out_h = anim_h - 20;
            break;
        case TYPE_73_RING:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_FLASH:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            *out_x += (*out_w >> 1) - 2;
            *out_y += (*out_h >> 1) - 2;
            *out_w = 4;
            *out_h = 4;
            break;
        case TYPE_ECLAIR:
        case TYPE_ETINC:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 10;
            *out_y = anim_y + 4;
            *out_w = anim_w - 30;
            *out_h = anim_h - 8;
            break;
        case TYPE_CLASH:
        case TYPE_NOTE:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 2;
            *out_y = anim_y - 4;
            *out_w = anim_w - 4;
            *out_h = anim_h + 3;
            break;
        case TYPE_BBL:
            old_flip_x = obj->flags.flip_x;
            obj->flags.flip_x = 0;
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h); /* ... */
            obj->flags.flip_x = old_flip_x;
            if (obj->param != 2)
            {
                *out_x = anim_x + 3;
                *out_y = anim_y + 20;
                *out_w = 50;
                *out_h = 40;
            }
            else
            {
                *out_x = anim_x + 20;
                *out_y = anim_y + 35;
                *out_w = 1;
                *out_h = 1;
            }
            break;
        case TYPE_MARTEAU:
        case TYPE_MOVE_MARTEAU:
            GET_SPRITE_POS(obj, 2, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x;
            *out_y = anim_y;
            *out_w = 16;
            *out_h = 32;
            break;
        case TYPE_STONEDOG:
        case TYPE_STONEDOG2:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 4;
            *out_y = anim_y + 2;
            *out_w = anim_w - 8;
            *out_h = anim_h - 6;
            break;
        case TYPE_CAGE:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 8;
            *out_y = anim_y + 8;
            *out_w = anim_w - 16;
            *out_h = anim_h - 16;
            break;
        case TYPE_STONEMAN1:
        case TYPE_STONEMAN2:
        case TYPE_STONEWOMAN2:
        case TYPE_STONEWOMAN:
            *out_x = obj->x + obj->offset_bx - 8;
            *out_y = obj->y + obj->offset_by - 60;
            *out_w = 16;
            *out_h = 50;
            if (obj->main_etat == 0) {
                if (obj->sub_etat == 5) {
                    *out_y += obj->anim_frame * 2;
                    *out_h -= obj->anim_frame * 2;
                } else if (obj->sub_etat == 6) {
                    *out_h >>= 1;
                    *out_y += *out_h;
                    *out_y -= min32(15, obj->anim_frame) * 2;
                    *out_h += min32(15, obj->anim_frame) * 2;
                }
            }
            break;
        case TYPE_CAGE2:
            *out_x = obj->x;
            *out_y = obj->y;
            *out_w = 0;
            *out_h = 0;
            break;
        case TYPE_WIZARD1:
            *out_x = obj->x;
            *out_y = obj->y;
            *out_w = 50;
            *out_h = 110;
            break;
        case TYPE_BIG_CLOWN:
            if (obj->main_etat == 0 && obj->sub_etat == 2 && obj->anim_frame >= 16) {
                *out_x = (obj->flags.flip_x ? 80 : 16) + obj->x;
                *out_y = obj->y + 40;
                *out_w = 68;
                *out_h = 48;
            } else {
                if (obj->main_etat == 0 && obj->sub_etat == 2)
                    *out_x = (obj->flags.flip_x ? 80 : 48) + obj->x;
                else
                    *out_x = obj->x + 64;
                *out_y = obj->y + 38;
                *out_w = 32;
                *out_h = 48;
            }
            break;
        case TYPE_66_DARD:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_DROP:
            GET_SPRITE_POS(obj, 0, &anim_x, &anim_y, &anim_w, &anim_h);
            if (obj->main_etat == 2 && obj->sub_etat == 2) {
                *out_x = anim_x + 5;
                *out_y = anim_y + 28;
                *out_w = 6;
                *out_h = 22;
            } else if (obj->main_etat == 2 && obj->sub_etat == 1) {
                *out_x = anim_x + 6;
                *out_y = anim_y + 18;
                *out_w = 13;
                *out_h = 11;
            } else if (obj->main_etat == 2 && obj->sub_etat == 0) {
                *out_x = anim_x + 7;
                *out_y = anim_y + 8;
                *out_w = 19;
                *out_h = 11;
            }
            break;
        case TYPE_TROMPETTE:
            GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);
            *out_x = anim_x + 24;
            *out_y = anim_y + 14;
            *out_w = anim_w - 48;
            *out_h = anim_h - 27;
            break;
        case TYPE_TNT_BOMB:
            *out_x = obj->x + 76;
            *out_y = obj->y + 85;
            *out_w = 8;
            *out_h = 8;
            break;
        case TYPE_EXPLOSION:
            *out_x = obj->x;
            *out_y = obj->y;
            if (obj->sub_etat == 0) {
                *out_w = 22;
                *out_h = 22;
                *out_x += 68;
                *out_y += 65;
            } else {
                *out_w = 0;
                *out_h = 0;
            }
            break;
        case TYPE_COUTEAU:
            get_cou_zdc(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_SPIDER_PLAFOND:
            get_spi_zdc(obj, out_x, out_y, out_w, out_h);
            break;
        case TYPE_POI3:
            if ((obj->main_etat == 2 && obj->sub_etat == 15) ||
                (obj->main_etat == 2 && obj->sub_etat == 12) ||
                (obj->main_etat == 2 && obj->sub_etat == 13)) {
                *out_x = obj->x + 54;
                *out_y = obj->y + 55;
                *out_w = 14;
                *out_h = 21;
            } else {
                *out_x = obj->x + 74;
                *out_y = obj->y + 45;
                *out_w = 14;
                *out_h = 21;
            }
            break;
        case TYPE_PETIT_COUTEAU:
            if (obj->hit_points == 1)
            {
                *out_x = obj->x + 138;
                *out_y = obj->y + 138;
                *out_w = 43;
                *out_h = 15;
            }
            if (obj->hit_points == 2)
            {
                *out_x = obj->x + 131;
                *out_y = obj->y + 88;
                *out_w = 17;
                *out_h = 49;
            }
            if (obj->hit_points == 3)
            {
                *out_x = obj->x + 62;
                *out_y = obj->y + 138;
                *out_w = 43;
                *out_h = 16;
            }
            if (obj->hit_points == 4)
            {
                *out_x = obj->x + 96;
                *out_y = obj->y + 87;
                *out_w = 17;
                *out_h = 48;
            }
            break;
        case TYPE_164_GENERATING_DOOR:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            /* DOS 1:1bfce..1:1bfe9 extracts the two flag bitfields separately. */
            inset = 24 - 12 * (get_eta(obj)->flags_bits.bit7 + 2 * get_eta(obj)->flags_bits.bit6);
            *out_x += inset;
            *out_y += inset;
            *out_w -= inset * 2;
            *out_h -= inset * 2;
            break;
        default:
            GET_ANIM_POS(obj, out_x, out_y, out_w, out_h);
            break;
    }

}

//2C028
/* DOS BOX_IN_COLL_ZONES 1:1c9d0..1c9f9 passes a sign-extended index. */
s16 GET_SPRITE_ZDC(obj_t* obj, s16 index, s16 *out_x, s16 *out_y, s16 *out_w, s16 *out_h) {
    s16 unk_y; s16 unk_x; s16 unk_h; s16 unk_w; s16 spr_x; u16 type; s16 spr_y; s16 spr_w; s16 spr_h; s16 succ; s16 temp_h;

    type = obj->type;
    unk_h = 0;
    unk_w = 0;
    unk_y = 0;
    unk_x = 0;
    succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);
    if (succ)
    {
        switch (type)
        {
            case TYPE_CYMBALE:
                switch (index)
                {
                    case 3:
                        unk_x = 4;
                        unk_y = 2;
                        unk_w = -4;
                        unk_h = -10;
                        break;
                    case 4:
                        unk_x = 0;
                        unk_y = 2;
                        unk_w = 0;
                        unk_h = -10;
                        break;
                    case 5:
                        unk_x = 0;
                        unk_y = 2;
                        unk_w = -4;
                        unk_h = -10;
                        break;
                }
                break;
            case TYPE_GENEBADGUY:
                if (
                        (obj->main_etat == 0 && obj->sub_etat == 0) ||
                        (obj->main_etat == 1 && obj->sub_etat == 0)
                        )
                {
                    switch (index)
                    {
                        case 0:
                            unk_x = 10;
                            unk_y = 0;
                            spr_w = 34;
                            spr_h = 32;
                            break;
                        case 1:
                            unk_x = 4;
                            unk_y = 0;
                            spr_w = 26;
                            break;
                        case 2:
                            unk_x = 4;
                            unk_y = 0;
                            spr_w = 48;
                            spr_h = 24;
                            break;
                    }
                }
                else if (obj->main_etat == 0 && obj->sub_etat == 1)
                {
                    spr_x = obj->x + 112;
                    spr_y = obj->y + 184;
                    spr_w = 32;
                    spr_h = 16;
                }
                else
                {
                    spr_w = 0;
                    spr_h = 0;
                    succ = false;
                }
                break;
            case TYPE_MITE:
            case TYPE_MITE2:
                switch (index)
                {
                    case 1:
                        if (obj->main_etat == 0 && (obj->sub_etat == 9 || obj->sub_etat == 10 || obj->sub_etat == 11)) {
                            unk_x = -8;
                            unk_y = 5;
                            spr_w = 26;
                            spr_h = 8;
                        } else {
                            unk_x = 1;
                            unk_y = -5;
                            spr_w = 16;
                            spr_h = 30;
                        }
                        break;
                    case 2:
                        unk_x = 0;
                        unk_y = 7;
                        spr_w = 16;
                        spr_h = 16;
                        break;
                    case 3:
                        unk_x = 0;
                        unk_y = 2;
                        spr_w = 13;
                        spr_h = 9;
                        break;
                }
                break;
        }
        *out_x = spr_x + unk_x;
        *out_y = spr_y + unk_y;
        *out_w = spr_w + unk_w;
        *out_h = spr_h + unk_h;
    }
    return succ;
}

//2C33C
s16 BOX_HIT_SPECIAL_ZDC(s16 in_x, s16 in_y, s16 in_w, s16 in_h, obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 frame;
    s16 d;
    s16 bb1_x_2; s16 bb1_y_2; s16 bb1_w_2; s16 bb1_h_2; s16 bb1_x_1; s16 bb1_y_1; s16 bb1_h_1; s16 bb1_w_1;
    s16 res = -1;

    switch (obj->type) {
        case TYPE_BAG1:
            frame = obj->anim_frame;
            d = bagDy[frame];
            if (d != -1) {
                if (inter_box(in_x, in_y, in_w, in_h,
                        obj->x + obj->offset_bx - (bagW[frame] >> 1),
                        obj->y + obj->offset_by + d - bagH[frame],
                        bagW[frame],
                        bagH[frame]
                ))
                    res = 1;
            }
            break;
        case TYPE_BB1:
        case TYPE_BB12:
        case TYPE_BB13:
        case TYPE_BB14:
            if (in_h == ray_zdc_y) {
                GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);

                if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1))
                    res = 1;
                else if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2))
                    res = 1;
            } else {
                GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);

                if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1))
                    res = 6;
                if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2))
                    res = 1;

                if (obj->main_etat == 0 && obj->sub_etat == 10) {
                    GET_SPRITE_POS(obj, 9, &bb1_x_1, &bb1_y_1, &bb1_h_1, &bb1_w_1);
                    if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1))
                        res = 9;
                }
            }
            break;
    }
    return res;
}

//2C670
s16 BOX_IN_COLL_ZONES(u16 type, s16 x, s16 y, s16 w, s16 h, obj_t* obj) {
    /* 1C7E4 80140FE4 -O2 -msoft-float */
    s16 res;
    s16 unk_x; s16 unk_y; s16 unk_w; s16 unk_h;
    s16 nb_zdc;
    s16 i;
    s16 sprite_index;
    zdc_t* cur_zdc;
    anim_t* cur_anim;
    anim_layer_t* cur_layer;
    s16 layers_count;

    unk_h = 0;
    unk_w = 0;
    unk_y = 0;
    unk_x = 0;
    res = -1;



    if (obj->zdc != 0)
    {
        nb_zdc = get_nb_zdc(obj);
        if (
                num_world == 1 &&
                (
                        ((obj->type == TYPE_MOSKITO || obj->type == TYPE_MOSKITO2) &&
                         !(obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)) ||
                        (obj->type == TYPE_BADGUY3 &&
                         obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)
                )
                )
            nb_zdc--;

        for (i = 0; i < nb_zdc; i++)
        {
            cur_zdc = get_zdc(obj, i);
            if (!(cur_zdc->flags & 4) || type == TYPE_POING)
            {
                if (cur_zdc->flags & 2)
                {
                    cur_anim = &obj->animations[obj->anim_index];
                    cur_layer = &cur_anim->layers[obj->anim_frame * (cur_anim->layers_per_frame & 0x3FFF)];
                    if (cur_layer[cur_zdc->sprite].sprite_index != 0)
                    {
                        GET_SPRITE_POS(obj, cur_zdc->sprite, &unk_x, &unk_y, &unk_w, &unk_h);
                        if (obj->flags.flip_x)
                            unk_x += unk_w - cur_zdc->width - cur_zdc->x_pos;
                        else
                            unk_x += cur_zdc->x_pos;

                        unk_y += cur_zdc->y_pos;
                        unk_w = cur_zdc->width;
                        unk_h = cur_zdc->height;
                    }
                }
                else
                {
                    unk_x = obj->x + cur_zdc->x_pos;
                    unk_y = obj->y + cur_zdc->y_pos;
                    unk_w = cur_zdc->width;
                    unk_h = cur_zdc->height;
                    /* TODO: can't take << 1 or * 2??? */
                    if (obj->flags.flip_x)
                        unk_x =
                                (obj->x + obj->offset_bx) + (obj->x + obj->offset_bx) -
                                (unk_x + unk_w);
                }

                if (inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h))
                {
                    res = cur_zdc->sprite;
                    break;
                }
            }
        }
    }
    else
    {
        if (obj->hit_sprite == 0xFE)
        {
            GET_OBJ_ZDC(obj, &unk_x, &unk_y, &unk_w, &unk_h);
            if (obj->flags.flip_x)
                unk_x =
                        (obj->x + obj->offset_bx) + (obj->x + obj->offset_bx) -
                        (unk_x + unk_w);

            res = inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h);
            res = res ? res : -1;
        }
        else if (obj->hit_sprite == 0xFF)
        {
            layers_count = obj->animations[obj->anim_index].layers_per_frame & 0x3FFF;
            for (sprite_index = 0; sprite_index < layers_count; sprite_index++)
            {
                if (
                        in_coll_sprite_list(obj, sprite_index) &&
                        GET_SPRITE_ZDC(obj, sprite_index, &unk_x, &unk_y, &unk_w, &unk_h) &&
                        inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)
                        )
                {
                    res = sprite_index;
                    break;
                }
            }
        }
    }

    return res;
}

//2CA84
s16 COLL_BOX_SPRITE(s16 x, s16 y, s16 w, s16 h, obj_t* obj) {
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;
    s16 succ;
    succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);
    if (succ) succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h);
    return succ ? obj->hit_sprite : -1;
}

//2CB1C
s16 CHECK_BOX_COLLISION(u16 type, s16 x, s16 y, s16 w, s16 h, obj_t* obj) {
    s16 hit_sprite = obj->hit_sprite;
    switch (hit_sprite) {
        case 0xFD:
            return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);
        case 0xFE:
        case 0xFF:
            return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);
        default:
            return COLL_BOX_SPRITE(x, y, w, h, obj);
    }
}

//2CB9C
s16 possible_sprite(obj_t* obj, s16 index) {
    /* 1CF08 80141708 -O2 -msoft-float */
    s16 spr[10];

    switch (obj->type) {
        case TYPE_258_NOUGAT:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 2;
            spr[3] = 255;
            break;
        case TYPE_CYMBALE:
        case TYPE_CYMBAL2:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 2;
            spr[3] = 255;
            break;
        case TYPE_BON3:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 2;
            spr[3] = 3;
            spr[4] = 4;
            spr[5] = 255;
            break;
        case TYPE_ROULETTE:
            spr[0] = 5;
            spr[1] = 6;
            spr[2] = 7;
            spr[3] = 8;
            spr[4] = 255;
            break;
        case TYPE_ROULETTE2:
            spr[0] = 1;
            spr[1] = 2;
            spr[2] = 3;
            spr[3] = 4;
            spr[4] = 255;
            break;
        case TYPE_ROULETTE3:
            spr[0] = 1;
            spr[1] = 2;
            spr[2] = 3;
            spr[3] = 255;
            break;
        case TYPE_FALLPLAT:
        case TYPE_LIFTPLAT:
        case TYPE_INTERACTPLT:
            spr[0] = 0;
            spr[1] = 1;
            spr[2] = 255;
            break;
        case TYPE_TIBETAIN_6:
            if (ray.flags.flip_x)
            {
                spr[0] = 4;
                spr[1] = 5;
                spr[2] = 6;
                spr[3] = 7;
                spr[4] = 8;
                spr[5] = 9;
            }
            else
            {
                spr[0] = 9;
                spr[1] = 8;
                spr[2] = 7;
                spr[3] = 6;
                spr[4] = 5;
                spr[5] = 4;
            }
            spr[6] = 255;
            break;
        case TYPE_TIBETAIN_2:
            spr[0] = 4;
            spr[1] = 5;
            spr[2] = 0x00FF;
            break;
    }
    return spr[index];
}

//2CE34
s16 setToleranceDist(s16 x, s16 w, s16 y) {
    /* 1D11C 8014191C -O2 -msoft-float */
    s16 ray_x = ray.x + ray.offset_bx;
    s16 ray_y = ray.y + ray.offset_by;
    s16 unk_1 = x + w - 1;

    s16 unk_2 = 8;
    if (RayEvts.tiny) unk_2 = 4;
    /* DOS 1:1ced4..1cf09: conditional absolute value. */
    if (inrange(ray_x, x - unk_2, unk_1 + unk_2)) {
        if (inrange(ray_x, x, unk_1)) return ((ray_y - y < 0) ? -(ray_y - y) : ray_y - y);
        else return -((ray_y - y < 0) ? -(ray_y - y) : ray_y - y) - 1;
    }
    return 10000;
}

//2CF14
s16 cosinus(s16 angle);

void SET_RAY_DIST_SLOPEY_PLAT(obj_t* obj) {
    s16 limit;
    s32 angle;
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;
    s16 ray_x;
    s16 ray_y;
    s16 projected_w;
    s16 slope_x2;
    s16 diff_y;
    s16 diff_x;

    limit = 8;
    if (RayEvts.tiny) limit = 4;
    GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);
    /* DOS 1:1cf57 loads signed hit_points at +0x76 (dword at +0x73, SAR 24). */
    angle = (s8)obj->hit_points;
    if (angle < 0) angle = -angle;
    projected_w = ((s32)cosinus(pente[angle]) * spr_w) >> 9;
    spr_x += (spr_w - projected_w) >> 1;
    spr_w = projected_w;
    slope_x2 = spr_x + spr_w - 1;
    spr_y += obj->offset_hy;
    ray_x = ray.x + ray.offset_bx;
    ray_y = ray.y + ray.offset_by;
    if (inrange(ray_x, spr_x - limit, slope_x2 + limit)) {
        diff_y = spr_y - ray_y;
        if (((s32)diff_y < 0 ? -(s32)diff_y : (s32)diff_y) < spr_w) {
            diff_x = ray_x - (spr_x + (spr_w >> 1));
            spr_h = diff_x * (s8)obj->hit_points;
            /* DOS 1:1d058: negate at 16 bits before the arithmetic shift. */
            spr_h = ashr16(spr_h, 4);
            if (inrange(ray_x, spr_x, slope_x2)) {
                obj->ray_dist = (diff_y - spr_h < 0) ? -(diff_y - spr_h) : diff_y - spr_h;
            } else {
                obj->ray_dist = (diff_y - spr_h < 0) ? -(diff_y - spr_h) : diff_y - spr_h;
                obj->ray_dist = -obj->ray_dist;
            }
            return;
        }
    }
    obj->ray_dist = 10000;
}

//2D0D4
void SET_RAY_DIST_MULTISPR_CANTCHANGE(obj_t* obj) {
    /* 1D228 80141A28 -O2 -msoft-float */

    s16 i;
    s16 new_dist;
    s16 spr_y; s16 unk_1;
    s16 sprite; s16 spr_w; s16 spr_x; s16 spr_h;
    s16 diff_x;
    s16 ray_y;

    s16 ray_x;
    unk_1 = 8;
    if (RayEvts.tiny) unk_1 = 4;
    i = 0;
    new_dist = 10000;
    ray_x = ray.x + ray.offset_bx;
    ray_y = ray.y + ray.offset_by;

    if (ray.follow_id == obj->id) {
        GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);
        spr_y += obj->offset_hy;
        if (obj->type == TYPE_ROULETTE || obj->type == TYPE_ROULETTE2 || obj->type == TYPE_ROULETTE3) {
            spr_w -= 10;
            spr_x += 5;
        }

        if (obj->type == TYPE_TIBETAIN_6 && ray.main_etat == 0 && (obj->anim_frame > 9 && obj->anim_frame < 40))
        {
            diff_x = (ray.x + ray.offset_bx) - (spr_x + (spr_w >> 1));
            if (diff_x > 0)
                ray.x--;
            else if (diff_x < 0)
                ray.x++;
            ray_x = ray.x + ray.offset_bx;
        }
        new_dist = setToleranceDist(spr_x, spr_w, spr_y);
    }

    if (new_dist == 10000)
    {
        sprite = possible_sprite(obj, i++);
        while (((s32)new_dist < 0 ? -(s32)new_dist : (s32)new_dist) > 7 && sprite != 0xFF) {
            GET_SPRITE_POS(obj, sprite, &spr_x, &spr_y, &spr_w, &spr_h);
            spr_y += obj->offset_hy;
            if (obj->type == TYPE_ROULETTE || obj->type == TYPE_ROULETTE2 || obj->type == TYPE_ROULETTE3)
            {
                spr_w -= 10;
                spr_x += 5;
            }

            if ((ray_x <= spr_x + spr_w + unk_1) && (ray_x >= spr_x - unk_1))
            {
                new_dist = ray_y - spr_y;
                if (obj->type == TYPE_TIBETAIN_6 && (new_dist >= 8 && new_dist < 11))
                    new_dist = 0;
            }
            else
                new_dist = 10000;

            if (new_dist != 10000)
                obj->follow_sprite = sprite;



            sprite = possible_sprite(obj, i++);
        }
    }
    obj->ray_dist = new_dist;
}

//2D334
void SET_RAY_DIST_PI(obj_t* obj) {
    /* 1D594 80141D94 -O2 -msoft-float */
    s16 x; s16 y; s16 w; s16 h;
    s16 new_dist;
    s16 prev_flip_x = obj->flags.flip_x;

    obj->flags.flip_x = 0;
    GET_SPRITE_POS(obj, 2, &x, &y, &w, &h);
    obj->flags.flip_x = prev_flip_x;
    y += obj->offset_hy;
    x += 4;
    w = 55;
    new_dist = setToleranceDist(x, w, y);
    if (new_dist != 10000)
        obj->follow_sprite = 2;
    obj->ray_dist = new_dist;
}

//2D3EC
void SET_RAY_DIST_BAG(obj_t* obj) {
    /* 1D66C 80141E6C -O2 -msoft-float */
    s16 x; s16 h; s16 w; s16 y;
    s16 new_w;
    s16 new_dist = 0;

    // NOTE: slight difference from PS1 version: parentheses placed differently
    if ((obj->sub_etat == 3 ||obj->sub_etat == 6) && obj->anim_frame < 12) {
        GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);
        new_w = 45;
        y += obj->offset_hy;
        x += ((s16) (w - new_w) >> 1);
        w = new_w;
        new_dist = setToleranceDist(x, w, y);
    } else {
        new_dist = 10000;
    }

    if (ray.follow_id == obj->id && obj->sub_etat == 6 && obj->anim_frame == 11) {
        ray.follow_id = -1;
        set_main_and_sub_etat(&ray, 2, 0);
        new_dist = 10000;
        ray.speed_y -= 10;
    }
    obj->ray_dist = new_dist;
}

//2D4D0
void SET_RAY_DIST(obj_t* obj) {
    /* 1D798 80141F98 -O2 -msoft-float */
    s16 x; s16 y; s16 w; s16 h;

    if (flags[obj->type].raw & flags1_8_ray_dist_multispr_cantchange) {
        SET_RAY_DIST_MULTISPR_CANTCHANGE(obj);
    } else {
        if (obj->type == TYPE_PI || obj->type == TYPE_BBL)
            SET_RAY_DIST_PI(obj);
        else if (obj->type == TYPE_BAG3)
            SET_RAY_DIST_BAG(obj);
        else if (obj->type == TYPE_254_SLOPEY_PLAT)
            SET_RAY_DIST_SLOPEY_PLAT(obj);
        else
        {
            GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);
            y += obj->offset_hy;
                    if (
                    obj->type == TYPE_FALLING_OBJ || obj->type == TYPE_FALLING_OBJ2 || obj->type == TYPE_FALLING_OBJ3 ||
                    obj->type == TYPE_FALLING_YING || obj->type == TYPE_FALLING_YING_OUYE
                    )
            {
                x -= 2;
                w += 4;
            }
            obj->ray_dist = setToleranceDist(x, w, y);
        }
    }
}

//2D5CC
void do_boum(void) {
    allocatePoingBoum();
    poing.is_boum = true;
    if (!poing.is_returning) {
        poing.charge = 0;
        poing_obj->speed_x = 0;
    }
}

//2D5FC
void DO_POING_COLLISION(void) {
    s16 x, y, w, h;
    u8 btyp;
    s16 i;
    obj_t *obj;
    s16 sprite;
    s16 old_hp;
    s32 unk_1;

    btyp = 0;
    GET_SPRITE_POS(poing_obj, 0, &x, &y, &w, &h);
    if (poing_obj->speed_x >= 0) {
        btyp = BTYP((s16)((poing_obj->x + poing_obj->offset_bx) >> 4),
                   (s16)((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4));
    } else if (poing_obj->speed_x < 0) {
        btyp = BTYP((s16)((poing_obj->x + poing_obj->offset_bx) >> 4),
                   (s16)((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4));
    }
    if (block_flags[btyp] & 2) {
        do_boum();
        fin_poing_follow(1);
    } else {
        i = 0;
        obj = level.objects + actobj.objects[i];
        while (i < actobj.num_active_objects) {
            if (new_world || new_level || fin_boss || boss_mort) {
                break;
            }
            if (obj->display_prio != 0 && obj->hit_points != 0 && (get_eta(obj)->flags & 8)) {
                sprite = CHECK_BOX_COLLISION(TYPE_POING, x, y, w, h, obj);
                if (sprite != -1) {
                    old_hp = obj->hit_points;
                    ObjectsFonctions[obj->type].poing_collision(obj, sprite);
                    do_boum();
                    obj->gravity_value_1 = 0;
                    /* DOS 1:1d7c4..1d7d7: high nibble, then promotion. */
                    unk_1 = get_eta(obj)->anim_speed_bits.high;
                    if (!(unk_1 == 11 || unk_1 == 10)) {
                        obj->gravity_value_2 = 0;
                    }
                    if (obj->hit_points == old_hp && (flags[obj->type].raw & flags3_1_poing_collision_snd)) {
                        PlaySnd(214, (flags[obj->type].raw & 0x80) ? poing_obj_id : obj->id);
                    }
                    break;
                }
            }
            ++i;
            obj = level.objects + actobj.objects[i];
        }
    }
}

//2D858
void DoPoingCollisionDefault(obj_t* obj, s16 sprite) {
    //nullsub
}

//2D85C
s16 COLL_BOX_ALL_SPRITES(s16 in_x, s16 in_y, s16 in_w, s16 in_h, obj_t *obj) {
    s16 i;
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;
    s16 res;

    for (i = 0; i < (obj->animations[obj->anim_index].layers_per_frame & 0x3fff); i++) {
        GET_SPRITE_POS(obj, i, &spr_x, &spr_y, &spr_w, &spr_h);
        if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) {
            res = i;
            break;
        }
    }
    return res;
}

//2D914
s16 COLL_RAY_PIC(void) {
    s32 index, left, right;
    s16 res = 0;
    if (ray.y + ray.offset_by < mp.height * 16) {
        index = ray.ray_dist;
        if (ray.follow_id == -1)
            res = mp.map[index].tile_type == BTYP_HURT;
        if (!res) {
            index -= mp.width;
            res = mp.map[index].tile_type == BTYP_HURT;
            if (!res) {
                right = index + 1;
                left = index - 1;
                if (ray.main_etat != 2 || ray.speed_x != 0)
                    res = mp.map[left].tile_type == BTYP_HURT || mp.map[right].tile_type == BTYP_HURT;
                if (!res && !RayEvts.tiny && !(get_eta(&ray)->flags & 0x40)) {
                    index -= mp.width;
                    left = index - mp.width;
                    while (left < 0) left += mp.width;
                    while (index < 0) index += mp.width;
                    res = mp.map[left].tile_type == BTYP_HURT || mp.map[index].tile_type == BTYP_HURT;
                }
            }
        }
    }
    return res;
}

//2DA98
void COLL_RAY_BLK_MORTEL(void) {
    if ((ray.y + ray.offset_by) < (mp.height * 16)) {
        u8 btyp = mp.map[ray.ray_dist].tile_type;
        if (btyp == BTYP_SPIKES) {
            set_main_and_sub_etat(&ray, 3, 32);
            if (dead_time != 1) {
                dead_time = 1;
                stop_all_snd();
                start_cd_perdu();
            }
            { s32 next_y = ray.y + 5;
            ray.speed_y = 0;
            ray.speed_x = 0;
            ray.y = next_y; }
        } else if (ray.btypes[3] == BTYP_SPIKES) {
            set_main_and_sub_etat(&ray, 3, 32);
            if (dead_time != 1) {
                dead_time = 1;
                stop_all_snd();
                start_cd_perdu();
            }
            ray.speed_y = 0;
            ray.speed_x = 0;
        } else if (btyp == BTYP_CLIFF) {
            set_main_and_sub_etat(&ray, 2, 31);
            scroll_end_y = ymap;
            scroll_start_y = ymap;
        }
    }
}

//2DBAC
void RAY_KO(void) {
    set_main_and_sub_etat(&ray, 3, 0);
    ray.speed_y = 0;
    ray.speed_x = 0;
    ray.config = 0;
}

//2DBDC
void RAY_HIT(u8 hurt, obj_t* obj) {
    if (hurt) {
        s16 tile_x;
        RAY_HURT();
        tile_x = ray.x + ray.offset_bx;
        while (BTYP((s16)(tile_x >> 4), (s16)((ray.y + ray.offset_by) >> 4)) == BTYP_WATER) {
            --ray.y;
        }
        ray.btypes[0] = BTYP_NONE;
    }
    if (ray.main_etat == 6) {
        set_main_and_sub_etat(&ray, 6, 8);
        ray.speed_y = 0;
        ray.speed_x = 0;
        poing.is_charging = 0;
    } else if (ray.flags.alive) {
        if (!(ray.main_etat == 3 && (ray.sub_etat == 22 || ray.sub_etat == 32)) &&
            !(ray.main_etat == 2 && ray.sub_etat == 31)
        ) {
            if ((get_eta(&ray)->flags & 0x40) && (block_flags[calc_typ_trav(&ray, 2)] & 0x10)) {
                set_main_and_sub_etat(&ray, 0, 61);
            } else {
                set_main_and_sub_etat(&ray, 2, 8);
            }
            ray_speed_inv = 0;
            if (obj == NULL && !(ray.main_etat == 0 && ray.sub_etat == 61)) {
                ray.speed_x = ray.flags.flip_x ? -2 : 2;
                ray.speed_y = -3;
            } else if (!(ray.main_etat == 0 && ray.sub_etat == 61)){
                s16 eject_sens = -1;
                s16 bump_speed;
                if (obj->type == TYPE_180_SAXO2) {
                    eject_sens = saxo2_get_eject_sens();
                } else if (obj->type == TYPE_150_SCORPION) {
                    eject_sens = sko_get_eject_sens();
                } else if (obj->type == TYPE_198_BB12) {
                    eject_sens = 1;
                } else if (obj->type == TYPE_200_BB13) {
                    eject_sens = -1;
                } else if (obj->type == TYPE_100_MITE) {
                    eject_sens = obj->flags.flip_x ? 1 : -1;
                } else if (obj->type == TYPE_120_BATTEUR_FOU) {
                    eject_sens = bat_get_eject_sens(obj);
                } else if (obj->type == TYPE_187_MAMA_PIRATE) {
                    eject_sens = pma_get_eject_sens(obj);
                } else if (obj->type == TYPE_209_FIRE_LEFT) {
                    eject_sens = 1;
                } else if (obj->type == TYPE_210_FIRE_RIGHT) {
                    eject_sens = -1;
                } else {
                    /* DOS 1:1de27..1de5b: sign of the promoted difference; zero falls back to facing. */
                    s32 xspeed_delta = (s32)obj->speed_x - ray.speed_x;
                    eject_sens = sgn(xspeed_delta);
                    if (eject_sens == 0) {
                        eject_sens = ray.flags.flip_x ? -1 : 1;
                    }
                }
                if (flags[obj->type].raw & flags1_4_fast_bump) {
                    bump_speed = 5;
                } else if (obj->type == TYPE_180_SAXO2) {
                    bump_speed = 4;
                } else {
                    bump_speed = 2;
                }
                if (eject_sens == 1) {
                    ray.speed_x = bump_speed;
                } else if (eject_sens == -1) {
                    ray.speed_x = bump_speed;
                    ray.speed_x = -ray.speed_x;
                }
                ray.speed_y = bump_speed + 1;
                ray.speed_y = -ray.speed_y;
            }

            in_air_because_hit = true;
            jump_time = 0;
            ray.follow_id = -1;
            decalage_en_cours = 0;
            helico_time = -1;
            ray.nb_cmd = 0;
            ray.gravity_value_1 = 0;
            ray.gravity_value_2 = 0;
            poing.is_charging = false;
            if (RayEvts.super_helico) {
                button_released = 1;
            }
            Reset_air_speed(false);
        }
    }
}

//2DF34
void standard_frontZone(obj_t* obj, s16* x, s16* w) {
    /* 1FA54 80144254 -O2 -msoft-float */
    if (!obj->flags.flip_x)
        *x -= obj->detect_zone;
    else
        *x += (*w >> 1);
    *w = (*w >> 1) + obj->detect_zone;
}

//2DF88
void SET_DETECT_ZONE_FLAG(obj_t* obj) {
    /* 1FAB4 801442B4 -O2 -msoft-float */

    s16 obj_w; s16 obj_y; s16 obj_h; s16 obj_x;
    u8 det_zn_flg;

    GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);
    switch (obj->type)
    {
        case TYPE_FEE:
            obj_x += (obj_h >> 1) - (obj->detect_zone >> 1);
            obj_y -= 16;
            obj_w = obj->detect_zone;
            obj_h += 64;
            break;
        case TYPE_MST_SHAKY_FRUIT:
            obj_y += obj_h;
            obj_h += 200;
            break;
        case TYPE_PIRATE_GUETTEUR:
        case TYPE_PIRATE_GUETTEUR2:
            standard_frontZone(obj, &obj_x, &obj_w);
            if (obj->main_etat == 0 && obj->sub_etat == 4)
            {
                obj_y += (obj_h >> 1);
                obj_h += (obj->detect_zone << 2) + (obj_h >> 1);
            }
            break;
        case TYPE_BLACKTOON1:
            switch (obj->follow_sprite)
            {
                case 3:
                    if (obj->main_etat == 0 && obj->sub_etat == 0)
                    {
                        obj_h += obj->detect_zone;
                        obj_y -= obj->detect_zone;
                        obj_w += obj->detect_zone;
                        obj_x -= (obj->detect_zone >> 1);
                    }
                    break;
                case 7:
                    if (obj->main_etat == 1 && obj->sub_etat == 1)
                    {
                        obj_x += (obj_w >> 1) - 1;
                        obj_y += (obj_h >> 1);
                        obj_w = 20;
                        obj_h += obj->detect_zone;
                        if (!obj->flags.flip_x)
                            obj_x -= obj_w;
                    }
                    break;
                case 4:
                    obj_y -= obj->detect_zone + obj_h;
                    obj_x -= (obj_w >> 1);
                    obj_w += obj_w;
                    obj_h += obj->detect_zone;
                    break;
                case 2:
                    obj_y -= obj->detect_zone + obj_h;
                    obj_h = obj->detect_zone + obj_h;
                    obj_w += obj->detect_zone * 2;
                    obj_x -= obj->detect_zone;
                    break;
            }
            break;
        case TYPE_POI3:
            obj_x += (obj->flags.flip_x) ? obj_w : -obj_w;
            obj_y -= 50;
            obj_h += 50;
            break;
        case TYPE_MST_SCROLL:
            if (obj->hit_points != 0)
            {
                obj_y = 0;
                obj_h = mp.height << 4;
            }
            break;
        case TYPE_SCROLL_SAX:
        case TYPE_BB1_VIT:
            obj_x = obj->x + obj->offset_bx;
            obj_w = 16;
            obj_y = 0;
            obj_h = mp.height << 4;
            break;
        case TYPE_WAT_CLOWN:
            obj_h += (obj->detect_zone >> 1);
            obj_y -= (obj->detect_zone >> 1);
            standard_frontZone(obj, &obj_x, &obj_w);
            break;
        case TYPE_CHASSEUR2:
            obj_h = obj->detect_zone;
            obj_y -= (obj->detect_zone >> 1);
            obj_w += obj->detect_zone * 2;
            obj_x -= obj->detect_zone;
            break;
        case TYPE_WIZARD1:
        case TYPE_CHASSEUR1:
        case TYPE_GENEBADGUY:
        case TYPE_CYMBALE:
        case TYPE_CYMBAL1:
        case TYPE_CYMBAL2:
            obj_w += obj->detect_zone * 2;
            obj_x -= obj->detect_zone;
            break;
        case TYPE_CLOWN_TNT:
            if (!obj->flags.flip_x)
                obj_x += (obj_w >> 1) - (obj->detect_zone >> 1);
            else
                obj_x += (obj_w >> 1);

            obj_w = (obj->detect_zone >> 1);
            obj_y += (obj_h >> 1);
            obj_h = (obj_h >> 1) + obj->detect_zone;
            break;
        case TYPE_STONEWOMAN:
        case TYPE_STONEWOMAN2:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = (obj_h << 1);
            break;
        case TYPE_MITE:
            if (!obj->flags.flip_x)
                obj_x -= obj->detect_zone;
            else
                obj_x += (obj_w >> 1) - 150;

            obj_w = obj->detect_zone + 150;
            obj_y -= ((obj->detect_zone - obj_h) >> 1);
            obj_h = obj->detect_zone;
            break;
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_P_45:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 150;
            break;
        case TYPE_PIRATE_POELLE_D:
        case TYPE_PIRATE_P_D_45:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 150;
            obj_x += 70;
            obj_y += 50;
            break;
        case TYPE_SPIDER_PLAFOND:
            if (
                    (
                            obj->main_etat == 0 &&
                            (obj->sub_etat == 24 || obj->sub_etat == 30 || obj->sub_etat == 11)
                    ) ||
                    (obj->main_etat == 1 && obj->sub_etat == 2)
                    )
            {
                obj_h = 10;
                obj_x -= 40;
                obj_w += 80;
            }
            else
            {
                obj_h = 200;
                obj_x -= 130;
                obj_w += 260;
            }
            break;
        case TYPE_DARK:
            standard_frontZone(obj, &obj_x, &obj_w);
            obj_h = 250;
            break;
        case TYPE_JOE:
            obj_h += 20;
            break;
        case TYPE_PHOTOGRAPHE:
            break;
        default:
            standard_frontZone(obj, &obj_x, &obj_w);
            break;
    }

    if (inter_box(obj_x, obj_y, obj_w, obj_h, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y))
    {
        det_zn_flg = obj->detect_zone_flag;
        if (det_zn_flg < 2)
        {
            if (det_zn_flg == 0)
                obj->detect_zone_flag = 1;
            else if (det_zn_flg == 1)
                obj->detect_zone_flag = 2;
        }
        else
        {
            obj->detect_zone_flag++;
            if (obj->detect_zone_flag == 20)
                obj->detect_zone_flag = 2;
        }
    }
    else
    {
        obj->detect_zone_flag = 0;
        if (obj->type == TYPE_PHOTOGRAPHE)
            obj->flags.hurtbyfist = 0;
    }
}

//2E738
void goToRay(obj_t* obj) {
    /* 201D8 801449D8 -O2 -msoft-float */
    u8 follow_sprite;

    PlaySnd(204, obj->id); // spawn sound
    switch (obj->type)
    {
        case TYPE_PLATFORM:
        case TYPE_FALLPLAT:
        case TYPE_LIFTPLAT:
        case TYPE_MOVE_PLAT:
        case TYPE_INST_PLAT:
        case TYPE_CRUMBLE_PLAT:
        case TYPE_BOING_PLAT:
        case TYPE_ONOFF_PLAT:
        case TYPE_AUTOJUMP_PLAT:
        case TYPE_OUYE:
        case TYPE_SIGNPOST:
        case TYPE_MOVE_OUYE:
        case TYPE_STONEBOMB2:
        case TYPE_CAGE:
        case TYPE_MOVE_START_NUA:
        case TYPE_BIG_BOING_PLAT:
        case TYPE_STONEBOMB3:
        case TYPE_JAUGEUP:
        case TYPE_MORNINGSTAR_MOUNTAI:
        case TYPE_MARTEAU:
        case TYPE_MOVE_MARTEAU:
        case TYPE_GROSPIC:
        case TYPE_PI:
        case TYPE_ONEUP:
        case TYPE_KILLING_EYES:
        case TYPE_RUBIS:
        case TYPE_MARK_AUTOJUMP_PLAT:
            obj->flags.flip_x = 0;
            break;
        case TYPE_STALAG:
            if (obj->sub_etat == 4)
                calc_obj_dir(obj);
            else
                obj->flags.flip_x = 0;
            break;
        case TYPE_30_AUDIOSTART:
            obj->timer = 1;
            break;
        case TYPE_BADGUY1:
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
            calc_obj_dir(obj);
            if (obj->flags.flip_x)
                skipToLabel(obj, 3, true);
            else
                skipToLabel(obj, 2, true);
            break;
        case TYPE_BLACKTOON1:
            follow_sprite = obj->follow_sprite;
            if (follow_sprite < 2 || (follow_sprite > 3 && (follow_sprite < 6 || follow_sprite > 7))) {
                calc_obj_dir(obj);
                if (obj->flags.flip_x)
                    skipToLabel(obj, 3, true);
                else
                    skipToLabel(obj, 2, true);
            }
            break;
        case TYPE_BIG_CLOWN:
            calc_obj_dir(obj);
            skipToLabel(obj, 7, true);
            break;
        default:
            calc_obj_dir(obj);
            break;
    }
}

//2E998
void unleashMonsterHost(obj_t* obj) {
    /* 20304 80144B04 -O2 -msoft-float */
    obj_t* linked_obj;
    s16 prev_id;
    s16 linked_id;
    s16 first_id = obj->link;
    linked_id = first_id;

    if (first_id != obj->id) {
        do {
            linked_obj = &level.objects[linked_id];
            if (!((flags[linked_obj->type].raw & flags1_2_is_collectible) && (bonus_taken(linked_obj->id)))) {
                obj_init(linked_obj);
                linked_obj->flags.alive = 1;
                make_active(linked_obj, true);
                goToRay(linked_obj);
            }
            prev_id = linked_id;
            linked_id = link_init[linked_obj->id];
            suppressFromLinkList(linked_obj);
        } while (prev_id != linked_id);
    }
}

//2EA2C
void SHOW_COLLISIONS_ZONES(void) {
    s16 i;
    obj_t *obj;
    i = 0;
    obj = &level.objects[actobj.objects[i]];
    while (i < actobj.num_active_objects) {
        if (!(flags[obj->type].raw & flags0_4_no_collision) && (get_eta(obj)->flags & 0x20)) {
            CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y, obj);
        }
        ++i;
        obj = &level.objects[actobj.objects[i]];
    }
}

//2EAE8
void DO_COLLISIONS(void) {
    s16 i;
    obj_t *obj;
    i = 0;
    obj = level.objects + actobj.objects[i];
    while (i < actobj.num_active_objects) {
        if (new_world || new_level || fin_boss || boss_mort) {
            break;
        }

        if (get_eta(obj)->flags & 0x20)  {
            if (!(flags[obj->type].raw & flags2_0x10_do_not_check_ray_collision)) {
                if (ray.config == 0 && ray.iframes_timer == -1) {
                if ((get_eta(&ray)->flags & 8) && !(flags[obj->type].raw & flags0_4_no_collision)) {
                    s16 collision;
                    if (obj->type == TYPE_170_RAYON && (obj->anim_frame > 3 || obj->anim_frame == 0)) {
                        collision = -1;
                    } else if (obj->type == TYPE_147_MST_SCROLL && obj->hit_points != 0) {
                        collision = -1;
                    } else {
                        collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y, obj);
                    }
                    if (collision != -1) {
                        ObjectsFonctions[obj->type].rayman_collision(obj);
                        if ((flags[obj->type].raw & flags0_8_hurts_rayman) && ray.iframes_timer == -1 &&
                                !(ray.main_etat == 3 && ray.sub_etat  == 32)
                        ) {
                            RAY_HIT(1, obj);
                            break;
                        }
                    }
                }
                }
            } else {
                if (!(ray.main_etat == 3 && ray.sub_etat == 32)) {
                    s16 collision;
                    if (obj->type  == TYPE_81_CYMBALE || obj->type == TYPE_169_CYMBAL2) {
                        collision = 0;
                    } else {
                        collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y, obj);
                    }
                    if (collision != -1) {
                        ObjectsFonctions[obj->type].rayman_collision(obj);
                    }
                }
            }
        }
        ++i;
        obj = level.objects + actobj.objects[i];
    }
    if (ray.config != 0) {
        RAY_KO();
    }
    if (ray.iframes_timer > -1) {
        --ray.iframes_timer;
    }
}

//2ED74
void DoRaymanCollisionDefault(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in PS1_DoRaymanCollision(). Seems to be functionally identical.
    if ((flags[obj->type].raw & flags2_0x10_do_not_check_ray_collision) && get_eta(obj)->flags & 0x20) {
        if (((ray.main_etat != 3 || ray.sub_etat != 32)) != 0) {
            s16 ray_iframes = ray.iframes_timer;
            if (ray_iframes >= 60) {
                RAY_HIT(false, obj);
                ray.iframes_timer = ray_iframes;
            } else if (ray_iframes == -1) {
                RAY_HIT(true, obj);
                if (!(ray_mode == MODE_3_MORT_DE_RAYMAN || ray_mode == MODE_4_MORT_DE_RAYMAN_ON_MS)) {
                    ray.iframes_timer = 60;
                }
            } else {
                RAY_HIT(false, obj);
                ray.iframes_timer = ray_iframes;
            }
        }
    }
}

//2EE20
void DO_OBJ_COLLISIONS(obj_t* obj, s16 offs) {
    /* 218C0 801460C0 -O2 -msoft-float */
    s16 in_x; s16 in_y; s16 in_w; s16 in_h;
    u8 done;
    s16 i;
    obj_t* cur_obj;
    s16 cur_x; s16 cur_y; s16 cur_w; s16 cur_h;

    done = false;
    GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);
    if (obj->type == TYPE_FALLING_OBJ || obj->type == TYPE_FALLING_OBJ2 || obj->type == TYPE_FALLING_OBJ3)
    {
        in_x += 10;
        in_y += 20;
        in_w += -20;
        in_h += -25;
    }
    else
    {
        in_x = offs + in_x;
        in_y = offs + in_y;
        in_w -= offs * 2;
        in_h -= offs * 2;
    }
    i = 0;
    cur_obj = &level.objects[actobj.objects[i]];
    if (i < actobj.num_active_objects)
    {
        do
        {
            if (obj != cur_obj)
            {
                switch (cur_obj->type)
                {
                    case TYPE_BADGUY1:
                        if (
                                cur_obj->main_etat != 2 && ((cur_obj->main_etat != 0 || cur_obj->sub_etat != 3) != 0) &&
                                (!(cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].flags & 0x40) &&
                                 GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) &&
                                 inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h)
                                )
                                )
                        {
                            set_main_and_sub_etat(cur_obj, 0, 4);
                            cur_obj->follow_sprite = 4;
                            cur_obj->follow_y = 0;
                            cur_obj->speed_x = 0;
                            cur_obj->speed_y = 0;
                            cur_obj->offset_hy = 11;
                            cur_obj->flags.follow_enabled = 1;
                            if (ray.follow_id == obj->id)
                                RAY_TOMBE();
                            if (cur_obj->cmd == GO_WAIT)
                            {
                                if ((cur_obj->flags.flip_x))
                                    skipToLabel(cur_obj, 3, true);
                                else
                                    skipToLabel(cur_obj, 2, true);
                            }
                            set_sub_etat(obj, 9);
                            obj->is_active = 0;
                            done = true;
                        }
                        break;
                    case TYPE_BOUEE_JOE:
                        if (
                                GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) &&
                                obj->type == TYPE_POI2 &&
                                inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h) &&
                                !(cur_obj->main_etat == 2 && cur_obj->sub_etat == 6)
                                )
                        {
                            /* sgn() on android */
                            if (
                                    obj->flags.flip_x &&
                                    sgn((cur_obj->x + cur_obj->offset_bx) - obj->x - obj->offset_bx) >= 0
                                    )
                            {
                                cur_obj->config = 1;
                                skipToLabel(obj, 3, true);
                                cur_obj->speed_x = 8;
                            }
                            if (
                                    !(obj->flags.flip_x) &&
                                    sgn((cur_obj->x + cur_obj->offset_bx) - obj->x - obj->offset_bx) < 0
                                    )
                            {
                                cur_obj->config = 1;
                                skipToLabel(obj, 4, true);
                                cur_obj->speed_x = -8;
                            }
                        }
                        break;
                }

                if (done)
                    break;
            }
            i++;
            cur_obj = &level.objects[actobj.objects[i]];
        } while (i < actobj.num_active_objects);
    }
}
