/*
 * Miscellaneous game structures (DOS v1.21). Layouts from Rayverse types.h; each confirmed by the STRICT unit noted.
 */
#ifndef GAME_H
#define GAME_H

#include "rtypes.h"

#include "palette.h"

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

typedef struct bb_attack_entry_t {  /* SerieAtakBB 2:5ec6: [6][7] x 4 bytes (Android 168); src/bbmont.c */
    s16 attack;
    u8 wait_for_fin_atan;
    u8 reserved;
} bb_attack_entry_t;

/* DOS save image; offsets from Rayverse types.h and direct DOS field loads in
 * DO_WIZARD / DO_PHOTOGRAPHE_CMD. The trailing byte accounts for the 604-byte
 * Android symbol size; no code in objupdate2 accesses it. save1 2:3e88c, save2 2:3eae8. */
typedef struct save_state_t {
    u32 triggered_objects[8];
    s16 nb_floc[8];
    s16 vent_x;
    s16 vent_y;
    s16 x_map;
    s16 y_map;
    s16 ray_x_pos;
    s16 ray_y_pos;
    s16 ray_screen_x;
    s16 ray_screen_y;
    u16 ray_flip_x;
    s16 save_obj_id;
    s16 save_obj_x_pos;
    s16 save_obj_y_pos;
    u16 link_init[256];
    u8 save_obj_detect_zone_flag;
    u8 save_obj_flag_1;
    u8 ray_coll_btype[5];
    u8 ray_anim_index;
    u8 ray_anim_frame;
    u8 ray_main_etat;
    u8 ray_sub_etat;
    u8 poing_sub_etat;
    u8 rayevts_reverse;
    u8 rayevts_super_helico;
    u8 rayevts_poing;
    u8 current_pal_id;
    u8 dead_time;
    u8 is_just_saved;
    u8 status_bar_tings;
    u8 field_25b;          /* unknown; size 604 = save2 - save1 (2:3eae8 - 2:3e88c) = Android */
} save_state_t;

typedef struct display_item_t {   /* 216 bytes; text_to_display 2:3c4a0 is [11] (next global level_alw 2:3cde8) */
    char text[200];
    s16 centered_x_pos;
    s16 centered_y_pos;
    s16 width;
    s16 height;
    s16 xpos;
    s16 ypos;
    u8 font_size;
    u8 field_D5;
    u8 is_fond;
    s8 color;
} display_item_t;

extern display_item_t text_to_display[11];

typedef struct options_jeu_t {     /* options_jeu: 32 bytes (Android 32); fields u16 (input switch, fee/worldmap via s16 params) */
    s16 (*test_fire1)(void);
    s16 (*test_fire0)(void);
    s16 (*test_button4)(void);
    s16 (*test_button3)(void);
    u16 jump;
    u16 fist;
    u16 field_14;
    u16 action;
    u16 music_enabled;
    u16 sound_volume;
    u16 is_stereo;
    u16 unknown_1e;
} options_jeu_t;

typedef struct loadinforay_t {
    u8 lives;
    u8 tings;
    u8 cages;
    u8 continues;
} loadinforay_t;

typedef struct credit_t {
    char *text;
    s16 x_pos;
    s16 y_pos;
    u8 font;
    u8 cmd;
    u8 color;
    u8 unknown_b;                  /* DOS stride 12: DISPLAY_TXT_CREDITS indexes (4i - i) * 4 */
} credit_t;

typedef struct record_t {        /* demo record (record 24 bytes); src/input.c, src/fee.c */
    s32 current_offset;
    s32 repeat_length;
    s32 repeat_index;
    s32 length;
    u8 *data;
    u8 is_recording;
    u8 is_playing;
    u8 is_finished;
    u8 unknown_17;
} record_t;

typedef s16 (scene_func_t)(u32 arg);   /* SYNCHRO_LOOP scene callback (Rayverse types.h) */

typedef struct mem_t {             /* block allocator arena (sysutils block_malloc) */
    u32 align;
    u8 *data;
    u32 capacity;
} mem_t;

typedef union obj_type_flags_t {   /* flags[262] at 2:257c: one u32 per object type (Android 1048 bytes) */
    u32 raw;
    struct {
        u8 flags0;
        u8 flags1_lo : 4;
        u8 move_x : 1;                 /* byte 1 bit 4: DOS makeUturn tests [flags+type*4+1] & 0x10 */
        u8 move_y : 1;                 /* byte 1 bit 5 */
        u8 flags1_hi : 2;
        u8 flags2;
        u8 flags3;
    };
} obj_type_flags_t;


typedef struct font_t {             /* alpha 2:3e6e0, alpha2 2:3edc8, alpha_numbers: 10 bytes (Android 12, natural
                                      alignment); copied from obj_t sprites/img_buffer/nb_sprites in LOAD_ALL_FIX. Name INVENTED */
    struct sprite_t *sprites;
    u8 *img_buffer;
    s16 nb_sprites;
} font_t;

#endif
