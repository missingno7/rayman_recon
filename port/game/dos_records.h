/* Serialized DOS records -> native structures (see dos_records.c). */
#ifndef PORT_GAME_DOS_RECORDS_H
#define PORT_GAME_DOS_RECORDS_H

#include <stdio.h>

#include "obj.h"

/* The four pointer-shaped words of a serialized instance (+0x00..+0x0c): descriptor/bank indices in their low
   16 bits (FILE_FORMATS.md: the loader narrows them with MOVSX). */
typedef struct port_obj_words_t {
    s32 sprites, animations, img_buffer, eta;
} port_obj_words_t;

void port_decode_obj_record(obj_t *obj, const u8 record[132], port_obj_words_t *words);
/* Read `count` 132-byte records like fread(objects, 132, count, fp); returns the number of complete records. */
size_t port_read_obj_records(obj_t *objects, s16 count, port_obj_words_t *words, FILE *fp);

#endif
