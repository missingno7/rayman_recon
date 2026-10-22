#ifndef HMI_LOCAL_H
#define HMI_LOCAL_H
/* INVENTED descriptive names, DOS evidence in NOTES.md. Watcom default packing = 1.
 * Unknown ranges describe observed prefixes, not proven complete SDK types. */
typedef signed long s32;
typedef unsigned long u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef u8 hmi_u8;
typedef u16 hmi_u16;
typedef u32 hmi_u32;
typedef s16 hmi_s16;
typedef s32 hmi_s32;
/* INVENTED DOS-derived field names for the HMI sound library.
 * The offsets below are observed directly in the linked DOS code. */
typedef union hmi_voice_flag_word {
    short raw;
    struct {
        unsigned char low;
        unsigned char high;
    } bytes;
} hmi_voice_flag_word;

typedef struct hmi_far_address {
    unsigned long offset;
    unsigned short selector;
} hmi_far_address;
typedef hmi_far_address hmi_far_handle;
typedef union hmi_far_value {
    hmi_far_address parts;
    unsigned char __far *pointer;
} hmi_far_value;


typedef struct hmi_voice_state {
    hmi_far_value pointer_00;
    unsigned char unknown_06[2];
    hmi_far_value pointer_08;
    unsigned char unknown_0e[2];
    hmi_far_value pointer_10;
    unsigned char unknown_16[2];
    unsigned long dword_18;              /* +0x18 */
    unsigned long dword_1c;              /* +0x1c */
    unsigned long dword_20;              /* +0x20 */
    unsigned char unknown_24[8];
    unsigned long dword_2c;              /* +0x2c */
    hmi_voice_flag_word flags;           /* +0x30 */
    short word_32;                       /* +0x32 */
    short word_34;                       /* +0x34 */
    short word_36;                       /* +0x36 */
    short word_38;                       /* +0x38 */
    short word_3a;                       /* +0x3a */
    hmi_far_value pointer_3c;           /* +0x3c */
    unsigned short unknown_42;
    unsigned long dword_44;              /* +0x44 */
    unsigned short word_48;              /* +0x48 */
    unsigned short word_4a;              /* +0x4a */
    unsigned long dword_4c;              /* +0x4c */
    unsigned long dword_50;              /* +0x50 */
    short word_54;                       /* +0x54 */
    short word_56;                       /* +0x56 */
    short word_58;                       /* +0x58 */
    short word_5a;                       /* +0x5a */
    short word_5c;                       /* +0x5c */
    short word_5e;                       /* +0x5e */
    short word_60;                       /* +0x60 */
    unsigned short unknown_62;
    unsigned long dword_64;              /* +0x64 */
    unsigned short word_68;              /* +0x68 */
} hmi_voice_state;

typedef hmi_far_address hmi_driver_buffer_info;

typedef union hmi_sample_flag_word {
    unsigned short raw;
    struct {
        unsigned char low;
        unsigned char high;
    } bytes;
} hmi_sample_flag_word;

typedef struct hmi_sample_template {
    hmi_far_value pointer_00;            /* +0x00 */
    unsigned short unknown_06;
    unsigned long dword_08;              /* +0x08 */
    unsigned short word_0c;              /* +0x0c */
    unsigned short word_0e;
    unsigned short word_10;              /* +0x10 */
    unsigned short word_12;
    unsigned short word_14;              /* +0x14 */
    unsigned short word_16;
    unsigned short word_18;              /* +0x18 */
    unsigned short word_1a;
    hmi_far_value pointer_1c;            /* +0x1c */
    unsigned short word_22;
    unsigned short word_24;              /* +0x24 */
    unsigned short word_26;
    hmi_sample_flag_word flags_28;        /* +0x28 */
    unsigned short word_2a;
    unsigned long dword_2c;              /* +0x2c */
    unsigned long dword_30;              /* +0x30 */
    unsigned long dword_34;              /* +0x34 */
    unsigned long dword_38;              /* +0x38 */
    unsigned char unknown_3c[4];
    unsigned short word_40;              /* +0x40 */
    unsigned short word_42;
    unsigned short word_44;              /* +0x44 */
    unsigned short word_46;
    unsigned short word_48;              /* +0x48 */
    unsigned short word_4a;
    unsigned short word_4c;              /* +0x4c */
    unsigned short word_4e;
    unsigned short word_50;              /* +0x50 */
    unsigned short word_52;
    unsigned short word_54;              /* +0x54 */
    unsigned short word_56;
    unsigned short word_58;              /* +0x58 */
} hmi_sample_template;

/* Overlapping word and dword accesses at the same DMA selector storage. */
typedef union hmi_selector_storage {
    unsigned value;
    unsigned short low;
} hmi_selector_storage;

/* DMA selector storage observed at 2:2eba4, within the runtime record at 2:2eb98. */
typedef struct hmi_dma_record {
    unsigned char unknown_00[0x0c];
    hmi_selector_storage selector;
} hmi_dma_record;

/* Packed six-byte far-address entry: driver arrays advance by six bytes. */
typedef struct hmi_far_slot {
    unsigned char offset_bytes[4];
    unsigned char selector_bytes[2];
} hmi_far_slot;

/* Fields read or written by the 1:7367d driver initializer; other bytes are unknown. */
typedef struct hmi_driver_init_state {
    unsigned value_00;
    unsigned value_04;
    unsigned short value_08;
    unsigned char unknown_0a[2];
    unsigned value_0c;
    unsigned value_10;
    unsigned value_14;
    unsigned value_18;
    unsigned value_1c;
    unsigned short value_20;
    unsigned char unknown_22[2];
    unsigned value_24;
    unsigned short value_28;
    unsigned char unknown_2a[2];
    unsigned value_2c;
    unsigned short value_30;
    unsigned char unknown_32[2];
    unsigned value_34;
    unsigned short value_38;
    unsigned char unknown_3a[2];
    unsigned value_3c;
    unsigned short value_40;
    unsigned char unknown_42[2];
    unsigned value_44;
    unsigned value_48;
} hmi_driver_init_state;

/* Four observed dwords at offsets 0, 4, 8, and 0x0c of the register far pointer. */
typedef struct hmi_driver_record {
    unsigned value_00;
    unsigned value_04;
    unsigned value_08;
    unsigned value_0c;
} hmi_driver_record;


/* Candidate view of shared loader state. Field offsets/strides are observed,
 * but the global start and total extent remain OPEN (lk_libs/NOTES.md). */
typedef struct hmi_driver_tables {
    hmi_u32 unknown_00;
    hmi_u32 slot_a[5];
    hmi_u32 slot_b[5];
    hmi_far_value far_a[5];
    hmi_far_value far_b[5];
    hmi_u32 slot_c[5];
    hmi_u32 slot_d[5];
    hmi_far_value far_c[5];
    hmi_far_value far_d[5];
    hmi_u32 slot_e[5];
    hmi_u32 slot_f[5];
    hmi_u32 loaded[6];
} hmi_driver_tables_t;
/* A view over observed lock-region addresses, not an owning C declaration.
 * Five-entry spans delimit the next table; valid allocation count remains OPEN. */
extern unsigned char hmi_system_data_2dd10[];
#define hmi_driver_tables (*(hmi_driver_tables_t *)hmi_system_data_2dd10)

/* Separate HMI driver identifier list, loaded from 2:2deae. */
extern hmi_u32 hmi_driver_ids[];

typedef struct hmi_detect_header {
    hmi_u8 unknown_00[0x20];
    hmi_u32 record_count;
    hmi_u8 unknown_24[4];
    hmi_u32 length;
} hmi_detect_header;

typedef struct hmi_detect_driver_info {
    hmi_u8 unknown_00[0x24];
    hmi_u32 value_24;
    hmi_u32 value_28;
    hmi_u8 unknown_2c;
    hmi_u8 flags_2d;
    hmi_u8 unknown_2e[2];
} hmi_detect_driver_info;

/* 1:72e02..72e41: resource list is a far pointer at +0x48,
 * not the beginning of the outer detector request. */
typedef struct hmi_detect_request {
    hmi_u8 unknown_00[0x48];
    hmi_s16 __far *resources;
} hmi_detect_request;

typedef struct hmi_detect_state {
    hmi_u32 initialized;
    void __far *memory_a;
    void __far *memory_b;
    hmi_s32 file_handle;
    hmi_u32 unknown_14;
    hmi_u32 record_index;
    hmi_u32 unknown_1c;
    hmi_u32 unknown_20;
    void __far *table;
    hmi_u16 data_selector;
    hmi_u8 unknown_2c[0x3a];
    hmi_u8 flags_66;
    hmi_u8 flags_67;
} hmi_detect_state;

typedef struct hmi_detect_results {
    hmi_u32 values[4];
} hmi_detect_results;

extern char hmi_system_path[0xff];
extern hmi_detect_header hmi_loader_header_a;
extern hmi_detect_driver_info hmi_loader_header_b;
extern char hmi_detect_path[];
extern hmi_detect_header hmi_detect_file_header;
extern hmi_detect_driver_info hmi_detect_info;
extern hmi_detect_state hmi_detect;
/* Canonical assembly-owned state; interior C views are fields, never bindings. */
typedef struct hmi_dpmi_asm_state_t {
    hmi_u8 unknown_00[0xec];
    hmi_detect_results results;
} hmi_dpmi_asm_state_t;
extern hmi_dpmi_asm_state_t hmi_dpmi_asm_state;
#define hmi_results (hmi_dpmi_asm_state.results)
/* HMI timer event tables. The entry counts and widths are read from the
 * indexed accesses in 1:71cae..72309. */
extern unsigned long hmi_timer_divisor;
extern void __far *hmi_timer_callbacks[16];
extern unsigned long hmi_timer_periods[16];
extern unsigned long hmi_timer_counts[16];
extern unsigned long hmi_timer_fractions[16];
extern unsigned char hmi_timer_options[16];
extern unsigned char hmi_timer_last_option;
extern unsigned long hmi_timer_reentry_count;
extern unsigned long hmi_timer_dispatch_index;
typedef struct hmi_timer_asm_state_t {
    hmi_u8 unknown_00[0x1030];
    hmi_u8 enabled;
    hmi_u8 unknown_1031[3];
    hmi_u8 install_mode;
    hmi_u8 unknown_1035[3];
} hmi_timer_asm_state_t;
extern hmi_timer_asm_state_t hmi_timer_asm_state;
#define hmi_timer_enabled (hmi_timer_asm_state.enabled)
#define hmi_timer_install_mode (hmi_timer_asm_state.install_mode)
extern unsigned long hmi_timer_asm_data_begin;
extern unsigned long hmi_timer_asm_data_end;
extern unsigned long hmi_dpmi_asm_data_begin;
extern unsigned long hmi_dpmi_asm_data_end;
s32 hmi_timer_program_divisor(u32 divisor);

/* HMI driver tables: six-byte stride, starts proven by 1:79102/7977e.
 * Allocation count OPEN: adjacent starts are five entries apart even though
 * 1:791bd accepts index <= 5. Do not infer six-element owning objects. */
extern hmi_far_value hmi_driver_entrypoints[];
extern hmi_far_value hmi_driver_contexts[];
extern unsigned long hmi_loaded_driver_handles[6];
extern unsigned long hmi_driver_callback_values[6];

extern hmi_voice_state __far *hmi_voice_slots[][32];
extern hmi_driver_buffer_info __far *hmi_driver_buffers[];
#endif
