/*
 * Virtual pad (port/docs/TOUCH_CONTROLS.md). The Android release drives the game through one set of pad buttons:
 * its touch overlay and a real gamepad both report the same button ids, and its native layer turns held buttons into
 * the keys the DOS code reads. The port does the same: an SDL gamepad and the touch overlay (platform/touch.c) set
 * pad buttons, and port_pad_update turns them into PC scan codes for the game's own keyboard handler.
 *
 * Runs on the thread that polls SDL events (the presenter, platform/host.c).
 */
#ifndef PORT_PLATFORM_PAD_H
#define PORT_PLATFORM_PAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Button ids of the Android release (RaymanGameManager._Buttons, 1-based). */
enum port_pad_button {
    PORT_PAD_A = 1, PORT_PAD_B, PORT_PAD_X, PORT_PAD_Y, PORT_PAD_START, PORT_PAD_BACK,
    PORT_PAD_LB, PORT_PAD_RB, PORT_PAD_L3, PORT_PAD_R3,
    PORT_PAD_UP, PORT_PAD_DOWN, PORT_PAD_LEFT, PORT_PAD_RIGHT,
    PORT_PAD_BUTTONS
};

/* Who holds a button: sources are combined (any source holding it holds it). */
enum port_pad_source { PORT_PAD_FROM_GAMEPAD, PORT_PAD_FROM_TOUCH, PORT_PAD_SOURCES };

/* What the game is showing, read from game state (render/scene_capture.c port_game_input_context). The Android
   HUD contexts are named in the comments. */
enum port_input_context {
    PORT_CONTEXT_MENU,          /* every DOS screen outside the world map and levels: menus, in-level menu, movies */
    PORT_CONTEXT_WORLDMAP,      /* "Worldmap" */
    PORT_CONTEXT_GAMEPLAY,      /* "Gameplay" */
    PORT_CONTEXT_MOSKITO,       /* "GameplayRayOnMoskito" */
    PORT_CONTEXT_LEVEL_NO_HUD   /* a level showing a vignette, the level start or a fairy: "ExoticScreen", "Fee_no_hud" */
};

/* The configured keys of the game (key_right, key_up, key_left, key_down, key_fist, key_jump, key_action). */
enum { PORT_KEY_RIGHT, PORT_KEY_UP, PORT_KEY_LEFT, PORT_KEY_DOWN, PORT_KEY_FIST, PORT_KEY_JUMP, PORT_KEY_ACTION,
       PORT_KEY_COUNT };

/* Game state reader (compiled with the game). Returns enum port_input_context and the configured keys. Rayman's
   powers for the HUD: bit 0 fist, 1 helicopter, 2 super helicopter, 3 run, 4 magic seed. */
int port_game_input_context(uint8_t keys[PORT_KEY_COUNT], unsigned *powers);
enum { PORT_POWER_FIST = 1, PORT_POWER_HELICO = 2, PORT_POWER_SUPER_HELICO = 4, PORT_POWER_RUN = 8,
       PORT_POWER_SEED = 16 };

void port_pad_init(void);
/* SDL gamepad events (SDL_EVENT_GAMEPAD_*); returns 1 if consumed. Takes a const SDL_Event *. */
int port_pad_event(const void *sdl_event);
void port_pad_set(int source, int button, int held);
void port_pad_clear(int source);
/* A short press of a button (the touch pause button): held for one tap period from now. */
void port_pad_tap(int source, int button);
/* A gamepad is connected (the Android HUD then hides the touch controls). */
int port_pad_gamepad_connected(void);
/* Combine the sources, map them for the current context and queue the changed keys. Call once per event poll. */
void port_pad_update(void);
/* The context of the last update and Rayman's powers then. */
int port_pad_context(unsigned *powers);

#ifdef __cplusplus
}
#endif

#endif
