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
        u8 unused0 : 1;
        u8 solid : 1;                  /* bit 1 - STRONG: basic calc_btyp extracts it with shl 6 / shr 7 */
        u8 unused2 : 3;
        u8 chdir : 1;
        u8 unused1 : 2;
    } bits;
} block_flags_t;

/* Proposed shared map descriptors; DOS offsets and Android BIG_MAP size32. */
typedef struct display_map_t {
    s16 x;
    s16 y;
    u8 width_in_tiles;
    u8 height_in_tiles;
    u16 tile_index;
} display_map_t;

typedef struct big_map_t {
    map_tile_t* map;
    u8** tile_texture_offsets;
    display_map_t* display_map;
    u8* map_blocks;
    u32 field_10;
    s16 field_14;
    s16 width;
    s16 height;
    s16 field_1a;
    s16 field_1c;
    s16 field_1e;
} big_map_t;

#endif
