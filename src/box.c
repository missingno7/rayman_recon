#include "inline_helpers.h"
/* box.c (1:13f30..1:14e90: plasma/fire boxes, gauges, palette/capture helpers; PC-only). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_box. ABI pragmas describe external assembly; the marked ORIGINAL_INLINE pragmas contain only proven port/interrupt instructions. */
/* d_box: preserve the five accepted STRICT functions. CalcTab uses intrinsic sin
 * and one expression: one remaining reciprocal byte differs. See NOTES.md. */
#include "game.h"
#include "rtypes.h"
#include <conio.h>
#include <math.h>
#include <string.h>
/* CalcTab: sqrt uses the ordinary entry; sin keeps the software intrinsic ABI. */
#pragma function(sqrt)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define MODE_X 1

extern u8 *draw_buffer;
extern u8 *EffetBufferNormal;
extern u8 byte_DA43C[128 * 128];
/* Android p1: 42000 bytes; DOS InitFire 1:14491 has a 600-byte row stride. */
extern s16 p1[70][300];
extern rgb_palette_t rvb[3];
extern rgb_palette_t MenuPalette;
extern rgb_t PalPlasma[16];
extern rgb_t pal[16];
extern u8 plasma_palette_color_index;
extern u8 menuEtape;
extern u8 ModeVideoActuel;
extern u8 current_pal_id;
extern u8 dans_la_map_monde;
extern u8 OptionGame;
extern s16 word_95FA0;
extern s16 word_95FA2;
extern s16 word_95FA4;
extern s16 word_95FA6;
extern s16 word_95FA8;
extern s16 word_95FAA;
extern s16 word_95FAC;
extern s16 word_95FAE;
extern s16 word_95FB0;
extern s16 word_DE73E;
extern s16 word_DE740;
extern s16 word_DE742;
extern s16 word_DE744;
extern s16 word_DE746;
extern s16 word_DE748;
extern s16 word_DE74A;
extern s16 word_DE74C;

extern void DrawBlackBoxNormal(u8 *buffer, s32 x, s32 y, s32 height, s32 width);
#pragma aux DrawBlackBoxNormal parm [edi] [eax] [ebx] [ecx] [edx] modify []
extern void DrawBorderBoxNormal(u8 *buffer, s32 x, s32 y, s32 height, s32 width, s32 colors);
#pragma aux DrawBorderBoxNormal parm [edi] [esi] [ebx] [ecx] [edx] [eax] modify [eax ebx ecx edx]
extern s16 cosinus(s16 x);
extern s16 sinus(s16 x);
extern void MakeMyRand(int set_seed);
extern s16 myRand(s16 max);

/* ORIGINAL_INLINE: InitPlasma 1:14053, InitFire 1:1447c: OUT DX,AL. */
extern void box_dac_index(u16 port, u8 index);
#pragma aux box_dac_index = "out dx,al" parm [dx] [al] modify []
/* ORIGINAL_INLINE: 1:14059 / 1:14482: CLI. */
extern void box_palette_lock(void);
#pragma aux box_palette_lock = "cli" modify []
/* ORIGINAL_INLINE: 1:1405e..14060 / 1:14487..14489: REP OUTSB; STI. */
extern void box_palette_write(u16 port, u8 *data, u32 count);
#pragma aux box_palette_write = "rep outsb" "sti" parm [dx] [esi] [ecx] modify [esi ecx]

#define BOX_SET_PALETTE(first,last) palette_output(first, last, &rvb[0])

void CalcTab(void)
{
    s16 j;
    for (j = 0; j < 128; ++j) {
        s16 i;
        for (i = 0; i < 128; ++i) {
            byte_DA43C[128 * j + i] =
                (sin((u8)sqrt((64 - j) * (64 - j) + (64 - i) * (64 - i))
                     * (1.0 / 9.5)) + 1.0) * 40.0;
        }
    }
}

void InitPlasma(u8 need_set_palette)
{
    s16 i;
    if (menuEtape == 0)
        plasma_palette_color_index = 74;
    else
        plasma_palette_color_index = 112;
    for (i = 0; i < 16; ++i) {
        rvb[0].colors[plasma_palette_color_index + i].r = PalPlasma[i].b;
        rvb[0].colors[plasma_palette_color_index + i].g = PalPlasma[i].g;
        rvb[0].colors[plasma_palette_color_index + i].b = PalPlasma[i].r;
    }
    if (need_set_palette)
        BOX_SET_PALETTE(plasma_palette_color_index, plasma_palette_color_index + 15);
}

void Plasma(s16 x, s16 y, s16 width, s16 height,
            s16 a5, s16 a6, s16 a7, s16 a8, s16 a9, s16 a10, s16 a11, s16 a12, s16 a13)
{
    u8 yy[4];
    u8 xx[4];
    u8 *pos = draw_buffer + x + SCREEN_WIDTH * y;
    u16 skip = SCREEN_WIDTH - width;
    u16 j;
    yy[0] = a6; yy[1] = a8; yy[2] = a10; yy[3] = a12;
    for (j = 0; j < height; ++j) {
        u16 i;
        *(u32 *)yy += 0x01010101;
        *(u32 *)yy &= 0x7f7f7f7f;
        xx[0] = a5; xx[1] = a7; xx[2] = a9; xx[3] = a11;
        for (i = 0; i < width; ++i) {
            u8 value;
            s32 color;
            *(u32 *)xx += 0x01010101;
            *(u32 *)xx &= 0x7f7f7f7f;
            value = byte_DA43C[128 * yy[0] + xx[0]] + a13;
            value += byte_DA43C[128 * yy[1] + xx[1]];
            value += byte_DA43C[128 * yy[2] + xx[2]];
            value += byte_DA43C[128 * yy[3] + xx[3]];
            color = (value >> 4) + plasma_palette_color_index;
            *pos++ = color;
        }
        pos += skip;
    }
}

void PlasmaBox(s16 x, s16 y, s16 width, s16 height, u8 a5)
{
    if (a5 != 0) {
        word_95FA0 += 1;
        word_95FA2 -= 2;
        word_95FA4 += 1;
        word_95FA6 -= 2;
        word_95FA8 += 3;
        word_95FAA -= 2;
        word_95FAC += 1;
        word_95FAE -= 3;
        word_95FB0 += 5;
        word_DE748 = (sinus(word_95FA0) + 32) >> 4;
        word_DE746 = (cosinus(word_95FA2) + 32) >> 4;
        word_DE74C = (cosinus(word_95FA4) + 32) >> 4;
        word_DE74A = (sinus(word_95FA6) + 32) >> 4;
        word_DE744 = (cosinus(word_95FA8) + 32) >> 4;
        word_DE742 = (sinus(word_95FAA) + 32) >> 4;
        word_DE740 = (cosinus(word_95FAC) + 32) >> 4;
        word_DE73E = (sinus(word_95FAE) + 32) >> 4;
    }
    x -= 3;
    y -= 3;
    width += 6;
    height += 6;
    if (x < 0) {
        width += x;
        x = 0;
    } else if (x + width > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - x;
    }
    if (y < 0) {
        y = 0;
        height += x;
    } else if (y + height > SCREEN_WIDTH) {
        height = SCREEN_HEIGHT - y;
    }
    Plasma(x, y, width, height, word_DE74C, word_DE74A, word_DE748, word_DE746,
           word_DE744, word_DE742, word_DE740, word_DE73E, word_95FB0);
}

void InitFire(void)
{
    s16 i;

    MakeMyRand(1);
    if (menuEtape == 0)
        plasma_palette_color_index = 74;
    else
        plasma_palette_color_index = 112;
    for (i = 0; i < 16; ++i) {
        rvb[0].colors[plasma_palette_color_index + i].r = pal[i].r >> 2;
        rvb[0].colors[plasma_palette_color_index + i].g = pal[i].b >> 2;
        rvb[0].colors[plasma_palette_color_index + i].b = pal[i].g >> 2;
    }
    BOX_SET_PALETTE(plasma_palette_color_index, plasma_palette_color_index + 15);
    {
        s16 y;
        for (y = 0; y < 70; ++y) {
            s16 x;
            for (x = 0; x < 300; ++x)
                p1[y][x] = plasma_palette_color_index;
        }
    }
}

void Fire(s16 x, s16 y, s16 width, s16 height)
{
    u8 *pos = draw_buffer + x + SCREEN_WIDTH * y;
    s16 color = plasma_palette_color_index;
    s16 i;
    MakeMyRand(0);
    for (i = 0; i < width; ++i) {
        if (myRand(10) < 5)
            color = 15 * myRand(1) + plasma_palette_color_index;
        p1[height][i] = color;
        p1[height + 1][i] = color;
    }
    for (i = 1; i < height + 2; ++i) {
        s16 j;
        for (j = 0; j < width; ++j) {
            if (j == 0) {
                p1[i - 1][j] = (p1[i - 1][width - 1] + p1[i][j] + p1[i][j + 1] + p1[i + 1][j]) >> 2;
            } else if (j == width - 1) {
                p1[i - 1][j] = (p1[i][j - 1] + p1[i][j] + p1[i + 1][0] + p1[i + 1][j]) >> 2;
            } else {
                p1[i - 1][j] = (p1[i][j - 1] + p1[i][j] + p1[i][j + 1] + p1[i + 1][j]) >> 2;
            }
        }
    }
    for (i = 0; i < height; ++i) {
        s16 j;
        for (j = 0; j < width; ++j)
            *pos++ = p1[i][j];
        pos += SCREEN_WIDTH - width;
    }
}

void FireBox(s16 x, s16 y, s16 width, s16 height)
{
    x -= 3;
    y -= 3;
    width += 6;
    height += 6;
    if (x < 0) {
        width += x;
        x = 0;
    } else if (x + width > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - x;
    }
    if (y < 0) {
        y = 0;
        height += x;
    } else if (y + height > SCREEN_WIDTH) {
        height = SCREEN_HEIGHT - y;
    }
    Fire(x, y, width, height);
}

void InitMenuPalette(void)
{
    s16 i;
    for (i = 0; i < 80; ++i) {
        MenuPalette.colors[i].r = rvb[current_pal_id].colors[i].r;
        MenuPalette.colors[i].g = rvb[current_pal_id].colors[i].g;
        MenuPalette.colors[i].b = rvb[current_pal_id].colors[i].b;
    }
    for (i = 70; i < 102; ++i) {
        MenuPalette.colors[i].r = 10 - i * 2;
        MenuPalette.colors[i].g = i * 2 - 140;
        MenuPalette.colors[i].b = 0;
    }
    for (i = 102; i < 112; ++i) {
        MenuPalette.colors[i].r = rvb[current_pal_id].colors[i].r;
        MenuPalette.colors[i].g = rvb[current_pal_id].colors[i].g;
        MenuPalette.colors[i].b = rvb[current_pal_id].colors[i].b;
    }
    for (i = 112; i < 128; ++i) {
        MenuPalette.colors[i].r = PalPlasma[i - 112].b;
        MenuPalette.colors[i].g = PalPlasma[i - 112].g;
        MenuPalette.colors[i].b = PalPlasma[i - 112].r;
    }
    for (i = 128; i < 192; ++i) {
        MenuPalette.colors[i].r = i - 128;
        MenuPalette.colors[i].g = i - 128;
        MenuPalette.colors[i].b = i - 128;
    }
    for (i = 192; i < 256; ++i) {
        MenuPalette.colors[i].r = 0;
        MenuPalette.colors[i].g = 0;
        {
            u8 c = i - 192 > 3 ? i - 192 : 3;
            MenuPalette.colors[i].b = c;
        }
    }
    if (dans_la_map_monde) {
        MenuPalette.colors[32].r = 16;
        MenuPalette.colors[32].g = 14;
        MenuPalette.colors[32].b = 13;
        MenuPalette.colors[38].r = 49;
        MenuPalette.colors[38].g = 47;
        MenuPalette.colors[38].b = 45;
    }
}

void CaptureVideo(u8 *source_buf, u8 grayscale)
{
    if (ModeVideoActuel == MODE_X) {
        u16 plane;
        for (plane = 0; plane < 4; ++plane) {
            u8 *source_pos = source_buf;
            u8 *dest_pos = EffetBufferNormal + plane;
            u16 i;
            outpw(0x3ce, (plane << 8) | 4);
            for (i = 0; i < 16000; ++i) {
                u8 c = *source_pos;
                if (grayscale)
                    c = (rvb[current_pal_id].colors[c].g + rvb[current_pal_id].colors[c].r + rvb[current_pal_id].colors[c].b) / 3 + 128;
                *dest_pos = c;
                dest_pos += 4;
                ++source_pos;
            }
        }
    } else {
        u8 *source_pos = source_buf;
        u8 *dest_pos = EffetBufferNormal;
        if (grayscale) {
            u16 i;
            for (i = 0; i < 0xfa00; ++i) {
                u8 c = *source_pos++;
                *dest_pos++ = (rvb[current_pal_id].colors[c].r + rvb[current_pal_id].colors[c].g + rvb[current_pal_id].colors[c].b) / 3 + 128;
            }
        } else {
            memcpy(dest_pos, source_pos, 0xfa00);
        }
    }
}

void CadreTrans(s16 x, s16 y, s16 w, s16 h)
{
    u32 *pos;
    s16 skip;
    s16 j;
    s16 old_w;
    x += 4 - (x & 3);
    w += 4 - (w & 3);
    skip = (320 - w) >> 2;
    pos = (u32 *)(draw_buffer + x + SCREEN_WIDTH * y);
    old_w = w;
    w >>= 2;
    for (j = 0; j < h; ++j) {
        s16 i;
        for (i = 0; i < w; ++i) {
            if (*pos)
                *pos += 0x40404040;
            else
                *pos = 0xc8c8c8c8;
            ++pos;
        }
        pos += skip;
    }
    DrawBorderBoxNormal(draw_buffer, x, y, h, old_w, 0x2026);
}

void DrawBlackBorderBox(s16 x, s16 y, s16 width, s16 height, u8 a5)
{
    s16 colors;
    if (a5)
        colors = 0x2026;
    else
        colors = 0x2620;
    DrawBlackBoxNormal(draw_buffer, x, y, height, width);
    DrawBorderBoxNormal(draw_buffer, x, y, height, width, colors);
}

void InitGauge(void)
{
    if (menuEtape != 0 && OptionGame == 0) {
        u8 i = 0;
        u8 r = 63;
        for (; i < 32; ++i) {
            u8 g = i + i;
            rvb[current_pal_id].colors[70 + i].r = r;
            rvb[current_pal_id].colors[70 + i].g = g;
            rvb[current_pal_id].colors[70 + i].b = 0;
            r -= 2;
        }
    }
}

void gauge(s16 x, s16 y, s16 width, s16 height, s16 min_value, s16 max_value, s16 value)
{
    u8 *pos = draw_buffer + x + 1 + SCREEN_WIDTH * (y + 1);
    s16 step;
    s16 fill;
    u8 row, col, color, run;
    if (value > max_value) value = max_value;
    if (value < min_value) value = 0;
    else value -= min_value;
    step = max32(width >> 3, 8);
    fill = x + 1 + value * (s16)(width - step - 3) / (max_value - min_value);
    DrawBorderBoxNormal(draw_buffer, x, y, height, width, 0x2026);
    color = 0;
    for (row = 0; row < height - 2; ++row) {
        for (col = 0, run = 0; col < width - 2; ++col) {
            *pos = 70 + color;
            if (run > ((width - 2) >> 5)) {
                run = 0;
                ++color;
            }
            ++pos;
            ++run;
        }
        color = 0;
        pos += SCREEN_WIDTH - width + 2;
    }
    DrawBlackBoxNormal(draw_buffer, fill, y + 1, height - 3, step);
    DrawBorderBoxNormal(draw_buffer, fill, y + 1, height - 3, step, 0x0308);
}

void ClearBorder(s16 lim_H1, s16 lim_H2, s16 lim_W1, s16 lim_W2)
{
    u8 *pos;
    s16 y;
    memset(draw_buffer, 0, lim_H1 * SCREEN_WIDTH);
    pos = draw_buffer + lim_H1 * SCREEN_WIDTH;
    memset(draw_buffer + lim_H2 * SCREEN_WIDTH, 0, (SCREEN_HEIGHT - lim_H2) * SCREEN_WIDTH);
    for (y = lim_H1; y < lim_H2; ++y) {
        s16 x;
        for (x = 0; x < lim_W1; ++x)
            *pos++ = 0;
        pos += lim_W2 - lim_W1;
        for (x = lim_W2; x < SCREEN_WIDTH; ++x)
            *pos++ = 0;
    }
}
