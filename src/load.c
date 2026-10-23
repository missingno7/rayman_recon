/* load object, DOS v1.21. Names RAYVERSE/ANDROID; g_load worker candidate.
   See NOTES.md for exact inline sites, STRICT results and remaining shape differences. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>

/* DOS archive/PCX layouts; field names inherited from the Rayverse baseline. */
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
extern font_t alpha, alpha2;
extern obj_t *ScrollDiffSprites;
extern u8 *alpha_image_atlas, *alpha2_image_atlas;
extern s16 alpha_sprite_count, alpha2_sprite_count;
extern eta_t **loaded_eta[100];
extern archive_header_t HeaderFilevignet[71], language_infos[5];
extern char *language_txt[];
extern rgb_palette_t current_rvb, rvb[3];
extern map_data_t mp;
extern level_local_t level;
extern level_obj_local_t level_obj, level_alw;
extern big_map_local_t BIG_MAP;
extern x_texture_t *gros_patai_block;
extern u32 gros_patai_src[1200], TailleMainMemSprite, TailleMainMemLevel, TailleMainMemWorld, TailleMainMemFix;
extern u32 RaymanExeSize, RaymanExeCheckSum1, RaymanExeCheckSum2, RaymanExeCheckSum3;
extern s32 block_add[1200];
extern u32 nb_total_blocks, nb_blocks_plein;
extern u32 new_sprite_deform;
extern mem_t main_mem_sprite, main_mem_level, main_mem_world, main_mem_fix;
extern s16 *link_init;
extern u8 *DrawBufferNormal, *EffetBufferNormal;
extern u32 plan0bit_nb_bytes, plan0bit_length;
extern u16 plan0_width, plan0_height;
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

/* INVENTED purpose names; only original non-C instructions are inline.
   Arithmetic and BIOS timer memory accesses remain ordinary C. */
void load_watch_address(u32 address);
/* ORIGINAL_INLINE 1:34e10 and 1:34e3f: MOV DR0,EAX (0f 23 c0). */
#pragma aux load_watch_address = "mov dr0,eax" parm [eax] modify exact [];
u32 load_watch_control(void);
/* ORIGINAL_INLINE 1:34e13 and 1:34e42: MOV EAX,DR7 (0f 21 f8). */
#pragma aux load_watch_control = "mov eax,dr7" value [eax] modify exact [eax];
void load_set_watch_control(u32 control);
/* ORIGINAL_INLINE 1:34e1b and 1:34e4a: MOV DR7,EAX (0f 23 f8). */
#pragma aux load_set_watch_control = "mov dr7,eax" parm [eax] modify exact [];
void load_enable_interrupts(void);
/* ORIGINAL_INLINE 1:34e1e and 1:34e2c: STI (fb). */
#pragma aux load_enable_interrupts = "sti" modify exact [];
void load_break_world_open(void);
/* ORIGINAL_INLINE 1:34f5d: INT3 (cc); no GPR changes. */
#pragma aux load_break_world_open = "int 3" modify exact [];
void load_break_world_backgrounds(void);
/* ORIGINAL_INLINE 1:35008: INT3 (cc); no GPR changes. */
#pragma aux load_break_world_backgrounds = "int 3" modify exact [];
void load_break_world_eta(void);
/* ORIGINAL_INLINE 1:35604: INT3 (cc); no GPR changes. */
#pragma aux load_break_world_eta = "int 3" modify exact [];
void load_break_level_open(void);
/* ORIGINAL_INLINE 1:357f1: INT3 (cc); no GPR changes. */
#pragma aux load_break_level_open = "int 3" modify exact [];
void load_break_level_map(void);
/* ORIGINAL_INLINE 1:35e4c: INT3 (cc); no GPR changes. */
#pragma aux load_break_level_map = "int 3" modify exact [];
void load_break_level_object(void);
/* ORIGINAL_INLINE 1:35f25: INT3 (cc); no GPR changes. */
#pragma aux load_break_level_object = "int 3" modify exact [];
void load_break_level_close(void);
/* ORIGINAL_INLINE 1:36054: INT3 (cc); no GPR changes. */
#pragma aux load_break_level_close = "int 3" modify exact [];
void load_break_big_ray_animation(void);
/* ORIGINAL_INLINE 1:36264: INT3 (cc); no GPR changes. */
#pragma aux load_break_big_ray_animation = "int 3" modify exact [];
void load_break_big_ray_eta(void);
/* ORIGINAL_INLINE 1:363c4: INT3 (cc); no GPR changes. */
#pragma aux load_break_big_ray_eta = "int 3" modify exact [];

void load_break_world_animation(void);
/* ORIGINAL_INLINE 1:35389: INT3 (cc); no GPR changes. */
#pragma aux load_break_world_animation = "int 3" modify exact [];

void load_break_fix_eta(void);
/* ORIGINAL_INLINE 1:3664a: INT3 (cc); no GPR changes. */
#pragma aux load_break_fix_eta = "int 3" modify exact [];

extern u16 path_has_changed;
extern void sub_5BB78(void);
extern void sub_5BB94(void);
extern void sub_5BAE0(void);
void load_world(mem_t* mem_world, mem_t* mem_sprite, const char* filename) {
  s32 i;
  u16 layer_table_size;
  u8 img_buffer_checksum;
  char *full_filename;
  FILE *fp;
  full_filename = (char *) temp_malloc(100);
  stop_cd();
  sub_5BB78();
  sub_5BB94();
  load_watch_address(0x46c);
  load_set_watch_control(load_watch_control() | 0x30003);
  {
    u8 tick;
    load_enable_interrupts();
    {
      u8 perm_value_3;
      u8 *perm_address_2;
      perm_address_2 = &perm_value_3;
      *perm_address_2 = *((volatile u8 *) 0x46c);
      tick = perm_value_3;
    }
    while (tick == (*((volatile u8 *) 0x46c)))
    {
    }

    load_enable_interrupts();
    tick = *((volatile u8 *) 0x46c);
    while (tick == (*((volatile u8 *) 0x46c)))
    {
    }

  }
  load_watch_address(0);
  load_set_watch_control(load_watch_control() & 0xde00);
  if (GameModeVideo == MODE_X)
  {
    PLAN1_BUFFER = block_malloc(mem_world, 71680);
  }
  else
  {
    PLAN1_BUFFER = 0;
  }
  i = 0;
  do
    Plan0NumPcx[i++] = -1;
  while (i < 10);
  if (path_has_changed != 0)
  {
    strcpy(full_filename, "..\\");
  }
  else
  {
    full_filename[0] = '\0';
  }
  strcat(full_filename, "PCMAP");
  strcat(full_filename, "\\");
  strcat(full_filename, filename);
  fp = fopen(full_filename, "rb");
  if (fp == 0)
  {
    char *error_buffer;
    error_buffer = (char *) temp_malloc(200);
    sprintf(error_buffer, "Can not open file %s (load_world).\n", full_filename);
    FatalError(error_buffer);
    temp_free(error_buffer);
  }
  else
  {
    u16 background_width;
    s32 des_index;
    u8 plan0_num_pcx_count;
    u16 background_height;
    load_break_world_open();
    fread(&background_width, 2, 1, fp);
    fread(&background_height, 2, 1, fp);
    if (GameModeVideo == MODE_NORMAL)
    {
      PLAN0 = allocate_PLAN0(mem_world, background_width * 2, background_height);
    }
    else
      if (FondAutorise != 0)
    {
      PLAN0 = allocate_PLAN0(mem_world, background_width, background_height);
    }
    fread(&plan0_num_pcx_count, 1, 1, fp);
    fread(&BiosCheckSum, 1, 1, fp);
    fread(&VideoBiosCheckSum, 1, 1, fp);
    load_break_world_backgrounds();
    for (i = 0; i < plan0_num_pcx_count; ++i)
    {
      s8 pcx_number;
      fread(&pcx_number, 1, 1, fp);
      pcx_number ^= 0x15;
      Plan0NumPcx[i] = pcx_number;
    }

    fread(&nb_des, 2, 1, fp);
    for (des_index = nb_fix_des; des_index < (nb_des + nb_fix_des); ++des_index)
    {
      u8 sprite_is_always_used;
      fread(&sprite_is_always_used, 1, 1, fp);
      fread(&wldobj[des_index].x, 4, 1, fp);
      if (sprite_is_always_used || ToutSpriteAutorise)
      {
        s32 image_offset;
        u32 retry_index;
        char *error_buffer;
        wldobj[des_index].img_buffer = (u8 *) block_malloc(mem_sprite, wldobj[des_index].x);
        image_offset = ftell(fp);
        fread(wldobj[des_index].img_buffer, 1, wldobj[des_index].x, fp);
        fread(&img_buffer_checksum, 1, 1, fp);
        for (i = 0; i < wldobj[des_index].x; ++i)
        {
          img_buffer_checksum -= wldobj[des_index].img_buffer[i];
          wldobj[des_index].img_buffer[i] ^= 0x8F;
        }

        if (img_buffer_checksum != 0)
        {
          fseek(fp, image_offset, 0);
          for (retry_index = 0; retry_index < ((u32) wldobj[des_index].x); retry_index++)
            fread(wldobj[des_index].img_buffer + retry_index, 1, 1, fp);

          fread(&img_buffer_checksum, 1, 1, fp);
          for (i = 0; i < wldobj[des_index].x; i++)
          {
            img_buffer_checksum -= wldobj[des_index].img_buffer[i];
            wldobj[des_index].img_buffer[i] ^= 0x8f;
          }

          if (img_buffer_checksum != 0)
          {
            error_buffer = (char *) temp_malloc(200);
            sprintf(error_buffer, "%s : File integrity fault.\n", full_filename);
            FatalError(error_buffer);
            temp_free(error_buffer);
          }
        }
        fread(&wldobj[des_index].nb_sprites, 2, 1, fp);
        wldobj[des_index].sprites = (sprite_t *) block_malloc(mem_world, wldobj[des_index].nb_sprites * (sizeof(sprite_t)));
        fread(wldobj[des_index].sprites, sizeof(sprite_t), wldobj[des_index].nb_sprites, fp);
        fread(&wldobj[des_index].anim_count, 1, 1, fp);
        if (wldobj[des_index].anim_count != 0)
        {
          wldobj[des_index].animations = (anim_t *) block_malloc(mem_world, wldobj[des_index].anim_count * (sizeof(anim_t)));
          for (i = 0; i < wldobj[des_index].anim_count; ++i)
          {
            fread(&wldobj[des_index].animations[i].layers_per_frame, 2, 1, fp);
            fread(&wldobj[des_index].animations[i].frames_count, 2, 1, fp);
            fread(&wldobj[des_index].animations[i].frames, 4, 1, fp);
            fread(&layer_table_size, 2, 1, fp);
            wldobj[des_index].animations[i].layers = (anim_layer_t *) block_malloc(mem_world, layer_table_size);
            fread(wldobj[des_index].animations[i].layers, 1, layer_table_size, fp);
            if (wldobj[des_index].animations[i].frames != ((anim_frame_t *) (-1)))
            {
              wldobj[des_index].animations[i].frames = (anim_frame_t *) block_malloc(mem_world, wldobj[des_index].animations[i].frames_count * (sizeof(anim_frame_t)));
              fread(wldobj[des_index].animations[i].frames, sizeof(anim_frame_t), wldobj[des_index].animations[i].frames_count, fp);
            }
            load_break_world_animation();
          }

        }
        else
        {
          wldobj[des_index].animations = wldobj[0].animations;
          wldobj[des_index].anim_count = wldobj[0].anim_count;
        }
      }
      else
      {
        wldobj[des_index].img_buffer = 0;
        fseek(fp, wldobj[des_index].x, 1);
        fread(&img_buffer_checksum, 1, 1, fp);
        fread(&wldobj[des_index].nb_sprites, 2, 1, fp);
        wldobj[des_index].sprites = 0;
        fseek(fp, wldobj[des_index].nb_sprites * (sizeof(sprite_t)), 1);
        fread(&wldobj[des_index].anim_count, 1, 1, fp);
        if (wldobj[des_index].anim_count != 0)
        {
          wldobj[des_index].animations = 0;
          for (i = 0; i < wldobj[des_index].anim_count; ++i)
          {
            u16 layers_per_frame;
            u16 frame_count;
            s32 frames_ptr;
            fread(&layers_per_frame, 2, 1, fp);
            fread(&frame_count, 2, 1, fp);
            fread(&frames_ptr, 4, 1, fp);
            fread(&layer_table_size, 2, 1, fp);
            fseek(fp, layer_table_size, 1);
            if (frames_ptr != (-1))
            {
              fseek(fp, frame_count * (sizeof(anim_frame_t)), 1);
            }
          }

        }
        else
        {
          wldobj[des_index].animations = wldobj[0].animations;
          wldobj[des_index].anim_count = wldobj[0].anim_count;
        }
      }
    }

    nb_des += nb_fix_des;
    fread(&nb_loaded_eta, 1, 1, fp);
    for (i = nb_fix_eta; i < (nb_loaded_eta + nb_fix_eta); ++i)
    {
      u8 num_eta_groups;
      s32 j;
      fread(&num_eta_groups, 1, 1, fp);
      loaded_eta[i] = (eta_t **) block_malloc(mem_world, num_eta_groups * (sizeof(eta_t *)));
      for (j = 0; j < num_eta_groups; ++j)
      {
        u8 num_eta_entries;
        fread(&num_eta_entries, 1, 1, fp);
        loaded_eta[i][j] = (eta_t *) block_malloc(mem_world, num_eta_entries * (sizeof(eta_t)));
        fread(loaded_eta[i][j], sizeof(eta_t), num_eta_entries, fp);
        load_break_world_eta();
      }

    }

    nb_loaded_eta += nb_fix_eta;
    if (FondAutorise != 2)
    {
      LoadPlan0InVignet(Plan0NumPcx[0]);
    }
    fclose(fp);
  }
  check_cd_for_save();
  sub_5BAE0();
  temp_free(full_filename);

}

extern u16 path_has_changed;
extern s16 num_world;
extern u8 current_pal_id;
extern void sub_5BB78(void);
extern void sub_5BAE0(void);
void load_level(mem_t *mem_level, s32 world_id, const char *filename)
{
    char world_names[6][13] = {
        {'j', 'u', 'n', 'g', 'l', 'e'},
        {'m', 'u', 's', 'i', 'c'},
        {'m', 'o', 'u', 'n', 't', 'a', 'i', 'n'},
        {'i', 'm', 'a', 'g', 'e'},
        {'c', 'a', 'v', 'e'},
        {'c', 'a', 'k', 'e'}
    };
    char *full_filename, *error_buffer;
    FILE *fp;
    s32 event_offset, texture_data_size;
    u32 x_texture_count, x_code_size;
    s32 texture_offset, image_offset;
    s32 i;
    u32 retry_index;
    u8 background_static, checksum, background_scroll;
    u16 cmd_count, label_count;






    full_filename = (char *)temp_malloc(100);
    stop_cd();
    sub_5BB78();
    if (path_has_changed != 0) strcpy(full_filename, "..\\");
    else full_filename[0] = '\0';
    strcat(full_filename, "PCMAP");
    strcat(full_filename, "\\");
    strcat(full_filename, world_names[num_world - 1]);
    strcat(full_filename, "\\");
    strcat(full_filename, filename);
    fp = fopen(full_filename, "rb");
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "Can not open file %s (load_level).\n", full_filename);
        FatalError(error_buffer);
        temp_free(error_buffer);
    } else {
        load_break_level_open();
        fread(&event_offset, 4, 1, fp);
        fread(&texture_offset, 4, 1, fp);
        fread(&mp.width, 2, 1, fp);
        fread(&mp.height, 2, 1, fp);
        fread(rvb, 1, sizeof(rvb), fp);
        fread(&last_plan1_palette, 1, 1, fp);
        current_pal_id = 0;
        memcpy(&current_rvb, &rvb[last_plan1_palette], sizeof(rgb_palette_t));
        mp.length = mp.width * mp.height;
        mp.map = (map_tile_t *)block_malloc(mem_level, mp.length * sizeof(map_tile_t));
        fread(mp.map, sizeof(map_tile_t), mp.length, fp);
        fread(&background_static, 1, 1, fp);
        fread(&background_scroll, 1, 1, fp);
        fread(&ScrollDiffSprites, 4, 1, fp);
        if (ScrollDiffSprites == (obj_t *)-1) ScrollDiffSprites = NULL;
        else {
            ScrollDiffSprites = &wldobj[(s32)ScrollDiffSprites];
            if (GameModeVideo == MODE_NORMAL && ScrollDiffOn) MaskScrollDiffSprites(mem_level);
        }
        if (GameModeVideo == MODE_X || !ScrollDiffOn) no_fnd = background_static;
        else if (GameModeVideo == MODE_NORMAL && ScrollDiffOn) no_fnd = background_scroll;
        if (GameModeVideo == MODE_X) {
            fread(&x_texture_count, 4, 1, fp);
            fread(&x_code_size, 4, 1, fp);
            if (x_texture_count != 0) gros_patai_block = (x_texture_t *)block_malloc(mem_level, x_texture_count * sizeof(x_texture_t));
            else gros_patai_block = NULL;
            if (x_code_size != 0) blocks_code = (u8 *)block_malloc(mem_level, x_code_size);
            else blocks_code = NULL;
            image_offset = ftell(fp);
            fread(gros_patai_block, 1, x_texture_count * sizeof(x_texture_t), fp);
            fread(&checksum, 1, 1, fp);
            for (retry_index = 0; retry_index < x_texture_count * sizeof(x_texture_t); retry_index++) {
                checksum -= ((u8 *)gros_patai_block)[retry_index];
                ((u8 *)gros_patai_block)[retry_index] ^= 0x7d;
            }
            if (checksum != 0) {
                fseek(fp, image_offset, 0);
                for (retry_index = 0; retry_index < x_texture_count * sizeof(x_texture_t); retry_index++) fread((u8 *)gros_patai_block + retry_index, 1, 1, fp);
                fread(&checksum, 1, 1, fp);
                for (retry_index = 0; retry_index < x_texture_count * sizeof(x_texture_t); retry_index++) {
                    checksum -= ((u8 *)gros_patai_block)[retry_index];
                    ((u8 *)gros_patai_block)[retry_index] ^= 0x7d;
                }
                if (checksum != 0) {
                    error_buffer = (char *)temp_malloc(200);
                    sprintf(error_buffer, "%s : File integrity fault.\n", full_filename);
                    FatalError(error_buffer);
                    temp_free(error_buffer);
                }
            }
            fread(gros_patai_src, 4, 1200, fp);
            for (i = 0; i < 1200; i++) gros_patai_src[i] = (u32)gros_patai_block + gros_patai_src[i];
            image_offset = ftell(fp);
            fread(blocks_code, 1, x_code_size, fp);
            fread(&checksum, 1, 1, fp);
            for (retry_index = 0; retry_index < x_code_size; retry_index++) {
                checksum -= blocks_code[retry_index];
                blocks_code[retry_index] ^= 0xf3;
            }
            if (checksum != 0) {
                fseek(fp, image_offset, 0);
                for (retry_index = 0; retry_index < x_code_size; retry_index++) fread(blocks_code + retry_index, 1, 1, fp);
                fread(&checksum, 1, 1, fp);
                for (retry_index = 0; retry_index < x_code_size; retry_index++) {
                    checksum -= blocks_code[retry_index];
                    blocks_code[retry_index] ^= 0xf3;
                }
                if (checksum != 0) {
                    error_buffer = (char *)temp_malloc(200);
                    sprintf(error_buffer, "%s : File integrity fault.\n", full_filename);
                    FatalError(error_buffer);
                    temp_free(error_buffer);
                }
            }
            fread(block_add, 4, 1200, fp);
            for (i = 0; 1200 > i; i++) {
                if (block_add[i] != -1) block_add[i] = (s32)blocks_code + block_add[i];
                else block_add[i] = 0;
            }
            fseek(fp, event_offset, 0);
        } else if (GameModeVideo == MODE_NORMAL) {
            fseek(fp, texture_offset, 0);
            fread(block_add, 4, 1200, fp);
            fread(&nb_total_blocks, 4, 1, fp);
            fread(&nb_blocks_plein, 4, 1, fp);
            fread(&texture_data_size, 4, 1, fp);
            MAP_BLOCKS = (u8 *)block_malloc(mem_level, texture_data_size);
            image_offset = ftell(fp);
            fread(MAP_BLOCKS, texture_data_size, 1, fp);
            fread(&checksum, 1, 1, fp);
            for (i = 0; i < texture_data_size; i++) {
                checksum -= MAP_BLOCKS[i];
                MAP_BLOCKS[i] ^= 0xff;
            }
            if (checksum != 0) {
                fseek(fp, image_offset, 0);
                for (i = 0; i < texture_data_size; i++) fread(MAP_BLOCKS + i, 1, 1, fp);
                fread(&checksum, 1, 1, fp);
                for (i = 0; i < texture_data_size; i++) {
                    checksum -= MAP_BLOCKS[i];
                    MAP_BLOCKS[i] ^= 0xff;
                }
                if (checksum != 0) {
                    error_buffer = (char *)temp_malloc(200);
                    sprintf(error_buffer, "%s : File integrity fault (Block Normal).\n", full_filename);
                    FatalError(error_buffer);
                    temp_free(error_buffer);
                }
            }
            construct_MAP(mem_level, &BIG_MAP, MAP_BLOCKS);
        }
        load_break_level_map();
        fread(&level.nb_objects, 2, 1, fp);
        level.objects = (obj_t *)block_malloc(mem_level, sizeof(obj_t) * level.nb_objects);
        level_obj.obj_ids = (s16 *)block_malloc(mem_level, 2 * level.nb_objects);
        level_alw.obj_ids = (s16 *)block_malloc(mem_level, 2 * level.nb_objects);
        (memset)(level.objects, 0, sizeof(obj_t) * level.nb_objects);
        link_init = (s16 *)block_malloc(mem_level, 2 * level.nb_objects + 1);
        fread(link_init, 2, level.nb_objects, fp);
        fread(level.objects, sizeof(obj_t), level.nb_objects, fp);
        for (i = 0; i < level.nb_objects; i++) {
            load_break_level_object();
            level.objects[i].sprites = wldobj[(s16)(s32)level.objects[i].sprites].sprites;
            level.objects[i].animations = wldobj[(s16)(s32)level.objects[i].animations].animations;
            level.objects[i].img_buffer = wldobj[(s16)(s32)level.objects[i].img_buffer].img_buffer;
            level.objects[i].eta = loaded_eta[(s16)(s32)level.objects[i].eta];
            fread(&cmd_count, 2, 1, fp);
            fread(&label_count, 2, 1, fp);
            if (cmd_count != 0) {
                level.objects[i].cmds = (u8 *)block_malloc(mem_level, cmd_count);
                fread(level.objects[i].cmds, 1, cmd_count, fp);
            } else level.objects[i].cmds = NULL;
            if (label_count != 0) {
                level.objects[i].cmd_labels = (s16 *)block_malloc(mem_level, label_count * 2);
                fread(level.objects[i].cmd_labels, 2, label_count, fp);
            } else level.objects[i].cmd_labels = NULL;
        }
        load_break_level_close();
        fclose(fp);
    }
    temp_free(full_filename);
    sub_5BAE0();
}

extern void sub_5BB78(void);
extern void sub_5BAE0(void);
void load_big_ray(mem_t *buffer)
{
  FILE *fp;
  char *filename;
  char *error_buffer;
  s32 index;
  s32 i;
  s32 j;
  s32 anim_index;
  u8 groups;
  u8 entries;
  u16 layer_table_size;
  filename = (char *) temp_malloc(100);
  filename[0] = '\0';
  stop_cd();
  sub_5BB78();
  strcpy(filename, "PCMAP");
  strcat(filename, "\\");
  strcat(filename, "bray.dat");
  fp = fopen(filename, "rb");
  if (fp == 0)
  {
    error_buffer = (char *) temp_malloc(200);
    sprintf(error_buffer, "Can not open file %s (load_big_ray).\n", filename);
    FatalError(error_buffer);
    temp_free(error_buffer);
  }
  else
  {
    {
      s32 perm_value_6;
      perm_value_6 = nb_des;
      index = perm_value_6;
    }
    fread(&wldobj[index].x, 4, 1, fp);
    wldobj[index].img_buffer = (u8 *) block_malloc(buffer, wldobj[index].x);
    fread(wldobj[index].img_buffer, 1, wldobj[index].x, fp);
    for (i = 0; (i < wldobj[index].x) != 0; i++)
      wldobj[index].img_buffer[i] ^= 0x8f;

    fread(&wldobj[index].nb_sprites, 2, 1, fp);
    wldobj[index].sprites = (sprite_t *) block_malloc(buffer, wldobj[index].nb_sprites * (sizeof(sprite_t)));
    fread(wldobj[index].sprites, sizeof(sprite_t), wldobj[index].nb_sprites, fp);
    fread(&wldobj[index].anim_count, 1, 1, fp);
    wldobj[index].animations = (anim_t *) block_malloc(buffer, wldobj[index].anim_count * (sizeof(anim_t)));
    for (anim_index = 0; anim_index < wldobj[index].anim_count; anim_index++)
    {
      load_break_big_ray_animation();
      fread(&wldobj[index].animations[anim_index].layers_per_frame, 2, 1, fp);
      fread(&wldobj[index].animations[anim_index].frames_count, 2, 1, fp);
      fread(&wldobj[index].animations[anim_index].frames, 4, 1, fp);
      fread(&layer_table_size, 2, 1, fp);
      wldobj[index].animations[anim_index].layers = (anim_layer_t *) block_malloc(buffer, layer_table_size);
      fread(wldobj[index].animations[anim_index].layers, 1, layer_table_size, fp);
      if (wldobj[index].animations[anim_index].frames != ((anim_frame_t *) (-1)))
      {
        wldobj[index].animations[anim_index].frames = (anim_frame_t *) block_malloc(buffer, wldobj[index].animations[anim_index].frames_count * (sizeof(anim_frame_t)));
        fread(wldobj[index].animations[anim_index].frames, sizeof(anim_frame_t), wldobj[index].animations[anim_index].frames_count, fp);
      }
    }

    fread(&nb_loaded_eta, 1, 1, fp);
    for (i = 0; i < nb_loaded_eta; i++)
    {
      fread(&groups, 1, 1, fp);
      loaded_eta[i] = (eta_t **) block_malloc(buffer, groups * (sizeof(eta_t *)));
      for (j = 0; j < groups; j++)
      {
        load_break_big_ray_eta();
        fread(&entries, 1, 1, fp);
        loaded_eta[i][j] = (eta_t *) block_malloc(buffer, entries * (sizeof(eta_t)));
        fread(loaded_eta[i][j], sizeof(eta_t), entries, fp);
      }

    }

    wldobj[nb_des].eta = loaded_eta[0];
    memcpy(&bigray, &wldobj[nb_des], sizeof(obj_t));
    nb_des++;
    fclose(fp);
  }
  temp_free(filename);
  sub_5BAE0();

}

extern u16 path_has_changed;
extern void sub_5BB78(void);
extern void sub_5BAE0(void);
void LOAD_ALL_FIX(void)
{
    char *filename, *error_buffer;
    FILE *fp;
    s32 index, i;
    s32 image_offset;
    u32 retry_index;




    stop_cd();
    filename = (char *)temp_malloc(100);
    init_memory(&main_mem_sprite, TailleMainMemSprite);
    init_memory(&main_mem_level, TailleMainMemLevel);
    init_memory(&main_mem_world, TailleMainMemWorld);
    init_memory(&main_mem_fix, TailleMainMemFix);
    if (path_has_changed != 0) strcpy(filename, "..\\");
    else filename[0] = '\0';
    strcat(filename, "PCMAP");
    strcat(filename, "\\");
    strcat(filename, "ALLFIX.DAT");
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "Can not open file %s (load_all_fix).\n", filename);
        FatalError(error_buffer);
        temp_free(error_buffer);
    } else {
        sub_5BB78();
        fread(&nb_loaded_eta, 1, 1, fp);
        nb_fix_eta = nb_loaded_eta;
        for (index = 0; index < nb_loaded_eta; index++) {
            u8 num_eta_groups;
            fread(&num_eta_groups, 1, 1, fp);
            loaded_eta[index] = (eta_t **)block_malloc(&main_mem_fix, num_eta_groups * sizeof(eta_t *));
            for (i = 0; i < num_eta_groups; i++) {
                u8 num_eta_entries;
                load_break_fix_eta();
                fread(&num_eta_entries, 1, 1, fp);
                loaded_eta[index][i] = (eta_t *)block_malloc(&main_mem_fix, num_eta_entries * sizeof(eta_t));
                fread(loaded_eta[index][i], sizeof(eta_t), num_eta_entries, fp);
            }
        }
        fread(&nb_des, 2, 1, fp);
        nb_fix_des = nb_des;
        for (index = 1; index < nb_des; index++) {
            u8 checksum_byte;
            fread(&wldobj[index].eta, 4, 1, fp);
            if (wldobj[index].eta == (eta_t **)-1) wldobj[index].eta = NULL;
            else wldobj[index].eta = loaded_eta[(s32)wldobj[index].eta];
            fread(&RaymanExeSize, 4, 1, fp);
            fread(&RaymanExeCheckSum1, 4, 1, fp);
            fread(&wldobj[index].x, 4, 1, fp);
            wldobj[index].img_buffer = (u8 *)block_malloc(&main_mem_sprite, wldobj[index].x);
            image_offset = ftell(fp);
            fread(wldobj[index].img_buffer, 1, wldobj[index].x, fp);
            fread(&checksum_byte, 1, 1, fp);
            for (i = 0; i < wldobj[index].x; i++) {
                checksum_byte -= wldobj[index].img_buffer[i];
                wldobj[index].img_buffer[i] ^= 0x8f;
            }
            if (checksum_byte != 0) {
                fseek(fp, image_offset, 0);
                for (retry_index = 0; retry_index < (u32)wldobj[index].x; retry_index++) {
                    fread(wldobj[index].img_buffer + retry_index, 1, 1, fp);
                }
                fread(&checksum_byte, 1, 1, fp);
                for (i = 0; i < wldobj[index].x; i++) {
                    checksum_byte -= wldobj[index].img_buffer[i];
                    wldobj[index].img_buffer[i] ^= 0x8f;
                }
                if (checksum_byte != 0) {
                    error_buffer = (char *)temp_malloc(200);
                    sprintf(error_buffer, "%s : File integrity fault.\n", filename);
                    FatalError(error_buffer);
                    temp_free(error_buffer);
                }
            }
            fread(&RaymanExeCheckSum2, 4, 1, fp);
            fread(&wldobj[index].nb_sprites, 2, 1, fp);
            wldobj[index].sprites = (sprite_t *)block_malloc(&main_mem_fix, wldobj[index].nb_sprites * sizeof(sprite_t));
            fread(wldobj[index].sprites, sizeof(sprite_t), wldobj[index].nb_sprites, fp);
            fread(&wldobj[index].anim_count, 1, 1, fp);
            if (wldobj[index].anim_count != 0) {
                wldobj[index].animations = (anim_t *)block_malloc(&main_mem_fix, wldobj[index].anim_count * sizeof(anim_t));
                for (i = 0; i < wldobj[index].anim_count; i++) {
                    u16 layer_table_size;
                    fread(&wldobj[index].animations[i].layers_per_frame, 2, 1, fp);
                    fread(&wldobj[index].animations[i].frames_count, 2, 1, fp);
                    fread(&wldobj[index].animations[i].frames, 4, 1, fp);
                    fread(&layer_table_size, 2, 1, fp);
                    wldobj[index].animations[i].layers = (anim_layer_t *)block_malloc(&main_mem_fix, layer_table_size);
                    fread(wldobj[index].animations[i].layers, 1, layer_table_size, fp);
                    if (wldobj[index].animations[i].frames != (anim_frame_t *)-1) {
                        wldobj[index].animations[i].frames = (anim_frame_t *)block_malloc(&main_mem_fix, wldobj[index].animations[i].frames_count * sizeof(anim_frame_t));
                        fread(wldobj[index].animations[i].frames, sizeof(anim_frame_t), wldobj[index].animations[i].frames_count, fp);
                    }
                }
            } else {
                wldobj[index].animations = NULL;
            }
        }
        fread(&RaymanExeCheckSum3, 4, 1, fp);
        fread(&index, 4, 1, fp);
        memcpy(&ray, &wldobj[index], sizeof(obj_t));
        fread(&index, 4, 1, fp);
        alpha.sprites = wldobj[index].sprites;
        alpha.img_buffer = wldobj[index].img_buffer;
        alpha.nb_sprites = wldobj[index].nb_sprites;
        fread(&index, 4, 1, fp);
        alpha2.sprites = wldobj[index].sprites;
        alpha2.img_buffer = wldobj[index].img_buffer;
        alpha2.nb_sprites = wldobj[index].nb_sprites;
        fread(&index, 4, 1, fp);
        raylittle.sprites = wldobj[index].sprites;
        raylittle.img_buffer = wldobj[index].img_buffer;
        raylittle.nb_sprites = wldobj[index].nb_sprites;
        fread(&index, 4, 1, fp);
        {
            u8 *atlas = wldobj[index].img_buffer;
            for (i = 0; i < 24; i++) {
                mapobj[i].sprites = wldobj[index].sprites;
                mapobj[i].animations = wldobj[index].animations;
                mapobj[i].nb_sprites = wldobj[index].nb_sprites;
                mapobj[i].anim_count = wldobj[index].anim_count;
                mapobj[i].img_buffer = atlas;
                mapobj[i].eta = wldobj[index].eta;
            }
        }
        fread(&index, 4, 1, fp);
        memcpy(&clockobj, &wldobj[index], sizeof(obj_t));
        fread(&index, 4, 1, fp);
        memcpy(&div_obj, &wldobj[index], sizeof(obj_t));
        fclose(fp);
    }
    new_sprite_deform = (u32)block_malloc(&main_mem_fix, 0x4b0);
    SpriteFixeOffset(&main_mem_sprite);
    temp_free(filename);
    sub_5BAE0();
}

extern u16 path_has_changed;
extern void sleep(u32 seconds);
u8 *LoadPcxInVignet(mem_t *buffer, s32 resource_id, u16 *width, u16 *height)
{
  u8 *pcx_buf;
  char *path;
  pcx_header_local_t header;
  FILE *fp;
  u32 y;
  u32 source_pos;
  u32 i;
  u8 retry_checksum;
  u8 *pos;
  u32 pcx_data_size;
  u16 line_bytes;
  u8 checksum;
  u8 *end;
  u32 x;
  char *error_buffer;
  s32 run_length;
  u8 value;
  u8 *pixels;
  stop_cd();
  path = (char *) temp_malloc(100);
  if (path_has_changed != 0)
  {
    strcpy(path, "..\\");
  }
  else
  {
    path[0] = '\0';
  }
  strcat(path, "VIGNET.DAT");
  i = 0;
  fp = fopen(path, "rb");
  for (; (fp == 0) && (i < 10);)
  {
    fp = fopen(path, "rb");
    i++;
    sleep(1);
  }

  if (fp == 0)
  {
    error_buffer = (char *) temp_malloc(200);
    sprintf(error_buffer, "%s : Can not open (pcx).\n", path);
    FatalError(error_buffer);
    temp_free(error_buffer);
  }
  checksum = HeaderFilevignet[resource_id].checksum_byte;
  pcx_data_size = HeaderFilevignet[resource_id].size - 128;
  fseek(fp, HeaderFilevignet[resource_id].offset, 0);
  fread(&header, 128, 1, fp);
  for (i = 0; i < 128; i++)
  {
    checksum -= ((u8 *) (&header))[i];
    ((u8 *) (&header))[i] ^= HeaderFilevignet[resource_id].xor_byte;
  }

  *width = (header.max_x - header.min_x) + 1;
  *height = (header.max_y - header.min_y) + 1;
  line_bytes = header.bytes_per_line;
  pixels = (u8 *) block_malloc(buffer, (*width) * (*height));
  pcx_buf = (u8 *) temp_malloc(pcx_data_size);
  fread(pcx_buf, 1, pcx_data_size, fp);
  end = pcx_buf + pcx_data_size;
  for (pos = pcx_buf; pos < end; pos++)
  {
    checksum -= *pos;
    *pos ^= HeaderFilevignet[resource_id].xor_byte;
  }

  if (checksum != 0)
  {
    retry_checksum = HeaderFilevignet[resource_id].checksum_byte;
    fseek(fp, HeaderFilevignet[resource_id].offset, 0);
    pos = (u8 *) (&header);
    end = pos + 16384;
    do
    {
      fread(pos, 1, 1, fp);
      pos += 128;
    }
    while (pos != end);
    for (i = 0; i <= 127; i++)
    {
      retry_checksum -= ((u8 *) (&header))[i];
      ((u8 *) (&header))[i] ^= HeaderFilevignet[resource_id].xor_byte;
    }

    *width = (header.max_x - header.min_x) + 1;
    *height = (header.max_y - header.min_y) + 1;
    line_bytes = header.bytes_per_line;
    pixels = (u8 *) block_malloc(buffer, (*width) * (*height));
    pcx_buf = (u8 *) temp_malloc(pcx_data_size);
    for (i = 0, pos = pcx_buf; i < pcx_data_size; i++, pos++)
      fread(pos, 1, 1, fp);

    end = pcx_buf + pcx_data_size;
    for (pos = pcx_buf; pos < end; pos++)
    {
      retry_checksum -= *pos;
      *pos ^= HeaderFilevignet[resource_id].xor_byte;
    }

    if ((retry_checksum != 0) != 0)
    {
      error_buffer = (char *) temp_malloc(200);
      sprintf(error_buffer, "%s : File integrity fault (%d).\n", path, resource_id);
      FatalError(error_buffer);
      temp_free(error_buffer);
    }
  }
  fclose(fp);
  if ((pixels != 0) && (pcx_buf != 0))
  {
    source_pos = 0;
    for (y = 0; y < (*height); y++)
    {
      x = 0;
      do
      {
        {
          u8 perm_value_11;
          perm_value_11 = pcx_buf[source_pos++];
          value = perm_value_11;
        }
        if (0xc0 == (value & 0xc0))
        {
          run_length = value & 0x3f;
          {
            u8 perm_value_26;
            perm_value_26 = pcx_buf[source_pos++];
            value = perm_value_26;
          }
          while ((--run_length) != (-1))
            pixels[(y * (*width)) + (x++)] = value;

        }
        else
        {
          pixels[(x++) + (y * (*width))] = value;
        }
      }
      while (x < line_bytes);
    }

    temp_free(pcx_buf);
  }
  else
  {
    FatalError("No memory allocated for it (load_pcx) !!");
  }
  temp_free(path);
  return pixels;

}

void LoadPcxPaletteInVignet(s32 resource_id, rgb_palette_t *palette)
{
    FILE *fp;
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
    for (; fp == NULL && tries < 10; ) {
        fp = fopen(path, "rb");
        ++tries;
        sleep(1);
    }
    if (fp == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open (palette).\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    fseek(fp, HeaderFilevignet[resource_id].offset + HeaderFilevignet[resource_id].size - 0x300, 0);
    fread(palette, 0x300, 1, fp);
    fclose(fp);
    colors = (u8 *)palette;
    for (i = 0; i < 768; ++i) {
        colors[i] ^= HeaderFilevignet[resource_id].xor_byte;
        colors[i] /= 4;
    }
    temp_free(path);
}

extern u8 P486;
extern u16 path_has_changed;
extern void sleep(u32 seconds);
void LoadPlan0InVignet(s32 resource_id)
{
  u32 header_index;
  u8 *destination;
  s32 run;
  u8 *end;
  u16 line_bytes;
  s32 x;
  u8 truncated;
  u8 *pos;
  u8 checksum;
  u8 *compressed;
  s32 tries;
  pcx_header_local_t header;
  u16 width;
  char *error_buffer;
  s32 data_size;
  s32 i;
  u32 source_pos;
  u8 value;
  FILE *fp;
  char *path;
  u16 height;
  s32 y;
  s32 crop;
  s32 output_y;
  if (FondAutorise != 0)
  {
    stop_cd();
    path = (char *) temp_malloc(100);
    truncated = 0;
    if (path_has_changed != 0)
    {
      strcpy(path, "..\\");
    }
    else
    {
      path[0] = '\0';
    }
    strcat(path, "VIGNET.DAT");
    tries = 0;
    fp = fopen(path, "rb");
    for (; (fp == 0) && (tries < 10);)
    {
      fp = fopen(path, "rb");
      tries++;
      sleep(1);
    }

    if (fp == 0)
    {
      error_buffer = (char *) temp_malloc(200);
      sprintf(error_buffer, "%s : Can not open (background).\n", path);
      FatalError(error_buffer);
      temp_free(error_buffer);
    }
    {
      u8 perm_value_4;
      perm_value_4 = HeaderFilevignet[resource_id].checksum_byte;
      checksum = perm_value_4;
    }
    data_size = HeaderFilevignet[resource_id].size - 128;
    fseek(fp, HeaderFilevignet[resource_id].offset, 0);
    fread(&header, 128, 1, fp);
    for (header_index = 0; header_index < 128; header_index++)
    {
      checksum -= ((u8 *) (&header))[header_index];
      ((u8 *) (&header))[header_index] ^= HeaderFilevignet[resource_id].xor_byte;
    }

    width = (header.max_x - header.min_x) + 1;
    height = (header.max_y - header.min_y) + 1;
    line_bytes = header.bytes_per_line;
    if (FondAutorise == 2)
    {
      compressed = EffetBufferNormal;
      destination = DrawBufferNormal;
      if (data_size > 128000)
      {
        data_size = 128000;
        truncated = 1;
      }
    }
    else
    {
      compressed = (u8 *) temp_malloc(data_size);
      destination = PLAN0;
    }
    fread(compressed, 1, data_size, fp);
    for (i = 0, pos = compressed; i < data_size; i++, pos++)
    {
      checksum -= *pos;
      *pos ^= HeaderFilevignet[resource_id].xor_byte;
    }

    if ((truncated == 0) && (checksum != 0))
    {
      fseek(fp, HeaderFilevignet[resource_id].offset, 0);
      pos = (u8 *) (&header);
      end = pos + 16384;
      do
      {
        fread(pos, 1, 1, fp);
        pos += 128;
      }
      while (pos != end);
      for (header_index = 0; header_index < 128; header_index++)
      {
        checksum -= ((u8 *) (&header))[header_index];
        ((u8 *) (&header))[header_index] ^= HeaderFilevignet[resource_id].xor_byte;
      }

      width = (header.max_x - header.min_x) + 1;
      height = (header.max_y - header.min_y) + 1;
      line_bytes = header.bytes_per_line;
      if (FondAutorise == 2)
      {
        compressed = EffetBufferNormal;
        destination = DrawBufferNormal;
        if (data_size > 128000)
        {
          data_size = 128000;
          truncated = 1;
        }
      }
      else
      {
        compressed = (u8 *) temp_malloc(data_size);
        destination = PLAN0;
      }
      for (i = 0, pos = compressed; i < data_size; i++, pos++)
        fread(pos, 1, 1, fp);

      for (i = 0, pos = compressed; i < data_size; i++, pos++)
      {
        checksum -= *pos;
        *pos ^= HeaderFilevignet[resource_id].xor_byte;
      }

      if ((truncated == 0) && (checksum != 0))
      {
        error_buffer = (char *) temp_malloc(200);
        sprintf(error_buffer, "%s : File integrity fault (%d).\n", path, resource_id);
        FatalError(error_buffer);
        temp_free(error_buffer);
      }
    }
    fclose(fp);
    temp_free(path);
    if ((destination != 0) && (compressed != 0))
    {
      if ((FondAutorise == 2) || (P486 == 1))
      {
        crop = 0;
        source_pos = 0;
        if (height > 200)
        {
          crop = height - 200;
          height = 200;
          if (truncated)
          {
            crop = 0;
          }
        }
        if (width > 320)
        {
          width = 320;
        }
        for (y = 0; y < (height + crop); y++)
        {
          output_y = y - crop;
          x = 0;
          do
          {
            value = compressed[source_pos++];
            if ((value & 0xc0) == 0xc0)
            {
              {
                s32 perm_value_28;
                s32 *perm_address_25;
                perm_address_25 = &perm_value_28;
                *perm_address_25 = value & 0x3f;
                run = perm_value_28;
              }
              value = compressed[source_pos++];
              while ((--run) != (-1))
              {
                if (((x < width) && (output_y >= 0)) && (output_y < 200))
                {
                  destination[(width * output_y) + x] = value;
                }
                x++;
              }

            }
            else
            {
              if (((x < width) && (output_y >= 0)) && (output_y < 200))
              {
                destination[(width * output_y) + x] = value;
              }
              x++;
            }
          }
          while (x < line_bytes);
        }

        if (FondAutorise != 2)
        {
          temp_free(compressed);
        }
      }
      else
      {
        source_pos = 0;
        for (y = 0; y < height; y++)
        {
          x = 0;
          do
          {
            {
              u8 perm_value_37;
              perm_value_37 = compressed[source_pos++];
              value = perm_value_37;
            }
            if ((value & 0xc0) == 0xc0)
            {
              run = value & 0x3f;
              {
                u8 perm_value_39;
                perm_value_39 = compressed[source_pos++];
                value = perm_value_39;
              }
              while ((--run) != (-1))
              {
                if (GameModeVideo == MODE_NORMAL)
                {
                  destination[((width * 2) * y) + x] = value;
                  destination[(((width * 2) * y) + width) + x] = value;
                }
                else
                {
                  destination[(width * y) + x] = value;
                }
                x++;
              }

            }
            else
            {
              if (GameModeVideo == MODE_NORMAL)
              {
                destination[((width * 2) * y) + x] = value;
                destination[(((width * 2) * y) + width) + x] = value;
              }
              else
              {
                destination[(width * y) + x] = value;
              }
              x++;
            }
          }
          while (x < line_bytes);
        }

        temp_free(compressed);
      }
    }
    else
    {
      FatalError("No memory allocated (load_plan0).");
    }
    if (GameModeVideo == MODE_NORMAL)
    {
      PLAN0 = PLAN0BIT;
      plan0_width = width * 2;
      plan0_height = height;
    }
    else
    {
      PLAN0FND_to_bits_planes(PLAN0, width, height);
    }
  }

}

extern u8 FondAutorise;
extern u8 *PLAN0BIT;
extern u8 *DrawBufferNormal;
extern u8 *EffetBufferNormal;
extern void *block_malloc(mem_t *mem, u32 size);
extern u32 plan0bit_nb_bytes;
extern u32 plan0bit_length;
extern u16 plan0_width;
extern u16 plan0_height;
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
  u32 x;
  u32 y;
  u8 *source;
  u32 offset;
  plan0_width = width;
  plan0bit_nb_bytes = plan0_width;
  plan0bit_length = plan0bit_nb_bytes * height;
  plan0_height = height;
  source = buffer;
  buffer = PLAN0BIT;
  if (buffer == 0)
  {
    FatalError("No memory allocated (PLAN0FND_to_bits_planes).");
  }
  for (y = 0; y < height; ++y)
    for (x = 0; x < plan0bit_nb_bytes; ++x)
  {
    offset = ((x & 3) * (plan0bit_length >> 1)) + (x >> 2);
    offset += y * (plan0bit_nb_bytes >> 1);
    {
      u8 pixel;
      pixel = source[(y * width) + x];
      buffer[offset] = pixel;
    }
    buffer[offset + (plan0bit_nb_bytes >> 2)] = source[(y * width) + x];
    if (y < 16)
    {
      {
        u32 perm_value_2;
        perm_value_2 = (((plan0bit_length >> 1) << 2) + (x >> 2)) + (y * (plan0bit_nb_bytes >> 1));
        offset = perm_value_2;
      }
      {
        u8 pixel;
        pixel = source[(y * width) + x];
        buffer[offset] = pixel;
      }
      buffer[offset + (plan0bit_nb_bytes >> 2)] = source[(y * width) + x];
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
extern u32 plan2_width;
extern s16 plan2_height;
extern u32 plan2bit_length;
extern u32 plan2bit_nb_bytes;

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
    plan2_height = height;
    plan2bit_nb_bytes /= 4;
    PLAN2BIT = PLAN2;
}

char* GetStringTxt(char* txt, char* out_buf) {
    while (*txt != ',' && *txt != '*') {
        if (*txt >= ' ') {
            *out_buf++ = *txt++;
        } else {
            txt++;
        }
    }
    if (*txt == '*') {
        return NULL;
    } else {
        *out_buf = '\0';
        return txt + 1;
    }
}

void LoadLanguageTxt(mem_t *mem, s32 language_index)
{
    char *language_data;
    u16 str_index;
    u8 checksum;
    char *parse_buffer;
    char *temp_buffer;
    u8 retry_checksum;
    char *error_buffer;
    u8 *pos;
    char *txt;
    u32 i;
    char *path;
    char *next_txt;
    u8 *end;
    FILE *lng_file;
    u32 size;

    stop_cd();
    path = (char *)temp_malloc(100);
    strcpy(path, "RAY.LNG");
    lng_file = fopen(path, "rb");
    if (lng_file == NULL) {
        error_buffer = (char *)temp_malloc(200);
        sprintf(error_buffer, "%s : Can not open.\n", path);
        FatalError(error_buffer);
        temp_free(error_buffer);
    }
    checksum = language_infos[language_index].checksum_byte;
    size = language_infos[language_index].size;
    language_data = (char *)block_malloc(mem, size);
    temp_buffer = (char *)temp_malloc(400);
    parse_buffer = temp_buffer;
    if (language_data == NULL || temp_buffer == NULL) {
        FatalError("Memory error in LoadLanguageTxt");
    }
    fseek(lng_file, language_infos[language_index].offset, 0);
    end = (u8 *)language_data + size;
    fread(language_data, 1, size, lng_file);
    for (pos = (u8 *)language_data; pos < end; pos++) {
        checksum -= *pos;
        *pos ^= language_infos[language_index].xor_byte;
    }
    if (checksum != 0) {
        retry_checksum = language_infos[language_index].checksum_byte;
        fseek(lng_file, language_infos[language_index].offset, 0);
        for (i = 0, pos = (u8 *)language_data; i < size; i++, pos++) {
            fread(pos, 1, 1, lng_file);
        }
        for (i = 0; i < size; i++) {
            retry_checksum -= language_data[i];
            language_data[i] ^= language_infos[language_index].xor_byte;
        }
        if (retry_checksum != 0) {
            error_buffer = (char *)temp_malloc(200);
            sprintf(error_buffer, "%s : File integrity fault.\n", path);
            FatalError(error_buffer);
            temp_free(error_buffer);
        }
    }
    fclose(lng_file);
    txt = language_data;
    next_txt = language_data;
    str_index = 0;
    for (;;) {
        next_txt = GetStringTxt(next_txt, parse_buffer);
        if (next_txt == NULL) break;
        {
            language_txt[str_index++] = txt;
            next_txt[-1] = '\0';
            txt = next_txt + 2;
        }
    }
    temp_free(temp_buffer);
    temp_free(path);
}

extern u16 path_has_changed;
extern void sleep(u32 seconds);
u8 *LoadPcxOptions(u8 *buffer, s32 resource_id, u16 *width, u16 *height)
{
  u32 y;
  u8 checksum;
  s32 run_length;
  u8 *pcx_buf;
  u32 source_pos;
  char *path;
  u8 value;
  u8 *pos;
  u32 x;
  u8 *end;
  FILE *fp;
  char *error_buffer;
  pcx_header_local_t header;
  u16 line_bytes;
  u32 pcx_data_size;
  u32 i;
  u8 retry_checksum;
  stop_cd();
  path = (char *) temp_malloc(100);
  if (path_has_changed != 0)
  {
    strcpy(path, "..\\");
  }
  else
  {
    path[0] = '\0';
  }
  strcat(path, "VIGNET.DAT");
  fp = fopen(path, "rb");
  if (fp == 0)
  {
    error_buffer = (char *) temp_malloc(200);
    sprintf(error_buffer, "%s : Can not open.\n", path);
    FatalError(error_buffer);
    temp_free(error_buffer);
  }
  {
    u8 perm_value_0;
    perm_value_0 = HeaderFilevignet[resource_id].checksum_byte;
    checksum = perm_value_0;
  }
  pcx_data_size = HeaderFilevignet[resource_id].size - 128;
  fseek(fp, HeaderFilevignet[resource_id].offset, 0);
  fread(&header, 128, 1, fp);
  for (i = 0; i < 128; i++)
  {
    checksum -= ((u8 *) (&header))[i];
    ((u8 *) (&header))[i] ^= HeaderFilevignet[resource_id].xor_byte;
  }

  *width = (header.max_x - header.min_x) + 1;
  *height = (header.max_y - header.min_y) + 1;
  line_bytes = header.bytes_per_line;
  pcx_buf = (u8 *) temp_malloc(pcx_data_size);
  fread(pcx_buf, 1, pcx_data_size, fp);
  end = pcx_buf + pcx_data_size;
  for (pos = pcx_buf; pos < end; pos++)
  {
    checksum -= *pos;
    *pos ^= HeaderFilevignet[resource_id].xor_byte;
  }

  if (checksum != 0)
  {
    {
      u8 perm_value_10;
      {
        u8 perm_value_20;
        u8 *perm_address_5;
        perm_address_5 = &perm_value_20;
        *perm_address_5 = HeaderFilevignet[resource_id].checksum_byte;
        perm_value_10 = perm_value_20;
      }
      retry_checksum = perm_value_10;
    }
    fseek(fp, HeaderFilevignet[resource_id].offset, 0);
    pos = (u8 *) (&header);
    end = pos + 16384;
    do
    {
      fread(pos, 1, 1, fp);
      pos += 128;
    }
    while (pos != end);
    for (i = 0; i < 128; i++)
    {
      retry_checksum -= ((u8 *) (&header))[i];
      ((u8 *) (&header))[i] ^= HeaderFilevignet[resource_id].xor_byte;
    }

    *width = (header.max_x - header.min_x) + 1;
    *height = (header.max_y - header.min_y) + 1;
    line_bytes = header.bytes_per_line;
    pcx_buf = (u8 *) temp_malloc(pcx_data_size);
    for (i = 0, pos = pcx_buf; (i < pcx_data_size) != 0; i++, pos++)
      fread(pos, 1, 1, fp);

    end = pcx_buf + pcx_data_size;
    for (pos = pcx_buf; pos < end; pos++)
    {
      retry_checksum -= *pos;
      *pos ^= HeaderFilevignet[resource_id].xor_byte;
    }

    if (0 != retry_checksum)
    {
      error_buffer = (char *) temp_malloc(200);
      sprintf(error_buffer, "%s : File integrity fault (%d).\n", path, resource_id);
      FatalError(error_buffer);
      temp_free(error_buffer);
    }
  }
  fclose(fp);
  if (!((buffer != 0) && (pcx_buf != 0)))
  {
    FatalError("No memory allocated for it (load_pcx) !!");
  }
  else
  {
    source_pos = 0;
    for (y = 0; y < (*height); y++)
    {
      x = 0;
      do
      {
        {
          u8 perm_value_15;
          perm_value_15 = pcx_buf[source_pos++];
          value = perm_value_15;
        }
        if ((value & 0xc0) == 0xc0)
        {
          run_length = 0x3f & value;
          {
            u8 perm_value_29;
            perm_value_29 = pcx_buf[source_pos++];
            value = perm_value_29;
          }
          while ((--run_length) != (-1))
            buffer[(y * (*width)) + (x++)] = value;

        }
        else
        {
          buffer[(x++) + (y * (*width))] = value;
        }
      }
      while (x < line_bytes);
    }

    temp_free(pcx_buf);
  }
  temp_free(path);
  return buffer;

}
