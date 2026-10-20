/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 vitrail_clignotement[4][5] = {
        {0, 0, 2, 0, 2},
        {2, 0, 0, 2, 0},
        {2, 2, 0, 0, 0},
        {0, 0, 0, 0, 0}
};

u8 dark_attaque = 0; //9637D
u8 type_dark_attaque = 0; //9637E
