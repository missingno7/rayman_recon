/*
 * Frame recorder (port/docs/RENDERER.md): builds the display list of the frame the original code is drawing
 * (render/frame.h), renders it natively at the swap, and optionally checks it against the original buffers.
 * The drawing primitives are observed through link-time wraps (port/CMakeLists.txt); the original passes and
 * semantic layers are reported by render/scene_capture.c, which is compiled with the game.
 */
#ifndef PORT_RENDER_RECORD_H
#define PORT_RENDER_RECORD_H

#include "render/frame.h"

#include <stdint.h>

/* Passes (enum port_pass): begin returns the enclosing pass, end restores it. */
int port_record_pass_begin(int pass);
void port_record_pass_end(int previous);
/* The frame is a gameplay frame (the gameplay loop's frame-start pass ran). */
void port_record_gameplay(void);
/* The semantic payload of the frame being recorded, filled by the capture before a layer operation is added. */
port_scene *port_record_world(void);
/* Append a semantic layer operation (PORT_OP_CLEAR / BACKGROUND / MAP / FRONT) for viewport (x0, y0)-(x1, y1). */
void port_record_layer(int kind, int x0, int y0, int x1, int y1);

/* The firefly darkness aff_luciole drew (render/scene_capture.c, after Display_and_free_luciole). */
void port_record_firefly(int center_x, int center_y, int rayon, int addluclip, int w1, int w2, int h1, int h2);

/* ClearBorder (src/box.c) cleared everything outside (x0, y0)-(x1, y1) of the buffer at `target`. */
void port_record_clear_outside(const uint8_t *target, int x0, int y0, int x1, int y1);

/* Renderer selection: 0 = reference (the original draw path's display), 1 = native (the native renderer's
   display only; nothing of the original buffers is presented). */
void port_record_set_native(int on);
int port_record_native(void);
/* The image to present: the native display in native mode, otherwise NULL (present the original display). */
const uint8_t *port_record_presented(void);
/* Verification: render every frame natively and compare it with the original draw buffer and display (full
   frame), and each pass's operations with the original buffer changes over that pass (diagnostics). */
void port_record_set_verify(int on);
void port_record_report(void);

/* Called after every frame swap (the simulation-side frame boundary, independent of presentation). */
void port_record_set_swap_hook(void (*hook)(void));

/* The world payload of a gameplay frame swapped since the last call, or NULL (legacy margin compositor). */
const port_scene *port_scene_take_pending(void);

#endif
