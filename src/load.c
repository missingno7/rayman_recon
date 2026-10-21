/* load.c (1:34d90..1:384c0: world/level/sprite/vignette/language loading). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_load. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include "game.h"


/* Temporary local PCX/archive views for the Rayverse-source baseline. */
typedef struct archive_header_local_t { s32 offset; s32 size; u8 xor_byte; u8 checksum_byte; u8 reserved[2]; } archive_header_local_t;
typedef struct pcx_header_local_t {
    u8 header_byte, software_version, encoding_method, bits_per_bitplane;
    u16 min_x, min_y, max_x, max_y, hor_dpi, ver_dpi;
    u8 ega_palette[48], reserved1;
    u8 n_color_planes;
    u16 bytes_per_line, palette_mode, source_hor_res, source_ver_res;
    u8 reserved2[54];
} pcx_header_local_t;
typedef struct image_local_t { u8 *memory; u32 memory_size; rgb_palette_t *pal; s32 width, height; u8 pal_needs_free, is_valid; } image_local_t;
#define archive_header_t archive_header_local_t
#define pcx_header_t pcx_header_local_t
#define image_t image_local_t

typedef struct x_texture_local_t { u8 pixels[256]; } x_texture_local_t;
#define x_texture_t x_texture_local_t
typedef struct level_local_t { obj_t *objects; s16 nb_objects; } level_local_t;
typedef struct level_obj_local_t { s16 *obj_ids; s16 nb_objects; } level_obj_local_t;
typedef struct display_map_local_t { s16 x, y; u8 width_in_tiles, height_in_tiles; s16 tile_index; } display_map_local_t;
typedef struct big_map_local_t { map_tile_t *map; u8 **tile_texture_offsets; display_map_local_t *display_map; u8 *map_blocks; u32 field_10; s16 field_14, width, height, field_1A, field_1C; } big_map_local_t;
enum { MODE_NORMAL = 0, MODE_X = 1 };
extern u8 GameModeVideo, FondAutorise, ToutSpriteAutorise, BiosCheckSum, VideoBiosCheckSum;
extern u8 *PLAN0, *PLAN0BIT, *PLAN1_BUFFER, *MAP_BLOCKS, *blocks_code;
extern s8 Plan0NumPcx[10];
extern s16 nb_des, nb_fix_des;
extern u8 nb_loaded_eta, nb_fix_eta, ScrollDiffOn, no_fnd, last_plan1_palette;
extern obj_t wldobj[100], ray, div_obj, bigray, raylittle, clockobj, mapobj[25];
extern obj_t *alpha, *alpha2, *ScrollDiffSprites;
extern u8 *alpha_image_atlas, *alpha2_image_atlas;
extern s16 alpha_sprite_count, alpha2_sprite_count;
extern eta_t **loaded_eta[100];
extern archive_header_t HeaderFilevignet[71], language_infos[5];
extern char *language_txt[300];
extern rgb_palette_t current_rvb, rvb[3];
extern map_data_t mp;
extern level_local_t level;
extern level_obj_local_t level_obj, level_alw;
extern big_map_local_t BIG_MAP;
extern x_texture_t *gros_patai_block;
extern u32 gros_patai_src[1200], TailleMainMemSprite, TailleMainMemLevel, TailleMainMemWorld, TailleMainMemFix;
extern u32 RaymanExeSize, RaymanExeCheckSum1, RaymanExeCheckSum2, RaymanExeCheckSum3;
extern s32 block_add[1200], nb_total_blocks, nb_blocks_plein;
extern u32 new_sprite_deform;
extern mem_t main_mem_sprite, main_mem_level, main_mem_world, main_mem_fix;
extern s16 *link_init;
extern u8 *DrawBufferNormal, *EffetBufferNormal;
extern s32 plan0bit_nb_bytes, plan0bit_length;
extern s16 plan0_width, plan0_height;
extern void stop_cd(void);
extern void *block_malloc(mem_t *mem, u32 size);
extern void *temp_malloc(u32 size);
extern void temp_free(void *ptr);
extern void fatal_error(void);
extern void FatalError();
extern void init_memory(mem_t *mem, u32 size);
extern void SpriteFixeOffset(mem_t *mem);
extern void detect_and_remove_invalid_link_cycles(void);
extern void MaskScrollDiffSprites(mem_t *mem);
extern void construct_MAP(mem_t *mem, big_map_local_t *map, u8 *blocks);
extern void calculate_link_positions(mem_t *mem);
extern void LoadPlan0InVignet(s32 resource_id);
extern u8 *LoadPcxInVignet(mem_t *buffer, s32 resource_id, u16 *width, u16 *height);
extern void LoadPcxPaletteInVignet(s32 resource_id, rgb_palette_t *palette);
extern u8 *allocate_PLAN0(mem_t *mem_world, u16 width, u16 height);
extern void PLAN0FND_to_bits_planes(u8 *buffer, u16 width, u16 height);

char* GetStringTxt(char* txt, char* out_buf);

void load_world(mem_t* mem_world, mem_t* mem_sprite, const char* filename) {
    s32 i;
    char *full_filename;
    FILE *fp;
    extern u16 path_has_changed;
    extern void sub_5BB78(void);
    extern void sub_5BB94(void);
    extern void sub_5BAE0(void);

    full_filename = (char *)temp_malloc(100);
    stop_cd();
    sub_5BB78();
    sub_5BB94();
    if (GameModeVideo == MODE_X) {
        PLAN1_BUFFER = block_malloc(mem_world, 71680);
    } else {
        PLAN1_BUFFER = 0;
    }
    for (i = 0; i < 10; ++i) {
        Plan0NumPcx[i] = -1;
    }


    if (path_has_changed != 0) {
        strcpy(full_filename, "..\\");
    } else {
        full_filename[0] = '\0';
    }
    strcat(full_filename, "PCMAP");
    strcat(full_filename, "\\");
    strcat(full_filename, filename);

    fp = fopen(full_filename, "rb");
    if (fp) {
        u16 background_width;
        u16 background_height;
        u8 plan0_num_pcx_count;
    s32 i;
    s32 des_index;





        // Header block
        fread(&background_width, 2, 1, fp);
        fread(&background_height, 2, 1, fp);
        if (GameModeVideo != MODE_NORMAL) {
            if (FondAutorise != 0) {
                PLAN0 = allocate_PLAN0(mem_world, background_width, background_height);
            }
        } else {
            // NOTE: is the background buffer wider to allow for differential scrolling?
            PLAN0 = allocate_PLAN0(mem_world, background_width * 2, background_height);
        }
        fread(&plan0_num_pcx_count, 1, 1, fp);
        fread(&BiosCheckSum, 1, 1, fp);
        fread(&VideoBiosCheckSum, 1, 1, fp);

        fread(Plan0NumPcx, plan0_num_pcx_count, 1, fp);
        for (i = 0; i < plan0_num_pcx_count; ++i) {
            Plan0NumPcx[i] ^= 0x15;
        }

        // Sprites block
        fread(&nb_des, 2, 1, fp);
        for (des_index = nb_fix_des; des_index < nb_des + nb_fix_des; ++des_index) {
            obj_t* cur_wldobj;
            u8 sprite_is_always_used;


            cur_wldobj = wldobj + des_index;


            fread(&sprite_is_always_used, 1, 1, fp);
            fread(&cur_wldobj->x, 4, 1, fp); // NOTE: this is read, but cur_wldobj->xpos is used instead

            if (sprite_is_always_used || ToutSpriteAutorise) {
                u8 img_buffer_checksum;
    s32 i;

                cur_wldobj->img_buffer = (u8*) block_malloc(mem_sprite, cur_wldobj->x);
                fread(cur_wldobj->img_buffer, cur_wldobj->x, 1, fp);


                fread(&img_buffer_checksum, 1, 1, fp);

                for (i = 0; i < cur_wldobj->x; ++i) {
                    img_buffer_checksum -= cur_wldobj->img_buffer[i];
                    cur_wldobj->img_buffer[i] ^= 0x8F;
                }

                if (img_buffer_checksum != 0) {
                    // error condition
                }

                fread(&cur_wldobj->nb_sprites, 2, 1, fp);
                cur_wldobj->sprites = (sprite_t*)block_malloc(mem_world, cur_wldobj->nb_sprites * sizeof(sprite_t));
                fread(cur_wldobj->sprites, cur_wldobj->nb_sprites * sizeof(sprite_t), 1, fp);

                fread(&cur_wldobj->anim_count, 1, 1, fp);
                if (cur_wldobj->anim_count != 0) {
    s32 anim_index;

                    cur_wldobj->animations = (anim_t*)block_malloc(mem_world, cur_wldobj->anim_count * sizeof(anim_t));
                    for (anim_index = 0; anim_index < cur_wldobj->anim_count; ++anim_index) {
                        s32 frames_ptr;
                        u16 layer_table_size;

                        anim_t* anim = cur_wldobj->animations + anim_index;
                        fread(&anim->layers_per_frame, 2, 1, fp);
                        fread(&anim->frames_count, 2, 1, fp);
                        frames_ptr = 0;
                        fread(&frames_ptr, 4, 1, fp); // ptr field, but might be -1

                        fread(&layer_table_size, 2, 1, fp);
                        anim->layers = (anim_layer_t*) block_malloc(mem_world, layer_table_size);
                        fread(anim->layers, layer_table_size, 1, fp);
                        if (frames_ptr != -1) {
                            ASSERT(sizeof(anim_frame_t) == 4);
                            anim->frames = (anim_frame_t*) block_malloc(mem_world, anim->frames_count * sizeof(anim_frame_t));
                            fread(anim->frames, anim->frames_count * sizeof(anim_frame_t), 1, fp);
                        }

                    }
                }
            } else {
                u8 img_buffer_checksum;

                fseek(fp, cur_wldobj->x, 1); // skip

                fread(&img_buffer_checksum, 1, 1, fp);
                fread(&cur_wldobj->nb_sprites, 2, 1, fp);
                fseek(fp, cur_wldobj->nb_sprites * sizeof(sprite_t), 1);
                fread(&cur_wldobj->anim_count, 1, 1, fp);
                if (cur_wldobj->anim_count != 0) {
    s32 anim_index;

                    cur_wldobj->animations = NULL;
                    for (anim_index = 0; anim_index < cur_wldobj->anim_count; ++anim_index) {
                        u16 layers_per_frame;
                        u16 frame_count;
                        s32 frames_ptr;
                        u16 layer_table_size;

                        layers_per_frame = 0;
                        fread(&layers_per_frame, 2, 1, fp);
                        frame_count = 0;
                        fread(&frame_count, 2, 1, fp);
                        frames_ptr = 0;
                        fread(&frames_ptr, 4, 1, fp); // ptr field, but might be -1

                        fread(&layer_table_size, 2, 1, fp);
                        fseek(fp, layer_table_size, 1);
                        if (frames_ptr != -1) {
                            fseek(fp, frame_count * sizeof(anim_frame_t), 1);
                        }
                    }
                }
            }
        }
        nb_des += nb_fix_des;

        // ETA block
        fread(&nb_loaded_eta, 1, 1, fp);
        for (i = nb_fix_eta; i < nb_loaded_eta + nb_fix_eta; ++i) {
            u8 num_eta_groups;
    s32 j;

            num_eta_groups = 0;
            fread(&num_eta_groups, 1, 1, fp);
            loaded_eta[i] = (eta_t**)block_malloc(mem_world, num_eta_groups * sizeof(eta_t*));
            for (j = 0; j < num_eta_groups; ++j) {
                u8 num_eta_entries;

                num_eta_entries = 0;
                fread(&num_eta_entries, 1, 1, fp);
                loaded_eta[i][j] = (eta_t*)block_malloc(mem_world, num_eta_entries * sizeof(eta_t));
                fread(loaded_eta[i][j], sizeof(eta_t) * num_eta_entries, 1, fp);
            }
        }
        nb_loaded_eta += nb_fix_eta;

        if (FondAutorise != 2) {
            // NOTE: I am not sure why exactly this is here. However, it seems that LoadPlan0InVignet should
            // ALWAYS fail the first time it is called because that erroneously sets PLAN0BIT = PLAN0 at the end, which
            // leaks away everything that was loaded into PLAN0, *UNLESS* they were the same buffer to begin with.
            // However, that will only be the case from the second time LoadPlan0InVignet is called.
            // Also, every time allocate_PLAN0() is called, the problem reappears, because then PLAN0 no longer equals
            // PLAN0BIT.

            // So, my guess is that they put this here as a workaround for the PLAN0BIT = PLAN0 bug, which they never
            // found the root cause for.

            // --> After the bug is fixed it should be safe to delete this?
            LoadPlan0InVignet(Plan0NumPcx[0]);
        }

        fclose(fp);

        //do_some_drm_check();
    } else {
        char *error_buffer;

        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "Can not open file %s (load_world).\n", full_filename);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    check_cd_for_save();
    sub_5BAE0();
    temp_free(full_filename);
}


void load_level(mem_t* mem_level, s32 world_id, const char* filename) {
    char world_names[6][13] = {
        {'j', 'u', 'n', 'g', 'l', 'e'},
        {'m', 'u', 's', 'i', 'c'},
        {'m', 'o', 'u', 'n', 't', 'a', 'i', 'n'},
        {'i', 'm', 'a', 'g', 'e'},
        {'c', 'a', 'v', 'e'},
        {'c', 'a', 'k', 'e'}
    };
    char *full_filename;
    FILE *fp;
    extern u16 path_has_changed;
    extern s16 num_world;
    extern void sub_5BB78(void);

    full_filename = (char *)temp_malloc(100);
    stop_cd();
    sub_5BB78();
    if (path_has_changed != 0) {
        strcpy(full_filename, "..\\");
    } else {
        full_filename[0] = '\0';
    }
    strcat(full_filename, "PCMAP");
    strcat(full_filename, "\\");
    strcat(full_filename, world_names[num_world - 1]);
    strcat(full_filename, "\\");
    strcat(full_filename, filename);

    fp = fopen(full_filename, "rb");
    if (fp) {
        s32 event_block_offset;
        s32 texture_block_offset;
    s32 i;
        u8 background_image_static;
        u8 background_image_scrolling;
        s32 ScrollDiffSprites_value;

        // Header


        fread(&event_block_offset, 4, 1, fp);
        fread(&texture_block_offset, 4, 1, fp);
        fread(&mp.width, 2, 1, fp);
        fread(&mp.height, 2, 1, fp);
        fread(rvb, sizeof(rvb), 1, fp);
        // Convert 6-bit colors into 8-bit colors by multiplying by 4
        for (i = 0; i < COUNT(rvb); ++i) {
            u8* colors;
    s32 j;

            colors = (u8*)&rvb[i].colors;
            for (j = 0; j < 256 * 3; ++j) {
                colors[j] <<= 2;
            }
        }
        fread(&last_plan1_palette, 1, 1, fp);
        current_rvb = rvb[last_plan1_palette];

        // Map
        mp.length = mp.width * mp.height;
        mp.map = (map_tile_t*) block_malloc(mem_level, mp.length * sizeof(map_tile_t));
        fread(mp.map, mp.length * sizeof(map_tile_t), 1, fp);



        ScrollDiffSprites_value = 0;
        fread(&background_image_static, 1, 1, fp);
        fread(&background_image_scrolling, 1, 1, fp);
        fread(&ScrollDiffSprites_value, 4, 1, fp);

        if (ScrollDiffSprites_value == -1) {
            ScrollDiffSprites = NULL;
        } else {
            ScrollDiffSprites = wldobj + ScrollDiffSprites_value;
            if (GameModeVideo == MODE_NORMAL && ScrollDiffOn) {
                MaskScrollDiffSprites(mem_level);
            }
        }

        if (GameModeVideo == MODE_X || !ScrollDiffOn) {
            no_fnd = background_image_static;
        } else if (GameModeVideo == MODE_NORMAL && ScrollDiffOn) {
            no_fnd = background_image_scrolling;
        }

        if (GameModeVideo == MODE_X) {
            u32 x_texture_count;
            u32 x_unk_table_size;
            u8 x_texture_checksum;
            u8* pos;
    s32 i;
            u8 x_unk_table_checksum;

            // X Mode Textures



            fread(&x_texture_count, 4, 1, fp);
            fread(&x_unk_table_size, 4, 1, fp);
            if (x_texture_count != 0) {
                gros_patai_block = (x_texture_t*) block_malloc(mem_level, x_texture_count * sizeof(x_texture_t));
                fread(gros_patai_block, x_texture_count * sizeof(x_texture_t), 1, fp);
            } else {
                gros_patai_block = NULL;
            }

            fread(&x_texture_checksum, 1, 1, fp);
            pos = (u8*) gros_patai_block;
            for (i = 0; i < x_texture_count * sizeof(x_texture_t); ++i) {
                x_texture_checksum -= *pos;
                *pos ^= 0x7D;
                ++pos;
            }
            if (x_texture_checksum != 0) {
                printf("%s : File integrity fault.\n", full_filename);
                fatal_error();
            }

            fread(gros_patai_src, sizeof(gros_patai_src), 1, fp);

            if (x_unk_table_size != 0) {
                blocks_code = (u8*) block_malloc(mem_level, x_unk_table_size);
                fread(blocks_code, x_unk_table_size, 1, fp);
            } else {
                blocks_code = NULL;
            }

            fread(&x_unk_table_checksum, 1, 1, fp);
            pos = blocks_code;
            for (i = 0; i < x_unk_table_size; ++i) {
                x_unk_table_checksum -= *pos;
                *pos ^= 0xF3;
                ++pos;
            }
            if (x_unk_table_checksum != 0) {
                printf("%s : File integrity fault.\n", full_filename);
                fatal_error();
            }

            fread(block_add, sizeof(block_add), 1, fp);
            // NOTE: the game uses a direct pointer here, but we can't do that on a 64-bit system
            // so we're using an offset instead (need to keep this in mind when we want to use it)
            // Also we need to check for the value -1
#if 0
            for (i = 0; i < 1200; ++i) {
                s32 value;

                value = block_add[i];
                if (value == -1) {
                    block_add[i] = 0;
                } else {

                    block_add[i] = blocks_code + value;
                }
            }
#endif
            fseek(fp, event_block_offset, 0);
        } else if (GameModeVideo == MODE_NORMAL) {
            u32 texture_data_size;
            u8 texture_data_checksum;
            u8* pos;
    s32 i;

            // Normal Mode Textures

            fseek(fp, texture_block_offset, 0);



            // offset table for the textures, based from the start of the tile texture arrays
            fread(block_add, sizeof(block_add), 1, fp);

            fread(&nb_total_blocks, 4, 1, fp);
            fread(&nb_blocks_plein, 4, 1, fp); // opaque blocks
            fread(&texture_data_size, 4, 1, fp);
            MAP_BLOCKS = (u8*) block_malloc(mem_level, texture_data_size);
            fread(MAP_BLOCKS, texture_data_size, 1, fp);
            fread(&texture_data_checksum, 1, 1, fp);
            pos = MAP_BLOCKS;
            for (i = 0; i < texture_data_size; ++i) {
                texture_data_checksum -= *pos;
                *pos ^= 0xFF;
                ++pos;
            }
            if (texture_data_checksum != 0) {
                printf("%s : File integrity fault (Block Normal).\n", full_filename);
                fatal_error();
            }
            construct_MAP(mem_level, &BIG_MAP, MAP_BLOCKS);
        } else {
            fatal_error();
        }

        // Events
        fread(&level.nb_objects, 2, 1, fp);
        level.objects = (obj_t*)block_malloc(mem_level, sizeof(obj_t) * level.nb_objects);
        level_obj.obj_ids = (s16*)block_malloc(mem_level, 2 * level.nb_objects);
        level_alw.obj_ids = (s16*)block_malloc(mem_level, 2 * level.nb_objects);
        memset(level.objects, 0, sizeof(obj_t) * level.nb_objects);

        link_init = (s16*)block_malloc(mem_level, 2 * level.nb_objects);
        fread(link_init, 2 * level.nb_objects, 1, fp);
        detect_and_remove_invalid_link_cycles(); // NOTE: added as a precaution

        // We can't directly read the obj_t because the pointer size is not 4 bytes on all systems.
        // So we read everything field by field
        // TODO: make it work on big-endian systems
        for (i = 0; i < level.nb_objects; ++i) {
            obj_t* obj;
            s16 sprites_value;
            s16 animations_value;
            s16 img_buffer_value;
            s16 eta_value;
            u8 flags_lo;
            u8 flags_hi;
            obj_flags_t flags;

            obj = level.objects + i;

            // The wldobj index is stored only in the lower 2 bytes of the pointer values for sprites, animations, img_buffer and eta




            fread(&sprites_value, 2, 1, fp);
            fseek(fp, 2, 1);
            fread(&animations_value, 2, 1, fp);
            fseek(fp, 2, 1);
            fread(&img_buffer_value, 2, 1, fp);
            fseek(fp, 2, 1);
            fread(&eta_value, 2, 1, fp);
            fseek(fp, 2, 1);

            fread(&obj->cmds, 4, 1, fp);
            fread(&obj->cmd_labels, 4, 1, fp);
            fread(&obj->cmd_contexts, 4, 1, fp);
            fread(&obj->field_1C, 4, 1, fp);
            fread(&obj->link_has_gendoor, 4, 1, fp);
            fread(&obj->is_active, 4, 1, fp);
            fread(&obj->x, 4, 1, fp);
            fread(&obj->y, 4, 1, fp);
            fread(&obj->active_flag, 4, 1, fp);
            fread(&obj->id, 2, 1, fp);
            fread(&obj->screen_x, 2, 1, fp);
            fread(&obj->screen_y, 2, 1, fp);
            fread(&obj->sub_speed, 2, 1, fp);
            fread(&obj->init_x, 2, 1, fp);
            fread(&obj->init_y, 2, 1, fp);
            fread(&obj->speed_x, 2, 1, fp);
            fread(&obj->speed_y, 2, 1, fp);
            fread(&obj->nb_sprites, 2, 1, fp);
            fread(&obj->cmd_offset, 2, 1, fp);
            fread(&obj->nb_cmd, 2, 1, fp);
            fread(&obj->follow_id, 2, 1, fp);
            fread(&obj->follow_y, 2, 1, fp);
            fread(&obj->follow_x, 2, 1, fp);
            fread(&obj->param, 2, 1, fp);
            fread(&obj->link, 2, 1, fp);
            fread(&obj->ray_dist, 2, 1, fp);
            fread(&obj->iframes_timer, 2, 1, fp);
            fread(&obj->test_block_index, 2, 1, fp);
            fread(&obj->scale, 2, 1, fp);
            fread(&obj->zdc, 2, 1, fp);
            fread(&obj->active_timer, 2, 1, fp);
            fread(&obj->type, 2, 1, fp);
            fread(&obj->btypes, 5, 1, fp);
            fread(&obj->unused_padding, 1, 1, fp);
            fread(&obj->offset_bx, 1, 1, fp);
            fread(&obj->offset_by, 1, 1, fp);
            fread(&obj->anim_index, 1, 1, fp);
            fread(&obj->anim_frame, 1, 1, fp);
            fread(&obj->sub_etat, 1, 1, fp);
            fread(&obj->main_etat, 1, 1, fp);
            fread(&obj->init_sub_etat, 1, 1, fp);
            fread(&obj->init_main_etat, 1, 1, fp);
            fread(&obj->cmd, 1, 1, fp);
            fread(&obj->gravity_value_1, 1, 1, fp);
            fread(&obj->gravity_value_2, 1, 1, fp);
            fread(&obj->change_anim_mode, 1, 1, fp);
            fread(&obj->offset_hy, 1, 1, fp);
            fread(&obj->follow_sprite, 1, 1, fp);
            fread(&obj->hit_points, 1, 1, fp);
            fread(&obj->init_hit_points, 1, 1, fp);
            fread(&obj->init_flag, 1, 1, fp);
            fread(&obj->hit_sprite, 1, 1, fp);
            fread(&obj->detect_zone, 1, 1, fp);
            fread(&obj->detect_zone_flag, 1, 1, fp);
            fread(&obj->cmd_context_depth, 1, 1, fp);
            fread(&obj->config, 1, 1, fp);
            fread(&obj->display_prio, 1, 1, fp);
            fread(&obj->timer, 1, 1, fp);
            fread(&obj->anim_count, 1, 1, fp);

            // The exact bit field layout isn't guaranteed by the C standard (make extra effort to deserialize properly).


            fread(&flags_lo, 1, 1, fp);
            fread(&flags_hi, 1, 1, fp);
            memset(&flags, 0, sizeof(flags));
            flags.hurtbyfist = flags_lo & 1u;
            flags.command_test = (flags_lo & 2u) != 0;
            flags.alive = (flags_lo & 4u) != 0;
            flags.flip_x = (flags_lo & 8u) != 0;
            flags.read_commands = (flags_lo & 0x10u) != 0;
            flags.follow_enabled = (flags_lo & 0x20u) != 0;
            flags.flag_0x40 = (flags_lo & 0x40u) != 0;
            flags.anim_changed = (flags_lo & 0x80u) != 0;
            flags.flag_0x100 = flags_hi & 1u;
            obj->flags = flags;

            fread(&obj->field_83, 1, 1, fp);

            // correct pointers
            obj->sprites = wldobj[sprites_value].sprites;
            obj->animations = wldobj[animations_value].animations;
            obj->img_buffer = wldobj[img_buffer_value].img_buffer;
            obj->eta = loaded_eta[eta_value];
        }

        for (i = 0; i < level.nb_objects; ++i) {
            obj_t* obj;
            u16 cmd_count;
            u16 cmd_label_count;

            obj = level.objects + i;

            cmd_count = 0;
            cmd_label_count = 0;
            fread(&cmd_count, 2, 1, fp);
            fread(&cmd_label_count, 2, 1, fp);

            if (cmd_count > 0) {
                obj->cmds = block_malloc(mem_level, cmd_count);
                fread(obj->cmds, cmd_count, 1, fp);
            } else {
                obj->cmds = NULL;
            }

            if (cmd_label_count > 0) {
                obj->cmd_labels = (s16*)block_malloc(mem_level, 2 * cmd_label_count);
                fread(obj->cmd_labels, 2 * cmd_label_count, 1, fp);
            } else {
                obj->cmd_labels = NULL;
            }

        }

        fclose(fp);

    } else {
        char *error_buffer;

        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "Can not open file %s (load_level).\n", full_filename);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    temp_free(full_filename);
}


void load_big_ray(mem_t* buffer) {
    FILE* fp;
    char *filename;
    char *error_buffer;
    extern void sub_5BB78(void);
    extern void sub_5BAE0(void);

    filename = (char *)temp_malloc(100);
    filename[0] = '\0';
    stop_cd();
    sub_5BB78();
    strcpy(filename, "PCMAP");
    strcat(filename, "\\");
    strcat(filename, "bray.dat");
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "Can not open file %s (load_big_ray).\n", filename);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    if (fp) {
        obj_t* sprite_group;
        u32 sprite_group_size;
        u8* image_atlas;
    u32 i;
        u8* sprite_buffer;

        sprite_group = wldobj + nb_des;
        sprite_group_size = 0;
        fread(&sprite_group_size, 4, 1, fp);
        sprite_group->x = (s32)sprite_group_size; // why is this saved in the xpos field??
        image_atlas = block_malloc(buffer, sprite_group_size);
        fread(image_atlas, sprite_group_size, 1, fp);
        for (i = 0; i < sprite_group_size; ++i) {
            image_atlas[i] ^= 0x8F;
        }
        sprite_group->img_buffer = image_atlas;

        fread(&sprite_group->nb_sprites, 2, 1, fp);
        sprite_buffer = block_malloc(buffer, sprite_group->nb_sprites * sizeof(sprite_t));
        fread(sprite_buffer, sizeof(sprite_t), sprite_group->nb_sprites, fp);
        sprite_group->sprites = (sprite_t*)sprite_buffer;

        fread(&sprite_group->anim_count, 1, 1, fp);
        sprite_group->animations = (anim_t*)block_malloc(buffer, sprite_group->anim_count * sizeof(anim_t));
        for (i = 0; i < sprite_group->anim_count; ++i) {
            s32 field_4;
            u16 layer_table_size;

            anim_t* anim_desc = sprite_group->animations + i;
            fread(&anim_desc->layers_per_frame, 2, 1, fp);
            fread(&anim_desc->frames_count, 2, 1, fp);
            field_4 = 0;
            fread(&field_4, 4, 1, fp); // ptr field, but might be -1
//			anim_desc->anim_layers_or_frames = (u8*)field_4;
            layer_table_size = 0;
            fread(&layer_table_size, 2, 1, fp);
            anim_desc->layers = (anim_layer_t*) block_malloc(buffer, layer_table_size);
            fread(anim_desc->layers, 1, layer_table_size, fp);
            if (field_4 != -1) {
                anim_desc->frames = (anim_frame_t*) block_malloc(buffer, anim_desc->frames_count * 4);
                fread(anim_desc->frames, 4, anim_desc->frames_count, fp);
            }
        }
        fread(&nb_loaded_eta, 1, 1, fp);
        for (i = 0; i < nb_loaded_eta; ++i) {
            u8 num_eta_groups;
    s32 j;

            num_eta_groups = 0;
            fread(&num_eta_groups, 1, 1, fp);
            loaded_eta[i] = (eta_t**) block_malloc(buffer, num_eta_groups * sizeof(eta_t*));
            for (j = 0; j < num_eta_groups; ++j) {
                u8 num_eta_entries;

                num_eta_entries = 0;
                fread(&num_eta_entries, 1, 1, fp);
                loaded_eta[i][j] = (eta_t*) block_malloc(buffer, num_eta_entries * sizeof(eta_t));
                fread(loaded_eta[i][j], sizeof(eta_t), num_eta_entries, fp);
            }
        }
        sprite_group->eta = loaded_eta[0];
        bigray = *sprite_group; // instantiates the bigray object?
        ++nb_des;

        fclose(fp);

    }
    temp_free(filename);
    sub_5BAE0();
}


void LOAD_ALL_FIX(void) {
    char *filename;
    char *error_buffer;
    FILE *fp;
    extern u16 path_has_changed;
    extern void sub_5BB78(void);
    extern void sub_5BAE0(void);

    stop_cd();
    filename = (char *)temp_malloc(100);
    init_memory(&main_mem_sprite, TailleMainMemSprite);
    init_memory(&main_mem_level, TailleMainMemLevel);
    init_memory(&main_mem_world, TailleMainMemWorld);
    init_memory(&main_mem_fix, TailleMainMemFix);
    if (path_has_changed != 0) {
        strcpy(filename, "..\\");
    } else {
        filename[0] = '\0';
    }
    strcat(filename, "PCMAP");
    strcat(filename, "\\");
    strcat(filename, "ALLFIX.DAT");
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "Can not open file %s (load_all_fix).\n", filename);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    if (fp) {
    s32 i;
        s32 des_index;
        obj_t* obj;

        sub_5BB78();

        fread(&nb_loaded_eta, 1, 1, fp);
        nb_fix_eta = nb_loaded_eta;
        for (i = 0; i < nb_loaded_eta; ++i) {
            u8 num_eta_groups;
    s32 j;

            num_eta_groups = 0;
            fread(&num_eta_groups, 1, 1, fp);
            loaded_eta[i] = (eta_t**)block_malloc(&main_mem_fix, num_eta_groups * sizeof(eta_t*));
            for (j = 0; j < num_eta_groups; ++j) {
                u8 num_eta_entries;

                num_eta_entries = 0;
                fread(&num_eta_entries, 1, 1, fp);
                loaded_eta[i][j] = (eta_t*)block_malloc(&main_mem_fix, num_eta_entries * sizeof(eta_t));
                fread(loaded_eta[i][j], sizeof(eta_t) * num_eta_entries, 1, fp);
            }
        }

        fread(&nb_des, 2, 1, fp);
        nb_fix_des = nb_des;
        des_index = 1;
        for (; des_index < nb_des; ++des_index) {
            obj_t* obj;
            s32 which_eta;
            u32 sprite_group_size;
            u8* image_atlas;
            u8 checksum_byte;
    u32 i;
            u8* sprite_buffer;


            obj = wldobj + des_index;

            which_eta = 0;
            fread(&which_eta, 4, 1, fp);
            if (which_eta == -1) {
                obj->eta = NULL;
            } else {
                obj->eta = loaded_eta[which_eta];
            }

            sprite_group_size = 0;
            fread(&sprite_group_size, 4, 1, fp);
            obj->x = (s32)sprite_group_size; // why is this saved in the xpos field??
            image_atlas = block_malloc(&main_mem_sprite, sprite_group_size);
            fread(image_atlas, sprite_group_size, 1, fp);

            checksum_byte = 0;
            fread(&checksum_byte, 1, 1, fp);

            for (i = 0; i < sprite_group_size; ++i) {
                checksum_byte -= image_atlas[i];
                image_atlas[i] ^= 0x8F;
            }
            if (checksum_byte != 0) {
                error_buffer = (char *)temp_malloc(200);
                sprintf(error_buffer, "%s : File integrity fault.\n", filename);
                FatalError(error_buffer);
                temp_free(error_buffer);
            }
            obj->img_buffer = image_atlas;

            fread(&obj->nb_sprites, 2, 1, fp);
            sprite_buffer = block_malloc(&main_mem_fix, obj->nb_sprites * sizeof(sprite_t));
            fread(sprite_buffer, sizeof(sprite_t) * obj->nb_sprites, 1, fp);
            obj->sprites = (sprite_t*)sprite_buffer;

            fread(&obj->anim_count, 1, 1, fp);
            if (obj->anim_count > 0) {
    s32 i;

                obj->animations = (anim_t*)block_malloc(&main_mem_fix, obj->anim_count * sizeof(anim_t));
                for (i = 0; i < obj->anim_count; ++i) {
                    s32 frames_ptr;
                    u16 layer_table_size;

                    anim_t* anim_desc = obj->animations + i;
                    fread(&anim_desc->layers_per_frame, 2, 1, fp);
                    fread(&anim_desc->frames_count, 2, 1, fp);
                    frames_ptr = 0;
                    fread(&frames_ptr, 4, 1, fp); // ptr field, but might be -1
                    layer_table_size = 0;
                    fread(&layer_table_size, 2, 1, fp);
                    anim_desc->layers = (anim_layer_t*) block_malloc(&main_mem_fix, layer_table_size);
                    fread(anim_desc->layers, layer_table_size, 1, fp);
                    if (frames_ptr != -1) {
                        anim_desc->frames = (anim_frame_t*) block_malloc(&main_mem_fix, anim_desc->frames_count * sizeof(anim_frame_t));
                        fread(anim_desc->frames, anim_desc->frames_count * sizeof(anim_frame_t), 1, fp);
                    }
                }
            }
        }
        fread(&des_index, 4, 1, fp);
        ray = wldobj[des_index];

        fread(&des_index, 4, 1, fp);
        alpha = wldobj + des_index;
        alpha_image_atlas = alpha->img_buffer;
        alpha_sprite_count = alpha->nb_sprites;

        fread(&des_index, 4, 1, fp);
        alpha2 = wldobj + des_index;
        alpha2_image_atlas = alpha2->img_buffer;
        alpha2_sprite_count = alpha2->nb_sprites;

        fread(&des_index, 4, 1, fp);
        obj = wldobj + des_index;
        raylittle.sprites = obj->sprites;
        raylittle.img_buffer = obj->img_buffer;
        raylittle.nb_sprites = obj->nb_sprites;

        fread(&des_index, 4, 1, fp);
        obj = wldobj + des_index;
        for (i = 0; i < 25; ++i) {
            obj_t* m;

            m = mapobj + i;
            m->sprites = obj->sprites;
            m->animations = obj->animations;
            m->img_buffer = obj->img_buffer;
            m->nb_sprites = obj->nb_sprites;
            m->anim_count = obj->anim_count;
            m->eta = obj->eta;
        }

        fread(&des_index, 4, 1, fp);
        clockobj = wldobj[des_index];

        fread(&des_index, 4, 1, fp);
        div_obj = wldobj[des_index];

        fclose(fp);
    }
    new_sprite_deform = (u32)block_malloc(&main_mem_fix, 0x4b0);
    SpriteFixeOffset(&main_mem_sprite);
    temp_free(filename);
    sub_5BAE0();
}


u8 *LoadPcxInVignet(mem_t *buffer, s32 resource_id, u16 *width, u16 *height)
{
    FILE *fp;
    archive_header_t *info;
    char *path;
    char *error_buffer;
    pcx_header_t header_storage;
    pcx_header_t *header;
    u8 *pcx_buf;
    u8 *pixels;
    u8 checksum;
    u8 value;
    s32 i;
    s32 pixel_count;
    s32 pcx_data_size;
    s32 run_length;
    s32 line_bytes;
    s32 source_pos;
    s32 x;
    s32 y;
    s32 tries;
    extern u16 path_has_changed;
    extern void sleep(u32 seconds);

    stop_cd();
    path = (char *)temp_malloc(100);
    if (path_has_changed != 0) {
        strcpy(path, "..\\");
    } else {
        path[0] = '\0';
    }
    strcat(path, "VIGNET.DAT");
    tries = 0;
    fp = fopen(path, "rb");
    while (fp == NULL && tries < 10) {
        fp = fopen(path, "rb");
        ++tries;
        sleep(1);
    }
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open (pcx).\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    info = HeaderFilevignet + resource_id;
    header = &header_storage;
    fseek(fp, info->offset, 0);
    fread(header, 1, 128, fp);
    checksum = info->checksum_byte;
    for (i = 0; i < 128; ++i) {
        value = ((u8 *)header)[i];
        checksum -= value;
        ((u8 *)header)[i] = (u8)(value ^ info->xor_byte);
    }
    *width = header->max_x - header->min_x + 1;
    *height = header->max_y - header->min_y + 1;
    pixel_count = (s32)*width * *height;
    pixels = (u8 *)block_malloc(buffer, pixel_count);
    line_bytes = header->bytes_per_line;
    pcx_data_size = info->size - 128;
    pcx_buf = (u8 *)temp_malloc(pcx_data_size);
    fread(pcx_buf, 1, pcx_data_size, fp);
    fclose(fp);
    for (i = 0; i < pcx_data_size; ++i) {
        value = pcx_buf[i];
        checksum -= value;
        pcx_buf[i] = (u8)(value ^ info->xor_byte);
    }
    if (checksum != 0) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : File integrity fault (%d).\n", path, resource_id);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    if (pcx_buf == NULL || pixels == NULL) {
        FatalError("No memory allocated for it (load_pcx) !!");
    } else {
        source_pos = 0;
        for (y = 0; y < *height; ++y) {
            x = 0;
            while (x < line_bytes && source_pos < pcx_data_size) {
                value = pcx_buf[source_pos++];
                if ((value & 0xc0) == 0xc0) {
                    run_length = value & 0x3f;
                    value = pcx_buf[source_pos++];
                } else {
                    run_length = 1;
                }
                while (run_length > 0) {
                    if (x < *width) {
                        pixels[y * *width + x] = value;
                    }
                    ++x;
                    --run_length;
                }
            }
        }
    }
    temp_free(pcx_buf);
    temp_free(path);
    return pixels;
}


void LoadPcxPaletteInVignet(s32 resource_id, rgb_palette_t *palette)
{
    FILE *fp;
    archive_header_t *info;
    char *path;
    char *error_buffer;
    u8 *colors;
    s32 i;
    s32 tries;
    extern u16 path_has_changed;
    extern void sleep(u32 seconds);

    stop_cd();
    path = (char *)temp_malloc(100);
    if (path_has_changed != 0) {
        strcpy(path, "..\\");
    } else {
        path[0] = '\0';
    }
    strcat(path, "VIGNET.DAT");
    tries = 0;
    fp = fopen(path, "rb");
    if (fp == NULL) {
        do {
            fp = fopen(path, "rb");
            ++tries;
            sleep(1);
        } while (fp == NULL && tries < 10);
    }
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open (palette).\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    info = HeaderFilevignet + resource_id;
    fseek(fp, info->offset + info->size - 0x300, 0);
    fread(palette, 0x300, 1, fp);
    fclose(fp);
    colors = (u8 *)palette;
    for (i = 0; i < 0x300; ++i) {
        colors[i] = (u8)((colors[i] ^ info->xor_byte) / 4);
    }
    temp_free(path);
}


void LoadPlan0InVignet(s32 resource_id)
{
    FILE *fp;
    archive_header_t *info;
    pcx_header_t pcx_header;
    char *path;
    char *compressed;
    char *error_buffer;
    u8 *destination;
    u8 checksum;
    u8 value;
    u16 width;
    u16 height;
    u16 line_bytes;
    s32 data_size;
    s32 tries;
    s32 i;
    s32 x;
    s32 y;
    s32 run;
    s32 output_width;
    s32 output_height;
    s32 stride;
    s32 source_pos;
    extern u16 path_has_changed;
    extern void sleep(u32 seconds);

    if (FondAutorise == 0) {
        return;
    }

    stop_cd();
    path = (char *)temp_malloc(100);
    if (path_has_changed != 0) {
        strcpy(path, "..\\");
    } else {
        path[0] = '\0';
    }
    strcat(path, "VIGNET.DAT");
    tries = 0;
    fp = fopen(path, "rb");
    if (fp == NULL) {
        do {
            fp = fopen(path, "rb");
            ++tries;
            sleep(1);
        } while (fp == NULL && tries < 10);
    }
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open (background).\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    } else {
        info = HeaderFilevignet + resource_id;
        data_size = info->size - 128;
        checksum = info->checksum_byte;
        fseek(fp, info->offset, 0);
        fread(&pcx_header, 1, 128, fp);
        for (i = 0; i < 128; ++i) {
            value = ((u8 *)&pcx_header)[i];
            checksum -= value;
            ((u8 *)&pcx_header)[i] = (u8)(value ^ info->xor_byte);
        }

        width = pcx_header.max_x - pcx_header.min_x + 1;
        height = pcx_header.max_y - pcx_header.min_y + 1;
        line_bytes = pcx_header.bytes_per_line;
        output_width = width;
        if (output_width > 320) {
            output_width = 320;
        }
        output_height = height;
        if (output_height > 200) {
            output_height = 200;
        }

        if (FondAutorise == 2) {
            compressed = (char *)PLAN0;
        } else {
            compressed = (char *)temp_malloc(data_size);
        }
        fread(compressed, 1, data_size, fp);
        fclose(fp);
        for (i = 0; i < data_size; ++i) {
            value = (u8)compressed[i];
            checksum -= value;
            compressed[i] = (char)(value ^ info->xor_byte);
        }

        destination = (GameModeVideo == MODE_NORMAL) ? PLAN0BIT : PLAN0;
        stride = (GameModeVideo == MODE_NORMAL) ? output_width * 2 : output_width;
        source_pos = 0;
        for (y = 0; y < height; ++y) {
            x = 0;
            while (x < line_bytes && source_pos < data_size) {
                value = (u8)compressed[source_pos++];
                if ((value & 0xc0) == 0xc0) {
                    run = value & 0x3f;
                    value = (u8)compressed[source_pos++];
                } else {
                    run = 1;
                }
                while (run > 0) {
                    if (x < output_width && y < output_height) {
                        destination[y * stride + x] = value;
                        if (GameModeVideo == MODE_NORMAL) {
                            destination[y * stride + x + output_width] = value;
                        }
                    }
                    ++x;
                    --run;
                }
            }
        }
        if (checksum != 0) {
            FatalError();
        }
        if (FondAutorise != 2 && data_size != 0) {
            temp_free(compressed);
        }
        if (GameModeVideo == MODE_NORMAL) {
            PLAN0 = PLAN0BIT;
            plan0_width = (s16)(output_width * 2);
            plan0_height = (s16)output_height;
        } else {
            PLAN0FND_to_bits_planes(PLAN0, (u16)output_width, (u16)output_height);
        }
    }
    temp_free(path);
}

extern u8 FondAutorise;
extern u8 *PLAN0BIT;
extern u8 *DrawBufferNormal;
extern u8 *EffetBufferNormal;
extern void *block_malloc(mem_t *mem, u32 size);
extern s32 plan0bit_nb_bytes;
extern s32 plan0bit_length;
extern s16 plan0_width;
extern s16 plan0_height;
extern void FatalError();

u8* allocate_PLAN0(mem_t* mem_world, u16 width, u16 height)
{
    u8* buffer1;
    u8* buffer2;

    if (FondAutorise == 2) {
        buffer1 = DrawBufferNormal;
        buffer2 = EffetBufferNormal;
    } else {
        buffer1 = (u8*)block_malloc(mem_world, width * height);
        buffer2 = (u8*)block_malloc(mem_world, 2 * width * height + 8 * width);
    }
    PLAN0BIT = buffer2;
    return buffer1;
}

void PLAN0FND_to_bits_planes(u8 *buffer, u16 width, u16 height)
{
    s32 x;
    s32 y;

    plan0_width = width;
    plan0bit_nb_bytes = width;
    plan0bit_length = width * height;
    plan0_height = height;
    if (PLAN0BIT == NULL) {
        FatalError();
    }

    for (y = 0; y < height; ++y) {
        for (x = 0; x < plan0bit_nb_bytes; ++x) {
            PLAN0BIT[(x & 3) * (plan0bit_length >> 1) + (x >> 2) + y * (plan0bit_nb_bytes >> 1)] = buffer[y * width + x];
            PLAN0BIT[(x & 3) * (plan0bit_length >> 1) + (x >> 2) + y * (plan0bit_nb_bytes >> 1) + (plan0bit_nb_bytes >> 2)] = buffer[y * width + x];
            if (y < 16) {
                PLAN0BIT[((x & 3) * (plan0bit_length >> 1) << 2) + (x >> 2) + y * (plan0bit_nb_bytes >> 1)] = buffer[y * width + x];
                PLAN0BIT[((x & 3) * (plan0bit_length >> 1) << 2) + (x >> 2) + y * (plan0bit_nb_bytes >> 1) + (plan0bit_nb_bytes >> 2)] = buffer[y * width + x];
            }
        }
    }

    plan0bit_nb_bytes >>= 1;
    plan0bit_length *= 2;
}

extern void stop_cd(void);
extern u8 *LoadPcxInVignet(mem_t *buffer, s32 resource_id, u16 *width, u16 *height);
extern void LoadPcxPaletteInVignet(s32 resource_id, rgb_palette_t *palette);
extern rgb_palette_t current_rvb;
extern rgb_palette_t rvb_pres;
extern u8 *PLAN2;
extern u8 *PLAN2BIT;
extern s32 plan2_width;
extern s16 plan2_height;
extern s32 plan2bit_length;
extern s32 plan2bit_nb_bytes;

void LoadPlan2InVignet(mem_t *buffer, s32 resource_id)
{
    s32 saved_resource_id = resource_id;
    u16 width;
    u16 height;

    stop_cd();
    PLAN2 = LoadPcxInVignet(buffer, saved_resource_id, &width, &height);
    LoadPcxPaletteInVignet(saved_resource_id, &current_rvb);
    memcpy(&rvb_pres, &current_rvb, sizeof(rgb_palette_t));
    plan2_width = width;
    plan2bit_nb_bytes = width;
    plan2bit_length = plan2bit_nb_bytes * height;
    plan2bit_nb_bytes >>= 2;
    plan2_height = height;
    PLAN2BIT = PLAN2;
}

char* GetStringTxt(char* txt, char* out_buf) {
    char* next;
    if (*txt != ',') {
        for (;;) {
            if (*txt == '*') {
                break;
            }
            next = txt + 1;
            if (*txt >= ' ') {
                *out_buf++ = *txt;
            }
            txt = next;
            if (*txt == ',') {
                break;
            }
        }
    }
    if (*txt == '*') {
        return NULL;
    } else {
        *out_buf = '\0';
        return txt + 1;
    }
}


void LoadLanguageTxt(mem_t* mem, s32 language_index) {
    FILE *lng_file;
    archive_header_t *lng_archive_header;
    char *path;
    char *error_buffer;
    char *temp_buffer;
    char *language_data;
    char *txt;
    char *next_txt;
    u8 *buffer_pos;
    u8 *buffer_end;
    u8 checksum_byte;
    u8 retry_checksum_byte;
    u8 c;
    s32 i;

    stop_cd();
    path = (char *)temp_malloc(100);
    memcpy(path, "RAY.LNG", 8);
    lng_file = fopen(path, "rb");
    if (lng_file == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open.\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }

    lng_archive_header = &language_infos[language_index];
    language_data = (char *)block_malloc(mem, lng_archive_header->size);
    temp_buffer = (char *)temp_malloc(400);
    if (language_data == NULL || temp_buffer == NULL) {
        FatalError("Memory error in LoadLanguageTxt");
    }

    fseek(lng_file, lng_archive_header->offset, 0);
    fread(language_data, 1, lng_archive_header->size, lng_file);
    buffer_pos = (u8 *)language_data;
    buffer_end = buffer_pos + lng_archive_header->size;
    checksum_byte = lng_archive_header->checksum_byte;
    for (; buffer_pos < buffer_end; ++buffer_pos) {
        c = *buffer_pos;
        checksum_byte -= c;
        c ^= lng_archive_header->xor_byte;
        *buffer_pos = c;
    }

    if (checksum_byte != 0) {
        retry_checksum_byte = lng_archive_header->checksum_byte;
        fseek(lng_file, lng_archive_header->offset, 0);
        for (i = 0; i < lng_archive_header->size; ++i) {
            fread(&c, 1, 1, lng_file);
            retry_checksum_byte -= c;
            c ^= lng_archive_header->xor_byte;
            language_data[i] = c;
        }
        if (retry_checksum_byte != 0) {
            error_buffer = (char *)temp_malloc(200);
            sprintf(error_buffer, "%s : File integrity fault.\n", path);
            FatalError(error_buffer);
            temp_free(error_buffer);
        }
    }

    fclose(lng_file);
    txt = language_data;
    next_txt = txt;
    for (i = 0; next_txt != NULL; ++i) {
        next_txt = GetStringTxt(txt, temp_buffer);
        if (next_txt != NULL) {
            language_txt[i] = txt;
            next_txt[-1] = '\0';
            txt = next_txt + 2;
        }
    }
    temp_free(temp_buffer);
    temp_free(path);
}


u8 *LoadPcxOptions(u8 *buffer, s32 resource_id, u16 *width, u16 *height)
{
    FILE *fp;
    archive_header_t *info;
    char *path;
    char *error_buffer;
    pcx_header_t header_storage;
    pcx_header_t *header;
    u8 *pcx_buf;
    u8 checksum;
    u8 value;
    s32 i;
    s32 pcx_data_size;
    s32 run_length;
    s32 line_bytes;
    s32 source_pos;
    s32 x;
    s32 y;
    extern u16 path_has_changed;

    stop_cd();
    path = (char *)temp_malloc(100);
    if (path_has_changed != 0) {
        strcpy(path, "..\\");
    } else {
        path[0] = '\0';
    }
    strcat(path, "VIGNET.DAT");
    fp = fopen(path, "rb");
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open.\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    info = HeaderFilevignet + resource_id;
    header = &header_storage;
    fseek(fp, info->offset, 0);
    fread(header, 1, 128, fp);
    checksum = info->checksum_byte;
    for (i = 0; i < 128; ++i) {
        value = ((u8 *)header)[i];
        checksum -= value;
        ((u8 *)header)[i] = (u8)(value ^ info->xor_byte);
    }
    *width = header->max_x - header->min_x + 1;
    *height = header->max_y - header->min_y + 1;
    line_bytes = header->bytes_per_line;
    pcx_data_size = info->size - 128;
    pcx_buf = (u8 *)temp_malloc(pcx_data_size);
    fread(pcx_buf, 1, pcx_data_size, fp);
    fclose(fp);
    for (i = 0; i < pcx_data_size; ++i) {
        value = pcx_buf[i];
        checksum -= value;
        pcx_buf[i] = (u8)(value ^ info->xor_byte);
    }
    if (checksum != 0) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : File integrity fault (%d).\n", path, resource_id);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    if (pcx_buf == NULL || buffer == NULL) {
        FatalError("No memory allocated (LoadPcxOptions).");
    } else {
        source_pos = 0;
        for (y = 0; y < *height; ++y) {
            x = 0;
            while (x < line_bytes && source_pos < pcx_data_size) {
                value = pcx_buf[source_pos++];
                if ((value & 0xc0) == 0xc0) {
                    run_length = value & 0x3f;
                    value = pcx_buf[source_pos++];
                } else {
                    run_length = 1;
                }
                while (run_length > 0) {
                    if (x < *width) {
                        buffer[y * *width + x] = value;
                    }
                    ++x;
                    --run_length;
                }
            }
        }
    }
    temp_free(pcx_buf);
    temp_free(path);
    return buffer;
}
