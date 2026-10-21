#ifndef TU_SOUND_TYPES_H
#define TU_SOUND_TYPES_H

typedef struct voice_t {
    s16 obj;
    s16 vol;
    s16 pan;
    s16 snd;
    s16 field_8;
    s16 field_A;
} voice_t;

typedef struct pile_snd_t {
    s16 obj;
    s16 snd;
    s16 prog;
    s16 tone;
    s16 note;
    s16 vol;
    s16 field_C;
    s16 pan;
    s32 end_time;
    s16 field_14;
    s16 field_16;
} pile_snd_t;

typedef struct sound_table_entry_t {
    u8 note;
    u8 prog;
    u8 tone;
    u8 volume;
} sound_table_entry_t;

#endif
