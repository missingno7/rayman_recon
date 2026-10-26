/*
 * Semantic world renderer: draws the layers of a render snapshot (render/scene.h) into an indexed surface at
 * any logical rectangle, with the original per-pixel rules (docs/port/RENDERING.md, docs/port/ASM_SPEC.md) but
 * without the original 320-byte pitch, 4-pixel clip granularity or draw-buffer alignment shift. Portable C.
 */
#ifndef PORT_RENDER_WORLD_H
#define PORT_RENDER_WORLD_H

#include "render/scene.h"

typedef struct port_surface {
    uint8_t *pixels;
    int pitch;
    int x0, y0;               /* logical coordinate of pixels[0] */
    int width, height;
    uint8_t *written;         /* optional, same geometry: set to 1 for every pixel a layer wrote */
} port_surface;

typedef struct port_rect { int x0, y0, x1, y1; } port_rect;   /* half-open logical rectangle */

/* Fidelity. ORIGINAL reproduces what the original code draws in the frame's own viewport, including its
   viewport-dependent decisions: exactly the two instances of a repeating background sprite that
   Display_Back_Screen/Display_Sprite_On_Front draw, and no map at all when DRAW_MAP's tile grid is one tile wide
   or high. Used inside the original image and by the exactness gate. EXTENDED draws the same world for any
   rectangle (every sprite instance it meets, every tile): the presentation margins. */
enum port_render_fidelity { PORT_RENDER_ORIGINAL, PORT_RENDER_EXTENDED };

/* Background (bands or clear) then band sprites: the image Display_Back_Screen leaves before DRAW_MAP. */
void port_render_background(const port_scene *s, port_surface *dst, port_rect clip, int fidelity);
/* Map tiles, as DRAW_MAP. */
void port_render_map(const port_scene *s, port_surface *dst, port_rect clip, int fidelity);
/* Foreground background-sprites, as Display_Sprite_On_Front (drawn after the objects). */
void port_render_front(const port_scene *s, port_surface *dst, port_rect clip, int fidelity);

#endif
