#include "type_island_abi.h"
#include "map.h"
/* bbmont.c (1:100b0..1:13030). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_bbmont. worker w2_bbskops candidate; measured results in NOTES.md. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"

typedef struct bb_data_t {
    s16 speed_x;
    s16 field1_0x2;
    u8 field2_0x4;
    u8 saved_main_etat;
    u8 saved_sub_etat;
    u8 _pad_7;
    s16 saved_cmd;
    s16 sprite6_x;
    s16 sprite6_y;
    u8 field8_0xe;
    u8 _pad_15;
} bb_data_t;
extern bb_data_t bb1;

extern bb_attack_entry_t SerieAtakBB[6][7];
extern s16 NextAtak;
extern s16 horlogeDog;
extern u8 scrollLocked;
extern u8 fin_boss;
extern u8 boss_mort;
extern finbosslevel_t finBosslevel;

#define BLOCK_CH_DIR 5
#define BLOCK_FLAG_4 4
#define BLOCK_SOLID 1
#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_UP 3
#define GO_WAIT 2
#define TYPE_BB1_PLAT 115
#define TYPE_BOUT_TOTEM 211
#define TYPE_BBL 97
#define TYPE_ECLAIR 121
#define TYPE_ETINC 135
#define TYPE_STONEDOG2 122
#define Abs(value) ((value) >= 0 ? (value) : -(value))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
extern u8 FinAnim;
extern u8 Phase;
extern s16 PosPierre;
extern u8 WaitForFinAtan;
extern u8 WaitForAnim;
extern s16 IndAtak;
extern s16 IndSerie;
extern s16 PierreDoitExploser;
extern s16 RMrepere;
extern s16 screen_trembling2;
extern s16 ray_zdc_x;
extern s16 ray_zdc_y;
extern s16 ray_zdc_h;
extern s16 ray_zdc_w;
extern s16 scroll_end_x;
extern s16 scroll_end_y;
extern s16 xmap;
extern s16 ymap;
extern s16 xmapmax;
extern s16 ymapmax;
extern u8 IsBossThere;
extern s16 YaDesChiens;
extern block_flags_t block_flags[64];
extern u8 horloge[25];
extern level_t level;
extern map_data_t mp;
extern s16 niveau;
extern obj_t *poing_obj;
extern obj_t ray;
extern s16 screen_trembling3;
s16 inter_box(s32 x1, s32 y1, s32 w1, s32 h1, s16 x2, s16 y2, s32 w2, s32 h2);
u8 calc_typ_travd(obj_t *obj, u8 direction);
void CALC_MOV_ON_BLOC(obj_t* obj);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void PlaySnd(s16 snd, s16 obj_id);
void PlaySnd_old(s16 sound_id);
void SET_X_SPEED(obj_t* obj);
void add_alwobj(obj_t* obj);
obj_t* allocateExplosion(obj_t* obj);
void allocateLandingSmoke(obj_t* in_obj);
void allocateStoneChips(obj_t* obj);
void calc_obj_pos(obj_t* obj);
void obj_init(obj_t* obj);
void recale_position(obj_t* obj);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
s16 x_pos(u8 param_1, u8 nb_couteau);
void DO_TOTEM_COMMAND(obj_t *obj);
void allocateTir(obj_t *obj, s16 type);
void INIT_BBMONT(obj_t *obj);
void DO_BBL_COMMAND(obj_t *obj);
void BB_Attaque(obj_t *obj);
void Fin_BB_Attaque(obj_t *obj);
void DO_BBMONT_COMMAND(obj_t *obj);
void DO_BBMONT2_COMMAND(obj_t *obj);
void DO_BBMONT2_MARCHE(obj_t *obj);
void DO_BBMONT2_ATTER(obj_t *obj);
void DO_BBMONT3_COMMAND(obj_t *obj);
void DO_TOTEM_TOUCHE(obj_t* obj, s16 sprite);
void DO_TOTBT_REBOND(obj_t *obj);
void DO_PI_EXPLOSION2(obj_t *obj);
void DO_BBL_REBOND(obj_t* obj);
void allocateDog(obj_t* bb1_obj);
void CreateFirstBBL(void);
void BBMONT_ECLAIR(obj_t* bb1_obj);
void BBMONT_ETINCELLES(obj_t* in_obj);
void BBMONT_TIRE(obj_t *obj);
void Cree_BBL(obj_t *obj);
void BB_Atan(obj_t *obj);
void DO_BBMONT_ATTER(obj_t *obj);
void DO_BBMONT_TOUCHE(obj_t* obj, s16 sprite);
void DO_BBMONT3_ATTER(obj_t* obj);
void DO_BBMONT4_COMMAND(obj_t *obj);
void DO_ECLAIR_COMMAND(obj_t* ecl_obj);
void DO_BB1_PLAT_CMD(obj_t *obj);

void BB2_MARCHE(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem identical. The function is inlined in the PS1 version.
    switch (bb1.speed_x) {
        case 2:
            set_main_and_sub_etat(obj, 1, 1);
            break;
        case 3:
            set_main_and_sub_etat(obj, 1, 2);
            break;
        case 0:
        default:
            set_main_and_sub_etat(obj, 1, 0);
            break;
    }
}

void DO_TOTEM_TOUCHE(obj_t* obj, s16 sprite) {
    /* 5783C 8017C03C -O2 -msoft-float */
    if (sprite == 6 && obj->param == 0) {
        if (poing_obj->speed_x > 0) {
            set_sub_etat(obj, 4);
            obj->x = obj->init_x + 71;
        } else {
            set_sub_etat(obj, 5);
            obj->x = obj->init_x - 85;
        }
        obj->iframes_timer = 0;
        obj->param = 255;
        obj->anim_frame = 255;
    } else if (sprite > 1) {
        if (poing_obj->speed_x > 0)
            set_sub_etat(obj, 7);
        else
            set_sub_etat(obj, 6);
    }
}

void DO_TOTEM_COMMAND(obj_t* tot_obj) {
    /* 57950 8017C150 -O2 -msoft-float */
    s16 x; s16 w; s16 y; s16 h;
    s16 nb_objs;
    s16 i;
    s16 j;
    obj_t *cur_obj;
    s16 new_spd_x;
    u8 unk_1;
    s16 new_x;

    tot_obj->param--;
    if (tot_obj->param < 0)
        tot_obj->param = 0;
    if (bb1.field2_0x4 == 3) {
        tot_obj->is_active = 0;
        tot_obj->flags.alive = 0;
        cur_obj = level.objects;
        nb_objs = level.nb_objects;
        i = 0;
        if (nb_objs > 0) {
            do {
                if (cur_obj->type == TYPE_BOUT_TOTEM) {
                    for (j = 0; j < 6; cur_obj++, j++) {
                        GET_SPRITE_POS(tot_obj, j, &x, &y, &w, &h);
                        cur_obj->x = x;
                        cur_obj->y = y;
                        cur_obj->is_active = 1;
                        cur_obj->flags.alive = 1;
                        if (j == 0) {
                            cur_obj->speed_x = 0;
                            cur_obj->speed_y = 0;
                        } else {
                            new_spd_x = 2 - myRand(4);
                            cur_obj->speed_x = new_spd_x;
                            if (new_spd_x == 0)
                                cur_obj->speed_x = 1;
                            cur_obj->speed_y = -4 - myRand(3);
                        }
                        cur_obj->gravity_value_1 = 0;
                        cur_obj->gravity_value_2 = 5;
                        switch (j) {
                            case 1:
                                set_main_and_sub_etat(cur_obj, 2, 8);
                                cur_obj->offset_by = 43;
                                cur_obj->offset_bx = 25;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 31;
                                break;
                            case 2:
                                set_main_and_sub_etat(cur_obj, 2, 10);
                                cur_obj->offset_by = 17;
                                cur_obj->offset_bx = 12;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 33;
                                break;
                            case 3:
                                set_main_and_sub_etat(cur_obj, 2, 11);
                                cur_obj->offset_by = 7;
                                cur_obj->offset_bx = 7;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 34;
                                break;
                            case 4:
                                set_main_and_sub_etat(cur_obj, 2, 9);
                                cur_obj->offset_by = 28;
                                cur_obj->offset_bx = 17;
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 32;
                                break;
                            case 5:
                                set_main_and_sub_etat(cur_obj, 2, 10);
                                cur_obj->offset_by = 17;
                                cur_obj->offset_bx = 12;
                                cur_obj->flags.flip_x = 1;
                                cur_obj->anim_index = 33;
                                break;
                            case 0:
                                set_main_and_sub_etat(cur_obj, 0, 21);
                                cur_obj->flags.flip_x = 0;
                                cur_obj->anim_index = 37;
                                break;
                        }
                    }
                    break;
                }
                i++;
                cur_obj++;
            } while (i < nb_objs);
        }
    } else if (!(tot_obj->sub_etat == 7 || tot_obj->sub_etat == 6 || tot_obj->sub_etat == 3)) {
        GET_SPRITE_POS(tot_obj, 6, &x, &y, &w, &h);
        x += 5; y += 5; w -= 10; h -= 10;
        if (inter_box(x, y, w, h, bb1.sprite6_x, bb1.sprite6_y, 32, 32) && bb1.field2_0x4 == 0) {
            bb1.field2_0x4 = 1;
            tot_obj->iframes_timer = 4;
            IndSerie++;
            if (IndSerie >= 6)
                IndSerie--;
            IndAtak = 0;
        }

        if (tot_obj->anim_frame == (tot_obj->animations[tot_obj->anim_index].frames_count - 1) &&
            horloge[tot_obj->eta[tot_obj->main_etat][tot_obj->sub_etat].anim_speed & 0xf] == 0)
            unk_1 = 1;
        else
            unk_1 = 0;

        switch (tot_obj->sub_etat) {
            case 4:
                if (unk_1) {
                    if (++tot_obj->iframes_timer < 4) {
                        set_sub_etat(tot_obj, 5);
                        tot_obj->x = tot_obj->init_x - 85;
                    } else {
                        set_sub_etat(tot_obj, 6);
                        tot_obj->x = tot_obj->init_x;
                    }
                }
                break;
            case 5:
                if (unk_1) {
                    if (++tot_obj->iframes_timer < 4) {
                        set_sub_etat(tot_obj, 4);
                        tot_obj->x = tot_obj->init_x + 71;
                    } else {
                        set_sub_etat(tot_obj, 7);
                        tot_obj->x = tot_obj->init_x;
                    }
                }
                break;
        }
    }
}

void DO_TOTBT_REBOND(obj_t *obj)
{
    s32 speed_x;
    s32 speed_y;

    if (obj->speed_y > 0 && obj->y + obj->offset_by > 264)
    {
        speed_x = obj->speed_x / 2;
        obj->speed_x = speed_x;
        speed_y = -obj->speed_y / 2;
        obj->speed_y = speed_y;
        if (obj->speed_y == 0)
            obj->gravity_value_2 = 0xFF;
        obj->gravity_value_1 = 5 - obj->gravity_value_1;
    }
}

void DO_PI_EXPLOSION2(obj_t *obj)
{
    PlaySnd_old(0x54);
    obj->speed_x = -1;
    obj->x += 20;
    allocateStoneChips(obj);
    obj->speed_x = 1;
    obj->x -= 40;
    allocateStoneChips(obj);
    obj->x += 20;
    allocateExplosion(obj);
}

void DO_BBL_REBOND(obj_t* obj) {
    /* 57FBC 8017C7BC -O2 -msoft-float */
    PlaySnd(200, obj->id);
    switch (obj->param) {
        case 0:
            obj->speed_y = -5;
            obj->gravity_value_1 = 3 - obj->gravity_value_1;
            break;
        case 2:
            obj->speed_y = -5;
            obj->gravity_value_1 = 3 - obj->gravity_value_1;
            PosPierre = obj->x + (obj->flags.flip_x ? -130 : 25);
            break;
        case 4:
            DO_PI_EXPLOSION2(obj);
            obj->is_active = 0;
            obj->flags.alive = 0;
            if (ray.follow_id == obj->id) {
                ray.follow_id = -1;
                obj->ray_dist = 1000;
                set_main_and_sub_etat(&ray, 2, 2);
            }
//            PS1_AlwaysObjectsCount -= 1;
            break;
    }
    recale_position(obj);
}

void allocateDog(obj_t* bb1_obj) {
    /* 580F8 8017C8F8 -O2 -msoft-float */
    s16 new_x;
    obj_t *cur;
    s16 nb_objs;
    s16 i = 0;
    cur = level.objects;
    nb_objs = level.nb_objects;

    if (nb_objs > 0) {
        do {
            if (cur->type == TYPE_STONEDOG2 && !cur->is_active) {
                if (!((bb1_obj->flags.flip_x + YaDesChiens) & 1)) {
                    new_x = -20;
                    cur->flags.flip_x = 1;
                } else {
                    new_x = 260;
                    cur->flags.flip_x = 0;
                }
                obj_init(cur);
                cur->cmd_offset = -1;
                cur->x = new_x;
                cur->y = bb1_obj->y - 20;
                cur->is_active = 1;
                cur->flags.alive = 1;
                add_alwobj(cur);
                cur->speed_x = 0;
                cur->speed_y = 0;
                cur->main_etat = 2;
                cur->flags.flag_0x40 = 0;
                cur->sub_etat = 2;
                if (cur->flags.flip_x)
                    skipToLabel(cur, 1, 1);
                else
                    skipToLabel(cur, 0, 1);
                calc_obj_pos(cur);
                if (niveau == 0)
                    cur->hit_points = 1;
                break;
            }
            cur++;
            i++;
        } while (i < nb_objs);
    }
}

void allocateTir(obj_t* bb1_obj, s16 param_2) {
    /* 58278 8017CA78 -O2 -msoft-float */
    s16 sprite_ind;
    s32 new_x_1;
    s32 bb1_x;
    s32 new_spd_x;
    s32 new_x_2;
    s16 nb_objs;
    s16 i = 0;
    obj_t *cur_obj;
    cur_obj = level.objects;
    nb_objs = level.nb_objects;

    if (nb_objs > 0) {
        do {
            if (cur_obj->type == TYPE_BBL && !cur_obj->is_active) {
                s16 y; s16 x; s16 h; s16 w;
                if (param_2 == 4) {
                    cur_obj->flags.follow_enabled = 0;
                } else {
                    cur_obj->flags.follow_enabled = 1;
                }
                if (param_2 != 0)
                    sprite_ind = 8;
                else
                    sprite_ind = 9;
                cur_obj->flags.flip_x = 0;
                GET_SPRITE_POS(bb1_obj, sprite_ind, &x, &y, &w, &h);
                if (param_2 == 1) {
                    bb1_x = x - cur_obj->offset_bx;
                    new_x_1 = (w >> 1) + bb1_x;
                    cur_obj->x = new_x_1 + (bb1_obj->flags.flip_x ? 64 : -64);
                    cur_obj->y = y - 40;
                    if (bb1_obj->flags.flip_x)
                        new_spd_x = 12;
                    else
                        new_spd_x = -12;
                    cur_obj->speed_x = new_spd_x;
                    cur_obj->speed_y = 0;
                    cur_obj->flags.flip_x = bb1_obj->flags.flip_x;
                } else if (param_2 == 0) {
                    bb1_x = x - cur_obj->offset_bx;
                    new_x_1 = bb1_x; new_x_1 += w >> 1;
                    cur_obj->x = new_x_1 + (bb1_obj->flags.flip_x ? 9 : -9);
                    cur_obj->y = y - 40;
                    if (bb1_obj->flags.flip_x)
                        new_spd_x = 2;
                    else
                        new_spd_x = -2;
                    cur_obj->speed_x = new_spd_x;
                    cur_obj->speed_y = -5;
                    cur_obj->flags.flip_x = bb1_obj->flags.flip_x;
                } else {
                    if (param_2 == 3) {
                        cur_obj->x = bb1_obj->x + (bb1_obj->flags.flip_x ? 120 : -36);
                        cur_obj->y = bb1_obj->y - 185;
                        cur_obj->speed_y = 4;
                    } else {
                        if (param_2 < 10) {
                            cur_obj->x = ray.x;
                            if (cur_obj->x < 40)
                                cur_obj->x = 40;
                            if (cur_obj->x > 200)
                                cur_obj->x = 200;
                        } else {
                            if (bb1_obj->flags.flip_x)
                                cur_obj->x = 240 - bb1.field8_0xe * 50;
                            else {
                                cur_obj->x = bb1.field8_0xe * 50 - 20;
                            }
                        }
                        cur_obj->y = bb1_obj->y - 200;
                        cur_obj->speed_y = 0;
                    }
                    cur_obj->speed_x = 0;
                }
                cur_obj->iframes_timer = 38;
                if (param_2 != 3)
                    cur_obj->gravity_value_2 = 10;
                else
                    cur_obj->gravity_value_2 = 7;

                if (param_2 == 0)
                    cur_obj->gravity_value_2 = 3;
                cur_obj->main_etat = 2;
                cur_obj->sub_etat = 6;
                skipToLabel(cur_obj, 1, 1);
                calc_obj_pos(cur_obj);
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                add_alwobj(cur_obj);
                cur_obj->gravity_value_1 = 0;
                if (param_2 < 10)
                    cur_obj->param = param_2;
                else
                    cur_obj->param = 4;
//                if (param_2 != 3)
//                {
//                    PS1_AlwaysObjects[PS1_AlwaysObjectsCount] = cur_obj->id;
//                    PS1_AlwaysObjectsCount++;
//                }
                break;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs);
    }
}

void CreateFirstBBL(void) {
    s16 nb_objs;
    obj_t *obj;
    s16 i = 0;
    obj = level.objects;
    nb_objs = level.nb_objects;
    if (nb_objs > 0) {
        do {
            if (obj->type == TYPE_BBL && !obj->is_active) {
                obj->x = -40;
                obj->y = 160;
                obj->speed_x = 0;
                obj->speed_y = 0;
                PosPierre = obj->x + 25;
                obj->iframes_timer = 38;
                obj->gravity_value_2 = 3;
                obj->main_etat = 2;
                obj->flags.flip_x = 0;
                obj->sub_etat = 6;
                skipToLabel(obj, 1, 1);
                calc_obj_pos(obj);
                obj->is_active = 1;
                obj->flags.alive = 1;
                obj->flags.follow_enabled = 1;
                add_alwobj(obj);
                obj->gravity_value_1 = 0;
                obj->param = 2;
                break;
            }
            i++;
            obj++;
        } while (i < nb_objs);
    }
}

void INIT_BBMONT(obj_t* obj) {
    calc_obj_pos(obj);
    obj->is_active = 0;
    WaitForFinAtan = 10;
    obj->param = 0;
    obj->flags.alive = 1;
    obj->gravity_value_2 = 10;
    bb1.field8_0xe = 1;
    YaDesChiens = 0;
    screen_trembling2 = 0;
    PierreDoitExploser = 0;
    IndSerie = 0;
    IndAtak = 0;
    RMrepere = 0;
    bb1.speed_x = 0;
    bb1.saved_cmd = 0;
    Phase = 0;
    PosPierre = 9999; /* COMPILER_STEERED: independent post-call store order. */
    WaitForAnim = 0;
    bb1.field1_0x2 = -1;
    bb1.field2_0x4 = 0;
}

void DO_BBL_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    // There is a bug in the original game, though, causing a potential out-of-bounds access (see below).

    s16 new_spd_x;
    s16 speed_x;
    s32 spd_x_abs;
    s32 spd_x;
    s32 bb1_x;
    s16 unk_x;
    s32 tile_x;
    s32 tile_y;
    map_tile_t *map;

    if (PierreDoitExploser) {
        DO_PI_EXPLOSION2(obj);
        obj->is_active = 0;
        obj->flags.alive = 0;
        if (ray.follow_id == obj->id) {
            ray.follow_id = -1;
            obj->ray_dist = 1000;
            set_main_and_sub_etat(&ray, 2, 2);
        }
        PierreDoitExploser = 0;
        PosPierre = 9999;
    }

    if (obj->param == 3 && --obj->iframes_timer == 0) {
        obj->is_active = 0;
        obj->flags.alive = 0;
    }

    unk_x = 20;
    if (obj->speed_x < 0)
        unk_x = -20;

    tile_x = (obj->x + obj->offset_bx + unk_x) >> 4;
    tile_y = (obj->y + 40) >> 4;
    tile_x += mp.width * tile_y;
    map = mp.map;
    if (((u8 *)block_flags)[map[(s16)tile_x].tile_type] & (1 << BLOCK_FLAG_4)) {
        switch (obj->param) {
            case 1:
                PlaySnd(200, obj->id);
                spd_x = obj->speed_x;
                spd_x_abs = Abs(spd_x);
                if (spd_x_abs == 12) {
                    if (obj->speed_x > 0)
                        new_spd_x = -13;
                    else
                        new_spd_x = 13;
                    obj->speed_x = new_spd_x;
                    obj->speed_y = 0;
                } else if (Abs((s32)obj->speed_x) == 13) {
                    if (obj->speed_x > 0)
                        new_spd_x = -14;
                    else
                        new_spd_x = 14;
                    obj->speed_x = new_spd_x;
                    obj->speed_y = 0;
                } else if (Abs((s32)obj->speed_x) == 14) {
                    if (obj->speed_x > 0)
                        new_spd_x = -15;
                    else
                        new_spd_x = 15;
                    obj->speed_x = new_spd_x;
                    obj->speed_y = 0;
                } else if (Abs((s32)obj->speed_x) == 15) {
                    DO_PI_EXPLOSION2(obj);
                    obj->is_active = 0;
                    obj->flags.alive = 0;
                    if (ray.follow_id == obj->id) {
                        ray.follow_id = -1;
                        obj->ray_dist = 1000;
                        set_main_and_sub_etat(&ray, 2, 2);
                    }
                    return;
                }
                break;
            case 0:
                PlaySnd(200, obj->id);
                obj->speed_x = 0;
                obj->param = 2;
                break;
        }
    }
}

void BBMONT_ECLAIR(obj_t* bb1_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    s16 new_spd_x;
    eta_t **eta_rows;
    s16 x; s16 h; s16 y; s16 w;
    s16 nb_objs;
    s16 i = 0;
    obj_t* cur_obj;
    cur_obj = level.objects;
    nb_objs = level.nb_objects;

    if (nb_objs > 0) {
        do {
            if (cur_obj->type == TYPE_ECLAIR && !cur_obj->is_active) {
                cur_obj->flags.flip_x = bb1_obj->flags.flip_x;
                cur_obj->speed_y = 0;
                eta_rows = cur_obj->eta;
                new_spd_x = eta_rows[cur_obj->main_etat][cur_obj->sub_etat].speed_x_right;
                cur_obj->speed_x = cur_obj->flags.flip_x ? new_spd_x : -new_spd_x;
                GET_SPRITE_POS(bb1_obj, 10, &x, &y, &w, &h);
                { /* COMPILER_STEERED: INVENTED local left names the promoted X operand. */
                    s32 left;
                    left = x;
                    cur_obj->x = ((w + left) - cur_obj->offset_bx) + (bb1_obj->flags.flip_x ? 16 : -16);
                }
                cur_obj->y = y;
                cur_obj->init_x = cur_obj->x;
                cur_obj->init_y = cur_obj->y;
                skipToLabel(cur_obj, cur_obj->flags.flip_x, 1);
                calc_obj_pos(cur_obj);
                cur_obj->is_active = 1;
                cur_obj->flags.alive = 1;
                add_alwobj(cur_obj);
                allocateExplosion(cur_obj);
                break;
            }
            cur_obj++;
            i++;
        } while (i < nb_objs);
    }
}

void BBMONT_ETINCELLES(obj_t* in_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    s16 in_x; s16 in_w; s16 in_y; s16 in_h;
    s32 new_x;
    obj_t *cur_obj;
    s16 nb_objs;
    s16 i;
    cur_obj = level.objects;
    nb_objs = level.nb_objects;
    i = 0;

    if (nb_objs > 0) {
        do {
        if (cur_obj->type == TYPE_ETINC && !cur_obj->is_active) {
            /* sprite 3 */
            cur_obj->flags.flip_x = in_obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (!cur_obj->flags.flip_x)
                cur_obj->speed_x = 7;
            else
                cur_obj->speed_x = -7;

            GET_SPRITE_POS(in_obj, 3, &in_x, &in_y, &in_w, &in_h);
            if (!in_obj->flags.flip_x)
                in_w = 0;

            new_x = in_x + in_w - cur_obj->offset_bx;
            cur_obj->x = new_x;
            cur_obj->y = in_y - 8;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, 1);
            calc_obj_pos(cur_obj);
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
            add_alwobj(cur_obj);

            /* sprite 2 */
            cur_obj++;
            cur_obj->flags.flip_x = in_obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (!(cur_obj->flags.flip_x))
                cur_obj->speed_x = 7;
            else
                cur_obj->speed_x = -7;

            GET_SPRITE_POS(in_obj, 2, &in_x, &in_y, &in_w, &in_h);
            if (!in_obj->flags.flip_x)
                in_w = 0;

            { /* COMPILER_STEERED: INVENTED local left names the promoted X operand. */
                    s32 left;
                    left = in_x;
                    new_x = in_w + left - cur_obj->offset_bx;
            }
            cur_obj->x = new_x;
            cur_obj->y = in_y - 8;
            cur_obj->init_x = cur_obj->x;
            cur_obj->init_y = cur_obj->y;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, 1);
            calc_obj_pos(cur_obj);
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
            add_alwobj(cur_obj);

            allocateExplosion(cur_obj);
            break;
        }
        cur_obj++;
        i++;
        } while (i < nb_objs);
    }
}

void BBMONT_TIRE(obj_t *obj)
{
    allocateTir(obj, 1);
}

void Cree_BBL(obj_t *obj)
{
    allocateTir(obj, 0);
}

void BB_Attaque(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    switch (NextAtak) {
        case 0:
            set_main_and_sub_etat(obj, 0, 4);
            Phase = 1;
            break;
        case 1:
            set_main_and_sub_etat(obj, 0, 3);
            Phase = 2;
            break;
        case 2:
            set_main_and_sub_etat(obj, 0, 6);
            Phase = 3;
            break;
        case 3:
            set_main_and_sub_etat(obj, 0, 7);
            Phase = 4;
            break;
        case 4:
            set_main_and_sub_etat(obj, 0, 2);
            Phase = 5;
            break;
        case 5:
            set_main_and_sub_etat(obj, 0, 4);
            Phase = 12;
            break;
        case 6:
            set_main_and_sub_etat(obj, 2, 3);
            obj->iframes_timer = 35;
            Phase = 11;
            bb1.field8_0xe = 1;
            break;
        case 7:
            set_main_and_sub_etat(obj, 0, 22);
            Phase = 13;
        default: break;
    }
}

void Fin_BB_Attaque(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    IndAtak++;
    if (IndAtak < 7)
        NextAtak = SerieAtakBB[IndSerie][IndAtak].attack;
    else
        NextAtak = 0xff;

    if (NextAtak == 0xff) {
        IndAtak = 0;
        NextAtak = SerieAtakBB[IndSerie][IndAtak].attack;
    }
    WaitForFinAtan = SerieAtakBB[IndSerie][IndAtak].wait_for_fin_atan;
    BB_Attaque(obj);
}

void BB_Atan(obj_t *obj)
{
    if (WaitForFinAtan != 0)
    {
        set_main_and_sub_etat(obj, 0, 0);
        skipToLabel(obj, 0, 1);
        Phase = 6;
    }
    else
        Fin_BB_Attaque(obj);
}

void DO_BBMONT_ATTER(obj_t *obj)
{
    obj->speed_y = 0;
    obj->speed_x = 0;
    recale_position(obj);
    set_sub_etat(obj, 6);
    allocateLandingSmoke(obj);
    screen_trembling3 = 1;
}

void DO_BBMONT_COMMAND(obj_t* obj) {
    // NOTE(Falcury): I did not spot functional differences PS1 and PC versions at first glance
    s16 unk_1; s16 unk_2;
    s32 x_1;
    s32 x_2;
    s32 pos_pierre;
    u8 wait;
    s32 new_dogs;
    s16 new_spd_x_1;
    s16 new_spd_x_2;

    scrollLocked = 1;
    if (YaDesChiens != 0 && --horlogeDog < 0) {
        horlogeDog = 50;
        allocateDog(obj);
        YaDesChiens--;
    }
    GET_SPRITE_POS(obj, 6, &bb1.sprite6_x, &bb1.sprite6_y, &unk_1, &unk_2);
    unk_1 = obj->x;
    /* SYNTHETIC: preserve the original s16 assignment via a local address. */
    {
        s16 y_snapshot;
        s16 *y_address;
        y_address = &y_snapshot;
        *y_address = obj->y;
        unk_2 = y_snapshot;
    }
    if (obj->anim_frame == obj->animations[obj->anim_index].frames_count - 1 &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
    ) {
        FinAnim = 1;
    } else {
        FinAnim = 0;
    }

    if (bb1.field2_0x4 == 2 && FinAnim) {
        if (obj->hit_points == obj->init_hit_points - 1) {
            skipToLabel(obj, 4, 1);
            Phase = 7;
        }
        set_main_and_sub_etat(obj, bb1.saved_main_etat, bb1.saved_sub_etat);
        WaitForFinAtan = 1;
        obj->cmd = bb1.saved_cmd;
    }
    if (bb1.field2_0x4 == 1) {
        bb1.saved_main_etat = obj->main_etat;
        bb1.saved_sub_etat = obj->sub_etat;
        set_main_and_sub_etat(obj, 0, 9);
        bb1.field2_0x4 = 2;
        FinAnim = 0;
        obj->speed_x = 0;
        obj->hit_points--;
        bb1.saved_cmd = obj->cmd;
        if (obj->hit_points < 1) {
            bb1.field2_0x4 = 3;
            set_main_and_sub_etat(obj, 0, 20);
            obj->speed_x = 0;
            bb1.saved_cmd = 420;
            boss_mort = 1;
        }
    }

    if (bb1.field2_0x4 == 3) {
        if (--bb1.saved_cmd < 1) {
            fin_boss = 1;
            bb1.saved_cmd = 999;
            finBosslevel.mr_stone = 1;
        }
    } else {
        switch (obj->cmd) {
            case GO_LEFT:
                obj->flags.flip_x = 0;
            case GO_RIGHT:
                if (obj->cmd == GO_RIGHT)
                    obj->flags.flip_x = 1;
                SET_X_SPEED(obj);
                if (((u8 *)block_flags)[calc_typ_travd(obj, 0)] & (1 << BLOCK_FLAG_4)) {
                    obj->flags.flip_x = !obj->flags.flip_x;
                    skipToLabel(obj, 6, 1);
                    Phase = 8;
                }

                if ((Phase == 7) != 0) {
                    if (PosPierre + 4 + (obj->flags.flip_x ? 10 : -10) > obj->x &&
                        PosPierre - 4 + (obj->flags.flip_x ? 10 : -10) < obj->x) {
                        set_main_and_sub_etat(obj, 0, 2);
                        skipToLabel(obj, 0, 1);
                        Phase = 9;
                    }
                }
                CALC_MOV_ON_BLOC(obj);
                break;
            case GO_WAIT:
                switch (Phase) {
                    default: break;
                    case 0:
                        if (bb1.field8_0xe != 0) {
                            CreateFirstBBL();
                            WaitForAnim = 1;
                            bb1.field8_0xe = 0;
                        }

                        if (FinAnim && WaitForAnim) {
                            obj->anim_frame = 0;
                            IndSerie = 0;
                            IndAtak = 0;
                            WaitForFinAtan = SerieAtakBB[IndSerie][IndAtak].wait_for_fin_atan;
                            NextAtak = SerieAtakBB[IndSerie][IndAtak].attack;
                            BB_Attaque(obj);
                        }
                        break;
                    case 1:
                        if (obj->sub_etat == 4 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            screen_trembling2 == 0
                        ) {
                            screen_trembling2 = 1;
                            allocateTir(obj, 4);
                        }

                        if (FinAnim)
                            BB_Atan(obj);
                        break;
                    case 2:
                        if (obj->sub_etat == 11 && obj->anim_frame == 23 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            allocateTir(obj, 1);
                        }
                        if (obj->sub_etat == 3 && obj->anim_frame == 14) {
                            eta_t *eta = obj->eta[obj->main_etat];
                            if (horloge[eta[obj->sub_etat].anim_speed & 0xf] == 0 && screen_trembling2 == 0) {
                            screen_trembling2 = 1;
                            allocateTir(obj, 3);
                            WaitForFinAtan += niveau == 0 ? 3 : 1;
                        }
                        }
                        if (obj->sub_etat == 10 && FinAnim) {
                            if (--WaitForFinAtan != 0) {
                                obj->anim_frame = 0;
                                set_sub_etat(obj, 10);
                            }
                        }
                        if (obj->sub_etat == 11 && FinAnim) {
                            Phase = 11;
                            obj->iframes_timer = 35;
                        }
                        break;
                    case 3:
                        if (obj->sub_etat == 6 && obj->anim_frame == 64 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            new_dogs = (niveau != 0);
                            YaDesChiens = new_dogs + 3;
                            horlogeDog = 0;
                        }

                        if (FinAnim)
                            BB_Atan(obj);
                        break;
                    case 4:
                        if (obj->sub_etat == 7 && obj->anim_frame == 44 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            BBMONT_ETINCELLES(obj);
                            PlaySnd(114, obj->id);
                        }

                        if (FinAnim)
                            BB_Atan(obj);
                        break;
                    case 5:
                        if (obj->sub_etat == 2 && obj->anim_frame == 21 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            BBMONT_ECLAIR(obj);
                            PlaySnd(114, obj->id);
                        }

                        if (FinAnim) {
                            obj->anim_frame = 0;
                            set_sub_etat(obj, 1);
                            Phase = 10;
                        }
                        break;
                    case 6:
                        if (FinAnim && --WaitForFinAtan == 0)
                            Fin_BB_Attaque(obj);
                        break;
                    case 7:
                        if (obj->sub_etat == 8 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        ) {
                            allocateTir(obj, 0);
                        } else {
                            if (FinAnim) {
                                if (--WaitForFinAtan == 0) {
                                    set_main_and_sub_etat(obj, 1, 2);
                                    if (obj->flags.flip_x)
                                        skipToLabel(obj, 5, 1);
                                    else
                                        skipToLabel(obj, 4, 1);
                                } else {
                                    set_sub_etat(obj, 1);
                                }
                            }
                        }
                        break;
                    case 8:
                        if (FinAnim && (obj->sub_etat != 9)) {
                            if (bb1.field8_0xe == 0) {
                                WaitForFinAtan = SerieAtakBB[IndSerie][0].wait_for_fin_atan;
                                NextAtak = SerieAtakBB[IndSerie][0].attack;
                                IndAtak = 0;
                                BB_Attaque(obj);
                            } else {
                                Fin_BB_Attaque(obj);
                                bb1.field8_0xe = 0;
                            }
                        }
                        break;
                    case 9:
                        if (obj->anim_frame == 20 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            (obj->sub_etat != 9)
                        ) {
                            PierreDoitExploser = 1;
                        }

                        if (FinAnim && bb1.field2_0x4 != 2) {
                            set_main_and_sub_etat(obj, 1, 0);
                            if (obj->flags.flip_x)
                                skipToLabel(obj, 5, 1);
                            else
                                skipToLabel(obj, 4, 1);
                            PierreDoitExploser = 0;
                        }
                        break;
                    case 10:
                        if (FinAnim) {
                            obj->anim_frame = 0;
                            set_main_and_sub_etat(obj, 1, 1);
                            if (obj->flags.flip_x)
                                skipToLabel(obj, 5, 1);
                            else
                                skipToLabel(obj, 4, 1);
                        }
                        break;
                    case 11:
                        switch (obj->sub_etat) {
                            case 0:
                            case 3:
                                if (--obj->iframes_timer > 1) {
                                    obj->speed_y = -8;
                                    obj->gravity_value_1 = 0;
                                    if (obj->flags.flip_x)
                                        new_spd_x_1 = 4;
                                    else
                                        new_spd_x_1 = -4;
                                    obj->speed_x = new_spd_x_1;
                                } else {
                                    set_sub_etat(obj, 4);
                                    if (obj->flags.flip_x)
                                        new_spd_x_2 = 2;
                                    else
                                        new_spd_x_2 = -2;
                                    obj->speed_x = new_spd_x_2;
                                }
                                break;
                            case 6:
                                if (FinAnim) {
                                    set_main_and_sub_etat(obj, 1, 0);
                                    if (!obj->flags.flip_x)
                                        skipToLabel(obj, 4, 1);
                                    else
                                        skipToLabel(obj, 5, 1);
                                }
                                break;
                        }
                        break;
                    case 12:
                        if (obj->sub_etat == 4 && obj->anim_frame == 14 &&
                            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                            screen_trembling2 == 0
                        ) {
                            screen_trembling2 = 1;
                            allocateTir(obj, bb1.field8_0xe + 10);
                            bb1.field8_0xe++;
                        }

                        if (FinAnim && bb1.field8_0xe == 3) {
                            bb1.field8_0xe = 0;
                            BB_Atan(obj);
                        }
                        break;
                    case 13:
                        if (obj->sub_etat == 22) {
                            if (obj->anim_frame == 63 &&
                                horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                            ) {
                                allocateTir(obj, 1);
                            }
                            if (obj->anim_frame == 14 &&
                                horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0 &&
                                screen_trembling2 == 0
                            ) {
                                screen_trembling2 = 1;
                                allocateTir(obj, 3);
                            }

                            if (FinAnim) {
                                Phase = 11;
                                obj->iframes_timer = 35;
                                bb1.field8_0xe = 1;
                            }
                        }
                }
                break;
        }

        if (FinAnim && bb1.field2_0x4 == 2) {
            bb1.field2_0x4 = 0;
        }
    }
}

void DO_BBMONT_TOUCHE(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): PS1 and PC versions are slightly different (but in the end functionally identical).
    // In the PS1 version the sprite check is in DO_POING_COLLISION, while in the PC version the check is moved here.
    if (sprite == 9) {
        set_sub_etat(obj, 8);
        obj->anim_frame = 0;
        Phase = 7;
        WaitForFinAtan = 2;
    }
}

void DO_BBMONT2_COMMAND(obj_t* obj) {
    /* ORIGINAL_INLINE abs sites: DOS 1:12829, 1:12a27. */
    //NOTE: DO_BBMONT2_COMMAND is still non-matching in the PS1 decomp!
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_v0_2;
    s16 temp_v0_3;
    s16 var_v0_2;
    s16 var_v1;
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;
    s32 var_v0_6;
    s32 var_v1_2;
    u16 temp_v0_4;
    u16 var_v0;
    u8 temp_v1;
    u8 temp_v1_2;
    u8 temp_v1_3;
    bb_data_t *test_1;
    s32 test_2;
    s32 test_3;
    u8 test_4;
    u8 test_5;
    u32 test_6;

    temp_s2 = (s16) obj->x + obj->offset_bx;
    /* COMPILER_STEERED: retain the same s16 conversion through a scoped copy. */
    {
        s16 reach_y;
        reach_y = obj->offset_hy + (s16) obj->y + 0x40;
        temp_s3 = reach_y;
    }
    if (obj->anim_frame == obj->animations[obj->anim_index].frames_count - 1 &&
        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0)
        FinAnim = 1;
    else
        FinAnim = 0;
    if ((u8) Phase < 2) {
        var_a0 = (obj->main_etat != 2);
        if (var_a0 && inrange(ray_zdc_x - temp_s2, -16, 16) && (ray_zdc_h < (temp_s3 - 32))) {
            set_main_and_sub_etat(obj, 0, 16);
            obj->speed_x = 0;
            obj->speed_y = 0;
        }
    }

    if (obj->main_etat == 1)
    {
        if (Phase == 0)
        {
            Phase = 1;
        }
        SET_X_SPEED(obj);
        if ((u8) Phase < 4U)
        {
            CALC_MOV_ON_BLOC(obj);
        }
        if ((FinAnim != 0) && (bb1.speed_x != (obj->sub_etat + 1)))
        {
            BB2_MARCHE(obj);
        }
    }
    if ((bb1.field1_0x2 < 8) && (Phase != 0) && (Phase != 5))
    {
        if (scroll_end_x == xmapmax)
        {
            var_a0 = max32(xmap, temp_s2);
            scroll_end_x = var_a0;
        }
        else if (scroll_end_x < temp_s2)
        {
            bb1.saved_cmd = 1;
        }
        if (bb1.saved_cmd != 0)
        {
            if (obj->main_etat == 1)
            {
                scroll_end_x = (u16) scroll_end_x + (obj->sub_etat + 1);
            }
            else
            {
                scroll_end_x = scroll_end_x + (u16) obj->speed_x;
            }
        }
    }
    else
    {
        scroll_end_x = xmapmax;
    }
    switch (Phase)
    {
        case 0:
            /* DOS 1:122a3..1:122b7 keeps the phase-0 distance in a byte. */
            if (!(((u8 *)block_flags)[obj->btypes[0]] & (1 << BLOCK_SOLID)))
                temp_v1 = 60;
            else
                temp_v1 = 40;
            if (((((u8 *)block_flags)[obj->btypes[0]] & (1 << BLOCK_SOLID))) == 0)
            {
                if ((ray_zdc_x > (temp_v1 + temp_s2)) && (obj->main_etat == 0) && (bb1.speed_x != 0))
                {
                    obj->flags.flip_x = 1;
                    set_main_and_sub_etat(obj, 2, 1);
                    obj->speed_y = 2;
                    return;
                }
            }
            else if ((obj->main_etat == 0) && (obj->sub_etat == 0) && (ray_zdc_x > (temp_s2 + temp_v1)))
            {
                Phase = 1;
                BB2_MARCHE(obj);
            }
            break;
        case 1:
            if ((obj->main_etat == 1) &&
                !(((u8 *)block_flags)[mp.map[(s16)((((obj->x + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4)))].tile_type] & (1 << BLOCK_SOLID))
            ) {
                set_main_and_sub_etat(obj, 2U, 4U);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_y = -8;
                obj->speed_x = 3;
            }
            break;
        case 2:
            if (
                    (obj->main_etat == 1) &&
                    !(((u8 *)block_flags)[
                              (u16) mp.map [
                                      (s16) (((obj->x - 8 + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))
                              ].tile_type
                      ] & 2)
                    )
            {
                set_main_and_sub_etat(obj, 2U, 1U);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_y = 0;
                obj->speed_x = 0;
                bb1.field1_0x2 = -1;
            }
            break;
        case 3:
            var_a0 = max32(scroll_end_x, 5580);
            scroll_end_x = var_a0;

            if ((ray_zdc_x > (temp_s2 + 32)) && (bb1.field1_0x2 == -1) && (temp_s3 < ray_zdc_h))
            {
                if (bb1.speed_x != -1)
                {
                    BB2_MARCHE(obj);
                    bb1.field1_0x2 = 0;
                }
                else
                {
                    if (obj->param < 200)
                    {
                        if (++obj->param == 200)
                        {
                            bb1.speed_x = 1;
                            BB2_MARCHE(obj);
                            bb1.field1_0x2 = 0;
                            obj->param = 255;
                        }
                    }
                    scroll_end_x = xmap;
                    if (scroll_end_x < temp_s2)
                    {
                        test_2 = max32(1, ray.speed_x);
                        scroll_end_x = scroll_end_x + test_2;
                    }
                }
            }
            if ((FinAnim != 0) && (obj->main_etat == 0) && (obj->sub_etat == 16))
                BB2_MARCHE(obj);
            if ((FinAnim != 0) && (obj->main_etat == 0) && (obj->sub_etat == 1))
                BB2_MARCHE(obj);
            if (obj->main_etat == 1)
            {
                if (!(((u8 *)block_flags)[mp.map[(s16)((((obj->x - 8 + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4)))].tile_type] & (1 << BLOCK_SOLID))) {
                    obj->speed_y = -6;
                    obj->speed_x = 3;
                    set_main_and_sub_etat(obj, 2, 4);
                    obj->gravity_value_2 = 5;
                    obj->gravity_value_1 = 0;
                }
                if ((var_a0 = (obj->main_etat != 2)) && inrange(ray_zdc_x - temp_s2, -16, 16))
                {
                    if (ray_zdc_h < (temp_s3 - 32))
                    {
                        set_main_and_sub_etat(obj, 0U, 16);
                        obj->speed_y = 0;
                    }
                    else
                    {
                        set_main_and_sub_etat(obj, 0U, 1);
                        obj->speed_y = 0;
                    }
                    obj->speed_x = obj->speed_y;
                }
            }
            break;
        case 4:
            if (bb1.sprite6_y < scroll_end_y)
            {
                scroll_end_y -= 1;
            }
            if (FinAnim != 0)
            {
                if ((obj->main_etat == 2) && (obj->sub_etat == 2))
                {
                    bb1.speed_x = 1;
                    BB2_MARCHE(obj);
                }
                else if ((obj->main_etat == 0) && (obj->sub_etat == 5))
                {
                    if (++bb1.sprite6_x < 3)
                    {
                        bb1.speed_x = 1;
                        BB2_MARCHE(obj);
                    }
                    else
                    {
                        set_main_and_sub_etat(obj, 0, 2);
                        Phase = 5;
                        bb1.sprite6_y = ymap;
                    }
                }
            }
            if (obj->main_etat == 1)
            {
                if (!(((u8 *)block_flags)[mp.map[(s16)(((((obj->flags.flip_x ? 16 : -16) + obj->x + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4)))].tile_type] & (1 << BLOCK_SOLID))) {
                    obj->speed_y = 0;
                    obj->speed_x = 0;
                    /* see also: makeUturn() case TYPE_SPIDER_PLAFOND */
                    obj->flags.flip_x = !obj->flags.flip_x;
                    set_main_and_sub_etat(obj, 0, 5);
                }
            }
            if (abs32_inline((s32)(temp_s2 - (ray_zdc_x + (ray_zdc_w >> 1)))) > 250) {
                obj->is_active = 0;
                IsBossThere = 0;
                Phase = 0;
                obj->flags.alive = 0;
                scroll_end_x = xmapmax;
                scroll_end_y = ymapmax;
            }
            break;
        case 5:
            if (bb1.field1_0x2 < 0)
            {
                ++bb1.field1_0x2;
                /* DOS 1:1284b..1:12857 branches on zero after the increment. */
                if ((bb1.field1_0x2 == 0) != 0)
                    scroll_end_y = ymapmax;
                else
                    scroll_end_y = bb1.sprite6_y - horloge[5];
            }

            // NOTE: this part is fixed for the PC version. Not necessarily matching for the PS1 version.
            if (FinAnim && obj->main_etat == 0 && obj->sub_etat == 5) {
                bb1.speed_x = 3;
                BB2_MARCHE(obj);
            }
            if (obj->main_etat == 0 && obj->sub_etat == 2) {
                if (FinAnim) {
                    bb1.speed_x = 2;
                    BB2_MARCHE(obj);
                } else if (obj->anim_frame == 21 && (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0)) {
                    bb1.field1_0x2 = -30;
                }
            }

            if (obj->main_etat == 1) {
                if (!(((u8 *)block_flags)[mp.map[(s16)(
                        ((((obj->flags.flip_x ? 16 : -16) + obj->x + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))
                    )].tile_type] & (1 << BLOCK_SOLID))
                ) {
                    if (bb1.speed_x == 3)
                    {
                        set_main_and_sub_etat(obj, 2, 4);
                        obj->speed_y = -2;
                        obj->speed_x = 4;
                        obj->gravity_value_2 = 0xFF;
                        scroll_end_y = ymapmax;
                        obj->gravity_value_1 = 0;
                        scroll_end_x = xmapmax;
                    }
                    else
                    {
                        obj->speed_y = 0;
                        obj->speed_x = 0;
                        obj->flags.flip_x = !obj->flags.flip_x;
                        set_main_and_sub_etat(obj, 0, 5);
                    }
                }
            }
            if (abs32_inline((s32)(temp_s2 - (ray_zdc_x + (ray_zdc_w >> 1)))) > 400) {
                obj->is_active = 0;
                IsBossThere = 0;
                Phase = 0;
                obj->flags.alive = 0;
                scroll_end_x = xmapmax;
                scroll_end_y = ymapmax;
            }
            break;
    }
}

void DO_BBMONT2_MARCHE(obj_t* obj) {
    bb1.speed_x = obj->hit_points;
}

void DO_BBMONT2_ATTER(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    if (obj->speed_y > 1)
    {
        obj->speed_y = 0;
        obj->speed_x = 0;
        recale_position(obj);
        switch (Phase)
        {
            case 0:
                set_sub_etat(obj, 2);
                break;
            case 1:
                BB2_MARCHE(obj);
                Phase = 2;
                obj->speed_x = bb1.speed_x;
                break;
            case 2:
                set_sub_etat(obj, 2);
                Phase = 3;
                obj->x -= 32;
                bb1.speed_x = -1;
                obj->param = 0;
                break;
            case 3:
                bb1.field1_0x2 += 1;
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_x = 2;
                set_sub_etat(obj, 2);
                obj->speed_x = 0;
                bb1.sprite6_y = 691;
                bb1.sprite6_x = 0;
                scroll_end_y = ymap;
                if (ymap < 691) {
                    scroll_end_y = 691;
                }
                Phase = 4;
                obj->speed_y = 0;
                break;
        }
    }
}

void DO_BBMONT3_COMMAND(obj_t* obj) {
    // NOTE(Falcury): I did not spot functional differences PS1 and PC versions at first glance
    u8 eight;

    if (obj->anim_frame == (obj->animations[obj->anim_index].frames_count - 1) &&
        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
    )
        FinAnim = 1;
    else
        FinAnim = 0;

    if (obj->main_etat == 1) {
        SET_X_SPEED(obj);
        CALC_MOV_ON_BLOC(obj);
        if (FinAnim && (bb1.speed_x != obj->sub_etat + 1))
        {
            BB2_MARCHE(obj);
        }
    }
    switch (Phase)
    {
        case 0:
            obj->flags.flip_x = 0;
            if (obj->main_etat != 2)
                CALC_MOV_ON_BLOC(obj);

            if (FinAnim && obj->main_etat == 0)
            {
                bb1.speed_x = 2;
                BB2_MARCHE(obj);
            }
            eight = 8;
            if ((obj->main_etat == 1) &&
                ((((u8 *)block_flags)[mp.map[(s16)
                        (((obj->x - eight + obj->offset_bx) >> 4) + mp.width * ((obj->y + obj->offset_by + eight) >> 4))
                ].tile_type] & (1 << BLOCK_SOLID)) == 0)
            ) {
                set_main_and_sub_etat(obj, 2, 4);
                obj->gravity_value_2 = 5;
                obj->gravity_value_1 = 0;
                obj->speed_y = -8;
                obj->speed_x = -2;
            }
            break;
        case 1:
            if ((obj->main_etat == 1) &&
                ((((u8 *)block_flags)[mp.map[(s16)
                        (((obj->x + (obj->offset_bx)) >> 4) + mp.width * ((obj->y + obj->offset_by + 8) >> 4))
                ].tile_type] & (1 << BLOCK_SOLID)) == 0)
            ) {
                set_main_and_sub_etat(obj, 0, 1);
                obj->speed_y = 0;
                obj->speed_x = 0;
                Phase = 2;
            }
            break;
    }
}

void DO_BBMONT3_ATTER(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    u8 phase;
    if (obj->speed_y > 1) {
        obj->speed_y = 0;
        obj->speed_x = 0;
        CALC_MOV_ON_BLOC(obj);
        phase = Phase;
        if (phase == 0) {
            BB2_MARCHE(obj);
            Phase = 1;
        }
    }
}

void DO_BBMONT4_COMMAND(obj_t *obj)
{
    obj->flags.flip_x = 1;
    if (
        obj->anim_frame == (obj->animations[obj->anim_index].frames_count - 1) &&
        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
    )
        FinAnim = 1;
    else
        FinAnim = 0;

    if (obj->main_etat == 0)
    {
        if (
            obj->sub_etat == 7 &&
            obj->anim_frame == 0x2C &&
            horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
        )
            BBMONT_ETINCELLES(obj);
        if (FinAnim && obj->sub_etat == 0)
            set_sub_etat(obj, 7);
    }
}

void DO_ECLAIR_COMMAND(obj_t* ecl_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;

    switch (ecl_obj->cmd) {
        case GO_LEFT:
            ecl_obj->flags.flip_x = 0;
        case GO_RIGHT:
            if (ecl_obj->cmd == GO_RIGHT)
                ecl_obj->flags.flip_x = 1;

            SET_X_SPEED(ecl_obj);
            ecl_obj->speed_y = 0;
            if (((u8 *)block_flags)[mp.map[(s16)
                    (((ecl_obj->x + ecl_obj->offset_bx) >> 4) + ((ecl_obj->y + ecl_obj->offset_by) >> 4) * mp.width)
                ].tile_type] & (1 << BLOCK_CH_DIR)
            ) {
                cur_obj = level.objects;
                nb_objs = level.nb_objects;
                i = 0;
                if (nb_objs > 0) do {
                    if (cur_obj->type == TYPE_BB1_PLAT && !cur_obj->is_active) {
                        cur_obj->speed_y = 0;
                        cur_obj->speed_x = 0;
                        cur_obj->flags.flip_x = 0;
                        cur_obj->x = ecl_obj->x - 32;
                        cur_obj->y = ecl_obj->y - 48;
                        cur_obj->init_x = cur_obj->x;
                        cur_obj->init_y = cur_obj->y;
                        calc_obj_pos(cur_obj);
                        cur_obj->flags.alive = 1;
                        cur_obj->is_active = 1;
                        add_alwobj(cur_obj);
                        cur_obj->iframes_timer = 255;
                        cur_obj->param = 30;
                        cur_obj->link = 30;
                        cur_obj->main_etat = 0;
                        cur_obj->sub_etat = 4;
                        cur_obj->anim_frame = 0;
                        cur_obj->cmd_offset = -1;
                        cur_obj->nb_cmd = 0;
                        break;
                    }
                    i++;
                    cur_obj++;
                } while (i < nb_objs);

                ecl_obj->is_active = 0;
                ecl_obj->flags.alive = 0;
            }
    }
}

void DO_BB1_PLAT_CMD(obj_t *obj)
{
    switch (obj->cmd)
    {
    case GO_UP:
        obj->speed_y = -1;
        break;
    case GO_WAIT:
        obj->speed_y = 0;
        break;
    }
}
