/*
 * Map structures (DOS v1.21). Layouts: Rayverse types.h; sizes agree with Android symbols (mp: 12 bytes).
 * Used by STRICT functions in src/ufo.c.
 */
#ifndef MAP_H
#define MAP_H

#include "rtypes.h"

typedef struct map_tile_t {
    u16 texture_id;
    u8 tile_type;
    u8 field_3;
    u8 transparency;
    u8 field_5;
} map_tile_t;

typedef struct map_data_t {
    s16 width;
    s16 height;
    s32 length;
    map_tile_t *map;
} map_data_t;

#endif
