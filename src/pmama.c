/*
 * Space Mama boss and knives (GOG v1.21 object 1:543f0-1:56460). File name invented (Rayverse pmama.c).
 * Worker candidate; measurements and unresolved differences: NOTES.md.
 * Function/global names: Rayverse/Android. Descriptive added locals: INVENTED.
 */
#define PC 1
#include <string.h>
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"

#ifndef true
#define true 1
#define false 0
#endif
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define Abs(value) ((s32)(value) < 0 ? -(s32)(value) : (s32)(value))
#define ashl16(value, amount) ((s32)(s16)(value) << (amount))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define TYPE_COUTEAU 190

/* DOS records occupy 20 bytes (Android CouteauxInfos size 100; DOS index scale 0x14).
   Field spellings/first 18 bytes follow Rayverse types.h until byte evidence refines them. */
typedef struct couteaux_info_t {
    s16 x_pos;
    s16 y_pos;
    s16 field2_0x4;
    s16 field3_0x6;
    s16 field4_0x8;
    s16 field5_0xa;
    s16 field6_0xc;
    s16 field7_0xe;
    u8 record_flag;  /* DOS +0x10: saved animation frame (placeholder name INVENTED). */
    u8 active;
    u8 field9_0x12;
    u8 dos_tail;
} couteaux_info_t;

extern level_t level;
extern obj_t ray;
extern couteaux_info_t CouteauxInfos[5];
extern u8 pma_couteaux[6];
extern u8 pma_sequence[4][2];
extern u8 place_sequence[5];
extern s16 bateau_obj_id;
extern s16 mama_pirate_obj_id;
extern s16 cou_tempo;
extern s16 pma_tempo;
extern u8 pma_touched;
extern u8 pma_nb_couteau;
extern u8 pma_phase;
extern u8 pma_attaque;
extern u8 cou_place;
extern u8 pma_groupe;
extern u8 pma_type_attaque;
extern s16 xmap;
extern s16 ymap;
extern s16 xmapmax;
extern s16 ymapmax;
extern s16 screen_trembling;
extern s16 scroll_start_x;
extern s16 scroll_end_x;
extern s16 scroll_start_y;
extern s16 scroll_end_y;
extern u8 scrollLocked;
extern u8 horloge[25];
typedef union pma_block_flags_t {
    u8 raw;
    struct { u8 unused0:1; u8 solid:1; u8 unused1:6; } bits;
} pma_block_flags_t;
extern pma_block_flags_t block_flags[64];
extern u8 fin_boss;
extern u8 boss_mort;
extern finbosslevel_t finBosslevel;

void snapToSprite(obj_t *obj, obj_t *target, u8 sprite, s32 x_offs, s32 y_offs);
u8 BTYP(s16 tile_x, s16 tile_y);
s16 dist_to_bloc_floor(s16 btyp, s16 x, s32 y);
s16 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void add_alwobj(obj_t *obj);
void set_main_and_sub_etat(obj_t *obj, u8 main_etat, u8 sub_etat);
void PlaySnd(s16 sound, s16 obj_id);
void CALC_MOV_ON_BLOC(obj_t *obj);
u8 EOA(obj_t *obj);
void recale_position(obj_t *obj);
void allocateLandingSmoke(obj_t *obj);

//643F0
void pmamaFollowsShip(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (bateau_obj_id != -1)
        snapToSprite(obj, &level.objects[bateau_obj_id], 6, -165, -94);
    else
    {
        obj->speed_x = 0;
        obj->speed_y = 0;
    }
}

//64440
void init_couteaux(void) {
    // DOS: the object count and scan index are words; object pointer advances by one 0x84-byte record.
    u8 cout_ind = 0;
    s16 i = 0;
    obj_t *obj = level.objects;
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cout_ind >= 5)
            return;
        if (obj->type == TYPE_COUTEAU) {
            CouteauxInfos[cout_ind].x_pos = 0;
            CouteauxInfos[cout_ind].y_pos = 0;
            CouteauxInfos[cout_ind].record_flag = 0;
            CouteauxInfos[cout_ind].active = false;
            CouteauxInfos[cout_ind].field7_0xe = i;
            CouteauxInfos[cout_ind].field2_0x4 = 1;
            CouteauxInfos[cout_ind].field3_0x6 = 1;
            CouteauxInfos[cout_ind].field9_0x12 = 5;
            CouteauxInfos[cout_ind].field4_0x8 = 15;
            CouteauxInfos[cout_ind].field5_0xa = 0;
            CouteauxInfos[cout_ind].field6_0xc = 0;
            obj->flags.alive = 0;
            obj->is_active = 0;
            obj->param = cout_ind;
            cout_ind++;
        }
        obj++;
        i++;
    }
}

//64500
void reset_couteaux(void) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 i;
    for (i = 0; i < LEN(CouteauxInfos); i++) {
        CouteauxInfos[i].active = false;
    }
}

//6451C
u8 check_couteaux(void) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 nb_active;
    u8 i;

    nb_active = 0;
    for (i = 0; i < LEN(CouteauxInfos); i++) {
        if (CouteauxInfos[i].active == true) {
            nb_active++;
        }
    }

    return nb_active == pma_couteaux[pma_sequence[pma_groupe][pma_attaque]];
}

//64568
u8 find_couteau(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 bVar1 = (u8)obj->param;

    if (bVar1 >= 5) {
        bVar1 = -1;
    }
    return bVar1;
}

//64574
s16 x_pos(u8 param_1, u8 nb_couteau) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 x;

    if (nb_couteau == 0) {
        x = 0;
    } else {
        x = (240 / (nb_couteau * 2)) * (param_1 * 2 + 1);
        if (level.objects[mama_pirate_obj_id].flags.flip_x)
            x += 80;
    }
    return x;
}

//645CC
s16 y_pos(s16 param_1, s16 param_2) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 y;

    if (param_2 == 0)
        y = 44;
    else
        y = param_1 * 16 + 44;
    return y;
}

//645E0
s16 y_floor(s16 x, s16 y) {
    u8 btyp;
    s16 tx = (s16)x >> 4;
    s32 ty = (s16)y >> 4;
    btyp = BTYP((s16)tx, (s16)ty);
    while (!block_flags[btyp].bits.solid) {
        y += 16;
        btyp = BTYP((s16)(x >> 4), (s16)((s16)y >> 4));
    }
    return (y & ~0xf) + dist_to_bloc_floor(btyp, x & 0xf, 0);
}

//64650
void init_move_couteau(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical at first glance.
    u8 cout_ind;
    s16 x;
    s16 y;
    u8 test_1;

    cout_ind = (u8)find_couteau(obj);
    if (cout_ind != (u8)-1) {
        obj_t cout_obj = level.objects[CouteauxInfos[cout_ind].field7_0xe];
        x = cout_obj.x + cout_obj.offset_bx;
        y = cout_obj.y + cout_obj.offset_by;
        CouteauxInfos[cout_ind].active = false;

        switch (pma_type_attaque) {
            case 0:
            case 1:
            case 2:
            case 5:
                CouteauxInfos[cout_ind].x_pos = x;
                CouteauxInfos[cout_ind].y_pos = y;
                CouteauxInfos[cout_ind].field9_0x12 = 2;
                CouteauxInfos[cout_ind].field2_0x4 = 1;
                CouteauxInfos[cout_ind].field3_0x6 = 1;
                break;
            case 3:
            case 4:
                if (pma_type_attaque == 3) test_1 = 4; else test_1 = 0;
                if (cout_ind != 1)
                    test_1 = 4 - test_1;
                CouteauxInfos[cout_ind].x_pos = x_pos(test_1, 6);
#ifdef PC
                CouteauxInfos[cout_ind].y_pos = y_pos(9 - cout_ind, 1) + 8;
#else
                CouteauxInfos[cout_ind].y_pos = y_pos(11 - cout_ind, 1) + 8;
#endif
                CouteauxInfos[cout_ind].field9_0x12 = 0;
                if (cout_ind == 0)
                    CouteauxInfos[cout_ind].field2_0x4 = 50;
                else
                    CouteauxInfos[cout_ind].field2_0x4 = 50;
                CouteauxInfos[cout_ind].field3_0x6 = 70;
                break;
        }
    }
}

//647CC
void init_lance_couteau(u8 index) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical at first glance.
    obj_t *obj;
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;

    if (index < pma_nb_couteau && index < LEN(CouteauxInfos)) {
        obj = &level.objects[CouteauxInfos[index].field7_0xe];
        if (!obj->is_active) {
            obj->display_prio = 0;
            obj->flags.alive = 1;
            obj->is_active = 1;
#ifdef PC
            add_alwobj(obj);
#endif
            CouteauxInfos[index].active = false;
            if (pma_type_attaque == 0 || pma_type_attaque == 3 || pma_type_attaque == 4) {
                set_main_and_sub_etat(obj, 2, 10);
                obj->anim_index = obj->eta[2][10].anim_index;
                CouteauxInfos[index].field9_0x12 = 4;
                CouteauxInfos[index].x_pos = x_pos(index, pma_nb_couteau);
                CouteauxInfos[index].y_pos =
                        level.objects[mama_pirate_obj_id].offset_by +
                        level.objects[mama_pirate_obj_id].screen_y -
                        obj->offset_by;
                CouteauxInfos[index].field2_0x4 = 1;
            } else {
                set_main_and_sub_etat(obj, 2, 11);
                obj->anim_index = obj->eta[2][11].anim_index;
                if (pma_type_attaque == 5) {
                    CouteauxInfos[index].x_pos = x_pos(index, pma_nb_couteau);
                    CouteauxInfos[index].y_pos = y_pos(0, 0);
                    CouteauxInfos[index].field9_0x12 = 1;
                    CouteauxInfos[index].field2_0x4 = cou_tempo;
                } else {
                    if (index >= place_sequence[cou_place]) {
                        CouteauxInfos[index].x_pos = x_pos(index + 1, pma_nb_couteau + 1);
                    } else {
                        CouteauxInfos[index].x_pos = x_pos(index, pma_nb_couteau + 1);
                    }

                    CouteauxInfos[index].x_pos -= index * 4;
                    CouteauxInfos[index].y_pos = y_pos(0, 0);
                    CouteauxInfos[index].field9_0x12 = 3;
                    if (index == 0) {
                        CouteauxInfos[index].field2_0x4 = cou_tempo;
                    } else {
                        CouteauxInfos[index].field2_0x4 = 1;
                    }
                }
            }

            obj->anim_frame = 0;
            GET_SPRITE_POS(&level.objects[mama_pirate_obj_id], 6, &spr_x, &spr_y, &spr_w, &spr_h);
            if (level.objects[mama_pirate_obj_id].flags.flip_x) {
                spr_x += spr_w;
            }
            obj->x = spr_x - obj->offset_bx;
            obj->y = spr_y - obj->offset_by;
            obj->init_x = obj->x;
            obj->init_y = obj->y;
        }
    }
}

//64A80
u8 couteau_frame(s16 spd_x, s16 spd_y) {
    s32 frame;
    s16 diff;
    s32 value;
    if (spd_x == 0) {
        if (spd_y >= 0) frame = 0; else frame = 8;
    } else if (spd_y == 0) {
        if (spd_x == 0) frame = 0;
        else if (spd_x > 0) frame = 4; else frame = 12;
    } else {
        value = spd_x;
        if (value < 0) value = -value;
        diff = value;
        value = spd_y;
        if (value < 0) value = -value;
        diff -= value;
        if (diff == 0) {
            frame = 6;
        } else if (diff >= 0) {
            frame = 5;
        } else {
            frame = 7;
        }

        if (spd_x > 0) {
            if (spd_y > 0)
                return 8 - frame;
        } else {
            if (spd_y < 0)
                return 16 - frame;
            else
                return 8 + frame;
        }
    }
    return frame;
}

//64B14
void update_couteau(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    obj->anim_frame = couteau_frame(obj->speed_x, obj->speed_y);
}

//64B70
void get_cou_zdc(obj_t* obj, s16 *x, s16 *y, s16 *w, s16 *h) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical at first glance.
    s16 spr_x; s16 spr_w; s16 spr_y; s16 spr_h;

    GET_SPRITE_POS(obj, 0, &spr_x, &spr_y, &spr_w, &spr_h);
    *w = 6;
    *h = 6;
    if (obj->main_etat == 2 && (obj->sub_etat == 10 || obj->sub_etat == 11)) {
        switch (obj->anim_frame) {
            case 0:
                *x = spr_x + ((spr_w - *w) >> 1);
                *y = spr_y + spr_h - *h;
                return;
            case 1:
            case 2:
            case 3:
                *x = spr_x + spr_w - *w;
                *y = spr_y + spr_h - *h;
                return;
            case 4:
                *w = spr_w;
                *x = spr_x + spr_w - *w;
                *y = spr_y + ((spr_h - *h) >> 1);
                return;
            case 5:
            case 6:
            case 7:
                *x = spr_x + spr_w - *w;
                *y = spr_y;
                return;
            case 9:
            case 10:
            case 11:
                *x = spr_x;
                *y = spr_y;
                return;
            case 12:
                *w = spr_w;
                *x = spr_x;
                *y = spr_y + ((spr_h - *h) >> 1);
                return;
            case 13:
            case 14:
            case 15:
                *x = spr_x;
                *y = spr_y + spr_h - *h;
                return;
            default:
                *x = spr_x + ((spr_w - *w) >> 1);
                *y = spr_y + spr_h - *h;
                return;
            case 8:
                *x = spr_x + ((spr_w - *w) >> 1);
                *y = spr_y;
                return;
        }
    } else if (obj->main_etat == 2 && obj->sub_etat == 14 && !(obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf)) {
        *h = spr_h;
        *x = spr_x + ((spr_w - *w) >> 1);
        *y = spr_y;
        return;
    } else if ((obj->main_etat == 2 && obj->sub_etat == 13) ||
               (obj->main_etat == 2 && obj->sub_etat == 14 &&
                (obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf))) {
        *x = obj->x + 128 - (*w >> 1);
        *y = obj->y + 96 - (*h >> 1);
        return;
    } else if (obj->main_etat == 2 && obj->sub_etat == 12) {
        *x = spr_x + ((spr_w - *w) >> 1);
        *y = spr_y + spr_h - *h;
        return;
    }
    *x = -*w;
    *y = -*h;
}

//64EA4
void pma_attaque_suivante(void) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 i;

    if ((pma_type_attaque == 1 || pma_type_attaque == 2) && ++cou_place >= 5) {
        cou_place = 0;
    }

    if (!pma_touched) {
        if (++pma_attaque == 2) {
            pma_attaque = 0;
            pma_groupe++;
            if (pma_groupe == 4) {
                pma_groupe = 0;
                pma_tempo -= 10;
                cou_tempo -= 10;
            }
        }
    } else {
        pma_groupe++;
        pma_attaque = 0;
        if (pma_groupe == 4) {
            pma_groupe = 0;
            pma_tempo -= 10;
            cou_tempo -= 10;
        }
    }
    if (pma_tempo < 100) {
        pma_tempo = 100;
    }

    if (cou_tempo < 50)
        cou_tempo = 50;
    pma_phase = 0;
    pma_type_attaque = pma_sequence[pma_groupe][pma_attaque];
    pma_nb_couteau = pma_couteaux[pma_type_attaque];
    pma_touched = false;

    for (i = 0; i < LEN(CouteauxInfos); i++) {
        level.objects[CouteauxInfos[i].field7_0xe].is_active = 0;
        level.objects[CouteauxInfos[i].field7_0xe].flags.alive = 0;
    }
}

//64FF8
s16 convertspeed(s16 x) {
    s32 result = ashl16(x, 4);
    return result;
}

//65000
void lance_couteau_parabolique(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 diff_x; s16 diff_y;
    s16 spd_x; s16 spd_y;
    s32 motion;
    u8 cout_ind = find_couteau(obj);

    if (cout_ind != 0xFF) {
        diff_x = (xmap + CouteauxInfos[cout_ind].x_pos) - (obj->x + obj->offset_bx);
        diff_y = (ymap + CouteauxInfos[cout_ind].y_pos) - (obj->y + obj->offset_by);
        if (CouteauxInfos[cout_ind].field2_0x4) {
            CouteauxInfos[cout_ind].field2_0x4 = 0;
            if (diff_x == 0) {
                obj->speed_x = convertspeed(0);
                obj->speed_y = convertspeed(-1);
            } else {
                if (diff_x >= 0)
                    spd_x = 2;
                else
                    spd_x = -2;
                spd_y =
                    -((((diff_y << 4) * spd_x) << 3) +
                            diff_x * (diff_x << 3)
                    ) /
                    ((diff_x * spd_x) << 3);
                obj->speed_x = convertspeed(spd_x) - 10;
                obj->speed_y = spd_y;
            }
            update_couteau(obj);
            obj->display_prio = 3;
        } else {
            motion = diff_x * obj->speed_x;
            obj->speed_y += 4;
            if (motion < 0) {
                obj->speed_x = 0;
            }
            update_couteau(obj);
        }
    }
}

//65120
void lance_couteau_droit(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 cout_ind;
    s16 diff_x;
    s16 diff_y;
    s32 abs_x;
    s32 abs_y;
    s32 divisor;

    cout_ind = find_couteau(obj);
    if (cout_ind != 0xFF) {
        diff_x = (xmap + CouteauxInfos[cout_ind].x_pos) - (obj->offset_bx + obj->x);
        diff_y = (ymap + CouteauxInfos[cout_ind].y_pos) - (obj->offset_by + obj->y);
        if (Abs(diff_x) >= 4 || Abs(diff_y) >= 4) {
            CouteauxInfos[cout_ind].active = false;
            abs_x = Abs(diff_x);
            abs_y = Abs(diff_y);
            abs_x = MAX(abs_x, abs_y);
            divisor = 4;
            divisor = ashl16(abs_x, 4) / divisor;
            obj->speed_x = convertspeed(ashl16(diff_x, 4) / (s16) divisor);
            obj->speed_y = convertspeed(ashl16(diff_y, 4) / (s16) divisor);
            update_couteau(obj);
            obj->display_prio = 3;
        } else {
            CouteauxInfos[cout_ind].active = true;
            if ((u32) (pma_type_attaque - 1) >= 2) {
                if (obj->main_etat != 2 || obj->sub_etat != 13) {
                    obj->main_etat = 2;
                    obj->sub_etat = 13;
                }
            } else {
                obj->anim_frame = couteau_frame(0, 1);
            }
            obj->speed_x = 0;
            obj->speed_y = 0;
            obj->x = (xmap + CouteauxInfos[cout_ind].x_pos) - obj->offset_bx;
            obj->y = (ymap + CouteauxInfos[cout_ind].y_pos) - obj->offset_by;
            if (check_couteaux()) {
                pma_phase = 3;
                reset_couteaux();
            }
        }
    }
}

//652EC
void retour_couteau(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 cout_ind;
    s16 diff_x;
    s16 diff_y;
    s32 abs_x;
    s32 abs_y;
    s32 divisor;

    cout_ind = find_couteau(obj);
    if (cout_ind != 0xFF && (cout_ind == 0 || (cout_ind != 0 && CouteauxInfos[cout_ind - 1].field3_0x6 == 0))) {
        if (CouteauxInfos[cout_ind].field3_0x6 == 0) {
            diff_x = 0;
            diff_y = 0;
            if (CouteauxInfos[cout_ind].field9_0x12 != 2) {
                diff_x = (xmap + CouteauxInfos[cout_ind].x_pos) - (obj->offset_bx + obj->x);
                diff_y = (ymap + CouteauxInfos[cout_ind].y_pos) - (obj->offset_by + obj->y);
            }

            if (Abs(diff_x) >= 2 || Abs(diff_y) >= 2) {
                CouteauxInfos[cout_ind].active = false;
                abs_x = Abs(diff_x);
                abs_y = Abs(diff_y);
                if (abs_x < abs_y)
                    abs_x = abs_y;
                divisor = 2;
                divisor = ashl16(abs_x, 4) / divisor;
                obj->speed_x = convertspeed(ashl16(diff_x, 4) / (s16) divisor);
                obj->speed_y = convertspeed(ashl16(diff_y, 4) / (s16) divisor);
            } else {
                CouteauxInfos[cout_ind].active = true;
                obj->speed_x = convertspeed(0);
                obj->speed_y = convertspeed(0);
                if (CouteauxInfos[cout_ind].field9_0x12 != 2) {
                    obj->x = (xmap + CouteauxInfos[cout_ind].x_pos) - obj->offset_bx;
                    obj->y = (ymap + CouteauxInfos[cout_ind].y_pos) - obj->offset_by;
                }
                if (check_couteaux()) {
                    pma_phase = 3;
                    reset_couteaux();
                }
            }
        }
        else
            CouteauxInfos[cout_ind].field3_0x6--;
    }
}

//654F8
void lance_couteau_lineaire(obj_t* in_obj) {
    // NOTE(Falcury): PS1 and PC versions seem mostly identical (minor difference).
    s16 unk_1;
    obj_t *cout_obj;
    s16 cout_x; s16 cout_y;
    s16 unk_2;
    s16 diff_x; s16 diff_y;
    s16 unk_sub_etat;
    s32 unk_3;
    s16 unk_4;
    s16 unk_x_1; s16 unk_y_1;
    u8 unk_anim_frame;
    u8 unk_5;
    eta_t *unk_state_1;
    eta_t *unk_state_2;
    eta_t *unk_state_3;
    u8 cout_ind = find_couteau(in_obj);

    if (cout_ind != 0xFF && (cout_ind == 0 || (cout_ind != 0 && CouteauxInfos[cout_ind - 1].field2_0x4 == 0))) {
        unk_1 = CouteauxInfos[cout_ind].field2_0x4;
        if (unk_1 == 0) {
            if (!CouteauxInfos[cout_ind].active) {
                cout_obj = &level.objects[CouteauxInfos[cout_ind].field7_0xe];
                cout_x = cout_obj->x + cout_obj->offset_bx;
                cout_y = cout_obj->y + cout_obj->offset_by;
                if (cout_x < -20 || (cout_x > xmapmax + (SCREEN_WIDTH + 20)) ||
                    cout_y < -20 || (cout_y > ymapmax + (SCREEN_HEIGHT + 20))
                ) {
                    CouteauxInfos[cout_ind].active = true;
                    level.objects[CouteauxInfos[cout_ind].field7_0xe].is_active = 0;
                    level.objects[CouteauxInfos[cout_ind].field7_0xe].flags.alive = 0;
                    if (check_couteaux()) {
                        pma_phase = 4;
                        if (level.objects[mama_pirate_obj_id].hit_points == 0) {
                            fin_boss = true;
#ifdef PC
                            boss_mort = true;
#endif
                            finBosslevel.viking_mama = true;
                        }
                    }
                }
            }
        } else if (unk_1 == 1) {
            unk_2 = CouteauxInfos[cout_ind].field4_0x8;
            if (unk_2 == 15) {
                CouteauxInfos[cout_ind].x_pos = ray.x + ray.offset_bx;
                CouteauxInfos[cout_ind].y_pos = ray.y + ray.offset_by;
                diff_x = CouteauxInfos[cout_ind].x_pos - (in_obj->x + in_obj->offset_bx);
                diff_y = CouteauxInfos[cout_ind].y_pos - (in_obj->y + in_obj->offset_by);
                switch (CouteauxInfos[cout_ind].field9_0x12) {
                    case 1:
                        unk_sub_etat = 11;
                        unk_3 = MAX(Abs(diff_x), Abs(diff_y));
                        unk_4 = 4; /* get rid of this? */
                        unk_4 = ashl16(unk_3, 4) / unk_4;
                        unk_x_1 = convertspeed(ashl16(diff_x, 4) / unk_4);
                        unk_y_1 = convertspeed(ashl16(diff_y, 4) / unk_4);
                        unk_anim_frame = couteau_frame(unk_x_1, unk_y_1);
                        break;
                    case 3:
                        unk_sub_etat = 11;
                        unk_x_1 = convertspeed(0);
                        unk_y_1 = convertspeed(3);
                        unk_anim_frame = couteau_frame(unk_x_1, unk_y_1);
                        break;
                    case 0:
                        unk_sub_etat = 11;
                        unk_x_1 = convertspeed(3 * (diff_x >= 0 ? 1 : -1));
                        unk_y_1 = convertspeed(0);
                        unk_anim_frame = couteau_frame(unk_x_1, unk_y_1);
                        break;
                    case 2:
                        unk_sub_etat = 14;
                        unk_x_1 = convertspeed(-1);
                        unk_y_1 = convertspeed(5);
                        unk_anim_frame = couteau_frame(0, 1);
                        break;
                }

                /* duplication? */
                unk_5 = CouteauxInfos[cout_ind].field9_0x12;
                if (unk_5 == 2) {
                    in_obj->sub_etat = unk_sub_etat;
                    in_obj->anim_frame = unk_anim_frame;
                    unk_state_1 = &in_obj->eta[in_obj->main_etat][in_obj->sub_etat];
                    unk_state_1->anim_speed &= 0xF0;
                    CouteauxInfos[cout_ind].record_flag = unk_anim_frame;
                    CouteauxInfos[cout_ind].field5_0xa = unk_x_1;
                    CouteauxInfos[cout_ind].field6_0xc = unk_y_1;
                    CouteauxInfos[cout_ind].field4_0x8--;
                    in_obj->speed_x = CouteauxInfos[cout_ind].field5_0xa;
                    in_obj->speed_y = CouteauxInfos[cout_ind].field6_0xc;
                    CouteauxInfos[cout_ind].field4_0x8 = 15;
                    CouteauxInfos[cout_ind].field2_0x4 = 0;
                    CouteauxInfos[cout_ind].active = false;
                    unk_state_2 = &in_obj->eta[in_obj->main_etat][in_obj->sub_etat];
                    unk_state_2->anim_speed &= 0xF0;
                    unk_state_2->anim_speed |= 2;
                    in_obj->gravity_value_1 = 0;
                    in_obj->gravity_value_2 = 8;
                } else if (unk_5 == 3) {
                    in_obj->sub_etat = unk_sub_etat;
                    in_obj->anim_frame = unk_anim_frame;
                    CouteauxInfos[cout_ind].field5_0xa = unk_x_1;
                    CouteauxInfos[cout_ind].record_flag = unk_anim_frame;
                    CouteauxInfos[cout_ind].field6_0xc = unk_y_1;
                    CouteauxInfos[cout_ind].field4_0x8--;
                    in_obj->speed_x = CouteauxInfos[cout_ind].field5_0xa;
                    in_obj->speed_y = CouteauxInfos[cout_ind].field6_0xc;
                    CouteauxInfos[cout_ind].field4_0x8 = 15;
                    CouteauxInfos[cout_ind].field2_0x4 = 0;
                    CouteauxInfos[cout_ind].active = false;
                } else if (in_obj->anim_frame == unk_anim_frame) {
                    in_obj->sub_etat = unk_sub_etat;
                    in_obj->anim_frame = unk_anim_frame;
                    CouteauxInfos[cout_ind].record_flag = unk_anim_frame;
                    CouteauxInfos[cout_ind].field5_0xa = unk_x_1;
                    CouteauxInfos[cout_ind].field6_0xc = unk_y_1;
                    CouteauxInfos[cout_ind].field4_0x8--;
                }
            } else if (unk_2 == 0) {
                in_obj->speed_x = (s16) CouteauxInfos[cout_ind].field5_0xa;
                in_obj->speed_y = (s16) CouteauxInfos[cout_ind].field6_0xc;
                if (CouteauxInfos[cout_ind].field9_0x12 == 2) {
                    unk_state_3 = &in_obj->eta[in_obj->main_etat][in_obj->sub_etat];
                    unk_state_3->anim_speed &= 0xF0;
                    unk_state_3->anim_speed |= 2;
                    in_obj->gravity_value_1 = 0;
                    in_obj->gravity_value_2 = 8;
                }
                CouteauxInfos[cout_ind].field4_0x8 = 15;
                CouteauxInfos[cout_ind].field2_0x4 = 0;
                CouteauxInfos[cout_ind].active = 0;
            } else {
                in_obj->speed_x = 0;
                in_obj->speed_y = 0;
                in_obj->anim_frame = CouteauxInfos[cout_ind].record_flag;
                /* duplication */
                if (CouteauxInfos[cout_ind].field4_0x8 & 1)
                    in_obj->y += 2;
                else
                    in_obj->y -= 2;

                if (CouteauxInfos[cout_ind].field4_0x8 > 0)
                    CouteauxInfos[cout_ind].field4_0x8--;
            }
        } else {
            /* unk_1 is field2_0x4*/
            if (unk_1 > 1)
                CouteauxInfos[cout_ind].field2_0x4--;

            if (CouteauxInfos[cout_ind].field2_0x4 == 1)
                CouteauxInfos[cout_ind].field4_0x8 = 15;
        }
    }
}

//65B10
void DO_COU_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    switch (obj->main_etat) {
        case 0: {
            u8 sub_etat = obj->sub_etat;
            if (sub_etat == 9) {
                obj->flags.follow_enabled = 1;
                obj->speed_x = ashr16(obj->speed_x, 1);
                CALC_MOV_ON_BLOC(obj);
                if (screen_trembling == 1 || pma_phase == 2) {
                    screen_trembling = 0;
                    if (pma_phase != 2) {
                        pma_phase = 2;
                        reset_couteaux();
                    }
                    set_main_and_sub_etat(obj, 2, 12);
                }
            }
            break;
        }
        default:
            break;
        case 2:
            obj->flags.follow_enabled = 0;
            switch (obj->sub_etat) {
                case 10:
                    if (pma_phase == 0)
                        lance_couteau_parabolique(obj);
                    break;
                case 11:
                    if (pma_phase == 3)
                        lance_couteau_lineaire(obj);
                    else if (pma_phase == 0)
                        lance_couteau_droit(obj);
                    break;
                case 14:
                    if (pma_phase == 3)
                        lance_couteau_lineaire(obj);
                    else if (pma_phase == 0)
                        lance_couteau_droit(obj);

                    if (obj->speed_y >= 0 && couteau_frame(0, -1) == obj->anim_frame)
                        obj->eta[obj->main_etat][obj->sub_etat].anim_speed &= 0xF0;
                    break;
                case 13:
                    if (pma_phase == 2)
                        retour_couteau(obj);
                    else if (pma_phase == 3)
                        lance_couteau_lineaire(obj);
                    break;
                case 12:
                    obj->speed_x = convertspeed(0);
                    obj->speed_y = convertspeed(-1);
                    if (EOA(obj)) {

                            obj->speed_y = convertspeed(0);
                            if (pma_phase == 2) {
                                init_move_couteau(obj);
                                set_main_and_sub_etat(obj, 2, 13);
                            } else {
                                set_main_and_sub_etat(obj, 0, 9);
                            }
                    }
                    break;
            }
            break;
    }
}

//65D00
void calc_pma_dir(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 diff_x;

    diff_x = (ray.offset_bx + ray.x) - (obj->offset_bx + obj->x);
    if (diff_x != 0) {
        obj->flags.flip_x = (diff_x > 0) ? 1 : 0;
    }
}

//65D90
void DO_PMA_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical at first glance.
    s16 main_etat;
    s16 sub_etat;
    s16 i;
    s16 y;
    s16 x;

    scrollLocked = true;
    ymap = scroll_start_y = scroll_end_y = ymapmax;
    main_etat = obj->main_etat;
    sub_etat = obj->sub_etat;
    switch (main_etat) {
        case 0:
            switch (sub_etat) {
                case 6:
                    obj->speed_x = 1;
                    obj->speed_y = 0;
                    CALC_MOV_ON_BLOC(obj);
                    if ((obj->offset_bx + obj->x) > (xmap +
#ifdef PC
                            256
#else
                            272
#endif
                    )) {
                        obj->speed_x = 0;
                        set_main_and_sub_etat(obj, 0, 2);
                    }
                    break;
                default:
                    return;
                case 2:
                    calc_pma_dir(obj);
                    CALC_MOV_ON_BLOC(obj);
                    if (pma_phase == 4) {
                        reset_couteaux();
                        pma_attaque_suivante();
                    }
                    if (pma_phase == 0)
                        set_main_and_sub_etat(obj, 0, 1);
                    break;
                case 1:
                    CALC_MOV_ON_BLOC(obj);
                    if (EOA(obj))
                        switch (pma_phase) { case 0: set_main_and_sub_etat(obj, 0, 5); break; }
                    break;
                case 5:
                    CALC_MOV_ON_BLOC(obj);
                    if (obj->anim_frame == 35 && (horloge[obj->eta[main_etat][sub_etat].anim_speed & 0xF] == 0)) {
                        PlaySnd(153, obj->id);
                        i = 0;
                        for (; i < pma_nb_couteau && i < 5; i++) {
                            init_lance_couteau(i);
                        }
                    } else {
                        if (EOA(obj)) {
                            set_main_and_sub_etat(obj, 2, 1);
                        }
                    }
                    break;
                case 7:
                    obj->speed_x = 0;
                    obj->speed_y = -2;
                    if (EOA(obj)) {
                            obj->is_active = 0;
                            obj->flags.alive = 0;
                            for (i = 0; i < 5; i++) {
                                CouteauxInfos[i].field9_0x12 = 2;
                                CouteauxInfos[i].field2_0x4 = 1;
                                CouteauxInfos[i].active = false;
                                CouteauxInfos[i].field4_0x8 = 0xf;
                            }
                            pma_phase = 2;
                            return;
                    }
                    break;
            }
            break;
        case 2:
            switch (sub_etat) {
                case 9:
                    obj->speed_x = 0;
                    if (EOA(obj) || (obj->hit_points == 1 && obj->speed_y >= 0)) {
                        pma_touched = true;
                        obj->hit_points--;
                        if (obj->hit_points == 0)
                            set_main_and_sub_etat(obj, 0, 7);
                        else
                            set_main_and_sub_etat(obj, 2, 4);
                    }
                    break;
                case 6:
                    obj->speed_x = 0;
                    obj->flags.flip_x = 0;
                    if (obj->anim_frame < 6)
                        pmamaFollowsShip(obj);
                    else
                        obj->speed_y = -2;
                    break;
                case 1:
                    calc_pma_dir(obj);
                    obj->speed_x = 0;
                    obj->speed_y = 0;
                    break;
                case 8:
                    obj->speed_x = 0;
                    obj->speed_y = 0;
                    break;
                case 2:
                    obj->speed_y = -2;
                    if (obj->anim_frame >= 15)
                        obj->speed_y = 0;
                    obj->param = 0;
                    return;
                case 3:
                    obj->speed_y = 0;
                    if (check_couteaux()) {
                        if (pma_phase == 0)
                            pma_phase = 1;

                        if (++obj->param == pma_tempo || pma_type_attaque == 5) {
                            obj->gravity_value_1 = 0;
                            obj->gravity_value_2 = 3;
                            set_main_and_sub_etat(obj, 2, 4);
                        }
                    }
                    break;
                case 4:
                    y = obj->offset_by + obj->y;
                    x = xmap + SCREEN_WIDTH / 2;
                    if ((y_floor(x, y) - y) < 5) {
                        obj->speed_y = 1;
                        obj->param = 0;
                        set_main_and_sub_etat(obj, 2, 5);
                    }
                    break;
                case 5:
                    if (EOA(obj))
                        set_main_and_sub_etat(obj, 0, 2);
                    break;
            }
            break;
    }
}

//66170
void init_mama_pirate(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    init_couteaux();
    obj->hit_points = 6;
    obj->init_hit_points = 6;
    pma_groupe = 0;
    pma_attaque = 0;
    pma_phase = 0;
    cou_place = 0;
    pma_type_attaque = pma_sequence[0][0];
    pma_tempo = 150;
    cou_tempo = 70;
    pma_nb_couteau = pma_couteaux[pma_type_attaque];
    pma_touched = false;
    obj->flags.flip_x = 0;
    obj->speed_x = 0;
    obj->speed_y = 0;
    set_main_and_sub_etat(obj, 2, 6);
    obj->anim_frame = 0;
    obj->anim_index = obj->eta[2][6].anim_index;
}

//66218
void PMA_SORT_DU_CANON(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    init_mama_pirate(obj);
    scroll_end_x = xmap;
    scroll_start_x = xmap;
    scroll_end_y = ymap;
    scroll_start_y = ymap;
}

//66244
void DO_PMA_POING_COLLISION(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PC version adds an extra check for the sprite.
    if (
#ifdef PC
        sprite == 5 &&
#endif
        !pma_touched && obj->main_etat == 2 && obj->sub_etat == 3
    ) {
        set_main_and_sub_etat(obj, 2, 9);
        obj->speed_x = 0;
        obj->speed_y = -2;
        obj->gravity_value_1 = 0;
        obj->gravity_value_2 = 8;
    }
}

//6628C
s32 pma_get_eject_sens(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions are identical.
    ray.iframes_timer = 40;
    return -1;
}

//662A0
void DO_COU_ATTER(obj_t* obj) {
    eta_t *eta;
    if (obj->main_etat == 2 && obj->sub_etat == 10) {
        recale_position(obj);
        CouteauxInfos[obj->param].active = true;
        set_main_and_sub_etat(obj, 0, 9);
        obj->speed_x = 0;
        obj->speed_y = 0;
    } else if (obj->main_etat == 2 && obj->sub_etat == 11) {
        if (pma_type_attaque == 1 || pma_type_attaque == 2) {
            CouteauxInfos[obj->param].active = true;
            if (check_couteaux())
                pma_phase = 1;
            recale_position(obj);
            set_main_and_sub_etat(obj, 0, 9);
            obj->speed_x = 0;
            obj->speed_y = 0;
        }
    } else if (obj->main_etat == 2 && obj->sub_etat == 14) {
        obj->speed_y = -obj->speed_y;
        obj->gravity_value_1 = obj->gravity_value_2 - obj->gravity_value_1;
        eta = &obj->eta[obj->main_etat][obj->sub_etat];
        eta->anim_speed &= 0xf0;
        eta->anim_speed |= 2;
        recale_position(obj);
    }
}

//663B0
/* DOS: no first-landing shake; sound 62 accompanies the later landing smoke. */
void DO_PMA_ATTER(obj_t* obj) {
    if (obj->main_etat == 2 && obj->sub_etat == 7) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        recale_position(obj);
        set_main_and_sub_etat(obj, 2, 8);
    } else if (obj->main_etat == 2 && obj->sub_etat == 5) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        if (screen_trembling == 0) {
            if (obj->param == 0) {
                recale_position(obj);
                allocateLandingSmoke(obj);
                PlaySnd(62, obj->id);
                obj->param = 1;
            }
            screen_trembling = 1;
        }
        if (pma_phase == 1) {
            pma_phase = 2;
            reset_couteaux();
        }
    }
}
