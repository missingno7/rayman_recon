/* Exact hifi.c compatibility declarations; no ABI or ownership change. */
#ifndef HIFI_LOCAL_H
#define HIFI_LOCAL_H
#include "rtypes.h"
typedef struct archive_header_t {
    s32 offset;
    u32 size;
    u8 xor_byte;
    u8 checksum_byte;
    u8 pad[2];
} archive_header_t;

typedef struct bnk_header_t {
    u8 *offset;
    u32 size;
    u32 field_8;
    u32 field_C;
} bnk_header_t;

typedef struct dos_sound_config_t {
    u32 reserved_00[4];
    s32 timeout;
    u32 reserved_14[2];
    void __far *callback;
    u16 pad_22;
} dos_sound_config_t;

typedef struct dos_sound_request_t {
    u8 __far *data;
    u16 pointer_pad;
    u32 size;
    s32 field_0c;
    u32 field_10;
    s32 volume;
    u32 field_18[4];
    u32 flags;
    u32 field_2c;
    u32 field_30;
    u32 field_34;
    s32 sample_rate;
    u32 field_3c;
    s32 pan;
    u32 reserved[12];
} dos_sound_request_t;
#endif
