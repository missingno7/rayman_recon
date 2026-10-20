/* breakout.c (1:16a20..1:18b40, casse-brique minigame). Names RAYVERSE/ANDROID; Rayverse bodies are mostly stubs here, so most C is written from the DOS code (worker tu_breakout). See config/units.json for statuses. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include <stdio.h>
#ifndef ABS
#define ABS(x) (x >= 0 ? (x) : -(x))
#endif

extern u8 CasseBriqueON;
extern u8 casse_brique_active;
extern s16 num_level_choice;
extern s16 num_world;
extern s16 num_world_choice;
extern s16 old_num_level_choice;
extern s16 old_num_world_choice;
extern u8 cbout;
extern u8 undestroy;
extern u8 upscreen;
extern s16 speed_ball_angle;
extern u8 speed_ball_level;
extern s16 speed_ball_x;
extern s16 speed_ball_y;
extern s32 ball_x;
extern s32 ball_y;
extern s16 ymap;
extern level_t level;
extern s16 CBdata[19][17];
extern s16 myRand(s16 max);

typedef struct casse_brique_entry_t {
    s16 x;
    s16 y;
    s16 object_index;
    u8 kind;
    u8 reserved;
} casse_brique_entry_t;

extern casse_brique_entry_t CasseBrique[20];
extern u8 BonusMode;
extern u8 BonusBrik;
extern u8 BonusBrikType;
extern u8 NbClassicBrik;
extern u8 NbBrikTouched;
extern u8 InvadersMode;
extern u8 LastTouched;
extern s16 CasseBriqueOffsetX[7];
extern s16 CasseBriqueOffsetY[7];
extern s16 NoCB;
extern s16 NoCBInit;
extern s16 tempo;
extern s16 no_sound;
extern u8 horloge[25];
extern s16 xmap;
extern s16 ymap;
extern s16 scroll_start_x;
extern s16 scroll_end_x;
extern s16 CBoffsetX;
extern s16 CBoffsetY;
extern s16 Xdir;
extern s16 Ydir;
extern s32 dist2;
extern s16 CBoffsetXMin;
extern s16 CBoffsetYMin;
extern s16 CBoffsetYMax;
extern s16 CBoffsetXMax;
extern s16 LIMITE_GAUCHE;
extern s16 XOFFSETCBRIGHT;
extern s16 XOFFSETRAY;
extern s16 LIMITE_DROITE;
extern s16 YOFFSETCB;
extern s16 XOFFSETCBLEFT;
extern u8 nb_collis;
extern u8 GameModeVideo;
extern u8 byte_93fa20;
extern s16 ray_mode;
extern s16 new_world;
extern obj_t ray;
extern obj_t *poing_obj;
extern status_bar_t status_bar;
extern s16 ray_zdc_x;
extern s16 ray_zdc_y;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern void Add_One_RAY_lives(void);
extern void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
extern void DO_FADE_OUT(void);
extern void GET_ANIM_POS(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
extern u8 GET_SPRITE_POS(obj_t *obj, s16 sprite_index, s16 *x, s16 *y, s16 *w, s16 *h);
extern void calc_obj_pos(obj_t *obj);
extern s16 inter_box(s32 x1, s32 y1, s32 w1, s32 h1,
                     s16 x2, s16 y2, s32 w2, s32 h2);
extern void DO_NOVA(obj_t *obj);
extern void add_alwobj(obj_t *obj);
extern s32 but0pressed(void);
extern s32 rightjoy(void);
extern s32 leftjoy(void);
extern void SET_X_SPEED(obj_t *obj);
extern s16 instantSpeed(s16 speed);
extern void GET_RAY_ZDC(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
extern void DO_COLLISIONS(void);
extern void DO_ANIM(obj_t *obj);
extern void display_sprite(obj_t *obj, u8 sprite_index, s16 x, s16 y, u8 flipped);
extern void display_text(const char *text, s16 x, s16 y, u8 color, u8 centered);
void AllocateBonus(obj_t *obj, s16 type, u8 sub_etat, u8 main_etat);
u8 get_casse_brique_ON(void);
void set_casse_brique_ON(u8 on);
u8 get_casse_brique_active(void);
void set_casse_brique_active(void);
void enter_casse_brique(void);
void get_speed_ball(void);
void speed_ball_null(obj_t *obj);
void speed_ball_init(obj_t *obj);
void speed_up_ball(obj_t *obj);
void invert_speed_ball_x(obj_t *obj);
void invert_speed_ball_y(obj_t *obj);
void set_ball_pos(obj_t *obj, s16 x, s16 y);
s16 angle_direction(s16 x, s16 y);
void move_ball(obj_t *obj);
void get_brik_bonus(void);
void do_bonus(void);
void frame_briques(obj_t *obj);
void move_briques(obj_t *obj);
void reset_briques(void);
void exit_casse_brique(void);
void exit_casse_brique2(void);
void casse_brique_init(obj_t *obj);
void casse_brique_start(obj_t *obj);
void casse_brique_play(obj_t *obj);
void casse_brique_end(obj_t *obj);
void casse_brique_done(obj_t *obj);
u8 TOUCHE(s16 input);
void PlaySnd_old(s16 sound_id);

u8 get_casse_brique_ON(void) {
    return CasseBriqueON;
}

void set_casse_brique_ON(u8 on) {
    CasseBriqueON = on;
}

u8 get_casse_brique_active(void) {
    return casse_brique_active;
}

void set_casse_brique_active(void) {
    casse_brique_active = 1;
}

void enter_casse_brique(void) {
    if (casse_brique_active) {
        old_num_level_choice = num_level_choice;
        num_level_choice = 22;
        old_num_world_choice = num_world_choice;
        num_world_choice = 1;
        num_world = 1;
        CasseBriqueON = 1;
    }
}

void enter_casse_brique2(void)
{
    cbout = 0;
}

void exit_casse_brique(void)
{
    num_level_choice = old_num_level_choice;
    num_world_choice = old_num_world_choice;
}

void exit_casse_brique2(void)
{
    num_world = 7;
    casse_brique_active = 0;
}

void get_speed_ball(void)
{
    s16 x;
    s16 y;
    u8 level_index;

    x = speed_ball_x;
    y = speed_ball_y;
    level_index = speed_ball_level;
    if (speed_ball_angle < 0)
        speed_ball_angle += 72;
    else if (speed_ball_angle > 72)
        speed_ball_angle -= 72;

    if (18 - speed_ball_angle >= 0) {
        x = CBdata[speed_ball_angle][level_index * 2 + 1];
        y = CBdata[speed_ball_angle][level_index * 2 + 2];
    } else if (36 - speed_ball_angle >= 0) {
        x = -CBdata[36 - speed_ball_angle][level_index * 2 + 1];
        y = CBdata[36 - speed_ball_angle][level_index * 2 + 2];
    } else if (54 - speed_ball_angle >= 0) {
        x = -CBdata[speed_ball_angle - 36][level_index * 2 + 1];
        y = -CBdata[speed_ball_angle - 36][level_index * 2 + 2];
    } else if (72 - speed_ball_angle >= 0) {
        x = CBdata[72 - speed_ball_angle][level_index * 2 + 1];
        y = -CBdata[72 - speed_ball_angle][level_index * 2 + 2];
    }
    speed_ball_y = y;
    speed_ball_x = x;
    speed_ball_level = level_index;
}

void speed_ball_null(obj_t *obj)
{
    obj->speed_y = 0;
    obj->speed_x = obj->speed_y;
    speed_ball_angle = 0;
    speed_ball_level = 0;
    speed_ball_y = 0;
    speed_ball_x = 0;
}

void speed_ball_init(obj_t *obj)
{
    obj->speed_y = 0;
    obj->speed_x = obj->speed_y;
    speed_ball_angle = 18;
    speed_ball_level = 0;
    get_speed_ball();
}

void speed_up_ball(obj_t *obj)
{
    obj->speed_y = 0;
    obj->speed_x = obj->speed_y;
    speed_ball_level++;
    if (speed_ball_level > 7)
        speed_ball_level = 7;
    get_speed_ball();
}

void invert_speed_ball_x(obj_t *obj)
{
    obj->speed_y = 0;
    obj->speed_x = obj->speed_y;
    speed_ball_angle = 36 - speed_ball_angle;
    get_speed_ball();
}

void invert_speed_ball_y(obj_t *obj)
{
    obj->speed_y = 0;
    obj->speed_x = obj->speed_y;
    speed_ball_angle = 72 - speed_ball_angle;
    get_speed_ball();
}

void set_ball_pos(obj_t *obj, s16 x, s16 y)
{
    s32 new_x;
    s32 new_y;

    new_x = x;
    new_y = y;
    obj->x = new_x;
    obj->y = new_y;
    ball_x = new_x << 8;
    ball_y = new_y << 8;
}

s16 angle_direction(s16 x, s16 y)
{
    s32 ratio;
    s16 angle;
    s16 original_x;

    original_x = x;
    if (x != 0) {
        ratio = (y << 9) / x;
        if (ratio < 0)
            ratio = -ratio;
        angle = 17;
        while (ratio < -CBdata[angle][0])
            angle--;
    } else if (y != 0) {
        angle = 18;
    } else {
        angle = 0;
    }
    if (original_x < 0)
        angle = 36 - angle;
    if (y > 0)
        angle = 72 - angle;
    return angle;
}

void move_ball(obj_t *obj)
{
    s32 dx;
    s32 dy;

    dx = 0;
    dy = 0;
    if (undestroy > 5 || upscreen != 0) {
        if (undestroy > 5)
            undestroy = 0;
        dx = myRand(257) - 128;
        dy = myRand(257) - 128;
        upscreen = 0;
    }
    ball_x += dx + speed_ball_x;
    ball_y += dy + speed_ball_y;
    obj->x = ball_x / 256;
    obj->y = ball_y / 256;
}

/* move_ball: no seed source */

void get_brik_bonus(void)
{
    u8 start;
    u8 tries;
    u8 brik_index;
    obj_t *obj;

    start = (u8)myRand(19);
    tries = 0;
    do {
        brik_index = (u8)(tries + start);
        if (brik_index >= 20)
            brik_index -= 20;
        obj = level.objects + CasseBrique[brik_index].object_index;
        if (brik_index != BonusBrik) {
            if (obj->param == 2 || obj->param == 1 || obj->param == 3) {
                obj->hit_points = 10;
                BonusBrik = (u8)brik_index;
                BonusMode = 2;
                BonusBrikType = (u8)obj->param;
                obj->param = 6;
                return;
            }
        }
        tries++;
    } while (tries < 20);
}

void do_bonus(void)
{
    if (NbBrikTouched >= 5) {
        if (BonusMode == 1)
            get_brik_bonus();
        else if (BonusMode != 2)
            BonusMode = 1;
    }
}

u8 get_brik_info(obj_t *obj, s16 entry_index, s16 *x, s16 *y, u8 *kind)
{
    u8 *entry;
    sprite_t *sprite;
    u32 anim_index;
    u8 sprite_index;

    anim_index = obj->animations[2].layers_per_frame & 0x3fff;
    entry = (u8 *)obj->animations[2].layers + (anim_index * NoCB + entry_index) * 4;
    sprite_index = entry[3];
    if (sprite_index == 0)
        return 0;
    sprite = obj->sprites + sprite_index;
    if (sprite->id == 0)
        return 0;
    *x = entry[1];
    *y = entry[2];
    switch (sprite->id) {
    case 0x27: *kind = 2; break;
    case 0x28: *kind = 1; break;
    case 0x29: *kind = 3; break;
    case 0x2a: *kind = 4; break;
    case 0x2b: *kind = 5; break;
    case 0x2c: *kind = 7; break;
    default: *kind = 0; break;
    }
    return 1;
}

void frame_briques(obj_t *obj)
{
    switch (obj->param) {
    case 0:
        obj->flags.alive = 0;
        obj->is_active = 0;
        break;
    case 1:
        obj->anim_frame = 1;
        break;
    case 2:
        obj->anim_frame = 0;
        break;
    case 3:
        obj->anim_frame = 2;
        break;
    case 4:
        if (obj->hit_points <= 5)
            obj->anim_frame = 7;
        else
            obj->anim_frame = 3;
        break;
    case 5:
        obj->anim_frame = 4;
        break;
    case 6:
        obj->anim_frame = 5;
        break;
    case 7:
        if (obj->anim_frame == obj->animations[1].frames_count - 1) {
            obj->anim_frame = 0;
            obj->hit_points--;
            if (obj->hit_points == 0) {
                obj->param = BonusBrikType;
                get_brik_bonus();
            }
        } else if (horloge[4] == 1) {
            obj->anim_frame++;
        }
        break;
    }

    if (obj->param != 0) {
        obj->x = xmap + CBoffsetX + CasseBrique[obj->iframes_timer].x - obj->offset_bx;
        obj->y = ymap + CBoffsetY + CasseBrique[obj->iframes_timer].y - obj->offset_by;
    }
}

void AllocateCasseBriqueBoum(obj_t *obj)
{
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 i;
    obj_t *explosion;

    for (i = 0; i < level.nb_objects; i++) {
        explosion = level.objects + i;
        if (explosion->type == 0xb && explosion->is_active == 0) {
            GET_ANIM_POS(obj, &x, &y, &w, &h);
            explosion->anim_frame = 0;
            explosion->x = x + (w >> 1) - explosion->offset_bx;
            explosion->is_active = 1;
            explosion->y = y + h - explosion->offset_by;
            explosion->flags.alive = 1;
            add_alwobj(explosion);
            return;
        }
    }
}

void AllocateBonus(obj_t *obj, s16 type, u8 sub_etat, u8 main_etat)
{
    s16 i;
    obj_t *bonus;

    for (i = 0; i < level.nb_objects; i++) {
        bonus = level.objects + i;
        if (bonus->type == type && bonus->is_active == 0) {
            bonus->x = obj->x + obj->offset_bx - bonus->offset_bx;
            bonus->y = obj->y + obj->offset_by - bonus->offset_by;
            set_main_and_sub_etat(bonus, sub_etat, main_etat);
            bonus->flags.alive = 1;
            bonus->is_active = 1;
            bonus->speed_x = 0;
            bonus->display_prio = 1;
            bonus->active_timer = 0;
            bonus->active_flag = 0;
            bonus->speed_y = bonus->y + bonus->offset_bx - ymap < 100 ? 2 : 1;
            return;
        }
    }
}

void bonus_fall(obj_t *obj)
{
    s16 type;
    u8 sub_etat;
    u8 main_etat;
    u8 state;

    BonusMode = 1;
    if (status_bar.max_hitp != 4) {
        type = 0x52;
        sub_etat = 6;
        main_etat = 2;
    } else {
        state = poing_obj->init_sub_etat;
        if (state == 1 || state == 3 || state == 5) {
            type = 0x5f;
            sub_etat = 5;
            main_etat = 7;
        } else if (state == 12) {
            type = 0xa1;
            sub_etat = 5;
            main_etat = (u8)(myRand(7) + 24);
        } else {
            type = 0x5f;
            sub_etat = 5;
            main_etat = 14;
        }
    }
    AllocateBonus(obj, type, sub_etat, main_etat);
}

u8 collision_briques(obj_t *obj)
{
    s16 i;
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    s16 abs_x;
    s16 abs_y;
    s16 old_param;
    s16 speed_sign_x;
    s16 speed_sign_y;
    u8 reflect_x;
    u8 reflect_y;
    obj_t *brick;

    x = (s16)(obj->x - xmap + obj->offset_bx + 8);
    y = (s16)(obj->y - ymap + obj->offset_by);
    nb_collis = 0;
    reflect_x = 0;
    reflect_y = 0;
    for (i = 0; i < 20; i++) {
        if (nb_collis >= 1)
            break;
        brick = level.objects + CasseBrique[i].object_index;
        if (brick->param != 0 && i != LastTouched) {
            dx = x - (s16)(brick->x - xmap + brick->offset_bx + 8);
            abs_x = (s16)ABS(dx);
            if (abs_x <= 21) {
                dy = y - (s16)(brick->y - ymap + brick->offset_by);
                abs_y = (s16)ABS(dy);
                if (abs_y <= 21 && dx * dx + dy * dy <= dist2) {
                    LastTouched = (u8)i;
                    if (brick->param == 1) {
                        brick->param = 2;
                        undestroy = 0;
                        no_sound = 0xc1;
                        NbBrikTouched++;
                    } else if (brick->param == 2) {
                        brick->param = 3;
                        undestroy = 0;
                        no_sound = 0xc1;
                        NbBrikTouched++;
                    } else if (brick->param == 3 || brick->param == 6) {
                        old_param = brick->param;
                        DO_NOVA(brick);
                        NbBrikTouched++;
                        if (NbClassicBrik != 0)
                            NbClassicBrik--;
                        if (old_param == 6)
                            bonus_fall(brick);
                        brick->param = 0;
                        NbBrikTouched = 0;
                        no_sound = 0xe;
                    } else if (brick->param == 4) {
                        no_sound = 0xd6;
                        undestroy++;
                        brick->hit_points--;
                        if (brick->hit_points == 0) {
                            DO_NOVA(brick);
                            brick->param = 0;
                            no_sound = 0xe;
                        }
                    } else if (brick->param == 5) {
                        speed_up_ball(obj);
                        undestroy++;
                        no_sound = 0x36;
                    } else if (brick->param == 7) {
                        tempo = 1;
                        CBoffsetY -= 5;
                        undestroy++;
                        no_sound = 0x36;
                    }

                    speed_sign_x = speed_ball_x > 0 ? 1 : (speed_ball_x < 0 ? -1 : 0);
                    speed_sign_y = speed_ball_y > 0 ? 1 : (speed_ball_y < 0 ? -1 : 0);
                    if (speed_sign_x == (dx > 0 ? 1 : (dx < 0 ? -1 : 0)))
                        reflect_x = 1;
                    if (speed_sign_y == (dy > 0 ? 1 : (dy < 0 ? -1 : 0)))
                        reflect_y = 1;
                    nb_collis++;
                    if (nb_collis > 20)
                        nb_collis = 20;
                }
            }
        }
    }

    if (nb_collis != 0) {
        if (reflect_x)
            invert_speed_ball_x(obj);
        if (reflect_y)
            invert_speed_ball_y(obj);
        PlaySnd_old(no_sound);
    }
    return nb_collis;
}

u8 collision_rayman(obj_t *obj)
{
    s16 ball_x;
    s16 ball_y;
    s16 ray_x;
    s16 ray_y;
    s16 ray_w;
    s16 ray_h;
    s16 dx;
    s16 dy;
    u8 state;

    calc_obj_pos(obj);
    ball_x = obj->screen_x + obj->offset_bx - 8;
    ball_y = obj->screen_y + obj->offset_by - 8;
    if (ray.main_etat == 0 && ray.sub_etat == 4) {
        ray_x = ray.screen_x - 0x16;
        ray_y = ray.screen_y + 0x20;
        ray_w = 0x20;
        ray_h = 0x2e;
        state = 0x19;
    } else if (ray.main_etat == 0 && ray.sub_etat == 0x14) {
        ray_x = ray.screen_x - 0x26;
        ray_y = ray.screen_y + 0x4a;
        ray_w = 0x20;
        ray_h = 0x4e;
        state = 0x1a;
    } else {
        ray_x = ray.screen_x;
        ray_y = ray.screen_y;
        ray_w = 0x2e;
        ray_h = 0x2e;
        state = 0;
    }
    if (state == 0 || !inter_box(ball_x, ball_y, 0x10, 0x10,
                                  ray_x, ray_y, ray_w, ray_h))
        return 0;

    if (speed_ball_y < 0)
        return 0;
    if (NbClassicBrik <= 5 && NoCB != 3)
        speed_ball_level = 1;
    else
        speed_ball_level = 0;
    dx = (s16)(ball_x + 8 - (ray_x + 16));
    dy = (s16)(ball_y + 8 - (ray_y + 16));
    speed_ball_angle = angle_direction(dx, dy);
    get_speed_ball();
    AllocateCasseBriqueBoum(obj);
    PlaySnd_old(0x2c);
    if (state == 0x19)
        set_main_and_sub_etat(&ray, 3, 0x32);
    LastTouched = state;
    return 1;
}

void move_briques(obj_t *obj)
{
    s16 y;

    (void)obj;
    if (InvadersMode != 1)
        return;

    tempo--;
    if (tempo == 0) {
        tempo = 500;
        y = CBoffsetY + Ydir;
        if (y >= CBoffsetYMax) {
            CBoffsetY = CBoffsetYMax;
            Xdir = 0;
            Ydir = 0;
        } else if (y <= CBoffsetYMin) {
            CBoffsetY = CBoffsetYMin;
            Xdir = 0;
            Ydir = 1;
        } else {
            CBoffsetY = y;
            Xdir = 0;
            Ydir = 1;
        }
    }
}

void init_briques(obj_t *obj)
{
    s16 i;
    s16 level_index;
    s16 x;
    s16 y;
    u8 kind;
    s16 min_x;
    s16 max_x;
    s16 min_y;
    s16 max_y;
    obj_t *brick;
    u8 brick_index;

    BonusMode = 0;
    NbClassicBrik = 0;
    NbBrikTouched = 0;
    LastTouched = 0;
    for (i = 0; i < 20; i++) {
        CasseBrique[i].x = 0;
        CasseBrique[i].y = 0;
        CasseBrique[i].object_index = 0;
        CasseBrique[i].kind = 0;
    }
    obj->iframes_timer = 100;

    max_x = -320;
    min_x = 320;
    max_y = -200;
    min_y = 200;
    brick_index = 0;
    for (level_index = 0; level_index < level.nb_objects && brick_index < 20; level_index++) {
        brick = level.objects + level_index;
        if (brick->type == 0x100) {
            if (!get_brik_info(obj, brick_index, &x, &y, &kind)) {
                x = 0;
                y = 128;
                kind = 0;
            }
            CasseBrique[brick_index].x = x + 0x2a;
            CasseBrique[brick_index].y = y + 9;
            CasseBrique[brick_index].object_index = level_index;
            CasseBrique[brick_index].kind = kind;
            if (x + 0x2a > max_x)
                max_x = x + 0x2a;
            if (x + 0x2a < min_x)
                min_x = x + 0x2a;
            if (y + 9 > max_y)
                max_y = y + 9;
            if (y + 9 < min_y)
                min_y = y + 9;
            brick_index++;
        }
    }

    CBoffsetXMin = min_x > 12 ? 0 : 12 - min_x;
    CBoffsetXMax = max_x > 0x124 ? 0 : 0x124 - max_x;
    CBoffsetYMin = min_y > 8 ? 0 : 8 - min_y;
    CBoffsetYMax = max_y > 0x78 ? 0 : 0x78 - max_y;
    CBoffsetX = CasseBriqueOffsetX[NoCB];
    CBoffsetY = 0;
    if (GameModeVideo == 1) {
        CBoffsetX -= 8;
        if (byte_93fa20 == GameModeVideo)
            CBoffsetY = CasseBriqueOffsetX[NoCB];
    }
    LIMITE_GAUCHE = 0x38 - ray.offset_bx;
    XOFFSETCBRIGHT = 0x108 - ray.offset_bx;
    XOFFSETRAY = -8;
    LIMITE_DROITE = 0x108 - ray.offset_bx - 8;
    YOFFSETCB = 0x20;
    XOFFSETCBLEFT = 0x20;
    NoCBInit = 0;
    NbClassicBrik = 0;

    for (i = 0; i < 20; i++) {
        brick = level.objects + CasseBrique[i].object_index;
        brick->x = xmap + CBoffsetX + CasseBrique[i].x - brick->offset_bx;
        brick->y = ymap + CBoffsetY + CasseBrique[i].y - brick->offset_by;
        brick->is_active = 1;
        brick->flags.alive = 1;
        brick->param = CasseBrique[i].kind;
        brick->iframes_timer = i;
        add_alwobj(brick);
        brick->display_prio = 4;
        brick->speed_x = 0;
        brick->speed_y = 0;
        if (brick->param == 4)
            brick->hit_points = 20;
        if (brick->param == 1)
            NbClassicBrik++;
        else if (brick->param == 2)
            InvadersMode = 2;
        else if (brick->param == 3)
            brick->hit_points = 20;
        frame_briques(brick);
    }
    NbBrikTouched = 0;
    upscreen = 0;
}

void reset_briques(void)
{
    u8 i;

    for (i = 0; i < 20; i++) {
        (level.objects + CasseBrique[i].object_index)->flags.alive = 0;
        (level.objects + CasseBrique[i].object_index)->is_active = 0;
    }
}

void casse_brique_init(obj_t *obj)
{
    s16 angle;

    ray_mode = 5;
    exit_casse_brique();
    enter_casse_brique2();
    if (GameModeVideo == 0)
        xmap = 0;
    else
        xmap = -5;
    scroll_start_x = xmap;
    scroll_end_x = xmap;

    CBoffsetY = 0;
    CBoffsetX = CasseBriqueOffsetX[NoCB];
    Xdir = 0;
    Ydir = 0;
    YOFFSETCB = 0;
    if (GameModeVideo == 0) {
        xmap += 8;
        LIMITE_GAUCHE = 0x38 - ray.offset_bx;
        XOFFSETCBRIGHT = 0x100 - ray.offset_bx;
        XOFFSETCBLEFT = 0x20;
        LIMITE_DROITE = 0xf8 - ray.offset_bx;
        XOFFSETRAY = -8;
    } else {
        ymap -= 8;
        if (byte_93fa20 == 1) {
            CBoffsetY = CasseBriqueOffsetY[NoCB];
            YOFFSETCB = 10;
        }
        XOFFSETRAY = -8;
        XOFFSETCBRIGHT = 0x18;
        XOFFSETCBLEFT = 0x28;
        LIMITE_GAUCHE = 0x28 - ray.offset_bx;
        LIMITE_DROITE = 0xf8 - ray.offset_bx;
    }

    obj->display_prio = 0;
    speed_ball_null(obj);
    ray.y = ymap + 0xc0 - ray.offset_by;
    if (num_world == 2) {
        angle = myRand(obj->animations[0].frames_count - 1);
        NoCB = angle;
        NoCBInit = angle;
        obj->hit_points = 3;
        init_briques(obj);
        set_main_and_sub_etat(&ray, 3, 2);
    } else if (num_world == 0x32) {
        obj->hit_points = 1;
        obj->iframes_timer = 0;
        set_main_and_sub_etat(&ray, 0, 1);
    }

}

void casse_brique_start(obj_t *obj)
{
    s16 x;
    s16 y;
    s16 w;
    s16 h;

    Ydir = 0;
    Xdir = 0;
    if (!((ray.main_etat == 0 && ray.sub_etat == 4) ||
          (ray.main_etat == 1 && ray.sub_etat == ray.main_etat)))
        return;

    speed_ball_null(obj);
    GET_SPRITE_POS(&ray, 1, &x, &y, &w, &h);
    if (ray.flags.flip_x)
        x = 0;
    else
        x -= 2;
    set_ball_pos(obj, x - obj->offset_bx, y + h - obj->offset_by);
    if (obj->display_prio == 0) {
        DO_NOVA(obj);
        PlaySnd_old(0xc2);
    }
    obj->display_prio = 2;
    obj->is_active = 1;
    obj->flags.alive = 1;
    if (but0pressed()) {
        speed_ball_init(obj);
        set_main_and_sub_etat(obj, 0, 2);
        set_main_and_sub_etat(&ray, 3, 0x32);
        if (InvadersMode == 2) {
            InvadersMode = 1;
            Xdir = 0;
            Ydir = 1;
        } else {
            InvadersMode = 0;
        }
        LastTouched = 0x14;
    }
}

u8 ray_face_brik(s16 x)
{
    s16 delta;

    delta = x - (ray.screen_x + ray.offset_bx);
    if ((ray.flags.flip_x && delta >= 0) ||
        (!ray.flags.flip_x && delta <= 0))
        return 1;
    return 0;
}

void casse_brique_play(obj_t *obj)
{
    s16 ball_x_screen;
    s16 ball_y_screen;
    s16 ray_x;
    s16 ray_y;
    s16 ray_w;
    s16 ray_h;

    if (NbClassicBrik == 0) {
        obj->display_prio = 0;
        speed_ball_null(obj);
        obj->iframes_timer--;
        if (obj->iframes_timer == 0) {
            reset_briques();
            NoCB++;
            if (NoCB == NoCBInit) {
                obj->iframes_timer = 100;
                set_main_and_sub_etat(obj, 0, 4);
                PlaySnd_old(0x6b);
            } else {
                init_briques(obj);
                set_main_and_sub_etat(obj, 0, 1);
                PlaySnd_old(0x69);
            }
        }
        return;
    }

    ball_x_screen = (s16)(obj->x - xmap + obj->offset_bx + 8);
    ball_y_screen = (s16)(obj->y - ymap + obj->offset_by);
    if (ball_x_screen + 8 >= LIMITE_DROITE) {
        if (LastTouched != 0x16) {
            invert_speed_ball_x(obj);
            LastTouched = 0x16;
            undestroy = 0;
            PlaySnd_old(0x2d);
        }
        return;
    }
    if (ball_x_screen - 8 <= LIMITE_GAUCHE) {
        if (LastTouched != 0x15) {
            invert_speed_ball_x(obj);
            LastTouched = 0x15;
            undestroy = 0;
            PlaySnd_old(0x2d);
        }
        move_ball(obj);
        do_bonus();
        return;
    }
    if (ball_y_screen - 8 <= YOFFSETCB) {
        if (LastTouched != 0x17) {
            invert_speed_ball_y(obj);
            if (undestroy != 0)
                undestroy++;
            LastTouched = 0x17;
            PlaySnd_old(0x2d);
            upscreen = 1;
        }
        move_ball(obj);
        do_bonus();
        return;
    }
    if (ball_y_screen + 8 > 0xc8 - XOFFSETCBLEFT) {
        if (!ray_face_brik(ball_x_screen) || ray.speed_x == 0) {
            set_main_and_sub_etat(obj, 0, 3);
            obj->display_prio = 0;
            speed_ball_null(obj);
            if (InvadersMode == 1)
                InvadersMode = 2;
            move_ball(obj);
            do_bonus();
            return;
        }
        set_main_and_sub_etat(&ray, 3, 0x11);
        move_ball(obj);
        do_bonus();
        return;
    }

    if (collision_briques(obj) == 0)
        collision_rayman(obj);
    GET_SPRITE_POS(&ray, 1, &ray_x, &ray_y, &ray_w, &ray_h);
    (void)ray_x;
    (void)ray_y;
    (void)ray_w;
    (void)ray_h;
    move_ball(obj);
    do_bonus();
}

void casse_brique_end(obj_t *obj)
{
    obj_t *brick;

    if (BonusMode == 2) {
        brick = level.objects + CasseBrique[BonusBrik].object_index;
        brick->param = BonusBrikType;
    }
    BonusMode = 0;
    obj->hit_points--;
    if (obj->hit_points == 0) {
        obj->iframes_timer = 100;
        set_main_and_sub_etat(obj, 0, 4);
        PlaySnd_old(0x16);
    } else {
        set_main_and_sub_etat(obj, 0, 1);
        PlaySnd_old(0xf);
    }
}

void casse_brique_done(obj_t *obj)
{
    obj->display_prio = 0;
    speed_ball_null(obj);
    Ydir = 0;
    obj->iframes_timer--;
    Xdir = 0;
    if (obj->iframes_timer == 0) {
        if (obj->hit_points != 0)
            Add_One_RAY_lives();
        ray_mode = 1;
        reset_briques();
        obj->is_active = 0;
        obj->flags.alive = 0;
        set_main_and_sub_etat(&ray, 3, 2);
        new_world = 1;
        exit_casse_brique();
        exit_casse_brique2();
        DO_FADE_OUT();
    }
}

void DISPLAY_FIXE_CB(obj_t *obj)
{
    s16 i;
    s16 x;

    if (obj->hit_points > 1) {
        x = 0xc8;
        if (GameModeVideo == 1 && byte_93fa20 == GameModeVideo)
            x = 0xc0;
        for (i = 1; i < obj->hit_points; i++)
            display_sprite(obj, 0x0b, x - i * 20, 11, 1);
    }

    if (NbBrikTouched <= 20) {
        display_sprite(obj, 0x39, 0xa0, 0xaf + NbBrikTouched / 10, 1);
        display_sprite(obj, 0x39, 0xa0, 0x27 + NbBrikTouched / 10, 1);
    }

    if (obj->sub_etat == 1)
        display_text("BRICKS", 0xa0, 0x64, 0, 0);
    else if (obj->sub_etat == 4)
        display_text("GAME OVER", 0xa0, 0x64, 0, 0);
}

void DO_CB_BALL_COMMAND(obj_t *obj)
{
    u8 sub_etat;

    if (TOUCHE(1) && cbout == 0) {
        obj->hit_points = 0;
        cbout = 1;
        obj->iframes_timer = 100;
        set_main_and_sub_etat(obj, 0, 4);
        PlaySnd_old(0x16);
    }
    if (obj->main_etat != 0)
        return;
    sub_etat = obj->sub_etat;
    if (sub_etat == 10 || sub_etat > 4)
        return;
    switch (sub_etat) {
    case 0: casse_brique_init(obj); break;
    case 1: casse_brique_start(obj); break;
    case 2: casse_brique_play(obj); break;
    case 3: casse_brique_end(obj); break;
    case 4: casse_brique_done(obj); break;
    }
}

void DO_CB_BRIK_COMMAND(obj_t *obj)
{
    move_briques(obj);
    frame_briques(obj);
}

void DO_RAY_CASSE_BRIQUE(void)
{
    s16 speed;
    u8 can_steer;

    can_steer = (u8)((ray.main_etat == 0 && ray.sub_etat == 4) ||
                     (ray.main_etat == 1 && ray.sub_etat == ray.main_etat));
    if (can_steer && rightjoy()) {
        if (!ray.flags.flip_x) {
            ray.flags.flip_x = 1;
            ray.speed_x = -ray.speed_x;
        }
        if (ray.main_etat == 0 && ray.sub_etat == 4) {
            set_main_and_sub_etat(&ray, 1, 0x10);
            ray.speed_x = 0x10;
        } else {
            speed = ray.speed_x + 1;
            if (speed > 0x2f)
                speed = 0x2f;
            ray.speed_x = speed;
        }
    } else if (can_steer && leftjoy()) {
        if (ray.flags.flip_x) {
            ray.flags.flip_x = 0;
            ray.speed_x = -ray.speed_x;
        }
        if (ray.main_etat == 0 && ray.sub_etat == 4) {
            set_main_and_sub_etat(&ray, 1, (u8)-16);
            ray.speed_x = -0x10;
        } else {
            speed = ray.speed_x - 1;
            if (speed < -0x2f)
                speed = -0x2f;
            ray.speed_x = speed;
        }
    } else if (can_steer && ray.speed_x > 0) {
        ray.speed_x--;
    } else if (can_steer && ray.speed_x < 0) {
        ray.speed_x++;
    }

    if ((ray.main_etat == 3 && ray.sub_etat == 0x12) ||
        (ray.main_etat == 0 && ray.sub_etat == 0x14)) {
        SET_X_SPEED(&ray);
        if (but0pressed())
            set_main_and_sub_etat(&ray, 0, 4);
    } else if (ray.main_etat == 3 && ray.sub_etat == 0x11) {
        SET_X_SPEED(&ray);
    }

    if (ray.x <= LIMITE_GAUCHE && ray.speed_x < 0) {
        ray.x = LIMITE_GAUCHE;
        ray.speed_x = 0;
    } else if (ray.x >= LIMITE_DROITE && ray.speed_x > 0) {
        ray.x = LIMITE_DROITE;
        ray.speed_x = 0;
    } else {
        ray.x += instantSpeed(ray.init_y);
    }
    GET_RAY_ZDC(&ray, &ray_zdc_x, &ray_zdc_y, &ray_zdc_w, &ray_zdc_h);
    DO_COLLISIONS();
    DO_ANIM(&ray);
}
