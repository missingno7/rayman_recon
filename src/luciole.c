/*
 * Fireflies (GOG v1.21 object 1:384c0-1:38ef0). File name invented (Rayverse luciole.c; stubs there, bodies from
 * the binary and PS1). Names: Rayverse/Android. Status per function: config/units.json.
 */
#include <string.h>

#include "obj.h"
#include "game.h"
#include "rtypes.h"

extern obj_t ray;
extern obj_t *poing_obj;
extern poing_t poing;
extern rayevts_t RayEvts;
extern u8 horloge[25];
extern u8 GameModeVideo;
extern s16 ray_mode;
extern s16 id_Cling_Old;
extern s16 nb_fade;
extern s16 xmap;
extern s16 ymap;

extern s16 x_ray[4];
extern s16 y_ray[4];
extern s16 positiony2;
extern s16 old_x_luc;
extern s16 debut_titre;
extern s16 old_y_luc;
extern s16 choix_menu;
extern s16 ADDLUCLIP;
extern s16 debut_options;
extern s16 rayon_luciole;
extern s16 x_luc;
extern s16 y_luc;
extern s16 coeffktxt;
extern s16 y_main_luc;
extern s16 x_main_luc;
extern s16 vx_luc;
extern s16 n_ray;
extern s16 vy_luc;
extern s16 rotationtxt;

extern s32 Bloc_lim_W2_Aff;
extern s32 Bloc_lim_W1_Aff;
extern s32 Bloc_lim_H2_Glob;
extern s32 Bloc_lim_H1_Glob;
extern s32 Bloc_lim_W2_Glob;
extern s32 Bloc_lim_W1_Glob;
extern s32 Bloc_lim_H2_Aff;
extern s32 Bloc_lim_H1_Aff;
extern s32 Bloc_lim_W2;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_H1;

void sprite_clipping(s32 xmin, s32 ymax, s32 xmax, s32 ymin);
void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height, u8 font_size, u8 is_fond);
u8 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
s16 myRand(s16 max);
s16 cosinus(s16 x);
s16 sinus(s16 x);

void get_luciole(void)
{
    if (RayEvts.firefly) {
        Bloc_lim_H1_Glob = Bloc_lim_H1;
        Bloc_lim_H2_Glob = Bloc_lim_H2;
        Bloc_lim_W1_Glob = Bloc_lim_W1;
        Bloc_lim_W2_Glob = Bloc_lim_W2;

        Bloc_lim_H1 = Bloc_lim_H1_Aff;
        Bloc_lim_H2 = Bloc_lim_H2_Aff;
        Bloc_lim_W1 = Bloc_lim_W1_Aff;
        Bloc_lim_W2 = Bloc_lim_W2_Aff;
        sprite_clipping(Bloc_lim_W1, Bloc_lim_H2, Bloc_lim_W2, Bloc_lim_H1);
    }
}

void CLEAR_FIXE_LUCIOLE(void)
{
    if (RayEvts.firefly) {
        s16 y = 35;
        s16 x = 77;

        if (id_Cling_Old != 0) {
            x = 85;
            y = 65;
            id_Cling_Old = 0;
        }
        DISPLAY_BLACKBOX(13, 0, x, y, 0xff, 0);
        DISPLAY_BLACKBOX(241, 4, 68, 23, 0xff, 0);
    }
}

void set_luciole(s32 a1, s32 a2)
{
    s32 x1;
    s32 x2;
    s32 y1;
    s32 y2;

    x1 = a1 - rayon_luciole;
    x2 = a1 + rayon_luciole;
    y1 = a2 - rayon_luciole;
    y2 = a2 + rayon_luciole;

    if (y1 < Bloc_lim_H1_Glob)
        y1 = Bloc_lim_H1_Glob;
    if (y1 > Bloc_lim_H2_Glob)
        y1 = Bloc_lim_H2_Glob;
    if (x1 < Bloc_lim_W1_Glob)
        x1 = Bloc_lim_W1_Glob;
    if (x1 > Bloc_lim_W2_Glob)
        x1 = Bloc_lim_W2_Glob;
    if (y2 > Bloc_lim_H2_Glob)
        y2 = Bloc_lim_H2_Glob;
    if (y2 < Bloc_lim_H1_Glob)
        y2 = Bloc_lim_H1_Glob;
    if (x2 > Bloc_lim_W2_Glob)
        x2 = Bloc_lim_W2_Glob;
    if (x2 < Bloc_lim_W1_Glob)
        x2 = Bloc_lim_W1_Glob;

    Bloc_lim_H1_Aff = y1;
    Bloc_lim_H2_Aff = y2;
    Bloc_lim_W2_Aff = x2;
    Bloc_lim_W1_Aff = x1;
}

void init_aff_luciole(s32 a1, s32 a2)
{
    rotationtxt = 0;
    Bloc_lim_H1_Glob = Bloc_lim_H1;
    coeffktxt = 5;
    Bloc_lim_H2_Glob = Bloc_lim_H2;
    ADDLUCLIP = 15;
    Bloc_lim_W1_Glob = Bloc_lim_W1;
    rayon_luciole = 0;
    Bloc_lim_W2_Glob = Bloc_lim_W2;
    set_luciole(a1, a2);
}

void plot2line(s32 a1, s32 a2, s32 a3, s32 a4, u8 *draw_buf)
{
    s32 x_outer_left;
    s32 left_width;
    s32 right_width;
    register s32 radius;
    register s32 x_outer_right;
    register s32 x_inner_left;
    register s32 x_inner_right;
    register s32 y_top;
    register s32 y_bottom;
    register s32 i;
    register u8 *p;

    radius = rayon_luciole + ADDLUCLIP;
    x_outer_left = a1 - radius;
    x_outer_right = a1 + radius;
    x_inner_left = a1 - a3;
    if (x_outer_left < Bloc_lim_W1_Glob)
        x_outer_left = Bloc_lim_W1_Glob;
    if (x_inner_left > Bloc_lim_W2_Glob)
        x_inner_left = Bloc_lim_W2_Glob;
    left_width = x_inner_left - x_outer_left;

    x_inner_right = a1 + a3;
    if (x_outer_right > Bloc_lim_W2_Glob)
        x_outer_right = Bloc_lim_W2_Glob;
    if (x_inner_right < Bloc_lim_W1_Glob)
        x_inner_right = Bloc_lim_W1_Glob;
    right_width = x_outer_right - x_inner_right;

    y_top = a2 - a4;
    if (left_width > 0) {
        if (y_top >= Bloc_lim_H1_Glob && y_top < Bloc_lim_H2_Glob) {
            p = draw_buf + y_top * 320 + x_outer_left;
            for (i = 0; i < left_width; ++i)
                p[i] = 0;
        }
        y_bottom = a2 + a4;
        if (y_bottom >= Bloc_lim_H1_Glob && y_bottom < Bloc_lim_H2_Glob) {
            p = draw_buf + y_bottom * 320 + x_outer_left;
            for (i = 0; i < left_width; ++i)
                p[i] = 0;
        }
    }

    if (right_width > 0) {
        if (y_top >= Bloc_lim_H1_Glob && y_top < Bloc_lim_H2_Glob) {
            p = draw_buf + y_top * 320 + x_inner_right;
            for (i = 0; i < right_width; ++i)
                p[i] = 0;
        }
        y_bottom = a2 + a4;
        if (y_bottom >= Bloc_lim_H1_Glob && y_bottom < Bloc_lim_H2_Glob) {
            p = draw_buf + y_bottom * 320 + x_inner_right;
            for (i = 0; i < right_width; ++i)
                p[i] = 0;
        }
    }
}

void aff_luciole(s32 a1, s32 a2, s32 a3, u8 *draw_buf)
{
    s32 i;
    s32 y;
    s32 d;
    s32 err;
    s32 x;

    i = 0;
    y = a3;
    while (i <= ADDLUCLIP) {
        plot2line(a1, a2, 0, y, draw_buf);
        ++i;
        ++y;
    }

    x = a3;
    if (x >= 0) {
        i = 0;
        d = 1;
        err = 2 * x - 1;
        y = 0;
        while (i <= x) {
            plot2line(a1, a2, i, x, draw_buf);
            plot2line(a1, a2, x, i, draw_buf);
            ++i;
            y += d;
            d += 2;
            if (x <= y) {
                --x;
                y -= err;
                err -= 2;
            }
        }
    }
}

void Display_and_free_luciole(u8 *draw_buf)
{
    if (RayEvts.firefly) {
        s32 dx;
        s32 dy;
        s32 distance;

        dx = old_x_luc - x_luc;
        if (dx < 0)
            dx = -dx;
        dy = old_y_luc - y_luc;
        if (dy < 0)
            dy = -dy;
        distance = dy;
        if (distance < dx)
            distance = dx;
        ADDLUCLIP = distance + 6;

        old_y_luc = y_luc;
        old_x_luc = x_luc;
        aff_luciole(y_luc, x_luc, rayon_luciole - 4, draw_buf);

        Bloc_lim_H1 = Bloc_lim_H1_Glob;
        Bloc_lim_H2 = Bloc_lim_H2_Glob;
        Bloc_lim_W1 = Bloc_lim_W1_Glob;
        Bloc_lim_W2 = Bloc_lim_W2_Glob;
    }
}

void free_luciole(void)
{
    if (RayEvts.firefly) {
        Bloc_lim_H1 = Bloc_lim_H1_Glob;
        Bloc_lim_H2 = Bloc_lim_H2_Glob;
        Bloc_lim_W1 = Bloc_lim_W1_Glob;
        Bloc_lim_W2 = Bloc_lim_W2_Glob;
    }
}

void INIT_LUCIOLE(void)
{
    s16 x = ray.x + ray.offset_bx;
    s16 y = ray.y + ray.offset_hy - 16;
    u8 i;

    y_luc = x - xmap + 8;
    x_luc = y - ymap;
    n_ray = 0;
    for (i = 0; i < 4; ++i) {
        x_ray[i] = x;
        y_ray[i] = y;
    }
    x_main_luc = x;
    y_main_luc = y;
    vx_luc = 0;
    vy_luc = 0;
    if (GameModeVideo == 0)
        init_aff_luciole(y_luc, x_luc);
}

void DO_LUCIOLE(void)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;
    register s32 old_x;
    register s32 old_y;
    register s16 delta;

    if (ray_mode != 3 &&
        !(ray.main_etat == 2 && ray.sub_etat == 9) &&
        !(ray.main_etat == 3 && (ray.sub_etat == 0x16 || ray.sub_etat == 0x20))) {
        if (rayon_luciole < 54 && nb_fade == 0) {
            rayon_luciole = ((512 - cosinus(rotationtxt)) * 54) >> 9;
            if (rotationtxt < 128)
                rotationtxt += 2;
        } else if (horloge[2] != 0) {
            rayon_luciole--;
        } else {
            rayon_luciole++;
        }

        old_x = x_ray[n_ray];
        old_y = y_ray[n_ray];
        if (poing.is_active) {
            x_ray[n_ray] = poing_obj->x + poing_obj->offset_bx;
            y_ray[n_ray] = poing_obj->y + poing_obj->offset_by;
        } else {
            GET_SPRITE_POS(&ray, 2, &spr_x, &spr_y, &spr_w, &spr_h);
            x_ray[n_ray] = ray.x + ray.offset_bx;
            y_ray[n_ray] = ray.y + ray.offset_hy - 4;
            x_ray[n_ray] = spr_x + (spr_w > 1);
            y_ray[n_ray] = spr_y + (spr_h > 1);
        }
        n_ray++;
        n_ray &= 3;

        delta = old_x - x_main_luc - vx_luc;
        if (delta != 0)
            delta = delta > 0 ? 1 : -1;
        vx_luc += delta;
        if (vx_luc > 8)
            vx_luc = 8;
        if (vx_luc < -8)
            vx_luc = -8;
        delta = old_y - y_main_luc - vy_luc;
        if (delta != 0)
            delta = delta > 0 ? 1 : -1;
        vy_luc += delta;
        if (vy_luc > 8)
            vy_luc = 8;
        if (vy_luc < -8)
            vy_luc = -8;
    } else {
        if (rayon_luciole >= 54) {
            rotationtxt = myRand(512);
            rayon_luciole -= 2;
        }

        vx_luc = coeffktxt * cosinus(rotationtxt) >> 9;
        vy_luc = coeffktxt * sinus(rotationtxt) >> 9;
        if (horloge[2] != 0) {
            rayon_luciole--;
            if (rayon_luciole < 0)
                rayon_luciole = 0;
        }
        rotationtxt += myRand(54);
        if (rotationtxt > 512) {
            rotationtxt = 0;
            coeffktxt++;
            if (coeffktxt > 10)
                coeffktxt = 10;
        }
    }

    x_main_luc += vx_luc;
    y_main_luc += vy_luc;
    y_luc = x_main_luc - xmap + 8;
    x_luc = y_main_luc - ymap;
    if (GameModeVideo == 0)
        set_luciole(y_luc, x_luc);
}
