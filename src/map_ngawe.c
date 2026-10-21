/* 1:4d230..1:4def0: DRAW_MAP/INIT_GAME_MODE_NORMAL/FIN_GAME_MODE_NORMAL (Rayverse map.c) + ngawe.c functions; one or two original objects (a 16-byte-aligned boundary would be invisible) - file split INVENTED. Seeded by tools/seed.py, worker tu_map. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define Abs(value) ((value) >= 0 ? (value) : -(value))

typedef struct vec2b_t {
    u8 x;
    u8 y;
} vec2b_t;

#define BLOCK_FULLY_SOLID 0
#define BLOCK_SOLID 1
#define GO_SPEED 20
#define TYPE_RING 73
extern s16 bateau_obj_id;
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W2;
extern u8 block_flags[64];
extern u8 horloge[25];
extern level_t level;
extern poing_t poing;
extern obj_t *poing_obj;
extern s16 poing_obj_id;
extern obj_t ray;
extern big_map_t BIG_MAP;
void init_build_map(big_map_t *map, u8 *map_blocks);
extern u8 *DrawBufferNormal;
extern u8 *DrawBufferNormalIni;
extern u8 *draw_buffer;
extern u8 *DisplayBufferNormal;
extern void (*Copy_Plan0_To_Buf)(u8 *, u8 *, s32, s32, s32);
extern u8 *MAP_BLOCKS;
extern void Copy_Plan0Diff_To_Buf(u8 *, u8 *, s32, s32, s32);
extern void clear_memory(u8 *buffer, s32 color, s32 length);
u8 BTYP(s16 tile_x, s16 tile_y);
void CALC_MOV_ON_BLOC(obj_t* obj);
void DO_NOVA(obj_t* obj);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void SET_X_SPEED(obj_t* obj);
void add_alwobj(obj_t* obj);
void calc_obj_dir(obj_t* obj);
void calc_obj_pos(obj_t* obj);
u8 calc_typ_travd(obj_t* obj, u8 a2);
s32 instantSpeed(s16 speed);
void obj_hurt(obj_t* target);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
u8 skipOneCommand(obj_t *obj);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void snapToSprite(obj_t* obj_1, obj_t* obj_2, u8 obj_2_spr, s32 x_offs, s16 y_offs);
void ngaweFollowsShip(obj_t *obj);
void ngaweTriesToGrabShip(obj_t *obj);
void allocatePirateNgaweRing(obj_t* ngw_obj, s16 spd_y, u8 param_3);
void DO_NGW_TIR(obj_t *obj);
void NGW_REACT_TO_RAY_IN_ZONE(obj_t *obj);
void DO_ONE_NGW_COMMAND(obj_t* obj);
void DO_NGW_POING_COLLISION(obj_t* ngw_obj, s16 sprite);
void DO_ONE_NGW_RING_COMMAND(obj_t* ring_obj);

void DRAW_MAP(u8 *draw_buf, big_map_t *big_map)
{
    display_map_t *display_map;
    s32 tile_y;
    s32 tile_x;
    u32 tile_index;
    u32 row_index;
    s16 x;
    s16 y;
    s16 start_x;
    s16 start_y;
    u8 *row_buffer;

    display_map = big_map->display_map;
    if (display_map->height_in_tiles > 1 && display_map->width_in_tiles > 1) {
        draw_buf += display_map->x + 320 * display_map->y;
        tile_index = display_map->tile_index;
        x = start_x = display_map->x;
        y = start_y = display_map->y;
        row_index = tile_index;
        row_buffer = draw_buf;
        for (tile_x = 0; tile_x < display_map->width_in_tiles; tile_x++) {
            if (big_map->map[tile_index].transparency != 0) {
                if (big_map->map[tile_index].transparency == 1)
                    Display_Bloc_Plein_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, x, y);
                else
                    Display_Bloc_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, x, y);
            }
            draw_buf += 16;
            x += 16;
            tile_index++;
        }
        row_buffer += 5120;
        row_index += big_map->width;
        for (tile_y = 1; tile_y < display_map->height_in_tiles - 1; tile_y++) {
            tile_index = row_index;
            draw_buf = row_buffer;
            if (big_map->map[tile_index].transparency != 0) {
                if (big_map->map[tile_index].transparency == 1)
                    Display_Bloc_Plein_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, start_x, (s16)(start_y + tile_y * 16));
                else
                    Display_Bloc_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, start_x, (s16)(start_y + tile_y * 16));
            }
            tile_index++;
            draw_buf += 16;
            for (tile_x = 1; tile_x < display_map->width_in_tiles - 1; tile_x++) {
                if (big_map->map[tile_index].transparency != 0) {
                    if (big_map->map[tile_index].transparency == 1)
                        Display_Bloc_Plein(big_map->tile_texture_offsets[tile_index], draw_buf);
                    else
                        Display_Bloc(big_map->tile_texture_offsets[tile_index], draw_buf);
                }
                draw_buf += 16;
                tile_index++;
            }
            if (big_map->map[tile_index].transparency != 0) {
                if (big_map->map[tile_index].transparency == 1)
                    Display_Bloc_Plein_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, (s16)(start_x + tile_x * 16), (s16)(start_y + tile_y * 16));
                else
                    Display_Bloc_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, (s16)(start_x + tile_x * 16), (s16)(start_y + tile_y * 16));
            }
            row_buffer += 5120;
            row_index += big_map->width;
        }
        tile_index = row_index;
        draw_buf = row_buffer;
        x = start_x;
        y = start_y + tile_y * 16;
        for (tile_x = 0; tile_x < display_map->width_in_tiles; tile_x++) {
            if (big_map->map[tile_index].transparency != 0) {
                if (big_map->map[tile_index].transparency == 1)
                    Display_Bloc_Plein_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, x, y);
                else
                    Display_Bloc_Clippe(big_map->tile_texture_offsets[tile_index], draw_buf, x, y);
            }
            draw_buf += 16;
            x += 16;
            tile_index++;
        }
    }
}

void INIT_GAME_MODE_NORMAL(void)
{
    sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
    DrawBufferNormalIni = DrawBufferNormal;
    Copy_Plan0_To_Buf = Copy_Plan0Diff_To_Buf;
    DrawBufferNormal += 4;
    init_build_map(&BIG_MAP, MAP_BLOCKS);
    clear_memory(DisplayBufferNormal, 0, 64000);
    N_CLRSCR(DrawBufferNormal);
}

void FIN_GAME_MODE_NORMAL(void)
{
    DrawBufferNormal = DrawBufferNormalIni;
    draw_buffer = DrawBufferNormalIni;
}

void ngaweFollowsShip(obj_t *obj)
{
    if (bateau_obj_id != -1)
        snapToSprite(obj, &level.objects[bateau_obj_id], 2, 21, -51);
}

void ngaweTriesToGrabShip(obj_t *obj)
{
    s16 ship_h; s16 ship_w; s16 ship_y; s16 ship_x;

    obj_t *ship;
    s32 in_range;

    if (bateau_obj_id != -1)
    {
        ship = &level.objects[bateau_obj_id];
        GET_SPRITE_POS(ship, 2, &ship_x, &ship_y, &ship_w, &ship_h);
        obj->x = ship_x + 21;
        ship_h = ship_y - 67;
        in_range = (obj->y >= ship_h - obj->speed_y) && (obj->y <= ship_h + obj->speed_y);
        if (in_range)
            set_main_and_sub_etat(obj, 2, 2);
    }
}

void allocatePirateNgaweRing(obj_t* ngw_obj, s16 spd_y, u8 param_3) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 ngw_x; s16 ngw_h; s16 ngw_y; s16 ngw_w;
    s16 unk_1;
    s16 i = 0;
    obj_t *cur_obj = &level.objects[i];
    s16 nb_objs = level.nb_objects;

    while (i < nb_objs) {
        if (cur_obj->type == TYPE_RING && !cur_obj->is_active) {
            if (param_3)
                cur_obj->flags.flip_x = ngw_obj->flags.flip_x;
            else
                cur_obj->flags.flip_x = !ngw_obj->flags.flip_x;

            cur_obj->speed_y = spd_y;

            GET_SPRITE_POS(ngw_obj, 6, &ngw_x, &ngw_y, &ngw_w, &ngw_h);
            cur_obj->x = ngw_x - cur_obj->offset_bx;
            if (ngw_obj->flags.flip_x)
                cur_obj->x += ngw_w;

            cur_obj->y = ngw_y - ((cur_obj->offset_hy + cur_obj->offset_by) >> 1);

            skipToLabel(cur_obj, cur_obj->flags.flip_x, 1);
            calc_obj_pos(cur_obj);
            cur_obj->flags.alive = 1;
            cur_obj->is_active = 1;
            add_alwobj(cur_obj);
            cur_obj->flags.flag_0x40 = 0;

            /* see FLIP_X_SGN macro? */
            unk_1 =
                    (cur_obj->flags.flip_x ? 1 : -1) *
                    (s16) (ray.offset_bx + ray.x - cur_obj->x - cur_obj->offset_bx);
            unk_1 = MAX(0, (s32)unk_1) + 15;
            cur_obj->link = unk_1;

            cur_obj->flags.hurtbyfist = 0;
            cur_obj->flags.command_test = 0;
            cur_obj->param = ngw_obj->id;
            ngw_obj->config--;
            break;
        }
        cur_obj++;
        i++;
    }
}

void DO_NGW_TIR(obj_t *obj)
{
    if (obj->anim_frame < 26)
        obj->link = 0;

    if (
        obj->link == 0 && obj->anim_frame == 26 &&
        horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xF] == 0
    )
    {
        allocatePirateNgaweRing(obj, 0, 1);
        obj->link = 1;
    }
}

void NGW_REACT_TO_RAY_IN_ZONE(obj_t *obj)
{
    if (
        (
            (obj->main_etat == 0 && obj->sub_etat == 0) ||
            (obj->main_etat == 1 && obj->sub_etat == 0)
        ) && obj->config > 0
    )
        set_main_and_sub_etat(obj, 0, 2);
}

void DO_ONE_NGW_COMMAND(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 prev_flip_x = obj->flags.flip_x;

    calc_obj_dir(obj);
    switch (obj->main_etat * 0x100 + obj->sub_etat)
    {
        case 0x3:
            obj->speed_y = -8;
            obj->speed_x = 0;
            break;
        case 0x205:
        case 0x5:
            obj->speed_x = 0;
            break;
        case 0x0:
            obj->speed_x = 0;
            if (poing.is_active) {
                set_main_and_sub_etat(obj, 0, 7);
            } else {
                if (obj->config == 1) {
                    if (obj->flags.flip_x != prev_flip_x) {
                        set_sub_etat(obj, 6);
                    } else {
                        set_main_and_sub_etat(obj, 1, 0);
                    }
                } else {
                    obj->flags.flip_x = prev_flip_x;
                }
            }
            break;
        case 0x2:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            if (poing.is_active) {
                set_main_and_sub_etat(obj, 0, 7);
            } else {
                DO_NGW_TIR(obj);
            }
            break;
        case 0x100:
            if (poing.is_active) {
                set_main_and_sub_etat(obj, 0, 7);
            } else {
                if (obj->config == 1) {
                    if ((block_flags[calc_typ_travd(obj, 1)] & 1) ||
                        !(block_flags[
                                BTYP((obj->offset_bx + obj->x + obj->speed_x) >> 4, (obj->y + obj->offset_by) >> 4)
                        ] & 2)
                    ) {
                        if (prev_flip_x == obj->flags.flip_x) {
                            prev_flip_x = !obj->flags.flip_x;
                        }
                    }

                    if (obj->flags.flip_x != prev_flip_x) {
                        set_main_and_sub_etat(obj, 0, 6);
                    }

                    SET_X_SPEED(obj);
                    CALC_MOV_ON_BLOC(obj);
                } else {
                    set_main_and_sub_etat(obj, 0, 0);
                }
            }
            break;
        case 0x207:
            ngaweTriesToGrabShip(obj);
            break;
        case 0x202:
        case 0x203:
        case 0x204:
            ngaweFollowsShip(obj);
            break;
        case 0x6:
        case 0x4:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            if (poing.is_active) {
                set_main_and_sub_etat(obj, 0, 7);
            }
            break;
        case 0x8:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            if (!poing.is_active)
                set_sub_etat(obj, 4);
            break;
        case 0x7:
            obj->speed_x = 0;
            obj->flags.flip_x = prev_flip_x;
            break;
        case 0x1:
            obj->timer = 100;
            obj->speed_x = 0;
            break;
    }

    if (obj->timer != 0) {
        obj->timer--;
    }
}

void DO_NGW_POING_COLLISION(obj_t* ngw_obj, s16 sprite) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.

    if (ngw_obj->timer == 0) {
        poing.damage = 1;
        obj_hurt(ngw_obj);
        if (ngw_obj->hit_points == 0) {
            set_main_and_sub_etat(ngw_obj, 0, 3);
            if (bateau_obj_id != -1) {
                skipOneCommand(&level.objects[bateau_obj_id]);
                level.objects[bateau_obj_id].nb_cmd = 0;
            }
        } else {
            if (poing_obj->speed_x > 0)
                ngw_obj->flags.flip_x = 0;
            else if ((poing_obj->speed_x >= 0) && poing_obj->flags.flip_x)
                ngw_obj->flags.flip_x = 0;
            else
                ngw_obj->flags.flip_x = 1;
            set_main_and_sub_etat(ngw_obj, 0, 1);
        }
    }
}

void DO_ONE_NGW_RING_COMMAND(obj_t* ring_obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s32 distance;
    obj_t *unk_obj = &level.objects[ring_obj->param];

    if (!(unk_obj->is_active)) {
        ring_obj->flags.alive = 0;
        DO_NOVA(ring_obj);
        return;
    }

    if (ring_obj->link <= 0) {
        if (ring_obj->flags.hurtbyfist) {
            ring_obj->is_active = 0;
            ring_obj->flags.alive = 0;
            unk_obj->config++;
        } else {
            ring_obj->flags.hurtbyfist = 1;
            ring_obj->flags.command_test = 1;
            ring_obj->link = (ring_obj->offset_bx + (s16)ring_obj->x) - (s16)unk_obj->x - unk_obj->offset_bx;
            distance = ring_obj->link;
            if (distance < 0)
                distance = -distance;
            ring_obj->link = distance;
        }
        return;
    }

    ring_obj->flags.command_test = 0;
    if (ring_obj->cmd == GO_SPEED) {
        ring_obj->speed_x = ring_obj->iframes_timer;
        if (ring_obj->follow_id != 0) {
            ring_obj->speed_y = ring_obj->follow_id;
        } else {
            if (ring_obj->flags.hurtbyfist) {
                unk_obj = &level.objects[ring_obj->param];
                ring_obj->link = (unk_obj->offset_bx + (s16)unk_obj->x) - (s16)ring_obj->x - ring_obj->offset_bx;
                ring_obj->speed_y =
                        (
                                unk_obj->y + ((unk_obj->offset_by + unk_obj->offset_hy) >> 1) -
                                (ring_obj->y + ((ring_obj->offset_by + ring_obj->offset_hy) >> 1))
                        ) * ring_obj->speed_x;
                if (ring_obj->link != 0) {
                    ring_obj->speed_y /= ring_obj->link;
                }

                distance = ring_obj->link;
            if (distance < 0)
                distance = -distance;
            ring_obj->link = distance;
            }
            distance = ring_obj->speed_x;
            if (distance < 0)
                distance = -distance;
            ring_obj->link -= instantSpeed(distance);
        }
    }
}
