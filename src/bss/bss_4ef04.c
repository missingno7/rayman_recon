/* Verified DOS BSS layout candidate. Names and types follow config/symbols.json; see NOTES.md. */
#include "bt.h"

u8 sav_window[8]; /* 2:4f704; size=8; key=152 */
u8 sav_big_window[256][8]; /* 2:4ef04; size=2048; key=146 */
u8 sav_checksum; /* 2:4f70c; size=1; key=133 */
