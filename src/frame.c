/* 1:2b4d0..1:2e580: frame.c tail (fades, palettes, plan3) + guetteur.c (pirate lookout); split candidate 1:2db40 (STRONG). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_frame. Pragmas declare assembly-routine register ABIs only. 20 functions not yet reconstructed. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <conio.h>

typedef struct vec2b_t { u8 x, y; } vec2b_t;
typedef void draw_func_t(s32 x, s32 sprite_field_A, s32 y, vec2b_t size, u8 *draw_buf, u8 *image_data);
typedef void fplot_func_t(u8 *buffer, s32 x, s32 y, u8 color);
typedef void void_func_t(void);
extern u8 *smap;
u8 *unidentified_record_allocator(s32 size);
void release_record(u8 *buffer);
#ifndef SGN
#define SGN(x) (x >= 0 ? (x > 0 ? 1 : 0) : -1)
#endif
#ifndef ashr16
#define ashr16(v, b) ((v) >> (b))
#endif

#define MODE_TEXT 255
#define MODE_NORMAL 0
#define MODE_X 1
#define TYPE_PIRATE_BOMB 78
#define TYPE_PIRATE_GUETTEUR 77
#define TYPE_PIRATE_GUETTEUR2 239
#define TYPE_158_PALETTE_SWAPPER 158
#define Abs(value) ((value) >= 0 ? (value) : -(value))
extern u8 *DisplayBufferNormal;
extern u8 *DrawBufferNormal;
extern u8 ModeVideoActuel;
extern s16 bateau_obj_id;
extern u8 current_pal_id;
extern rgb_palette_t current_rvb;
extern u8 fade;
extern s16 fade_speed;
extern u8 horloge[25];
extern level_t level;
extern u16 nb_fade;
extern poing_t poing;
extern obj_t ray;
extern rgb_palette_t rvb[3];
extern u16 rvb_fade[256 * 3];
extern rgb_palette_t rvb_save;
extern u8 save_current_pal;
extern u16 type_zdc[262];
extern u8 *draw_buffer;
extern u8 *display_buffer;
extern u8 *EffetBufferNormal;
extern u8 *PLAN3BIT;
extern u8 *PLANTMPBIT;
extern u8 *PLANVIGBIT[7];
extern s32 plan3bit_length;
extern s32 plan3bit_nb_bytes;
extern s32 plantmp_length;
extern s32 plantmp_nb_bytes;
extern s32 planVigbit_length[7];
extern s32 planVigbit_nb_bytes[7];
extern u8 display_Vignet;
extern u8 last_plan1_palette;
extern s16 num_world;
extern s16 num_level;
extern rgb_palette_t *rvb_special;
extern s32 CompteurEclair;
extern s16 ProchainEclair;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W2;
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H2;
extern s32 prev_Bloc_lim_W1;
extern u8 OptionGame;
extern draw_func_t *DrawSpriteNoClipNormalEtX;
extern draw_func_t *DrawSpriteFlipNoClipNormalEtX;
extern draw_func_t *DrawSpriteColorNormalEtX;
extern draw_func_t *DrawSpriteColorFlipNormalEtX;
extern draw_func_t *DrawSpriteFlipNormalEtX;
extern draw_func_t *DrawSpriteNormalEtX;
extern void_func_t *drawflocon1NormalETX;
extern void_func_t *drawflocon2NormalETX;
extern void_func_t *drawflocon3NormalETX;
extern void_func_t *drawflocon4NormalETX;
extern void_func_t *drawflocon5NormalETX;
extern void_func_t *drawflocon6NormalETX;
extern void_func_t *drawflocon7NormalETX;
extern void_func_t *drawpluie4NormalETX;
extern void_func_t *drawpluie5NormalETX;
extern void_func_t *drawpluie6NormalETX;
extern void_func_t *drawpluie7NormalETX;
extern fplot_func_t *fplotNormalETX;
extern active_objects_t actobj;
extern u8 is_joypad_mode;
extern u8 Main_Control;
typedef s32 (swap_func_t)(u8 *, u8 *, s32, s32);
#pragma aux swap_func_t parm [edi] [esi] [ecx] [eax]
extern swap_func_t *Swap_And_Test_Joystick;
void advance_frame(void);
void set_fade_palette(rgb_palette_t *palette);
void do_fade(rgb_palette_t *source_pal, rgb_palette_t *dest_pal);
void fade_out(s16 speed, rgb_palette_t *palette);
void cyclage_palette(s16 first, s16 low, s16 high);
void DO_SWAP_PALETTE(void);
void SYNCHRO_LOOP(scene_func_t scene_func);
void DISPLAY_ANYSIZE_PICTURE(void *image, s16 arg2, s16 arg3, s16 x, s16 y, s16 h, s16 w);
void SAVE_PLAN3(void);
void RESTORE_PLAN3(void);
void DISPLAY_FOND3(void);
void DISPLAY_FOND_MENU(void);
void InitPaletteSpecialPC(void);
void DoFadePaletteSpecialPC(s16 start, s16 end);
void DoPaletteSpecialPC(void);
void SWAP_BUFFERS(void);
void calc_obj_dir(obj_t *obj);
u8 EOA(obj_t *obj);
void SET_X_SPEED(obj_t *obj);
void synchro(void);
void DisplayBufferModeNormal(u8 *source, u8 *dest);
void select_display_buffer(u8 *buffer);
#pragma aux DisplayBufferModeNormal parm [esi] [edi]
#pragma aux display_emptypicture parm [edi] [ecx]
#pragma aux ClearDrawAndDisplayBufferNormal parm [edi] [esi]
void ClearBorder(s16 h1, s16 h2, s16 w1, s16 w2);
void InitFirstModeVideo(void);
void InitModeNormal(void);
void GetVideoRegister(void);
void set_vga_frequency(u8 freq);
extern draw_func_t DrawSpriteNormalNoClip;
extern draw_func_t DrawSpriteFlipNormalNoClip;
extern draw_func_t DrawSpriteColorNormal;
extern draw_func_t DrawSpriteColorFlipNormal;
extern draw_func_t DrawSpriteNormal;
extern draw_func_t DrawSpriteFlipNormal;
extern void_func_t draw_flocon1_Normal;
extern void_func_t draw_flocon2_Normal;
extern void_func_t draw_flocon3_Normal;
extern void_func_t draw_flocon4_Normal;
extern void_func_t draw_flocon5_Normal;
extern void_func_t draw_flocon6_Normal;
extern void_func_t draw_flocon7_Normal;
extern void_func_t draw_pluie5_Normal;
extern void_func_t draw_pluie6_Normal;
extern void_func_t draw_pluie7_Normal;
extern fplot_func_t fplot_Normal;
void BombExplosion(obj_t* obj);
void ClearDrawAndDisplayBufferNormal(u8* draw_buf, u8* display_buf);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void PlaySnd_old(s16 sound_id);
void SetPalette(rgb_palette_t* palette, u8 first_color, s32 num_colors);
void add_alwobj(obj_t* obj);
void calc_obj_pos(obj_t* obj);
void display_emptypicture(u8 *buffer, s32 count);
void endsynchro(void);
short myRand(short max);
void obj_hurt(obj_t* target);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
u8 skipOneCommand(obj_t *obj);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void snapToSprite(obj_t* obj_1, obj_t* obj_2, u8 obj_2_spr, s32 x_offs, s16 y_offs);
void textmode(void);
void clear_palette(rgb_palette_t* palette);
void start_fade_in(s16 speed, rgb_palette_t *palette);
void start_fade_out(s16 speed, rgb_palette_t *palette);
void actualize_palette(u8 new_pal_id);
void DO_FADE(void);
void INIT_FADE_IN(void);
void INIT_FADE_OUT(void);
void DO_FADE_OUT(void);
void EFFACE_VIDEO(void);
void SAVE_PALETTE(rgb_palette_t* palette);
void RESTORE_PALETTE(void);
void InitTextMode(void);
void swapGuetteurCollZones(obj_t *obj, u8 param_2);
void guetteurFollowsShip(obj_t *obj);
s32 hasGuetteurABomb(obj_t* obj);
void allocatePirateGuetteurBomb(obj_t* gue_obj, s32 param_2, u8 param_3, u8 param_4);
void DO_PAR_TIR(obj_t* obj);
void DO_PAR_POING_COLLISION(obj_t *obj, s16 param_2);
void PAR_REACT_TO_RAY_IN_ZONE(obj_t* obj);
void DO_PAR_BOMB_COMMAND(obj_t *obj);

void SWAP_BUFFERS(void) {
    if (ModeVideoActuel == MODE_X) {
        if (display_buffer == (u8*)0xa0500) {
            draw_buffer = display_buffer;
            display_buffer = (u8*)0xa6500;
        } else {
            display_buffer = (u8*)0xa0500;
            draw_buffer = (u8*)0xa6500;
        }
        select_display_buffer(display_buffer);
    } else {
        draw_buffer = DrawBufferNormal;
        display_buffer = DisplayBufferNormal;
        if (is_joypad_mode == 1 && Main_Control)
            Swap_And_Test_Joystick(display_buffer, DrawBufferNormal, 320, 200);
        else
            DisplayBufferModeNormal(DrawBufferNormal, display_buffer);
    }
}

/* sub_3B580: no seed source */

/* sub_3B5E8: no seed source */

/* calc_gros_type: no seed source */

u8 find_in_map(s16 x, s16 y) {
    return smap[(y >> 4) * 40 + (x >> 4)];
}

void init_find_in_map(void) {
    s16 i;
    smap = unidentified_record_allocator(0x4b0);
    for (i = 0; i < 0x4b0; ++i)
        smap[i] = 1;
}

void end_find_in_map(void) {
    release_record(smap);
}

/* build_map: no seed source */

/* build_line_map: no seed source */

/* build_column_map: no seed source */

/* update_map: no seed source */

/* sub_3BE20: no seed source */

/* sub_3BEE0: no seed source */

/* set_vga_frequency: no seed source */

/* GetVideoRegister: no seed source */

/* SetVideoRegister: no seed source */

/* sub_3C3BC: no seed source */

void clear_palette(rgb_palette_t* palette) {
    s16 i = 0;
    u8 *p = (u8*)palette;
    u16 *q = rvb_fade;
    do {
        ++i;
        *p++ = 0;
        *q++ = 0;
    } while (i < 0x300);
}

void set_fade_palette(rgb_palette_t* palette) {
    s16 i;
    u16 *q = rvb_fade;
    u8 *p = (u8*)palette;
    u8 c1;
    u32 temp;
    for (i = 0; i < 256 * 3; ++i) {
        c1 = *p++;
        temp = c1 << 6;
        *q++ = temp;
    }
}

void start_fade_in(s16 speed, rgb_palette_t *palette) {
    (void)palette;
    fade_speed = speed;
    nb_fade = 1 << (6 - speed);
    fade = 1; // fade in
    clear_palette(&current_rvb);
    outp(0x3c8, 0);
    _disable();
    {
        u8 *p = (u8*)&current_rvb;
        u16 i;
        for (i = 0; i < 0x300; ++i) outp(0x3c9, *p++);
    }
    _enable();
}

void start_fade_out(s16 speed, rgb_palette_t *palette) {
    (void)palette;
    nb_fade = 1 << (6 - speed);
    fade = 2; // fade out
    fade_speed = speed;
}

void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
    if (nb_fade > 0) {
        s32 i;
        --nb_fade;
        if (fade == 1) {
            for (i = 0; i < 256 * 3; ++i) {
                u8 c1 = ((u8*)source_pal->colors)[i];
                u32 temp = c1 << fade_speed;
                rvb_fade[i] += temp;
            }
        } else if (fade == 2) {
            for (i = 0; i < 256 * 3; ++i) {
                if (rvb_fade[i] > 0) {
                    u8 c1 = ((u8*)source_pal->colors)[i];
                    u32 temp = c1 << fade_speed;
                    rvb_fade[i] -= temp;
                }
            }
        }
        for (i = 0; i < 256 * 3; ++i) {
            u16 temp = rvb_fade[i] >> 6;
            ((u8*)dest_pal->colors)[i] = (u8)temp;
        }
        SetPalette(&current_rvb, 0, 256);
        if (nb_fade == 0) fade |= 0x40;
    }
}

void fade_out(s16 speed, rgb_palette_t* palette) {
    u16 i;
    start_fade_out(speed, palette);
    for (i = 0; i < nb_fade; ++i) {
        endsynchro();
        synchro();
        do_fade(palette, &current_rvb);
    }
}

void actualize_palette(u8 new_pal_id) {
    if (new_pal_id != current_pal_id) {
        current_pal_id = new_pal_id;
        memcpy(&current_rvb, &rvb[new_pal_id], sizeof(current_rvb));
        set_fade_palette(&current_rvb);
        if (fade == 65) {
            SetPalette(&current_rvb, 0, 256);
        }
    }
}

/* cyclage_palette: no seed source */

void DO_SWAP_PALETTE(void) {
    s32 i;
    if (last_plan1_palette != 0) {
        if (fade & 0x40) {
            for (i = 0; i < actobj.num_active_objects; ++i) {
                obj_t* obj = level.objects + actobj.objects[i];
                if (obj->type == TYPE_158_PALETTE_SWAPPER) {
                    u8 pal_a = 0;
                    u8 pal_b = 0;
                    u8 new_pal = current_pal_id;
                    s16 ray_pos;
                    s16 obj_pos;
                    switch(obj->sub_etat) {
                        default: break;
                        case 0:
                            pal_a = 1;
                            pal_b = 0;
                            goto SwapHorizontal;
                        case 1:
                            pal_a = 2;
                            pal_b = 0;
                            goto SwapHorizontal;
                        case 2:
                            pal_a = 2;
                            pal_b = 1;
                            goto SwapHorizontal;
                        case 3:
                            pal_a = 0;
                            pal_b = 1;
                            goto SwapHorizontal;
                        case 4:
                            pal_a = 0;
                            pal_b = 2;
                            goto SwapHorizontal;
                        case 5:
                            pal_a = 1;
                            pal_b = 2;
                            goto SwapHorizontal;
                        case 6:
                            pal_a = 1;
                            pal_b = 0;
                            goto SwapVertical;
                        case 7:
                            pal_a = 2;
                            pal_b = 0;
                            goto SwapVertical;
                        case 8:
                            pal_a = 2;
                            pal_b = 1;
                            goto SwapVertical;
                        case 9:
                            pal_a = 0;
                            pal_b = 1;
                            goto SwapVertical;
                        case 10:
                            pal_a = 0;
                            pal_b = 2;
                            goto SwapVertical;
                        case 11:
                            pal_a = 1;
                            pal_b = 2;
                            goto SwapVertical;

                        SwapVertical:
                            ray_pos = ray.y + ray.offset_by;
                            obj_pos = obj->y + obj->offset_by;
                            if (ray_pos > obj_pos) {
                                new_pal = pal_a;
                            } else if (ray_pos < obj_pos) {
                                new_pal = pal_b;
                            }
                            break;

                        SwapHorizontal:
                            ray_pos = ray.x + ray.offset_bx;
                            obj_pos = obj->x + obj->offset_bx;
                            if (ray_pos > obj_pos) {
                                new_pal = pal_a;
                            } else if (ray_pos < obj_pos) {
                                new_pal = pal_b;
                            }
                            break;
                    }
                    actualize_palette(new_pal);
                }
            }
        }
    }
}


void DO_FADE(void) {
    rgb_palette_t* palette = rvb + current_pal_id;
    do_fade(palette, &current_rvb);
}

void INIT_FADE_IN(void) {
    start_fade_in(2, rvb + current_pal_id);
}

void INIT_FADE_OUT(void) {
    start_fade_out(2, rvb + current_pal_id);
}

void DO_FADE_OUT(void) {
    fade_out(2, rvb + current_pal_id);
}

void EFFACE_VIDEO(void) {
    endsynchro();
    if (ModeVideoActuel == MODE_X) {
        SWAP_BUFFERS();
        display_emptypicture(draw_buffer, 0x3e80);
        display_emptypicture(display_buffer, 0x3e80);
    } else {
        ClearDrawAndDisplayBufferNormal(DrawBufferNormal, DisplayBufferNormal);
    }
}

/* SYNCHRO_LOOP: no seed source */

/* DISPLAY_ANYSIZE_PICTURE: no seed source */

void SAVE_PALETTE(rgb_palette_t* palette) {
    save_current_pal = current_pal_id;
    memcpy(&rvb_save, &rvb[0], sizeof(rvb_save));
    memcpy(&rvb[0], palette, sizeof(rvb[0]));
    current_pal_id = 0;
    memcpy(&current_rvb, palette, sizeof(current_rvb));
}

void RESTORE_PALETTE(void) {
    current_pal_id = save_current_pal;
    memcpy(&rvb[0], &rvb_save, sizeof(rvb[0]));
}

void SAVE_PLAN3(void) {
    PLANTMPBIT = PLAN3BIT;
    plantmp_length = plan3bit_length;
    plantmp_nb_bytes = plan3bit_nb_bytes;
    PLAN3BIT = PLANVIGBIT[display_Vignet];
    plan3bit_length = planVigbit_length[display_Vignet];
    plan3bit_nb_bytes = planVigbit_nb_bytes[display_Vignet];
}

void RESTORE_PLAN3(void) {
    PLAN3BIT = PLANTMPBIT;
    plan3bit_length = plantmp_length;
    plan3bit_nb_bytes = plantmp_nb_bytes;
}

void DISPLAY_FOND3(void) {
    memcpy(DrawBufferNormal, PLAN3BIT, 320 * 200);
}

void DISPLAY_FOND_MENU(void) {
    s32 v1;
    s32 x;
    s16 row;
    u8 *dest_pos;
    u8 *source_pos;
    v1 = 320 - (s16)Bloc_lim_W2 + (s16)Bloc_lim_W1;
    if (OptionGame) {
        if (prev_Bloc_lim_W1 != Bloc_lim_W1) {
            if (prev_Bloc_lim_W1 < Bloc_lim_W1)
                ClearBorder((s16)(Bloc_lim_H1 >> 16), (s16)(Bloc_lim_H2 >> 16),
                            (s16)(Bloc_lim_W1 >> 16), (s16)(Bloc_lim_W2 >> 16));
            Bloc_lim_W1 = prev_Bloc_lim_W1;
        }
        memset(draw_buffer, 0, Bloc_lim_H1 * 320);
        dest_pos = draw_buffer + Bloc_lim_H1 * 320 + Bloc_lim_W1;
        source_pos = EffetBufferNormal + Bloc_lim_H1 * 320 + Bloc_lim_W1;
        row = (s16)Bloc_lim_H1;
        while ((s32)row < Bloc_lim_H2) {
            x = (s16)Bloc_lim_W1;
            while (x < (s16)Bloc_lim_W2) {
                *dest_pos++ = *source_pos++;
                ++x;
            }
            dest_pos += v1;
            source_pos += v1;
            ++row;
        }
        memset(draw_buffer, 0, (200 - Bloc_lim_H2) * 320);
    } else {
        memcpy(draw_buffer, EffetBufferNormal, 320 * 200);
    }
}

/* InitPaletteSpecialPC: no seed source */

/* DoFadePaletteSpecialPC: no seed source */

/* DoPaletteSpecialPC: no seed source */

/* InitModeXWithFrequency: no seed source */

void InitTextMode(void) {
    textmode();
    ModeVideoActuel = MODE_TEXT;
}

void InitModeNormalWithFrequency(u8 freq) {
    if (ModeVideoActuel == MODE_TEXT) {
        InitFirstModeVideo();
        GetVideoRegister();
    }
    if (ModeVideoActuel != MODE_NORMAL) {
        InitModeNormal();
        memset(DrawBufferNormal, 0, 320 * 200);
        memset(DisplayBufferNormal, 0, 320 * 200);
        select_display_buffer(DisplayBufferNormal);
        set_vga_frequency(freq);
        ModeVideoActuel = MODE_NORMAL;
        DrawSpriteNoClipNormalEtX = &DrawSpriteNormalNoClip;
        DrawSpriteFlipNoClipNormalEtX = &DrawSpriteFlipNormalNoClip;
        DrawSpriteColorNormalEtX = &DrawSpriteColorNormal;
        DrawSpriteColorFlipNormalEtX = &DrawSpriteColorFlipNormal;
        draw_buffer = DrawBufferNormal;
        DrawSpriteFlipNormalEtX = &DrawSpriteFlipNormal;
        DrawSpriteNormalEtX = &DrawSpriteNormal;
        drawflocon1NormalETX = &draw_flocon1_Normal;
        drawflocon2NormalETX = &draw_flocon2_Normal;
        drawflocon3NormalETX = &draw_flocon3_Normal;
        drawflocon4NormalETX = &draw_flocon4_Normal;
        drawflocon5NormalETX = &draw_flocon5_Normal;
        drawflocon6NormalETX = &draw_flocon6_Normal;
        drawflocon7NormalETX = &draw_flocon7_Normal;
        drawpluie4NormalETX = &draw_flocon4_Normal;
        drawpluie5NormalETX = &draw_pluie5_Normal;
        drawpluie6NormalETX = &draw_pluie6_Normal;
        drawpluie7NormalETX = &draw_pluie7_Normal;
        fplotNormalETX = &fplot_Normal;
    }
}

void WaitNSynchro(s32 n_frames) {
    s32 i;
    for (i = 0; i < n_frames; ++i) {
        endsynchro();
        synchro();
    }
}

void swapGuetteurCollZones(obj_t *obj, u8 param_2)
{
    obj->type = param_2 ? TYPE_PIRATE_GUETTEUR : TYPE_PIRATE_GUETTEUR2;
    obj->zdc = type_zdc[obj->type];
}

void guetteurFollowsShip(obj_t *obj)
{
    if (bateau_obj_id != -1)
        snapToSprite(obj, &level.objects[bateau_obj_id], 1, -52, -63);
    else
        obj->speed_x = obj->speed_y;
}

void DO_ONE_PAR_COMMAND(obj_t* obj) {
    s32 flip_x;
    if (obj->timer != 0) obj->timer--;
    flip_x = obj->flags.flip_x;
    if (obj->type == TYPE_PIRATE_GUETTEUR2)
        calc_obj_dir(obj);
    else
        guetteurFollowsShip(obj);
    switch (obj->main_etat * 0x100 + obj->sub_etat) {
        case 0x3:
            obj->speed_y = -8;
            obj->speed_x = 0;
            break;
        case 0x2:
            obj->flags.flip_x = flip_x;
        case 0x5:
        case 0xd:
            obj->timer = 50;
            DO_PAR_TIR(obj);
            break;
        case 0x0:
        case 0x11:
            if (poing.is_active) set_sub_etat(obj, 15);
            break;
        case 0x8:
        case 0xb:
        case 0x14:
            if (poing.is_active) set_sub_etat(obj, 18);
            break;
        case 0x10:
            if (!poing.is_active) set_sub_etat(obj, 17);
            break;
        case 0x13:
            if (!poing.is_active) set_sub_etat(obj, 20);
            break;
        case 0x6:
            obj->flags.flip_x = flip_x;
            if (EOA(obj)) {
                swapGuetteurCollZones(obj, 0);
                obj->speed_x = 0;
            }
            break;
        case 0x9:
            obj->flags.flip_x = flip_x;
            if (EOA(obj)) {
                SET_X_SPEED(obj);
                obj->speed_y = -3;
            }
            break;
        case 0x200:
            obj->flags.flip_x = flip_x;
            SET_X_SPEED(obj);
            break;
        case 0xa:
            obj->flags.flip_x = flip_x;
            break;
        case 0x1:
            obj->iframes_timer = 100;
            break;
    }
    if (obj->iframes_timer != 0) obj->iframes_timer--;
}

s32 hasGuetteurABomb(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s16 unk_1;
    anim_t *anim;
    anim_layer_t *unk_2;

    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 4) {
        unk_1 = obj->type == TYPE_PIRATE_GUETTEUR ? 0 : 5;
        anim = &obj->animations[obj->anim_index];
        unk_2 = &anim->layers[(anim->layers_per_frame & 0x3fff) * obj->anim_frame];
        if (unk_2[unk_1].sprite_index == 0)
            unk_1 = -1;
    }

    return unk_1;
}

void allocatePirateGuetteurBomb(obj_t* gue_obj, s32 param_2, u8 param_3, u8 param_4) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    s32 unk_1;
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;
    s16 gue_x; s16 gue_y; s16 gue_w; s16 gue_h;
    s16 unk_x;
    s16 new_spd_x;
    u8 mul_x_pos;
    eta_t *cur_eta_1;
    eta_t *cur_eta_2;
    eta_t *cur_eta_3;

#ifdef NUGGET
    /* reg s3 set to 2 at 801424dc. but can have other values? */
    mul_x_pos = 2;
#endif
    unk_1 = hasGuetteurABomb(gue_obj);
    if ((s16) unk_1 != -1) {
        i = 0;
        cur_obj = &level.objects[i];
        nb_objs = level.nb_objects;
        while (i < nb_objs) {
            if (cur_obj->type == TYPE_PIRATE_BOMB) {
                if (!cur_obj->is_active) {
                    if (param_3)
                        cur_obj->flags.flip_x = gue_obj->flags.flip_x;
                    else
                        cur_obj->flags.flip_x = gue_obj->flags.flip_x ^ 1;

                    cur_obj->speed_y = param_2;
                    GET_SPRITE_POS(gue_obj, (s16) unk_1, &gue_x, &gue_y, &gue_w, &gue_h);
                    cur_obj->x = gue_x - cur_obj->offset_bx + (gue_w >> 1);
                    cur_obj->y = gue_y + gue_h - cur_obj->offset_by;
                    switch (gue_obj->sub_etat) {
                        case 2:
                        case 0x12:
                            new_spd_x = 48;
                            mul_x_pos = 3;
                            cur_obj->y -= 1;
                            cur_eta_1 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_1->anim_speed = (cur_eta_1->anim_speed & 0xF) | 0xA0;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 10;
                            break;
                        case 5:
                            new_spd_x = 32;
                            mul_x_pos = 3;
                            cur_obj->y += 24;
                            cur_eta_2 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_2->anim_speed = (cur_eta_2->anim_speed & 0xF) | 0x20;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 0;
                            break;
                        case 0x0D:
                            unk_x = 8 - myRand(32);
                            unk_x += 8 * SGN(unk_x);
                            new_spd_x = unk_x;
                            mul_x_pos = 0;
                            cur_eta_3 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_3->anim_speed = (cur_eta_3->anim_speed & 0xF) | 0x20;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 0;
                            cur_obj->y -= 20;
                            break;
                    }
                    if (param_3 == 0)
                        new_spd_x = 16;

                    if (gue_obj->flags.flip_x) /* ternary-free zone */
                        cur_obj->speed_x = new_spd_x;
                    else
                        cur_obj->speed_x = -new_spd_x;

                    cur_obj->x += mul_x_pos * ashr16(cur_obj->speed_x, 4);
                    skipToLabel(cur_obj, cur_obj->flags.flip_x, 1);
                    calc_obj_pos(cur_obj);
                    cur_obj->is_active = 1;
                    cur_obj->flags.alive = 1;
                    add_alwobj(cur_obj);
                    cur_obj->timer = param_4;
                    cur_obj->flags.flag_0x40 = 0;
                    break;
                }
            }
            cur_obj++;
            i++;
        }
    }
}

void DO_PAR_TIR(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    if (obj->main_etat == 0) {
        if (!((obj->sub_etat == 2 && obj->anim_frame >= 21) ||
              (obj->sub_etat == 5 && obj->anim_frame >= 18) ||
              (obj->sub_etat == 13 && obj->anim_frame >= 2))) {
            obj->link = 0;
        }

        if (obj->link == 0) {
            if ((obj->sub_etat == 2 && obj->anim_frame == 21) ||
                (obj->sub_etat == 5 && obj->anim_frame == 18) ||
                (obj->sub_etat == 13 && obj->anim_frame == 2)
            ) {
                if (horloge[obj->eta[obj->main_etat][obj->sub_etat].anim_speed & 0xf] == 0) {
                    if (obj->sub_etat == 5)
                        allocatePirateGuetteurBomb(obj, 2, 1, 40);
                    else if (obj->sub_etat == 2)
                        allocatePirateGuetteurBomb(obj, -1, 1, 40);
                    else {
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x = !obj->flags.flip_x;
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x = !obj->flags.flip_x;
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x = !obj->flags.flip_x;
                        allocatePirateGuetteurBomb(obj, -3, 1, 100);
                        obj->flags.flip_x = !obj->flags.flip_x;
                    }
                    obj->link = 1;
                }
            }
        }
    } else {
        obj->link = 0;
    }
}

void DO_PAR_POING_COLLISION(obj_t *obj, s16 param_2)
{
    if (
        (obj->eta[obj->main_etat][obj->sub_etat].flags & 1) &&
        obj->iframes_timer == 0 &&
        (obj->type == TYPE_PIRATE_GUETTEUR || param_2 == 3)
    )
    {
        if (obj->type == TYPE_PIRATE_GUETTEUR2 &&
            (obj->eta[obj->main_etat][obj->sub_etat].flags & 4))
            allocatePirateGuetteurBomb(obj, 1, 0, 20);

        poing.damage = 1;
        obj_hurt(obj);

        if (obj->hit_points == 0)
            set_main_and_sub_etat(obj, 0, 3);
        else
        {
            if (obj->type == TYPE_PIRATE_GUETTEUR)
            {
                if (obj->init_hit_points - obj->hit_points >= 3)
                {
                    set_sub_etat(obj, 6);
                    if (bateau_obj_id != -1)
                    {
                        skipOneCommand(&level.objects[bateau_obj_id]);
                        level.objects[bateau_obj_id].nb_cmd = 0;
                    }
                }
                else
                    set_sub_etat(obj, 12);
            }
            else
                set_main_and_sub_etat(obj, 0, 1);
        }
    }
}

void PAR_REACT_TO_RAY_IN_ZONE(obj_t* obj) {
    // NOTE(Falcury): PS1 and PC versions seem functionally identical.
    u8 sub_etat;

    if (!obj->timer && obj->main_etat == 0) {
        sub_etat = obj->sub_etat;
        if (sub_etat == 11)
            set_sub_etat(obj, 2);
        else if (sub_etat == 0) {
            obj->sub_speed = (obj->offset_bx + obj->x) - (ray.x + ray.offset_bx);
            if (Abs(obj->sub_speed) > obj->detect_zone + 16)
                set_sub_etat(obj, 8);
            else
                set_sub_etat(obj, 9);
        }
        else if (sub_etat == 4)
            set_sub_etat(obj, 5);
    }
}

void DO_PAR_BOMB_COMMAND(obj_t *obj)
{
    if (obj->timer != 0)
    {
        if (horloge[2] != 0)
            obj->timer--;
    }
    else
    {
        PlaySnd_old(147);
        BombExplosion(obj);
    }
}
