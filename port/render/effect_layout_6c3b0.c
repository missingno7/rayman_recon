/*
 * Build-time gate (compiled, never linked): a historical effect unit next to render/effect_layouts.h. Every field of
 * the port_ copy must have the offset and size of the original's. One file per unit (the units' private declarations
 * of shared helpers differ).
 */
#include "effect6c3b0.c"
#include "render/effect_layouts.h"

#define SAME_FIELD(original, copy, field) \
    _Static_assert(offsetof(original, field) == offsetof(copy, field) && \
                   sizeof(((original *)0)->field) == sizeof(((copy *)0)->field), #original "." #field)

SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, x);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, y);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, radius);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, width);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, height);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, map);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, screen);
SAME_FIELD(effect_state_6c_t, port_effect_state_6c_t, output);
_Static_assert(sizeof(effect_state_6c_t) == sizeof(port_effect_state_6c_t), "effect_state_6c_t size");
