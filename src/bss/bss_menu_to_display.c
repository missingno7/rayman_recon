/* DOS menu records 2:3bd08..3c4a0; layout fragment, not original TU.
 * Nine selectable rows (key screen 0..8). The four supposed item-9
 * header stores in INIT_AFFICHE_ECRAN_OPTIONS initialize text_to_display[0],
 * the option-value scratch record consumed by AFFICHE_ECRAN_OPTIONS.
 * All four stores remain in the consuming source. See id_conflicts/NOTES.md.
 * Names RAYVERSE; DOS count correction from access and screen-bound audit.
 */
#include "game.h"
display_item_t menu_to_display[9];
