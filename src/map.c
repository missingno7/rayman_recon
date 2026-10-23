/* map.c (1:4c4a0..1:4d230). Function names RAYVERSE; local names INVENTED.
   Worker sfin_map: candidate statuses and DOS evidence in NOTES.md. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"

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
extern u32 nb_total_blocks;
extern s32 BufferNormalDeplt;
extern u8 *DrawBufferNormal;
extern u8 *DrawBufferNormalIni;
extern u8 *draw_buffer;
extern obj_t *ScrollDiffSprites;
extern u8 *block_malloc(mem_t *mem, s32 size);
extern void Copy_Plan0Diff_To_Buf(u8 *source, u8 *dest, s32 width, s32 height, s32 draw_width);
void set_default_Bloc_clipping(void);
void set_xymapini(void);
void set_xymap(void);
void recaleRaysurlesBords(void);
void set_xywhmap(s16 xmin, s16 xmax, s16 ymin, s16 ymax);
void update_display_map(big_map_t *map);

u32 deter_vide_plein_panach(u8 *blocks, u16 width)
{
    u32 empty_count;
    u32 y;
    u8 *column;
    u8 *end;
    u8 empty_pixel;

    empty_count = 0;
    end = blocks + 16;
    y = 0;
    do {
        column = blocks;
        do {
            if ((empty_pixel = *(column + y * width)) == 0)
                empty_count++;
            column++;
        } while (column != end);
        y++;
    } while (y < 16);
    return empty_count;
}

void deter_nb_blocks(u8 *blocks, u16 width, u16 height, u32 *full_count, u32 *partial_count)
{
    u32 y, x, empty_count;
    for (y = 0; y < height; y += 16) {
        for (x = 0; x < width; x += 16) {
            if (x == 0 && y == 0)
                x += 16;
            empty_count = deter_vide_plein_panach(blocks + x + y * width, width);
            if (empty_count == 256)
                break;
            if (empty_count == 0)
                (*full_count)++;
            else
                (*partial_count)++;
        }
        if (empty_count == 256)
            break;
    }
}

u8 *Copy_Blocks(mem_t *mem, u8 *blocks, u16 width, u16 height)
{
    u32 width_in_tiles;
    u32 height_in_tiles;
    u32 partial_count;
    s32 tile_x;
    s32 tile_y;
    u32 tile_count;
    s32 row;
    u32 empty_count;
    u8 nonzero_count;
    u8 pixel;
    u32 row_flags;
    u8 *result;
    u8 *full_block;
    u8 *partial_block;
    u8 *dest;
    u8 *mask;
    u8 *source;
    u8 *end;
    u32 *flags;

    nb_blocks_plein = 1;
    tile_count = 0;
    partial_count = 0;
    width_in_tiles = width / 16;
    height_in_tiles = height / 16;
    deter_nb_blocks(blocks, width, height, &nb_blocks_plein, &partial_count);
    nb_total_blocks = nb_blocks_plein + partial_count;
    result = block_malloc(mem, nb_blocks_plein * 288 + partial_count * 544 + 32);
    full_block = result;
    partial_block = result + nb_blocks_plein * 288;
    for (tile_y = 0; tile_y < height_in_tiles; tile_y++) {
        if (tile_count > nb_total_blocks)
            break;
        for (tile_x = 0; tile_x < width_in_tiles; tile_x++) {
            tile_count++;
            if (tile_count > nb_total_blocks)
                break;
            empty_count = deter_vide_plein_panach(blocks + tile_x * 16 + tile_y * 16 * width, width);
            if (empty_count == 0 || empty_count == 256) {
                dest = full_block;
                full_block += 288;
                flags = (u32 *)(dest + 256);
            } else {
                dest = partial_block;
                partial_block += 544;
                mask = dest + 256;
                flags = (u32 *)(dest + 512);
            }
            block_add[tile_x + tile_y * width_in_tiles] = dest - result;
            end = blocks + tile_x * 16 + 16;
            row_flags = 0;
            for (row = 0; row < 16; row++) {
                nonzero_count = 0;
                source = blocks + tile_x * 16;
                do {
                    pixel = source[(row + tile_y * 16) * width];
                    *dest = pixel;
                    if (empty_count < 256 && empty_count != 0) {
                        if (pixel == 0)
                            *mask = 0xff;
                        else {
                            nonzero_count++;
                            *mask = 0;
                        }
                        mask++;
                    } else if (pixel != 0)
                        nonzero_count++;
                    source++;
                    dest++;
                } while (source != end);
                if (nonzero_count == 0)
                    row_flags++;
                else if (nonzero_count == 16)
                    row_flags += 2;
                if (row < 15)
                    row_flags <<= 2;
            }
            *flags = row_flags;
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

void init_build_map(big_map_t *map, u8 *map_blocks)  /* map_blocks: DOS caller passes MAP_BLOCKS in EDX; unused here */
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
    s16 clip_x;
    s16 clip_y;

    display_map = map->display_map;
    x = xmap + Bloc_lim_W1 - 8;
    y = ymap + Bloc_lim_H1;
    display_map->tile_index = map->width * (y >> 4) + (x >> 4);
    clip_x = Bloc_lim_W1;
    display_map->x = clip_x - (x & 0xf);
    clip_y = Bloc_lim_H1;
    display_map->y = clip_y - (y & 0xf);
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
    u16 clip_w;
    u16 clip_h;

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
        if (scroll_end_x < xmapmaxini + 8)
            scroll_end_x = xmapmaxini + 8;
        xmapmax += delta_w;
        if (xmapmax < xmapmaxini)
            xmapmax = xmapmaxini;
        if (xmapmax < xmap)
            xmapmax = xmap;
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

    set_xymap();
    clip_w = Bloc_lim_W2;
    clip_h = Bloc_lim_H2;
    if (xmapmax + clip_w > 16 * map->width)
        xmapmax = 16 * map->width - clip_w;
    if (ymapmax + clip_h > 16 * map->height)
        ymapmax = 16 * map->height - clip_h;
}

void recaleRaysurlesBords(void) {
    if (ray.x + ray.offset_bx <= xmap + Bloc_lim_W1 +16) {
        ray.x = Bloc_lim_W1 - ray.offset_bx + 16 + xmap;
        ray.screen_x = ray.x - scroll_start_x + 8;
    }
    if (ray.x + ray.offset_bx > xmap + Bloc_lim_W2 - 24) {
        ray.x = xmap + Bloc_lim_W2 - 24 - ray.offset_bx - 8;
        ray.screen_x = ray.x - xmap + 8;
    }
}

void set_xywhmap(s16 xmin, s16 xmax, s16 ymin, s16 ymax) {
    s32 may_lock;
    u16 clip_w;
    u16 clip_h;
    s16 delta_W1 = xmin - (s16)Bloc_lim_W1;
    s16 delta_W2 = (s16)Bloc_lim_W2 - xmax;
    s16 delta_H1 = ymin - (s16)Bloc_lim_H1;
    s16 delta_H2 = (s16)Bloc_lim_H2 - ymax;

    Bloc_lim_H1 = ymin;
    Bloc_lim_H2 = ymax;
    Bloc_lim_W1 = xmin;
    Bloc_lim_W2 = xmax;

    if (Bloc_lim_H2 > 200) {
        Bloc_lim_H2 = 200;
    }
    if (Bloc_lim_H1 < 0) {
        Bloc_lim_H1 = 0;
    }
    if (Bloc_lim_W2 > 320) {
        Bloc_lim_W2 = 320;
    }
    if (Bloc_lim_W1 < 4) {
        Bloc_lim_W1 = 4;
    }

    if (Bloc_lim_W1 >= 4) {
        scroll_start_x -= delta_W1;
        scroll_end_x += delta_W2;
        if (scroll_start_x > 12) {
            scroll_start_x = 12;
        }
        if (scroll_end_x < xmapmaxini + 8) {
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
        may_lock = num_world != world_3_mountain || num_level != 10;
        if (may_lock) {
            may_lock = num_world != world_2_music || num_level != 15;
            if (may_lock) {
                scroll_end_x = xmap;
                scroll_start_x = xmap;
            }
        }
    }
    clip_w = Bloc_lim_W2;
    clip_h = Bloc_lim_H2;
    if (xmapmax + clip_w > 16 * mp.width)
        xmapmax = 16 * mp.width - clip_w;
    if (ymapmax + clip_h > 16 * mp.height)
        ymapmax = 16 * mp.height - clip_h;
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
    u32 i;
    u32 y;
    u32 x;
    s16 nb_sprites;
    u8 *mask;
    u8 *image;
    u8 pixel;

    nb_sprites = ScrollDiffSprites->nb_sprites;
    for (i = 1; i < nb_sprites; i++) {
        sprite_t *sprite = ScrollDiffSprites->sprites + i;
        s16 height;
        s16 width = sprite->outer_width;
        height = sprite->outer_height;
        /* DOS 1:4d197..4d1a4: odd widths become zero at the signed-word conversion.
           Android MaskScrollDiffSprites confirms the zero allocation size for odd widths. */
        if (width & 1)
            width &= 0x10000;
        Scroll_Masque[i] = block_malloc(buffer, width * height);
        image = ScrollDiffSprites->img_buffer + sprite->offset_in_atlas;
        mask = Scroll_Masque[i];
        for (y = 0; y < height; y++) {
            for (x = 0; x < sprite->outer_width; x++) {
                if ((pixel = *image) == 0)
                    *mask = 0xff;
                else
                    *mask = 0;
                image++;
                mask++;
            }
        }
    }
}
