/* Verified DOS BSS layout candidate. Names and types follow config/symbols.json; see NOTES.md. */
#include "bt.h"

u8 __far * cd_buffer_89c; /* 2:4e89c; size=6; key=64 */
u8 __far * cd_buffer_896; /* 2:4e896; size=6; key=13 */
u16 cd_packet_argument_low; /* 2:4e8a4; size=2; key=44 */
u16 cd_packet_command_word; /* 2:4e8a2; size=2; key=36 */
