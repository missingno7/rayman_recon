/* Central data types. Natural structure alignment is tested against DOS
 * allocation starts; this does not change any canonical code-unit header.
 * Types/names RAYVERSE except where the shared headers document otherwise.
 */
#pragma pack(4)
#include "obj.h"
#include "game.h"
#include "map.h"

typedef struct grp_star_t {
    u8 timer, length, dist, sprite_table_index;
} grp_star_t;

typedef struct pix_gerbe_item_t {
    s16 x_pos, y_pos, speed_x, speed_y;
    u8 y_accel, unk_1;
} pix_gerbe_item_t;

typedef struct pix_gerbe_t {
    pix_gerbe_item_t items[64];
    s16 is_active;
} pix_gerbe_t;

typedef struct options_t {
    s16 music_enabled, sound_volume, SizeScreen, Frequence, fixon;
    s16 BackgroundOn, ScrollDiffOn, field_E, field_10, field_12;
} options_t;

typedef struct ray_stack_t {
    s16 x_pos, y_pos, poing_x_pos, poing_y_pos;
    u8 main_etat, sub_etat, anim_index, anim_frame, flip_x;
    u8 poing_anim_index, poing_anim_frame, poing_flip_x, poing_is_active, scale;
} ray_stack_t;

typedef struct flocon_t flocon_t;
typedef struct x_texture_t { u8 pixels[256]; } x_texture_t;
typedef void (void_func_t)(void);
typedef struct vec2b_t { u8 x, y; } vec2b_t;
typedef void (draw_func_t)(s32, s32, s32, vec2b_t, u8 *, u8 *);
typedef void (fplot_func_t)(u8 *, s32, s32, u8);
typedef s32 (swap_func_t)(u8 *, u8 *, s32, s32);

/* Type/member names INVENTED. DOS 1:6f753..6f7ca initializes 30 records
 * at 2:39688 with stride 12: capacity +0, buffer +4, allocation flag +8.
 * The trailing alignment is natural, not a source padding field. */
typedef struct temp_mem_entry_t {
    u32 capacity;
    u8 *data;
    u8 allocated;
} temp_mem_entry_t;
