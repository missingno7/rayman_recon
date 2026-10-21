/* PrintMemInfo + block_malloc, DOS 1:6f410..1:6f73f.
 * Both functions STRICT; CONST 833 bytes VERIFIED at 2:31434.
 * Extension of src/block_malloc.c; see worker NOTES.md. */
#include "game.h"
#include <stdio.h>
#include <string.h>

struct mem_peak_t {
    u32 used;
    u16 world;
    u16 level;
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

u32 get_avail_memory(void);

/* DOS 1:6f410; original name ANDROID, typed fields proven by word/dword loads. */
void PrintMemInfo(void)
{
    printf("Information sur le bloc_malloc.\n");
    printf("   - Taille allou\202e fixes                           : %u\n", main_mem_fix.capacity);
    printf("   - Taille maximum utilis\202e fixes   (w=%3d, l=%3d) : %u (%u*1024L)\n", data_22d970.fix.world, data_22d970.fix.level,
           data_22d970.fix.used, (data_22d970.fix.used >> 10) + 2);
    printf("   - Taille allou\202e mondes                          : %u\n", main_mem_world.capacity);
    printf("   - Taille maximum utilis\202e mondes  (w=%3d, l=%3d) : %u (%u*1024L)\n", data_22d970.world.world, data_22d970.world.level,
           data_22d970.world.used, (data_22d970.world.used >> 10) + 2);
    printf("   - Taille allou\202e maps                            : %u\n", main_mem_level.capacity);
    printf("   - Taille maximum utilis\202e maps    (w=%3d, l=%3d) : %u (%u*1024L)\n", data_22d970.level.world, data_22d970.level.level,
           data_22d970.level.used, (data_22d970.level.used >> 10) + 2);
    printf("   - Taille allou\202e sprites                         : %u\n", main_mem_sprite.capacity);
    printf("   - Taille maximum utilis\202e sprites (w=%3d, l=%3d) : %u (%u*1024L)\n", data_22d970.sprite.world, data_22d970.sprite.level,
           data_22d970.sprite.used, (data_22d970.sprite.used >> 10) + 2);
    printf("\n");
    printf("Information sur le temp_malloc.\n");
    printf("   - Taille de la m\202moire allou\202e            : %u\n", data_22d970.temp_capacity);
    printf("   - Taille maximum utilis\202e                 : %u (%u*1024L)\n", data_22d970.temp_peak, (data_22d970.temp_peak >> 10) + 2);
    printf("\n");
    printf("Left memory = %d\n", get_avail_memory());
}

u8 *block_malloc(mem_t *mem, u32 size)
{
    u32 new_cursor;
    u8 *result;
    char mem_name[12];
    char *message;

    size += size & 3;
    if (size == 0 || (new_cursor = mem->align + size) > mem->capacity) {
        message = (char *)temp_malloc(100);
        if (mem == &main_mem_fix)
            strcpy(mem_name, "fix");
        else if (mem == &main_mem_world)
            strcpy(mem_name, "world");
        else if (mem == &main_mem_level)
            strcpy(mem_name, "level");
        else if (mem == &main_mem_sprite)
            strcpy(mem_name, "sprite");
        else
            strcpy(mem_name, "unknown mem");
        sprintf(message, "Memory error in block_malloc : %s = %p, size = %ul.\n", mem_name, mem, size);
        FatalError(message);
        temp_free(message);
        return 0;
    }

    result = mem->data;
    result += mem->align;
    mem->align = new_cursor;

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
