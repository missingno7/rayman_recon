/*
 * Frame recorder (port/docs/RENDERER.md): builds the display list of the frame the original code is drawing
 * (render/frame.h), renders it natively at the swap, and optionally checks it against the original buffers.
 * The drawing primitives are observed through link-time wraps (port/CMakeLists.txt); the original passes and
 * semantic layers are reported by render/scene_capture.c, which is compiled with the game.
 */
#ifndef PORT_RENDER_RECORD_H
#define PORT_RENDER_RECORD_H

#include "render/frame.h"

#include <stddef.h>
#include <stdint.h>

/* Passes (enum port_pass): begin returns the enclosing pass, end restores it. */
int port_record_pass_begin(int pass);
void port_record_pass_end(int previous);
/* Scene label for statistics and diagnostics (the PcMain scene function running); returns the previous one. */
const char *port_record_scene(const char *name);
/* The frame is a gameplay frame (the gameplay loop's frame-start pass ran). */
void port_record_gameplay(void);
/* The semantic payload of the frame being recorded, filled by the capture before a layer operation is added. */
port_scene *port_record_world(void);
/* Append a semantic layer operation (PORT_OP_BACKGROUND / MAP / FRONT) for viewport (x0, y0)-(x1, y1). */
void port_record_layer(int kind, int x0, int y0, int x1, int y1);

/* The firefly darkness aff_luciole drew (render/scene_capture.c, after Display_and_free_luciole). */
void port_record_firefly(int center_x, int center_y, int rayon, int addluclip, int w1, int w2, int h1, int h2);

/* ClearBorder (src/box.c) cleared everything outside (x0, y0)-(x1, y1) of the buffer at `target`. */
void port_record_clear_outside(const uint8_t *target, int x0, int y0, int x1, int y1);

/* While on, primitives are not recorded: the caller records one semantic operation for what they draw. */
void port_record_claim(int on);

/* The target (enum port_target) holding address p and its linear offset from the target's origin;
   PORT_TARGET_COUNT when p is not video memory. */
int port_record_target(const void *p, int64_t *offset);
/* Append a screen-effect operation (render/effects.h) with a copy of its payload; 0 if it could not be stored. */
int port_record_effect(int effect, const void *payload, size_t size);

/* An effect pass wrote buffer p but could not be recorded (counted, frame marked incomplete). */
void port_record_unsupported_effect(const void *p);

/* An asset decoder (the movie player) wrote n bytes at buffer: if that is video memory, they become an image
   operation (render/record.c record_asset). */
void port_record_asset(void *buffer, size_t n);

/* Renderer selection: 0 = reference (the original draw path's display), 1 = native (the native renderer's
   display only; nothing of the original buffers is presented). */
void port_record_set_native(int on);
int port_record_native(void);
/* The view of the native renderer: `pad` columns beyond the original 320 on each side (0 = the original view;
   at most PORT_SCENE_MAX_PAD). Set before the game starts. */
void port_record_set_pad(int pad);
int port_record_pad(void);
/* The image to present: the native display (width x 200 pixels, pitch width) in native mode, otherwise NULL
   (present the original display, 320 x 200). */
const uint8_t *port_record_presented(int *width);
/* Verification: render every frame natively and compare it with the original draw buffer and display (full
   frame), and each pass's operations with the original buffer changes over that pass (diagnostics). */
void port_record_set_verify(int on);
int port_record_verify(void);
void port_record_report(void);

/* Called after every frame swap (the simulation-side frame boundary, independent of presentation). */
void port_record_set_swap_hook(void (*hook)(void));

#endif
