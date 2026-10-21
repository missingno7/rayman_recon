#ifndef BT_BS_3FA9D_H
#define BT_BS_3FA9D_H

/* bb1 is a 16-byte Android .bss object. DOS field accesses establish these
 * widths and offsets; names follow the Rayverse bb_data_t declaration. */
typedef struct bb_data_t {
    s16 speed_x;              /* +0 */
    s16 field1_0x2;           /* +2 */
    u8 field2_0x4;            /* +4 */
    u8 saved_main_etat;       /* +5 */
    u8 saved_sub_etat;        /* +6 */
    s16 saved_cmd;            /* +8 */
    s16 sprite6_x;            /* +10 */
    s16 sprite6_y;            /* +12 */
    u8 field8_0xe;            /* +14 */
} bb_data_t;

#endif
