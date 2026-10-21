/* breakout.c (DOS v1.21, 1:16a20..1:18b3a; object padding to 1:18b40).
 * Worker c2_breakout candidate; statuses and unresolved hypotheses in NOTES.md.
 * Function/global names RAYVERSE/ANDROID; descriptive local names INVENTED.
 */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"
#include <stdio.h>
#include <stdlib.h>
extern s32 map_time;
extern s16 scroll_start_y, scroll_end_y, num_level, sbar_obj_id;
extern u8 nb_max_collis;
typedef struct cb_collision_t {
    s16 dx, dy;
    u8 reflect_x, reflect_y, index, reserved;
} cb_collision_t;
extern cb_collision_t CBCollis[1];
extern char *language_txt[300];
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
extern u8 P486;
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
extern s16 inter_box(s16 x1, s16 y1, s16 w1, s16 h1,
    s16 x2, s16 y2, s16 w2, s16 h2);
extern void DO_NOVA(obj_t *obj);
extern void add_alwobj(obj_t *obj);
extern s16 but0pressed(void);
extern s16 rightjoy(void);
extern s16 leftjoy(void);
extern void SET_X_SPEED(obj_t *obj);
extern s16 instantSpeed(s16 speed);
extern void GET_RAY_ZDC(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
extern void DO_COLLISIONS(void);
extern void DO_ANIM(obj_t *obj);
extern void display_sprite(obj_t *obj, u8 sprite_index, s16 x, s16 y, u8 flipped);
extern void display_text(const char *text, s16 x, s16 y, u8 font_size, u8 color);
void AllocateBonus(obj_t *obj, u16 type, u8 sub_etat, u8 main_etat);
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
    s16 index;

    if (speed_ball_angle < 0)
        speed_ball_angle += 72;
    else if (speed_ball_angle > 72)
        speed_ball_angle -= 72;

    if (18 - speed_ball_angle >= 0) {
        speed_ball_x = CBdata[speed_ball_angle][speed_ball_level * 2 + 1];
        speed_ball_y = CBdata[speed_ball_angle][speed_ball_level * 2 + 2];
    /* DOS 1:16b3b and 1:16ba9 test the reflected index as a signed word. */
    } else if ((index = 36 - speed_ball_angle) >= 0) {
        speed_ball_x = -CBdata[index][speed_ball_level * 2 + 1];
        speed_ball_y = CBdata[index][speed_ball_level * 2 + 2];
    } else if (54 - speed_ball_angle >= 0) {
        speed_ball_x = -CBdata[(s16)(speed_ball_angle - 36)][speed_ball_level * 2 + 1];
        speed_ball_y = -CBdata[(s16)(speed_ball_angle - 36)][speed_ball_level * 2 + 2];
    } else if ((index = 72 - speed_ball_angle) >= 0) {
        speed_ball_x = CBdata[index][speed_ball_level * 2 + 1];
        speed_ball_y = -CBdata[index][speed_ball_level * 2 + 2];
    }
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
    s16 ratio;
    s16 angle;
    u8 index;

    if (x != 0) {
        ratio = (y << 9) / x;
        if (ratio < 0)
            ratio = -ratio;
        index = 17;
        while (ratio < -(u16)CBdata[index][0])
            index--;
        angle = index;
    } else {
        angle = y != 0 ? 18 : 0;
    }
    if (x < 0)
        angle = 36 - angle;
    if (y > 0)
        angle = 72 - angle;
    return angle;
}

void move_ball(obj_t *obj)
{
    s16 dx;
    s16 dy;

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
    obj->x = (s16)(ball_x ? ball_x >> 8 : -((-ball_x) >> 8));
    obj->y = (s16)(ball_y ? ball_y >> 8 : -((-ball_y) >> 8));
}

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

u16 get_brik_info(obj_t *obj, s16 entry_index, s16 *x, s16 *y, u8 *kind)
{
    u8 *entry;
    anim_layer_t **layers;
    sprite_t *sprite;
    u16 anim_index;
    u8 result;
    u8 sprite_index;

    anim_index = obj->animations[2].layers_per_frame & 0x3fff;
    layers = &obj->animations[2].layers;
    entry = (u8 *)*layers + anim_index * NoCB * 4 + entry_index * 4;
    sprite_index = entry[3];
    sprite = obj->sprites + sprite_index;
    if (sprite_index != 0 && sprite->id != 0) {
        *x = entry[1];
        *y = entry[2];
        /* DOS table 1:16efc..16f28; bodies 1:16fca..16fe6. */
        switch (sprite->id) {
        case 0x28: *kind = 2; break;
        case 0x27: *kind = 1; break;
        case 0x29: *kind = 3; break;
        case 0x2b: *kind = 4; break;
        case 0x2a: *kind = 5; break;
        case 0x32: *kind = 7; break;
        default: *kind = 0; break;
        }
        result = 1;
    } else result = 0;
    return result;
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
    /* DOS table 1:17014/17018 maps kind 7 to frame 5, kind 6 to animation. */
    case 7:
        obj->anim_frame = 5;
        break;
    case 6:
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
    s16 h;
    s16 x;
    s16 y;
    s16 w;
    s16 i;
    s16 count;
    obj_t *explosion;

    explosion = level.objects;
    count = level.nb_objects;
    for (i = 0; i < count; i++, explosion++) {
        if (explosion->type == 0xb && explosion->is_active == 0) {
            GET_ANIM_POS(obj, &x, &y, &w, &h);
            explosion->anim_frame = 0;
            explosion->x = x + (w >> 1) - explosion->offset_bx;
            explosion->y = y + h - explosion->offset_by;
            explosion->flags.alive = 1;
            explosion->is_active = 1;
            add_alwobj(explosion);
            return;
        }
    }
}

void AllocateBonus(obj_t *obj, u16 type, u8 sub_etat, u8 main_etat)
{
    s16 i;
    obj_t *bonus;

    bonus = level.objects;
    for (i = 0; i < level.nb_objects; i++, bonus++) {
        if (type == bonus->type && bonus->is_active == 0) {
            bonus->x = obj->x + obj->offset_bx - bonus->offset_bx;
            bonus->y = obj->y + obj->offset_by - bonus->offset_by;
            set_main_and_sub_etat(bonus, sub_etat, main_etat);
            bonus->flags.alive = 1;
            bonus->is_active = 1;
            bonus->speed_x = 0;
            bonus->display_prio = 1;
            bonus->active_timer = 0;
            bonus->active_flag = 0;
            bonus->speed_y = (bonus->y + bonus->offset_bx - ymap < 100) + 1;
            return;
        }
    }
}

void bonus_fall(obj_t *obj)
{
    u16 type;
    u8 sub_etat;
    u8 main_etat;

    BonusMode = 1;
    if (status_bar.max_hitp != 4) {
        type = 0x52;
        sub_etat = 6;
        main_etat = 2;
    } else {

        if (poing_obj->init_sub_etat == 1 || poing_obj->init_sub_etat == 3 || poing_obj->init_sub_etat == 5) {
            type = 0x5f;
            sub_etat = 5;
            main_etat = 7;
        } else if (poing_obj->init_sub_etat != 12) {
            type = 0x5f;
            sub_etat = 5;
            main_etat = 14;
        } else {
            type = 0xa1;
            sub_etat = 5;
            main_etat = (u8)(myRand(7) + 24);
        }
    }
    AllocateBonus(obj, type, sub_etat, main_etat);
}

u8 collision_briques(obj_t *obj)
{
    u8 i;
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    s16 abs_x;
    s16 abs_y;
    u8 reflect_x;
    u8 reflect_y;
    obj_t *brick;

    x = (s16)obj->x - xmap + obj->offset_bx + 8;
    y = (s16)obj->y - ymap + obj->offset_by;
    nb_collis = 0;
    reflect_x = 0;
    reflect_y = 0;
    for (i = 0; i < 20; i++) {
        if (nb_collis >= 1)
            break;
        brick = level.objects + CasseBrique[i].object_index;
        if (brick->param != 0 && i != LastTouched) {
            dx = x - (s16)((s16)brick->x - xmap + brick->offset_bx + 8);
            abs_x = (s16)abs(dx);
            if (abs_x <= 21) {
                dy = y - (s16)((s16)brick->y - ymap + brick->offset_by);
                abs_y = (s16)abs(dy);
                if (abs_x <= 21 && dy * dy + dx * dx <= dist2) {
                    LastTouched = (u8)i;
                    switch (brick->param) {
                    case 1:
                        brick->param = 2;
                        undestroy = 0;
                        no_sound = 193;
                        NbBrikTouched++;
                        break;
                    case 2:
                        undestroy = 0;
                        brick->param = 3;
                        no_sound = 193;
                        NbBrikTouched++;
                        break;
                    case 3:
                    case 6:
                        DO_NOVA(brick);
                        NbBrikTouched++;
                        NbClassicBrik--;
                        if (brick->param == 6) bonus_fall(brick);
                        brick->param = 0;
                        undestroy = 0;
                        no_sound = 14;
                        break;
                    case 4:
                        no_sound = 214;
                        undestroy++;
                        brick->hit_points--;
                        if (brick->hit_points == 0) {
                            DO_NOVA(brick);
                            brick->param = 0;
                            undestroy = 0;
                            no_sound = 14;
                        }
                        break;
                    case 5:
                        speed_up_ball(obj);
                        no_sound = 54;
                        undestroy++;
                        break;
                    case 7:
                        tempo = 1;
                        no_sound = 54;
                        CBoffsetY -= 5;
                        undestroy++;
                        break;
                    }
                    CBCollis[nb_collis].reflect_x = 0;
                    CBCollis[nb_collis].reflect_y = 0;
                    if (abs_y <= abs_x) {
                        if (sgn(speed_ball_x) == sgn(dx)) {
                            reflect_y++;
                            CBCollis[nb_collis].reflect_y = 1;
                        } else {
                            reflect_x++;
                            CBCollis[nb_collis].reflect_x = 1;
                        }
                    }
                    if (abs_x <= abs_y) {
                        if (sgn(speed_ball_y) == sgn(dy)) {
                            reflect_x++;
                            CBCollis[nb_collis].reflect_x = 1;
                        } else {
                            CBCollis[nb_collis].reflect_y = 1;
                            reflect_y++;
                        }
                    }
                    CBCollis[nb_collis].dx = dx;
                    CBCollis[nb_collis].dy = dy;
                    CBCollis[nb_collis].index = i;
                    nb_collis++;
                    if (nb_collis > nb_max_collis) nb_max_collis = nb_collis;
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

void collision_rayman(obj_t *obj)
{
    s16 ball_x, ball_y, ray_x, ray_y, ray_w, ray_h;
    u8 state;
    state = 0;
    calc_obj_pos(obj);
    ball_x = obj->screen_x + obj->offset_bx - 8;
    ball_y = obj->screen_y + obj->offset_by - 8;
    ray_w = 46;
    ray_h = 2;
    calc_obj_pos(&ray);
    ray_x = ray.offset_bx + ray.screen_x;
    ray_y = ray.screen_y;
    if ((ray.main_etat == 0 && ray.sub_etat == 4) ||
        (ray.main_etat == 1 && ray.main_etat == ray.sub_etat)) {
        ray_x -= 22;
        ray_y += 32;
        if (LastTouched != 25) state = 25;
    } else if ((ray.main_etat == 0 && ray.sub_etat == 20) ||
        (ray.main_etat == 3 && ray.sub_etat == 17) ||
        (ray.main_etat == 3 && ray.sub_etat == 18)) {
        ray_w += 32;
        ray_x -= 38;
        ray_y += 74;
        if (LastTouched != 26) state = 26;
    }
    if (state != 0 && inter_box(ball_x, ball_y, 16, 16, ray_x, ray_y, ray_w, ray_h) && speed_ball_y >= 0) {
        if (NbClassicBrik <= 5 && NoCB != 3) speed_ball_level = 1;
        else speed_ball_level = 0;
        ball_x = ball_x + 16 - ray_x;
        ray_w += 15;
        speed_ball_angle = (s16)((-120 * ball_x / ray_w + 150.0) * 0.2);
        get_speed_ball();
        AllocateCasseBriqueBoum(obj);
        PlaySnd_old(44);
        if (state == 25) set_main_and_sub_etat(&ray, 3, 50);
        LastTouched = state;
        undestroy = 0;
    }
}

void move_briques(obj_t *obj)
{
    (void)obj;
    if (InvadersMode == 1) {
        tempo--;
        if (tempo == 0) {
            CBoffsetY += Ydir;
            tempo = 500;
            if (CBoffsetY >= CBoffsetYMax) {
                CBoffsetY = CBoffsetYMax;
                Xdir = 0;
                Ydir = 0;
            } else if (CBoffsetY <= CBoffsetYMin) {
                CBoffsetY = CBoffsetYMin;
                Xdir = 0;
                Ydir = 1;
            } else {
                Xdir = 0;
                Ydir = 1;
            }
        }
    }
}

void init_briques(obj_t *obj)
{
    u8 i;
    s16 level_index;
    s16 x;
    s16 y;
    u8 kind;
    s16 min_x;
    s16 max_x;
    s16 min_y;
    s16 max_y;
    s16 count;
    obj_t *brick;
    u8 brick_index;

    max_x = -320;
    max_y = -200;
    min_x = 320;
    min_y = 200;
    InvadersMode = 0;
    BonusMode = 0;
    BonusBrik = 20;
    for (i = 0; i < 20; i++) {
        CasseBrique[i].x = 0;
        CasseBrique[i].y = 0;
        CasseBrique[i].kind = 0;
        CasseBrique[i].object_index = 0;
    }
    obj->iframes_timer = 100;

    brick = level.objects;
    count = level.nb_objects;
    brick_index = 0;
    for (level_index = 0; level_index < count; level_index++, brick++) {
        if (brick_index >= 20) break;
        if (brick->type == 0x100) {
            if (!get_brik_info(obj, brick_index, &x, &y, &kind)) {
                y = 0;
                x = 128;
                kind = 0;
            }
            CasseBrique[brick_index].x = x + 0x2a;
            CasseBrique[brick_index].y = y + 9;
            CasseBrique[brick_index].kind = kind;
            CasseBrique[brick_index].object_index = level_index;
            if (CasseBrique[brick_index].x > max_x)
                max_x = CasseBrique[brick_index].x;
            else if (CasseBrique[brick_index].x < min_x)
            min_x = CasseBrique[brick_index].x;
            if (CasseBrique[brick_index].y > max_y)
                max_y = CasseBrique[brick_index].y;
            else if (CasseBrique[brick_index].y < min_y)
            min_y = CasseBrique[brick_index].y;
            brick_index++;
        }
    }

    if (min_x <= 12) CBoffsetXMin = 0;
    else CBoffsetXMin = 12 - min_x;
    if (max_x > 0x124) CBoffsetXMax = 0;
    else CBoffsetXMax = 0x124 - max_x;
    if (min_y <= 8) CBoffsetYMin = 0;
    else CBoffsetYMin = 8 - min_y;
    if (max_y > 0x78) CBoffsetYMax = 0;
    else CBoffsetYMax = 0x78 - max_y;
    CBoffsetX = CasseBriqueOffsetX[NoCB];
    CBoffsetY = 0;
    if (GameModeVideo == 1) {
        CBoffsetX -= 8;
        if (P486 == GameModeVideo)
            CBoffsetY = CasseBriqueOffsetX[NoCB];
    }
    NbClassicBrik = 0;

    for (i = 0; i < 20; i++) {
        brick = level.objects + CasseBrique[i].object_index;
        brick->x = xmap + CasseBrique[i].x - brick->offset_bx + CBoffsetX;
        brick->y = ymap + CasseBrique[i].y - brick->offset_by + CBoffsetY;
        brick->is_active = 1;
        brick->param = CasseBrique[i].kind;
        brick->iframes_timer = i;
        brick->flags.alive = 1;
        add_alwobj(brick);
        brick->display_prio = 4;
        brick->speed_x = 0;
        brick->speed_y = 0;
        switch (brick->param) {
        case 1: case 2: case 3: NbClassicBrik++; break;
        case 7: InvadersMode = 2; break;
        case 4: brick->hit_points = 20; break;
        }
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
    ray_mode = 5;
    exit_casse_brique();
    enter_casse_brique2();
    if (GameModeVideo == 0) xmap = 0;
    else xmap = -5;
    scroll_end_x = xmap;
    scroll_start_x = xmap;
    CBoffsetY = 0;
    CBoffsetX = CasseBriqueOffsetX[NoCB];
    YOFFSETCB = 0;
    XOFFSETRAY = 56;
    LIMITE_GAUCHE = 56 - ray.offset_bx - 8;
    ymap = 24;
    scroll_end_y = 24;
    scroll_start_y = 24;
    LIMITE_DROITE = 264 - ray.offset_bx - 8;
    XOFFSETCBLEFT = 32;
    XOFFSETCBRIGHT = 32;
    if (GameModeVideo == 0) xmap += 8;
    else {
        CBoffsetX -= 8;
        if (P486 == 1) {
            CBoffsetY = CasseBriqueOffsetY[NoCB];
            YOFFSETCB = 10;
        }
        XOFFSETRAY = -8;
        XOFFSETCBLEFT -= 8;
        XOFFSETCBRIGHT += 8;
        LIMITE_GAUCHE -= 8;
        LIMITE_DROITE -= 8;
    }
    obj->display_prio = 0;
    speed_ball_null(obj);
    ray.y = ymap + 192 - ray.offset_by;
    if (map_time == 2) {
        NoCB = myRand(obj->animations[2].frames_count - 1);
        NoCBInit = NoCB;
        obj->hit_points = 3;
        init_briques(obj);
        set_main_and_sub_etat(&ray, 3, 1);
    } else if (map_time == 50) set_main_and_sub_etat(obj, 0, 1);
}

void casse_brique_start(obj_t *obj)
{
    s16 y;
    s16 h;
    s16 x;
    s16 w;

    Ydir = 0;
    Xdir = 0;
    if (!((ray.main_etat == 0 && ray.sub_etat == 4) ||
        (ray.main_etat == 1 && ray.main_etat == ray.sub_etat)))
    return;

    speed_ball_null(obj);
    GET_SPRITE_POS(&ray, 1, &x, &y, &w, &h);
    x += (ray.flags.flip_x == 0 ? w - 2 : 0) - obj->offset_by;
    y -= obj->offset_by + 5;
    set_ball_pos(obj, x, y);
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
    s16 x, y;
    if (NbClassicBrik == 0) {
        s16 countdown;
        obj->display_prio = 0;
        countdown = obj->iframes_timer;
        countdown--;
        obj->iframes_timer = countdown;
        speed_ball_null(obj);
        if (obj->iframes_timer == 0) {
            reset_briques();
            NoCB++;
            if (NoCB > obj->animations[2].frames_count - 1) NoCB = 0;
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
    } else {
        x = (s16)obj->x - xmap + obj->offset_bx + 8;
        y = (s16)obj->y - ymap + obj->offset_by;
        if (x + 8 >= 320 - XOFFSETCBRIGHT) {
            if (LastTouched != 22) {
                invert_speed_ball_x(obj);
                LastTouched = 22;
                undestroy = 0;
                PlaySnd_old(45);
            }
        } else if (x - 8 <= XOFFSETCBLEFT) {
            if (LastTouched != 21) {
                invert_speed_ball_x(obj);
                LastTouched = 21;
                undestroy = 0;
                PlaySnd_old(45);
            }
        } else if (y - 8 <= YOFFSETCB) {
            if (LastTouched != 23) {
                invert_speed_ball_y(obj);
                LastTouched = 23;
                if (undestroy != 0) undestroy++;
                PlaySnd_old(45);
                upscreen = 1;
            }
        } else {
            s32 ray_bottom = ray.screen_y + 64;
            if (y + 8 > ray_bottom) {
                if (ray.speed_x != 0 && ray_face_brik(x) &&
                    ((ray.main_etat == 0 && ray.sub_etat == 4) ||
                    (ray.main_etat == 1 && ray.main_etat == ray.sub_etat))) {
                    set_main_and_sub_etat(&ray, 3, 17);
                } else if (200 - YOFFSETCB <= y) {
                    set_main_and_sub_etat(obj, 0, 3);
                    obj->display_prio = 0;
                    speed_ball_null(obj);
                    if (InvadersMode == 1) InvadersMode = 2;
                } else collision_rayman(obj);
            } else if (collision_briques(obj) == 0) {
                collision_rayman(obj);
            }
        }
        move_ball(obj);
        do_bonus();
    }
}

void casse_brique_end(obj_t *obj)
{

    if (BonusMode == 2) {
        (level.objects + CasseBrique[BonusBrik].object_index)->param = BonusBrikType;
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
    s16 y, x;
    s32 icon_x;
    s32 icon_y;
    u8 i;
    obj_t *digits;
    char text[20];
    if (obj->hit_points > 0) {
        y = 320;
        x = 200;
        if (GameModeVideo == 1 && GameModeVideo == P486) {
            y = 317;
            x = 192;
        }
        for (i = 1; i < obj->hit_points; i++)
            display_sprite(obj, 11, y - 28, x - i * 20, 1);
    }
    if (NbClassicBrik <= 20) {
        icon_x = 16;
        icon_y = 175;
        if (GameModeVideo == 1 && GameModeVideo == P486) {
            icon_x = 12;
            icon_y = 169;
        }
        digits = level.objects + sbar_obj_id;
        display_sprite(digits, 57, icon_x - 2, icon_y, 1);
        display_sprite(digits, 28 + NbClassicBrik / 10, icon_x + 39, icon_y, 1);
        display_sprite(digits, 28 + NbClassicBrik % 10, icon_x + 54, icon_y, 1);
    }
    if (obj->main_etat == 0) {
        switch (obj->sub_etat) {
        case 1:
            if ((map_time & 63) < 32) {
                sprintf(text, "/%s/", language_txt[188]);
                display_text(text, 160, 100, 0, 0);
                display_text(language_txt[180], 160, 150, 2, 0);
            }
            break;
        case 4:
            if (cbout != 0) display_text(language_txt[128], 160, 100, 0, 0);
            else if (obj->hit_points == 0) display_text(language_txt[128], 160, 100, 0, 0);
            else display_text(language_txt[157], 160, 100, 0, 0);
            break;
        }
    }
}

void DO_CB_BALL_COMMAND(obj_t *obj)
{

    if (TOUCHE(1) && cbout == 0) {
        obj->hit_points = 0;
        cbout = 1;
        obj->iframes_timer = 100;
        set_main_and_sub_etat(obj, 0, 4);
        PlaySnd_old(0x16);
    }
    if (obj->main_etat == 0) {

        if (obj->sub_etat != 10)
            switch (obj->sub_etat) {
        case 0: casse_brique_init(obj); break;
        case 1: casse_brique_start(obj); break;
        case 2: casse_brique_play(obj); break;
        case 3: casse_brique_end(obj); break;
        case 4: casse_brique_done(obj); break;
        }
    }
}

void DO_CB_BRIK_COMMAND(obj_t *obj)
{
    move_briques(obj);
    frame_briques(obj);
}

void DO_RAY_CASSE_BRIQUE(void)
{
    if ((ray.main_etat == 0 && ray.sub_etat == 4) ||
        (ray.main_etat == 1 && ray.main_etat == ray.sub_etat)) {
        if (rightjoy()) {
            if (!ray.flags.flip_x) {
                ray.flags.flip_x = 1;
                ray.speed_x = -ray.speed_x;
            }
            if (ray.main_etat == 0 && ray.sub_etat == 4) {
                set_main_and_sub_etat(&ray, 1, 1);
                ray.speed_x = 16;
            } else {
                ray.speed_x = min32(47, ray.speed_x + 1);
            }
        } else if (leftjoy()) {
            if (ray.flags.flip_x) {
                ray.flags.flip_x = 0;
                ray.speed_x = -ray.speed_x;
            }
            if (ray.main_etat == 0 && ray.sub_etat == 4) {
                set_main_and_sub_etat(&ray, 1, 1);
                ray.speed_x = -16;
            } else {
                ray.speed_x = max32(-47, ray.speed_x - 1);
            }
        } else {
            if (ray.speed_x > 0) {
                ray.speed_x = max32(0, ray.speed_x - 1);
            }
            else if (ray.speed_x < 0) {
                ray.speed_x = min32(0, ray.speed_x + 1);
            }
            if (ABS((s32)ray.speed_x) < 16) set_main_and_sub_etat(&ray, 0, 4);
        }
    } else if ((ray.main_etat == 3 && ray.sub_etat == 18) ||
        (ray.main_etat == 0 && ray.sub_etat == 20)) {
        SET_X_SPEED(&ray);
        if (but0pressed()) set_main_and_sub_etat(&ray, 0, 4);
    } else if (ray.main_etat == 3 && ray.sub_etat == 17) SET_X_SPEED(&ray);
    if (LIMITE_GAUCHE >= ray.x && ray.speed_x < 0) {
        ray.x = LIMITE_GAUCHE;
        ray.speed_x = 0;
    } else if (LIMITE_DROITE <= ray.x && ray.speed_x > 0) {
        ray.x = LIMITE_DROITE;
        ray.speed_x = 0;
    } else ray.x += instantSpeed(ray.speed_x);
    /* DOS 1:18b07..18b20: stack arg=2:3f876, EBX arg=2:3f862. */
    GET_RAY_ZDC(&ray, &ray_zdc_x, &ray_zdc_h, &ray_zdc_w, &ray_zdc_y);
    DO_COLLISIONS();
    DO_ANIM(&ray);
}
