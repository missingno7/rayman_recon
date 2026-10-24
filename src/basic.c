/*
 * Core object/engine helpers (GOG v1.21 object 1:0d070-1:0fa40). File name invented (Rayverse basic.c).
 * The object starts with the 4-byte initialiser constant of deter_num_let's local array (1:0d070); EOA is at 1:0d074.
 * Names: Rayverse/Android. Worker statuses: NOTES.md and final.json. Several Rayverse return types were widened in the
 * port (e.g. s16 get_proj_x); DOS widths are used here where the binary proves them.
 */
#include "obj.h"
#include "game.h"
#include "rtypes.h"
#include "inline_helpers.h"
#include "basic_local.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <conio.h>

//1D074
bool EOA(obj_t* obj) {
    s32 on_last_frame;
    eta_t* eta = get_eta(obj);
    if (eta->flags & eta_flags_0x10_anim_reverse) {
        on_last_frame = (obj->anim_frame == 0);
    } else {
        anim_t* anim = obj->animations + obj->anim_index;
        on_last_frame = (obj->anim_frame == anim->frames_count - 1);
    }
    if (on_last_frame && horloge[eta->anim_speed & 15] == 0) {
        return true;
    } else {
        return false;
    }
}

//1D0EC
void save_objects_flags(void) {
    s32 i = 0;
    s32 nb_objects = level.nb_objects;
    obj_t* obj = &level.objects[0];
    while (i < nb_objects) {
        set2bits(&saveobj[i >> 4], i & 0xF, obj->flags.alive + 2 * obj->is_active);
        obj++;
        i++;
    }
}

//1D15C
void restore_objects_flags(void) {
    s32 high_bit;
    s32 low_bit;
    s32 i = 0;
    s32 nb_objects = level.nb_objects;
    obj_t* obj = &level.objects[0];
    while (i < nb_objects) {
        if (obj->type != TYPE_58_CAGE) {
            s32 word_index = i >> 4;
            s32 bit_index = i & 0xF;
            read2bits((s32*)&saveobj[word_index], bit_index, &high_bit, &low_bit);
            obj->is_active = high_bit;
            obj->flags.alive = low_bit;
        }
        i++;
        obj++;
    }
}

//1D204
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d204); meanings in docs/semantics/basic.json. */
void snapToSprite(obj_t* obj_1, obj_t* obj_2, u8 obj_2_spr, s32 x_offs, u16 y_offs) {
  /* COMPILER_STEERED: snapshots of the flag byte and X case order preserve
   * all eight targets carried by restore_objects_flags. Locals INVENTED. */
  u8 obj_2_frames_count;
  eta_t *obj_2_eta_2;
  s16 prev_frame;
  s16 prev_index;
  s16 obj_2_x;
  s16 obj_2_y;
  s16 obj_2_w;
  s16 obj_2_h;
  s16 new_spd_x;
  s16 new_spd_y;
  eta_t *obj_2_eta_1 = &obj_2->eta[obj_2->main_etat][obj_2->sub_etat];
  s16 obj_2_anim_index = obj_2_eta_1->anim_index;
  s16 obj_2_anim_frame = obj_2->anim_frame;
  if (horloge[obj_2_eta_1->anim_speed & 0xF] == 0)
  {
    obj_2_frames_count = obj_2->animations[obj_2_anim_index].frames_count;
    obj_2_anim_frame += (obj_2_eta_1->flags & 0x10) ? (-1) : (1);
    if (!inrange(obj_2_anim_frame, 0, obj_2_frames_count - 1))
    {
      obj_2_eta_2 = &obj_2->eta[obj_2_eta_1->next_main_etat][obj_2_eta_1->next_sub_etat];
      obj_2_anim_index = obj_2_eta_2->anim_index;
      obj_2_frames_count = obj_2->animations[obj_2_anim_index].frames_count;
      {
        s16 next_animation_start_frame;
        next_animation_start_frame = (obj_2_eta_2->flags & 0x10) ? (obj_2_frames_count - 1) : (0);
        obj_2_anim_frame = next_animation_start_frame;
      }
    }
  }
  prev_frame = obj_2->anim_frame;
  prev_index = obj_2->anim_index;
  obj_2->anim_frame = obj_2_anim_frame;
  obj_2->anim_index = obj_2_anim_index;
  GET_SPRITE_POS(obj_2, obj_2_spr, &obj_2_x, &obj_2_y, &obj_2_w, &obj_2_h);
  obj_2->anim_index = prev_index;
  obj_2->anim_frame = prev_frame;
  new_spd_x = (x_offs + obj_2_x) - obj_1->x;
  while (abs32_inline(new_spd_x) >= 16)
  {
    obj_1->x += sgn(new_spd_x) * 16;
    new_spd_x += sgn(new_spd_x) * (-16);
  }

  new_spd_y = (y_offs + obj_2_y) - obj_1->y;
  while (abs32_inline(new_spd_y) >= 16)
  {
    obj_1->y += sgn(new_spd_y) * 16;
    new_spd_y += sgn(new_spd_y) * (-16);
  }

  {
    s32 first_flags = flags[obj_1->type].flags1;
    s32 second_flags = flags[obj_2->type].flags1;
    switch (((first_flags >> 4) & 1) + (((second_flags >> 4) & 1) * 2))
    {
      case 0:
        new_spd_x += obj_2->speed_x;
        break;

      case 3:
        new_spd_x = instantSpeed(obj_2->speed_x) + (new_spd_x * 16);
        break;

      case 1:
        new_spd_x = (new_spd_x + obj_2->speed_x) * 16;
        break;

      case 2:
        new_spd_x += instantSpeed(obj_2->speed_x);
        break;


    }

  }
  {
    s32 first_flags = flags[obj_1->type].flags1;
    s32 second_flags = flags[obj_2->type].flags1;
    switch (((first_flags >> 5) & 1) + (((second_flags >> 5) & 1) * 2))
    {
      case 0:
        new_spd_y += obj_2->speed_y;
        break;

      case 3:
        new_spd_y = instantSpeed(obj_2->speed_y) + (new_spd_y * 16);
        break;

      case 1:
        new_spd_y = (new_spd_y + obj_2->speed_y) * 16;
        break;

      case 2:
        new_spd_y += instantSpeed(obj_2->speed_y);
        break;

    }

  }
  obj_1->speed_x = new_spd_x;
  obj_1->speed_y = new_spd_y;

}

//1D4C4
obj_t* findfirstObject(u16 obj_type) {
    obj_t* obj = level.objects;
    s16 i = 0;
    s16 nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (obj->type == obj_type) {
            break;
        }
        ++obj;
        ++i;
    }
    return obj;
}

//1D4F4
obj_t* findfirstInactiveObject(u16 obj_type) {
    obj_t* obj = level.objects;
    s16 i = 0;
    s16 nb_objs = level.nb_objects;
    while (i < nb_objs) {
        if (obj->type == obj_type && !obj->is_active)
            break;
        ++obj;
        ++i;
    }
    return obj;
}

//1D528
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d528); meanings in docs/semantics/basic.json. */
s16 sinYspeed(obj_t* obj, s16 phase_step, s16 amplitude, s16* phase_accumulator) {
    /* 225B8 80146DB8 -O2 -msoft-float */
    s16 phase_4096 = (*phase_accumulator + phase_step) & 0xFFF;
    s16 diff_y = obj->y - obj->init_y;
    s32 target_y_offset = (sinus(phase_4096 >> 3) * amplitude) >> 9;

    *phase_accumulator = phase_4096;
    return target_y_offset - diff_y;
}

//1D560
void set_proj_center(s16 x, s16 y) {
    PROJ_CENTER_X = x;
    PROJ_CENTER_Y = y;
}

//1D570
s16 get_proj_dist(s16 scale, s16 outer_dim) {
    //NOTE: needs checking
    if (zoom_mode == 0) {
        return outer_dim;
    } else {
        s32 temp = ((256 * 256) / (scale + 256)) * (outer_dim);
        return (temp / 256);
    }
}

//1D5B0
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d5b0); meanings in docs/semantics/basic.json. */
s32 get_proj_dist2(s16 scale, s16 unprojected_dimension) {
    return (unprojected_dimension * ((256*256) / (scale + 256))) >> 8;
}

//1D5D8
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d5d8); meanings in docs/semantics/basic.json. */
s16 get_proj_x(s16 scale, s16 unprojected_x) {
    //NOTE: needs checking
    s32 temp = ((256 * 256) / (scale + 256)) * (unprojected_x - PROJ_CENTER_X);
    return ((temp / 256) + PROJ_CENTER_X);
}

//1D614
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d614); meanings in docs/semantics/basic.json. */
s16 get_proj_y(s16 scale, s16 unprojected_y) {
    //NOTE: needs checking
    s32 temp = ((256 * 256) / (scale + 256)) * (unprojected_y - PROJ_CENTER_Y);
    return ((temp / 256) + PROJ_CENTER_Y);
}

//1D650
void set_zoom_mode(u8 mode) {
    zoom_mode = mode;
}

//1D658
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d658); meanings in docs/semantics/basic.json. */
s16 inverse_proj_x(s16 scale, s16 projected_x) {
    s32 divisor = (256*256) / (scale + 256);
    return ((projected_x - PROJ_CENTER_X) << 8) / divisor + PROJ_CENTER_X;
}

//1D690
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d690); meanings in docs/semantics/basic.json. */
s16 inverse_proj_y(s16 scale, s16 projected_y) {
    s32 divisor = (256*256) / (scale + 256);
    return ((projected_y - PROJ_CENTER_Y) << 8) / divisor + PROJ_CENTER_Y;
}

//1D6C8
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0d6c8); meanings in docs/semantics/basic.json. */
s32 vblToEOA(obj_t* obj, u8 duration_multiplier) {
    eta_t* eta = get_eta(obj);
    s16 anim_speed = eta->anim_speed & 15;
    s16 duration = anim_speed;
    s16 result;
    duration *= obj->animations[eta->anim_index].frames_count;
    result = duration * duration_multiplier - horloge[anim_speed] + 1;
    if (result > 0) return result;
    else return 1;
}

//1D738
void GET_ANIM_POS(obj_t* obj, s16* x, s16* y, s16* w, s16* h) {
    ASSERT(obj->anim_index != 255);
    anim_frame_t* frame = obj->animations[obj->anim_index].frames;
    frame += obj->anim_frame;
    *w = frame->width;
    *h = frame->height;
    if (!obj->flags.flip_x) {
        *x = obj->x + frame->x;
    } else {
        *x = (s16)obj->x + 2 * obj->offset_bx - frame->x - *w;
    }
    *y = obj->y + frame->y;
}

//1D7D8
void add_actobj(s16 obj_index) {
    s16 index = actobj.num_active_objects;
    actobj.num_active_objects++;
    actobj.objects[index] = obj_index;
    actobj.objects[index + 1] = -1;
}

// 1D808
void set_sub_etat(obj_t* obj, u8 sub_etat) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->sub_etat = sub_etat;
}

// 1D810
void set_main_etat(obj_t* obj, u8 etat) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->main_etat = etat;
}

// 1D818
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->main_etat = etat;
    obj->sub_etat = sub_etat;
}

//1D824
s16 get_center_x(obj_t* obj) {
    /* 22C84 80147484 -O2 -msoft-float */
    s16 res;

    switch (obj->type) {
        case TYPE_MOVE_PLAT:
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 64;
                    break;
                default:
                    res = 40;
                    break;
            }
            break;
        case TYPE_MOVE_OUYE:
            /* TODO: ... why does this work? */
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 64;
                    break;
                default:
                    res = 40;
                    break;
            }
            res = 18;
            break;
        case TYPE_MOVE_RUBIS:
            res = 18;
            break;
        case TYPE_MOVE_MARTEAU:
            res = 70;
            break;
        case TYPE_MOVE_START_PLAT:
            res = 128;
            break;
        case TYPE_BLACKTOON1:
            switch (obj->follow_sprite) {
                case 5:
                case 6:
                case 7:
                    res = 80;
                    break;
                default:
                    res = 40;
                    break;
            }
            break;
        case TYPE_PIRATE_P_45:
        case TYPE_PIRATE_P_D_45:
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_POELLE_D:
            res = 80;
            break;
        case TYPE_ROULETTE:
        case TYPE_ROULETTE2:
        case TYPE_ROULETTE3:
            res = 104;
            break;
        case TYPE_PT_GRAPPIN:
            res = 36;
            break;
        default:
            res = 40;
            break;
    }

    return res;
}

//1D954
s16 get_center_y(obj_t* obj) {
    /* 22DEC 801475EC -O2 -msoft-float */
    s16 res;

    switch (obj->type) {
        case TYPE_MOVE_PLAT:
            /* TODO: ... why does this work? */
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 40;
                    break;
                default:
                    res = 40;
                    break;
            }
            break;
        case TYPE_MOVE_OUYE:
            /* TODO: ... why does this work? */
            switch (num_world) {
                case 1:
                    res = 40;
                    break;
                case 2:
                    res = 64;
                    break;
                default:
                    res = 40;
                    break;
            }
            res = 18;
            break;
        case TYPE_MOVE_RUBIS:
            res = 18;
            break;
        case TYPE_MOVE_MARTEAU:
            res = 40;
            break;
        case TYPE_MOVE_START_PLAT:
            res = 64;
            break;
        case TYPE_BLACKTOON1:
            switch (obj->follow_sprite) {
                case 6:
                case 7:
                    if (obj->speed_y > 0)
                        res = 64;
                    else
                        res = 78;
                    break;
                case 5:
                    res = 72;
                    break;
                default:
                    /* TODO: ??? */
                    /* DOS leaves the return value unassigned for other follow sprites. */
                    break;
            }
            break;
        case TYPE_ROULETTE:
        case TYPE_ROULETTE2:
        case TYPE_ROULETTE3:
            res = 104;
            break;
        case TYPE_PT_GRAPPIN:
            res = 32;
            break;
        case TYPE_PIRATE_P_45:
        case TYPE_PIRATE_P_D_45:
        case TYPE_PIRATE_POELLE:
        case TYPE_PIRATE_POELLE_D:
            res = 96;
            break;
        case TYPE_SPIDER_PLAFOND:
            res = 90;
            break;
        default:
            res = 40;
            break;
    }

    return res;
}

//1DAC8
s16 on_block_chdir(obj_t* obj, s16 center_x, s16 center_y) {
    return block_flags[(u8)BTYP((obj->x + center_x) >> 4, (obj->y + center_y) >> 4)].bits.chdir;
}

//1DB04
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0db04); meanings in docs/semantics/basic.json. */
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, anim_t* anim_1, anim_t* anim_2, s16 anim_frame_2) {
    /* COMPILER_STEERED: declaration and independent X-load order. */
    s32 mirrored_layer_origin_x;
    s32 x_2;
    s32 x_1;
    u8 width_1;
    u8 follow_layer_index;
    anim_layer_t* layer_1 = &anim_1->layers[(anim_1->layers_per_frame & 0x3FFF) * obj->anim_frame];
    anim_layer_t* layer_2 = &anim_2->layers[(anim_2->layers_per_frame & 0x3FFF) * anim_frame_2];
    follow_layer_index = obj->follow_sprite;

    width_1 = obj->sprites[layer_1[follow_layer_index].sprite_index].outer_width;
    if (obj->flags.flip_x) {
        mirrored_layer_origin_x = (obj->offset_bx * 2) - width_1;
        x_1 = mirrored_layer_origin_x - layer_1[follow_layer_index].x;
        x_2 = mirrored_layer_origin_x - layer_2[follow_layer_index].x;
    } else {
        x_1 = layer_1[follow_layer_index].x;
        x_2 = layer_2[follow_layer_index].x;
    }
    obj->follow_x = x_1 - x_2;
    obj->follow_y = layer_1[follow_layer_index].y - layer_2[follow_layer_index].y;
}

//1DBB4
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0dbb4); meanings in docs/semantics/basic.json. */
s16 GET_SPRITE_POS(obj_t* obj, s16 layer_index, s16* x, s16* y, s16* w, s16* h) {
    /* 230E0 801478E0 -O2 -msoft-float */
    anim_t *anim;
    anim_layer_t *layer;
    u8 selected_sprite_index;
    sprite_t *sprite;
    s16 res;

    anim = &obj->animations[obj->anim_index];
    layer = &anim->layers[(anim->layers_per_frame & 0x3FFF) * obj->anim_frame];
    layer += layer_index;
    selected_sprite_index = layer->sprite_index;
    sprite = &obj->sprites[selected_sprite_index];
    if (selected_sprite_index != 0 && sprite->id != 0) {
        *w = sprite->inner_width;
        *h = sprite->inner_height;
        if (obj->flags.flip_x != (sprite->flags_bits.flip_x)) {
            *x = obj->x + (obj->offset_bx * 2 - (layer->x + sprite->sprite_pos.bits.x) - sprite->outer_width);
        } else {
            *x = obj->x + layer->x + sprite->sprite_pos.bits.x;
        }
        *y = layer->y + sprite->sprite_pos.bits.y + obj->y;
        res = true;
    }
    else
        res = false;
    return res;
}

//1DCFC
void GET_RAY_ZDC(obj_t* obj, s16* x, s16* y, s16* w, s16* h) {
    /* 23204 80147A04 -O2 -msoft-float */
    s16 frame;

    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)
    {
        *x = obj->x + 72;
        *y = obj->y + 64;
        *w = 16;
        *h = 14;
    }
    else if (obj->main_etat == 5)
    {
        *x = obj->x + 72;
        *y = obj->y + 40;
        *w = 16;
        *h = 46;
    }
    else if (obj->main_etat == 7)
    {
        frame = obj->anim_frame;
        if (frame > 32)
            frame -= 32;
        if (frame > 9 && frame < 23)
        {
            *x = obj->x + 64;
            *y = obj->y + 54;
            *w = 36;
            *h = 20;
        }
        else
        {
            *x = obj->x + 70;
            *y = obj->y + 40;
            *w = 20;
            *h = 40;
        }
    }
    else
    {
        *x = obj->x + 72;
        *y = obj->y + 24;
        *w = 16;
        *h = 54;
    }
    if (obj->scale != 0)
    {
        set_proj_center(obj->x + obj->offset_bx, obj->y + obj->offset_by);
        *x = get_proj_x(obj->scale, *x);
        *y = get_proj_y(obj->scale, *y);
        *w = get_proj_dist2(obj->scale, *w);
        *h = get_proj_dist2(obj->scale, *h);
        set_proj_center(obj->screen_x + obj->offset_bx, obj->screen_y + obj->offset_by);
    }
}

//1DEC0
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0dec0); meanings in docs/semantics/basic.json. */
void GET_BB1_ZDCs(obj_t* obj, s16 *x_1, s16 *y_1, s16 *w_1, s16 *h_1, s16 *x_2, s16 *y_2, s16 *w_2, s16 *h_2) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 collision_layer_index = 6;
    s16 h_loc;

    GET_ANIM_POS(obj, x_1, y_1, w_1, &h_loc);
    if (obj->main_etat == 2 && (obj->sub_etat == 0 || obj->sub_etat == 4))
        collision_layer_index = 3;
    GET_SPRITE_POS(obj, collision_layer_index, x_1, y_1, w_1, h_1);
    *x_2 = *x_1 - 7;
    *y_2 = *y_1 + *h_1;
    *w_2 = *w_1 + 14;
    *h_2 = h_loc >> 1;
    *x_1 += 4;
    *w_1 -= 8;
    *y_1 += 4;
    *h_1 -= 8;
    if ((obj->main_etat == 0 && obj->sub_etat == 16) || (obj->main_etat == 0 && obj->sub_etat == 19))
        GET_SPRITE_POS(obj, 0, x_1, y_1, w_1, h_1);
}

//1DFB8
void calc_obj_dir(obj_t* obj) {
    s32 dx = ray.x + ray.offset_bx - obj->x - obj->offset_bx;
    if (dx > 0) obj->flags.flip_x = true;
    else obj->flags.flip_x = false;
}

//1DFF4
s16 OBJ_IN_ZONE(obj_t* obj) {
    /* 236C4 80147EC4 -O2 -msoft-float */
    switch(obj->type) {
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
        case TYPE_LIDOLPINK:
        case TYPE_LIDOLPINK2:
            return obj->detect_zone_flag == 1;
        default:
            return obj->detect_zone_flag >= 1;
    }
}

//1E030
void calc_obj_pos(obj_t* obj) {
    obj->screen_y = obj->y - ymap;
    obj->screen_x = obj->x - xmap + 8;
}

//1E05C
void makeUturn(obj_t* obj) {
    /* 2375C 80147F5C -O2 -msoft-float */
    u8 label = 255;
    u8 foll_spr;

    switch (obj->type)
    {
        case TYPE_BLACKTOON1:
            if (obj->follow_sprite == 1 || obj->follow_sprite == 2 || obj->follow_sprite == 4 || obj->follow_sprite == 7)
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 3;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 2;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_BADGUY1:
            if (
                    !(obj->main_etat == 0 &&
                      (obj->sub_etat == 3 || obj->sub_etat == 1 || obj->sub_etat == 5 ||
                       obj->sub_etat == 4 || obj->sub_etat == 6))
                    )
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 1;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 0;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_BADGUY2:
        case TYPE_BADGUY3:
            if (!(obj->main_etat == 0 && (obj->sub_etat == 3 || obj->sub_etat == 1)))
            {
                if (obj->flags.read_commands)
                {
                    if (obj->cmd == GO_LEFT)
                    {
                        label = 1;
                        obj->flags.flip_x = 1;
                    }
                    else if (obj->cmd == GO_RIGHT)
                    {
                        label = 0;
                        obj->flags.flip_x = 0;
                    }
                }
                else
                {
                    if (obj->cmd == GO_LEFT)
                    {
                        label = 6;
                        obj->flags.flip_x = 1;
                    }
                    else if (obj->cmd == GO_RIGHT)
                    {
                        label = 5;
                        obj->flags.flip_x = 0;
                    }
                }
            }
            break;
        case TYPE_GENEBADGUY:
            if (!(obj->main_etat == 0 && obj->sub_etat == 3))
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 1;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 0;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_STONEMAN1:
        case TYPE_STONEMAN2:
            if (obj->cmd == GO_LEFT)
            {
                label = 5;
                obj->flags.flip_x = 1;
            }
            else if (obj->cmd == GO_RIGHT)
            {
                label = 4;
                obj->flags.flip_x = 0;
            }
            break;
        case TYPE_TROMPETTE:
            if (!(obj->main_etat == 0 && obj->sub_etat == 1))
            {
                if (obj->flags.read_commands)
                {
                    if (obj->cmd == GO_LEFT)
                    {
                        label = 1;
                        obj->flags.flip_x = 1;
                    }
                    else if (obj->cmd == GO_RIGHT)
                    {
                        label = 0;
                        obj->flags.flip_x = 0;
                    }
                }
            }
            break;
        case TYPE_BIG_CLOWN:
        case TYPE_WAT_CLOWN:
            if (obj->flags.read_commands)
            {
                if (obj->cmd == GO_LEFT)
                {
                    label = 1;
                    obj->flags.flip_x = 1;
                }
                else if (obj->cmd == GO_RIGHT)
                {
                    label = 0;
                    obj->flags.flip_x = 0;
                }
            }
            break;
        case TYPE_SPIDER:
            if (obj->cmd == GO_LEFT)
            {
                obj->flags.flip_x = 1;
                label = 2;
            }
            else
            {
                obj->flags.flip_x = 0;
                label = 0;
            }
            break;
        case TYPE_MITE2:
            if (!(obj->flags.flip_x))
                obj->flags.flip_x = 1;
            else
                obj->flags.flip_x = 0;

        case TYPE_MITE:
            if (!(obj->main_etat == 0 && obj->sub_etat == 3))
            {
                label = 3;
                obj->follow_id = 0;
            }
            break;
        case TYPE_CAISSE_CLAIRE:
            if (!(obj->main_etat == 0 && obj->sub_etat == 2))
            {
                set_main_and_sub_etat(obj, 1, 1);
                if (!(obj->flags.flip_x))
                {
                    obj->flags.flip_x = 1;
                    label = 3;
                }
                else
                {
                    obj->flags.flip_x = 0;
                    label = 2;
                }
            }
            break;
        case TYPE_WALK_NOTE_1:
            if (obj->cmd == GO_LEFT)
            {
                label = 2;
                obj->flags.flip_x = 1;
            }
            else if (obj->cmd == GO_RIGHT)
            {
                label = 1;
                obj->flags.flip_x = 0;
            }
            break;
        case TYPE_SPIDER_PLAFOND:
            if (obj->main_etat == 0 && obj->sub_etat == 24) break;
            if (obj->main_etat == 0 && obj->sub_etat == 18) break;
            if (obj->main_etat == 0 && obj->sub_etat == 19) break;
            {
                set_main_and_sub_etat(obj, 0, 24);
                obj->flags.flip_x = 1 - obj->flags.flip_x;
                obj->speed_y = 0;
                obj->speed_x = obj->speed_y;
            }
            break;
    }

    if (label != 255) {
        // NOTE: difference with PS1 version
        if (flags[obj->type].speed_x_uses_sixteenths) {
            obj->x -= instantSpeed(obj->speed_x);
        } else {
            obj->x -= obj->speed_x;
        }
        obj->speed_x = 0;
        skipToLabel(obj, label, true);
    }
}

//1E588
/* DOS 1:e5f7/e600 returns a full zero-extended value. Keep each
 * coordinate live only after its guard; callers narrow to a tile byte. */
s32 BTYP(s16 tile_x, s16 tile_y) {
    s32 x = tile_x;
    s32 y;
    if (!inrange(x, 0, mp.width - 1)) goto outside;
    y = tile_y;
    if (!inrange(y, 0, mp.height - 1)) goto outside;
    return mp.map[y * mp.width + x].tile_type;
outside:
    return BTYP_NONE;
}

//1E608
void calc_btyp_square(obj_t* obj) {
  s16 x;
  s16 y;
  s16 tile_x_right;
  s16 tile_x;
  s16 tile_x_left;
  s16 tile_y;
  {
    u8 x_offset;
    if (obj->type == 23)
    {
      if (RayEvts.tiny)
      {
        x_offset = 4;
      }
      else
      {
        x_offset = 8;
      }
    }
    else
    {
      x_offset = 16;
    }
    x = obj->x + obj->offset_bx;
    y = obj->y + obj->offset_by;
    tile_y = ashr16(y, 4);
    tile_x = ashr16(x, 4);
    tile_x_left = ashr16(x - x_offset, 4);
    tile_x_right = ashr16(x + x_offset, 4);
  }
  obj->btypes[3] = BTYP(tile_x, tile_y - 1);
  obj->btypes[1] = BTYP(tile_x_left, tile_y);
  {
    /* COMPILER_STEERED: both arms fit s16; s32 center plus x + x_offset reproduces the DOS homes and operand order (1:0e6a5, 1:0e71e). */
    s32 center_btyp = (obj->main_etat == 2) ? (bloc_floor((u8) BTYP(tile_x, tile_y), x & 15, y & 15)) : ((u8) BTYP(tile_x, tile_y));
    obj->btypes[0] = center_btyp;
  }
  obj->btypes[2] = BTYP(tile_x_right, tile_y);
  obj->btypes[4] = BTYP(tile_x, tile_y + 1);


}

//1E76C
void DO_OBJ_REBOND_EN_X(obj_t* obj) {
    if (block_flags[obj->btypes[2]].raw & 0x10 || block_flags[obj->btypes[1]].raw & 0x10) {
        obj->speed_x = -obj->speed_x;
    }
}

//1E790
u8 calc_btyp(obj_t* obj) {
    /* 23F80 80148780 -O2 -msoft-float */
    u8 btyp;
    u8 btypes_1_2_solid;
    s16 x, y;
    u8 foll_spr;

    calc_btyp_square(obj);
    x = obj->offset_bx + (s16)obj->x;
    y = obj->offset_by + (s16)obj->y;

    if (obj->type == TYPE_RAYMAN)
    {
        ray.param = -1;
        switch (ray.btypes[0])
        {
            case BTYP_NONE:
            case BTYP_CHDIR:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SOLID_LEFT_45:
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_LIANE:
            case BTYP_SOLID_PASSTHROUGH:
            case BTYP_SOLID:
                ray_last_ground_btyp = true;
                break;
            case BTYP_SLIPPERY_RIGHT_45:
            case BTYP_SLIPPERY_LEFT_45:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
                ray_last_ground_btyp = false;
                break;
            case 25: /* DOS 1:0e7e3 leaves ray_last_ground_btyp unchanged. */
                break;
            case BTYP_SLIPPERY:
                ray_last_ground_btyp = false;
                break;
        }
    }

    if (!(block_flags[obj->btypes[0]].raw & 2)) {
        if (obj->type == TYPE_RAYMAN) {
            btyp = mp.map[ray.ray_dist].tile_type;
        } else {
            btyp = BTYP(x >> 4, y >> 4);
        }

        if (obj->main_etat == 2 && !(block_flags[btyp].raw & 2)) {
            btyp = BTYP(x >> 4, (y + 16) >> 4);
        }

        if (!(block_flags[btyp].raw & 2)) {
            btypes_1_2_solid = block_flags[obj->btypes[1]].bits.solid + 2 * block_flags[obj->btypes[2]].bits.solid;
            if (btypes_1_2_solid == 3) /* both 1 and 2 have solid flag */ {
                if (((s16)(x & 0xF)) < 8)
                    btypes_1_2_solid = 1;
                else
                    btypes_1_2_solid = 2;
            }

            if (btypes_1_2_solid != 0) {
                btypes_1_2_solid--;
                if (obj->type == TYPE_RAYMAN) {
                    if (ray_last_ground_btyp == true) {
                        if (btypes_1_2_solid)
                            x += 16;
                        else
                            x -= 16;
                        if (!(block_flags[(u8) BTYP(x >> 4, (y - 16) >> 4)].raw & 2)) {
                            if (ray.main_etat != 2) {
                                /* ??? don't understand */
                                if (btypes_1_2_solid != (ray.flags.flip_x))
                                    obj->param = 0;
                                else
                                    obj->param = 1;
                            }

                            ray.btypes[0] = ray.btypes[1 + (btypes_1_2_solid != 0)];
                        }
                    }
                } else {
                    if ((obj->flags.flip_x) != btypes_1_2_solid && obj->main_etat != 2 &&
                        ((obj->type == TYPE_BADGUY1 && obj->flags.read_commands) ||
                            obj->type == TYPE_BADGUY2 || obj->type == TYPE_BADGUY3 || obj->type == TYPE_GENEBADGUY ||
                            obj->type == TYPE_STONEMAN1 || obj->type == TYPE_STONEMAN2 ||
                            obj->type == TYPE_BIG_CLOWN || obj->type == TYPE_WAT_CLOWN ||
                         (obj->type == TYPE_SPIDER && obj->param != 0) ||
                            obj->type == TYPE_TROMPETTE || obj->type == TYPE_MITE || obj->type == TYPE_MITE2 ||
                            obj->type == TYPE_CAISSE_CLAIRE || obj->type == TYPE_WALK_NOTE_1 || obj->type == TYPE_SPIDER_PLAFOND ||
                            (obj->type == TYPE_BLACKTOON1 &&
                             (foll_spr = obj->follow_sprite, foll_spr == 1 || foll_spr == 4 || (foll_spr == 7 && obj->config == 2)) &&
                             !(obj->main_etat == 0 && obj->sub_etat == 4)
                            )
                        )
                    ) {
                        makeUturn(obj);
                        obj->btypes[0] = BTYP_SOLID;
                    }
                }
            }
            btyp = obj->btypes[0];
            return btyp;
        } else {
            return btyp;
        }
    } else {
        btyp = obj->btypes[0];
        return btyp;
    }
}

//1EAA8
void init_obj_in_the_air(obj_t* obj) {
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 0;
}

//1EAB4
void make_my_fruit_go_down(obj_t* obj, s16 x_accel) {
    /* 243C4 80148BC4 -O2 -msoft-float */
    if (obj->main_etat == 2 && Abs((s32)obj->speed_x) < 32)
        obj->speed_x += x_accel;
    if (obj->type == TYPE_FALLING_YING_OUYE)
    {
        if (obj->main_etat != 2 || obj->sub_etat != 0)
        {
            set_main_and_sub_etat(obj, 2, 0);
            init_obj_in_the_air(obj);
        }
    }
    else
    {
        if (obj->main_etat != 2 || obj->sub_etat != 2)
        {
            set_main_and_sub_etat(obj, 2, 2);
            init_obj_in_the_air(obj);
        }
    }
}

//1EB18
void switchOff(obj_t* obj) {
    if (EOA(obj)) {
        obj->flags.alive = 0;
        obj->is_active = 0;
        if (!(ray_mode == MODE_5_CASSE_BRIQUE && obj->type == TYPE_161_WIZ)) {
            del_alwobj(obj->id);
        }
        if (obj->type == TYPE_170_RAYON || obj->type == TYPE_75_PAILLETTE) {
            obj->x = -32000;
            obj->y = -32000;
        }
    }
}

//1EB80
void obj_hurt(obj_t* target) {
    if (target->hit_points < poing.damage) {
        target->hit_points = 0;
    } else {
        target->hit_points -= poing.damage;
    }
}

//1EBA0
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0eba0); meanings in docs/semantics/basic.json. */
void Projectil_to_RM(obj_t* obj, s16* out_spd_x, s16* out_spd_y, s16 new_spd_x, s16 new_spd_y) {
    /* 249D0 801491D0 -O2 -msoft-float */
    s16 diff_x; s16 diff_y;
    s16 diff_x_abs; s16 diff_y_abs;
    s16 target_step_x; s16 target_step_y;
    s32 spd_x;

    *out_spd_x = 0;
    *out_spd_y = 0;
    diff_x = ray.offset_bx + obj->follow_x - obj->x - obj->offset_bx;
    diff_y = ray.offset_by + obj->follow_y - obj->y - obj->offset_by;
    diff_x_abs = Abs(diff_x);
    diff_y_abs = Abs(diff_y);
    if (diff_x != 0 && diff_y != 0) {
        if (diff_x_abs > diff_y_abs) {
            target_step_x = diff_x / diff_y_abs;
            target_step_y = diff_y / diff_y_abs;
            *out_spd_x = new_spd_x;
            if (diff_x <= 0) {
                *out_spd_x = -new_spd_x;
            }
            *out_spd_y = diff_y * new_spd_y / diff_x_abs;
            if (*out_spd_y == 0) {
                spd_x = *out_spd_x;
                if (diff_x > 0) {
                    spd_x += 16;
                } else {
                    spd_x -= 16;
                }
                *out_spd_x = spd_x;
            }
        } else {
            target_step_y = diff_y / diff_x_abs;
            target_step_x = diff_x / diff_x_abs;
            *out_spd_y = new_spd_y;
            if (diff_y <= 0) {
                *out_spd_y = -new_spd_y;
            }
            *out_spd_x = diff_x * new_spd_x / diff_y_abs;
        }
    } else if (diff_y == 0) {
        *out_spd_x = new_spd_x;
        if (diff_x > 0) {
            *out_spd_x = -new_spd_x;
        }
        target_step_x = -1;
        if (diff_x > 0) {
            target_step_x = 1;
        }
        target_step_y = 0;
    } else {
        *out_spd_y = new_spd_y;
        if (diff_y <= 0) {
            *out_spd_y = -new_spd_y;
        }
        target_step_x = 0;
        target_step_y = -1;
        if (diff_y > 0) {
            target_step_y = 1;
        }
    }

    if ((!(obj->flags.flip_x) && *out_spd_x > 0) || (obj->flags.flip_x && *out_spd_x < 0)) {
        *out_spd_x = -*out_spd_x;
        if (*out_spd_y == 0) {
            *out_spd_y = 2;
        }
    }

    obj->follow_x += target_step_x * 2;
    obj->follow_y += target_step_y * 2;
}

//1EE18
void del_actobj(obj_t* obj) {
    s16 j;
    
    s16 i;
    obj_t* active;
    obj->is_active = 0;
    i = 0;
    active = &level.objects[actobj.objects[i]];
    while (i < actobj.num_active_objects) {
        if (active->id == obj->id) break;
        ++i;
        active = level.objects + actobj.objects[i];
    }
    for (j = i; j <= actobj.num_active_objects; ++j)
        actobj.objects[j - 1] = actobj.objects[j];
    actobj.num_active_objects--;
}

//1EEB8
s16 calc_let_Width(u8 font_size, u32 num_let) {
    sprite_t* sprite;
    if (num_let >= 1000) {
        sprite = alpha_numbers.sprites + (num_let - 1000);
    } else {
        switch (font_size) {
            case 0:
            case 1:
                if (font_size == 1) num_let += 41;
                sprite = alpha2.sprites + num_let;
                break;
            case 2:
                sprite = alpha.sprites + num_let;
                break;
        }
    }
    return sprite->outer_width;
}

//1EF1C
s16 calc_let_Width2(u8 font_size, u32 num_let) {
    sprite_t* sprite;
    if (num_let >= 1000) {
        sprite = alpha_numbers.sprites + (num_let - 1000);
    } else {
        switch (font_size) {
            case 0:
            case 1:
                if (font_size == 1) num_let += 41;
                sprite = alpha2.sprites + num_let;
                break;
            case 2:
                sprite = alpha.sprites + num_let;
                break;
        }
    }
    return sprite->inner_width + sprite->sprite_pos.bits.x;
}

//1EF80
void INIT_TEXT_TO_DISPLAY(void) {
    s32 i;
    for (i = 0; i < 10;) {
        ++i;
        strcpy(text_to_display[i].text, "");
        text_to_display[i].field_D5 = 0;
        text_to_display[i].is_fond = 0;
    }
}

//1EFB4
s32 deter_num_let(u8 c, const char* next_chars) {
    s32 result = 0;
    if (c == ']') {
        char next_text[4] = {0};
        memcpy(next_text, next_chars, 3);
        result = atoi(next_text) + 1000;
    } else {
        if (c >= 'a' && c <= 'z') result = c - 93;
        if (c >= '0' && c <= '9') result = c - 18;
        switch (c) {
            case 63:
                result = 1; break;
            case 33:
                result = 2; break;
            case 46:
                result = 3; break;
            case 58:
                result = 40; break;
            case 39:
                result = 41; break;
            case 35:
                result = 47; break;
            case 94:
            case 126:
                result = 46; break;
            case 37:
            case 248:
                result = 45; break;
            case 42:
                result = 48; break;
            case 124:
                result = 49; break;
            case 148:
                result = 43; break;
            case 129:
                result = 44; break;
            case 132:
            case 156:
                result = 42; break;
        }
    }
    return result;
}

//1F194
void calc_num_let_spr(u8 font_size, u8* letter_num) {
    if (*letter_num < 1000 && font_size == 1)
        *letter_num += 41;
}

//1F1B0
s16 calc_largmax_text(const char* text, s16 char_index, s16 space_width, s16 char_spacing, u8 font_size) {
    s16 width = 0;
    s16 i;
    u32 num_let;
    /* DOS 1:f1c2 jumps to the increment/test at 1:f204. Keep that footer
       explicit; a while condition moves the test before the character body. */
    i = char_index;
    goto next_character;
character:
    if (text[i] == '/') goto done;
    if (text[i] == ' ') {
        width += space_width;
    } else {
        num_let = deter_num_let(text[i], text + (i + 1));
        if (num_let != 0) {
            width += calc_let_Width2(font_size, num_let) - char_spacing;
            if (num_let >= 1000) i += 3;
        }
    }
next_character:
    ++i;
    if (text[i] != '\0') goto character;
done:
    return width;
}

//1F21C
void INIT_TXT_BOX(display_item_t* box) { /* Close-out compiler steering; introduced local names INVENTED. Quality: RESULTS.json. */
    s16 i;
    char text[200];
    s16 max_width = 0;
    u32 num_let = 0;
    s16 num_lines = 0;
    s16 max_chars = 0;
    s16 total_chars = 0;
    s16 is_slash = 0;
    s16 char_spacing = 0;
    s16 space_width = 0;
    s16 line_height = 0;
    s16 total_width = 0;
    u8 font_size = box->font_size;

    if (font_size == 2) {
        line_height = 15;
        char_spacing = 1;
        space_width = 8;
    } else if (font_size == 1) {
        line_height = 23;
        char_spacing = 3;
        space_width = 10;
    } else {
        line_height = 36;
        char_spacing = 3;
        space_width = 12;
    }
    memcpy(text, box->text, sizeof(text));
    if (text[0] != '\0') {
        for (i = 0; text[i] != '\0'; ++i) {
            if (text[i] == '/') {
                is_slash = 1;
                ++num_lines;
                if (total_width > max_width) {
                    max_width = total_width;
                }
                total_width = 0;
                if (total_chars > max_chars) {
                    max_chars = total_chars;
                }
                total_chars = 0;
                num_let = 0;
            } else if (text[i] == ' ') {
                num_let = 0;
                total_width += space_width;
                ++total_chars;
            } else {
                num_let = deter_num_let(text[i], text + (i + 1));
            }
            if (num_let != 0) {
                total_chars += 1;
                total_width += calc_let_Width2(font_size, num_let) - char_spacing;
                if (num_let >= 1000) {
                    i += 3;
                }
            }
        }
        if (num_lines == 0) {
            max_width = total_width;
            num_lines = 1;
        } else {
            --num_lines;
        }
        if (is_slash) {
            box->centered_x_pos = box->xpos - (max_width >> 1);
        } else {
            box->centered_x_pos = box->xpos;
        }
        box->width = max_width + 2;
        box->centered_y_pos = box->ypos - line_height + 2;
        box->height = num_lines * line_height - 2;
    }
}


//1F4A0
void Deter_Option_Caract(char* text, s16 key, s16 offset) {
    u8 i;
    if (offset == 255) {
        for (i = 0; text[i] != '\0'; i++) {
            if (text[i] == '$') offset = i;
        }
    }
    if (offset != 255) {
        switch(key) {
            case 0: {
                text[offset] = 'a';
            } break;
            case 1: {
                text[offset] = 'b';
            } break;
            case 2: {
                text[offset] = 'c';
            } break;
            case 3: {
                text[offset] = 'd';
            } break;
            default: break;
        }
    }
}

//1F50C
void SwapAB(s16* a, s16* b) {
    s16 temp = *b;
    *b = *a;
    *a = temp;
}

/* DOS-derived Bresenham control flow. Endpoint fraction has denominator 128.
   The original redundant shift-helper branches are still not reproduced by C. */
//1F520
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0f520); meanings in docs/semantics/basic.json. */
void Bresenham(void (*func)(s16, s16), s16 origin_x, s16 origin_y,
               s16 dest_x, s16 dest_y, s16 callback_gap, s16 endpoint_fraction_128) {
    s32 dx = dest_x - origin_x;
    s32 dy = dest_y - origin_y;
    s16 reversed = 0;
    s16 counter;
    s16 gap = callback_gap;
    s16 x_straight, x_diagonal, y_straight, y_diagonal;
    s32 step_x, step_y;
    s32 error, x, y, end;

    if (dx != 0 && Abs(dy / dx) < 1) counter = 1;
    else counter = 0;
    if (counter == 1) {
        if (dx < 0) {
            reversed = 1;
            SwapAB(&origin_x, &dest_x);
            SwapAB(&origin_y, &dest_y);
            dx = -dx;
            dy = -dy;
        } else if (dx == 0 && dy < 0) {
            reversed = 1;
            SwapAB(&origin_y, &dest_y);
            dy = -dy;
        }
        if (dy > 0) step_y = 1;
        else if (dy < 0) step_y = -1;
        else step_y = 0;
        dy = Abs(dy);
        error = 2 * dy - dx;
        x_straight = 2 * dy;
        x_diagonal = 2 * (dy - dx);
        if (reversed) {
            x = dest_x;
            end = x + (((origin_x - x) * endpoint_fraction_128) >> 7);
            ++x;
            y = dest_y;
            step_y = -step_y;
            while (x >= end) {
                ++counter;
                if (error > 0) { y += step_y; error += x_diagonal; }
                else error += x_straight;
                if (gap < counter) { func(x, y); counter = 0; }
                --x;
            }
        } else {
            x = origin_x;
            end = x + (((dest_x - x) * endpoint_fraction_128) >> 7);
            ++x;
            y = origin_y;
            while (x <= end) {
                ++counter;
                if (error > 0) { y += step_y; error += x_diagonal; }
                else error += x_straight;
                if (gap < counter) { func(x, y); counter = 0; }
                ++x;
            }
        }
    } else {
        if (dy < 0) {
            reversed = 1;
            SwapAB(&origin_x, &dest_x);
            SwapAB(&origin_y, &dest_y);
            dx = -dx;
            dy = -dy;
        } else if (dy == 0 && dx < 0) {
            reversed = 1;
            SwapAB(&origin_x, &dest_x);
            dx = -dx;
        }
        if (dx > 0) step_x = 1;
        else if (dx < 0) step_x = -1;
        else step_x = 0;
        dx = Abs(dx);
        error = 2 * dx - dy;
        y_straight = 2 * dx;
        y_diagonal = 2 * (dx - dy);
        if (reversed) {
            y = dest_y;
            end = y + (((origin_y - y) * endpoint_fraction_128) >> 7);
            ++y;
            x = dest_x;
            step_x = -step_x;
            while (y >= end) {
                ++counter;
                if (error > 0) { x += step_x; error += y_diagonal; }
                else error += y_straight;
                if (gap < counter) { func(x, y); counter = 0; }
                --y;
            }
        } else {
            y = origin_y;
            end = y + (((dest_y - y) * endpoint_fraction_128) >> 7);
            ++y;
            x = origin_x;
            while (y <= end) {
                ++counter;
                if (error > 0) { x += step_x; error += y_diagonal; }
                else error += y_straight;
                if (gap < counter) { func(x, y); counter = 0; }
                ++y;
            }
        }
    }
}

//1F880
void init_finBossLevel(void) {
    finBosslevel.bzzit = 0;
    finBosslevel.moskito = 0;
    finBosslevel.mr_sax = 0;
    finBosslevel.mr_stone = 0;
    finBosslevel.viking_mama = 0;
    finBosslevel.space_mama = 0;
    finBosslevel.mr_skops = 0;
    finBosslevel.mr_dark = 0;
    finBosslevel.crazy_drummer = 0;
    finBosslevel.helped_joe_1 = 0;
    finBosslevel.helped_joe_2 = 0;
    finBosslevel.helped_musician = 0;
}

//1F8A4
/* Parameter/local names INVENTED from the PROVEN computation (DOS 1:0f8a4); meanings in docs/semantics/basic.json. */
void Change_Wait_Anim(void) {
    eta_t** ray_eta = ray.eta;
    u8 previous_idle_animation = ray_eta[0][0].anim_index;
    ray_eta[3][20].anim_index = ray_eta[0][56].anim_index;
    ray_eta[0][0].anim_index = ray_eta[0][56].anim_index;
    ray_eta[0][56].anim_index = previous_idle_animation;
    first_boss_meet = 1 - first_boss_meet;
}

//1F8E8
void add_alwobj(obj_t* obj) {
    s16 index = level_obj.nb_objects;
    level_obj.nb_objects++;
    level_obj.obj_ids[index] = obj->id;
}

//1F918
void del_alwobj(s16 obj_index) {
    s16 i;
    for (i = 0; i < level_obj.nb_objects; i++) {
        if (obj_index == level_obj.obj_ids[i])
            break;
    }
    if (i < level_obj.nb_objects) {
        --level_obj.nb_objects;
        level_obj.obj_ids[i] = level_obj.obj_ids[level_obj.nb_objects];
    }
}

//1F988
void FatalError(char* message) {
    stop_cd();
    InitTextMode();
    printf("Rayman says fatal error :\n%s\n", message);
    printf("System info : \n");
    printf("  - errno = %d\n", errno);
    printf("\n");
    PrintMemInfo();
    if (input_mode != 1) {
        printf("\nPress a key to exit.\n");
        getch();
    }
    exit(-1);
}

//1FA00
u8 sprite_of_ref(obj_t* obj, u8 ref) {
    s16 i;
    for (i = 0; i < (u16)obj->nb_sprites; i++) {
        if (obj->sprites[i].id == ref) return i;
    }
    return 0;
}

