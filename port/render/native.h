/*
 * Native renderer: draws a recorded frame (render/frame.h) into an indexed surface (render/world.h) with the
 * original per-pixel rules of each operation (docs/port/ASM_SPEC.md), without the original draw buffer. Portable C.
 */
#ifndef PORT_RENDER_NATIVE_H
#define PORT_RENDER_NATIVE_H

#include "render/frame.h"
#include "render/world.h"

/* Can the native renderer draw this operation? */
int port_native_supports(const port_op *op);
/* Draw operations [first, last) of the frame, in order, onto the surface of each operation's target
   (surfaces[PORT_TARGET_COUNT]; NULL skips). Logical coordinates; the surfaces clip.

   pad = 0 draws the original 320x200 view. pad > 0 draws a view `pad` columns wider on each side (the surfaces must
   cover x -pad .. 320 + pad): in gameplay frames, every bound of a draw-buffer operation that is the original full
   viewport widens with the view (sprite clip box, weather bounds, firefly darkness, the world layers, the swap),
   the world layers use their extended rules, and the original's 4-pixel border clear is not drawn. Screen-space
   scenes (menus, movies, transitions) keep the 320 columns, centred, with black margins (port/docs/RENDERER.md 9). */
void port_native_draw(const port_frame *f, int first, int last, port_surface *const *surfaces, int pad);

#endif
