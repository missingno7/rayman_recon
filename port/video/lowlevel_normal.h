#ifndef PORT_VIDEO_LOWLEVEL_NORMAL_H
#define PORT_VIDEO_LOWLEVEL_NORMAL_H
#include <stddef.h>
#include <stdint.h>

/* Contracts: docs/port/ASM_SPEC.md, sections named after each DOS entry. */
/* 1:04ab7: copy 64000 bytes, forward, between disjoint buffers. */
void DisplayBufferModeNormal(uint8_t *source, uint8_t *dest);
/* 1:04ac1: draw the four-row world-map point at pitch 384. */
void DrawWldPointPlan2Normal(uint8_t *buffer, int32_t x, int32_t y);
/* 1:04aef: clear an unclipped positive rectangle at pitch 320. */
void DrawBlackBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width);
/* 1:04b2c: diagonal +/-5 palette shading with XOR bank crossing. */
void DrawFondBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, int32_t type);
/* 1:04bbb: beveled border, including its extra shadow row and column. */
void DrawBorderBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, uint16_t colors);
/* 1:04c19: copy a positive rectangle, source pitch stride, destination pitch 320. */
void DisplayAnyPictureNormal(uint8_t *source, uint8_t *dest, int32_t sx, int32_t sy,
                             int32_t dx, int32_t dy, int32_t stride, int32_t width, int32_t height);
/* 1:04c6b: zero both 64000-byte buffers. */
void ClearDrawAndDisplayBufferNormal(uint8_t *draw, uint8_t *display);
/* 1:04c8e: zero exactly 64000 bytes. */
void N_CLRSCR(uint8_t *buffer);
/* 1:04c9c: copy width rounded down to four, with configured pitches; notify video. */
void Swap_To_Screen(uint8_t *source, uint8_t *dest, int32_t width, int32_t height);
extern void port_vga_display_changed(void);
/* 1:04cc7: clear four-byte borders on height rounded down to two rows. */
void clear_borders_Normal(uint8_t *dest, int32_t height, int32_t right);
/* 1:04cfa: positive rectangle copy, width rounded up to eight. */
void Copy_Plan0Diff_To_Buf(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t source_pitch);
/* 1:04d55: same footprint, in 8x8 groups; positive height must be a multiple of eight. */
void asm_04d55(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t source_pitch);
/* 1:04e64: masked 516-byte tile, two row flag bits, mutable destination pitch. */
void Display_Bloc(uint8_t *source, uint8_t *dest);
/* 1:05041: opaque 16x16 tile, fixed destination pitch 320. */
void Display_Bloc_Plein(uint8_t *source, uint8_t *dest);
/* 1:053c6: masked tile with dword-rounded viewport clipping. */
void Display_Bloc_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y);
/* 1:0555e: opaque tile with dword-rounded viewport clipping. */
void Display_Bloc_Plein_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y);
/* Shared game-owned viewport (not the independent sprite clip box). */
extern int32_t Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2;
/* 1:04c86/04c8a data: first pitch also drives masked/clipped tiles. */
void lowlevel_normal_set_strides(uint32_t source_pitch, uint32_t display_pitch);
/* 1:05609: store the low color byte at (x,y), without clipping. */
void fplot_Normal(uint8_t *dest, int32_t x, int32_t y, int32_t color);
/* 1:05628: set inclusive anchor bounds, without sorting or clamping. */
void flocon_clipping(int32_t top, int32_t bottom, int32_t left, int32_t right);
/* Weather entries admit the anchor only; the whole stamp footprint must be mapped. */
void draw_flocon1_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:05670: 7. */
void draw_flocon2_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:056a0: 2x2. */
void draw_flocon3_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:056dc: 3x3. */
void draw_flocon4_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:05725: 3. */
void draw_flocon5_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:05640: 5. */
void draw_flocon6_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:0587d: 6x6. */
void draw_flocon7_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:057df: 4x4. */
void draw_normal_5755(uint8_t *dest, int32_t x, int32_t y); /* 1:05755: 4x4. */
void draw_pluie5_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:057a6: 2x2. */
void draw_pluie6_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:05830: 3x3. */
void draw_pluie7_Normal(uint8_t *dest, int32_t x, int32_t y); /* 1:058f4: 4x4. */
/* 1:06194: set half-open sprite box [8,312)x[0,200). */
void default_sprite_clipping(void);
/* 1:061bd: sequentially copy the four bounds through output pointers. */
void get_sprite_clipping(int32_t *xmin, int32_t *xmax, int32_t *ymin, int32_t *ymax);
int32_t get_sprite_clipping_xmin(void); /* 1:061e2: return xmin. */
int32_t get_sprite_clipping_xmax(void); /* 1:061e8: return xmax. */
int32_t get_sprite_clipping_ymin(void); /* 1:061ee: return ymin. */
int32_t get_sprite_clipping_ymax(void); /* 1:061f4: return ymax. */
/* 1:061fa: independently clamp bounds to screen limits; do not sort. */
void sprite_clipping(int32_t xmin, int32_t xmax, int32_t ymin, int32_t ymax);
/* 1:0626d/06323: internal helper state; return 0 on rejection, retain partial crop. */
typedef struct lowlevel_sprite_clip {
    int32_t x, y;
    uint32_t size;
    uint8_t *source;
} lowlevel_sprite_clip;
int clip_sprite_on_screen(lowlevel_sprite_clip *sprite);
int clip_sprite_on_screen_flipped(lowlevel_sprite_clip *sprite);
/* Six-argument island_draw_t ABI; size is CL width / CH height, upper bits ignored. */
void DrawSpriteNormalNoClip(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06a18: marker runs, no clip. */
void DrawSpriteNormal(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06a24: marker runs. */
void DrawSpriteNormal256(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06a9d: zero transparent. */
void DrawSpriteFlipNormalNoClip(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06afc: mirror, no clip. */
void DrawSpriteFlipNormal(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06b08: mirror runs. */
void DrawSpriteFlipNormal256(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06b88: mirror, zero transparent. */
void DrawSpriteColorNormal(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06bea: runs, OR color*8. */
void DrawSpriteColorFlipNormal(int32_t x, int32_t color, int32_t y, uint32_t size, uint8_t *dest, uint8_t *source); /* 1:06c89: mirror/color. */
/* 1:06e0d: dword mask compositor with original asymmetric cropping and rounded fringe. */
void DrawSpriteDiffNormal(int16_t x, int16_t y, uint16_t size, uint8_t *mask, uint8_t *dest, uint8_t *source);
/* 1:02737: forward sequential dword copy (overlap is intentional, not memmove). */
void copy_dwords(uint32_t *source, uint32_t *dest, int32_t count);
/* Explicit DF adapter for testing/clients needing the DOS reverse traversal. */
void lowlevel_copy_dwords_direction(uint8_t *source, uint8_t *dest, uint32_t count, int direction);
/* 1:04828: descriptor from action_local.h; sentinel and integer stepping retained. */
typedef struct deform_blit_t {
    int32_t x_step_1, row_step_1, dx_1, dy_1;
    int32_t x_step_2, row_step_2, dx_2, dy_2;
    int32_t dest_width, dest_height, dest_offset, source_width, source_height;
} deform_blit_t;
void draw_sprite_deform(uint8_t *source, uint8_t *dest, deform_blit_t *geometry);
/* 1:04a83: standalone packed-to-four-RAM-plane transpose, not a deform helper. */
void local_04a83(uint8_t *source, uint8_t *dest);
/* 1:00010: reset 200 raster row bounds and 80 span displacements; retain UV bytes. */
void effect_reset_geometry(void);
/* Native subrecord: pointer grows with host ABI; vertex words retain signed 32 bits. */
typedef struct lowlevel_effect_descriptor {
    uint8_t *screen;
    int32_t vertices[8];
    uint8_t rectangle[8];
} lowlevel_effect_descriptor;
/* 1:00045: inclusive affine quad rasterizer, pitch-256 texture, pitch-320 destination. */
void draw_effect_segment(uint8_t *dest, uint8_t *descriptor);

/* Small test/state accessors; region IDs and DOS mapping are in tests/test_lowlevel_native.py.
 * kind 0 = byte array, 1 = native 32-bit words, 2 = native pointer. No DOS addresses in C. */
void *lowlevel_normal_state_region(unsigned id, size_t *size, unsigned *kind);
unsigned lowlevel_normal_state_region_count(void);
#endif
