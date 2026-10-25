/*
 * Boundary entries the port does not provide (yet). Each stops the program with its name, so an unported path
 * can never run silently. port/docs/BOUNDARY.md says why each is absent and what replaces it.
 */
#include "compat/src/pc_hardware.h"

#define ABSENT(name, why) \
    void name(void) { port_unimplemented(#name ": " why); }

/* Mode X (planar) presentation: the port presents normal mode only. */
ABSENT(DrawSpriteX, "Mode X sprite")
ABSENT(DrawSpriteFlipX, "Mode X sprite")
ABSENT(DrawSpriteNoClipX, "Mode X sprite")
ABSENT(DrawSpriteFlipNoClipX, "Mode X sprite")
ABSENT(DrawSpriteColorX, "Mode X sprite")
ABSENT(DrawSpriteColorFlipX, "Mode X sprite")
ABSENT(fplot_mode_x_2b86, "Mode X plot")
ABSENT(mode_x_effect_2b3d, "Mode X weather")
ABSENT(mode_x_effect_2bb7, "Mode X weather")
ABSENT(mode_x_effect_2c00, "Mode X weather")
ABSENT(mode_x_effect_2c59, "Mode X weather")
ABSENT(mode_x_effect_2cbf, "Mode X weather")
ABSENT(mode_x_effect_2d08, "Mode X weather")
ABSENT(mode_x_effect_2da9, "Mode X weather")
ABSENT(mode_x_effect_2dfb, "Mode X weather")
ABSENT(mode_x_effect_2e9c, "Mode X weather")
ABSENT(mode_x_effect_2f0c, "Mode X weather")
ABSENT(mode_x_effect_300f, "Mode X weather")
ABSENT(asm_0283c, "Mode X plane select")
ABSENT(asm_02850, "Mode X tile ring records")
ABSENT(asm_029d3, "Mode X background records")
ABSENT(asm_02a73, "Mode X cached background records")
ABSENT(asm_03257, "Mode X row records")
ABSENT(asm_037e4, "Mode X row records")
ABSENT(asm_03912, "Mode X row records")
ABSENT(asm_03ea1, "Mode X row records")
ABSENT(asm_04430, "Mode X tile callbacks")
ABSENT(asm_04493, "Mode X tile callbacks")
ABSENT(asm_04504, "Mode X borders")
ABSENT(asm_0456e, "Mode X borders")
ABSENT(display_anypicture, "Mode X picture")
ABSENT(display_emptypicture, "Mode X picture")
ABSENT(draw_blackbox, "Mode X box")
ABSENT(draw_borderbox, "Mode X box")
ABSENT(block_clear, "Mode X tile plane")
ABSENT(block_copy, "Mode X tile plane")
ABSENT(sub_1268A, "Mode X VGA cache upload")

/* Movies: the FLC player (src/movie.c) reads with DOS _dos_read; not ported yet (docs/port/MOVIE_FORMAT.md). */
ABSENT(_dos_read, "movie file read")
/* BIOS services without a caller on the supported paths. */
ABSENT(bios_wait_timer, "INT 15h wait (unused benchmark, docs/port/PLATFORM_OPEN.md)")
/* VGA graphics-controller write inside an effect (effect1), and the sound fatal-error text mode switch. */
ABSENT(write_vga_register, "VGA register write from an effect")
ABSENT(sound_set_video_mode, "sound fatal-error text mode")
