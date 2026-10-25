/*
 * Port services used by the derivatives in port/derive (each use is marked PORT in the derived source and listed
 * in port/derive/provenance.json).
 */
#ifndef PORT_DERIVED_H
#define PORT_DERIVED_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Free extended memory the port's machine reports through DPMI 0500h (16 MiB). */
#define PORT_FREE_MEMORY 0x1000000u

/* Native records holding pointers are larger than their DOS images; the game's block arenas grow accordingly. */
#define PORT_ARENA_SCALE (sizeof(void *) > 4 ? 2u : 1u)

/* A 32-bit file word stored into a pointer-typed field, as the DOS loader did with fread(&field, 4, 1, fp):
   the word is sign-extended so that the sentinel -1 still compares equal to (T *)-1, and the historical code
   narrows it back with (s32)field before using it as an index. */
static inline size_t port_fread_pointer_word(void *pointer_field, FILE *fp)
{
    int32_t word = 0;
    intptr_t value;
    size_t n = fread(&word, 4, 1, fp);
    value = (intptr_t)word;
    memcpy(pointer_field, &value, sizeof value);
    return n;
}

/* Low byte of the BIOS time-of-day tick count (DWORD at linear 0x46C, 18.2065 Hz), from the port clock. */
uint8_t port_bios_tick_byte(void);

#endif
