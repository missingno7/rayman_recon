#ifndef TU_FEE_TYPES_H
#define TU_FEE_TYPES_H

#include "rtypes.h"

/* Local DOS layouts used by fee.c; the shared obj_t/game_t layouts stay in src/include. */

typedef struct fee_options_jeu_t {
    s32 (*test_fire1)(void);
    s32 (*test_fire0)(void);
    s32 (*test_button4)(void);
    s32 (*test_button3)(void);
    u16 jump;
    u16 fist;
    u16 field_14;
    u16 action;
    u16 music_enabled;
    u16 sound_volume;
    u16 is_stereo;
} fee_options_jeu_t;

typedef struct fee_record_t {
    s32 current_offset;
    s32 repeat_length;
    s32 repeat_index;
    s32 length;
    u8 *data;
    u8 is_recording;
    u8 is_playing;
    u8 is_finished;
} fee_record_t;

#endif
