/* Interface of the semantic capture (render/scene_capture.c, compiled with the game) towards the port. */
#ifndef PORT_RENDER_CAPTURE_H
#define PORT_RENDER_CAPTURE_H

#include <stdint.h>

/* Digest of gameplay state (camera, clock, RNG, objects) for presentation-independence tests. */
uint64_t port_scene_state_digest(void);

#endif
