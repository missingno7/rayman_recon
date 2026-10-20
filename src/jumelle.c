/* jumelle.c (1:32930..1:34580, binoculars/world-map zoom; floating point). Names RAYVERSE; worker tu_jumelle first pass. See config/units.json for statuses. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include <string.h>
#include <math.h>

typedef int bool;
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define Abs(x) ((x) >= 0 ? (x) : -(x))
#define sgn(x) ((x) < 0 ? -1 : ((x) > 0))
#ifndef ashl16
#define ashl16(v, b) ((v) << (b))
#endif
#ifndef ashr16
#define ashr16(v, b) ((v) >> (b))
#endif

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

void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
void DisplayAnyPictureNormal(u8 *source_buffer, u8 *dest_buffer, s32 source_x, s32 source_y,
                             s32 dest_x, s32 dest_y, s32 stride, s32 width, s32 height);
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
    s32 v55 = SCREEN_WIDTH * JumelleYMin + JumelleXMin;
    s32 v56 = jumelle_x - 112;
    s32 v7 = jumelle_x - plot_x - circle2_offset;
    s32 v53 = jumelle_y - 64;
    if (plot_x <= circle2_offset) {
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
                    /* DOS zoom mapping is reconstructed from the binary below. */
                } else {
                    memcpy(v29, v30, v60);
                }
            }

            // Top segment of right circle
            if (v59 > 0) {
                u8* v35 = effet_buf + SCREEN_WIDTH * v54 + v57 - v55;
                u8* v36 = draw_buf + SCREEN_WIDTH * v54 + v57;
                if (JumelleZoomActif) {
                    //stub
                } else {
                    memcpy(v36, v35, v59);
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
                    //stub
                } else {
                    memcpy(v41, v42, v60);
                }
            }

            if (v59 > 0) {
                u8* v18 = draw_buf + SCREEN_WIDTH * v52 + v57;
                u8* v24 = effet_buf + SCREEN_WIDTH * v52 + v57 - v55;
                if (JumelleZoomActif) {
                    //stub
                } else {
                    memcpy(v18, v24, v59);
                }
            }
        }
    } else {
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
                    //stub
                } else {
                    memcpy(v10, v11, v58);
                }
            }

            v16 = jumelle_y + plot_y;
            if (v16 >= 0 && v16 < SCREEN_HEIGHT) {
                u8* v10 = draw_buf + SCREEN_WIDTH * v16 + v7;
                u8* v11 = effet_buf + SCREEN_WIDTH * v16 + v7 - v55;
                if (JumelleZoomActif) {
                    //stub
                } else {
                    memcpy(v10, v11, v58);
                }
            }
        }
    }

}


void DisplayJumellesNormal(s32 x, s32 y, s32 rayon, s32 a4, u8* effet_buf, u8* draw_buf) {
    s32 v6 = 0;
    s32 v7 = 0;
    s32 v8 = rayon;
    while (v8 >= v7) {
        plot2linejumelle(x, y, v7, v8, a4, effet_buf, draw_buf);
        plot2linejumelle(x, y, v8, v7, a4, effet_buf, draw_buf);
        v6 = v6 + 2 * v7 + 1;
        ++v7;
        if (v6 >= v8) {
            v6 = v6 - 2 * v8 + 1;
            --v8;
        }
    }
    if (ParamZoomChange) {
        PrepareJumelleZoom();
    }
}


void InitMatriceJumelle(void) {
    if (JumelleEffetAutorise) {
        DistPointX = (s32*)JumelleCalloc(161, sizeof(s32));
        DistPointY = (s32*)JumelleCalloc(65, sizeof(s32));
        ExpPoint = (s32*)JumelleCalloc(5120, sizeof(s32));
        JumelleZoomDef = (s32*)JumelleCalloc(29025, sizeof(s32));
        if (DistPointX && DistPointY && ExpPoint && JumelleZoomDef) {
            s32 y;
            s32 x;
            s32 i;
            for (y = 0; y < 65; ++y) {
                DistPointY[y] = y * y;
                for (x = 0; x < 161; ++x) {
                    DistPointX[x] = x * x;
                }
            }
            for (i = 0; i < 5120; ++i) {
                ExpPoint[i] = (s32)(exp((double)(-i) / 1024.0) * 65536.0);
            }
        } else {
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
        }
    }
    DefaultJumelleVariable();
}


void DefaultJumelleVariable(void) {
    if (JumelleEffetAutorise) {
        JumelleZoomActif = 1;
        ChangeJumelleSizeOK = 3;
    } else {
        JumelleZoomActif = JumelleEffetAutorise;
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
        JumelleZoomAmp = -26 * RayonJumelle + 32150;
        PrepareJumelleZoom();
    }
}

void ChangeDeltaSizeJumelle(s32 delta_size) {
    RayonJumelle += delta_size;
    if (64 < RayonJumelle)
        RayonJumelle = 64;
    if (RayonJumelle < 25)
        RayonJumelle = 25;
    ChangeJumelleVariable();
    if (JumelleZoomActif) {
        ChangeDeltaZoomAmpJumelle(delta_size <= 0 ? 65 : -65);
    }
}

void CalculVitesseJumelle(void) {
    s16 delta_x = (s16)PositionJumelleDemandeX - (s16)JumellePosX;
    s16 delta_y = (s16)PositionJumelleDemandeY - (s16)JumellePosY;
    s16 dist = Abs(delta_y) + Abs(delta_x);
    if (dist > 0) {
        delta_x = ashl16(delta_x, 6) / dist;
        delta_y = ashl16(delta_y, 6) / dist;
    }
    XSpeedJumelle = MAX(MIN(XSpeedJumelle + sgn(delta_x - XSpeedJumelle), 16), -16);
    YSpeedJumelle = MAX(MIN(YSpeedJumelle + sgn(delta_y - YSpeedJumelle), 16), -16);
}

void ChangeDeltaPosXJumelleWithoutLimit(s32 delta_pos_x) {
    if (16 <= delta_pos_x)
        delta_pos_x = 16;
    if (-16 >= delta_pos_x)
        delta_pos_x = -16;

    if (ModeAutoJumelle & 1) {
        Xmap16 = (PositionJumelleX16 & 0xF) | (Xmap16 & ~0xF);
        Xmap16 = MAX(16 * scroll_start_x, MIN(16 * scroll_end_x, delta_pos_x + Xmap16));
        xmap = Xmap16 >> 4;
    } else {
        xmap = MAX(scroll_start_x, MIN(scroll_end_x, xmap + (delta_pos_x >> 4)));
    }

    PositionJumelleX16 += delta_pos_x;
    JumellePosX = PositionJumelleX16 >> 4;
    ChangeJumelleVariable();
}

void ChangeDeltaPosYJumelleWithoutLimit(s32 delta_pos_y) {
    if (16 <= delta_pos_y)
        delta_pos_y = 16;
    if (-16 >= delta_pos_y)
        delta_pos_y = -16;

    if (ModeAutoJumelle & 1) {
        Ymap16 = (PositionJumelleY16 & 0xF) | (Ymap16 & ~0xF);
        Ymap16 = MAX(16 * scroll_start_y, MIN(16 * scroll_end_y, delta_pos_y + Ymap16));
        ymap = Ymap16 >> 4;
    } else {
        ymap = MAX(scroll_start_y, MIN(scroll_end_y, ymap + (delta_pos_y >> 4)));
    }

    PositionJumelleY16 += delta_pos_y;
    JumellePosY = PositionJumelleY16 >> 4;
    ChangeJumelleVariable();
}

s32 ChangeDeltaPosXJumelleWithLimit(s32 delta_pos_x) {
    bool changed;
    if (16 <= delta_pos_x)
        delta_pos_x = 16;
    if (-16 >= delta_pos_x)
        delta_pos_x = -16;

    changed = 0;
    if (((xmap != scroll_start_x && xmap != scroll_end_x) ||
        (xmap == scroll_start_x && delta_pos_x > 0) ||
        (xmap == scroll_end_x && delta_pos_x < 0)) &&
        JumellePosX > RayonJumelle / 3 && JumellePosX < 320 - RayonJumelle / 3
    ) {
        if (ModeAutoJumelle & 1) {
            Xmap16 = (PositionJumelleX16 & 0xF) | (Xmap16 & ~0xF);
            Xmap16 = MAX(16 * scroll_start_x, MIN(16 * scroll_end_x, delta_pos_x + Xmap16));
            xmap = Xmap16 >> 4;
        } else {
            xmap = MAX(scroll_start_x, MIN(scroll_end_x, xmap + (delta_pos_x >> 4)));
        }

        PositionJumelleX16 += delta_pos_x;
        JumellePosX = PositionJumelleX16 >> 4;
        changed = 1;
    }

    ChangeJumelleVariable();
    return changed;
}


s32 ChangeDeltaPosYJumelleWithLimit(s32 delta_pos_y) {
    bool changed;
    if (16 <= delta_pos_y)
        delta_pos_y = 16;
    if (-16 >= delta_pos_y)
        delta_pos_y = -16;

    changed = 0;
    if (((ymap != scroll_start_y && ymap != scroll_end_y) ||
         (ymap == scroll_start_y && delta_pos_y > 0) ||
         (ymap == scroll_end_y && delta_pos_y < 0)) &&
        JumellePosY > RayonJumelle / 3 && JumellePosY < 320 - RayonJumelle / 3
            ) {
        if (ModeAutoJumelle & 1) {
            Ymap16 = (PositionJumelleY16 & 0xF) | (Ymap16 & ~0xF);
            Ymap16 = MAX(16 * scroll_start_y, MIN(16 * scroll_end_y, delta_pos_y + Ymap16));
            ymap = Ymap16 >> 4;
        } else {
            ymap = MAX(scroll_start_y, MIN(scroll_end_y, ymap + (delta_pos_y >> 4)));
        }

        PositionJumelleY16 += delta_pos_y;
        JumellePosY = PositionJumelleY16 >> 4;
        changed = 1;
    }

    ChangeJumelleVariable();
    return changed;
}


void ChangeJumelleVariable(void) {
    s32 half_width;
    s32 height;
    s32 half_height;
    LargeurJumelle = (7 * RayonJumelle) / 2;
    half_width = LargeurJumelle / 2;
    height = 2 * RayonJumelle;
    half_height = height / 2;
    JumelleXMin = JumellePosX - half_width;
    JumelleYMin = JumellePosY - half_height;
    scroll_start_x = (s16)(half_width + 2);
    scroll_start_y = (s16)(half_height + 2);
    scroll_end_x = (s16)(WidthNworld - half_width - 4);
    scroll_end_y = (s16)(HeightNworld - half_height - 4);
    HauteurJumelle = height;

//    scroll_start_x = (half_width + 2);
//    scroll_start_y = (half_height + 2);
//    scroll_end_x = WidthNworld - SCREEN_WIDTH + 32;
//    scroll_end_y = HeightNworld - SCREEN_HEIGHT + 32;
}


void ChangeDeltaZoomAmpJumelle(s32 a1) {
    if ((JumAmpDefPlus + JumAmpDefMul * -RayonJumelle == JumelleZoomAmp &&
        (!(ModeAutoJumelle & 2) || JumelleZoomAmp == ModeAutoJumelleZoomAmp)) || !JumelleEffetAutorise
    ) {
        if (JumelleZoomAmp != 0 && JumelleEffetAutorise) {
            JumelleZoomActif = 1;
            ParamZoomChange = 0;
        } else {
            JumelleZoomActif = 0;
            ParamZoomChange = 0;
        }
    } else {
        JumelleZoomAmp = MAX(0, MIN(-26 * RayonJumelle + 32150, a1 + JumelleZoomAmp));
        JumelleZoomActif = 1;
        ParamZoomChange = 1;
        ChangeJumelleVariable();
    }
}

void PrepareJumelleZoom(void) {
    if (JumelleEffetAutorise) {

    }
}


void RecaleRayPosInJumelle(void) {
    // NOTE: in the PS1 version this is identical to recale_ray_pos()
    //recale_ray_pos(); return;
    if (scroll_y == -1) {
        s32 v13 = ((3 * HauteurJumelle) >> 2) - ray.offset_by;
        if (v_scroll_speed != 255 || decalage_en_cours != 0) {
            s32 v14 = ray.screen_y - v13;
            v_scroll_speed = ashr16(v14, 2);
            if (Abs(ray.speed_y) <= Abs(v_scroll_speed)) {
                s32 v16 = MAX(3, Abs(ray.speed_y));
                if (v_scroll_speed <= 0) {
                    if (v_scroll_speed != 0) {
                        v_scroll_speed = MAX(v_scroll_speed, -v16);
                    } else if (v14 <= 0) {
                        if (v14 != 0) {
                            v_scroll_speed = -1;
                        }
                    } else {
                        v_scroll_speed = 1;
                    }
                } else {
                    v_scroll_speed = MIN(v_scroll_speed, v16);
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

        if (Abs(dhspeed) > 4) {
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
    height += 2;
    width += 2;
    sprite_clipping(0, height, width, 0);
//    DisplayAnyPictureNormal(PLAN2BIT, EffetBufferNormal, xmap - LargeurJumelle/2, ymap - HauteurJumelle/2, 0, 0, WidthNworld, 320, 200);
    DisplayAnyPictureNormal(PLAN2BIT, EffetBufferNormal, xmap - LargeurJumelle/2, ymap - HauteurJumelle/2, 0, 0, WidthNworld, width, height);
}


void DoScrollInWorldMap(s16 h_speed, s16 v_speed) {
    bool need_scroll_xmap = 1;
    bool need_scroll_ymap = 1;
    s32 delta_pos_x = 0;
    s32 delta_pos_y = 0;
    if (ChangeJumelleSizeOK != 4) {
        s16 scroll_limit_x;
        s16 scroll_limit_y;
        if (ray.speed_x != 0 || ray.speed_y != 0) {
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
        } else {
            PositionJumelleDemandeY = SCREEN_HEIGHT / 2;
            PositionJumelleDemandeX = SCREEN_WIDTH / 2;
            delta_pos_x = 16 * (PositionJumelleDemandeX - JumellePosX);
            delta_pos_y = 16 * (PositionJumelleDemandeY - JumellePosY);
            ChangeDeltaPosXJumelleWithoutLimit(delta_pos_x);
            ChangeDeltaPosYJumelleWithoutLimit(delta_pos_y);
        }

        if (need_scroll_xmap) {
            xmap += MAX(-1, MIN(1, h_speed));
        }
        if (need_scroll_ymap) {
            ymap += MAX(-1, MIN(1, v_speed));
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

    if (delta_pos_x == 0 && delta_pos_y == 0 && ray.speed_y == 0 && ray.speed_x == 0 && ChangeJumelleSizeOK != 4) {
        if (RayonJumelle < 64 && !ModeAutoJumelle) {
            ChangeDeltaSizeJumelle(1);
        }

        if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle >= JumelleZoomAmp || ModeAutoJumelle) {
            if (JumAmpDefPlus + JumAmpDefMul * -RayonJumelle > JumelleZoomAmp && !ModeAutoJumelle) {
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
            ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
        } else if (ModeAutoJumelle != 0) {
            if (!(ModeAutoJumelle & 1) && RayonJumelle == 25) {
                if (JumelleZoomAmp == 31500) {
                    ModeAutoJumelle |= 1;
                    RayonJumelle = 25;
                    ModeAutoPosXJumelle = myRand(SCREEN_WIDTH) + 8;
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
        if (JumelleZoomAmp < ModeAutoJumelleZoomAmp) {
            ChangeDeltaZoomAmpJumelle(300);
            if (JumelleZoomAmp > ModeAutoJumelleZoomAmp) {
                JumelleZoomAmp = ModeAutoJumelleZoomAmp;
            }
        } else if (JumelleZoomAmp > ModeAutoJumelleZoomAmp) {
            ChangeDeltaZoomAmpJumelle(-300);
            if (JumelleZoomAmp < ModeAutoJumelleZoomAmp) {
                JumelleZoomAmp = ModeAutoJumelleZoomAmp;
            }
        }
    }

    if (ModeAutoJumelle & 4) {
        if (RayonJumelle < ModeAutoRayonJumelle) {
            ChangeDeltaSizeJumelle(1);
            ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
        } else if (RayonJumelle > ModeAutoRayonJumelle) {
            ChangeDeltaSizeJumelle(-1);
            ModeAutoJumelleZoomAmp = -26 * RayonJumelle + 32150;
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
