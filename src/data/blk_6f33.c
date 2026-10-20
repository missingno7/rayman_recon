/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 firstEncounterSequence[40] = {
        13, 26, 14, 4, 19, 21, 4, 20, 18, 21, 4, 20, 19, 21, 4, 20, 18, 21, 4, 20, 10, 26,
        14, 4, 18, 21, 4, 20, 10, 26, 14, 4, 19, 21, 4, 20, 10, 26, 14, 2
};

u8 secondEncounterSequence1[13] = {13, 26, 14, 4, 7, 0, 7, 0, 10, 26, 14, 4, 3}; //96F5B
u8 secondEncounterSequenceSuite[12] = {7, 4, 7, 8, 10, 26, 14, 13, 26, 14, 4, 2}; //96F68
u8 secondEncounterSequence2[21] = {8, 0, 8, 0, 8, 0, 8, 4, 18, 21, 4, 20, 19, 21, 4, 20, 10, 26, 14, 4, 2}; //96F74
u8 secondEncounterSequence3[19] = {9, 0, 18, 21, 4, 20, 9, 4, 18, 21, 4, 20, 10, 26, 14, 13, 14, 4, 2}; //96F89
u8 secondEncounterSequence4[21] = {7, 8, 8, 8, 9, 0, 19, 21, 4, 20, 9, 8, 18, 21, 4, 20, 10, 26, 14, 4, 2}; //96F9C
