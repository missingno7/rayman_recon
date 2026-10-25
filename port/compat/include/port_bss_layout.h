/*
 * Pre-included into the src/bss data units. src/bss/bt.h opens `#pragma pack(4)` (never closed) before it includes
 * the shared headers, so that Watcom places the central BSS with natural (<= 4) alignment, as the original compiler
 * did. Natively the shared structures hold 8-byte pointers: under pack(4) the data units would define, e.g., mem_t
 * with 16 bytes while every code unit uses its natural 24-byte layout (the arena capacity then aliased the next
 * global). Including the shared headers first gives every unit the same natural layout; bt.h's own includes become
 * no-ops. bt.h's local types keep its pragma (the only pointer-holding one, temp_mem_entry_t, is declared with the
 * same packing in sysutils_local.h).
 */
#ifndef PORT_BSS_LAYOUT_H
#define PORT_BSS_LAYOUT_H
#include "obj.h"
#include "game.h"
#include "map.h"
#endif
