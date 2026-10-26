/*
 * Menu fade capture (port/docs/RENDERER.md "Screen effects"): the menu fades are SYNCHRO_LOOP scene callbacks of
 * menu.c, called through a pointer inside frame.c. SYNCHRO_LOOP is wrapped (it is called from other units) and the
 * known callbacks are replaced by trampolines that record the pixel pass with the step the callback starts from,
 * then run the callback once. Kept in its own file so that a test defining its own SYNCHRO_LOOP wrapper
 * (tests/effect_diff.c) does not pull this one in.
 */
#include "rtypes.h"
#include "game.h"

#include "render/effects.h"
#include "render/record.h"

extern u8 *EffetBufferNormal, *DrawBufferNormal;
extern s32 Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2;
extern s16 xFondu, yFondu, wFondu, hFondu, EtapeFondu;

static int buffer(const void *p, port_effect_buffer *b)
{
    b->pad = 0;
    b->target = port_record_target(p, &b->offset);
    return b->target < PORT_TARGET_COUNT;
}

/* apparition_prg first restores the menu background (DISPLAY_FOND_MENU, recorded copies), so its zoom operation is
   added after it returns; the other two only draw their pass. */
s16 apparition_prg(u32 a1);
s16 fondu_prg(u32 a1);
s16 FonduPixel_prg(u32 arg);

static int fade_inputs(port_effect_fade *e)
{
    e->x = xFondu;
    e->y = yFondu;
    e->width = wFondu;
    e->height = hFondu;
    e->step = EtapeFondu;
    e->view_x0 = Bloc_lim_W1;
    e->view_x1 = Bloc_lim_W2;
    e->view_y0 = Bloc_lim_H1;
    e->view_y1 = Bloc_lim_H2;
    return buffer(DrawBufferNormal, &e->draw) && buffer(EffetBufferNormal + 64000, &e->picture);
}

static s16 zoom_in_scene(u32 a)
{
    static port_effect_fade e;
    int ok = fade_inputs(&e);
    s16 r = apparition_prg(a);
    if (!ok || !port_record_effect(PORT_EFFECT_ZOOM_IN, &e, sizeof e))
        port_record_unsupported_effect(DrawBufferNormal);
    return r;
}

static s16 fade_scene(int effect, s16 (*scene)(u32), u32 a)
{
    static port_effect_fade e;
    s16 r;
    if (!fade_inputs(&e) || !port_record_effect(effect, &e, sizeof e)) {
        port_record_unsupported_effect(DrawBufferNormal);
        return scene(a);
    }
    port_record_claim(1);
    r = scene(a);
    port_record_claim(0);
    return r;
}

static s16 blend_scene(u32 a) { return fade_scene(PORT_EFFECT_BLEND, fondu_prg, a); }
static s16 dissolve_scene(u32 a) { return fade_scene(PORT_EFFECT_DISSOLVE, FonduPixel_prg, a); }

void __real_SYNCHRO_LOOP(scene_func_t scene_func);
void __wrap_SYNCHRO_LOOP(scene_func_t scene_func)
{
    if (scene_func == apparition_prg)
        scene_func = zoom_in_scene;
    else if (scene_func == fondu_prg)
        scene_func = blend_scene;
    else if (scene_func == FonduPixel_prg)
        scene_func = dissolve_scene;
    __real_SYNCHRO_LOOP(scene_func);
}
