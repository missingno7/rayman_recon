/* objupdate.c, part 4 (1:53800..1:543f0): special platform. Names RAYVERSE; seeded by tools/seed.py, worker tu_objupd3. */
#include "obj.h"
#include "game.h"
#include "map.h"
#define PC
typedef s32 ObjType;
typedef unsigned char bool;
#define Abs(x) (((s32)(x)) < 0 ? -((s32)(x)) : ((s32)(x)))
#define TYPE_INST_PLAT 25
#define TYPE_CRUMBLE_PLAT 26
#define TYPE_BOING_PLAT 27
#define TYPE_ONOFF_PLAT 28
#define TYPE_AUTOJUMP_PLAT 29
#define TYPE_MOVE_AUTOJUMP_PLAT 34
#define TYPE_BIG_BOING_PLAT 68
#define TYPE_PAILLETTE 75
#define TYPE_DESTROYING_DOOR 76
#define TYPE_BB1_PLAT 115
#define TYPE_CAISSE_CLAIRE 152
#define TYPE_COUTEAU 190
#define TYPE_GOMME 237
#define TYPE_MARK_AUTOJUMP_PLAT 243


extern rayevts_t RayEvts;
extern s16 decalage_en_cours;
extern obj_t ray;
extern u8 button_released;
extern s16 jump_time;
extern s16 num_world;
extern u8 horloge[25];
extern options_jeu_t options_jeu;
bool EOA(obj_t* obj);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void RAY_HIT(bool hurt, obj_t* obj);
void RAY_THROW_FIST(void);
void Reset_air_speed(u8 is_rolling_speed);
void ray_jump(void);
void allocatePaillette(obj_t* obj);
void skipToLabel(obj_t* obj, u8 label, bool jump);
void obj_init(obj_t* obj);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
#include <stdlib.h>
#ifndef MAX_2
#define MAX_2(x, y) if (x < y) x = y;
#endif

extern obj_t *poing_obj;
void set_sub_etat(obj_t* obj, u8 sub_etat);
void DoPlatformPoingCollision(obj_t* obj, s16 sprite);

void DO_SPECIAL_PLATFORM(obj_t* obj) {
    /* 3FFAC 801647AC -O2 -msoft-float */
    s16 obj_type;
    s16 plat_x; s16 plat_y; s16 plat_w; s16 plat_h;
    u8 unk_2;
    u16 test_1;
    s16 unk_1;
    u8 is_rolling_speed;
    if (RayEvts.tiny) unk_1 = 0x100;
    else unk_1 = 0x200;
    is_rolling_speed = Abs(decalage_en_cours) > unk_1;

    switch (obj->type)
    {
        case TYPE_AUTOJUMP_PLAT:
        case TYPE_MOVE_AUTOJUMP_PLAT:
        case TYPE_CAISSE_CLAIRE:
        case TYPE_GOMME:
        case TYPE_MARK_AUTOJUMP_PLAT:
            if (ray.follow_id == obj->id)
            {
                if (obj->iframes_timer != 0)
                    obj->iframes_timer--;
                if (obj->iframes_timer == 0)
                {
                    button_released = 1;
                    switch (ray.main_etat * 0x100 + ray.sub_etat)
                    {
                        case 0xb:
                        case 0xc:
                            RAY_THROW_FIST();
                            break;
                    }

                    set_main_and_sub_etat(&ray, 0, 0);
                    ray_jump();
                    obj->ray_dist = 10000;
                    jump_time = 12;
                    ray.speed_y -= 3;
                    obj_type = obj->type;
                    if (obj_type == TYPE_MARK_AUTOJUMP_PLAT)
                        set_main_and_sub_etat(obj, 0, 17);
                    else if (obj_type == TYPE_CAISSE_CLAIRE)
                    {
                        if (!(obj->main_etat == 0 && obj->sub_etat == 2))
                            set_main_and_sub_etat(obj, 0, 1);
                        else
                            set_main_and_sub_etat(obj, 0, 3);
                    }
                    else if (obj_type == TYPE_GOMME)
                    {
                        if (obj->main_etat == 0 && obj->sub_etat == 0)
                        {
                            allocatePaillette(&ray);
                            skipToLabel(obj, 1, 1);
                        }
                    }
                    GET_SPRITE_POS(obj, obj->follow_sprite, &plat_x, &plat_y, &plat_w, &plat_h);
                    if (ray.x + ray.offset_bx - plat_x < plat_w >> 1) /* TODO: ternary? */
                        decalage_en_cours = -0x0200;
                    else
                        decalage_en_cours = 0x0200;
                }
            }
            else
                obj->iframes_timer = obj->follow_id;
            break;
        case TYPE_INST_PLAT:
            if (ray.follow_id == obj->id)
            {
                if (obj->iframes_timer != 0)
                    obj->iframes_timer--;
                if (obj->iframes_timer == 0)
                {
                    GET_SPRITE_POS(obj, obj->follow_sprite, &plat_x, &plat_y, &plat_w, &plat_h);
                    RAY_HIT(0, NULL);
                    if (ray.x + ray.offset_bx - plat_x < plat_w >> 1)
                        ray.flags.flip_x = 1;
                    else
                        ray.flags.flip_x = 0;
                }
            }
            else
                obj->iframes_timer = obj->follow_id;
            break;
        case TYPE_COUTEAU:
            if (
                    ray.follow_id == obj->id &&
                    obj->main_etat == 0 && obj->sub_etat == 9
                    )
            {
                if (obj->iframes_timer != 0)
                    obj->iframes_timer--;
                if (obj->iframes_timer == 0)
                {
                    button_released = 1;
                    switch (ray.main_etat * 0x100 + ray.sub_etat)
                    {
                        case 0xb:
                        case 0xc:
                            RAY_THROW_FIST();
                            break;
                    }

                    set_main_and_sub_etat(&ray, 0, 0);
                    ray_jump();
                    obj->ray_dist = 10000;
                    jump_time = 12;
                    ray.speed_y -= 4;
                    set_main_and_sub_etat(obj, 0, 10);
                    GET_SPRITE_POS(obj, obj->follow_sprite, &plat_x, &plat_y, &plat_w, &plat_h);
                    if (ray.x + ray.offset_bx - plat_x < plat_w >> 1)
                        decalage_en_cours = -0x0200;
                    else
                        decalage_en_cours = 0x0200;
                }
            }
            else
                obj->iframes_timer = obj->follow_id;
            break;
        case TYPE_CRUMBLE_PLAT:
            if (obj->link != 0) {
                if (
                        (ray.follow_id == obj->id || obj->link != 0x0014) &&
                        --obj->link == 0
                        )
                    set_sub_etat(obj, 11);
            }
            else
            {
                if (obj->param != 0)
                {
                    obj->param--;
                    if (num_world == 1)
                    {
                        if (horloge[2] != 0) /* TODO: ternary? */
                            obj->display_prio = 0;
                        else
                            obj->display_prio = 4;
                    }

                    if (obj->param <= obj->iframes_timer)
                    {
                        if (ray.follow_id == obj->id)
                        {
                            ray.follow_id = -1;
                            set_main_and_sub_etat(&ray, 2, 1);
                            Reset_air_speed(is_rolling_speed);
                            jump_time = 0;
                            ray.link = 0;
                            obj->ray_dist = 10000;
                            ray.speed_y = 0;
                        }
                        obj->flags.follow_enabled = 0;
                    }
                }
                else if (num_world != 1) /* this? or else{} then nest? */
                {
                    if (ray.follow_id == obj->id)
                    {
                        ray.follow_id = -1;
                        set_main_and_sub_etat(&ray, 2, 1);
                        Reset_air_speed(is_rolling_speed);
                        jump_time = 0;
                        ray.link = 0;
                        ray.speed_y = 0;
                        obj->ray_dist = 10000;
                    }

                    if (obj->sub_etat == 6 && EOA(obj))
                        obj_init(obj);

                    if (obj->sub_etat == 20)
                        obj->flags.follow_enabled = 1;
                }
                else
                {
                    obj->is_active = 0;
                    obj->active_flag = 4;//ACTIVE_SPECIAL;
                    obj->display_prio = 4;
                    obj->flags.follow_enabled = 1;
                    if (ray.follow_id == obj->id)
                    {
                        ray.follow_id = -1;
                        set_main_and_sub_etat(&ray, 2, 1);
                        Reset_air_speed(is_rolling_speed);
                        jump_time = 0;
                        ray.link = 0;
                        ray.speed_y = 0;
                        obj->ray_dist = 10000;
                    }
                }
            }
            break;
        case TYPE_BIG_BOING_PLAT:
            unk_2 = 0;
            if (ray.follow_id == obj->id)
            {
                if (obj->main_etat != 2 && obj->follow_id != 0)
                {
                    set_main_and_sub_etat(obj, 2, 4);
                    obj->link = 0;
                    obj->gravity_value_1 = 0;
                    obj->gravity_value_2 = 0;
                    obj->speed_y =
                    obj->iframes_timer =
                            obj->follow_id + 1;
                    obj->follow_id = 0;
                    unk_2 = 1;
                }
                else if (obj->iframes_timer == obj->follow_id + 1)
                    unk_2 = 1;
            }
            if (obj->main_etat == 2)
            {
                if (obj->speed_y < -obj->iframes_timer)
                {
                    set_main_and_sub_etat(obj, 0, 2);
                    obj->iframes_timer = 0;
                    obj->y = obj->init_y;
                    obj->speed_y = 0;

                    if (ray.follow_id == obj->id)
                    {
                        ray.y += obj->init_y - obj->y;
                        switch (ray.main_etat * 0x100 + ray.sub_etat)
                        {
                            case 0xb:
                            case 0xc:
                                RAY_THROW_FIST();
                                break;
                        }

                        set_main_and_sub_etat(&ray, 0, 0);
                        button_released = 1;
                        ray_jump();
                        obj->ray_dist = 10000;
                        jump_time = 12;
                    }
                }
                else
                {
                    test_1 = 1; /* TODO: remove? */
                    if (
                            !unk_2 && ray.follow_id == obj->id &&
                            button_released % 2 == test_1 &&
                            options_jeu.test_fire1()
                            )
                    {
                        ray_jump();
                        if (ray.main_etat == 2)
                            obj->ray_dist = 10000;

                        if (!(obj->speed_y < -2 || obj->speed_y > 2))
                            ray.speed_y -= 2;
                        else if (obj->speed_y < -2) {
                            ray.speed_y += obj->speed_y - 2;
                        }
                    }
                }
            }
            break;
        case TYPE_BOING_PLAT:
            unk_2 = 0;
            if (ray.follow_id == obj->id)
            {
                if (obj->main_etat != 2 && obj->follow_id != 0)
                {
                    set_main_and_sub_etat(obj, 2, 3);
                    obj->link = 0;
                    obj->gravity_value_1 = 0;
                    obj->gravity_value_2 = 0;
                    obj->speed_y =
                    obj->iframes_timer =
                            obj->follow_id + 1;
                    obj->follow_id = 0;
                    unk_2 = 1;
                }
                else if (obj->iframes_timer == obj->follow_id + 1)
                    unk_2 = 1;
            }

            if (obj->main_etat == 2)
            {
                if (obj->speed_y < -obj->iframes_timer)
                {
                    set_main_and_sub_etat(obj, 0, 24);
                    obj->iframes_timer = 0;
                    obj->speed_y = 0;
                    if (ray.follow_id == obj->id)
                    {
                        ray.y += obj->init_y - obj->y;
                        switch (ray.main_etat * 0x100 + ray.sub_etat)
                        {
                            case 0xb:
                            case 0xc:
                                RAY_THROW_FIST();
                                break;
                        }

                        set_main_and_sub_etat(&ray, 0, 0);
                        button_released = 1;
                        ray_jump();
                        obj->ray_dist = 10000;
                        jump_time = 12;
                    }
                    obj->y = obj->init_y;
                }
                else
                {
                    test_1 = 1;
                    if (
                            !unk_2 && ray.follow_id == obj->id &&
                            button_released % 2 == test_1 &&
                            options_jeu.test_fire1()
                            )
                    {
                        ray_jump();
                        if (ray.main_etat == 2)
                            obj->ray_dist = 10000;

                        if (!(obj->speed_y < -2 || obj->speed_y > 2))
                            ray.speed_y -= 2;
                        else if (obj->speed_y < -2) {
                            ray.speed_y += obj->speed_y;
                            MAX_2(ray.speed_y, -7);
                        }
                    }
                }
            }
            break;
        case TYPE_ONOFF_PLAT:
            if (obj->sub_etat == 20)
            {
                if (obj->iframes_timer == 0)
                {
                    obj->iframes_timer = obj->follow_id;
                    if (num_world != 1)
                        set_sub_etat(obj, 7);
                    else
                    {
                        if (obj->display_prio != 0)
                            obj->display_prio = 0;
                        else
                            obj->display_prio = 3;
                    }
                }
                else
                    obj->iframes_timer--;
            }

            if (
                    (num_world != 1 || obj->display_prio == 0) &&
                    (num_world == 1 || obj->sub_etat != 20)
                    )
            {
                if (ray.follow_id == obj->id)
                {
                    ray.follow_id = -1;
                    set_main_and_sub_etat(&ray, 2, 2);
                    Reset_air_speed(is_rolling_speed);
                    jump_time = 0;
                    ray.link = 0;
                    ray.speed_y = 0;
                }
                obj->ray_dist = 1000;
            }
            break;
        case TYPE_BB1_PLAT:
            if (obj->sub_etat == 7)
            {
                if (
                        obj->anim_frame == obj->animations[obj->anim_index].frames_count - 1 &&
                        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0
                        )
                {
                    obj->is_active = 0;
                    obj->flags.alive = 0;
                }

                if (ray.follow_id == obj->id)
                {
                    ray.follow_id = -1;
                    set_main_and_sub_etat(&ray, 2, 2);
                    Reset_air_speed(is_rolling_speed);
                    jump_time = 0;
                    ray.link = 0;
                    ray.speed_y = 0;
                }
                obj->ray_dist = 1000;
            }

            if (obj->sub_etat == 20)
            {
                if (ray.follow_id == obj->id || obj->link != obj->param)
                    obj->link--;

                if (obj->iframes_timer == 0 || obj->link == 0)
                    set_sub_etat(obj, 7);
                else
                    obj->iframes_timer--;
            }
            break;
    }
}

void DoPlatformPoingCollision(obj_t* obj, s16 sprite) {
    if (sprite == obj->hit_sprite) {
        if (poing_obj->speed_x > 0) {
            set_sub_etat(obj, 25);
        } else if (poing_obj->speed_x < 0) {
            set_sub_etat(obj, 26);
        }
    }
}
