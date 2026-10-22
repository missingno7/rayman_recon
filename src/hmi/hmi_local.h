#ifndef HMI_LOCAL_H
#define HMI_LOCAL_H
/* HMI_SDK public identifiers are taken from the pinned SOS library. Private
 * field views retain only DOS-observed names and extents. */
/* Exact SOS scalar typedefs from the pinned sos.h/sosdefs.h prelude. */
#ifndef _SOSDEFS_DEFINED
#define _SOSDEFS_DEFINED
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned WORD;
typedef signed long LONG;
typedef unsigned long DWORD;
typedef BYTE *PBYTE;
typedef char *PSTR;
typedef WORD *PWORD;
typedef LONG *PLONG;
typedef void *PVOID;
typedef BYTE __far *LPBYTE;
typedef BYTE __far *LPSTR;
typedef WORD __far *LPWORD;
typedef LONG __far *LPLONG;
typedef void __far *LPVOID;
typedef unsigned HANDLE;
#endif
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
/* SOS public pointer table type; the DOS-used prefix is laid out at the
 * offsets in the pinned sos.h _SOS_SAMPLE declaration. */
typedef hmi_voice_state _SOS_SAMPLE;

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
/* sosDIGIStartSample/ContinueSample use the _SOS_START_SAMPLE SDK type.
 * This local view names the DOS-observed members at those SDK offsets. */
typedef hmi_sample_template _SOS_START_SAMPLE;

/* Pinned sos.h VDS_STRUCT layout. The DOS view exposes a typed overlay on
 * physical because the target masks its low word at +0x0c. */
typedef union hmi_vds_physical {
    unsigned physical;
    struct {
        unsigned short low;
        unsigned short high;
    } words;
} hmi_vds_physical;
typedef struct VDS_STRUCT {
    unsigned region_size;
    unsigned offset;
    unsigned short segment;
    unsigned short ID;
    hmi_vds_physical dos_physical;
} VDS_STRUCT;

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
/* sosDIGIInitDriver receives this SDK record; the local split members preserve
 * the observed six-byte far-pointer pairs and padding. */
typedef hmi_driver_init_state _SOS_INIT_DRIVER;

/* Four observed dwords at offsets 0, 4, 8, and 0x0c of the register far pointer. */
typedef struct hmi_driver_record {
    unsigned value_00;
    unsigned value_04;
    unsigned value_08;
    unsigned value_0c;
} hmi_driver_record;
typedef struct _SOS_HARDWARE {
    unsigned wPort;
    unsigned wIRQ;
    unsigned wDMA;
    unsigned wParam;
} _SOS_HARDWARE;

/* SDK globals are independent objects, not fields of the lock marker. */
extern WORD _sosDIGIData_Start;
extern WORD _sosDIGIData_End;
extern WORD _wSOSDriverLinear[], _wSOSTimerLinear[];
extern WORD _wSOSDriverSize[], _wSOSTimerSize[];
extern WORD _wSOSDriverMemHandle[], _wSOSTimerMemHandle[];
extern hmi_far_value _lpSOSTimer[], _lpSOSTimerCS[];

/* Separate HMI driver identifier list, loaded from 2:2deae. */
extern WORD _wSOSDriverType[];

/* SOS file and driver headers. Layouts match the pinned sos.h fields at the
 * DOS-read offsets 0x20, 0x24, 0x28, and 0x2c. */
typedef struct _FILEHEADER {
    unsigned char szName[32];
    unsigned wDrivers;
    unsigned lOffset;
    unsigned lFileSize;
} _FILEHEADER;

typedef struct _DRIVERHEADER {
    unsigned char szName[32];
    unsigned lNextDriver;
    unsigned wSize;
    unsigned wDeviceID;
    unsigned wExtenderType;
} _DRIVERHEADER;

/* 1:72e02..72e41: resource list is a far pointer at +0x48,
 * not the beginning of the outer detector request. */
typedef struct hmi_detect_request {
    hmi_u8 unknown_00[0x48];
    hmi_s16 __far *resources;
} hmi_detect_request;

#pragma pack(4)
typedef struct _SOS_CAPABILITIES {
    unsigned char szDeviceName[32];
    unsigned wDeviceVersion;
    unsigned wBitsPerSample;
    unsigned wChannels;
    unsigned wMinRate;
    unsigned wMaxRate;
    unsigned wMixerOnBoard;
    unsigned wMixerFlags;
    unsigned wFlags;
    short __far *lpPortList;
    short __far *lpDMAList;
    short __far *lpIRQList;
    short __far *lpRateList;
    unsigned fBackground;
    unsigned wDeviceID;
    unsigned wTimerID;
} _SOS_CAPABILITIES;
typedef _SOS_CAPABILITIES __far *LPSOSDEVICECAPS;
#pragma pack()

typedef struct hmi_detect_results {
    hmi_u32 values[4];
} hmi_detect_results;

extern BYTE _pSOSDriverPath[0xff];
extern _FILEHEADER sLOADFileHeader;
extern _DRIVERHEADER sLOADDriverHeader;
extern BYTE _pSOSTempDriverPath[];
extern _FILEHEADER _sDETFileHeader;
extern _DRIVERHEADER _sDETDriverHeader;
extern BOOL _wDETInitialized;
extern LPSTR _lpDETDriverBuffer;
extern LPSTR _lpDETDriverBufferCS;
extern WORD _hDETFile;
extern DWORD _dwDETDriverIndex;
extern WORD _wDETDriverIndexCur;
extern WORD _wDETMemHandle;
extern WORD _wDETLinear;
extern LPSOSDEVICECAPS _lpDETDeviceCaps;
extern _SOS_CAPABILITIES _sDETCaps;
/* SDK WORD results occupy four bytes (word result and reserved zero word). */
extern WORD wDetectPort, wDetectDMA, wDetectIRQ, wDetectParam;
/* HMI timer event tables. The entry counts and widths are read from the
 * indexed accesses in 1:71cae..72309. */
extern WORD _wTIMERValue;
extern void __far *_lpTIMEREvents[16];
extern WORD _wTIMEREventRate[16];
extern WORD _dwTIMEREventFraction[16];
extern WORD _dwTIMEREventFractionCurrent[16];
extern BYTE _bSOSMIDITimerSongHandler[16];
extern BYTE _bSOSMIDISongHandle;
extern WORD _wSOSTimerEntered;
extern WORD _wSOSTimerEventIndex;
extern BYTE _bTIMERInstalled, _bTIMERDPMI;
extern WORD _sosDIGIData1_Start;
extern WORD _sosDIGIData1_End;
extern WORD _sosDIGIData2_Start;
extern WORD _sosDIGIData2_End;
s32 sosTIMERSetRate(u32 divisor);

/* HMI driver tables: six-byte stride, starts proven by 1:79102/7977e.
 * Allocation count OPEN: adjacent starts are five entries apart even though
 * 1:791bd accepts index <= 5. Do not infer six-element owning objects. */
extern hmi_far_value _lpSOSDriver[];
extern hmi_far_value _lpSOSDriverCS[];
extern BOOL _wSOSDriverLoaded[5];
extern WORD _wSOSOutputRate[5];

extern _SOS_SAMPLE __far *_lpSOSSampleList[][32];
extern hmi_driver_buffer_info __far *_lpSOSDMAFillCount[];
#endif
