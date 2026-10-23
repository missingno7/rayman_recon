/* DOS v1.21 cdaudio object, 1:193b0..1:1a940. Worker results: NOTES.md. */
#include "rtypes.h"
#include <stdio.h>
#include <i86.h>
#include "cd_local.h"

/* Typed initializers with original TU string literals. See NOTES.md. */
#include "rtypes.h"


char *dos_error_messages[16] = {
    "Write Protect Violation",
    "Unknown unit",
    "Drive not ready",
    "Unknown command",
    "CRC error",
    "Bad drive request structure length",
    "Seek error",
    "Unknown media",
    "Sector not found",
    "Printer out of paper",
    "Write fault",
    "Read fault",
    "General failure",
    "Reserved",
    "Reserved",
    "Invalid disk change",
};


extern u8 byte_DEEFB, byte_DEEFC, byte_DEEFD, byte_DEF00;
extern u16 word_DE8BC;
extern u8 WindowsLance, byte_CFA14, JeuCracker;
extern void sub_564f8(s32 command, s32 value);
extern u16 dos_allocate_conventional_block(u16 bytes, u32 *out_offset);
extern u8 dos_free_conventional_block(u16 handle);
extern void sub_5bbe8(void);
extern void FatalError(const char *message);
extern void clear_memory(void *dest, s32 value, s32 count);

/* INVENTED representation view; the DOS stores prove offset:selector = 4:2 bytes.
 * Keep each global a native six-byte far pointer at its true start. */
typedef struct { u32 offset; u16 selector; } cd_far_parts_t;
#define CD_OFFSET(p) (((cd_far_parts_t *)&(p))->offset)
#define CD_SELECTOR(p) (((cd_far_parts_t *)&(p))->selector)

/* DOS MSCDEX state. The far-pointer globals are offset:selector pairs. */
extern u32 cd_offset_830;
extern u32 cd_offset_834;
extern u32 cd_offset_838;
extern u32 cd_offset_83c;
extern u32 cd_offset_840;
extern u32 cd_offset_844;
extern u32 cd_offset_848;
extern u32 cd_offset_84c;
extern u32 cd_offset_850;
extern u32 cd_offset_854;
extern u32 cd_offset_858;
extern u8 __far *cd_buffer_860;
extern u8 __far *cd_buffer_866;
extern u8 __far *cd_buffer_86c;
extern u8 __far *cd_buffer_872;
extern u8 __far *cd_buffer_878;
extern u8 __far *cd_buffer_87e;
extern u8 __far *cd_buffer_884;
extern u8 __far *cd_buffer_88a;
extern u8 __far *cd_buffer_890;
extern u8 __far *cd_buffer_896;
extern u8 __far *cd_buffer_89c;
#include "dos_regs.h"
/* One real-mode register block at 2:4e8a2 (DOS passes its address to sub_564f8 at every INT 2Fh call;
 * fields ax +0, bx +2, cx +4, es +18 are written here). Formerly four separate globals: their adjacency
 * only held at the oracle addresses (natural-link runtime 2026-10-23: ES:BX lost -> MSCDEX request failed). */
extern dos_regs_t cd_request_regs;
extern u16 cd_drive_handle;
extern s16 cd_computed_lba;
extern u8 cd_drive_count;
extern dos_cd_disc_info_t cd_disc_info;
extern u8 cd_drive_map[26];
extern u8 cd_status_ready;
extern u8 cd_status_error;
extern u8 cd_request_busy;
extern u8 cd_status_playing;

extern u8 AudioStatusByte(void);
extern void sub_193d0(u8 value);
extern void sub_193f0(s32 value);
extern u16 sub_19400(u16 value);
extern u16 sub_19410(u8 command, u32 argument, u8 __far *request);
extern void sub_19474(u8 __far *request, u8 command);
extern void sub_194a8(u8 __far *request, u32 argument, u8 command, u16 size);
extern u32 sub_194f4(u8 command);
extern u32 AudioStatusCheck(u8 status);
extern void sub_195ec(u8 command);
extern u8 sub_196c4(void);
extern void sub_19700(void);
extern void sub_19768(void);
extern u8 sub_19790(void);
extern u32 sub_19a8c(void);
extern u32 sub_19ac8(void);
extern u32 sub_19b34(u8 minute, u8 second, u8 frame);
extern u32 sub_19b7c(u8 high, u8 middle, u8 low);
extern void sub_19b9c(s32 frames, u8 *minute, u8 *second, u8 *frame);
extern void sub_19bdc(s32 value, u8 *high, u8 *middle, u8 *low);
extern u32 sub_19c0c(u32 frames);
extern u32 sub_19c40(u32 packed_msf);
extern u16 sub_19c74(u8 track);
extern u16 sub_19cc0(u8 track);
extern u16 sub_19ce4(u8 track);
extern u16 sub_19d30(u8 command, u8 byte_value, u32 value, u32 address);
extern u16 sub_19d88(u8 track);
extern u16 sub_19df0(u8 track);
extern u16 sub_19e58(u8 track);
extern u16 sub_19eb4(u8 track);
extern u16 sub_19f10(u8 command, u8 *minute, u8 *second, u32 *frame);
extern u16 sub_19fa0(u8 command, u8 track, u32 *address, u8 *flags);
extern u16 sub_1a02c(u8 command, u8 *track, u8 *minute, u8 *second, u8 *frame);
extern u16 sub_1a0e4(u8 command, u32 *value);
extern u16 sub_1a158(u8 command, u8 *a, u8 *b, u8 *c, u8 *d);
extern u16 sub_1a1e8(u8 command, u8 a, u8 b, u8 c, u8 d);
extern u16 sub_1a280(u8 track, u8 command);
extern u32 sub_1a2c8(u8 track);
extern u32 sub_1a2e8(u8 track);
extern u32 sub_1a300(u8 track);
extern u32 sub_1a318(u8 track);
extern u32 sub_1a330(u8 track);
extern u32 sub_1a348(u8 track);
extern u32 sub_1a360(u8 track);
extern void sub_1a384(u8 track);
extern void sub_1a3b0(u8 track);
extern void sub_1a3e8(u8 track);
extern void sub_1a440(u8 track);
extern u8 sub_1a4a0(u8 track);
extern void sub_1a4ac(u8 command, u8 track, u8 minute, u8 second);
extern void effect_set_audio_status(u8 status);
extern void effect_restore_audio_status(u8 status);
extern void sub_1a5e0(u8 track, u8 minute, u8 second);
extern u32 sub_1a61c(u8 track, u8 command);
extern void sub_1a654(u8 track);
extern void sub_1a6bc(void);
extern void sub_1a6c4(void);
extern void AudioTrackCommand(u8 track);
extern void sub_1a724(u8 track);
extern u8 sub_1a734(u8 track);
extern void AudioStopCommand(u8 status);

u8 AudioStatusByte(void) { return byte_DEEFB; }
u8 sub_193b8(void) { return byte_DEF00; }
u8 sub_193c0(void) { return byte_DEEFC; }
u8 sub_193c8(void) { return byte_DEEFD; }
void sub_193d0(u8 value) { byte_DEEFB = value; }
void sub_193d8(u8 value) { byte_DEF00 = value; }
void sub_193e0(u8 value) { byte_DEEFC = value; }
void sub_193e8(u8 value) { byte_DEEFD = value; }
void sub_193f0(s32 value) { sub_564f8(0x2f, value); }
u16 sub_19400(u16 value) { if (value & 0x8000) return value & 0xff; return 0x100; }

u16 sub_19410(u8 command, u32 argument, u8 __far *request)
{
    u8 tries;
    tries = 0;
    do {
        cd_request_regs.ax = 0x1510;
        cd_request_regs.es = (u16)(argument >> 16);
        cd_request_regs.bx = (u16)argument;
        cd_request_regs.cx = command;
        ++tries;
        sub_193f0((s32)&cd_request_regs);
        if (tries == 10)
            break;
    } while (request[4] & 0x80);
    return sub_19400(*(u16 __far *)(request + 3));
}

void sub_19474(u8 __far *request, u8 command)
{
    u8 i;
    request[0] = 0x0d;
    request[1] = 0;
    *(u16 __far *)(request + 3) = 0;
    request[2] = command;
    for (i = 0; i < 8; ++i)
        request[i + 5] = 0;
}

void sub_194a8(u8 __far *request, u32 argument, u8 command, u16 size)
{
    sub_19474(request, command);
    request[13] = 0;
    *(u16 __far *)(request + 20) = 0;
    *(u32 __far *)(request + 22) = 0;
    *(u16 __far *)(request + 14) = (u16)argument;
    *(u16 __far *)(request + 16) = (u16)(argument >> 16);
    *(u16 __far *)(request + 18) = size;
}

u32 sub_194f4(u8 command)
{
    sub_194a8(cd_buffer_878, cd_offset_844, 3, 11);
    cd_buffer_890[0] = 0x0f;
    word_DE8BC = sub_19410(command, cd_offset_850, cd_buffer_878);
    return (cd_buffer_890[1] & 1) != 0;
}

u32 AudioStatusCheck(u8 command)
{
    sub_194a8(cd_buffer_878, cd_offset_858, 3, 5);
    cd_buffer_872[0] = 6;
    cd_request_regs.cx = command;
    cd_request_regs.es = (u16)(cd_offset_850 >> 16);
    cd_request_regs.bx = (u16)cd_offset_850;
    cd_request_regs.ax = 0x1510;
    sub_193f0((s32)&cd_request_regs);
    return (cd_buffer_878[4] & 2) != 0;
}

void sub_195ec(u8 command)
{
    sub_194a8(cd_buffer_878, cd_offset_844, 3, 11);
    cd_buffer_890[0] = 0x0f;
    cd_request_regs.cx = command;
    cd_request_regs.es = (u16)(cd_offset_850 >> 16);
    cd_request_regs.bx = (u16)cd_offset_850;
    cd_request_regs.ax = 0x1510;
    sub_193f0((s32)&cd_request_regs);
    cd_status_playing = (*(u16 __far *)(cd_buffer_878 + 3) & 0x200) != 0;
    cd_status_ready = (cd_buffer_890[1] & 1) ? 1 : 0;
    cd_status_error = !(cd_status_playing | cd_status_ready);
    {
        u16 status = *(u16 __far *)(cd_buffer_878 + 3);
        word_DE8BC = sub_19400(status);
    }
}

u8 sub_196c4(void)
{
    cd_request_regs.ax = 0x1500;
    cd_request_regs.bx = 0;
    cd_request_regs.cx = 0;
    sub_193f0((s32)&cd_request_regs);
    byte_DEEFB = (u8)cd_request_regs.cx;
    return (u8)cd_request_regs.bx;
}

void sub_19700(void)
{
    u8 remaining;
    u8 drive;
    remaining = sub_196c4();
    cd_drive_count = remaining;
    drive = 0;
    while (drive < 26) {
        cd_drive_map[drive] = 0;
        if (remaining != 0) {
            cd_request_regs.ax = 0x150b;
            cd_request_regs.cx = drive;
            cd_request_regs.bx = 0;
            sub_193f0((s32)&cd_request_regs);
            if (cd_request_regs.ax != 0) {
                --remaining;
                cd_drive_map[drive] = 1;
            }
        }
        ++drive;
    }
}

void sub_19768(void)
{
    u8 released;
    released = 0;
    if (cd_drive_handle != 0)
        released = dos_free_conventional_block(cd_drive_handle);
    if (released != 0)
        word_DE8BC = 0x102;
}


u8 sub_19790(void)
{
    u8 drive;
    u8 selected;
    u8 allocation_failed;
    word_DE8BC = 0x100;
    cd_request_busy = 0;
    if ((u8)sub_19a8c() == 1 && (u8)sub_19ac8() == 1) {
        if (sub_196c4()) {
            word_DE8BC = 0x100;
            sub_19700();
            cd_drive_handle = dos_allocate_conventional_block(0x8a, &cd_offset_840);
            CD_OFFSET(cd_buffer_872) = 0x30;
            CD_OFFSET(cd_buffer_866) = 0x35;
            CD_OFFSET(cd_buffer_884) = 0x3c;
            CD_OFFSET(cd_buffer_890) = 0x43;
            CD_OFFSET(cd_buffer_88a) = 0;
            CD_OFFSET(cd_buffer_878) = 0;
            CD_OFFSET(cd_buffer_86c) = 0x1a;
            cd_offset_850 = cd_offset_840;
            cd_offset_858 = cd_offset_840 + 0x30;
            allocation_failed = cd_drive_handle == 0;
            CD_SELECTOR(cd_buffer_88a) = cd_drive_handle;
            CD_SELECTOR(cd_buffer_878) = cd_drive_handle;
            cd_offset_854 = cd_offset_840 + 0x35;
            CD_SELECTOR(cd_buffer_872) = cd_drive_handle;
            CD_SELECTOR(cd_buffer_866) = cd_drive_handle;
            cd_offset_84c = cd_offset_840 + 0x3c;
            CD_SELECTOR(cd_buffer_884) = cd_drive_handle;
            CD_SELECTOR(cd_buffer_86c) = cd_drive_handle;
            cd_offset_848 = cd_offset_840 + 0x1a;
            CD_SELECTOR(cd_buffer_890) = cd_drive_handle;
            CD_SELECTOR(cd_buffer_860) = cd_drive_handle;
            cd_offset_844 = cd_offset_840 + 0x43;
            CD_OFFSET(cd_buffer_860) = 0x4e;
            CD_SELECTOR(cd_buffer_87e) = cd_drive_handle;
            cd_offset_838 = cd_offset_840 + 0x4e;
            CD_OFFSET(cd_buffer_87e) = 0x59;
            CD_SELECTOR(cd_buffer_89c) = cd_drive_handle;
            cd_offset_830 = cd_offset_840 + 0x59;
            CD_SELECTOR(cd_buffer_896) = cd_drive_handle;
            cd_offset_83c = cd_offset_840 + 0x5b;
            CD_OFFSET(cd_buffer_89c) = 0x5b;
            CD_OFFSET(cd_buffer_896) = 0x64;
            cd_offset_834 = cd_offset_840 + 0x64;
            if (allocation_failed)
                word_DE8BC = 0x101;
            else {
                if (sub_196c4() > 1) {
                    selected = 0;
                    for (drive = 0; drive < 26 && !selected; ++drive) {
                        if (sub_1a4a0(drive) == 1 && (u8)sub_1a318(drive) == 1 && (u8)sub_1a360(drive)) {
                            cd_request_busy = 0;
                            if (sub_1a734(drive) == 1 && !(u8)sub_1a348(1)) {
                                JeuCracker = 0;
                                sub_5bbe8();
                                if (JeuCracker == 0)
                                    selected = drive;
                                JeuCracker = 0;
                            }
                        }
                    }
                    if (selected)
                        sub_193d0(selected);
                }
                if (!(u8)sub_1a318(AudioStatusByte())) {
                    if (byte_CFA14 == 0)
                        FatalError("Corrupted CD-ROM driver (MSCDEX).");
                    else
                        FatalError("Please insert Rayman's CD-ROM and close the door.");
                }
                printf("CD ROM drive found on %c:\n", AudioStatusByte() + 0x41);
            }
        } else
            word_DE8BC = 1;
    } else
        word_DE8BC = 0x0c;
    if (word_DE8BC == 0x100) {
        if (!sub_1a4a0(AudioStatusByte()))
            word_DE8BC = 1;
        else if (!sub_1a734(AudioStatusByte()))
            word_DE8BC = 8;
    }
    return word_DE8BC == 0x100;
}

u32 sub_19a8c(void)
{
    cd_request_regs.ax = 0x1500;
    cd_request_regs.bx = 0;
    cd_request_regs.cx = 0;
    sub_193f0((s32)&cd_request_regs);
    return cd_request_regs.bx != 0;
}

u32 sub_19ac8(void)
{
    s32 version;
    cd_request_regs.ax = 0x150c;
    cd_request_regs.bx = 0;
    sub_193f0((s32)&cd_request_regs);
    version = cd_request_regs.bx;
    cd_computed_lba = (cd_request_regs.bx & 0xff) + ((version >> 8) & 0xff) * 100;
    return cd_computed_lba >= 0xd2;
}

u32 sub_19b34(u8 minute, u8 second, u8 frame)
{
    return (u32)minute * 4500 + (u32)second * 75 + frame - 150;
}

u32 sub_19b7c(u8 high, u8 middle, u8 low)
{
    return ((u32)high << 16) + ((u32)middle << 8) + low;
}

void sub_19b9c(s32 frames, u8 *minute, u8 *second, u8 *frame)
{
    *minute = (u8)(frames / 4500);
    *second = (u8)((frames % 4500) / 75);
    *frame = (u8)((frames % 4500) % 75);
}

void sub_19bdc(s32 value, u8 *high, u8 *middle, u8 *low)
{
    *high = (u8)((value >> 16) & 0xff);
    *middle = (u8)((value >> 8) & 0xff);
    *low = (u8)value;
}

u32 sub_19c0c(u32 frames)
{
    u8 minute, second, frame;
    sub_19b9c(frames, &minute, &second, &frame);
    return sub_19b7c(minute, second, frame);
}

u32 sub_19c40(u32 packed_msf)
{
    u8 minute, second, frame;
    sub_19bdc(packed_msf, &minute, &second, &frame);
    return sub_19b34(minute, second, frame);
}

u16 sub_19c74(u8 track)
{
    u8 __far *request;
    sub_19474(cd_buffer_86c, 0x85);
    request = cd_buffer_86c;
    word_DE8BC = sub_19410(track, cd_offset_848, request);
    return word_DE8BC;
}

u16 sub_19cc0(u8 track)
{
    word_DE8BC = sub_19c74(track);
    word_DE8BC = sub_19c74(track);
    return word_DE8BC;
}

u16 sub_19ce4(u8 track)
{
    sub_19474(cd_buffer_86c, 0x88);
    word_DE8BC = sub_19410(track, cd_offset_848, cd_buffer_86c);
    return word_DE8BC;
}

u16 sub_19d30(u8 command, u8 byte_value, u32 value, u32 address)
{
    sub_19474(cd_buffer_86c, 0x84);
    cd_buffer_86c[13] = byte_value;
    *(u32 __far *)(cd_buffer_86c + 18) = address;
    *(u32 __far *)(cd_buffer_86c + 14) = value;
    word_DE8BC = sub_19410(command, cd_offset_848, cd_buffer_86c);
    return word_DE8BC;
}

u16 sub_19d88(u8 track)
{
    u8 __far *request;
    sub_194a8(cd_buffer_878, cd_offset_830, 0x0c, 2);
    request = cd_buffer_87e;
    request[0] = 1;
    request[1] = 0;
    word_DE8BC = sub_19410(track, cd_offset_850, cd_buffer_878);
    return word_DE8BC;
}

u16 sub_19df0(u8 track)
{
    u8 __far *request;
    sub_194a8(cd_buffer_878, cd_offset_830, 0x0c, 2);
    request = cd_buffer_87e;
    request[0] = 1;
    request[1] = 1;
    word_DE8BC = sub_19410(track, cd_offset_850, cd_buffer_878);
    return word_DE8BC;
}

u16 sub_19e58(u8 track)
{
    u8 __far *request;
    sub_194a8(cd_buffer_878, cd_offset_858, 0x0c, 1);
    request = cd_buffer_872;
    request[0] = 0;
    word_DE8BC = sub_19410(track, cd_offset_850, cd_buffer_878);
    return word_DE8BC;
}

u16 sub_19eb4(u8 track)
{
    u8 __far *request;
    sub_194a8(cd_buffer_878, cd_offset_858, 0x0c, 1);
    request = cd_buffer_872;
    request[0] = 5;
    word_DE8BC = sub_19410(track, cd_offset_850, cd_buffer_878);
    return word_DE8BC;
}

u16 sub_19f10(u8 command, u8 *minute, u8 *second, u32 *frame)
{
    sub_194a8(cd_buffer_878, cd_offset_854, 3, 7);
    cd_buffer_866[0] = 0x0a;
    cd_buffer_866[1] = 0;
    cd_buffer_866[2] = 0;
    *(u32 __far *)(cd_buffer_866 + 3) = 0;
    word_DE8BC = sub_19410(command, cd_offset_850, cd_buffer_878);
    *minute = cd_buffer_866[1];
    *second = cd_buffer_866[2];
    *frame = *(u32 __far *)(cd_buffer_866 + 3);
    return word_DE8BC;
}

u16 sub_19fa0(u8 command, u8 track, u32 *address, u8 *flags)
{
    sub_194a8(cd_buffer_878, cd_offset_84c, 3, 7);
    cd_buffer_884[0] = 0x0b;
    cd_buffer_884[1] = track;
    *(u32 __far *)(cd_buffer_884 + 2) = 0;
    cd_buffer_884[6] = 0;
    word_DE8BC = sub_19410(command, cd_offset_850, cd_buffer_878);
    *address = *(u32 __far *)(cd_buffer_884 + 2);
    *flags = cd_buffer_884[6];
    return word_DE8BC;
}

u16 sub_1a02c(u8 command, u8 *track, u8 *minute, u8 *second, u8 *frame)
{
    u16 value;
    sub_194a8(cd_buffer_878, cd_offset_838, 3, 11);
    cd_buffer_860[0] = 0x0c;
    word_DE8BC = value = sub_19410(command, cd_offset_850, cd_buffer_878);
    *track = ((cd_buffer_860[2] & 0xf0) >> 4) * 10 + (cd_buffer_860[2] & 15);
    *minute = cd_buffer_860[4];
    *second = cd_buffer_860[5];
    *frame = cd_buffer_860[6];
    return (u16)value;
}

u16 sub_1a0e4(u8 command, u32 *value)
{
    sub_194a8(cd_buffer_878, cd_offset_858, 3, 5);
    cd_buffer_872[0] = 6;
    *(u32 __far *)(cd_buffer_872 + 1) = 0;
    word_DE8BC = sub_19410(command, cd_offset_850, cd_buffer_878);
    *value = *(u32 __far *)(cd_buffer_872 + 1);
    return word_DE8BC;
}

u16 sub_1a158(u8 command, u8 *a, u8 *b, u8 *c, u8 *d)
{
    u16 result;
    sub_194a8(cd_buffer_878, cd_offset_83c, 3, 9);
    cd_buffer_89c[0] = 4;
    word_DE8BC = result = sub_19410(command, cd_offset_850, cd_buffer_878);
    *a = cd_buffer_89c[2];
    *b = cd_buffer_89c[4];
    *c = cd_buffer_89c[6];
    *d = cd_buffer_89c[8];
    return result;
}

u16 sub_1a1e8(u8 command, u8 a, u8 b, u8 c, u8 d)
{
    u8 __far *request;
    sub_194a8(cd_buffer_878, cd_offset_83c, 0x0c, 9);
    request = cd_buffer_89c;
    request[0] = 3;
    request[1] = 0;
    request[3] = 1;
    request[5] = 2;
    request[7] = 3;
    request[2] = a;
    request[4] = b;
    request[6] = c;
    request[8] = d;
    word_DE8BC = sub_19410(command, cd_offset_850, cd_buffer_878);
    return word_DE8BC;
}

u16 sub_1a280(u8 track, u8 command)
{
    /* COMPILER_STEERED: the address-taken local retains the word temporary,
     * giving AX for the command and BL for track (DOS 1:1a280..1a2c5). */
    u16 function;
    u16 *p = &function;
    function = command;
    *p |= 0x1500;
    cd_request_regs.ax = function;
    cd_request_regs.es = (u16)(cd_offset_834 >> 16);
    cd_request_regs.bx = (u16)cd_offset_834;
    cd_request_regs.cx = track;
    sub_193f0((s32)&cd_request_regs);
    return word_DE8BC;
}

u32 sub_1a2c8(u8 track)
{
    u32 value;
    word_DE8BC = sub_1a0e4(track, &value);
    return value;
}

u32 sub_1a2e8(u8 track) { return (sub_1a2c8(track) & 1) == 0; }
u32 sub_1a300(u8 track) { return (sub_1a2c8(track) & 2) == 0; }
u32 sub_1a318(u8 track) { return (sub_1a2c8(track) & 0x10) != 0; }
u32 sub_1a330(u8 track) { return (sub_1a2c8(track) & 0x100) != 0; }
u32 sub_1a348(u8 track) { return (cd_disc_info.track_flags[track] & 0x40) == 0; }
u32 sub_1a360(u8 track)
{
    u32 status;
    status = sub_1a2c8(track);
    if ((status & 0x800) == 0 && word_DE8BC == 0x100)
        return 1;
    return 0;
}

void sub_1a384(u8 track)
{
    if ((u8)sub_1a300(track) == 1)
        word_DE8BC = sub_19d88(track);
    else
        word_DE8BC = 0x100;
}

void sub_1a3b0(u8 track)
{
    if (!(u8)sub_1a300(track) && (u8)sub_1a2e8(track) == 1)
        word_DE8BC = sub_19df0(track);
    else
        word_DE8BC = 0x100;
}

void sub_1a3e8(u8 track)
{
    if ((u8)sub_1a2e8(track) == 1) {
        if ((u8)sub_1a300(track) == 1)
            word_DE8BC = sub_19d88(track);
        word_DE8BC = sub_19e58(track);
        while ((u8)sub_1a2e8(track) == 1) { }
    } else {
        word_DE8BC = 0x100;
    }
}

void sub_1a440(u8 track)
{
    if (byte_CFA14 == 0 && WindowsLance == 0 && ((u8)sub_1a2e8(track) ? 1 : 0)) {
        word_DE8BC = sub_19eb4(track);
        while ((u8)sub_1a2e8(track) == 0) { }
    } else {
        word_DE8BC = 0x100;
    }
}

/* Define this before sub_1a4a0: Watcom keeps its cold return at 1:1a493,
 * then moves its entry/body beside the later sub_1a724 fall-through caller.
 * Nested guards below are also necessary for the DOS block placement. */
u8 sub_1a734(u8 track)
{
    u8 result;
    u8 first;
    u8 last;
    u8 index;
    u8 flag;
    u8 retry;
    u32 leadout;
    u32 start;

    result = 1;
    if (!cd_request_busy) {
        retry = 0xff;
        sub_1a440(track);
        while (!(u8)sub_1a360(track) && --retry) { }
        if ((u8)sub_1a360(track) == 1) {
            clear_memory((void *)&cd_disc_info, 0, 0x51f);
            word_DE8BC = sub_19f10(track, &first, &last, &leadout);
            cd_disc_info.first_track = first;
            cd_disc_info.last_track = last;
            cd_disc_info.leadout_lba = leadout;
            cd_disc_info.track_count = last + 1 - first;
            for (index = first; index <= cd_disc_info.last_track; ++index) {
                word_DE8BC = sub_19fa0(track, index, &start, &flag);
                cd_disc_info.track_start[index] = start;
                cd_disc_info.track_flags[index] = flag;
            }
            cd_disc_info.leadout = cd_disc_info.leadout_lba;
            cd_disc_info.track_start[cd_disc_info.last_track + 1] = cd_disc_info.leadout_lba;
            for (index = cd_disc_info.first_track; index <= cd_disc_info.last_track; ++index) {
                cd_disc_info.track_end[index] = sub_19c40(cd_disc_info.track_start[index + 1]) - sub_19c40(cd_disc_info.track_start[index]);
                cd_disc_info.track_length[index] = sub_19c40(cd_disc_info.track_start[index + 1]) - sub_19c40(cd_disc_info.track_start[index]);
            }
            cd_disc_info.track_start[cd_disc_info.last_track + 1] = 0;
            cd_disc_info.leadout = sub_19c0c(sub_19c40(cd_disc_info.leadout_lba) - sub_19c40(cd_disc_info.track_start[cd_disc_info.first_track]));
            cd_request_busy = 1;
            result = word_DE8BC == 0x100;
        } else
            result = 0;
    }
    return result;
}

u8 sub_1a4a0(u8 track) { return cd_drive_map[track]; }

void sub_1a4ac(u8 command, u8 track, u8 minute, u8 second)
{
    if (sub_1a734(command) == 1) {
        if (track >= cd_disc_info.first_track && track <= cd_disc_info.last_track) {
            if ((u8)sub_1a348(track) == 1) {
                s32 requested;
                {
                    s32 base;
                    requested = (second + (minute << 8)) << 8;
                    base = cd_disc_info.track_start[track];
                    requested += base;
                }
                if (requested < (s32)(track < cd_disc_info.last_track ? cd_disc_info.track_start[track + 1] : cd_disc_info.leadout_lba)) {
                    word_DE8BC = sub_19c74(command);
                    word_DE8BC = sub_19d30(command, 1, requested, cd_disc_info.track_length[track] - (minute * 60 + second) * 75);
                } else
                    word_DE8BC = 8;
            } else
                word_DE8BC = 7;
        } else if (track == 100)
            word_DE8BC = 7;
        else
            word_DE8BC = 8;
    }
}

void effect_set_audio_status(u8 status)
{
    word_DE8BC = sub_19c74(status);
}
void sub_1a5b8(u8 status)
{
    word_DE8BC = sub_19cc0(status);
}
void effect_restore_audio_status(u8 status)
{
    word_DE8BC = sub_19ce4(status);
}

void sub_1a5e0(u8 track, u8 minute, u8 second)
{
    if ((u8)sub_1a330(track) == 1)
        word_DE8BC = sub_1a1e8(track, minute, second, minute, second);
    else
        word_DE8BC = 3;
}

u32 sub_1a61c(u8 track, u8 command)
{
    u32 result;
    word_DE8BC = sub_1a280(track, command);
    if (word_DE8BC == 0x100) {
        u32 segment = (cd_offset_834 >> 12) & ~15;
        result = cd_offset_834 & 0xffff;
        result += segment;
    } else
        result = 0;
    return result;
}

void sub_1a654(u8 track)
{
    u8 status;
    status = AudioStatusByte();
    sub_19474(cd_buffer_86c, 0x84);
    cd_buffer_86c[13] = 1;
    *(u32 __far *)(cd_buffer_86c + 14) = cd_disc_info.track_start[track];
    *(u32 __far *)(cd_buffer_86c + 18) = cd_disc_info.track_end[track];
    word_DE8BC = sub_19410(status, cd_offset_848, cd_buffer_86c);
}

void sub_1a6bc(void)
{
    sub_1a6c4();
    sub_1a6c4();
}

void sub_1a6c4(void)
{
    u8 status;
    u8 __far *request;
    status = AudioStatusByte();
    sub_19474(cd_buffer_86c, 0x85);
    request = cd_buffer_86c;
    word_DE8BC = sub_19410(status, cd_offset_848, request);
}

void AudioTrackCommand(u8 track)
{
    sub_1a6c4();
    sub_1a654(track);
}

void sub_1a724(u8 track)
{
    cd_request_busy = 0;
    (void)sub_1a734(track);
}


void AudioStopCommand(u8 status)
{
    sub_195ec(status);
    if (!cd_status_error) {
        sub_19cc0(status);
        do {
            sub_195ec(status);
        } while (!cd_status_error);
    }
}
