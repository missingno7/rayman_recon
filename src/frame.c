/* 1:2b4d0..1:2e580: frame.c tail (fades, palettes, plan3, video modes, map building) + guetteur.c (pirate lookout); split candidate 1:2db40 (STRONG). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_frame. Pragmas declare assembly-routine register ABIs only. 3 renderer functions not yet reconstructed. */
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
extern u8 use_sync;
extern u8 *unk_ptr_3d840;
extern u32 gros_patai_src[1200];
extern s32 planar_copy_offset;
extern u8 type_fndscrX;
extern map_data_t mp;
extern u8 *MAP_BLOCKS;
extern u8 *PLAN1_BUFFER;
extern u32 PLAN1_BUFFER_START_POINTER;
extern s32 plan0bit_length;
extern u8 *PLAN0BIT;
extern s32 plan2bit_length;
extern u8 *PLAN2BIT;
extern s32 plan2bit_nb_bytes;
extern rgb_palette_t *rvb_special;
extern s16 numero_palette_special;
extern s32 CompteurEclair;
extern u8 saved_vga_registers[6];
extern u8 VGA_FREQ;
extern u8 need_timer;
extern u8 in_pause;
typedef void block_copy_t(u8 *source, u8 *dest);
typedef void block_clear_t(u8 *dest);
#pragma aux block_copy_t parm [esi] [edi]
#pragma aux block_clear_t parm [edi]
extern block_copy_t block_copy;
extern block_clear_t block_clear;
typedef void display_anypicture_t(s32 height, s32 stride_and_plane, s32 source_width, u8 *source, u8 *dest);
#pragma aux display_anypicture_t parm [eax] [edx] [ecx] [esi] [edi]
extern display_anypicture_t display_anypicture;
extern void speaker_enable(void);
extern s32 clock_ticks(void);
extern scene_func_t dummy_prg;
u8 *unidentified_record_allocator(s32 size);
void release_record(u8 *buffer);
typedef void delay_500_t(s32 milliseconds);
#pragma aux delay_500_t parm [eax]
extern delay_500_t wait_milliseconds;
typedef void bios_wait_timer_t(s32 high, s32 low);
#pragma aux bios_wait_timer_t parm [ecx] [edx]
extern bios_wait_timer_t bios_wait_timer;
typedef void copy_dwords_t(u32 *source, u32 *dest, s32 count);
#pragma aux copy_dwords_t parm [esi] [edi] [ecx]
extern copy_dwords_t copy_dwords;
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
extern draw_func_t DrawSpriteNoClipX;
extern draw_func_t DrawSpriteFlipNoClipX;
extern draw_func_t DrawSpriteColorX;
extern draw_func_t DrawSpriteColorFlipX;
extern draw_func_t DrawSpriteX;
extern draw_func_t DrawSpriteFlipX;
extern void_func_t mode_x_effect_2b3d;
extern void_func_t mode_x_effect_2c00;
extern void_func_t mode_x_effect_2c59;
extern void_func_t mode_x_effect_2d08;
extern void_func_t mode_x_effect_2bb7;
extern void_func_t mode_x_effect_2dfb;
extern void_func_t mode_x_effect_300f;
extern void_func_t mode_x_effect_2da9;
extern void_func_t mode_x_effect_2cbf;
extern void_func_t mode_x_effect_2e9c;
extern void_func_t mode_x_effect_2f0c;
extern fplot_func_t fplot_mode_x_2b86;
extern void InitModeX(void);
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

void sub_3B580(void) {
    if (use_sync)
        endsynchro();
    if (display_buffer == (u8*)0xa0500) {
        draw_buffer = display_buffer;
        display_buffer = (u8*)0xa6500;
    } else {
        display_buffer = (u8*)0xa0500;
        draw_buffer = (u8*)0xa6500;
    }
    select_display_buffer(display_buffer);
    if (use_sync)
        synchro();
}

s32 sub_3B5E8(void) {
    s32 iterations = 0;
    u8 *ready = unk_ptr_3d840;
    do {
        *ready = 0;
        bios_wait_timer(7, 0xa120);
        copy_dwords((u32*)0, (u32*)0xa0000, 0x3fff);
        ++iterations;
    } while (*ready == 0);
    return iterations / 8;
}

void calc_gros_type(void) {
    s32 map_offset;
    for (map_offset = 0; map_offset < mp.length; map_offset += 6) {
        map_tile_t *tile = (map_tile_t*)((u8*)mp.map + map_offset);
        u16 texture_id = tile->texture_id;
        {
            u8 *pixels = MAP_BLOCKS + 0x2800 +
                (texture_id / 40) * 16 * 640 + (texture_id % 40) * 16;
            s32 x;
            s32 y;
            s32 any_nonzero = 0;
            s32 all_nonzero = 1;
            for (y = 0; y < 16; ++y) {
                for (x = 0; x < 16; ++x) {
                    u8 pixel = pixels[y * 640 + x];
                    if (pixel != 0)
                        any_nonzero = 1;
                    else
                        all_nonzero = 0;
                }
            }
            if (all_nonzero)
                tile->transparency = 0;
            else if (!any_nonzero)
                tile->transparency = 1;
            else
                tile->transparency = 2;
        }
        if (tile->texture_id == 0 && tile->transparency != 1)
            tile->transparency = 1;
        if (tile->texture_id >= 0x4b0)
            tile->texture_id = 0;
        smap[tile->texture_id] = tile->transparency;
    }
}

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

void build_map(s16 x, s16 y) {
    s32 plane;
    s32 plane_offset = planar_copy_offset;
    s32 start_tile = ((s32)(y >> 4) * mp.width) + (x >> 4);
    u8 *buffer = PLAN1_BUFFER;
    PLAN1_BUFFER_START_POINTER = (u32)buffer;
    if (type_fndscrX != 0)
        plane_offset = 1;
    for (plane = 0; plane < 4; ++plane) {
        s32 row;
        map_tile_t *row_tile = mp.map + start_tile;
        u8 *row_dest = buffer;
        for (row = 0; row < 14; ++row) {
            s32 col;
            map_tile_t *tile = row_tile;
            u8 *dest = row_dest;
            for (col = 0; col < 20; ++col) {
                if ((s16)tile->texture_id >= 0 &&
                    tile->texture_id < 0x4b0 && tile->transparency == 0) {
                    u8 *source = (u8*)gros_patai_src[tile->texture_id] + plane_offset;
                    block_copy(source, dest);
                } else {
                    block_clear(dest);
                }
                ++tile;
                dest += 4;
            }
            row_tile += mp.width;
            row_dest += 0x500;
        }
        buffer += 0x4600;
        plane_offset *= 2;
        if (plane_offset == 16)
            plane_offset = 1;
    }
}

void build_line_map(void *buffer, s16 x, s16 y, s16 row) {
    s32 xoff = (s16)x >> 4;
    s32 yoff = (s16)y >> 4;
    s32 source_row = ((u32)buffer - (u32)PLAN1_BUFFER) / 0x50;
    map_tile_t *tile = mp.map + yoff * mp.width + xoff;
    u8 *dest = PLAN1_BUFFER + (yoff * mp.width + xoff) * 6;
    s32 r;
    for (r = 0; r < 14; ++r) {
        s32 col;
        map_tile_t *cur = tile;
        u8 *out = dest;
        for (col = 0; col < 20; ++col) {
            if ((s16)cur->texture_id >= 0 && cur->texture_id < 0x4b0 && cur->transparency == 0) {
                u8 *src = (u8 *)gros_patai_src[cur->texture_id] + source_row * 0x280 + row;
                block_copy(src, out);
            }
            cur++;
            out += 4;
        }
        tile += mp.width;
        dest += 0x4600;
    }
}

void build_column_map(void *buffer, s16 x, s16 y, s16 column) {
    s32 xoff = (s16)x >> 4;
    s32 yoff = (s16)y >> 4;
    s32 source_row = ((u32)buffer - (u32)PLAN1_BUFFER) / 0x50;
    map_tile_t *tile = mp.map + yoff * mp.width + xoff;
    u8 *dest = PLAN1_BUFFER + (yoff * mp.width + xoff) * 6;
    s32 r;
    for (r = 0; r < 14; ++r) {
        s32 col;
        map_tile_t *cur = tile;
        u8 *out = dest;
        for (col = 0; col < 20; ++col) {
            if ((s16)cur->texture_id >= 0 && cur->texture_id < 0x4b0 && cur->transparency == 0) {
                u8 *src = (u8 *)gros_patai_src[cur->texture_id] + source_row * 0x280 + column;
                block_copy(src, out);
            }
            cur++;
            out += 4;
        }
        tile += mp.width;
        dest += 0x4600;
    }
}

void update_map(s16 x1, s16 y1, s16 x2, s16 y2) {
    s16 dx = (s16)((x2 & 0xfff0) - (x1 & 0xfff0));
    s16 dy = (s16)((y2 & 0xfff0) - (y1 & 0xfff0));
    if (dx == 0 && dy == 0)
        return;
    if (dx > 0) build_column_map(PLAN1_BUFFER, x2, y1, 0x130);
    else if (dx < 0) build_column_map(PLAN1_BUFFER, x2, y1, 0x130);
    if (dy > 0) build_line_map(PLAN1_BUFFER, x1, y2, 0xb0);
    else if (dy < 0) build_line_map(PLAN1_BUFFER, x1, y2, 0xb0);
}

/* sub_3BE20: entry stack local values and renderer-call layout still being reconstructed */
/* sub_3BEE0: renderer-call structure and map wrap logic still being reconstructed */

void GetVideoRegister(void) {
    saved_vga_registers[0] = (u8)inp(0x3cc);
    outp(0x3d4, 0x10);
    saved_vga_registers[1] = (u8)inp(0x3d5);
    outp(0x3d4, 0x11);
    saved_vga_registers[2] = (u8)inp(0x3d5);
    outp(0x3d4, 6);
    saved_vga_registers[3] = (u8)inp(0x3d5);
    outp(0x3d4, 7);
    saved_vga_registers[4] = (u8)inp(0x3d5);
    outp(0x3d4, 0x16);
    saved_vga_registers[5] = (u8)inp(0x3d5);
}

void SetVideoRegister(void) {
    outp(0x3c2, saved_vga_registers[0]);
    outp(0x3d4, 0x10);
    outp(0x3d5, saved_vga_registers[1]);
    outp(0x3d4, 0x11);
    outp(0x3d5, saved_vga_registers[2]);
    outp(0x3d4, 6);
    outp(0x3d5, saved_vga_registers[3]);
    outp(0x3d4, 7);
    outp(0x3d5, saved_vga_registers[4]);
    outp(0x3d4, 0x16);
    outp(0x3d5, saved_vga_registers[5]);
}

void set_vga_frequency(u8 freq) {
    register u8 current_freq = freq;
    int value;
    SetVideoRegister();
    if (current_freq == 100) {
        value = inp(0x3cc);
        value |= 8;
        value &= 0xfb;
        outp(0x3c2, value);
    } else if (current_freq == 80) {
        value = inp(0x3cc);
        value |= 4;
        value &= 0xf7;
        outp(0x3c2, value);
    } else if (current_freq == 70) {
        value = inp(0x3cc);
        value &= 0xf3;
        outp(0x3c2, value);
    } else if (current_freq == 60) {
        outp(0x3c2, 0x63);
        outp(0x3d4, 0x11);
        value = inp(0x3d5) & 0x7f;
        outp(0x3d5, value);
        outp(0x3d4, 6);
        outp(0x3d5, 0x0c);
        outp(0x3d4, 7);
        value = inp(0x3d5);
        value &= 0xde;
        value |= 0x20;
        outp(0x3d5, value);
        outp(0x3d4, 0x16);
        outp(0x3d5, 0xff);
        outp(0x3d4, 0x10);
        outp(0x3d5, 0xc4);
        outp(0x3d4, 0x11);
    }
    if (current_freq == 50) {
        outp(0x3d4, 0x11);
        value = inp(0x3d5) & 0x7f;
        outp(0x3d5, value);
        outp(0x3d4, 6);
        outp(0x3d5, 0x74);
        outp(0x3d4, 7);
        value = inp(0x3d5);
        value &= 0xde;
        value |= 0x20;
        outp(0x3d5, value);
        outp(0x3d4, 0x16);
        outp(0x3d5, 0xff);
        outp(0x3d4, 0x10);
        outp(0x3d5, 0xfc);
        outp(0x3d4, 0x11);
    }
    VGA_FREQ = current_freq;
    wait_milliseconds(500);
}

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

#define WRITE_PALETTE_BYTES(palette_ptr, color_count) \
    do { \
        s32 palette_byte; \
        u8 *palette_data = (u8*)(palette_ptr); \
        outp(0x3c8, 0); \
        for (palette_byte = 0; palette_byte < (color_count) * 3; ++palette_byte) \
            outp(0x3c9, palette_data[palette_byte]); \
    } while (0)

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
        WRITE_PALETTE_BYTES(&current_rvb, 256);
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
            WRITE_PALETTE_BYTES(&current_rvb, 256);
        }
    }
}

void cyclage_palette(s16 palette_id, s16 first_color, s16 last_color) {
    s16 i;
    rgb_t first = rvb[palette_id].colors[first_color];
    for (i = first_color; i < last_color; ++i)
        rvb[palette_id].colors[i] = rvb[palette_id].colors[i + 1];
    rvb[palette_id].colors[last_color] = first;
    memcpy(&current_rvb, &rvb[palette_id], sizeof(current_rvb));
    set_fade_palette(&current_rvb);
    if (nb_fade == 0)
        nb_fade = 1;
}

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

void SYNCHRO_LOOP(scene_func_t scene_func) {
    s16 scene_ended = 0;
    u8 do_fade = 1;
    u32 timer = 0;
    speaker_enable();
    do {
        if (ModeVideoActuel == MODE_X) {
            endsynchro();
            SWAP_BUFFERS();
            synchro();
            if ((u8)scene_ended != need_timer)
                timer = (u32)clock_ticks();
            if (do_fade == 0)
                DO_FADE();
        } else {
            endsynchro();
            synchro();
            if ((u8)scene_ended != need_timer)
                timer = (u32)clock_ticks();
            if (do_fade == 0)
                DO_FADE();
            SWAP_BUFFERS();
        }
        scene_ended = scene_func(timer);
        do_fade = (u8)scene_ended;
    } while (scene_ended == 0);
    if (scene_func != dummy_prg)
        in_pause = 0;
}

void DISPLAY_ANYSIZE_PICTURE(void *image, s16 source_x, s16 source_width,
                             s16 x, s16 y, s16 height, s16 width) {
    s32 source_length;
    s32 source_stride;
    s32 width_was_defaulted = 0;
    s32 source_plane = (u8)source_x & 3;
    s32 source_column = (s16)source_x >> 2;
    u8 *source;
    u8 *dest;
    if (image == PLAN2BIT) {
        source_length = plan2bit_length;
        source_stride = plan2bit_nb_bytes;
    } else {
        source_length = plan3bit_length;
        source_stride = plan3bit_nb_bytes;
    }
    if (width == 0) {
        width = (s16)source_stride;
        width_was_defaulted = 1;
    }
    if (height == 0)
        height = (s16)((source_length >> 2) / width - 4);
    if (width_was_defaulted)
        width = (s16)(width >> 2);
    source = (u8 *)image + source_plane * (source_length >> 2) + source_width * source_stride + source_column;
    dest = draw_buffer + y * 80 + (x >> 2);
    display_anypicture(height, ((1 << source_plane) << 8) + source_stride,
                       source_length >> 2, source, dest);
}

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

#define clip_palette_component(value) ((value) > 63 ? 63 : (u8)(value))

void InitPaletteSpecialPC(void) {
    s32 i;
    CompteurEclair = 0;
    if ((num_world == 1 && num_level == 9) ||
        (num_world == 2 && num_level == 4) ||
        (num_world == 4 && num_level == 9)) {
        for (i = 0; i < 256; ++i) {
            rgb_t src = rvb[0].colors[i];
            rvb_special[15].colors[i] = src;
            rvb_special[0].colors[i].r = clip_palette_component((4 * src.r) / 8);
            rvb_special[0].colors[i].g = clip_palette_component((4 * src.g) / 8);
            rvb_special[0].colors[i].b = clip_palette_component((4 * src.b) / 5);
            rvb_special[5].colors[i].r = clip_palette_component((4 * src.r) / 3);
            rvb_special[5].colors[i].g = clip_palette_component((4 * src.g) / 3);
            rvb_special[5].colors[i].b = clip_palette_component((4 * src.b) / 3);
            rvb_special[7].colors[i].r = clip_palette_component((4 * src.r) / 6);
            rvb_special[7].colors[i].g = clip_palette_component((4 * src.g) / 6);
            rvb_special[7].colors[i].b = clip_palette_component(src.b);
            rvb_special[10].colors[i].r = clip_palette_component(2 * src.r);
            rvb_special[10].colors[i].g = clip_palette_component(2 * src.g);
            rvb_special[10].colors[i].b = clip_palette_component(2 * src.b);
            rvb_special[14].colors[i].r = clip_palette_component((4 * src.r) / 8);
            rvb_special[14].colors[i].g = clip_palette_component((4 * src.g) / 8);
            rvb_special[14].colors[i].b = clip_palette_component((4 * src.b) / 5);
            rvb[0].colors[i] = rvb_special[0].colors[i];
        }
        DoFadePaletteSpecialPC(0, 5);
        DoFadePaletteSpecialPC(5, 7);
        DoFadePaletteSpecialPC(7, 10);
        DoFadePaletteSpecialPC(10, 14);
        numero_palette_special = 0;
        ProchainEclair = (s16)(myRand(200) + 1);
    } else if (num_world == 5 && num_level == 4) {
        for (i = 0; i < 255; ++i) {
            rgb_t src = rvb[0].colors[i];
            rvb_special[15].colors[i] = src;
            rvb_special[0].colors[i].r = clip_palette_component(src.r);
            rvb_special[0].colors[i].g = clip_palette_component((77 * src.g) / 100);
            rvb_special[0].colors[i].b = clip_palette_component((12 * src.b) / 100);
            rvb_special[14].colors[i] = rvb_special[0].colors[i];
            rvb[0].colors[i] = rvb_special[0].colors[i];
        }
        DoFadePaletteSpecialPC(0, 14);
        for (i = 128; i <= 174; ++i) {
            rgb_t src = rvb[0].colors[i];
            rvb_special[9].colors[i].r = clip_palette_component((84 * src.r) / 100);
            rvb_special[9].colors[i].g = clip_palette_component((62 * src.g) / 100);
            rvb_special[9].colors[i].b = clip_palette_component((12 * src.b) / 100);
            rvb[0].colors[i] = rvb_special[9].colors[i];
        }
        DoFadePaletteSpecialPC(0, 9);
        DoFadePaletteSpecialPC(9, 14);
    }
}

void DoFadePaletteSpecialPC(s16 start, s16 end) {
    s32 p;
    s32 i;
    s32 denom = end - start;
    if (start + 1 >= end)
        return;
    for (p = start + 1; p < end; ++p) {
        s32 from_weight = end - p;
        s32 to_weight = p - start;
        for (i = 0; i < 256; ++i) {
            rgb_t from = rvb_special[start].colors[i];
            rgb_t to = rvb_special[end].colors[i];
            rvb_special[p].colors[i].r = clip_palette_component((from.r * from_weight + to.r * to_weight) / denom);
            rvb_special[p].colors[i].g = clip_palette_component((from.g * from_weight + to.g * to_weight) / denom);
            rvb_special[p].colors[i].b = clip_palette_component((from.b * from_weight + to.b * to_weight) / denom);
        }
    }
}

void DoPaletteSpecialPC(void) {
    if (fade & 0x40) {
        if (num_world == 5 && num_level == 4) {
            if (horloge[5] != 0) {
                if (numero_palette_special == 0)
                    numero_palette_special = 1;
            } else {
                WRITE_PALETTE_BYTES(&rvb_special[numero_palette_special], 255);
                ++numero_palette_special;
            }
            if (numero_palette_special == 15)
                numero_palette_special = 0;
        } else if ((num_world == 1 && num_level == 9) ||
                   (num_world == 2 && num_level == 4) ||
                   (num_world == 4 && num_level == 9)) {
            ++CompteurEclair;
            if (CompteurEclair >= ProchainEclair || numero_palette_special != 0) {
                WRITE_PALETTE_BYTES(&rvb_special[numero_palette_special], 255);
                if (numero_palette_special == 0) {
                    PlaySnd_old(195);
                    ++numero_palette_special;
                } else if (CompteurEclair % 2 == 0) {
                    ++numero_palette_special;
                }
                if (numero_palette_special == 15) {
                    CompteurEclair = 0;
                    numero_palette_special = 0;
                    ProchainEclair = (s16)(myRand(400) + 1);
                }
            }
        }
    }
}
#undef clip_palette_component
#undef WRITE_PALETTE_BYTES

void InitModeXWithFrequency(u8 freq) {
    if (ModeVideoActuel == MODE_TEXT) {
        InitFirstModeVideo();
        GetVideoRegister();
    }
    if (ModeVideoActuel != MODE_X) {
        InitModeX();
        set_vga_frequency(freq);
        ModeVideoActuel = MODE_X;
        DrawSpriteNormalEtX = &DrawSpriteX;
        DrawSpriteFlipNormalEtX = &DrawSpriteFlipX;
        DrawSpriteNoClipNormalEtX = &DrawSpriteNoClipX;
        DrawSpriteColorNormalEtX = &DrawSpriteColorX;
        DrawSpriteColorFlipNormalEtX = &DrawSpriteColorFlipX;
        draw_buffer = (u8 *)0xa0500;
        drawflocon5NormalETX = &mode_x_effect_2b3d;
        drawflocon2NormalETX = &mode_x_effect_2c00;
        drawflocon4NormalETX = &mode_x_effect_2d08;
        drawflocon1NormalETX = &mode_x_effect_2bb7;
        drawflocon7NormalETX = &mode_x_effect_2dfb;
        fplotNormalETX = &fplot_mode_x_2b86;
        drawpluie7NormalETX = &mode_x_effect_300f;
        drawpluie5NormalETX = &mode_x_effect_2da9;
        drawpluie4NormalETX = &mode_x_effect_2cbf;
        drawpluie6NormalETX = &mode_x_effect_2e9c;
        drawflocon6NormalETX = &mode_x_effect_2f0c;
        drawflocon3NormalETX = &mode_x_effect_2c59;
    }
}

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
