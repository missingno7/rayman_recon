/*
 * Fish enemies (GOG v1.21 object 1:57100-1:57860). File name invented (Rayverse poisson.c). Names: Rayverse/Android.
 * Worker candidate: 7/8 STRICT, contiguous _TEXT at 1:57100. See NOTES.md.
 * Accepted repository status remains in config/units.json.
 */
#include "obj.h"
#include "map.h"

extern level_t level;
extern s16 *link_init;
extern s16 ymap;
extern map_data_t mp;
extern obj_t ray;

#define SCREEN_HEIGHT 200
#define TYPE_FISH 10
#define TYPE_10_FISH 10
#define SIGNUM(value) ((value) == 0 ? 0 : ((value) > 0 ? 1 : -1))

void DO_ONE_CMD(obj_t *obj);
s16 on_block_chdir(obj_t *obj, s16 center_x, s16 center_y);
s16 test_allowed(obj_t *obj, s16 center_x, s16 center_y);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void DO_OBJ_COLLISIONS(obj_t *obj, s16 offs);
u8 EOA(obj_t *obj);
void allocate_splash(obj_t *obj);
void calc_obj_pos(obj_t *obj);
void obj_hurt(obj_t *obj);
void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
void set_main_etat(obj_t *obj, u8 etat);
void set_sub_etat(obj_t *obj, u8 sub_etat);

void DO_POISSON_VERT_CMD(obj_t *obj)
{
    DO_ONE_CMD(obj);
    if (
        (
            !(obj->flags.flip_x) &&
            on_block_chdir(obj, obj->offset_bx - 20, obj->offset_by - 3) &&
            (s16)test_allowed(obj, obj->offset_bx - 20, obj->offset_by - 3)
        ) ||
        (
            obj->flags.flip_x &&
            on_block_chdir(obj, obj->offset_bx + 20, obj->offset_by - 3) &&
            (s16)test_allowed(obj, obj->offset_bx + 20, obj->offset_by - 3)
        )
    ) {
        if (obj->flags.flip_x) {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 7, 1);
        } else {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 8, 1);
        }
    }
    DO_OBJ_COLLISIONS(obj, 4);
}

void DO_POISSON_BLEU_CMD(obj_t *obj)
{
    DO_ONE_CMD(obj);
    if (
        (
            !(obj->flags.flip_x) &&
            on_block_chdir(obj, obj->offset_bx - 20, obj->offset_by - 3) &&
            (s16)test_allowed(obj, obj->offset_bx - 20, obj->offset_by - 3)
        ) ||
        (
            obj->flags.flip_x &&
            on_block_chdir(obj, obj->offset_bx + 20, obj->offset_by - 3) &&
            (s16)test_allowed(obj, obj->offset_bx + 20, obj->offset_by - 3)
        )
    ) {
        if (obj->flags.flip_x) {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 13, 1);
        } else {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 14, 1);
        }
        obj->y = obj->init_y;
    }

    if (obj->main_etat == 2 && obj->sub_etat == 17) {
        if (obj->init_y <= obj->y) {
            if (obj->flags.flip_x)
                skipToLabel(obj, 10, 1);
            else
                skipToLabel(obj, 9, 1);
            obj->y = obj->init_y;
        }
    }
    if (obj->main_etat == 2 && obj->sub_etat == 18) {
        if (EOA(obj)) {
            if (obj->speed_y == 5) {
                obj->is_active = 0;
                obj->display_prio = 0;
            } else {
                obj->speed_x = 0;
                obj->speed_y = 5;
                skipToLabel(obj, 12, 1);
            }
        }
    }
}

void DoPoissonBleuPoingCollision(obj_t *obj, s16 sprite)
{
    if (!(obj->main_etat == 2 && obj->sub_etat == 18)) {
        skipToLabel(obj, 12, 1);
        obj->speed_x = 0;
        obj->speed_y = 0;
        obj->hit_points = 0;
    }
}

u8 can_free_fish(obj_t *fish)
{
    u8 result = 1;
    s16 i = 0;
    s16 nb_objs = level.nb_objects;
    obj_t *cur_obj = &level.objects[i];

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_10_FISH && cur_obj->init_x == fish->init_x &&
            cur_obj->is_active && cur_obj->timer < 100) {
            result = 0;
            break;
        }
        ++i;
        ++cur_obj;
    }
    return result;
}

void DESACTIVE_FISH_COLLIS(obj_t *fish)
{
    s16 index = link_init[fish->id];
    obj_t *unk_obj = &level.objects[index];

    if (unk_obj->y == fish->y) {
        unk_obj->y = ymap + SCREEN_HEIGHT;
        unk_obj->flags.alive = 0;
        unk_obj->is_active = 0;
    }

    index = link_init[unk_obj->id];
    unk_obj = &level.objects[index];
    if (unk_obj->y == fish->y) {
        unk_obj->is_active = 0;
        unk_obj->y = ymap + SCREEN_HEIGHT;
        unk_obj->flags.alive = 0;
    }
}

void DO_PYRANHA(obj_t *in_obj)
{
    u8 can_free;
    obj_t *cur_obj;
    s16 i;
    s16 nb_obj;

    DO_ONE_CMD(in_obj);
    can_free = 0;
    if (in_obj->main_etat == 0) {
        if (in_obj->y + in_obj->offset_by + 20 < ymap + SCREEN_HEIGHT)
            in_obj->timer++;
        if (in_obj->sub_etat == 3)
            in_obj->speed_y = 0;
        else if (in_obj->sub_etat == 9) {
            in_obj->speed_y = 6;
            if (in_obj->y + in_obj->offset_hy > ymap + SCREEN_HEIGHT ||
                in_obj->y + in_obj->offset_by + 14 > mp.height * 16) {
                s32 bottom = in_obj->y + in_obj->offset_by + 14;
                if (bottom > mp.height * 16)
                    allocate_splash(in_obj);
                in_obj->y = ymap + SCREEN_HEIGHT;
                in_obj->is_active = 0;
                in_obj->timer = 0;
                in_obj->flags.alive = 0;
                can_free = can_free_fish(in_obj);
            }
        } else if (in_obj->y + in_obj->offset_by < ymap) {
            in_obj->y = ymap + SCREEN_HEIGHT;
            in_obj->is_active = 0;
            in_obj->flags.alive = 0;
            can_free = can_free_fish(in_obj);
        }
    }
    if (in_obj->timer == 100)
        can_free = can_free_fish(in_obj);
    if (can_free) {
        cur_obj = level.objects;
        nb_obj = level.nb_objects;
        for (i = 0; i < nb_obj; i++) {
            if (cur_obj->type == TYPE_FISH && cur_obj->init_x == in_obj->init_x &&
                !(cur_obj->is_active)) {
                cur_obj->is_active = 1;
                cur_obj->flags.alive = 1;
                cur_obj->y = ymap + SCREEN_HEIGHT;
                cur_obj->x = in_obj->init_x;
                if (cur_obj->y + cur_obj->offset_by > ((mp.height - 1) << 4)) {
                    cur_obj->y = -cur_obj->offset_by + ((mp.height - 1) << 4);
                    allocate_splash(cur_obj);
                }
                cur_obj->main_etat = in_obj->init_main_etat;
                cur_obj->sub_etat = in_obj->init_sub_etat;
                cur_obj->hit_points = in_obj->init_hit_points;
                cur_obj->timer = 0;
                cur_obj->flags.read_commands = 1;
                calc_obj_pos(cur_obj);
                skipToLabel(cur_obj, 1, 1);
                break;
            }
            cur_obj++;
        }
    }
}

void DoFishPoingCollision(obj_t *obj, s16 sprite)
{
    obj_hurt(obj);
    if (obj->hit_points != 0) {
        obj->speed_x = 0;
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, 0, 1);
    } else {
        set_main_and_sub_etat(obj, 0, 3);
    }
    DESACTIVE_FISH_COLLIS(obj);
    if (obj->flags.flip_x)
        obj->cmd = 1;
    else
        obj->cmd = 0;
}

void DoPoissonBleuRaymanZDD(obj_t *obj)
{
    s32 state_ok;

    state_ok = obj->main_etat != 2 || obj->sub_etat != 16;
    if (!state_ok)
        return;
    state_ok = obj->main_etat != 2 || obj->sub_etat != 17;
    if (!state_ok)
        return;
    state_ok = obj->main_etat != 2 || obj->sub_etat != 18;
    if (!state_ok)
        return;

    if ((SIGNUM(ray.x + ray.offset_bx - obj->x - obj->offset_bx) >= 0 && obj->flags.flip_x) ||
        (SIGNUM(ray.x + ray.offset_bx - obj->x - obj->offset_bx) < 0 && !obj->flags.flip_x)) {
        if (obj->flags.flip_x)
            obj->speed_x = 3;
        else
            obj->speed_x = -3;

        obj->speed_y = -5;
        set_main_etat(obj, 2);
        set_sub_etat(obj, 16);
        skipToLabel(obj, 11, 1);
    }
}
