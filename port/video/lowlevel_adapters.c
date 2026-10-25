/*
 * Native views of assembly routines that the historical C reaches through more than one prototype. On DOS the
 * register ABI (#pragma aux) made the views equivalent; in portable C the argument order matters, so each extra
 * view gets an adapter with that view's C signature.
 */
#include "video/lowlevel_normal.h"

#include "rtypes.h"

/* island_copy_t (src/include/type_island_abi.h): ESI source, EDI destination, EDX source pitch, EBX height,
   ECX width - the order map_ngawe.c installs Copy_Plan0Diff_To_Buf under and skops.c calls it with. */
void port_island_copy_plan0diff(u8 *source, u8 *dest, u16 source_pitch, s16 height, s16 width)
{
    Copy_Plan0Diff_To_Buf(source, dest, width, height, source_pitch);
}
