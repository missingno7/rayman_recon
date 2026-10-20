/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 mereDenisDyingSequence[3] = {42, 46, 5}; //96E44
u8 mereDenisMachineHitSequence[2] = {44, 2}; //96E47
u8 mereDenisHitSequence[2] = {40, 2}; //96E49
u8 mereDenisArrivalSequence[7] = {7, 8, 12, 38, 13, 14, 0}; //96E4B
u8 mereDenisEncounter1[8] = {41, 15, 16, 41, 26, 18, 19, 1}; //96E52
u8 mereDenisEncounter2[9] = {21, 22, 45, 24, 41, 26, 18, 19, 1}; //96E5A
//96E63
u8 mereDenisEncounter3[56] = {
        27, 28, 0, 0, 29, 28, 32, 254, 29, 28, 0, 1, 29, 28,
        32, 254, 29, 28, 16, 2, 29, 28, 16, 2, 29, 28, 48, 3,
        29, 28, 0, 4, 29, 28, 92, 5, 29, 28, 54, 5, 29, 28,
        16, 5, 29, 28, 0, 6, 29, 30, 24, 41, 26, 18, 19, 0
};
u8 mereDenisGoBehindMachine[5] = {21, 22, 25, 31, 0}; //96E9B
//96EA0
u8 mereDenisBehindMachine[58] = {
        36, 35, 43, 1, 34, 36, 34, 33, 1, 35, 36, 35, 43, 1,
        34, 34, 33, 1, 35, 35, 43, 2, 34, 34, 33, 1, 35, 35,
        43, 2, 34, 32, 1, 34, 33, 3, 35, 35, 43, 1, 34, 34,
        33, 3, 35, 35, 43, 1, 34, 34, 33, 3, 35, 35, 43, 3,
        34, 1
};
u8 mereDenisMachineFlies[3] = {37, 39, 0}; //96EDA
//96EDD
u8 mereDenisMedley[27] = {
        45, 24, 41, 26, 18, 19, 27, 28, 16, 255, 29, 45, 24,
        41, 26, 18, 18, 19, 21, 22, 28, 32, 254, 29, 1, 0, 0
};
//96EF8
u8 *mereDenisActionSequences[] = {
        mereDenisArrivalSequence,
        mereDenisEncounter1,
        mereDenisEncounter2,
        mereDenisEncounter3,
        mereDenisGoBehindMachine,
        mereDenisBehindMachine,
        mereDenisMachineFlies,
        mereDenisMedley,
        mereDenisHitSequence,
        mereDenisMachineHitSequence,
        mereDenisDyingSequence
};

u8 dyingSequence[5] = {16, 23, 24, 22, 25}; //96F24
u8 hitSequence[6] = {16, 17, 15, 26, 11, 5}; //96F29
u8 arrivalSequence[4] = {13, 26, 14, 3}; //96F2F
