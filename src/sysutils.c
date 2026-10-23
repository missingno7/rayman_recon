/* DOS sysutils candidate, 1:6f3b0..1:6fbe6.
 * Accepted block_malloc/init_memory/sprite_blocks bodies retained unchanged.
 * 19/21 STRICT, full CONST verified; pack_temp_zone and temp_malloc remain NEAR.
 * See NOTES.md for layout conflict, experiments and data-type corrections.
 */
#include "game.h"
#include "obj.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <i86.h>
#include "sysutils_local.h"
#include "inline_helpers.h"

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
u32 init_memory(mem_t *mem, u32 capacity);


extern temp_mem_entry_t temp_mem[30];
extern mem_t temp_mem_buf;
extern dpmi_memory_info_t mem_32190;
extern eta_t **loaded_eta[100];
extern u8 nb_fix_eta, nb_loaded_eta;
extern s16 nb_des, nb_fix_des;
extern u32 OffsetSpriteFixe, OffsetSpriteWorld;
void clear_memory(void *, s32, s32);
void JumelleFreeArray(void **ptr);
void pack_temp_zone(void);
void world_free(void);
void fix_free(void);

u32 get_avail_memory(void)
{
    union REGS regs;
    struct SREGS segments;
    clear_memory(&regs, 0, sizeof(regs));
    clear_memory(&segments, 0, sizeof(segments));
    regs.x.eax = 0x500;
    segments.es = FP_SEG((void __far *)&mem_32190);
    regs.x.edi = (u32)&mem_32190;
    int386x(0x31, &regs, &regs, &segments);
    return mem_32190.largest_free_block;
}

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

void sub_6F740(void)
{
    JumelleFreeArray((void **)&temp_mem_buf.data);
    temp_mem_buf.data = 0;
    clear_memory(temp_mem, 0, sizeof(temp_mem));
}

void init_all_memory(u32 capacity)
{
    s16 i;
    capacity = init_memory(&temp_mem_buf, capacity);
    data_22d970.temp_capacity = capacity;
    temp_mem[0].data = temp_mem_buf.data;
    temp_mem[0].capacity = temp_mem_buf.capacity;
    temp_mem[0].allocated = 0;
    for (i = 1; i < 30; i++) {
        temp_mem[i].data = temp_mem_buf.data + capacity;
        temp_mem[i].capacity = 0;
        temp_mem[i].allocated = 0;
    }
}

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

void pack_temp_zone(void)
{
    s16 i, j;
    u32 merged_size;
    /* COMPILER_STEERED: name the pool end before clearing the free slot. */
    u8 *end;
    for (i = 0; i < 30; i++) {
        if (temp_mem[i].allocated == 0 && temp_mem[i].capacity != 0) {
            do {
                merged_size = 0;
                for (j = 0; j < 30; j++) {
                    if (temp_mem[i].data + temp_mem[i].capacity == temp_mem[j].data &&
                        temp_mem[j].allocated == 0)
                        break;
                }
                if (j < 30) {
                    merged_size = temp_mem[j].capacity;
                    temp_mem[i].capacity += merged_size;
                    end = temp_mem_buf.data + temp_mem_buf.capacity;
                    temp_mem[j].capacity = 0;
                    temp_mem[j].data = end;
                }
            } while (merged_size != 0);
        }
    }
}

void *temp_malloc(u32 size)
{
    s16 i, best;
    u32 smallest;
    u8 *result;
    char message[100];
    best = 30;
    smallest = temp_mem_buf.capacity;
    size += size & 3;
    for (i = 0; i < 30; i++) {
        if (temp_mem[i].allocated == 0 &&
            inrange(temp_mem[i].capacity, size, smallest)) {
            best = i;
            smallest = temp_mem[i].capacity;
        }
    }
    if (best < 30) {
        for (i = 0; i < 30; i++) {
            if (temp_mem[best].data + temp_mem[best].capacity == temp_mem[i].data)
                break;
        }
        if (temp_mem[i].allocated) {
            for (i = 0; i < 30; i++) {
                if (temp_mem_buf.data + temp_mem_buf.capacity == temp_mem[i].data)
                    break;
            }
        }
        temp_mem[i].data = temp_mem[best].data + size;
        temp_mem[i].capacity += temp_mem[best].capacity - size;
        temp_mem[best].capacity = size;
        temp_mem_buf.align += size;
        temp_mem[best].allocated = 1;
        result = temp_mem[best].data;
        if (data_22d970.temp_peak < temp_mem_buf.align)
            data_22d970.temp_peak = temp_mem_buf.align;
    } else {
        result = malloc(size);
        if (data_22d970.temp_capacity + size > data_22d970.temp_peak)
            data_22d970.temp_peak = data_22d970.temp_capacity + size;
        if (result == 0) {
            sprintf(message, "not enough memory in pool.\nMemory error in tmp_malloc : size = %ul.\n", size);
            FatalError(message);
        }
    }
    return result;
}

void temp_free(void *ptr)
{
    s16 i;
    u8 found;
    found = 0;
    for (i = 0; i < 30; i++) {
        if (temp_mem[i].data == ptr) {
            if (temp_mem[i].allocated) {
                found = 1;
                temp_mem[i].allocated = 0;
                temp_mem_buf.align -= temp_mem[i].capacity;
                pack_temp_zone();
            }
            break;
        }
    }
    if (!found)
        JumelleFreeArray(&ptr);
}

void free_main_mem(mem_t *mem)
{
    if (mem == &main_mem_fix)
        fix_free();
    JumelleFreeArray((void **)&mem->data);
    mem->data = 0;
    mem->capacity = 0;
    mem->align = 0;
}

void world_free(void)
{
    s16 i;
    for (i = nb_fix_eta; i < nb_loaded_eta; i++)
        loaded_eta[i] = 0;
    nb_loaded_eta = nb_fix_eta;
    nb_des = nb_fix_des;
}

void SpriteFixeOffset(mem_t *mem)
{
    OffsetSpriteFixe = mem->align + (((u32)(mem->data + mem->align)) & 3);
}

void SpriteWorldOffset(mem_t *mem)
{
    OffsetSpriteWorld = mem->align + (((u32)(mem->data + mem->align)) & 3);
}

void SpriteFixeBlocksFree(mem_t *mem)
{
    mem->align = OffsetSpriteFixe;
}

void SpriteWorldBlocksFree(mem_t *mem)
{
    mem->align = OffsetSpriteWorld;
}

void block_free(mem_t *mem)
{
    world_free();
    mem->align = (u32)mem->data & 3;
}

void align_mem_desc(mem_t *mem)
{
    mem->align = (u32)mem->data & 3;
}

void fix_free(void) {}

void *sub_6FBB4(u32 size)
{
    return malloc(size);
}

void *JumelleCalloc(u32 size, u32 count)
{
    return calloc(count, size);
}

void JumelleFreeArray(void **ptr)
{
    if (*ptr != 0) {
        free(*ptr);
        *ptr = 0;
    }
}


