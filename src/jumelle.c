/* jumelle.c (DOS v1.21, 1:32930..1:34580): binoculars/world-map zoom; -fpc. */
/* Worker candidate. Compile wc106 -zq -oaxt -s -5r -fpc; see NOTES.md.
 * Shared ORIGINAL_INLINE helpers are declared in inline_helpers.h.
 * Local match: 12/19 STRICT; _TEXT remains non-contiguous.
 * DisplayJumellesNormal: explicit guarded midpoint-circle recurrences are STRICT.
 * Function/global names RAYVERSE except ChangeRealPosJumelle (ANDROID).
 * Descriptive locals INVENTED. No shared types changed.
 */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
#include "inline_helpers.h"
#include <string.h>
#include <math.h>

typedef int bool;
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define Abs(x) ((s32)(x) >= 0 ? (s32)(x) : -(s32)(x))

extern u8 ChangeJumelleSizeOK;
extern s32 HauteurJumelle;
extern s32 JumAmpDefMul;
extern s32 JumAmpDefPlus;
extern u8 JumelleEffetAutorise;
extern s32 JumellePosX;
extern s32 JumellePosY;
extern s32 JumelleXMin;
extern s32 JumelleYMin;
extern u8 JumelleZoomActif;
extern s32 JumelleZoomAmp;
extern s32 LargeurJumelle;
extern u8 ModeAutoJumelle;
extern s32 ModeAutoJumelleZoomAmp;
extern u8 ParamZoomChange;
extern s32 PositionJumelleDemandeX;
extern s32 PositionJumelleDemandeY;
extern s32 PositionJumelleX16;
extern s32 PositionJumelleY16;
extern u8 PositionStageNameCalcule;
extern s32 RayonJumelle;
extern s32 XSpeedJumelle;
extern s32 Xmap16;
extern s32 YSpeedJumelle;
extern s32 Ymap16;
extern s16 decalage_en_cours;
extern s16 h_scroll_speed;
extern obj_t ray;
extern s16 scroll_end_x;
extern s16 scroll_end_y;
extern s16 scroll_start_x;
extern s16 scroll_start_y;
extern s16 v_scroll_speed;
extern s16 xmap;
extern s16 ymap;

extern s32 *DistPointX;
extern s32 *DistPointY;
extern s32 *ExpPoint;
extern s32 *JumelleZoomDef;
extern s16 scroll_x;
extern s16 scroll_y;
extern s16 dhspeed;
extern s16 dvspeed;
extern u16 WidthNworld;
extern u16 HeightNworld;
extern s32 CompteurJumelle;
extern s32 ModeAutoRayonJumelle;
extern s32 ModeAutoPosXJumelle;
extern s32 ModeAutoPosYJumelle;
extern u8 *EffetBufferNormal;
extern u8 *PLAN2BIT;

void FatalError(char *message);
void *JumelleCalloc(s32 count, s32 size);
void JumelleFreeArray(s32 **array);
s16 myRand(s16 max);
s32 ChangeDeltaPosXJumelleWithLimit(s32 delta_pos_x);
s32 ChangeDeltaPosYJumelleWithLimit(s32 delta_pos_y);
void DefaultJumelleVariable(void);
void ChangeDeltaSizeJumelle(s32 delta_size);
void CalculVitesseJumelle(void);
void ChangeDeltaPosXJumelleWithoutLimit(s32 delta_pos_x);
void ChangeDeltaPosYJumelleWithoutLimit(s32 delta_pos_y);
void ChangeDeltaZoomAmpJumelle(s32 a1);
void CalcObjPosInWorldMap(obj_t* obj);
void MoveRayInWorldMap(void);

void plot2linejumelle(s32 jumelle_x, s32 jumelle_y, s32 plot_x, s32 plot_y, s32 circle2_offset, u8* effet_buf, u8* draw_buf) {
    /* COMPILER_STEERED: local ownership and pointer increment sequencing. */
    s32 v53;
    s32 v55;
    s32 v56;
    s32 v7;
v53 = jumelle_y - 64;
    v7 = jumelle_x - plot_x - circle2_offset;
    v55 = SCREEN_WIDTH * JumelleYMin + JumelleXMin;
    v56 = jumelle_x - 112;
    if (plot_x > circle2_offset) {
        // Draw middle part of both circles
        s32 v58 = 2 * (plot_x + circle2_offset);
        if (v7 < 0) {
            v58 += v7;
            v7 = 0;
        }
        if (v7 + v58 >= SCREEN_WIDTH) {
            v58 = SCREEN_WIDTH - v7 - 1;
        }
        if (v58 > 0) {
            s32 v9 = jumelle_y - plot_y;
            s32 v16;
            if (v9 >= 0 && v9 < SCREEN_HEIGHT) {
                u8* v10 = draw_buf + SCREEN_WIDTH * v9 + v7;
                u8* v11 = effet_buf + SCREEN_WIDTH * v9 + v7 - v55;
                if (JumelleZoomActif) {
                    /* DOS zoom lookup; local names INVENTED. */
                    s32 *zoom = JumelleZoomDef + ((448 * (v9 - v53)) / 2 + (v7 - v56));
                    s32 count = v58;
                    do {
                        *v10 = v11[*zoom];
                        ++v11;
                        ++zoom;
                        ++v10;
                    } while (--count);
                } else {
                    copy(v11, v10, v58);
                }
            }

            v16 = jumelle_y + plot_y;
            if (v16 >= 0 && v16 < SCREEN_HEIGHT) {
                u8* v10 = draw_buf + SCREEN_WIDTH * v16 + v7;
                u8* v11 = effet_buf + SCREEN_WIDTH * v16 + v7 - v55;
                if (JumelleZoomActif) {
                    /* DOS zoom lookup; local names INVENTED. */
                    s32 *zoom = JumelleZoomDef + ((448 * (v16 - v53)) / 2 + (v7 - v56));
                    s32 count = v58;
                    do {
                        *v10 = v11[*zoom];
                        ++v11;
                        ++zoom;
                        ++v10;
                    } while (--count);
                } else {
                    copy(v11, v10, v58);
                }
            }
        }
        } else {
        s32 v60 = 2 * plot_x;
        s32 v25;
        s32 v59;
        s32 v57;
        s32 v26;
        s32 v54;
        s32 v8;
        s32 v52;
        if (v7 < 0) {
            v60 += v7;
            v7 = 0;
        }
        if (v7 + v60 >= SCREEN_WIDTH) {
            v60 = SCREEN_WIDTH - v7 - 1;
        }
        v25 = circle2_offset + jumelle_x - plot_x;
        v59 = 2 * plot_x;
        v57 = v25;
        if (v25 < 0) {
            v59 += v25;
            v57 = 0;
        }
        if (v59 + v57 >= SCREEN_WIDTH) {
            v59 = SCREEN_WIDTH - v57 - 1;
        }

        // Draw top segments
        v26 = jumelle_y - plot_y;
        v54 = v26;
        if (jumelle_y - plot_y >= 0 && v26 < SCREEN_HEIGHT) {
            // Top segment of left circle
            if (v60 > 0) {
                s32 v27 = SCREEN_WIDTH * v26 + v7;
                s32 v28 = v27 - v55;
                u8* v29 = draw_buf + v27;
                u8* v30 = effet_buf + v28;
                if (JumelleZoomActif) {
                    /* DOS zoom lookup; local names INVENTED. */
                    s32 *zoom = JumelleZoomDef + ((448 * (v26 - v53)) / 2 + (v7 - v56));
                    s32 count = v60;
                    do {
                        *v29 = v30[*zoom];
                        ++v30;
                        ++zoom;
                        ++v29;
                    } while (--count);
                } else {
                    copy(v30, v29, v60);
                }
            }

            // Top segment of right circle
            if (v59 > 0) {
                u8* v35 = effet_buf + SCREEN_WIDTH * v54 + v57 - v55;
                u8* v36 = draw_buf + SCREEN_WIDTH * v54 + v57;
                if (JumelleZoomActif) {
                    /* DOS zoom lookup; local names INVENTED. */
                    s32 *zoom = JumelleZoomDef + ((448 * (v54 - v53)) / 2 + (v57 - v56));
                    s32 count = v59;
                    do {
                        *v36++ = v35[*zoom++];
                        ++v35;
                    } while (--count);
                } else {
                    copy(v35, v36, v59);
                }
            }
        }

        // Draw bottom segments
        v8 = plot_y + jumelle_y;
        v52 = v8;
        if (v8 >= 0 && v8 < SCREEN_HEIGHT) {
            // Bottom segment of left circle
            if (v60 > 0) {
                u8* v41 = draw_buf + SCREEN_WIDTH * v52 + v7;
                u8* v42 = effet_buf + SCREEN_WIDTH * v52 + v7 - v55;
                if (JumelleZoomActif) {
                    /* DOS zoom lookup; local names INVENTED. */
                    s32 *zoom = JumelleZoomDef + ((448 * (v52 - v53)) / 2 + (v7 - v56));
                    s32 count = v60;
                    do {
                        *v41++ = v42[*zoom++];
                        ++v42;
                    } while (--count);
                } else {
                    copy(v42, v41, v60);
                }
            }

            if (v59 > 0) {
                u8* v18 = draw_buf + SCREEN_WIDTH * v52 + v57;
                u8* v24 = effet_buf + SCREEN_WIDTH * v52 + v57 - v55;
                if (JumelleZoomActif) {
                    /* DOS zoom lookup; local names INVENTED. */
                    s32 *zoom = JumelleZoomDef + ((448 * (v52 - v53)) / 2 + (v57 - v56));
                    s32 count = v59;
                    do {
                        *v18 = v24[*zoom];
                        ++v24;
                        ++zoom;
                        ++v18;
                    } while (--count);
                } else {
                    copy(v24, v18, v59);
                }
            }
        }
        }
}


void DisplayJumellesNormal(s32 x, s32 y, s32 rayon, s32 circle2_offset, u8* effet_buf, u8* draw_buf) {
    s32 plot_x = 0;
    s32 decision = 0;
    s32 plot_y = rayon;
    /* First differences of the midpoint-circle decision variable.
     * The guarded declarations also reproduce the DOS induction slots. */
    if (plot_x <= plot_y) {
        s32 east_step = 2 * plot_x + 1;
        s32 south_step = 2 * plot_y - 1;
        do {
            plot2linejumelle(x, y, plot_x, plot_y, circle2_offset, effet_buf, draw_buf);
            plot2linejumelle(x, y, plot_y, plot_x, circle2_offset, effet_buf, draw_buf);
            decision += east_step;
            east_step += 2;
            ++plot_x;
            if (decision >= plot_y) {
                decision -= south_step;
                south_step -= 2;
                --plot_y;
            }
        } while (plot_x <= plot_y);
    }
    if (ParamZoomChange) PrepareJumelleZoom();
}


void InitMatriceJumelle(void) {
    if (JumelleEffetAutorise) {
        DistPointX = (s32*)JumelleCalloc(161, sizeof(s32));
        DistPointY = (s32*)JumelleCalloc(65, sizeof(s32));
        ExpPoint = (s32*)JumelleCalloc(5120, sizeof(s32));
        JumelleZoomDef = (s32*)JumelleCalloc(29025, sizeof(s32));
        if (!DistPointX || !DistPointY || !ExpPoint || !JumelleZoomDef) {
            if (DistPointX) {
                JumelleFreeArray(&DistPointX);
            }
            if (DistPointY) {
                JumelleFreeArray(&DistPointY);
            }
            if (ExpPoint) {
                JumelleFreeArray(&ExpPoint);
            }
            if (JumelleZoomDef) {
                JumelleFreeArray(&JumelleZoomDef);
            }
            JumelleEffetAutorise = 0;
            FatalError("Pas de Jumelle");
        } else {
            s32 y;
            s32 x;
            s32 i;
            for (y = 0; y <= 64; ++y) {
                DistPointY[y] = y * y;
                for (x = 0; x <= 160; ++x) {
                    DistPointX[x] = x * x;
                }
            }
            for (i = 0; i < 5120; ++i) {
                ExpPoint[i] = (s32)(exp(-(double)i / 1024.0) * 65536.0);
            }
        }
    }
    DefaultJumelleVariable();
}


void DefaultJumelleVariable(void) {
    if (JumelleEffetAutorise) {
        JumelleZoomActif = 1;
        ChangeJumelleSizeOK = 3;
    } else {
        JumelleZoomActif = 0;
        ChangeJumelleSizeOK = 3;
    }
    RayonJumelle = 25;
    LargeurJumelle = 87;
    HauteurJumelle = 50;
    JumellePosX = 160;
    JumellePosY = 100;
    PositionJumelleX16 = 2560;
    PositionJumelleY16 = 1600;
    ModeAutoJumelle = 0;
    PositionStageNameCalcule = 0;
    ChangeJumelleVariable();
    if (JumelleEffetAutorise) {
        JumelleZoomAmp = 26 * -RayonJumelle + 32150;
        PrepareJumelleZoom();
    }
}

void ChangeDeltaSizeJumelle(s32 delta_size) {
    RayonJumelle = max32(25, min32(64, RayonJumelle + delta_size));
    ChangeJumelleVariable();
    if (JumelleZoomActif)
        ChangeDeltaZoomAmpJumelle(delta_size > 0 ? -65 : 65);
}

void CalculVitesseJumelle(void) {
    s16 delta_x = PositionJumelleDemandeX - JumellePosX;
    s16 delta_y = PositionJumelleDemandeY - JumellePosY;
    /* DOS 1:3316f..33189: promoted absolute-value snapshots.
     * The remaining difference is the timing of the EDX-to-EDI copy. */
    s32 absolute_x = Abs((s32)delta_x);
    s32 absolute_y = Abs((s32)delta_y);
    s16 dist = absolute_x + absolute_y;
    s32 dx, dy;
    if (dist > 0) {
        delta_x = ashl16(delta_x, 6) / dist;
        delta_y = ashl16(delta_y, 6) / dist;
    }
    dx = delta_x - XSpeedJumelle;
    dx = sgn(dx);
    dx += XSpeedJumelle;
    dx = max32(min32(dx, 16), -16);
    XSpeedJumelle = dx;
    dy = delta_y - YSpeedJumelle;
    dy = sgn(dy);
    dy += YSpeedJumelle;
    dy = max32(min32(dy, 16), -16);
    YSpeedJumelle = dy;
}

void ChangeDeltaPosXJumelleWithoutLimit(s32 delta_pos_x) {
    delta_pos_x = max32(-16, min32(16, delta_pos_x));

    if (ModeAutoJumelle & 1) {
        Xmap16 = (Xmap16 & ~0xF) | (PositionJumelleX16 & 0xF);
        Xmap16 = max32(16 * scroll_start_x, min32(16 * scroll_end_x, delta_pos_x + Xmap16));
        xmap = Xmap16 >> 4;
    } else {
        xmap = max32(scroll_start_x, min32(scroll_end_x, xmap + (delta_pos_x >> 4)));
    }

    PositionJumelleX16 += delta_pos_x;
    JumellePosX = PositionJumelleX16 >> 4;
    ChangeJumelleVariable();
}

void ChangeDeltaPosYJumelleWithoutLimit(s32 delta_pos_y) {
    delta_pos_y = max32(-16, min32(16, delta_pos_y));

    if (ModeAutoJumelle & 1) {
        Ymap16 = (Ymap16 & ~0xF) | (PositionJumelleY16 & 0xF);
        Ymap16 = max32(16 * scroll_start_y, min32(16 * scroll_end_y, delta_pos_y + Ymap16));
        ymap = Ymap16 >> 4;
    } else {
        ymap = max32(scroll_start_y, min32(scroll_end_y, ymap + (delta_pos_y >> 4)));
    }

    PositionJumelleY16 += delta_pos_y;
    JumellePosY = PositionJumelleY16 >> 4;
    ChangeJumelleVariable();
}

/* DOS 1:333d0: cache the radius margin and position for both tests.
 * Initializing the result before the entry clamp reproduces ESI at entry.
 * Still NEAR: the inner clamp and final update allocation differ. */
s32 ChangeDeltaPosXJumelleWithLimit(s32 delta_pos_x) {
    /* SYNTHETIC: two initialized, non-escaping local address copies of the true result. */
    bool changed = 0;
    delta_pos_x = max32(-16, min32(16, delta_pos_x));

    if ((xmap != scroll_start_x && xmap != scroll_end_x) ||
        (xmap == scroll_start_x && delta_pos_x > 0) ||
        (xmap == scroll_end_x && delta_pos_x < 0)) {
        s32 margin = RayonJumelle / 3;
        s32 position = JumellePosX;
        if (margin < position && 320 - margin > position) {
            if (ModeAutoJumelle & 1) {
                Xmap16 = (Xmap16 & ~0xF) | (PositionJumelleX16 & 0xF);
                Xmap16 = max32(16 * scroll_start_x,
                    min32(16 * scroll_end_x, delta_pos_x + Xmap16));
                xmap = Xmap16 >> 4;
            } else {
                xmap = max32(scroll_start_x,
                    min32(scroll_end_x, xmap + (delta_pos_x >> 4)));
            }
            PositionJumelleX16 += delta_pos_x;
            JumellePosX = PositionJumelleX16 >> 4;
            {
                bool result_value;
                bool *result_address;
                result_address = &result_value;
                *result_address = 1;
                {
                    bool second_value;
                    bool *second_address;
                    second_address = &second_value;
                    *second_address = result_value;
                    changed = second_value;
                }
            }
        }
    }
    ChangeJumelleVariable();
    return changed;
}

/* DOS 1:33520: cache the radius margin and position for both tests.
 * Initializing the result before the entry clamp reproduces ESI at entry.
 * Still NEAR: the inner clamp and final update allocation differ. */
s32 ChangeDeltaPosYJumelleWithLimit(s32 delta_pos_y) {
    /* SYNTHETIC: two initialized, non-escaping local address copies of the true result. */
    bool changed = 0;
    delta_pos_y = max32(-16, min32(16, delta_pos_y));

    if ((ymap != scroll_start_y && ymap != scroll_end_y) ||
        (ymap == scroll_start_y && delta_pos_y > 0) ||
        (ymap == scroll_end_y && delta_pos_y < 0)) {
        s32 margin = RayonJumelle / 3;
        s32 position = JumellePosY;
        if (margin < position && 200 - margin > position) {
            if (ModeAutoJumelle & 1) {
                Ymap16 = (Ymap16 & ~0xF) | (PositionJumelleY16 & 0xF);
                Ymap16 = max32(16 * scroll_start_y,
                    min32(16 * scroll_end_y, delta_pos_y + Ymap16));
                ymap = Ymap16 >> 4;
            } else {
                ymap = max32(scroll_start_y,
                    min32(scroll_end_y, ymap + (delta_pos_y >> 4)));
            }
            PositionJumelleY16 += delta_pos_y;
            JumellePosY = PositionJumelleY16 >> 4;
            {
                bool result_value;
                bool *result_address;
                result_address = &result_value;
                *result_address = 1;
                {
                    bool second_value;
                    bool *second_address;
                    second_address = &second_value;
                    *second_address = result_value;
                    changed = second_value;
                }
            }
        }
    }
    ChangeJumelleVariable();
    return changed;
}

/* Name ANDROID; recovered DOS entry 1:33670. */
void ChangeRealPosJumelle(s32 x, s32 y) {
    JumellePosX = x;
    JumellePosY = y;
    PositionJumelleX16 = x << 4;
    PositionJumelleY16 = y << 4;
    ChangeJumelleVariable();
}

/* W-TEMP: use the dimensions directly; -oa caches the globals and defers
 * HauteurJumelle's store. DOS 1:3368c..1:33730, all 164 bytes STRICT. */
void ChangeJumelleVariable(void) {
    LargeurJumelle = (7 * RayonJumelle) / 2;
    HauteurJumelle = 2 * RayonJumelle;
    JumelleXMin = JumellePosX - LargeurJumelle / 2;
    JumelleYMin = JumellePosY - HauteurJumelle / 2;
    scroll_start_x = LargeurJumelle / 2 + 2;
    scroll_start_y = HauteurJumelle / 2 + 2;
    scroll_end_x = WidthNworld - LargeurJumelle / 2 - 4;
    scroll_end_y = HeightNworld - HauteurJumelle / 2 - 4;
}

void ChangeDeltaZoomAmpJumelle(s32 a1) {
    if ((-RayonJumelle * JumAmpDefMul + JumAmpDefPlus != JumelleZoomAmp ||
         ((ModeAutoJumelle & 2) && JumelleZoomAmp != ModeAutoJumelleZoomAmp)) && JumelleEffetAutorise) {
        s32 amplitude = a1 + JumelleZoomAmp;
        s32 bound = 26 * -RayonJumelle + 32150;
        JumelleZoomAmp = max32(0, min32(bound, amplitude));
        JumelleZoomActif = 1;
        ParamZoomChange = 1;
        ChangeJumelleVariable();
    } else {
        if (JumelleZoomAmp != 0 && JumelleEffetAutorise) {
            JumelleZoomActif = 1;
            ParamZoomChange = 0;
        } else {
            JumelleZoomActif = 0;
            ParamZoomChange = 0;
        }
    }
}

void PrepareJumelleZoom(void) {
    /* COMPILER_STEERED: declaration order; initializer expressions/order retained. */
    s32 *right;
    u32 x;
    s32 *left;
    u32 y;
    u32 distance;
    u32 factor;
    u32 dx;
    u32 dy;
    s32 margin;
    s32 scale;
scale = 0x200000 / (RayonJumelle * RayonJumelle);
    margin = (3 * (64 - RayonJumelle)) >> 2;
    left = JumelleZoomDef + 14400 + margin;
    right = JumelleZoomDef + 14272 + 223 - margin;
    if (JumelleEffetAutorise) {
        ParamZoomChange = 0;
        memset(JumelleZoomDef, 0, 29025 * sizeof(s32));
        for (y = 0; y <= (u32)RayonJumelle; y++) {
            for (x = 0; x < (u32)(LargeurJumelle - RayonJumelle); x++) {
                distance = (DistPointX[x] + DistPointY[y]) * scale;
                if (distance < 0x500000) {
                    factor = ((u32)JumelleZoomAmp * (u32)ExpPoint[distance >> 10]) >> 16;
                    dx = (x * factor + 0x8000) >> 16;
                    dy = 320 * ((y * factor + 0x8000) >> 16);
                    if (x) {
                        left[x - y * 224] += dy - dx;
                        right[y * 224 - x] += dx - dy;
                    }
                    if (y) {
                        left[y * 224 + x] += -dx - dy;
                        right[-y * 224 - x] += dx + dy;
                    }
                    if (x <= (u32)RayonJumelle) {
                        if (y) {
                            left[-y * 224 - x] += dx + dy;
                            right[y * 224 + x] += -dx - dy;
                        }
                        if (x) {
                            left[y * 224 - x] += dx - dy;
                            right[-y * 224 + x] += dy - dx;
                        }
                    }
                }
            }
        }
    }
}


void RecaleRayPosInJumelle(void) {
    /* COMPILER_STEERED: exact s32 promotions of signed-word absolute-value inputs. */
    if (scroll_y == -1) {
        s32 v13 = ((3 * HauteurJumelle) >> 2) - ray.offset_by;
        if (v_scroll_speed != 255 || decalage_en_cours != 0) {
            s32 v14 = ray.screen_y - v13;
            v_scroll_speed = ashr16(v14, 2);
            if (Abs((s32)ray.speed_y) <= Abs((s32)v_scroll_speed)) {
                s32 v16 = max32(3, Abs((s32)ray.speed_y));
                if (v_scroll_speed <= 0) {
                    if (v_scroll_speed != 0) {
                        v_scroll_speed = max32(v_scroll_speed, -v16);
                    } else if (v14 <= 0) {
                        if (v14 != 0) {
                            v_scroll_speed = -1;
                        }
                    } else {
                        v_scroll_speed = 1;
                    }
                } else {
                    v_scroll_speed = min32(v_scroll_speed, v16);
                }
            }
        } else if (ray.main_etat != 1) {
            v_scroll_speed = (ray.screen_y >= v13 + 48) ? 0 : -4;
        }
    }

    if (scroll_x == -1) {
        s32 v2 = 3 * LargeurJumelle + 7;
        s32 min_x;
        s32 v4;
        s32 max_x;
        if (3 * LargeurJumelle >= 0) {
            v2 = 3 * LargeurJumelle;
        }
        min_x = (v2 >> 3) - ray.offset_bx;

        v4 = 5 * LargeurJumelle + 7;
        if (5 * LargeurJumelle >= 0) {
            v4 = 5 * LargeurJumelle;
        }
        max_x = (v4 >> 3) - ray.offset_bx;

        if (decalage_en_cours > 0 || ray.speed_x > 0) {
            s16 unk_x_3 = ashr16(ray.screen_x - min_x, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else if (decalage_en_cours < 0 || ray.speed_x < 0) {
            s16 unk_x_3 = ashr16(ray.screen_x - max_x, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else {
            if (ray.flags.flip_x) {
                s16 unk_x_3 = ashr16(ray.screen_x - min_x, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            } else {
                s16 unk_x_3 = ashr16(ray.screen_x - max_x, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            }
        }

        if (Abs((s32)dhspeed) > 4) {
            dhspeed = (dhspeed > 0) ? 4 : -4;
        }
        h_scroll_speed += ashr16(dhspeed, 2);

        if ((max_x > ray.screen_x && ray.speed_x < 0) || (min_x < ray.screen_x && ray.speed_x > 0)) {
            h_scroll_speed += ray.speed_x;
        }

    }
}


void DisplayJumellesFondNormal(void) {
    s32 width = LargeurJumelle;
    s32 height = HauteurJumelle;
    width += 2;
    height += 2;
    sprite_clipping(0, width, 0, height);
    DisplayAnyPictureNormal(PLAN2BIT, EffetBufferNormal, xmap - LargeurJumelle/2, ymap - HauteurJumelle/2, 0, 0, WidthNworld, width, height);
}


void DoScrollInWorldMap(s16 h_speed, s16 v_speed) {
    /* COMPILER_STEERED: declaration order; identical constant initializations. */
    u8 need_scroll_xmap;
    s32 delta_pos_x;
    u8 need_scroll_ymap;
    s32 delta_pos_y;
    need_scroll_xmap = 1;
    need_scroll_ymap = 1;
    delta_pos_x = 0;
    delta_pos_y = 0;
    if (ChangeJumelleSizeOK != 4) {
        s16 scroll_limit_x;
        s16 scroll_limit_y;
        if (ray.speed_x == 0 && ray.speed_y == 0) {
            PositionJumelleDemandeY = SCREEN_HEIGHT / 2;
            PositionJumelleDemandeX = SCREEN_WIDTH / 2;
            delta_pos_x = 16 * (PositionJumelleDemandeX - JumellePosX);
            ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            delta_pos_y = 16 * (PositionJumelleDemandeY - JumellePosY);
            ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
        } else {
            s32 ray_x = ray.offset_bx + ray.x;
            s32 ray_y;
            if (ray_x > scroll_start_x && ray_x < scroll_end_x) {
                delta_pos_x = 16 * (ray.offset_bx + ray.screen_x - LargeurJumelle / 2);
                if (ChangeDeltaPosXJumelleWithLimit(delta_pos_x)) {
                    need_scroll_xmap = 0;
                }
            } else {
                delta_pos_x = 16 * ((SCREEN_WIDTH / 2) - JumellePosX);
                ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            }

            ray_y = ray.offset_by + ray.y;
            if (ray_y > scroll_start_y && ray_y < scroll_end_y) {
                delta_pos_y = 16 * (ray.screen_y + ray.offset_by - ((3 * HauteurJumelle) >> 2));
                if (ChangeDeltaPosYJumelleWithLimit(delta_pos_y)) {
                    need_scroll_ymap = 0;
                }
            } else {
                delta_pos_y = 16 * (SCREEN_HEIGHT / 2 - JumellePosY);
                ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
            }
        }

        if (need_scroll_xmap) {
            xmap += max32(-1, min32(1, h_speed));
        }
        if (need_scroll_ymap) {
            ymap += max32(-1, min32(1, v_speed));
        }

        scroll_limit_x = scroll_start_x;
        if (xmap < scroll_start_x || (scroll_limit_x = scroll_end_x, xmap > scroll_end_x)) {
            xmap = scroll_limit_x;
            dhspeed = 0;
            h_speed = 0;
        }

        scroll_limit_y = scroll_start_y;
        if (ymap < scroll_start_y || (scroll_limit_y = scroll_end_y, ymap > scroll_end_y)) {
            ymap = scroll_limit_y;
            dvspeed = 0;
            v_speed = 0;
        }
    }

    CalcObjPosInWorldMap(&ray);

    if (v_speed == 0) {
        ChangeJumelleSizeOK &= ~1;
    }

    if (h_speed == 0) {
        ChangeJumelleSizeOK &= ~2;
    }

    if (delta_pos_x == 0 && delta_pos_y == 0 && ray.speed_y == 0 && ray.speed_x == 0 && !(ChangeJumelleSizeOK & 0xfb)) {
        if (RayonJumelle < 64 && !ModeAutoJumelle) {
            ChangeDeltaSizeJumelle(1);
        }

        if ((u32)(JumAmpDefPlus + JumAmpDefMul * -RayonJumelle) >= (u32)JumelleZoomAmp || ModeAutoJumelle) {
            if ((u32)(JumAmpDefPlus + JumAmpDefMul * -RayonJumelle) > (u32)JumelleZoomAmp && !ModeAutoJumelle) {
                ChangeDeltaZoomAmpJumelle(300);
            }
        } else {
            ChangeDeltaZoomAmpJumelle(-300);
        }

        if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle == JumelleZoomAmp && RayonJumelle == 64 && ChangeJumelleSizeOK == 0) {
            ModeAutoJumelle = 0;
            CompteurJumelle = 0;
            ChangeJumelleSizeOK = 4;
        }

    } else if (ray.speed_x != 0 || ray.speed_y != 0) {
        ChangeJumelleSizeOK &= ~4;
    }

    if (ChangeJumelleSizeOK == 4) {
        ++CompteurJumelle;
        if (CompteurJumelle == 1830) {
            ModeAutoRayonJumelle = 25;
            ModeAutoJumelle |= ChangeJumelleSizeOK;
        } else if (CompteurJumelle == 1800) {
            ModeAutoJumelle |= 2;
            ModeAutoJumelleZoomAmp = 26 * -RayonJumelle + 32150;
        } else if (ModeAutoJumelle != 0) {
            if (!(ModeAutoJumelle & 1) && RayonJumelle == 25) {
                if (26 * -RayonJumelle + 32150 == JumelleZoomAmp) {
                    ModeAutoJumelle |= 1;
                    RayonJumelle = 25;
                    ModeAutoPosXJumelle = myRand(SCREEN_WIDTH) + RayonJumelle / 3;
                    ModeAutoPosYJumelle = myRand(SCREEN_HEIGHT) + RayonJumelle / 3;
                    PositionJumelleDemandeX = ModeAutoPosXJumelle;
                    PositionJumelleDemandeY = ModeAutoPosYJumelle;
                    Xmap16 = 16 * xmap;
                    Ymap16 = 16 * ymap;
                    PositionJumelleX16 = 16 * JumellePosX;
                    PositionJumelleY16 = 16 * JumellePosY;
                }
            }
        }

        if (ModeAutoJumelle & 1) {
            CalculVitesseJumelle();
            ChangeDeltaPosXJumelleWithoutLimit(XSpeedJumelle);
            ChangeDeltaPosYJumelleWithoutLimit(YSpeedJumelle);
            if (JumellePosX == ModeAutoPosXJumelle && JumellePosY == ModeAutoPosYJumelle) {
                ModeAutoPosXJumelle = myRand(SCREEN_WIDTH) + RayonJumelle / 3;
                ModeAutoPosYJumelle = myRand(SCREEN_HEIGHT) + RayonJumelle / 3;
                PositionJumelleDemandeX = ModeAutoPosXJumelle;
                PositionJumelleDemandeY = ModeAutoPosYJumelle;
            }
        }

        if (ModeAutoJumelle & 2) {
            if ((u32)JumelleZoomAmp < (u32)ModeAutoJumelleZoomAmp) {
                ChangeDeltaZoomAmpJumelle(300);
                if ((u32)JumelleZoomAmp > (u32)ModeAutoJumelleZoomAmp) {
                    JumelleZoomAmp = ModeAutoJumelleZoomAmp;
                }
            } else if ((u32)JumelleZoomAmp > (u32)ModeAutoJumelleZoomAmp) {
                ChangeDeltaZoomAmpJumelle(-300);
                if ((u32)JumelleZoomAmp < (u32)ModeAutoJumelleZoomAmp) {
                    JumelleZoomAmp = ModeAutoJumelleZoomAmp;
                }
            }
        }

        if (ModeAutoJumelle & 4) {
            if ((u32)RayonJumelle < (u32)ModeAutoRayonJumelle) {
                ChangeDeltaSizeJumelle(1);
                ModeAutoJumelleZoomAmp = 26 * -RayonJumelle + 32150;
            } else if ((u32)RayonJumelle > (u32)ModeAutoRayonJumelle) {
                ChangeDeltaSizeJumelle(-1);
                ModeAutoJumelleZoomAmp = 26 * -RayonJumelle + 32150;
            }
        }

    }
}


void CalcObjPosInWorldMap(obj_t* obj) {
    obj->screen_x = obj->x - xmap + 8 + JumellePosX - JumelleXMin;
    obj->screen_y = obj->y - ymap + JumellePosY - JumelleYMin;
}

void MoveRayInWorldMap(void) {
    h_scroll_speed = 0;
    v_scroll_speed = 0;
    ray.x += ray.speed_x;
    decalage_en_cours = ray.speed_x;
    ray.y += ray.speed_y;
}
