/* Interface between the snapshot capture (render/scene_capture.c, compiled with the game) and the port. */
#ifndef PORT_RENDER_CAPTURE_H
#define PORT_RENDER_CAPTURE_H

#include "render/scene.h"

#include <stdint.h>

/* The snapshot captured since the last call (the frame now being built or just built), or NULL. */
const port_scene *port_scene_take_pending(void);
/* Exactness gate: compare the semantic background/map with the original draw buffer every frame. */
void port_scene_set_verify(int on);
void port_scene_verify_report(void);
/* Digest of gameplay state (camera, clock, RNG, objects) for presentation-independence tests. */
uint64_t port_scene_state_digest(void);

#endif
