#ifndef BT_BS_4E43C_H
#define BT_BS_4E43C_H

/* Types local to verified DOS BSS fragments in this worker. */
typedef struct effect5_state effect5_state_t;
typedef struct effect_state effect_state_t;

typedef struct cb_collision_t {
    s16 dx, dy;
    u8 reflect_x, reflect_y, index, reserved;
} cb_collision_t;

typedef struct casse_brique_entry_t {
    s16 x, y, object_index;
    u8 kind, reserved;
} casse_brique_entry_t;

typedef struct bnk_header_t {
    u8 *offset;
    u32 size;
    u32 field_8;
    u32 field_c;
} bnk_header_t;

typedef struct vitraux_info_t {
    s16 x_pos, y_pos;
    u8 obj_id, index, field_6, mode;
    u16 value;
} vitraux_info_t;

#endif
