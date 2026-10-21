/* Verified DOS BSS layout candidate. Names and types follow config/symbols.json; see NOTES.md. */
#include "bt.h"

u8 Touche_Enfoncee[128]; /* 2:50c30; size=128; key=58 */
void (*Read_Input_Func)(void); /* 2:50cb0; size=4; key=107 */
