/* GOG v1.21 _DATA 2:06664. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "rtypes.h"


/* INVENTED field names, ABI offsets read from 1:7367d..73f33.
 * hifi.c currently declares only the first 36 bytes; this defines all 76.
 * Far pointers occupy 6 bytes followed by 2 bytes of ABI slot padding. */
typedef struct dos_sound_config_t {
    u32 field_00;
    void __far *buffer;
    u16 pad_0a;
    u32 field_0c;
    s32 timeout;
    u32 field_14;
    u32 field_18;
    void __far *callback;
    u16 pad_22;
    void __far *field_24;
    u16 pad_2a;
    void __far *field_2c;
    u16 pad_32;
    void __far *field_34;
    u16 pad_3a;
    void __far *field_3c;
    u16 pad_42;
    u32 field_44;
    u32 field_48;
} dos_sound_config_t;
dos_sound_config_t sound_config = {
    8192, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
