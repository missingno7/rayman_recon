/*
 * Render frame: everything the original code drew into its draw buffer between two presentations, as an ordered
 * display list (port/docs/RENDERER.md). The native renderer (render/native.c) draws a frame from this list alone;
 * the original draw buffer is only the reference it is checked against.
 *
 * Two kinds of operations, in the original drawing order:
 *  - semantic layers captured from game state when the original pass starts (background, map, foreground band
 *    sprites): their payload is the frame's port_scene, which a wider viewport can extend;
 *  - primitive operations recorded where the original high-level code (draw order, culling, animation) calls a
 *    drawing primitive: the primitive's arguments in logical screen coordinates, the clip state in force, the
 *    image it reads, and the pass that issued it. The high-level code runs once per frame, as in the original; its
 *    pixels are produced by the native renderer.
 *
 * Plain data without game headers (portable). Image pointers name game-owned resources; a frame is valid until
 * the next frame is recorded (synchronous presentation), see RENDERER.md "Resource lifetimes".
 */
#ifndef PORT_RENDER_FRAME_H
#define PORT_RENDER_FRAME_H

#include "render/scene.h"

#include <stdint.h>

/* Original passes of DO_MAIN_LOOP_PC_NORMAL (src/pcmain.c), in drawing order; LOOP = code of the loop itself. */
enum port_pass {
    PORT_PASS_LOOP,
    PORT_PASS_FRAME_START,     /* CLEAR_FIXE_LUCIOLE: firefly HUD boxes */
    PORT_PASS_BACKGROUND,      /* Display_Back_Screen, or the loop's clear */
    PORT_PASS_REAR_WEATHER,    /* display_flocons_behind */
    PORT_PASS_MAP,             /* DRAW_MAP */
    PORT_PASS_STARS,           /* display_grp_stars */
    PORT_PASS_OBJECTS,         /* DISPLAY_ALL_OBJECTS */
    PORT_PASS_PARTICLES,       /* display_pix_gerbes */
    PORT_PASS_FRONT,           /* Display_Sprite_On_Front */
    PORT_PASS_FRONT_WEATHER,   /* display_flocons_before */
    PORT_PASS_FIREFLY,         /* Display_and_free_luciole */
    PORT_PASS_FAIRY_TEXT,      /* DISPLAY_TEXT_FEE */
    PORT_PASS_HUD,             /* DISPLAY_FIXE */
    PORT_PASS_CRACKERS,        /* DisplayCrackers */
    PORT_PASS_PROGRAMMER,      /* DisplayProgrammerMessage */
    PORT_PASS_TIME,            /* display_time */
    PORT_PASS_VIGNETTE,        /* DISPLAY_GAME_VIGNET */
    PORT_PASS_OPTIONS,         /* DO_OPTIONS_IN_GAME */
    PORT_PASS_PAUSE,           /* Do_Effect_Pause */
    PORT_PASS_KEYS,            /* HANDLE_KEY */
    PORT_PASS_COUNT
};

enum port_op_kind {
    PORT_OP_BACKGROUND,        /* scene background layer (render/world.c, ORIGINAL fidelity) */
    PORT_OP_MAP,               /* scene map layer */
    PORT_OP_FRONT,             /* scene foreground band sprites */
    PORT_OP_SPRITE,            /* DrawSprite*Normal*: marker-run or zero-transparent sprite */
    PORT_OP_CLEAR_BORDERS,     /* clear_borders_Normal */
    PORT_OP_PIXEL,             /* fplot_Normal */
    PORT_OP_WEATHER,           /* draw_flocon* and draw_pluie* stamps */
    PORT_OP_BLACK_BOX,         /* DrawBlackBoxNormal */
    PORT_OP_FILL,              /* memset/fill into a video buffer: w bytes of `color` from linear offset x */
    PORT_OP_COPY,              /* memcpy/copy into a video buffer: w bytes to linear offset x, forward, from `image`
                                  (bytes captured when recorded) or, with flags 1, from linear offset y of the
                                  surface of target `color` */
    PORT_OP_PICTURE,           /* DisplayAnyPictureNormal: w x h bytes from `image` (captured rows) at (x, y) */
    PORT_OP_SWAP,              /* frame swap: rows of w bytes (rounded down to 4) x h from draw (x, y) to display
                                  (clip[0], clip[1]) */
    PORT_OP_BORDER_BOX,        /* DrawBorderBoxNormal: bevel at linear offset x, h rows, w columns, size = colours
                                  (light | dark << 8) */
    PORT_OP_FOND_BOX,          /* DrawFondBoxNormal: +/-5 shading from the diagonal neighbour, linear offset x, h rows,
                                  w columns, flags = type (2 = backwards, lighter) */
    PORT_OP_DEFORM,            /* draw_sprite_deform into native scratch slot `w`: image = source sprite, payload in
                                  clip..: see render/effects.h port_deform */
    PORT_OP_EFFECT,            /* a screen effect's pixel pass (flags = enum port_effect, image = its payload,
                                  render/effects.h) */
    PORT_OP_ORIGIN,            /* the draw buffer's origin moved by x bytes (update_display_map, FIN_GAME_MODE_NORMAL):
                                  pixels keep their memory, so their logical position moves by -x */
    PORT_OP_CLEAR_OUTSIDE,     /* ClearBorder: zero everything outside the rectangle clip (x0, x1, y0, y1) */
    PORT_OP_FIREFLY,           /* aff_luciole: darkness around the lit window (x, y centre; size rayon_luciole;
                                  h ADDLUCLIP; clip = the full viewport Bloc_lim_*_Glob) */
    PORT_OP_UNSUPPORTED,       /* a primitive the native renderer does not draw yet (flags = enum port_prim) */
    PORT_OP_KIND_COUNT
};

/* Video memory the game draws into; each has a native surface. */
enum port_target {
    PORT_TARGET_DRAW,          /* DrawBufferNormal: the frame being built (320-byte rows, origin moves with the camera) */
    PORT_TARGET_DISPLAY,       /* the VGA display (written by swaps, and directly by some scenes) */
    PORT_TARGET_EFFECT,        /* EffetBufferNormal: 2 x 64000-byte off-screen images (menus, pause, world map) */
    PORT_TARGET_COUNT
};

/* Every drawing primitive the recorder observes (names of the original entries). */
enum port_prim {
    PORT_PRIM_SPRITE, PORT_PRIM_SPRITE_DIFF, PORT_PRIM_TILE, PORT_PRIM_PLAN0_COPY, PORT_PRIM_PIXEL,
    PORT_PRIM_WEATHER, PORT_PRIM_BLACK_BOX, PORT_PRIM_FOND_BOX, PORT_PRIM_BORDER_BOX, PORT_PRIM_PICTURE,
    PORT_PRIM_CLEAR_BORDERS, PORT_PRIM_CLEAR_SCREEN, PORT_PRIM_BUFFER_COPY, PORT_PRIM_DEFORM, PORT_PRIM_EFFECT,
    PORT_PRIM_DWORD_COPY, PORT_PRIM_MEMORY, PORT_PRIM_READBACK, PORT_PRIM_COUNT
};

/* PORT_OP_SPRITE variants (the eight DrawSprite*Normal* entries). */
#define PORT_SPRITE_FLIP 1     /* mirrored */
#define PORT_SPRITE_RAW 2      /* zero-transparent 256-colour (loader) instead of marker runs */
#define PORT_SPRITE_COLOR 4    /* visible pixels OR (color * 8) */
#define PORT_SPRITE_NOCLIP 8   /* no clipping */
#define PORT_SPRITE_SCRATCH 16 /* image read from native scratch slot `w` at byte `h` (a deformed sprite) */

typedef struct port_op {
    uint8_t kind, pass;
    uint8_t target;            /* enum port_target */
    uint8_t flags;             /* SPRITE: PORT_SPRITE_*; WEATHER: stamp; UNSUPPORTED: enum port_prim */
    uint8_t color;             /* SPRITE: colour argument; PIXEL: colour */
    int32_t x, y;              /* logical position (draw buffer offsets converted to x, y) */
    uint32_t size;             /* SPRITE: (height << 8) | width as passed */
    int32_t w, h;              /* BLACK_BOX: width, height; CLEAR_BORDERS: right offset, height */
    int32_t clip[4];           /* SPRITE: clip box xmin, xmax, ymin, ymax; WEATHER: top, bottom, left, right */
    const uint8_t *image;      /* SPRITE: image data (game-owned atlas, or bytes captured into the frame) */
    uint32_t check;            /* verification: hash of a game-owned image when recorded (0 = not checked) */
    int32_t base;              /* SPRITE: linear offset of the buffer the drawer was given (0 = the target's origin;
                                  the menus draw into the second half of the effect buffer) */
    int32_t source;            /* identity of the drawn game entity, -1 = not tagged yet */
} port_op;

#define PORT_FRAME_MAX_OPS 8192

typedef struct port_frame {
    uint32_t seq;
    int gameplay;              /* built by the gameplay loop (its frame-start pass ran) */
    int nb_ops, overflow;      /* overflow: operations beyond PORT_FRAME_MAX_OPS were lost */
    int unsupported;           /* operations the native renderer cannot draw yet */
    int display_writes;        /* primitives that wrote the display directly (not via the draw buffer) */
    port_scene world;          /* payload of BACKGROUND / MAP / FRONT */
    port_op ops[PORT_FRAME_MAX_OPS];
} port_frame;

const char *port_pass_name(int pass);
const char *port_prim_name(int prim);
const char *port_op_name(int kind);

#endif
