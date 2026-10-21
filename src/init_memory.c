/* init_memory: worker tu_pcmisc (1:61920 ray.c PC part / 1:6f3b0 sysutils.c, misc_source.c). Names RAYVERSE/ANDROID. */
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

void *temp_malloc(u32 size);
void temp_free(void *ptr);
void FatalError(const char *message);

u32 init_memory(mem_t *mem, u32 capacity)
{
    char *message;
    u8 *data;

    capacity += capacity & 3;
    mem->data = (u8 *)malloc(capacity);
    if (mem->data == 0) {
        message = (char *)temp_malloc(100);
        sprintf(message, "Can not allocate %d bytes in memory.\n", capacity);
        FatalError(message);
        temp_free(message);
    }
    mem->align = (u32)mem->data & 3;
    mem->capacity = capacity;
    return capacity;
}
