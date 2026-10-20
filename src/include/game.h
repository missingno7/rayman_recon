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

typedef struct poing_t {            /* poing: 20 bytes (Android agrees); s16 fields per src/poing.c */
    s32 y_16;
    s16 field_4;
    s16 speed_x;
    s16 charge;
    s16 field_A;
    u8 sub_etat;
    u8 is_returning;
    u8 is_active;
    u8 damage;
    u8 is_charging;
    u8 is_boum;
    u8 field_12;
    u8 field_13;
} poing_t;

typedef struct rayevts_t {          /* RayEvts: 2 bytes (Android agrees); src/poing.c */
    u8 poing : 1;
    u8 hang : 1;
    u8 helico : 1;
    u8 super_helico : 1;
    u8 handstand_dash : 1;
    u8 handstand : 1;
    u8 magicseed : 1;
    u8 grap : 1;
    u8 run : 1;
    u8 tiny : 1;
    u8 firefly : 1;
    u8 force_run : 2;
    u8 reverse : 2;
    u8 squashed : 1;
} rayevts_t;

typedef struct finbosslevel_t {    /* src/basic.c, src/moskito.c */
    u8 bzzit : 1; u8 moskito : 1; u8 mr_sax : 1; u8 mr_stone : 1;
    u8 viking_mama : 1; u8 space_mama : 1; u8 mr_skops : 1; u8 mr_dark : 1;
    u8 crazy_drummer : 1; u8 helped_joe_1 : 1; u8 helped_joe_2 : 1; u8 helped_musician : 1;
} finbosslevel_t;

#endif
