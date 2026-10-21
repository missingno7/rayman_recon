/* sprite_blocks: worker tu_pcmisc (1:61920 ray.c PC part / 1:6f3b0 sysutils.c, misc_source.c). Names RAYVERSE/ANDROID. */
#include "game.h"

extern u32 OffsetSpriteFixe;
extern u32 OffsetSpriteWorld;

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
