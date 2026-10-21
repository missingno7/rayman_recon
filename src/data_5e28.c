/* GOG v1.21 _DATA 2:05e28. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "rtypes.h"


/* Physical VGA aperture; oracle contains 0x000a0000 with NO fixup. */
u8 *DisplayBufferNormal = (u8 *)0x000a0000UL;
