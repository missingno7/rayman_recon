/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

extern u8 arrivalSequence[4];
extern u8 firstEncounterSequence[40];
extern u8 secondEncounterSequence1[13];
extern u8 secondEncounterSequenceSuite[12];
extern u8 secondEncounterSequence2[21];
extern u8 secondEncounterSequence3[19];
extern u8 secondEncounterSequence4[21];
extern u8 hitSequence[6];
extern u8 dyingSequence[5];

u8* moskitoActionSequences[10] = {
        arrivalSequence,                // 0: Bzzit (arrival phase)
        firstEncounterSequence,         // 1: Bzzit fight
        arrivalSequence,                // 2: Moskito (arrival phase)
        secondEncounterSequence1,       // 3: Moskito phase 1
        secondEncounterSequenceSuite,   // 4
        secondEncounterSequence2,       // 5: Moskito phase 2
        secondEncounterSequence3,       // 6: Moskito phase 3
        secondEncounterSequence4,       // 7: Moskito phase 4
        hitSequence,                    // 8: animation sequence after each hit
        dyingSequence,                  // 9: animation sequence when dying
};
