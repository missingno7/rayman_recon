/*
 * Touch controls (port/docs/TOUCH_CONTROLS.md): the Android release's overlay, feeding the virtual pad
 * (platform/pad.h). On by default on Android; RAYMAN_TOUCH=1 turns it on elsewhere (the mouse then acts as a
 * finger, for testing).
 */
#ifndef PORT_PLATFORM_TOUCH_H
#define PORT_PLATFORM_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

void port_touch_init(int enabled);
int port_touch_enabled(void);
/* SDL finger events; returns 1 if consumed. Takes a const SDL_Event *. */
int port_touch_event(const void *sdl_event);
/* Once per event poll, before port_pad_update: follow the game's context and set the touch pad buttons. */
void port_touch_update(int output_width, int output_height);
/* The overlay changed since it was last drawn (the presenter then redraws the last frame). */
int port_touch_needs_redraw(void);
/* Draw the overlay over the presented frame (SDL_Renderer *). */
void port_touch_draw(void *renderer);

#ifdef __cplusplus
}
#endif

#endif
