/*
 * Build-time gate (compiled, never linked): a historical effect unit next to render/effect_layouts.h. Every field of
 * the port_ copy must have the offset and size of the original's. One file per unit (the units' private declarations
 * of shared helpers differ).
 */
#include "effect633e0.c"
#include "render/effect_layouts.h"

#define SAME_FIELD(original, copy, field) \
    _Static_assert(offsetof(original, field) == offsetof(copy, field) && \
                   sizeof(((original *)0)->field) == sizeof(((copy *)0)->field), #original "." #field)

SAME_FIELD(effect_column_state_t, port_effect_column_state_t, pos_x);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, delta);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, field_04);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, field_08);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, field_0c);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, field_10);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, field_14);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, mode);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, initialized);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, width);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, table);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, source);
SAME_FIELD(effect_column_state_t, port_effect_column_state_t, destination);
_Static_assert(sizeof(effect_column_state_t) == sizeof(port_effect_column_state_t), "effect_column_state_t size");
