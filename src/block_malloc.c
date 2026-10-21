/* block_malloc: worker tu_pcmisc (1:61920 ray.c PC part / 1:6f3b0 sysutils.c, misc_source.c). Names RAYVERSE/ANDROID. */
#include "game.h"
#include <stdio.h>
#include <string.h>

struct mem_peak_t {
    u32 used;
    s16 world;
    s16 level;
};

typedef struct mem_usage_stats_t {
    u32 temp_capacity;
    struct mem_peak_t fix;
    struct mem_peak_t world;
    struct mem_peak_t level;
    struct mem_peak_t sprite;
    u32 temp_peak;
} mem_usage_stats_t;

extern mem_usage_stats_t data_22d970;
extern mem_t main_mem_fix;
extern mem_t main_mem_world;
extern mem_t main_mem_level;
extern mem_t main_mem_sprite;
extern s16 num_world;
extern s16 num_level;

void *temp_malloc(u32 size);
void temp_free(void *ptr);
void FatalError(const char *message);

u8 *block_malloc(mem_t *mem, u32 size)
{
    u32 old_cursor;
    u32 new_cursor;
    u8 *result;
    char mem_name[12];
    char *message;

    size += size & 3;
    new_cursor = mem->align + size;
    if (size == 0 || new_cursor > mem->capacity) {
        message = (char *)temp_malloc(100);
        if (mem == &main_mem_fix)
            memcpy(mem_name, "fix", 4);
        else if (mem == &main_mem_world)
            memcpy(mem_name, "world", 6);
        else if (mem == &main_mem_level)
            memcpy(mem_name, "level", 6);
        else if (mem == &main_mem_sprite)
            memcpy(mem_name, "sprite", 7);
        else
            memcpy(mem_name, "unknown mem", 12);
        sprintf(message, "Memory error in block_malloc : %s = %p, size = %ul.\n", mem_name, mem, size);
        FatalError(message);
        temp_free(message);
        return 0;
    }

    old_cursor = mem->align;
    mem->align = new_cursor;
    result = mem->data + old_cursor;

    if (data_22d970.fix.used < main_mem_fix.align) {
        data_22d970.fix.world = num_world;
        data_22d970.fix.used = main_mem_fix.align;
        data_22d970.fix.level = num_level;
    }
    if (data_22d970.world.used < main_mem_world.align) {
        data_22d970.world.world = num_world;
        data_22d970.world.used = main_mem_world.align;
        data_22d970.world.level = num_level;
    }
    if (data_22d970.level.used < main_mem_level.align) {
        data_22d970.level.world = num_world;
        data_22d970.level.used = main_mem_level.align;
        data_22d970.level.level = num_level;
    }
    if (data_22d970.sprite.used < main_mem_sprite.align) {
        data_22d970.sprite.world = num_world;
        data_22d970.sprite.used = main_mem_sprite.align;
        data_22d970.sprite.level = num_level;
    }
    return result;
}
