#include "obj.h"
#include "game.h"
#include "rtypes.h"

/* DoEffectBloodOut: DOS 1:13800..1:139f9 (object boundary 1:13a00).
 * DOS-derived finishing candidate, still NEAR_MATCH; see NOTES.md.
 * Names are Android/Rayverse. The render loop is expressed in C here.
 */
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

    for (seed_offset = 0; seed_offset < 10; ++seed_offset) {
        PosXLine[seed_offset] = myRand(159);
        SpeedYLine[seed_offset] = (myRand(1) + 2) * 320;
    }

    while (!done) {
        done = 1;
        for (line_offset = 0; line_offset < 10; ++line_offset) {
            PosYLine[PosXLine[line_offset]] += SpeedYLine[line_offset];
            if (PosYLine[PosXLine[line_offset]] < 64000)
                done = 0;

            for (j = PosXLine[line_offset] - 1;
                 j >= 0 && PosYLine[j + 1] >= PosYLine[j]; --j) {
                PosYLine[j] = ((PosYLine[j] / 320) +
                              myRand((PosYLine[j + 1] / 320) - (PosYLine[j] / 320))) * 320;
                if (PosYLine[j] < 64000)
                    done = 0;
            }

            for (j = PosXLine[line_offset] + 1;
                 j < 160 && PosYLine[j - 1] >= PosYLine[j]; ++j) {
                PosYLine[j] = (myRand((PosYLine[j - 1] / 320) - (PosYLine[j] / 320)) +
                               (PosYLine[j] / 320)) * 320;
                if (PosYLine[j] < 64000)
                    done = 0;
            }
        }

        if (!done) {
            synchro();
            dst = display_buffer;
            src = DrawBufferNormal;
            line = PosYLine;
            /* DOS 1:139ac sets the origin once for the entire frame. */
            row_start = dst;
            for (row = 200; row != 0; --row) {
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
