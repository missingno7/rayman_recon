/*
 * Miscellaneous game structures (DOS v1.21). Layouts from Rayverse types.h; each confirmed by the STRICT unit noted.
 */
#ifndef GAME_H
#define GAME_H

#include "rtypes.h"

typedef struct rgb_t {
    u8 r;
    u8 g;
    u8 b;
} rgb_t;

typedef struct rgb_palette_t {
    rgb_t colors[256];
} rgb_palette_t;

typedef struct status_bar_t {       /* src/bonus.c */
    s16 lives;
    u8 lives_digits[2];
    u8 hp_sprites[2];
    u8 num_wiz;
    u8 wiz_digits[2];
    u8 max_hitp;
} status_bar_t;

typedef struct world_info_t {       /* src/cage.c */
    s16 xpos;
    s16 ypos;
    u8 index_up;
    u8 index_down;
    u8 index_left;
    u8 index_right;
    u8 state;
    u8 nb_cages;
    u8 world;
    u8 level;
    s8 color;
    u8 field_D;
    u8 field_E;
    u8 field_F;
    char *text;
} world_info_t;

#endif
