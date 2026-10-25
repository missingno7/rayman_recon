/* PORT: native views of hifi_local.h and data_6664.c; no DOS pointer tokens. */
/* Native logical fields; serialized bank layout is decoded separately. */
#ifndef PORT_HMI_TYPES_H
#define PORT_HMI_TYPES_H
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

/* PORT: use the full data_6664.c view: its buffer pointer moves subsequent fields. */
typedef struct dos_sound_config_t {
    u32 field_00;
    void *buffer;
    u16 pad_0a;
    u32 field_0c;
    s32 timeout; /* Historical spelling: requested sample rate, 10000 Hz. */
    u32 field_14;
    u32 field_18;
    void *callback;
    u16 pad_22;
    void *field_24;
    u16 pad_2a;
    void *field_2c;
    u16 pad_32;
    void *field_34;
    u16 pad_3a;
    void *field_3c;
    u16 pad_42;
    u32 field_44;
    u32 field_48;
} dos_sound_config_t;

typedef struct dos_sound_request_t {
    u8 *data; /* Native pointer replaces the DOS far data pointer. */
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
