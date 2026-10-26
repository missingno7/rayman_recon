/*
 * Presentation viewport (port/docs/RENDERER.md): the original 320x200 image, unchanged, in the centre of a wider
 * indexed frame whose margins are drawn by the semantic renderer from the snapshot of the same game frame.
 */
#ifndef PORT_RENDER_COMPOSE_H
#define PORT_RENDER_COMPOSE_H

#include <stdint.h>

/* Margin per side in logical pixels (0 = the original 320 wide image; at most PORT_SCENE_MAX_PAD). */
void port_render_set_pad(int pad);
int port_render_pad(void);
/* Swap_To_Screen has copied a built gameplay frame to the display: bind the snapshot captured while it was built. */
void port_render_on_swap(const uint8_t *display);
/* The frame to present for this display content: the display itself when there is no margin, otherwise the wide
   frame (320 + 2 * pad columns, 200 rows). Margins stay black unless the display still holds the swapped
   gameplay frame and its snapshot is supported. */
const uint8_t *port_render_compose(const uint8_t *display, int *width);
/* The frame most recently composed (for frame dumps). */
const uint8_t *port_render_last(int *width);
/* Frames whose margins were drawn / left black (statistics for tests). */
void port_render_counts(long *drawn, long *black);

#endif
