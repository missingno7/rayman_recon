#include "obj.h"
#include "game.h"
#include "rtypes.h"

#define BLOOD_DWORD_AT(array, byte_offset) (*(s32 *)((u8 *)(array) + (byte_offset)))

extern s32 SpeedYLine[10];
extern s32 PosXLine[10];
extern s32 PosYLine[320];
extern u8 *display_buffer;
extern u8 *DrawBufferNormal;

extern void MakeMyRand(int set_seed);
extern s16 myRand(s16 max);
extern void synchro(void);

void DoEffectBloodOut(void)
{
    s32 done;
    s32 clear_offset;
    s32 seed_offset;
    s32 line_offset;
    s32 j;
    s32 row;
    s32 pixels;
    s32 *line;
    u8 *dst;
    u8 *src;
    u8 *row_start;

    MakeMyRand(1);

    done = 0;
    clear_offset = 0;
    do {
        clear_offset += 4;
        *(s32 *)((u8 *)PosYLine + clear_offset - 4) = 0;
    } while (clear_offset != 0x500);

    seed_offset = 0;
    do {
        BLOOD_DWORD_AT(PosXLine, seed_offset) = myRand(159);
        BLOOD_DWORD_AT(SpeedYLine, seed_offset) = (myRand(1) + 2) * 320;
        seed_offset += 4;
    } while (seed_offset != 0x28);

    while (!done) {
        done = 1;
        line_offset = 0;
        do {
            BLOOD_DWORD_AT(PosYLine, BLOOD_DWORD_AT(PosXLine, line_offset)) += BLOOD_DWORD_AT(SpeedYLine, line_offset);
            if (BLOOD_DWORD_AT(PosYLine, BLOOD_DWORD_AT(PosXLine, line_offset)) < 64000)
                done = 0;

            j = BLOOD_DWORD_AT(PosXLine, line_offset) - 1;
            while (j >= 0) {
                if (PosYLine[j + 1] >= PosYLine[j]) {
                    PosYLine[j] = (myRand((PosYLine[j + 1] / 320) - (PosYLine[j] / 320)) +
                                   (PosYLine[j] / 320)) * 320;
                    if (PosYLine[j] < 64000)
                        done = 0;
                }
                --j;
            }

            j = BLOOD_DWORD_AT(PosXLine, line_offset) + 1;
            while (j < 160) {
                if (PosYLine[j - 1] < PosYLine[j]) {
                    PosYLine[j] = (myRand((PosYLine[j - 1] / 320) - (PosYLine[j] / 320)) +
                                   (PosYLine[j] / 320)) * 320;
                    if (PosYLine[j] < 64000)
                        done = 0;
                }
                ++j;
            }
            line_offset += 4;
        } while (line_offset != 0x28);

        if (!done) {
            synchro();
            dst = display_buffer;
            src = DrawBufferNormal;
            line = PosYLine;
            for (row = 200; row != 0; --row) {
                row_start = dst;
                for (pixels = 320; pixels != 0; pixels -= 2) {
                    if ((s32)(*line + row_start) < (s32)dst)
                        *(s16 *)dst = *(s16 *)&src[-(*line)];
                    else
                        *(s16 *)dst = 0;
                    ++line;
                    dst += 2;
                    src += 2;
                }
                line -= 160;
            }
        }
    }
}
