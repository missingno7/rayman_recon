/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

// Two s16[8] tables (DOS reads them through separate bases 2:2d94c / 2:2d95c; PS1 declares two arrays).
s16 rayon_speed_x[8] = {4, 2, 0, -2, -4, 2, 0, -2};    /* INVENTED name (PS1 D_801C83A4): skops rayon speed_x */
s16 rayon_speed_y[8] = {0, 2, 4, 2, 0, -2, -4, -2};   /* INVENTED name (PS1 D_801C83B4): skops rayon speed_y */
