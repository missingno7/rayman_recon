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
/* Draw operations [first, last) of the frame, in order, onto the draw-buffer surface or the display surface
   named by each operation's target (display may be NULL). Logical coordinates; the surfaces clip. */
void port_native_draw(const port_frame *f, int first, int last, port_surface *draw, port_surface *display);

#endif
