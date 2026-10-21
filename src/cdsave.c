/*
 * Candidate for DOS obj1 1:5ba70..1:5bf60.
 * No Rayverse/PS1 body was seeded for this range. Function names not found in
 * Android are INVENTED and retain descriptive DOS behavior.
 */
#include <dos.h>
#include <i86.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "rtypes.h"

typedef void (__interrupt __far *dos_vector_t)();
extern dos_vector_t interrupt_vectors[256];

extern u8 saved_argv[255];
extern u8 drm_byte_CFA27;
extern u8 byte_CFA29;
extern u8 byte_CFA2A;
extern u8 WindowsLance;
extern u8 byte_CFA14;
extern u8 JeuCracker;
extern u8 OptionMusicCdActive;

extern void __GETDS(void);
extern u8 AudioStatusByte(void);
extern u8 sub_19a8c(void);
extern u8 sub_19ac8(void);
extern u8 sub_1a2e8(s32 value);
extern void sub_1a384(s32 value);
extern u8 sub_1a360(s32 value);
extern void sub_1a3e8(s32 value);
extern void sub_1a3b0(s32 value);
extern void sub_1a440(s32 value);
extern void sub_1a5b8(s32 value);
extern void sub_1a724(s32 value);
extern void stop_cd(void);
extern s16 myRand(s16 max);
extern u8 TOUCHE(u8 key);
extern u8 *temp_malloc(s32 size);
extern void temp_free(u8 *ptr);

void save_interrupt_vectors(void);
void sub_5BAE0(void);
u8 wait_for_cdrom_response(void);

void __interrupt __far interrupt_handler_5ba70(union INTPACK registers)
{
}

void empty_handler_5ba88(void)
{
}

void __interrupt __far interrupt_handler_5ba8c(union INTPACK registers)
{
    _disable();
    _enable();
}

void save_interrupt_vectors(void)
{
    s32 index;

    for (index = 0; index < 256; ++index)
        interrupt_vectors[index] = _dos_getvect(index);
}

/* INVENTED: empty function sharing the aligned return at DOS 1:5bad4. */
void empty_handler_5bad4(void)
{
}

void sub_5BAD8(void)
{
    save_interrupt_vectors();
    sub_5BAE0();
}

void sub_5BAE0(void)
{
    _dos_setvect(0, interrupt_handler_5ba8c);
    _dos_setvect(1, interrupt_handler_5ba8c);
    _dos_setvect(2, interrupt_handler_5ba8c);
    _dos_setvect(3, interrupt_handler_5ba8c);
    _dos_setvect(4, interrupt_handler_5ba8c);
    _dos_setvect(5, interrupt_handler_5ba8c);
}

void sub_5BB48(void)
{
    s32 index;

    for (index = 0; index < 256; ++index)
        _dos_setvect(index, interrupt_vectors[index]);
}

void sub_5BB78(void)
{
    _dos_setvect(3, interrupt_vectors[3]);
}

void sub_5BB94(void)
{
    _dos_setvect(1, interrupt_vectors[1]);
}

void check_saved_drive_letter(void)
{
    if (byte_CFA29 != 0) {
        drm_byte_CFA27 = (u8)(AudioStatusByte() + 0x41);
        if (saved_argv[0] == drm_byte_CFA27)
            byte_CFA2A = 1;
        else
            byte_CFA2A = 0;
    }
}

void sub_5bbe8(void)
{
}

void verify_mscdex(void)
{
    if (!sub_19a8c()) {
        printf("Please install MSCDEX v 2.1 or above.\n");
        sub_1a384(AudioStatusByte());
        exit(-1);
    }

    if (sub_19ac8() == 1) {
        byte_CFA29 = 1;
        JeuCracker = 0;
    } else {
        byte_CFA29 = 0;
    }
}

void check_cd_for_save(void)
{
    char path[20];
    FILE *file;
    s32 index;

    if (byte_CFA29 != 0) {
        if (sub_1a360(AudioStatusByte())) {
            stop_cd();
            sprintf(path, "%c:\\????????.???", drm_byte_CFA27);
            for (index = 0; index < 8; ++index)
                path[index + 2] = (char)(myRand(0x1a) + 'a');
            /* DOS 1:5bcaf stores the extension at offsets 11 through 13. */
            for (index = 0; index < 3; ++index)
                path[index + 11] = (char)(myRand(0x1a) + 'a');
            file = fopen(path, "wb");
            if (file != NULL) {
                fclose(file);
                unlink(path);
                JeuCracker = 1;
            }
        } else {
            JeuCracker = 1;
        }
    } else {
        JeuCracker = 1;
    }
}

u8 verify_cdrom_response(void)
{
    if (byte_CFA29 != 0) {
        while (0 == WindowsLance && !sub_1a2e8(AudioStatusByte()))
            ;
        sub_1a5b8(AudioStatusByte());
        if (!wait_for_cdrom_response()) {
            if (WindowsLance != 0 || byte_CFA14 != 0) {
                printf("Please insert Rayman's cdrom.\n");
                sub_1a384(AudioStatusByte());
                exit(-1);
            } else {
                printf("Please insert Rayman's cdrom.\n- If rayman can not find disk, press ESC,\n  and rerun Rayman.\n- If you want try Rayman in limited mode, press SPACE.\n");
            }

            sub_1a384(AudioStatusByte());
retry_response:
            sub_1a3e8(AudioStatusByte());
            while (0 == JeuCracker && !sub_1a2e8(AudioStatusByte())) {
                if (TOUCHE(1)) {
                    if (0 == WindowsLance && WindowsLance == JeuCracker) {
                        sub_1a440(AudioStatusByte());
                    }
                    exit(-1);
                }
                if (TOUCHE(0x39))
                    JeuCracker = 1;
            }

            if (0 == JeuCracker) {
                if (!wait_for_cdrom_response()) {
                    goto retry_response;
                }
            }
            if (WindowsLance == 0 && JeuCracker == 0) {
                sub_1a440(AudioStatusByte());
                sub_1a3b0(AudioStatusByte());
            }
        }
        /* DOS 1:5be5f..5be6d: this branch has its own return. */
        return (u8)(JeuCracker == 0);
    } else {
        printf("Please install MSCDEX v 2.1 or above.\n- press ESC to quit,\n- press SPACE to play rayman without music.\n");
        while (!TOUCHE(1) && !TOUCHE(57))
            ;
        if (TOUCHE(1))
            exit(-1);
        if (TOUCHE(57))
            OptionMusicCdActive = 0;
    }
    return (u8)(JeuCracker == 0);
}

u8 wait_for_cdrom_response(void)
{
    u8 *buffer;
    s32 count;
    u8 ready;

    buffer = temp_malloc(100);
    count = 0;
    ready = 1;
poll_status:
    if (!sub_1a360(AudioStatusByte()))
        goto count_poll;
poll_final_status:
    if (sub_1a360(AudioStatusByte())) {
        sub_1a724(AudioStatusByte());
        goto finished;
    }
    goto not_ready;
count_poll:
    ++count;
    if (count < 1000)
        goto poll_status;
    goto poll_final_status;
not_ready:
    ready = 0;
finished:
    temp_free(buffer);
    return ready;
}

u8 save_file_exists(const char *path)
{
    FILE *file;

    file = fopen(path, "rb");
    if (file == NULL)
        return 0;
    fclose(file);
    return 1;
}

void sub_5BF58(void)
{
}
