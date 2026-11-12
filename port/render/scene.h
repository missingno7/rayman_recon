/*
 * Render snapshot: the semantic scene of one built gameplay frame, captured by reading game state (never by
 * calling game draw functions). Plain data without game headers, so the renderer that consumes it
 * (render/world.c) is portable. Captured in render/scene_capture.c; docs: port/docs/RENDERER.md.
 *
 * Coordinates are the original logical screen space: x 0..319, y 0..199 is the original image; the presentation
 * viewport may extend it to negative x and x >= 320 (and later y). World point (wx, wy) is at
 * (wx - xmap + 8, wy - ymap), the projection of update_display_map/calc_obj_pos.
 */
#ifndef PORT_RENDER_SCENE_H
#define PORT_RENDER_SCENE_H

#include <stdint.h>

#define PORT_SCENE_MAX_PAD 160                 /* widest supported presentation margin per side */
/* How far the wider view can reach beyond the original 320 columns on one side: at a level edge the window stops
   at the map edge and the original area moves off centre, so the far side reaches 2 * pad (render/native.c
   world_window). Captured world data (the map window) covers this reach on both sides. */
#define PORT_SCENE_MAX_REACH (2 * PORT_SCENE_MAX_PAD)
#define PORT_SCENE_TILE_COLS ((320 + 2 * PORT_SCENE_MAX_REACH) / 16 + 2)
#define PORT_SCENE_TILE_ROWS (200 / 16 + 2)
#define PORT_SCENE_MAX_BANDS 300               /* Bande[300] */
#define PORT_SCENE_MAX_BG_SPRITES 20           /* Sprite[20] */

enum port_scene_background {
    PORT_SCENE_BG_CLEAR,      /* background disabled: the game clears the draw buffer to colour 0 */
    PORT_SCENE_BG_BANDS,      /* horizontal parallax bands (Type_Scroll 0/2), Display_Back_Screen */
    PORT_SCENE_BG_COLUMNS     /* vertical bands (Type_Scroll 1): column strips scrolling vertically; the strips
                                 cover the original 320 columns, margins wrap through the doubled image */
};

typedef struct port_scene_band {
    int16_t offset, length;   /* BANDS: rows [offset, offset + length) of the image; COLUMNS: columns */
    int16_t scroll;           /* Bande.field_6: BANDS: image column of screen x 0; COLUMNS: image row of screen y 0 */
    int16_t cam_mul, cam_div; /* the part of scroll that follows the camera's x: floor(xmap * cam_mul / cam_div)
                                 (Calcul_Deplacement_Bande); cam_mul 0 = none (autoscroll only, or a vertical level) */
} port_scene_band;

typedef struct port_scene_bg_sprite {
    int16_t x, y;             /* Sprite[i].x/y: background-image coordinates */
    int16_t band;             /* scrolls with this band; a zero-length band marks a foreground sprite */
    uint8_t width, height;    /* outer size; drawn in 4-pixel units with a byte mask (DrawSpriteDiffNormal) */
    const uint8_t *image, *mask;
} port_scene_bg_sprite;

typedef struct port_scene_tile {
    uint8_t transparency;     /* 0 none, 1 opaque, 2 masked (row-pair flags at +512, AND mask at +256) */
    const uint8_t *texture;
} port_scene_tile;

typedef struct port_scene {
    uint32_t seq;             /* capture counter; 0 = never captured */
    int16_t xmap, ymap;       /* logical camera */
    int32_t map_width;        /* the level map's width in pixels (16 x tiles); 0 = unknown */
    int16_t view_x0, view_x1, view_y0, view_y1;   /* Bloc_lim_W1/W2/H1/H2 of this frame */
    /* background */
    int background;           /* enum port_scene_background */
    const uint8_t *plan0;     /* background image, plan_width bytes per row (two scroll periods wide) */
    int plan_width, plan_height;
    int16_t bg_shift;         /* source row of screen y 0 (Display_Back_Screen's vertical offset) */
    int16_t bg_top;           /* first background row the game draws (curtain levels raise it) */
    int16_t bg_x0, bg_x1;     /* BANDS: columns the original band copy covers in the frame's viewport */
    int32_t bg_col_shift;     /* COLUMNS: screen column x shows image column x - bg_col_shift (0 in the original
                                 view; the wider view's presentation offset, render/native.c) */
    int front;                /* foreground band sprites (Display_Sprite_On_Front): BANDS or COLUMNS by the level's
                                 scroll type, drawn whether or not the background is enabled */
    int16_t front_y;          /* foreground sprites: bottom edge y (Display_Sprite_On_Front) */
    int32_t sprite_clip[4];   /* sprite clip box (xmin, xmax, ymin, ymax) the band sprites were drawn with */
    int nb_bands, nb_sprites;
    port_scene_band bands[PORT_SCENE_MAX_BANDS];
    port_scene_bg_sprite sprites[PORT_SCENE_MAX_BG_SPRITES];
    /* map window: tile (col0 + c, row0 + r); outside the map transparency is 0 */
    int16_t tile_col0, tile_row0;
    uint8_t original_map_grid_w, original_map_grid_h;   /* DRAW_MAP's display_map grid for the frame's viewport */
    port_scene_tile tiles[PORT_SCENE_TILE_ROWS][PORT_SCENE_TILE_COLS];
    /* firefly levels (RayEvts.firefly): the lit box the game clips this frame's world drawing to is
       [lit_x0, lit_x1] (set_luciole: centre +- rayon_luciole) cut to the full viewport [glob_x0, glob_x1]
       (Bloc_lim_*_Glob, src/luciole.c) */
    uint8_t firefly;
    int32_t lit_x0, lit_x1, glob_x0, glob_x1;
    /* the boss bar (display_bar_boss, src/affiche.c 547, drawn inside DISPLAY_ALL_OBJECTS): the images of its
       sprites (the fist object's 60 and 61 and the boss icons) and the rows it draws them at */
    const uint8_t *bossbar_images[9];
    int nb_bossbar_images;
    int32_t bossbar_y[3];
} port_scene;

#endif
