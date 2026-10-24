/*
 * Game object and animation structures (DOS v1.21 layout).
 *
 * Layout source: Rayverse types.h (derived from the PC binary). Field names: Rayverse; `field_XX` and
 * `unknown_XX` names are invented placeholders. The PS1 decomp's Obj differs (16-bit positions, u8 type,
 * u32 flags word, main_etat before sub_etat) and is NOT used for layout.
 *
 * Evidence: tests/test_layout.py pins the compiled offsets and marks each one PROVEN (fixed by an instruction in
 * a STRICT function or a code-wide statistic, e.g. sizeof == 0x84 from 84 `imul r,r,0x84` strides) or RAYVERSE
 * (layout source only, not yet covered by our matches).
 * Watcom packs structures to 1 byte by default (-zp1); no pragma is needed.
 */
#ifndef OBJ_H
#define OBJ_H

#include "rtypes.h"

typedef struct eta_t {
    s8 speed_x_right;
    s8 speed_x_left;
    u8 anim_index;
    u8 next_main_etat;
    u8 next_sub_etat;
    union { u8 anim_speed; struct { u8 low : 4; u8 high : 4; } anim_speed_bits; };   /* nibble view (ray RAY_HELICO) */
    u8 sound_index;
    union {                            /* bit view PROVEN by ray RAY_RESPOND_TO_DOWN (bitfield vs mask control) */
        u8 flags;
        struct { u8 bit0:1; u8 bit1:1; u8 bit2:1; u8 bit3:1; u8 anim_reverse:1; /* INVENTED name, PROVEN reverse playback: DO_ANIM DOS 1:467ac; EOA 1:0d074. */ u8 bit5:1; u8 bit6:1; u8 bit7:1; } flags_bits;
    };
} eta_t;

typedef struct sprite_t {
    u32 offset_in_atlas;
    u8 id;
    u8 outer_width;
    u8 outer_height;
    u8 inner_width;
    u8 inner_height;
    union {                            /* low nibble x, high nibble y offset (calc_let_Width2: bitfield read) */
        u8 raw;
        struct { u8 x : 4; u8 y : 4; } bits;
    } sprite_pos;
    union {
        u8 color;
        struct { u8 unused:4; u8 palette_index:4; } color_bits;
    };
    union {
        u8 flags;
        struct { u8 unused0:2; u8 flip_x:1; u8 unused3:5; } flags_bits;
    };
} sprite_t;

typedef struct anim_layer_t {
    u8 mirrored;
    u8 x;
    u8 y;
    u8 sprite_index;
} anim_layer_t;

typedef struct anim_frame_t {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} anim_frame_t;

typedef struct anim_t {
    anim_layer_t *layers;
    anim_frame_t *frames;
    u16 layers_per_frame;
    u16 frames_count;
} anim_t;

typedef struct obj_flags_t {
    unsigned char hurtbyfist : 1;       /* 0x01 */
    unsigned char command_test : 1;     /* 0x02 */
    unsigned char alive : 1;            /* 0x04 */
    unsigned char flip_x : 1;           /* 0x08 */
    unsigned char read_commands : 1;    /* 0x10 */
    unsigned char follow_enabled : 1;   /* 0x20 */
    unsigned char flag_0x40 : 1;        /* 0x40 */
    unsigned char anim_changed : 1;     /* 0x80 */
    unsigned char flag_0x100 : 1;       /* next byte, bit 0 */
} obj_flags_t;

typedef struct cmd_context_t {
    s16 cmd_offset;
    s16 count;
} cmd_context_t;

typedef struct obj_t {
    sprite_t *sprites;              /* 0x00 */
    anim_t *animations;             /* 0x04 */
    u8 *img_buffer;                 /* 0x08 */
    eta_t **eta;                    /* 0x0c */
    u8 *cmds;                       /* 0x10 */
    s16 *cmd_labels;                /* 0x14 */
    cmd_context_t cmd_contexts[1];  /* 0x18 */
    u32 field_1C;                   /* 0x1c */
    u32 link_has_gendoor;           /* 0x20 */
    s32 is_active;                  /* 0x24 */
    s32 x;                          /* 0x28 */
    s32 y;                          /* 0x2c */
    s32 active_flag;                /* 0x30 */
    s16 id;                         /* 0x34 */
    s16 screen_x;                   /* 0x36 */
    s16 screen_y;                   /* 0x38 */
    s16 sub_speed;                  /* 0x3a */
    s16 init_x;                     /* 0x3c */
    s16 init_y;                     /* 0x3e */
    s16 speed_x;                    /* 0x40 */
    s16 speed_y;                    /* 0x42 */
    s16 nb_sprites;                 /* 0x44 */
    s16 cmd_offset;                 /* 0x46 */
    s16 nb_cmd;                     /* 0x48 */
    s16 follow_id;                  /* 0x4a */
    s16 follow_y;                   /* 0x4c */
    s16 follow_x;                   /* 0x4e */
    s16 param;                      /* 0x50 */
    s16 link;                       /* 0x52 */
    s16 ray_dist;                   /* 0x54 */
    s16 iframes_timer;              /* 0x56 */
    u16 test_block_index;           /* 0x58 */
    u16 scale;                      /* 0x5a */
    u16 zdc;                        /* 0x5c */
    u16 active_timer;               /* 0x5e */
    u16 type;                       /* 0x60 */
    u8 btypes[5];                   /* 0x62 center, left, right, up, down */
    u8 unused_padding;              /* 0x67 */
    u8 offset_bx;                   /* 0x68 */
    u8 offset_by;                   /* 0x69 */
    u8 anim_index;                  /* 0x6a */
    u8 anim_frame;                  /* 0x6b */
    u8 sub_etat;                    /* 0x6c */
    u8 main_etat;                   /* 0x6d */
    u8 init_sub_etat;               /* 0x6e */
    u8 init_main_etat;              /* 0x6f */
    u8 cmd;                         /* 0x70 */
    u8 gravity_value_1;             /* 0x71 */
    u8 gravity_value_2;             /* 0x72 */
    u8 change_anim_mode;            /* 0x73 */
    u8 offset_hy;                   /* 0x74 unsigned: DO_PYRANHA 1:5752d zero-extends it; PS1 u8 (Rayverse: s8) */
    u8 follow_sprite;               /* 0x75 */
    u8 hit_points;                  /* 0x76 */
    u8 init_hit_points;             /* 0x77 */
    u8 init_flag;                   /* 0x78 */
    u8 hit_sprite;                  /* 0x79 */
    u8 detect_zone;                 /* 0x7a */
    u8 detect_zone_flag;            /* 0x7b */
    s8 cmd_context_depth;           /* 0x7c */
    u8 config;                      /* 0x7d */
    u8 display_prio;                /* 0x7e */
    u8 timer;                       /* 0x7f */
    u8 anim_count;                  /* 0x80 */
    obj_flags_t flags;              /* 0x81 (2 bytes) */
    u8 field_83;                    /* 0x83 */
} obj_t;

typedef struct zdc_t {
    s16 x_pos;
    s16 y_pos;
    u8 width;
    u8 height;
    u8 flags;
    u8 sprite;
} zdc_t;

typedef struct level_t {          /* DOS 6 bytes (packed); Android reports 8 (natural alignment) */
    obj_t *objects;
    s16 nb_objects;
} level_t;

typedef struct level_obj_t {       /* src/blacktoon.c */
    s16 *obj_ids;
    s16 nb_objects;
} level_obj_t;

typedef struct active_objects_t {  /* actobj: 202 bytes (Android agrees); src/ufo.c */
    s16 objects[100];
    s16 num_active_objects;
} active_objects_t;

typedef u8 (*cmd_fn_t)(obj_t *obj);
typedef struct cptr_t {            /* cptr_tab 2:70e4: 34 x 12 bytes (Android 408); src/objupdate1.c */
    cmd_fn_t read;
    cmd_fn_t skip;
    cmd_fn_t handle;
} cptr_t;

#endif
