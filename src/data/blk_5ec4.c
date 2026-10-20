/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "game.h"


s16 niveau = 0; //95EC4

//95EC6
bb_attack_entry_t SerieAtakBB[6][7] = {
        {{0, 2}, {255, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        {{0, 2}, {1, 3}, {255, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        {{6, 2}, {2, 2}, {0, 1}, {1, 1}, {0, 0}, {255, 0}, {0, 0}},
        {{3, 0}, {0, 1}, {1, 1}, {255, 0}, {0, 0}, {0, 0}, {0, 0}},
        {{5, 1}, {2, 3}, {0, 0}, {3, 1}, {4, 0}, {255, 0}, {0, 0}},
        {{7, 0}, {3, 0}, {6, 0}, {3, 0}, {0, 1}, {4, 0}, {255, 0}},
};

