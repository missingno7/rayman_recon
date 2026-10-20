/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 pma_couteaux[6] = {3, 5, 5, 3, 3, 4}; //9727C
u8 pma_sequence[4][2] = {{0, 3}, {5, 0}, {5, 1}, {4, 2}}; //97282
u8 place_sequence[5] = {3, 4, 2, 3, 2}; //9728A
s16 pst2_sequence[13] = {1, 21, -100, 8, 1, 0, 18, 160, 8, 3, 42, 19, 14}; //97290
s16 moust1_sequence[9] = {1, 21, -100, 8, 6, 15, 18, 330, 14}; //972AA
s16 moust2_sequence[9] = {0, 21, 330, 8, 6, 16, 18, -120, 14}; //972BC
s16 pst1_sequence[13] = {0, 21, 320, 8, 1, 0, 18, -8, 8, 3, 41, 19, 14}; //972CE
s16 worry_sequence[12] = {0, 21, 100, 8, 3, 43, 19, 8, 3, 44, 19, 14}; //972E8

//97300
s16 car_sequence[33] = {
        0, 21, 200, 8, 3, 47, 19, 8, 3, 48, 19, 17, 18, 20,
        -8, 17, 30, 20, -16, 17, 45, 20, -32, 17, 60, 20, -64,
        17, 65, 20, 0, 19, 14,
};
