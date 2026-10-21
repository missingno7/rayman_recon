/* Verified DOS BSS layout candidate. Names and types follow config/symbols.json; see NOTES.md. */
#include "bt.h"

void (__interrupt __far *old_timer)(void); /* 2:4f7cc; size=6; key=65 */
