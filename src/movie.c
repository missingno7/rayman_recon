#include "obj.h"
#include "game.h"
#include "map.h"
/* tools/seed.py compat prelude: PS1 helper macros as defined in rayman-ps1-decomp include/common.h;
   each guarded separately so a unit's own (DOS-verified) definition wins */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef FLG
#define FLG(x) (1 << x)
#endif
#ifndef LEN
#define LEN(x) (sizeof(x) / sizeof(x[0]))
#endif
#ifndef MAX_1
#define MAX_1(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MAX_2
#define MAX_2(x, y) if (x < y) x = y;
#endif
#ifndef MAX_3
#define MAX_3(x, y) if (y > x) x = y;
#endif
#ifndef MIN_1
#define MIN_1(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MIN_2
#define MIN_2(x, y) if (x > y) x = y;
#endif
#ifndef MIN_3
#define MIN_3(x, y) if (y < x) x = y;
#endif
#ifndef ABS
#define ABS(x) (x >= 0 ? (x) : -(x))
#endif
#ifndef SGN
#define SGN(x) (x >= 0 ? (x > 0 ? 1 : 0) : -1)
#endif
#ifndef ashl16
#define ashl16(v, b) ((v) << (b))
#endif
#ifndef ashr16
#define ashr16(v, b) ((v) >> (b))
#endif

s32 playVideo(const char* path, const char* filename, u8 a2);
s32 playVideo_alt(const char* path, const char* filename, u8 a2);
s32 playVideo2(const char* path, const char* filename, u8 a3, u8 a4);
extern u8 movie_video_buffer[768];
extern void endsynchro(void);
#pragma aux endsynchro parm [] modify exact []
extern void synchro(void);
#pragma aux synchro parm [] modify exact []
extern u8 byte_CFA2A;
extern u8 playing_intro_video;
extern u8 MusicCdActive;
extern u8 movie_playback_state;
extern u8 AudioStatusByte(void);
extern void sub_1a5b8(s32 state);
extern s32 sub_3A5B4(u8 *info);
extern s32 sub_3AA70(u8 *work, char *path);
extern void sub_3AFB4(u8 *work, u8 *info);
extern s32 sub_3B138(u8 *work, u8 *info);
extern s32 sub_3B014(u8 *work, u8 *info);
extern void sub_3AA4C(u8 *work);
extern void sub_3A634(u8 *info);
extern void sub_3B260(void);
extern void sub_3B274(void);

typedef struct movie_local_t {
    u8 work[0x98];
    u8 sound_mode;
    u8 pad[3];
    char path[0x80];
    u8 info[0x18];
} movie_local_t;

/* ORIGINAL_INLINE: DOS 1:2b2ed..1:2b308 saves registers, selects VGA DAC
   entry 1 at port 0x3c8, then sends 0x2fd bytes to port 0x3c9.
   Keep the standard implicit EAX/argument-register clobbers: modify [],
   rather than modify exact []. The DOS caller reloads AH and ESI each
   fade step (1:2b290 and 1:2b2e5), while retaining ECX across steps. */
#pragma aux write_movie_palette = \
    "pushad" \
    "add esi,3" \
    "mov edx,0x3c8" \
    "mov eax,1" \
    "out dx,al" \
    "mov ecx,0x2fd" \
    "mov edx,0x3c9" \
    "rep outsb" \
    "popad" \
    parm [esi] modify []
extern void write_movie_palette(u8 *palette);

void display_movie_frames(void)
{
    s32 pos;
    s32 count;
    s8 color;

    for (count = 0; count < 0x40; count += 2) {
        for (pos = 0; pos != 0x300; pos += 3) {
            color = movie_video_buffer[pos] - 2;
            if (color < 0)
                color = 0;
            movie_video_buffer[pos] = color;

            color = movie_video_buffer[pos + 1] - 2;
            if (color < 0)
                color = 0;
            movie_video_buffer[pos + 1] = color;

            color = movie_video_buffer[pos + 2] - 2;
            if (color < 0)
                color = 0;
            movie_video_buffer[pos + 2] = color;
        }

        endsynchro();
        synchro();

        write_movie_palette(movie_video_buffer);
    }
}

s32 playVideo2(const char *path, const char *filename, u8 a3, u8 a4)
{
    movie_local_t local;
    FILE *file;
    s32 status;

    movie_playback_state = 0;
    playing_intro_video = (stricmp("intro.dat", filename) == 0);

    if (byte_CFA2A) {
        if (strcmp(path, ".\\") == 0)
            return -5;
        strcpy(local.path, path);
        strcat(local.path, filename);
    } else {
        strcpy(local.path, filename);
    }

    file = fopen(local.path, "rb");
    if (file == NULL)
        return -5;
    fclose(file);

    if (MusicCdActive)
        sub_1a5b8(AudioStatusByte());
    else
        a3 = 0;

    sub_3B260();
    status = sub_3A5B4(local.info);
    if (status >= 0) {
        status = sub_3AA70(local.work, local.path);
        if (status >= 0) {
            local.sound_mode = a3;
            sub_3AFB4(local.work, local.info);
            if (a4)
                status = sub_3B138(local.work, local.info);
            else
                status = sub_3B014(local.work, local.info);
            display_movie_frames();
            sub_3AA4C(local.work);
        }
    }
    sub_3A634(local.info);
    sub_3B274();
    return status;
}

s32 playVideo(const char* path, const char* filename, u8 a2) {
    return playVideo2(path, filename, a2, 0);
}

s32 playVideo_alt(const char* path, const char* filename, u8 a2) {
    return playVideo2(path, filename, a2, 1);
}
