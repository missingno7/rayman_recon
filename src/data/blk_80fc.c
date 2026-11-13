/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). Reconstructed as source on 2026-11-13 (user decision: the port reads
   only the game data, never RAYMAN.EXE); before, these bytes were extracted from the player's EXE at build time
   (docs/LINK.md asset step, decision 2026-10-22). */
#include "rtypes.h"

/* src/screen_types.h def_bande_t (that header is not on the data units' include path; same layout, 8 bytes). */
typedef struct def_bande_t {
    u16 length;
    u8 field_2;
    u8 _pad_3;
    u8 field_4;
    u8 _pad_5;
    u8 field_6;
    u8 _pad_7;
} def_bande_t;

/* 980FC: the parallax bands of each world's backgrounds (skops.c), [world - 1][background][band]: band height or
   width (length), then three per-band parameters with their padding bytes ({length, field_2, pad, field_4, pad,
   field_6, pad}); Tab_NbBande (blk_8084.c) gives how many bands each background uses. Only the used entries are
   listed; the rest of each background's 300 entries are zero. */
def_bande_t Def_Bande[6][10][300] = {
    { /* world 1: jungle */
        { /* background 0 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 1 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 2 */
            {320, 2, 0, 0, 0, 0, 0},
        },
        { /* background 3 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 4 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 5 */
            {88, 4, 0, 4, 0, 4, 0},
            {110, 4, 0, 0, 0, 0, 0},
            {15, 3, 0, 0, 0, 0, 0},
            {75, 2, 0, 0, 0, 0, 0},
        },
        { /* background 6 */
            {0, 0, 0, 0, 0, 0, 0},
            {64, 2, 0, 0, 0, 0, 0},
            {192, 4, 0, 0, 0, 0, 0},
            {64, 2, 0, 0, 0, 0, 0},
        },
        { /* background 7 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 8 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 9 */
            {288, 2, 0, 0, 0, 0, 0},
        },
    },
    { /* world 2: music */
        { /* background 0 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 1 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 2 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 3 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 4 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 5 */
            {168, 4, 0, 4, 0, 5, 0},
            {70, 4, 0, 0, 0, 0, 0},
            {50, 2, 0, 0, 0, 0, 0},
        },
        { /* background 6 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 7 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 8 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 9 */
            {288, 2, 0, 0, 0, 0, 0},
        },
    },
    { /* world 3: mountain */
        { /* background 0 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 1 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 2 */
            {320, 2, 0, 0, 0, 0, 0},
        },
        { /* background 3 */
            {288, 1, 0, 0, 0, 0, 0},
        },
        { /* background 4 */
            {159, 4, 0, 0, 0, 0, 0},
            {129, 2, 0, 0, 0, 0, 0},
        },
        { /* background 5 */
            {12, 6, 0, 0, 0, 0, 0},
            {20, 4, 0, 0, 0, 0, 0},
            {64, 2, 0, 0, 0, 0, 0},
            {20, 4, 0, 0, 0, 0, 0},
            {56, 6, 0, 0, 0, 0, 0},
            {20, 4, 0, 0, 0, 0, 0},
            {64, 2, 0, 0, 0, 0, 0},
            {20, 4, 0, 0, 0, 0, 0},
            {28, 6, 0, 0, 0, 0, 0},
            {16, 8, 0, 0, 0, 0, 0},
        },
        { /* background 6 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 7 */
            {84, 4, 0, 0, 0, 0, 0},
            {204, 2, 0, 0, 0, 0, 0},
        },
        { /* background 8 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 9 */
            {288, 2, 0, 0, 0, 0, 0},
        },
    },
    { /* world 4: image */
        { /* background 0 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 1 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 2 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 3 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 4 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 5 */
            {177, 4, 0, 0, 0, 0, 0},
            {111, 2, 0, 0, 0, 0, 0},
        },
        { /* background 6 */
            {179, 0, 0, 0, 0, 3, 0},
            {20, 0, 0, 16, 0, 1, 0},
            {1, 0, 0, 32, 0, 1, 0},
            {40, 0, 0, 0, 0, 3, 0},
        },
        { /* background 7 */
            {183, 4, 0, 4, 0, 4, 0},
            {5, 8, 0, 0, 0, 0, 0},
            {21, 6, 0, 0, 0, 0, 0},
            {78, 4, 0, 0, 0, 0, 0},
            {1, 2, 0, 0, 0, 0, 0},
        },
        { /* background 8 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 9 */
            {288, 2, 0, 0, 0, 0, 0},
        },
    },
    { /* world 5: cave */
        { /* background 0 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 1 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 2 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 3 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 4 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 5 */
            {109, 6, 0, 0, 0, 0, 0},
            {178, 4, 0, 0, 0, 0, 0},
            {1, 2, 0, 0, 0, 0, 0},
        },
        { /* background 6 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 7 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 8 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 9 */
            {288, 2, 0, 0, 0, 0, 0},
        },
    },
    { /* world 6: cake */
        { /* background 0 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 1 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 2 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 3 */
            {213, 4, 0, 0, 0, 0, 0},
            {75, 2, 0, 0, 0, 0, 0},
        },
        { /* background 4 */
            {200, 6, 0, 0, 0, 0, 0},
            {87, 4, 0, 0, 0, 0, 0},
            {1, 2, 0, 0, 0, 0, 0},
        },
        { /* background 5 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 6 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 7 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 8 */
            {288, 2, 0, 0, 0, 0, 0},
        },
        { /* background 9 */
            {288, 2, 0, 0, 0, 0, 0},
        },
    },
};
