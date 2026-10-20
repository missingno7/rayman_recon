/* map.c (1:4c4a0..1:4d230). Names RAYVERSE; seeded by tools/seed.py, worker tu_map. See config/units.json for statuses. */
#include "obj.h"
#include "game.h"
#include "map.h"

typedef struct mem_t {
    u32 align;
    u8 *data;
    u32 capacity;
} mem_t;

typedef struct display_map_t {
    s16 x;
    s16 y;
    u8 width_in_tiles;
    u8 height_in_tiles;
    s16 tile_index;
} display_map_t;

typedef struct big_map_t {
    map_tile_t *map;
    u8 **tile_texture_offsets;
    display_map_t *display_map;
    u8 *map_blocks;
    u32 field_10;
    s16 field_14;
    s16 width;
    s16 height;
    s16 field_1a;
    s16 field_1c;
    s16 field_1e;
} big_map_t;

#define world_2_music 2
#define world_3_mountain 3
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H1_Aff;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_H2_Aff;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W1_Aff;
extern s32 Bloc_lim_W2;
extern s32 Bloc_lim_W2_Aff;
extern u8 OptionGame;
extern s16 bossScrollEndX;
extern s16 bossScrollStartX;
extern map_data_t mp;
extern s16 num_level;
extern s16 num_world;
extern obj_t ray;
extern u8 scrollLocked;
extern s16 scroll_end_x;
extern s16 scroll_end_y;
extern s16 scroll_start_x;
extern s16 scroll_start_y;
extern s16 xmap;
extern s16 xmapmax;
extern s16 xmapmaxini;
extern s16 ymap;
extern s16 ymapmax;
extern s16 ymapmaxini;
extern u8 *Scroll_Masque[20];
extern s32 block_add[1200];
extern u32 nb_blocks_plein;
extern s32 BufferNormalDeplt;
extern u8 *DrawBufferNormal;
extern u8 *DrawBufferNormalIni;
extern u8 *draw_buffer;
extern obj_t *ScrollDiffSprites;
extern u8 *block_malloc(mem_t *mem, s32 size);
extern void Display_Bloc_Plein_Clippe(s16 x, s16 y, s16 width, s16 height, u8 *buffer, u8 *tiles);
extern void Display_Bloc_Clippe(s16 x, s16 y, s16 width, s16 height, u8 *buffer, u8 *tiles);
extern void Display_Bloc_Plein(s16 x, s16 y, u8 *buffer, u8 *tiles);
extern void Display_Bloc(s16 x, s16 y, u8 *buffer, u8 *tiles);
extern void sprite_clipping(s32 x1, s32 x2, s32 y1, s32 y2);
extern void N_CLRSCR(u8 *buffer);
extern void Copy_Plan0Diff_To_Buf(u8 *source, u8 *dest, s32 width, s32 height, s32 draw_width);
void set_default_Bloc_clipping(void);
void set_xymapini(void);
void set_xymap(void);
void recaleRaysurlesBords(void);
void set_xywhmap(s16 xmin, s16 xmax, s16 ymin, s16 ymax);
void update_display_map(big_map_t *map);

u32 deter_vide_plein_panach(u8 *blocks, s16 width)
{
    u32 empty_count;
    s32 y;
    u8 *column;
    u8 *end;

    empty_count = 0;
    end = blocks + 16;
    y = 0;
    do {
        column = blocks;
        while (column != end) {
            if (*(column + y * width) == 0)
                empty_count++;
            column++;
        }
        y++;
    } while (y < 16);
    return empty_count;
}

void deter_nb_blocks(u8 *blocks, s16 width, s16 height, s32 *full_count, s32 *partial_count)
{
    s32 y;
    s32 x;
    s32 empty_count;

    y = 0;
    do {
        x = 0;
        if (y == 0)
            x = 16;
        do {
            empty_count = deter_vide_plein_panach(blocks + x + y * width, width);
            if (empty_count != 256) {
                if (empty_count == 0)
                    (*full_count)++;
                else
                    (*partial_count)++;
            }
            x += 16;
        } while (x < width);
        y += 16;
    } while (y < height);
}

u8 *Copy_Blocks(mem_t *mem, u8 *blocks, s16 width, s16 height)
{
    s32 width_in_tiles;
    s32 height_in_tiles;
    s32 full_count;
    s32 partial_count;
    s32 total_count;
    s32 tile_x;
    s32 tile_y;
    s32 tile_index;
    s32 row;
    s32 col;
    s32 empty_count;
    s32 nonzero_count;
    s32 row_flags;
    u8 *result;
    u8 *full_block;
    u8 *partial_block;
    u8 *source;

    width_in_tiles = width >> 4;
    height_in_tiles = height >> 4;
    full_count = 1;
    partial_count = 0;
    deter_nb_blocks(blocks, width, height, &full_count, &partial_count);
    total_count = full_count + partial_count;
    nb_blocks_plein = full_count;
    result = block_malloc(mem, full_count * 288 + partial_count * 544 + 32);
    full_block = result;
    partial_block = result + full_count * 288;
    tile_index = 0;

    for (tile_y = 0; tile_y < height_in_tiles; tile_y++) {
        for (tile_x = 0; tile_x < width_in_tiles; tile_x++) {
            if (tile_index < total_count) {
                source = blocks + 16 + tile_y * width + tile_x * 16;
                empty_count = deter_vide_plein_panach(source, width);
                if (empty_count == 0 || empty_count == 256) {
                    block_add[tile_index] = full_block - result;
                    row_flags = 0;
                    for (row = 0; row < 16; row++) {
                        nonzero_count = 0;
                        for (col = 0; col < 16; col++) {
                            full_block[row * 16 + col] = source[row * width + col];
                            if (source[row * width + col] != 0)
                                nonzero_count++;
                        }
                        if (nonzero_count == 0)
                            row_flags++;
                        else if (nonzero_count == 16)
                            row_flags += 2;
                        if (row < 15)
                            row_flags <<= 2;
                    }
                    *(u32 *)(full_block + 256) = row_flags;
                    full_block += 288;
                } else {
                    block_add[tile_index] = partial_block - result;
                    row_flags = 0;
                    for (row = 0; row < 16; row++) {
                        nonzero_count = 0;
                        for (col = 0; col < 16; col++) {
                            partial_block[row * 16 + col] = source[row * width + col];
                            if (source[row * width + col] == 0)
                                partial_block[256 + row * 16 + col] = 0xff;
                            else {
                                nonzero_count++;
                                partial_block[256 + row * 16 + col] = 0;
                            }
                        }
                        if (nonzero_count == 0)
                            row_flags++;
                        else if (nonzero_count == 16)
                            row_flags += 2;
                        if (row < 15)
                            row_flags <<= 2;
                    }
                    *(u32 *)(partial_block + 512) = row_flags;
                    partial_block += 544;
                }
                tile_index++;
            }
        }
    }
    return result;
}

void construct_MAP(mem_t *mem, big_map_t *map, u8 *map_blocks)
{
    s32 i;
    s32 texture_offset;
    u32 sample;
    u8 transparency;

    map->map = mp.map;
    map->map_blocks = map_blocks;
    map->tile_texture_offsets = (u8 **)block_malloc(mem, mp.length * sizeof(u8 *));
    map->display_map = (display_map_t *)block_malloc(mem, sizeof(display_map_t));
    for (i = 0; i < mp.length; i++) {
        texture_offset = block_add[map->map[i].texture_id];
        map->tile_texture_offsets[i] = map->map_blocks + texture_offset;
        if (texture_offset <= 288 * nb_blocks_plein)
            sample = *(u32 *)(map->tile_texture_offsets[i] + 256);
        else
            sample = *(u32 *)(map->tile_texture_offsets[i] + 512);
        if (sample == 0xaaaaaaaa)
            transparency = 1;
        else if (sample == 0x55555555)
            transparency = 0;
        else
            transparency = 2;
        map->map[i].transparency = transparency;
    }
}

void init_build_map(big_map_t *map)
{
    map->width = mp.width;
    map->height = mp.height;
    update_display_map(map);
}

void update_display_map(big_map_t *map)
{
    display_map_t *display_map;
    s32 y;
    s32 x;
    s32 v6;
    s32 v7;

    display_map = map->display_map;
    x = xmap + Bloc_lim_W1 - 8;
    y = ymap + Bloc_lim_H1;
    display_map->tile_index = map->width * (y >> 4) + (x >> 4);
    display_map->x = Bloc_lim_W1 - (x & 0xf);
    display_map->y = Bloc_lim_H1 - (y & 0xf);
    v6 = Bloc_lim_W2 - display_map->x;
    v7 = Bloc_lim_H2 - display_map->y;
    display_map->width_in_tiles = (v6 >> 4) + ((v6 & 0xf) != 0);
    display_map->height_in_tiles = (v7 >> 4) + ((v7 & 0xf) != 0);
    BufferNormalDeplt = 4 - (display_map->x & 3);
    DrawBufferNormal = DrawBufferNormalIni + BufferNormalDeplt;
    draw_buffer = DrawBufferNormalIni + BufferNormalDeplt;
}

void set_default_Bloc_clipping(void) {
    Bloc_lim_H1 = 0;
    Bloc_lim_H2 = 200;
    Bloc_lim_W1 = 4;
    Bloc_lim_W2 = 320;
}

void set_xymapini(void) {
    xmapmax = 16 * mp.width - Bloc_lim_W2;
    ymapmax = 16 * mp.height - Bloc_lim_H2;
    xmapmaxini = 16 * mp.width - 320;
    ymapmaxini = 16 * mp.height - 200;
    if (xmapmax < 0) {
        xmapmax = 0;
    }
    if (ymapmax < 0) {
        ymapmax = 0;
    }
    scroll_start_x = -Bloc_lim_W1 + 8;
    scroll_end_x = xmapmax + 8;
    scroll_start_y = -Bloc_lim_H1;
    scroll_end_y = ymapmax;
}

void set_xymap(void) {
    s32 v0 = xmap;
    if (xmap < -Bloc_lim_W1) {
        xmap = -Bloc_lim_W1;
    }
    if (xmap > xmapmax) {
        xmap = xmapmax;
    }
    if (ymap < -Bloc_lim_H1) {
        ymap = -Bloc_lim_H1;
    }
    if (ymap > ymapmax) {
        ymap = ymapmax;
    }
}

void set_whmap(s32 delta_w, s32 delta_h, big_map_t *map)
{
    s16 adjusted_xmapmax;

    Bloc_lim_H1 += delta_h;
    Bloc_lim_H2 -= delta_h;
    Bloc_lim_W1 += delta_w;
    Bloc_lim_W2 -= delta_w;

    if (Bloc_lim_H2 > 200)
        Bloc_lim_H2 = 200;
    if (Bloc_lim_H1 < 0)
        Bloc_lim_H1 = 0;
    if (Bloc_lim_W2 > 320)
        Bloc_lim_W2 = 320;
    if (Bloc_lim_W1 < 4)
        Bloc_lim_W1 = 4;

    if (Bloc_lim_W1 >= 4) {
        scroll_start_x -= delta_w;
        scroll_end_x += delta_w;
        if (scroll_start_x > 12)
            scroll_start_x = 12;
        if (scroll_end_x < xmapmaxini)
            scroll_end_x = xmapmaxini + 8;
        adjusted_xmapmax = xmapmax + delta_w;
        if (adjusted_xmapmax < xmapmaxini)
            adjusted_xmapmax = xmapmaxini;
        if (adjusted_xmapmax < xmap)
            adjusted_xmapmax = xmap;
        xmapmax = adjusted_xmapmax;
    }

    if (Bloc_lim_H1 >= 0) {
        scroll_start_y -= delta_h;
        scroll_end_y += delta_h;
        if (scroll_start_y > 0)
            scroll_start_y = 0;
        if (scroll_end_y < ymapmaxini)
            scroll_end_y = ymapmaxini;
        ymapmax += delta_h;
        if (ymapmax < ymapmaxini)
            ymapmax = ymapmaxini;
        if (ymapmax < ymap)
            ymapmax = ymap;
    }

    if (bossScrollStartX != 0) {
        bossScrollStartX = scroll_start_x;
        bossScrollEndX = scroll_end_x;
    }
    if (map->width <= 20 && Bloc_lim_W1 == 4) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
    set_xymap();
    if (scrollLocked && !((num_world == world_3_mountain && num_level == 10) ||
                          (num_world == world_2_music && num_level == 15))) {
        scroll_end_x = xmap;
        scroll_start_x = xmap;
    }
    if (xmapmax + Bloc_lim_W2 > 16 * map->width)
        xmapmax = 16 * map->width - Bloc_lim_W2;
    if (ymapmax + Bloc_lim_H2 > 16 * map->height)
        ymapmax = 16 * map->height - Bloc_lim_H2;
}

void recaleRaysurlesBords(void) {
    if (ray.x + ray.offset_bx <= xmap + Bloc_lim_W1 +16) {
        ray.x = xmap + Bloc_lim_W1 - ray.offset_bx + 16;
        ray.screen_x = ray.x - scroll_start_x + 8;
    }
    if (ray.x + ray.offset_bx >= xmap + Bloc_lim_W2 - 23) {
        ray.x = xmap + Bloc_lim_W2 - 32;
        ray.screen_x = ray.x - xmap + 8;
    }
}

void set_xywhmap(s16 xmin, s16 xmax, s16 ymin, s16 ymax) {
    s32 delta_W1 = xmin - Bloc_lim_W1;
    s32 delta_W2 = Bloc_lim_W2 - xmax;
    s32 delta_H1 = ymin - Bloc_lim_H1;
    s32 delta_H2 = Bloc_lim_H2 - ymax;

    Bloc_lim_H1 = ymin;
    Bloc_lim_H2 = ymax;
    Bloc_lim_W1 = xmin;
    Bloc_lim_W2 = xmax;

    if (ymax > 200) {
        Bloc_lim_H2 = 200;
    }
    if (ymin < 0) {
        Bloc_lim_H1 = 0;
    }
    if (xmax > 320) {
        Bloc_lim_W2 = 320;
    }
    if (xmin < 0) {
        Bloc_lim_W1 = 0;
    }

    if (Bloc_lim_W1 >= 4) {
        scroll_start_x -= delta_W1;
        scroll_end_x += delta_W2;
        if (scroll_start_x > 12) {
            scroll_start_x = 12;
        }
        if (scroll_end_x < xmapmaxini) {
            scroll_end_x = xmapmaxini + 8;
        }
        xmapmax += delta_W2;
        if (xmapmax < xmapmaxini) {
            xmapmax = xmapmaxini;
        }
        if (xmapmax < xmap) {
            xmapmax = xmap;
        }
    }

    if (Bloc_lim_H1 >= 0) {
        scroll_start_y -= delta_H1;
        scroll_end_y += delta_H2;
        if (scroll_start_y > 0) {
            scroll_start_y = 0;
        }
        if (scroll_end_y < ymapmaxini) {
            scroll_end_y = ymapmaxini;
        }
        ymapmax += delta_H2;
        if (ymapmax < ymapmaxini) {
            ymapmax = ymapmaxini;
        }
        if (ymapmax < ymap) {
            ymapmax = ymap;
        }
    }

    if (bossScrollStartX != 0) {
        bossScrollStartX = scroll_start_x;
        bossScrollEndX = scroll_end_x;
    }
    if (mp.width <= 20 && Bloc_lim_W1 == 4) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
    set_xymap();
    if (scrollLocked) {
        if (!((num_world == world_3_mountain && num_level == 10) || (num_world == world_2_music && num_level == 15))) {
            scroll_end_x = xmap;
            scroll_start_x = xmap;
        }
    }

    if (xmapmax + Bloc_lim_W2 > 16 * mp.width) {
        xmapmax = 16 * mp.width - Bloc_lim_W2;
    }
    if (ymapmax + Bloc_lim_H2 > 16 * mp.height) {
        ymapmax = 16 * mp.height - Bloc_lim_H2;
    }
    Bloc_lim_W1_Aff = Bloc_lim_W1;
    Bloc_lim_W2_Aff = Bloc_lim_W2;
    Bloc_lim_H1_Aff = Bloc_lim_H1;
    Bloc_lim_H2_Aff = Bloc_lim_H2;
    if (OptionGame) {
        recaleRaysurlesBords();
    }
}

void MaskScrollDiffSprites(mem_t *buffer)
{
    s32 i;
    s32 y;
    s32 x;
    s32 nb_sprites;
    sprite_t *sprite;
    u8 *mask;
    u8 *image;

    nb_sprites = ScrollDiffSprites->nb_sprites;
    for (i = 1; i < nb_sprites; i++) {
        sprite = ScrollDiffSprites->sprites + i;
        Scroll_Masque[i] = block_malloc(buffer, sprite->outer_width * sprite->outer_height);
        mask = Scroll_Masque[i];
        image = ScrollDiffSprites->img_buffer + sprite->offset_in_atlas;
        for (y = 0; y < sprite->outer_height; y++) {
            for (x = 0; x < sprite->outer_width; x++) {
                if (image[x] == 0)
                    mask[x] = 0xff;
                else
                    mask[x] = 0;
            }
            image += sprite->outer_width;
            mask += sprite->outer_width;
        }
    }
}
