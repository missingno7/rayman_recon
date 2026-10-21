#ifndef MENU_LOCAL_H
#define MENU_LOCAL_H
typedef u8 bool;
#ifndef true
#define true 1
#define false 0
#endif
typedef struct options_t {
    s16 music_enabled;
    s16 sound_volume;
    s16 SizeScreen;
    s16 Frequence;
    s16 fixon;
    s16 BackgroundOn;
    s16 ScrollDiffOn;
    s16 field_E;
    s16 field_10;
    s16 field_12;
} options_t;
#endif
