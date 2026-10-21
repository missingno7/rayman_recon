/* Verified BSS layout fragment, GOG v1.21. Names: RAYVERSE. */
/* DOS 2:35568..2:36d88; 2 objects, 6,176 bytes. See NOTES.md and refs.tsv. */
#include "bt.h"

s32 planVigbit_nb_bytes[8]; /* 2:36d68; 8 dword slots from DOS writes. */
rgb_palette_t rvb_Vig[8]; /* 2:35568; DOS index 0..7, stride 0x300. */
