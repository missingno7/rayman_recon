/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). Reconstructed as source on 2026-11-13 (user decision: the port reads
   only the game data, never RAYMAN.EXE); before, these bytes were extracted from the player's EXE at build time
   (docs/LINK.md asset step, decision 2026-10-22). */
#include "palette.h"

//95F70: the menus' plasma colours (box.c PlasmaBox, InitPlasma)
rgb_t PalPlasma[16] = {
        {32, 7, 63},
        {31, 7, 61},
        {32, 7, 58},
        {33, 7, 56},
        {36, 10, 53},
        {37, 12, 51},
        {45, 15, 48},
        {52, 17, 46},
        {45, 15, 48},
        {40, 15, 51},
        {35, 12, 53},
        {35, 10, 56},
        {32, 7, 58},
        {30, 5, 61},
        {27, 5, 61},
        {32, 7, 63},
};
