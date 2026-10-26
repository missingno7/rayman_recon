/*
 * Screen effects as native operations (port/docs/RENDERER.md "Screen effects"). The original effect code keeps its
 * state and runs once per frame; the capture (render/effect_capture.c) records the inputs of each pixel pass in one
 * of the payloads below, and render/effects.c reproduces the pass on the native surfaces. Portable C.
 */
#ifndef PORT_RENDER_EFFECTS_H
#define PORT_RENDER_EFFECTS_H

#include "render/frame.h"
#include "render/world.h"

#include <stdint.h>

/* PORT_OP_EFFECT flags */
enum port_effect {
    PORT_EFFECT_CURL = 1,      /* effect633e0.c effect_helper_63838: page curl (column remap through an arc table) */
    PORT_EFFECT_SEGMENT,       /* draw_effect_segment (ASM_SPEC 1:00045): textured affine quad, pitch-256 texture */
    PORT_EFFECT_PLASMA,        /* box.c Plasma (through PlasmaBox): the menus' animated plasma boxes */
    PORT_EFFECT_BINOCULARS,    /* jumelle.c DisplayJumellesNormal: the world map's two-circle view */
    PORT_EFFECT_LENS,          /* effect6c3b0.c effect_helper_6c9bc: moving lens (radial displacement map) */
    PORT_EFFECT_GREY,          /* box.c CaptureVideo, menu.c FonduOption: (r + g + b) / 3 + 128 through a palette */
    PORT_EFFECT_FRAME,         /* box.c CadreTrans: translucent frame (dword add 0x40404040, or 0xc8c8c8c8 on 0) */
    PORT_EFFECT_ZOOM_IN,       /* menu.c apparition_prg: the menu picture scaled in from the centre */
    PORT_EFFECT_BLEND,         /* menu.c fondu_prg: weighted fade of the draw buffer towards the menu picture */
    PORT_EFFECT_DISSOLVE,      /* menu.c FonduPixel_prg: every n-th pixel replaced by the menu picture */
    PORT_EFFECT_COPY_RECT,     /* frame.c DISPLAY_FOND_MENU (in game): the viewport of the menu background */
    PORT_EFFECT_FIRE,          /* box.c Fire: the rows of the fire field (message boxes, credits) */
    PORT_EFFECT_COUNT
};

/* A buffer of the game as seen by the native renderer: a target and a linear offset from its origin. */
typedef struct port_effect_buffer {
    int32_t target;            /* enum port_target */
    int32_t pad;
    int64_t offset;
} port_effect_buffer;

/* effect_helper_63838's inputs when it is called (state before its update). */
typedef struct port_effect_curl {
    int32_t mode, initialized, delta, field_0c, field_10, field_14, width;
    port_effect_buffer source, destination;
    int32_t table[80];         /* the arc table, 2 * width entries (width <= 40) */
} port_effect_curl;

/* draw_effect_segment's descriptor: the texture (a buffer of the game, 64 KiB addressed by a 16-bit index), the four
   vertices (x, y) and their (u, v) bytes. */
typedef struct port_effect_segment {
    port_effect_buffer texture, destination;
    int32_t vertices[8];
    uint8_t uv[8];
} port_effect_segment;

/* Plasma's arguments: the box, the four (x, y) phases, the bias, the palette base and the 128 x 128 distance table. */
typedef struct port_effect_plasma {
    port_effect_buffer destination;  /* draw_buffer */
    int32_t x, y, width, height;
    int32_t phase_x[4], phase_y[4], bias, color_index;
    uint8_t table[128 * 128];
} port_effect_plasma;

/* DisplayJumellesNormal's inputs; the zoom table (29025 offsets) only when zoom is active. */
typedef struct port_effect_binoculars {
    port_effect_buffer effect, draw;
    int32_t x, y, radius, circle2_offset, x_min, y_min, zoom;
    int32_t zoom_table[29025];
} port_effect_binoculars;

/* effect_helper_6c9bc's inputs: the lens square and its displacement map (width x height (dx, dy) byte pairs). */
typedef struct port_effect_lens {
    port_effect_buffer screen, output;
    int32_t x, y, radius, width, height;
    int8_t map[2 * 256 * 256];
} port_effect_lens;

/* The greyscale remap: width x height bytes (rows 320 apart) from source to destination (in place when equal). */
typedef struct port_effect_grey {
    port_effect_buffer source, destination;
    int32_t width, height;
    uint8_t palette[768];
} port_effect_grey;

/* A rectangle copy: width x height bytes, rows 320 apart, from source to destination. */
typedef struct port_effect_rect {
    port_effect_buffer source, destination;
    int32_t width, height;
} port_effect_rect;

/* Fire's output: width x height bytes of the fire field (its 16-bit cells narrowed as the original stores them). */
typedef struct port_effect_fire {
    port_effect_buffer destination;
    int32_t width, height;
    uint8_t field[70 * 300];
} port_effect_fire;

/* CadreTrans's rectangle after its alignment: x, y, width (bytes, multiple of 4), height. */
typedef struct port_effect_frame {
    port_effect_buffer destination;
    int32_t x, y, width, height;
} port_effect_frame;

/* The menu fades (menu.c): the rectangle (xFondu, yFondu, wFondu, hFondu), the step before the callback updated
   it, the viewport the zoom-in tests against, and the two buffers (draw buffer, menu picture). */
typedef struct port_effect_fade {
    port_effect_buffer draw, picture;
    int32_t x, y, width, height, step;
    int32_t view_x0, view_x1, view_y0, view_y1;
} port_effect_fade;

/* Native scratch buffers: memory the game uses as an intermediate image (the deformed-sprite buffer). A slot holds
   the native renderer's version of that buffer. */
#define PORT_SCRATCH_SLOTS 4
#define PORT_SCRATCH_BYTES 65536
uint8_t *port_native_scratch(int slot);

/* draw_sprite_deform's inputs: the 13-word geometry (action_local.h deform_blit_t) and the source sprite. */
typedef struct port_deform {
    int32_t geometry[13];
    const uint8_t *source;
    int32_t slot;
} port_deform;

void port_native_deform(const port_deform *e);

/* Draw one PORT_OP_EFFECT operation onto the surfaces of its buffers. */
void port_native_effect(const port_op *op, port_surface *const *surfaces);

#endif
