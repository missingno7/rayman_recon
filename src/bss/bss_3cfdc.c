/* GOG v1.21 BSS fragment 2:3cfdc..3d040 (100 bytes).
 * demo_video_mem is the existing INVENTED DOS label, not an original name.
 * Its true identity is RAYVERSE/Android CheminSauvegarde, char[100].
 * Keep one symbol at this address. The old mem_t annotation was incorrect:
 * DOS init_arg 1:07cc3/07d3c copies/appends a path string; playVideo2
 * 1:2b363..2b3ae copies that string and appends the movie filename.
 * Rayverse data.c:2555 and Android .bss size 100 establish the extent.
 * See symbols.json and NOTES.md; no descriptor or padding is fabricated. */
#include "bt.h"

char demo_video_mem[100];
