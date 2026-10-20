/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

s16 droppedBombLine0[8] = {8, 8, 8, 8, 8, 8, 8, 8}; //96C88
s16 droppedBombLine1[8] = {2, 8, 8, 8, 8, 8, 8, 6}; //96C98
s16 droppedBombLine2[8] = {0, 0, 0, 0, 8, 0, 0, 0}; //96CA8
s16 droppedBombLine3[8] = {7, 8, 8, 0, 0, 8, 8, 1}; //96CB8
s16 droppedBombLine4[8] = {8, 8, 1, 8, 8, 7, 8, 8}; //96CC8
s16 droppedBombLine5[8] = {0, 0, 0, 0, 0, 0, 0, 8}; //96CD8
s16 droppedBombLine6[8] = {0, 0, 0, 0, 0, 8, 0, 0}; //96CE8
s16 droppedBombLine7[8] = {8, 7, 8, 8, 8, 8, 1, 8}; //96CF8
s16 droppedBombLine8[8] = {7, 8, 8, 8, 8, 8, 8, 1}; //96D08
s16 droppedBombLine9[8] = {7, 8, 8, 8, 8, 8, 8, 8}; //96D18
s16 droppedBombLineA[8] = {8, 7, 8, 8, 8, 8, 8, 8}; //96D28
s16 droppedBombLineB[8] = {8, 8, 7, 8, 8, 8, 8, 8}; //96D38

s16 *droppedBombSequence1[8] = {droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine1}; //96D48
s16 *droppedBombSequence2[8] = {droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine6}; //96D68
s16 *droppedBombSequence3[8] = {droppedBombLine0, droppedBombLineB, droppedBombLine0, droppedBombLineA, droppedBombLine0, droppedBombLine9, droppedBombLine0, droppedBombLine0}; //96D88
s16 *droppedBombSequence4[8] = {droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine3, droppedBombLine0, droppedBombLine0, droppedBombLine0}; //96DA8
s16 *droppedBombSequence5[8] = {droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine4, droppedBombLine0, droppedBombLine1, droppedBombLine0}; //96DC8
s16 *droppedBombSequence6[8] = {droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine5, droppedBombLine0, droppedBombLine0, droppedBombLine0}; //96DE8
s16 *droppedBombSequence7[8] = {droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine0, droppedBombLine2, droppedBombLine0}; //96E08

//96E28
s16 **bombSequences[7] = {
        droppedBombSequence1, droppedBombSequence2, droppedBombSequence3, droppedBombSequence4,
        droppedBombSequence5, droppedBombSequence6, droppedBombSequence7
};
