/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

// The DOS object contains the 129-value table; Rayverse adds a trailing zero as element 130.
s16 costab[129] = {
        512, 512, 512, 512, 512, 512, 511, 511, 510, 509, 509, 508,
        507, 506, 505, 504, 503, 501, 500, 499, 497, 496, 494, 492,
        490, 489, 487, 485, 483, 480, 478, 476, 474, 471, 469, 466,
        463, 461, 458, 455, 452, 449, 446, 443, 440, 436, 433, 430,
        426, 423, 419, 415, 412, 408, 404, 400, 396, 392, 388, 384,
        380, 376, 371, 367, 363, 358, 354, 349, 344, 340, 335, 330,
        325, 320, 316, 311, 306, 300, 295, 290, 285, 280, 274, 269,
        264, 258, 253, 247, 242, 236, 231, 225, 219, 214, 208, 202,
        196, 191, 185, 179, 173, 167, 161, 155, 149, 143, 137, 131,
        125, 119, 113, 107, 100, 94, 88, 82, 76, 69, 63, 57,
        51, 44, 38, 32, 26, 19, 13, 7, 1
};
