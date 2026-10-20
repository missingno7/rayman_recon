/*
 * Pseudo-random table (GOG v1.21 obj1 0x5bf60-0x5bfcf, one complete object: _TEXT 0x6f bytes,
 * paragraph aligned). File name invented (Rayverse groups these two functions in rand.c).
 * Function names: Rayverse/Android symbols; global names: rayman-ps1-decomp.
 * The PC myRand differs from the PS1 one (PS1 accumulates PS1_RandSum); this follows the DOS binary.
 */
#include <stdlib.h>
#include <time.h>

extern short RandArray[256];
int RandomIndex;          /* defined here: alone in its own _BSS object at 2:5548c (BSS link order) */

short myRand(short max)
{
    if (++RandomIndex >= 256)
        RandomIndex = 0;
    return RandArray[RandomIndex] % (max + 1);
}

void MakeMyRand(int set_seed)
{
    int i;

    if (set_seed)
        srand(time(NULL));
    for (i = 0; i < 256; i++)
        RandArray[i] = rand() & 0xEFFF;
    RandomIndex = 0;
}
