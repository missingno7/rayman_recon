/* Contiguous palette prefix of the requested interval, not whole-TU closure.
 * Names and palette types: RAYVERSE; DOS addresses: external/rayverse/src/data.c.
 * The equal hash of rvb_pres/rvb_save requires rvb_save's later declaration
 * when the two first declarations are in the same index group.
 */
#include "game.h"

rgb_palette_t rvb_pres;
rgb_palette_t rvb_save;
u16 rvb_fade[256 * 3];
rgb_palette_t current_rvb;
rgb_palette_t rvb_plan3;
rgb_palette_t rvb[3];
