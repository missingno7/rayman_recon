#include "type_island_abi.h"
/* 1:2b4d0..1:2e580: frame.c tail + guetteur.c (pirate lookout).
   Names RAYVERSE/ANDROID except explicitly INVENTED renderer purposes.
   All 55 bodies are represented; object layout is still unverified.
   Existing frame_local.h contains original port-only inline instructions.
   All new pragmas below declare external assembly register ABIs only. */
#include "obj.h"
#include "init_data_state.h"
extern init_data_state_t init_data_state;
#include "game.h"
#include "map.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <i86.h>
#include "frame_local.h"

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
extern u32 plan0bit_length;
extern u8 *PLAN0BIT;
extern s32 plan2bit_length;
extern u8 *PLAN2BIT;
extern u32 plan2bit_nb_bytes;
extern rgb_palette_t *rvb_special;
extern s16 numero_palette_special;
extern s32 CompteurEclair;
extern u8 saved_vga_registers[6];
extern u8 VGA_FREQ;
extern u8 need_timer;
extern u8 in_pause;
typedef void block_copy_t(u8 *source, u8 *dest);
typedef void block_clear_t(u8 *dest);
#pragma aux block_copy_t parm [esi] [edi] modify exact []
#pragma aux block_clear_t parm [edi] modify exact []
extern block_copy_t block_copy;
extern block_clear_t block_clear;
typedef void display_anypicture_t(u8 *source, u8 *dest, s32 height, s32 stride_and_plane, s32 source_width, s32 width);
#pragma aux display_anypicture_t parm [esi] [edi] [eax] [edx] [ecx] [ebx] modify exact [eax ecx edx esi edi]
extern display_anypicture_t display_anypicture;
extern void speaker_enable(void);
extern s32 clock_ticks(void);
extern scene_func_t dummy_prg;
u8 *temp_malloc(s32 size);
void temp_free(u8 *buffer);
typedef void delay_500_t(s32 milliseconds);
#pragma aux delay_500_t parm [eax]
extern delay_500_t wait_milliseconds;
/* DOS 1:2b5fb loads the INT 15h/AH=83h event flag into EBX before
   1:2b60e calls the assembly wrapper at 1:0273a. */
typedef void bios_wait_timer_t(s32 high, s32 low, u8 *flag);
#pragma aux bios_wait_timer_t parm [ecx] [edx] [ebx]
extern bios_wait_timer_t bios_wait_timer;
typedef void copy_dwords_t(u32 *source, u32 *dest, s32 count);
#pragma aux copy_dwords_t parm [esi] [edi] [ecx] modify exact [ecx esi edi]
extern copy_dwords_t copy_dwords;
/* DOS record lists: helpers traverse 14 rows at 128-byte stride, reading dwords.
   Names are INVENTED; Android patai_line/vide_line are each 1792 bytes, mapping OPEN. */
extern u32 frame_records_38888[14][32];
extern u32 plan0bit_nb_bytes;
extern u8 P486;
/* ABI evidence: docs/reference/lowlevel_abi.h, 1:03257 and 1:037e4.
   asm_03257 parameter order is INVENTED: reverse argument preparation follows
   DOS 1:2be9e..2beab (context, destination, records, mask, stride).
   The parameter/register mapping and exact clobbers are unchanged. */
void asm_03257(u32 mask, u32 stride, u32 offset, const u32 *records, u32 dest, const u32 *context);
#pragma aux asm_03257 parm [eax] [edx] [ebx] [esi] [edi] [ecx] modify exact [eax ecx esi]
void asm_037e4(const void *records, u8 *dest);
#pragma aux asm_037e4 parm [esi] [ecx] modify exact [eax ebx ecx edx esi edi]

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
extern s16 rvb_fade[256 * 3];
extern rgb_palette_t rvb_save;
extern u8 save_current_pal;
extern u16 type_zdc[262];
extern u8 *draw_buffer;
extern u8 *display_buffer;
extern u8 *EffetBufferNormal;
extern u8 *PLAN3BIT;
extern u8 *PLANTMPBIT;
extern u8 *PLANVIGBIT[8];
extern s32 plan3bit_length;
extern s32 plan3bit_nb_bytes;
extern s32 plantmp_length;
extern s32 plantmp_nb_bytes;
extern s32 planVigbit_length[8];
extern s32 planVigbit_nb_bytes[8];
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
extern island_draw_t *DrawSpriteColorNormalEtX;
extern draw_func_t *DrawSpriteColorFlipNormalEtX;
extern island_draw_t *DrawSpriteFlipNormalEtX;
extern island_draw_t *DrawSpriteNormalEtX;
extern island_snow_t *drawflocon1NormalETX;
extern island_snow_t *drawflocon2NormalETX;
extern island_snow_t *drawflocon3NormalETX;
extern island_snow_t *drawflocon4NormalETX;
extern island_snow_t *drawflocon5NormalETX;
extern island_snow_t *drawflocon6NormalETX;
extern island_snow_t *drawflocon7NormalETX;
extern island_snow_t *drawpluie4NormalETX;
extern island_snow_t *drawpluie5NormalETX;
extern island_snow_t *drawpluie6NormalETX;
extern island_snow_t *drawpluie7NormalETX;
extern fplot_func_t *fplotNormalETX;
extern active_objects_t actobj;
extern u8 is_joypad_mode;
extern u8 Main_Control;
typedef s32 (swap_func_t)(u8 *, u8 *, s32, s32);
#pragma aux swap_func_t parm [edi] [esi] [ecx] [eax]
extern swap_func_t *Swap_And_Test_Joystick;
extern void clear_memory(u8 *dest, s32 value, s32 count);
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
extern void_func_t draw_normal_5755;
extern void_func_t draw_flocon5_Normal;
extern void_func_t draw_flocon6_Normal;
extern void_func_t draw_flocon7_Normal;
extern void_func_t draw_pluie5_Normal;
extern void_func_t draw_pluie6_Normal;
extern void_func_t draw_pluie7_Normal;
extern fplot_func_t fplot_Normal;
void BombExplosion(obj_t* obj);
void ClearDrawAndDisplayBufferNormal(u8* draw_buf, u8* display_buf);
void GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
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
void snapToSprite(obj_t* obj_1, obj_t* obj_2, u8 obj_2_spr, s32 x_offs, s32 y_offs);
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
s16 hasGuetteurABomb(obj_t* obj);
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
    u32 *dest = (u32*)0xa0000;
    s32 iterations = 0;
    *unk_ptr_3d840 = 0;
    bios_wait_timer(7, 0xa120, unk_ptr_3d840);
    do {
        copy_dwords((u32*)0, dest, 0x3fff);
        ++iterations;
    } while (*unk_ptr_3d840 == 0);
    return iterations / 8;
}

void calc_gros_type(void) {
    u8 *blocks = MAP_BLOCKS;
    s32 i;
    for (i = 0; i < mp.length; ++i) {
        s32 texture_id = mp.map[i].texture_id;
        s32 tile_x = (texture_id % 40) * 16;
        s32 tile_y = (texture_id / 40) * 16 * 640;
        u8 all_zero = 1;
        u8 all_nonzero = 1;
        s16 plane;
        for (plane = 0; plane < 4; ++plane) {
            u8 *pixels = blocks + tile_x + plane + tile_y;
            u8 *end = blocks + 0x2800 + tile_x + plane + tile_y;
            while (pixels != end) {
                u8 a = pixels[0];
                u8 b = pixels[4];
                u8 c = pixels[8];
                u8 d = pixels[12];
                if (a || b || c || d)
                    all_zero = 0;
                if (!a || !b || !c || !d)
                    all_nonzero = 0;
                pixels += 640;
            }
        }
        if (all_nonzero)
            mp.map[i].transparency = 0;
        else if (all_zero)
            mp.map[i].transparency = 1;
        else
            mp.map[i].transparency = 2;
        if (mp.map[i].texture_id == 0 && mp.map[i].transparency != 1)
            mp.map[i].transparency = 1;
        if (mp.map[i].texture_id >= 1200)
            mp.map[i].texture_id = 0;
        smap[mp.map[i].texture_id] = mp.map[i].transparency;
    }
}

u8 find_in_map(s16 x, s16 y) {
    y = (s32)y >> 4;
    x = (s32)x >> 4;
    return smap[y * 40 + x];
}

void init_find_in_map(void) {
    s16 i;
    smap = temp_malloc(0x4b0);
    for (i = 0; i < 0x4b0; ++i)
        smap[i] = 1;
}

void end_find_in_map(void) {
    temp_free(smap);
}

void build_map(s16 x, s16 y) {
    u8 *buffer = PLAN1_BUFFER;
    map_tile_t *start;
    s32 plane;
    PLAN1_BUFFER_START_POINTER = (u32)buffer;
    start = mp.map + ((y >> 4) * mp.width + (x >> 4));
    /* DOS 1:2b88e..2b938: four planes, source step 64 and dest step 0x4600. */
    for (plane = 0; 4 > plane; ++plane) {
        map_tile_t *tile = start;
        u8 *dest = buffer + plane * 0x4600;
        s32 row;
        for (row = 0; row < 224; row += 16) {
            s16 col;
            for (col = 0; col < 320; col += 16) {
                s16 texture_id = tile->texture_id;
                if (texture_id >= 0 && texture_id < 1200 && tile->transparency == 0)
                    block_copy((u8*)gros_patai_src[texture_id] + plane * 64, dest);
                else
                    block_clear(dest);
                ++tile;
                dest += 4;
            }
            tile += mp.width - 20;
            dest += 1200;
        }
    }
}

void build_line_map(void *buffer, s16 x, s16 y, s16 row) {
    u8 *base = (u8*)buffer;
    map_tile_t *start = mp.map + ((y >> 4) * mp.width + (x >> 4));
    s32 position = ((u32)base - (u32)PLAN1_BUFFER) % 80;
    s16 plane;
    s32 dest_offset;
    start += (row / 13) * mp.width;
    dest_offset = 0;
    for (plane = 0; plane != 256; plane += 64) {
        map_tile_t *tile = start;
        u8 *dest = base + dest_offset;
        s32 column = position;
        s16 pixel;
        for (pixel = 0; pixel < 320; pixel += 16) {
            s16 texture_id = tile->texture_id;
            if (texture_id >= 0 && texture_id < 1200 && tile->transparency == 0)
                block_copy((u8*)gros_patai_src[texture_id] + plane, dest);
            ++tile;
            column += 4;
            dest += 4;
            if (column >= 80) {
                dest -= 80;
                column = 0;
            }
        }
        dest_offset += 0x4600;
    }
}

void build_column_map(void *buffer, s16 x, s16 y, s16 column) {
    map_tile_t *start = mp.map + ((y >> 4) * mp.width + (x >> 4));
    s32 position;
    s32 plane;
    s32 dest_offset;
    start += column / 16;
    position = ((u32)buffer - (u32)PLAN1_BUFFER) / 80;
    dest_offset = 0;
    for (plane = 0; plane != 256; plane += 64) {
        u8 *dest = (u8*)buffer + dest_offset;
        map_tile_t *tile = start;
        s32 row = position;
        s16 pixel;
        for (pixel = 0; pixel < 224; pixel += 16) {
            s16 texture_id = tile->texture_id;
            if (texture_id >= 0 && texture_id < 1200 && tile->transparency == 0)
                block_copy((u8*)gros_patai_src[texture_id] + plane, dest);
            dest += 1280;
            row += 16;
            tile += mp.width;
            if (row >= 224) {
                dest -= 0x4600;
                row = 0;
            }
        }
        dest_offset += 0x4600;
    }
}

void update_map(s16 x1, s16 y1, s16 x2, s16 y2) {
    s16 old_x = x1 & 0xfff0;
    s16 old_y = y1 & 0xfff0;
    s16 new_x = x2 & 0xfff0;
    s16 new_y = y2 & 0xfff0;
    s16 dx, dy;
    s32 column, row;
    u8 *previous, *current;
    if (old_x == new_x && old_y == new_y)
        return;
    column = (PLAN1_BUFFER_START_POINTER - (u32)PLAN1_BUFFER) % 80;
    row = (PLAN1_BUFFER_START_POINTER - (u32)PLAN1_BUFFER) / 80;
    dx = old_x - new_x;
    dy = old_y - new_y;
    row += dy;
    column += dx / 4;
    if (column >= 80) column -= 80;
    if (column < 0) column += 80;
    if (row >= 224) row -= 224;
    if (row < 0) row += 224;
    previous = (u8*)PLAN1_BUFFER_START_POINTER;
    current = PLAN1_BUFFER + row * 80 + column;
    PLAN1_BUFFER_START_POINTER = (u32)current;
    if (dx > 0 && dy > 0) {
        build_column_map(previous + 1280 +
            (previous + 1280 > PLAN1_BUFFER + 0x4600 ? -0x4600 : 0), x1, y1, 304);
        build_line_map(previous + (column == 0 ? -76 : 4), x1, y1, 176);
    } else if (dx > 0 && dy < 0) {
        build_column_map(previous - 1280 +
            (previous - 1280 < PLAN1_BUFFER ? 0x4600 : 0), x1, y1, 304);
        build_line_map(current, x1, y1, 0);
    } else if (dx < 0 && dy > 0) {
        build_column_map(current, x1, y1, 0);
        build_line_map(previous + (column == 76 ? 76 : -4), x1, y1, 176);
    } else {
        if (dy > 0) build_line_map(previous, x1, y1, 176);
        if (dx > 0) build_column_map(previous, x1, y1, 304);
        if (dy < 0) build_line_map(current, x1, y1, 0);
        if (dx < 0) build_column_map(current, x1, y1, 0);
    }
}

/* DOS callers 1:795e and 1:7988 pass sign-extended camera x/y in EAX/EDX. */
void sub_3BE20(s16 x, s16 y) {
    /* DOS 1:2be25 reserves 24 bytes: context starts at ESP, dest is at
       +0x10 and plane at +0x14. Reconstruct the context's 16-byte capacity;
       the purpose of the unaccessed fourth word is OPEN. */
    u32 context[4];
    s32 mask;
    u32 dest = 0;
    s16 plane;
    u32 offset;
    context[0] = (u32)plan0bit_length >> 2;
    context[1] = (u32)PLAN0BIT + plan0bit_length;
    context[2] = plan0bit_length - 1;
    mask = planar_copy_offset;
    if (type_fndscrX)
        mask = 1;
    if (P486) {
        asm_037e4(frame_records_38888, draw_buffer);
    } else {
        for (plane = 0; plane < 4; ++plane) {
            offset = ((u32)plan0bit_length >> 2) * plane;
            asm_03257(mask, plan0bit_nb_bytes, offset, &frame_records_38888[0][0], dest, context);
            mask *= 2;
            if (mask == 16) {
                mask = 1;
                dest = 1;
            }
        }
    }
}
/* All purpose names below INVENTED. Context offsets follow DOS 1:2bf5b..
   2c0cc; callback-only fields are read at 1:0444c/04454/0444f. */
typedef struct frame_context_t {
    u32 plane_size;
    u32 source_end;
    u32 run_step;
    u32 source_stride;
    u32 source_three_rows;
    map_tile_t *last_tile;
    u8 *background_end;
    u32 background_length;
} frame_context_t;


extern u32 frame_records_38f88[14][32];
extern s32 block_add[1200];
void asm_03912(u32 mask, u32 stride, u32 offset, const frame_context_t *context,
               const u32 *records, u32 dest);
#pragma aux asm_03912 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi]
void asm_03ea1(u32 mask, u32 stride, u32 offset, const frame_context_t *context,
               const u32 *records, u32 dest);
#pragma aux asm_03ea1 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi]
void asm_0283c(u32 mask);
#pragma aux asm_0283c parm [eax] modify exact [eax]
/* PROVEN from 1:04430..04503 plus audit_callbacks.py: all 82 GOG level
   callback tables, 2380 distinct bodies, all branch paths. Callbacks change
   EDX and EDI only. Helpers preserve EAX/EBX/ECX/EBP and change ESI/EDI/EDX. */
void asm_04430(u32 phase, map_tile_t *tiles, const frame_context_t *context,
               const s32 *callbacks, const u8 *background, u8 *dest);
#pragma aux asm_04430 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [edx esi edi]
void asm_04493(u32 phase, map_tile_t *tiles, const frame_context_t *context,
               const s32 *callbacks, const u8 *background, u8 *dest);
#pragma aux asm_04493 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [edx esi edi]

void sub_3BEE0(s16 x, s16 y) {
  frame_context_t context;
  u8 *origin = (u8 *) init_data_state.draw_origin;
  s32 scroll_x = init_data_state.scroll_x;
  s32 scroll_y = init_data_state.scroll_y;
  u32 tile_stride = plan0bit_nb_bytes * 16;
  map_tile_t *tiles = mp.map + ((mp.width * (y >> 4)) + (x >> 4));
  s32 mask = planar_copy_offset;
  s32 saved_mask = mask;
  u32 dest = 0;
  u8 *saved_origin = origin;
  s16 plane;
  map_tile_t *draw_tiles;
  u8 *background;
  s32 plane_step;
  u16 phase;
  context.plane_size = 0x4600;
  context.source_end = (u32) PLAN1_BUFFER;
  context.run_step = 0x4600;
  for (plane = 0; plane < 4; ++plane)
  {
    u32 offset = plane * 0x4600;
    context.source_end += 0x4600;
    if (P486)
    {
      asm_03ea1(mask, 80, offset, &context, &frame_records_38f88[1][0], dest);
    }
    else
    {
      asm_03912(mask, 80, offset, &context, &frame_records_38f88[0][0], dest);
    }
    mask = mask * 2;
    if (mask == 16)
    {
      mask = 1;
      dest = 1;
      ++origin;
    }
  }

  origin = saved_origin;
  draw_tiles = tiles;
  background = (((scroll_x / 4) + ((scroll_x % 4) * (((u32) plan0bit_length) >> 2))) + PLAN0BIT) + (scroll_y * plan0bit_nb_bytes);
  plane_step = ((u32) plan0bit_length) >> 2;
  plane_step = -plane_step;
  context.plane_size = tile_stride;
  context.source_end = mp.width * 6;
  context.source_stride = plan0bit_nb_bytes;
  context.source_three_rows = plan0bit_nb_bytes * 3;
  mask = saved_mask;
  context.background_length = plan0bit_length;
  context.run_step = 1200;
  context.last_tile = mp.map + ((mp.width * mp.height) - 1);
  context.background_end = PLAN0BIT + plan0bit_length;
  for (phase = 0; phase < 4; ++phase)
  {
    asm_0283c(mask);
    plane_step += ((u32) plan0bit_length) >> 2;
    if (((background + plane_step) > (PLAN0BIT + plan0bit_length)) != 0)
    {
      plane_step -= plan0bit_length;
      ++background;
    }
    if (P486)
    {
      asm_04493(phase << 8, draw_tiles, &context, block_add, background + plane_step, origin);
    }
    else
    {
      asm_04430(phase << 8, draw_tiles, &context, block_add, background + plane_step, origin);
    }
    mask = mask * 2;
    if (mask == 16)
    {
      mask = 1;
      ++origin;
    }
  }

}

void set_vga_frequency(u8 freq) {
    u8 value;
    SetVideoRegister();
    if (freq == 100) {
        value = inp(0x3cc);
        value |= 8;
        value &= 0xfb;
        outp(0x3c2, value);
    } else if (freq == 80) {
        value = inp(0x3cc);
        value |= 4;
        value &= 0xf7;
        outp(0x3c2, value);
    } else if (freq == 70) {
        value = inp(0x3cc);
        value &= 0xf3;
        outp(0x3c2, value);
    } else if (freq == 60) {
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
    if (freq == 50) {
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
    VGA_FREQ = freq;
    wait_milliseconds(500);
}

void GetVideoRegister(void) {
    saved_vga_registers[2] = (u8)inp(0x3cc);
    outp(0x3d4, 0x10);
    saved_vga_registers[1] = (u8)inp(0x3d5);
    outp(0x3d4, 0x11);
    saved_vga_registers[0] = (u8)inp(0x3d5);
    outp(0x3d4, 6);
    saved_vga_registers[4] = (u8)inp(0x3d5);
    outp(0x3d4, 7);
    saved_vga_registers[3] = (u8)inp(0x3d5);
    outp(0x3d4, 0x16);
    saved_vga_registers[5] = (u8)inp(0x3d5);
}

void SetVideoRegister(void) {
    outp(0x3c2, saved_vga_registers[2]);
    outp(0x3d4, 0x10);
    outp(0x3d5, saved_vga_registers[1]);
    outp(0x3d4, 0x11);
    outp(0x3d5, saved_vga_registers[0]);
    outp(0x3d4, 6);
    outp(0x3d5, saved_vga_registers[4]);
    outp(0x3d4, 7);
    outp(0x3d5, saved_vga_registers[3]);
    outp(0x3d4, 0x16);
    outp(0x3d5, saved_vga_registers[5]);
}



/* SetVideoRegister: no seed source */

/* INVENTED purpose names. DOS 1:06fd4 stores draw-buffer-relative origin;
   1:07075 stores the PLAN0BIT-relative source pointer. */


extern u32 frame_records_38f88[14][32];
extern u8 BackgroundOn, FondAutorise;
/* External assembly ABIs proven by 1:02850..02b2f; see lowlevel_abi.h.
   The records are 128-byte rows. Row skip is measured in map-tile bytes. */
void asm_02850(map_tile_t *tiles, u32 *records, u8 *source, u8 *buffer,
               u8 *dest, s32 row_skip);
#pragma aux asm_02850 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax edx esi]
void asm_029d3(map_tile_t *tiles, u32 *records, u8 *source, u32 stride,
               u8 *dest, s32 row_skip);
#pragma aux asm_029d3 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ebx ecx edx esi]
void asm_02a73(map_tile_t *tiles, u32 *records, u8 *source, u32 stride,
               u8 *dest, s32 row_skip);
#pragma aux asm_02a73 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ebx ecx edx esi]

void sub_3C3BC(s16 x, s16 y) {
  s32 row;
  s32 column;
  u8 *buffer_start = (u8 *) PLAN1_BUFFER_START_POINTER;
  s32 row_skip = mp.width - 20;
  u8 *dest = (u8 *) init_data_state.draw_origin;
  map_tile_t *tiles = mp.map + ((mp.width * (y >> 4)) + (x >> 4));
  u8 *source = (u8 *) init_data_state.source_origin;
  if (BackgroundOn && FondAutorise)
  {
    u32 stride = plan0bit_nb_bytes * 16;
    if (P486)
    {
      s32 delta = (draw_buffer - dest) + 4;
      column = 4 - (((s32) delta) % 80);
      row = 16 - (((s32) delta) / 80);
      asm_02a73(tiles, &frame_records_38888[0][0], (((u8 *) 0xab000) + (row * 80)) + column, stride, (draw_buffer + (row * 80)) + column, row_skip * 6);
    }
    else
    {
      asm_029d3(tiles, &frame_records_38888[0][0], source, stride, dest, row_skip * 6);
    }
  }
  asm_02850(tiles, &frame_records_38f88[0][0], buffer_start, PLAN1_BUFFER, dest, row_skip * 6);
}

void clear_palette(rgb_palette_t* palette) {
    s16 i = 0;
    u8 *p = (u8*)palette;
    s16 *q = rvb_fade;
    do {
        ++i;
        *p++ = 0;
        *q++ = 0;
    } while (i < 0x300);
}

void set_fade_palette(rgb_palette_t* palette) {
    s16 i;
    s16 *q = rvb_fade;
    u8 *p = (u8*)palette;
    for (i = 0; i < 256 * 3; ++i) {
        *q++ = ashl16(*p++, 6);
    }
}

void start_fade_in(s16 speed, rgb_palette_t *palette) {
    u8 *palette_bytes = (u8*)&current_rvb;
    u32 first_color = 0;
    u32 last_color = 0xff;
    (void)palette;
    nb_fade = 1 << (6 - speed);
    fade = 1; // fade in
    /* W-STORE: this assignment order reproduces DOS 1:2c55d..2c57a. */
    fade_speed = speed;
    clear_palette((rgb_palette_t*)palette_bytes);
    palette_output(first_color, last_color, palette_bytes);
}

void start_fade_out(s16 speed, rgb_palette_t *palette) {
    (void)palette;
    nb_fade = 1 << (6 - speed);
    fade = 2; // fade out
    fade_speed = speed;
}

#define WRITE_PALETTE_BYTES(p,n) palette_output(0, (n)-1, p)

void do_fade(rgb_palette_t* source_pal, rgb_palette_t* dest_pal) {
    if (nb_fade > 0) {
        s16 i;
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
            s16 temp = rvb_fade[i] >> 6;
            ((u8*)dest_pal->colors)[i] = (u8)temp;
        }
        WRITE_PALETTE_BYTES(&current_rvb, 256);
        if (nb_fade == 0) fade |= 0x40;
    }
}

void fade_out(s16 speed, rgb_palette_t *palette) {
    s16 i, steps;
    start_fade_out(speed, palette);
    steps = nb_fade;
    for (i = 0; i < steps; ++i) {
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
    s32 i;
    rgb_t first;
    first.r = rvb[palette_id].colors[first_color].r;
    first.g = rvb[palette_id].colors[first_color].g;
    first.b = rvb[palette_id].colors[first_color].b;
    for (i = first_color; i < last_color; ++i) {
        rvb[palette_id].colors[i].r = rvb[palette_id].colors[i + 1].r;
        rvb[palette_id].colors[i].g = rvb[palette_id].colors[i + 1].g;
        rvb[palette_id].colors[i].b = rvb[palette_id].colors[i + 1].b;
    }
    rvb[palette_id].colors[last_color].r = first.r;
    rvb[palette_id].colors[last_color].g = first.g;
    rvb[palette_id].colors[last_color].b = first.b;
    memcpy(&current_rvb, &rvb[palette_id], sizeof(current_rvb));
    set_fade_palette(&current_rvb);
    if (nb_fade == 0)
        nb_fade = 1;
}

void DO_SWAP_PALETTE(void) {
    s16 i;
    obj_t *obj;
    /* DOS 1:2c93a..2ca21: byte allocation and independent X/Y homes. */
    u8 above, right, axis, below, left, new_pal;
    s16 ray_y, ray_x, obj_pos;
    if (last_plan1_palette != 0) {
        if (fade & 0x40) {
            obj = level.objects + actobj.objects[0];
            for (i = 0; i < actobj.num_active_objects; ++i, obj = level.objects + actobj.objects[i]) {
                if (obj->type == TYPE_158_PALETTE_SWAPPER) {
                    switch (obj->sub_etat) {
                        case 0: right=0; left=1; axis=0; break;
                        case 1: right=0; left=2; axis=0; break;
                        case 2: right=1; left=2; axis=0; break;
                        case 3: right=1; left=0; axis=0; break;
                        case 4: right=2; left=0; axis=0; break;
                        case 5: right=2; left=1; axis=0; break;
                        case 6: above=1; below=0; axis=1; break;
                        case 7: above=2; below=0; axis=1; break;
                        case 8: above=2; below=1; axis=1; break;
                        case 9: below=1; above=0; axis=1; break;
                        case 10: below=2; above=0; axis=1; break;
                        case 11: below=2; above=1; axis=1; break;
                    }
                    switch (axis) {
                        case 0:
                            ray_x = (s16)ray.x + (s16)ray.offset_bx;
                            obj_pos = (s16)obj->x + (s16)obj->offset_bx;
                            if (obj_pos < ray_x) new_pal = left;
                            else if (obj_pos > ray_x) new_pal = right;
                            else new_pal = current_pal_id;
                            break;
                        case 1:
                            ray_y = (s16)ray.y + (s16)ray.offset_by;
                            obj_pos = (s16)obj->y + (s16)obj->offset_by;
                            if (obj_pos < ray_y) new_pal = above;
                            else if (obj_pos > ray_y) new_pal = below;
                            else new_pal = current_pal_id;
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
    s16 scene_ended;
    u8 timer_enabled = 0;
    u8 do_fade = 1;
    u32 timer = 0;
    do {
        speaker_enable();
        if (ModeVideoActuel == MODE_X) {
            endsynchro();
            SWAP_BUFFERS();
            synchro();
            if (timer_enabled != need_timer)
                timer = (u32)clock_ticks();
            if (do_fade == 0)
                DO_FADE();
        } else {
            endsynchro();
            synchro();
            if (timer_enabled != need_timer)
                timer = (u32)clock_ticks();
            if (do_fade == 0)
                DO_FADE();
            SWAP_BUFFERS();
        }
        scene_ended = scene_func(timer);
        do_fade = timer_enabled;
    } while (scene_ended == 0);
    if (scene_func != dummy_prg)
        in_pause = 0;
}

void DISPLAY_ANYSIZE_PICTURE(void *image, s16 source_x, s16 source_width,
                             s16 x, s16 y, s16 height, s16 width) {
  s32 source_stride;
  u32 source_length;
  s16 width_was_defaulted = 0;
  s16 source_plane;
  u8 *source;
  u8 *dest;
  if (image == PLAN2BIT)
  {
    source_length = plan2bit_length;
    source_stride = plan2bit_nb_bytes;
  }
  else
  {
    source_length = plan3bit_length;
    source_stride = plan3bit_nb_bytes;
  }
  source_length >>= 2;
  if (width == 0)
  {
    width_was_defaulted = (s16) ((s16) 1);
    width = source_stride;
  }
  {
    if (!(height == 0))
    {
      goto perm_else_2;
    }
    {
      height = (source_length / width) - 4;
    }
    goto perm_end_2;
    perm_else_2:
    {
    }

    perm_end_2:
    ;

  }
  if (width_was_defaulted)
  {
    width = ((s32) width) >> 2;
  }
  {
    s16 perm_value_2;
    perm_value_2 = source_x & 3;
    source_plane = perm_value_2;
  }
  source_x >>= 2;
  source = ((((u8 *) image) + (source_length * source_plane)) + (source_width * source_stride)) + source_x;
  x >>= 2;
  dest = ((draw_buffer + (y << 6)) + (y << 4)) + x;
  display_anypicture(source, dest, height, source_stride + ((1 << source_plane) << 8), source_length, width);
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
    s16 v1;
    s16 x;
    s16 row;
    u8 *dest_pos;
    u8 *source_pos;
    v1 = 320 - (s16)Bloc_lim_W2 + (s16)Bloc_lim_W1;
    if (!OptionGame) {
        memcpy(draw_buffer, EffetBufferNormal, 320 * 200);
    } else {
        if (prev_Bloc_lim_W1 != Bloc_lim_W1) {
            if (prev_Bloc_lim_W1 < Bloc_lim_W1)
            ClearBorder((s16)Bloc_lim_H1, (s16)Bloc_lim_H2,
                        (s16)Bloc_lim_W1, (s16)Bloc_lim_W2);
            prev_Bloc_lim_W1 = Bloc_lim_W1;
        }
        clear_memory(draw_buffer, 0, Bloc_lim_H1 * 320);
        dest_pos = draw_buffer + Bloc_lim_W1 + Bloc_lim_H1 * 320;
        source_pos = EffetBufferNormal + Bloc_lim_W1 + Bloc_lim_H1 * 320;
        row = (s16)Bloc_lim_H1;
        while ((s32)row < Bloc_lim_H2) {
            x = (s16)Bloc_lim_W1;
            while (x < Bloc_lim_W2) {
                *dest_pos++ = *source_pos++;
                ++x;
            }
            dest_pos += v1;
            source_pos += v1;
            ++row;
        }
        clear_memory(draw_buffer + Bloc_lim_H2 * 320, 0, (200 - Bloc_lim_H2) * 320);
    }
}

#define clip_palette_component(value) min32((value), 63)

void InitPaletteSpecialPC(void) {
    s32 i;
    CompteurEclair = 0;
    if ((num_world == 1 && num_level == 9) ||
        (num_world == 2 && num_level == 4) ||
        (num_world == 4 && num_level == 9)) {
        for (i = 0; i < 256; ++i) {
            rvb_special[15].colors[i].r = rvb[0].colors[i].r;
            rvb_special[15].colors[i].g = rvb[0].colors[i].g;
            rvb_special[15].colors[i].b = rvb[0].colors[i].b;
            rvb_special[0].colors[i].r = clip_palette_component((4 * rvb[0].colors[i].r) / 8);
            rvb_special[0].colors[i].g = clip_palette_component((4 * rvb[0].colors[i].g) / 8);
            rvb_special[0].colors[i].b = clip_palette_component((4 * rvb[0].colors[i].b) / 5);
            rvb_special[5].colors[i].r = clip_palette_component((4 * rvb[0].colors[i].r) / 3);
            rvb_special[5].colors[i].g = clip_palette_component((4 * rvb[0].colors[i].g) / 3);
            rvb_special[5].colors[i].b = clip_palette_component((4 * rvb[0].colors[i].b) / 3);
            rvb_special[7].colors[i].r = clip_palette_component((4 * rvb[0].colors[i].r) / 6);
            rvb_special[7].colors[i].g = clip_palette_component((4 * rvb[0].colors[i].g) / 6);
            rvb_special[7].colors[i].b = clip_palette_component(rvb[0].colors[i].b);
            rvb_special[10].colors[i].r = clip_palette_component(2 * rvb[0].colors[i].r);
            rvb_special[10].colors[i].g = clip_palette_component(2 * rvb[0].colors[i].g);
            rvb_special[10].colors[i].b = clip_palette_component(2 * rvb[0].colors[i].b);
            rvb_special[14].colors[i].r = clip_palette_component((4 * rvb[0].colors[i].r) / 8);
            rvb_special[14].colors[i].g = clip_palette_component((4 * rvb[0].colors[i].g) / 8);
            rvb_special[14].colors[i].b = clip_palette_component((4 * rvb[0].colors[i].b) / 5);
            rvb[0].colors[i].r = rvb_special[0].colors[i].r;
            rvb[0].colors[i].g = rvb_special[0].colors[i].g;
            rvb[0].colors[i].b = rvb_special[0].colors[i].b;
        }
        DoFadePaletteSpecialPC(0, 5);
        DoFadePaletteSpecialPC(5, 7);
        DoFadePaletteSpecialPC(7, 10);
        DoFadePaletteSpecialPC(10, 14);
        numero_palette_special = 0;
        ProchainEclair = (s16)(myRand(200) + 1);
    } else if (num_world == 5 && num_level == 4) {
        for (i = 0; i < 255; ++i) {
            rvb_special[15].colors[i].r = rvb[0].colors[i].r;
            rvb_special[15].colors[i].g = rvb[0].colors[i].g;
            rvb_special[15].colors[i].b = rvb[0].colors[i].b;
            rvb_special[0].colors[i].r = clip_palette_component(rvb[0].colors[i].r);
            rvb_special[0].colors[i].g = clip_palette_component((77 * rvb[0].colors[i].g) / 100);
            rvb_special[0].colors[i].b = clip_palette_component((12 * rvb[0].colors[i].b) / 100);
            rvb_special[14].colors[i].r = clip_palette_component(rvb_special[0].colors[i].r);
            rvb_special[14].colors[i].g = clip_palette_component(rvb_special[0].colors[i].g);
            rvb_special[14].colors[i].b = clip_palette_component(rvb_special[0].colors[i].b);
            rvb[0].colors[i].r = rvb_special[0].colors[i].r;
            rvb[0].colors[i].g = rvb_special[0].colors[i].g;
            rvb[0].colors[i].b = rvb_special[0].colors[i].b;
        }
        DoFadePaletteSpecialPC(0, 14);
        for (i = 128; i <= 174; ++i) {
            rvb_special[9].colors[i].r = clip_palette_component((84 * rvb[0].colors[i].r) / 100);
            rvb_special[9].colors[i].g = clip_palette_component((62 * rvb[0].colors[i].g) / 100);
            rvb_special[9].colors[i].b = clip_palette_component((12 * rvb[0].colors[i].b) / 100);
            rvb[0].colors[i].r = rvb_special[0].colors[i].r;
            rvb[0].colors[i].g = rvb_special[0].colors[i].g;
            rvb[0].colors[i].b = rvb_special[0].colors[i].b;
        }
        DoFadePaletteSpecialPC(0, 9);
        DoFadePaletteSpecialPC(9, 14);
    }
}

void DoFadePaletteSpecialPC(s16 start, s16 end) {
    /* DOS 1:2d55f/2d6d6: short indices; 2d5fe, 2d660, 2d6b6:
       truncate each interpolated channel before the constant-first clamp. */
    s16 p, i;
    for (p = start + 1; p < end; ++p) {
        for (i = 0; i < 256; ++i) {
            rvb_special[p].colors[i].r = min32(63, (u8)(
                (rvb_special[start].colors[i].r * (end - p) +
                 rvb_special[end].colors[i].r * (p - start)) / (end - start)));
            rvb_special[p].colors[i].g = min32(63, (u8)(
                (rvb_special[start].colors[i].g * (end - p) +
                 rvb_special[end].colors[i].g * (p - start)) / (end - start)));
            rvb_special[p].colors[i].b = min32(63, (u8)(
                (rvb_special[start].colors[i].b * (end - p) +
                 rvb_special[end].colors[i].b * (p - start)) / (end - start)));
        }
    }
}

#define WRITE_SPECIAL_PALETTE(palette_ptr) \
    do { \
        u8 *palette_data = (u8*)(palette_ptr) + 3; \
        frame_palette_index(0x3c8, 1); \
        frame_palette_stream(0x3c9, palette_data, 765); \
    } while (0)

void DoPaletteSpecialPC(void) {
  if (fade & 0x40)
  {
    if ((num_world == 5) && (num_level == 4))
    {
      if (horloge[5] == 0)
      {
        do
        {
          u8 *palette_data = ((u8 *) (&rvb_special[numero_palette_special])) + 3;
          frame_palette_index(0x3c8, 1);
          {
            frame_palette_stream(0x3c9, palette_data, 765);
          }
        }
        while (0);
        ++numero_palette_special;
      }
      else
        if (numero_palette_special == 0)
      {
        numero_palette_special = 1;
      }
      if (numero_palette_special == 15)
      {
        numero_palette_special = 0;
      }
    }
    else
      if ((((num_world == 1) && (num_level == 9)) || ((num_world == 2) && (num_level == 4))) || ((num_world == 4) && (num_level == 9)))
    {
      ++CompteurEclair;
      if ((CompteurEclair >= ProchainEclair) || (numero_palette_special != 0))
      {
        do
        {
          u8 *palette_data = ((u8 *) (&rvb_special[numero_palette_special])) + 3;
          frame_palette_index(0x3c8, 1);
          frame_palette_stream(0x3c9, palette_data, 765);
        }
        while (0);
        if (numero_palette_special == 0)
        {
          PlaySnd_old(195);
          {
            ++numero_palette_special;
          }
        }
        else
          if (((CompteurEclair % 2) == 0) != 0)
        {
          ++numero_palette_special;
        }
        {
          if (!(numero_palette_special == 15))
          {
            goto perm_else_7;
          }
          {
            {
              CompteurEclair = 0;
              numero_palette_special = 0;
              ProchainEclair = (s16) (myRand(400) + 1);
            }
          }
          goto perm_end_7;
          perm_else_7:
          {
          }

          perm_end_7:
          ;

        }
      }
    }
  }
}
#undef clip_palette_component
#undef WRITE_PALETTE_BYTES
#undef WRITE_SPECIAL_PALETTE


void InitModeXWithFrequency(u8 freq) {
    if (ModeVideoActuel == MODE_TEXT) {
        InitFirstModeVideo();
        GetVideoRegister();
    }
    if (ModeVideoActuel != MODE_X) {
        InitModeX();
        clear_memory(DisplayBufferNormal, 0, 0x10000);
        set_vga_frequency(freq);
        ModeVideoActuel = MODE_X;
        draw_buffer = (u8 *)0xa0500;
        DrawSpriteNormalEtX = (island_draw_t *)(&DrawSpriteX);
        DrawSpriteNoClipNormalEtX = &DrawSpriteFlipNoClipX;
        DrawSpriteFlipNormalEtX = (island_draw_t *)(&DrawSpriteFlipX);
        DrawSpriteFlipNoClipNormalEtX = &DrawSpriteNoClipX;
        DrawSpriteColorNormalEtX = (island_draw_t *)(&DrawSpriteColorX);
        DrawSpriteColorFlipNormalEtX = &DrawSpriteColorFlipX;
        fplotNormalETX = &fplot_mode_x_2b86;
        drawflocon5NormalETX = (island_snow_t *)(&mode_x_effect_2b3d);
        drawflocon1NormalETX = (island_snow_t *)(&mode_x_effect_2bb7);
        drawflocon2NormalETX = (island_snow_t *)(&mode_x_effect_2c00);
        drawflocon3NormalETX = (island_snow_t *)(&mode_x_effect_2c59);
        drawflocon4NormalETX = (island_snow_t *)(&mode_x_effect_2d08);
        drawflocon7NormalETX = (island_snow_t *)(&mode_x_effect_2dfb);
        drawflocon6NormalETX = (island_snow_t *)(&mode_x_effect_2f0c);
        drawpluie7NormalETX = (island_snow_t *)(&mode_x_effect_300f);
        drawpluie6NormalETX = (island_snow_t *)(&mode_x_effect_2e9c);
        drawpluie5NormalETX = (island_snow_t *)(&mode_x_effect_2da9);
        drawpluie4NormalETX = (island_snow_t *)(&mode_x_effect_2cbf);
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
        clear_memory(DrawBufferNormal, 0, 320 * 200);
        clear_memory(DisplayBufferNormal, 0, 320 * 200);
        select_display_buffer(DisplayBufferNormal);
        set_vga_frequency(freq);
        ModeVideoActuel = MODE_NORMAL;
        DrawSpriteNormalEtX = (island_draw_t *)(&DrawSpriteNormal);
        DrawSpriteNoClipNormalEtX = &DrawSpriteNormalNoClip;
        draw_buffer = DrawBufferNormal;
        DrawSpriteFlipNormalEtX = (island_draw_t *)(&DrawSpriteFlipNormal);
        DrawSpriteFlipNoClipNormalEtX = &DrawSpriteFlipNormalNoClip;
        DrawSpriteColorNormalEtX = (island_draw_t *)(&DrawSpriteColorNormal);
        DrawSpriteColorFlipNormalEtX = &DrawSpriteColorFlipNormal;
        fplotNormalETX = &fplot_Normal;
        drawflocon5NormalETX = (island_snow_t *)(&draw_flocon5_Normal);
        drawflocon1NormalETX = (island_snow_t *)(&draw_flocon1_Normal);
        drawflocon2NormalETX = (island_snow_t *)(&draw_flocon2_Normal);
        drawflocon3NormalETX = (island_snow_t *)(&draw_flocon3_Normal);
        drawflocon4NormalETX = (island_snow_t *)(&draw_normal_5755);
        drawflocon7NormalETX = (island_snow_t *)(&draw_flocon7_Normal);
        drawflocon6NormalETX = (island_snow_t *)(&draw_flocon6_Normal);
        drawpluie7NormalETX = (island_snow_t *)(&draw_pluie7_Normal);
        drawpluie6NormalETX = (island_snow_t *)(&draw_pluie6_Normal);
        drawpluie5NormalETX = (island_snow_t *)(&draw_pluie5_Normal);
        drawpluie4NormalETX = (island_snow_t *)(&draw_flocon4_Normal);
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

void guetteurFollowsShip(obj_t *obj) {
    if (bateau_obj_id != -1)
        snapToSprite(obj, &level.objects[bateau_obj_id], 1, -52, -63);
    else {
        /* DOS 1:2dba5..2dbaf: assign Y, then reload it as the chain value. */
        obj->speed_x = obj->speed_y = 0;
    }
}

void DO_ONE_PAR_COMMAND(obj_t* obj) {
    u8 flip_x;
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

s16 hasGuetteurABomb(obj_t* obj) {
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
    s16 new_spd_x;
    eta_t *cur_eta_2;
    s16 gue_h;
    eta_t *cur_eta_3;
    eta_t *cur_eta_1;
    obj_t *cur_obj;
    s16 gue_w;
    s16 i;
    s16 nb_objs;
    s16 gue_y;
    s16 unk_1;
    u8 mul_x_pos;
    s16 gue_x;

    /* DOS leaves the multiplier/speed unset on unhandled substates. */
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
                        cur_obj->flags.flip_x = !gue_obj->flags.flip_x;

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
                            cur_eta_1->anim_speed &= 0x0f;
                            cur_eta_1->anim_speed |= 0xa0;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 10;
                            break;
                        case 5:
                            new_spd_x = 32;
                            mul_x_pos = 3;
                            cur_obj->y += 24;
                            cur_eta_2 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_2->anim_speed &= 0x0f;
                            cur_eta_2->anim_speed |= 0x20;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 0;
                            break;
                        case 0x0D:
                            /* Same short snapshot at DOS 1:2e076 and 1:2e095. */
                            new_spd_x = 8 - myRand(32);
                            new_spd_x += 8 * sgn(new_spd_x);
                            mul_x_pos = 0;
                            cur_eta_3 = &cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat];
                            cur_eta_3->anim_speed &= 0x0f;
                            cur_eta_3->anim_speed |= 0x20;
                            cur_obj->gravity_value_1 = 0;
                            cur_obj->gravity_value_2 = 0;
                            cur_obj->y -= 20;
                            break;
                    }
                    if (param_3 == 0)
                        new_spd_x = 16;

                    cur_obj->speed_x = new_spd_x;
                    if (!gue_obj->flags.flip_x)
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
