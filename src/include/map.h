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

/* block_flags[] element: the DOS code reads bit 5 through a bitfield (basic on_block_chdir is STRICT only with the
   bitfield read; byte-mask spellings are not). Type name and member names INVENTED; Rayverse uses u8. */
typedef union block_flags_t {
    u8 raw;
    struct {
        u8 unused0 : 5;
        u8 chdir : 1;
        u8 unused1 : 2;
    } bits;
} block_flags_t;

#endif
