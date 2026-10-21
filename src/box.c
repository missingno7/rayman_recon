/* box.c (1:13f30..1:14e90: plasma/fire boxes, gauges, palette/capture helpers; PC-only). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_box. Pragmas declare assembly-routine register ABIs only. */
/* DOS v1.21 box.c candidate: object 1:13f30..1:14e90. */
#include "game.h"
#include "rtypes.h"
#include <conio.h>
#include <math.h>
#include <string.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define MODE_X 1

extern u8 *draw_buffer;
extern u8 *EffetBufferNormal;
extern u8 byte_DA43C[128 * 128];
extern s16 p1[21000];
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

extern void DrawBlackBoxNormal(u8 *buffer, s16 x, s16 y, s16 width, s16 height);
#pragma aux DrawBlackBoxNormal parm [edi] [eax] [ebx] [edx] [ecx] modify []
extern void DrawBorderBoxNormal(u8 *buffer, s16 x, s16 y, s16 width, s16 height, u16 colors);
#pragma aux DrawBorderBoxNormal parm [edi] [esi] [ebx] [edx] [ecx] [eax] modify [eax ebx ecx edx]
extern s16 cosinus(s16 x);
extern s16 sinus(s16 x);
extern void MakeMyRand(int set_seed);
extern s16 myRand(s16 max);

void CalcTab(void)
{
    s16 j;

    for (j = 0; j < 128; ++j) {
        s16 i;
        for (i = 0; i < 128; ++i) {
            s32 squared = (64 - i) * (64 - i);
            double v;
            squared += (64 - j) * (64 - j);
            v = sqrt(squared);
            v /= 9.5;
            v = sin(v);
            byte_DA43C[128 * j + i] = (u8)((v + 1.0) * 40.0);
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
        rgb_t *color = rvb[0].colors + plasma_palette_color_index + i;
        color->r = PalPlasma[i].b;
        color->g = PalPlasma[i].g;
        color->b = PalPlasma[i].r;
    }
    if (need_set_palette) {
        outp(0x3c8, plasma_palette_color_index);
        for (i = 0; i < 16; ++i) {
            rgb_t *color = rvb[0].colors + plasma_palette_color_index + i;
            outp(0x3c9, color->r);
            outp(0x3c9, color->g);
            outp(0x3c9, color->b);
        }
    }
}

void Plasma(s16 x, s16 y, s16 width, s16 height,
            s16 a5, s16 a6, s16 a7, s16 a8, s16 a9, s16 a10, s16 a11, s16 a12, s16 a13)
{
    u8 *row = draw_buffer + SCREEN_WIDTH * y + x;
    u32 v17 = (u8)a6 | ((u8)a8 << 8) | ((u8)a10 << 16) | ((u8)a12 << 24);
    s16 j;

    for (j = 0; j < height; ++j) {
        u32 v18;
        u8 *pos;
        s16 i;

        v17 = (v17 + 0x01010101) & 0x7f7f7f7f;
        v18 = (u8)a5 | ((u8)a7 << 8) | ((u8)a9 << 16) | ((u8)a11 << 24);
        pos = row;
        for (i = 0; i < width; ++i) {
            u8 value;

            v18 = (v18 + 0x01010101) & 0x7f7f7f7f;
            value = (u8)a13 + byte_DA43C[128 * (u8)v17 + (u8)v18];
            value += byte_DA43C[128 * (u8)(v17 >> 8) + (u8)(v18 >> 8)];
            value += byte_DA43C[128 * (u8)(v17 >> 16) + (u8)(v18 >> 16)];
            value += byte_DA43C[128 * (u8)(v17 >> 24) + (u8)(v18 >> 24)];
            value >>= 4;
            value += plasma_palette_color_index;
            *pos++ = value;
        }
        row += SCREEN_WIDTH;
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
        rgb_t *color = rvb[0].colors + plasma_palette_color_index + i;
        color->r = pal[i].r >> 2;
        color->g = pal[i].b >> 2;
        color->b = pal[i].g >> 2;
    }
    outp(0x3c8, plasma_palette_color_index);
    for (i = 0; i < 16; ++i) {
        rgb_t *color = rvb[0].colors + plasma_palette_color_index + i;
        outp(0x3c9, color->r);
        outp(0x3c9, color->g);
        outp(0x3c9, color->b);
    }
    {
        s16 y;
        for (y = 0; y < 70; ++y) {
            s16 x;
            for (x = 0; x < 300; ++x)
                p1[300 * y + x] = plasma_palette_color_index;
        }
    }
}

void Fire(s16 x, s16 y, s16 width, s16 height)
{
    u8 *row = draw_buffer + SCREEN_WIDTH * y + x;
    u8 color = plasma_palette_color_index;
    s16 i;

    MakeMyRand(0);
    for (i = 0; i < width; ++i) {
        s16 r = myRand(10);
        if (r < 5)
            color = 15 * myRand(1) + plasma_palette_color_index;
        p1[300 * height + i] = color;
        p1[300 * height + 300 + i] = color;
    }

    for (i = 1; i <= height; ++i) {
        s16 j;
        for (j = 0; j < width; ++j) {
            if (j != 0) {
                if (j == width - 1) {
                    p1[300 * i - 300 + j] =
                        (p1[300 * i + 300] + p1[300 * i + j] + p1[300 * i + j - 1] +
                         p1[300 * i + j + 300]) >> 2;
                } else {
                    p1[300 * i - 300 + j] =
                        (p1[300 * i + j + 1] + p1[300 * i + j] + p1[300 * i + j - 1] +
                         p1[300 * i + j + 300]) >> 2;
                }
            } else {
                p1[300 * i - 300 + j] =
                    (p1[300 * i - 301 + width] + p1[300 * i] + p1[300 * i - 1] +
                     p1[300 * i + 300]) >> 2;
            }
        }
    }

    for (i = 0; i < height; ++i) {
        u8 *pos = row;
        s16 j;
        for (j = 0; j < width; ++j)
            *pos++ = p1[300 * i + j];
        row += SCREEN_WIDTH;
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
    rgb_palette_t *cur_pal = rvb + current_pal_id;
    s16 i;

    for (i = 0; i < 80; ++i)
        MenuPalette.colors[i] = cur_pal->colors[i];
    {
        u8 r = 126;
        u8 g = 0;
        u8 b = 0;
        for (i = 70; i < 102; ++i) {
            MenuPalette.colors[i].r = r;
            MenuPalette.colors[i].g = g;
            MenuPalette.colors[i].b = b;
            r -= 2;
            g += 2;
        }
    }
    for (i = 102; i < 112; ++i)
        MenuPalette.colors[i] = cur_pal->colors[i];
    for (i = 112; i < 128; ++i)
        MenuPalette.colors[i] = PalPlasma[i - 112];
    for (i = 128; i < 192; ++i) {
        u8 c = (u8)((i - 128) << 2);
        MenuPalette.colors[i].r = c;
        MenuPalette.colors[i].g = c;
        MenuPalette.colors[i].b = c;
    }
    for (i = 192; i < 256; ++i) {
        u8 c = i - 192;
        if (c <= 3)
            c = 3;
        c <<= 2;
        MenuPalette.colors[i].r = 0;
        MenuPalette.colors[i].g = 0;
        MenuPalette.colors[i].b = c;
    }
    if (dans_la_map_monde) {
        MenuPalette.colors[32].r = 16 << 2;
        MenuPalette.colors[32].g = 14 << 2;
        MenuPalette.colors[32].b = 13 << 2;
        MenuPalette.colors[38].r = 49 << 2;
        MenuPalette.colors[38].g = 47 << 2;
        MenuPalette.colors[38].b = 45 << 2;
    }
    (void)cur_pal;
}

void CaptureVideo(u8 *source_buf, u8 grayscale)
{
    if (ModeVideoActuel == MODE_X) {
        u8 *dest_buf = EffetBufferNormal;
        s16 plane;

        for (plane = 0; plane < 4; ++plane) {
            u8 *dest_pos = dest_buf + plane;
            s16 i;

            outpw(0x3ce, (plane << 8) | 4);
            for (i = 0; i < 16000; ++i) {
                u8 c = *source_buf++;

                if (grayscale) {
                    rgb_t rgb = rvb[current_pal_id].colors[c];
                    c = (u8)((rgb.r + rgb.g + rgb.b) / 3 + 128);
                }
                *dest_pos = c;
                dest_pos += 4;
            }
        }
    } else if (grayscale) {
        u8 *dest_buf = EffetBufferNormal;
        rgb_palette_t *palette = rvb + current_pal_id;
        s16 i;

        for (i = 0; i < 0xfa00; ++i) {
            u8 c = *source_buf++;
            rgb_t rgb = palette->colors[c];
            *dest_buf++ = (u8)((rgb.r + rgb.g + rgb.b) / 3 + 128);
        }
    } else {
        memcpy(EffetBufferNormal, source_buf, 0xfa00);
    }
}

void CadreTrans(s16 x, s16 y, s16 w, s16 h)
{
    u8 *line = draw_buffer + SCREEN_WIDTH * y + x;
    s16 j;

    for (j = 0; j < h; ++j) {
        u8 *pos = line;
        s16 i;
        for (i = 0; i < w; ++i) {
            if (*pos)
                *pos += 0x40;
            else
                *pos = 0xc8;
            ++pos;
        }
        line += SCREEN_WIDTH;
    }
    DrawBorderBoxNormal(draw_buffer, x, y, w, h, 0x2026);
}

void DrawBlackBorderBox(s16 x, s16 y, s16 height, s16 width, u8 a5)
{
    u16 colors = a5 ? 0x2026 : 0x2620;
    DrawBlackBoxNormal(draw_buffer, x, y, height, width);
    DrawBorderBoxNormal(draw_buffer, x, y, height, width, colors);
}

void InitGauge(void)
{
    u8 pal_id = current_pal_id;

    if (menuEtape != 0 && OptionGame == 0) {
        u8 r = 63;
        u8 g = 0;
        u8 i;

        for (i = 0; i < 32; ++i) {
            rgb_t *color = rvb[pal_id].colors + 70 + i;
            color->r = r;
            color->g = g;
            color->b = 0;
            r -= 2;
        }
    }
}

void gauge(s16 x, s16 y, s16 width, s16 height, s16 min_value, s16 max_value, s16 value)
{
    s16 step = height >> 3;
    s16 span;
    s16 amount;
    s16 fill;
    s16 row;
    u8 *pos;

    if (step < 8)
        step = 8;
    span = max_value - min_value;
    if (value > max_value)
        amount = span;
    else if (value < min_value)
        amount = 0;
    else
        amount = value - min_value;
    fill = (s16)(amount * (width - step - 3) / span);
    DrawBorderBoxNormal(draw_buffer, x, y, width, height, 0x2026);
    pos = draw_buffer + SCREEN_WIDTH * (y + 1) + x + 1;
    for (row = 0; row < height - 2; ++row) {
        s16 col;
        for (col = 0; col < step - 2; ++col)
            pos[col] = (u8)(0x46 + col / 32);
        pos += SCREEN_WIDTH;
    }
    DrawBlackBoxNormal(draw_buffer, x + 1, y + 1, width - 3, height - 3);
    DrawBorderBoxNormal(draw_buffer, x + 1, y + 1, width - 3, height - 3, 0x0308);
    (void)fill;
}

void ClearBorder(s16 lim_H1, s16 lim_H2, s16 lim_W1, s16 lim_W2)
{
    s16 y;

    memset(draw_buffer, 0, lim_H1 * SCREEN_WIDTH);
    memset(draw_buffer, 0, (SCREEN_HEIGHT - lim_H2) * SCREEN_WIDTH);
    for (y = lim_H1; y < lim_H2; ++y) {
        u8 *row = draw_buffer + SCREEN_WIDTH * y;
        if (lim_W1 > 0)
            memset(row, 0, lim_W1);
        if (lim_W2 < SCREEN_WIDTH)
            memset(row + lim_W2, 0, SCREEN_WIDTH - lim_W2);
    }
}
