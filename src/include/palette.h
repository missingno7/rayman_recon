/* Palette types (DOS v1.21). Separate header so data units that only need palettes do not depend on the size
   of game.h: Watcom's _BSS order depends on the number of declarations in the TU (docs/DATA.md). */
#ifndef PALETTE_H
#define PALETTE_H

#include "rtypes.h"

typedef struct rgb_t {
    u8 r;
    u8 g;
    u8 b;
} rgb_t;

typedef struct rgb_palette_t {
    rgb_t colors[256];
} rgb_palette_t;

#endif
