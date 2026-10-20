/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

#ifdef FIX_BLACK_BAR_ON_THE_LEFT
s16 TabW1[4] = {0, 12, 20, 24}; //9684C
#else
s16 TabW1[4] = {4, 12, 20, 24}; //9684C
#endif
s16 TabW2[4] = {320, 312, 304, 300}; //96854
s16 TabH1[4] = {0, 8, 16, 20}; //9685C
s16 TabH2[4] = {200, 192, 184, 180}; //96864
