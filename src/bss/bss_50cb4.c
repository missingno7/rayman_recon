/* Verified DOS BSS layout candidate. Names and types follow config/symbols.json; see NOTES.md. */
#include "bt.h"

void (__interrupt __far *saved_keyboard_interrupt_handler)(void); /* 2:50cb4; size=6; key=125 */
