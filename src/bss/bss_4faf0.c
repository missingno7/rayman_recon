/* Verified DOS BSS layout candidate. Names and types follow config/symbols.json; see NOTES.md. */
#include "bt.h"
#include "bt_bs_4e43c.h"

bnk_header_t bnkHeaderWorld[128]; /* 2:502f0; size=2048; key=201 */
bnk_header_t bnkHeaderFixe[128]; /* 2:4faf0; size=2048; key=21 */
s32 current_port; /* 2:50af0; size=4; key=142 */
