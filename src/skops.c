/* 1:678f0..1:6a340: screen.c (background bands, heat effect) + skops.c (Mr Skops boss); one or two original objects (swap starts at aligned 1:68db0 - split OPEN). Names RAYVERSE/ANDROID/PS1; sko_* bindings partly recovered from DOS fixups (docs/CENSUS.md). Seeded by tools/seed.py, worker tu_skops. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "screen_types.h"
#ifndef MAX_2
#define MAX_2(x, y) if (x < y) x = y;
#endif
#ifndef ABS
#define ABS(x) (x >= 0 ? (x) : -(x))
#endif
#ifndef SGN
#define SGN(x) (x >= 0 ? (x > 0 ? 1 : 0) : -1)
#endif
#ifndef ashl16
#define ashl16(v, b) ((v) << (b))
#endif

#define ANIMMODE_NONE 0
#define TYPE_SCORPION 150
#define TYPE_RAYON 170
#define TYPE_RUBIS 188
#define TYPE_LAVE 234
#define TYPE_SKO_PINCE 248
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define world_3_mountain 3
#define world_4_image 4
#define world_5_cave 5

extern bande_t Bande[300];
extern def_bande_t Def_Bande[18000];
extern def_sprite_t Def_Sprite[1200];
extern def_sprite_t Sprite[20];
extern u8 Tab_NbBande[60];
extern u8 Tab_NbSprite[60];
extern u8 Type_Scroll[60];
extern s16 NbBande;
extern s16 NbSprite;
extern s16 Num_Fond;
extern s16 Val_Div_Scroll_Y;
extern s16 Val_Add_Scroll_Y;
extern s16 PosLave_Y;
extern s32 sko_ecran_tremble;
extern u8 horloge[25];
extern u8 ModeDemo;
extern u8 gele;
extern s16 ray_mode;
extern s16 scroll_end_y;
extern s16 poing_obj_id;
extern poing_t poing;
extern s16 jump_time;
extern s16 xmap;
extern u8 SinTabFeu[16];
extern u8 data_22d948;
extern s16 rayon_speed_x[8];
extern s16 rayon_speed_y[8];
extern s16 D_801E4E10;
extern s16 rubis_list_calculated;
extern s16 sko_pince_obj_id;
extern u8 sko_nb_lave;
extern u8 sko_lave_obj[20];
extern s32 pixels_enfonce;
extern obj_t *ScrollDiffSprites;
extern u8 *Scroll_Masque[20];
extern u8 *draw_buffer;
extern s32 BufferNormalDeplt;
extern void (*Copy_Plan0_To_Buf)(u8 *, u8 *, s32, s32, s32);
extern u8 scrollLocked;
extern map_data_t mp;
extern s16 num_world;
extern s16 num_level;
extern s16 ymap;
extern s16 ymapmax;
extern s16 rideau_obj_id;
void DrawSpriteDiffNormal(s32 x, s32 y, vec2b_t size, u8 *mask, u8 *draw_buf, u8 *image_data);
void Init_Effet_Chaleur(s32 width, s32 height, u8 *source_buf, u8 *dest_buf);
void Do_Effet_Chaleur(s16 width, s16 height);
void Display_Sprite_On_Front(s32 plan_width, s32 plan_height, s32 w1, s32 h1, s32 w2, s32 h2);

extern u8 ecroule_plat_index;
extern u8 ecroule_rubis_list[8];
extern u8 ecroule_rubis_order[24];
extern finbosslevel_t finBosslevel;
extern u8 fin_boss;
extern level_t level;
extern obj_t ray;
extern s16 screen_trembling;
extern s16 sko_ecroule_plat;
extern u8 sko_enfonce_enable;
extern s16 sko_last_action;
extern s16 sko_nb_frap;
extern s16 sko_nb_hit;
extern s16 sko_phase;
extern s16 sko_rayon_on;
extern s16 sko_rayon_x;
extern s16 sko_rayon_y;
extern s16 sko_rayon_dx;
extern s16 sko_rayon_dy;
extern s16 sko_final_x;
extern s16 sko_final_y;
extern u8 data_23af64;
s16 BOX_IN_COLL_ZONES(u16 type, s16 x, s16 y, s16 w, s16 h, obj_t* obj);
void TEST_SIGNPOST(void);
void set_main_etat(obj_t* obj, u8 etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void PlaySnd(s32 sound, s32 object_id);
void DO_ONE_CMD(obj_t *obj);
void RAY_HIT(s32 force, obj_t *obj);
void add_alwobj(obj_t *obj);
obj_t *allocateNOVA(void);
void calc_obj_pos(obj_t *obj);
void start_pix_gerbe(s16 x, s16 y);
obj_t *findfirstObject(s16 type);
s16 myRand(s16 max);
void set_rubis_list(void);
void allocate_rayon(s16 x, s16 y);
void allocate_8_petits_rayons(s16 x, s16 y);
void do_sko_rayon(void);
void do_sko_rayon2(void);
void start_sko_rayon(s16 x, s16 y);
void start_sko_rayon2(s16 x, s16 y);
void lance_pince(obj_t *obj);
void swap(s32 ind_1, s32 ind_2);
s32 sko_get_eject_sens(void);
void DO_SOL_ENFONCE(void);
void DO_SKO_PHASE_0(obj_t *sko_obj);
void DO_SKO_PHASE_1(obj_t *obj);
void DO_SKO_PHASE_2(obj_t *obj);
void DO_SKO_PHASE_3(obj_t *obj);
void DO_SKO_PINCE(obj_t *obj);
void DO_SCORPION_COLLISION(obj_t *obj);
void DO_SCORPION_MORT(obj_t *obj);
void DO_SKO(obj_t *obj);
void SKO_ray_in_zone(obj_t *obj);
void DO_SKO_HIT(obj_t *obj);
s16 Get_PosLave_Y(void);

void Init_Bande(u8 fnd, s32 width, u16 height, u8* source_buf, u8* dest_buf) {
    s16 cumulative_length;
    s32 i;
    u8 type_scroll;

    // NB. Decided to initialize Def_Bande at runtime instead of compile-time because it's a very large and sparse array.
    Num_Fond = fnd;

    if (height > 200) {
        Val_Div_Scroll_Y = (16 * mp.height - 200) / (height - 200);
    } else {
        Val_Div_Scroll_Y = 1;
    }
    if (Val_Div_Scroll_Y == 0) {
        Val_Div_Scroll_Y = 2;
    } else {
        ++Val_Div_Scroll_Y;
    }
    Val_Add_Scroll_Y = height - 200 - (16 * mp.height - 200) / Val_Div_Scroll_Y - 1;

    if (num_world == world_5_cave && num_level == 10) {
        Init_Effet_Chaleur(width, height, source_buf, dest_buf);
        return;
    }

    NbBande = Tab_NbBande[10 * (num_world - 1) + Num_Fond];

    cumulative_length = 0;
    for (i = 0; i < NbBande; ++i) {
        def_bande_t* def_bande = Def_Bande + 3000 * (num_world - 1) + 300 * Num_Fond + i;
        bande_t* bande = Bande + i;
        bande->length = def_bande->length;
        bande->field_8 = def_bande->field_2;
        bande->field_9 = def_bande->field_4;
        bande->field_4 = def_bande->field_6;
        bande->field_6 = 0;
        bande->field_14 = 0;
        bande->offset = cumulative_length;
        type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
        if (type_scroll == 0 || type_scroll == 2) {
            bande->source_buffer_pos = source_buf + cumulative_length * width;
            bande->draw_buffer_pos = dest_buf + cumulative_length * 320;
        } else {
            bande->source_buffer_pos = source_buf + cumulative_length;
            bande->draw_buffer_pos = dest_buf + cumulative_length;
        }

        cumulative_length += bande->length;
    }

    NbSprite = Tab_NbSprite[10 * (num_world - 1) + Num_Fond];
    for (i = 0; i < NbSprite; ++i) {
        def_sprite_t* def_sprite = Def_Sprite + 200 * (num_world - 1) + 20 * Num_Fond + i;
        def_sprite_t* sprite = Sprite + i;
        sprite->x = def_sprite->x;
        sprite->y = def_sprite->y;
        sprite->bande_index = def_sprite->bande_index;
    }
}

void Display_Back_Screen(s32 plan_width, s32 plan_height, s32 w1, s32 h1, s32 w2, s32 h2) {
    s32 y;
    s32 v68;
    s32 i;
    u8 type_scroll;

    y = h1;
    type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
    if (type_scroll == 0 || type_scroll == 2) {
        if (num_world == world_4_image && (num_level == 4 || num_level == 11)) {
            obj_t* rideau = level.objects + rideau_obj_id;
            if (rideau->is_active) {
                if (h1 < rideau->y - 110 - (SCREEN_HEIGHT - h2)) {
                    y = rideau->y - 110 - (SCREEN_HEIGHT - h2);
                    if (y < 0) {
                        y = 0;
                    }
                }
                if (y >= h2) {
                    y = h2 - 1;
                }
            }
        }
        if (type_scroll != 0) {
            v68 = plan_height - h2;
        } else if (num_world == world_3_mountain && num_level == 10) {
            v68 = SCREEN_HEIGHT - h2 + (ymap >> 1) + 11;
        } else {
            v68 = Val_Add_Scroll_Y + ymap / Val_Div_Scroll_Y;
        }
        if (v68 > SCREEN_HEIGHT - h2 + (plan_height - SCREEN_HEIGHT)) {
            v68 = plan_height - h2;
        }
        for (i = 0; i < NbBande; ++i) {
            bande_t* bande = Bande + i;
            u8* source_buffer_pos = bande->source_buffer_pos + bande->field_6 + w1;
            u8* dest_buffer_pos = bande->draw_buffer_pos - SCREEN_WIDTH * v68 + w1;
            s32 draw_height = bande->length;
            s32 v34 = bande->offset - v68;
            s32 v36 = bande->length + v34;
            if (v34 < 0 && v36 > 0 && bande->length != 0) {
                draw_height = v36;
                dest_buffer_pos -= SCREEN_WIDTH * v34;
                source_buffer_pos -= plan_width * v34;
                v34 = 0;
            }
            if (y < v36 && v34 < h2) {
                if (y <= v34) {
                    if (h2 < v36) {
                        draw_height = h2 - v34;
                    }
                    Copy_Plan0_To_Buf(source_buffer_pos - BufferNormalDeplt, dest_buffer_pos, plan_width, draw_height, w2 - w1);
                } else {
                    Copy_Plan0_To_Buf(source_buffer_pos + (y - v34) * plan_width - BufferNormalDeplt, dest_buffer_pos + SCREEN_WIDTH * (y - v34), plan_width, draw_height - (y - v34), w2 - w1);
                }
            }
        }
        plan_width /= 2;
        for (i = 0; i < NbSprite; ++i) {
            def_sprite_t* sprite = Sprite + i;
            bande_t* bande = Bande + sprite->bande_index;
            if (bande->length != 0) {
                s32 source_x;
                sprite_t* scroll_diff_sprite;

                source_x = sprite->x - bande->field_6;
                if (source_x > SCREEN_WIDTH) {
                    source_x -= plan_width;
                }
                scroll_diff_sprite = ScrollDiffSprites->sprites + (i + 1);
                if (source_x + scroll_diff_sprite->outer_width > w1) {
                    vec2b_t size;
                    size.x = scroll_diff_sprite->outer_width;
                    size.y = scroll_diff_sprite->outer_height;
                    DrawSpriteDiffNormal(source_x, sprite->y - v68, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
                // Draw a second sprite further to the right if >1 fit on screen
                if (source_x + plan_width < w2) {
                    vec2b_t size;
                    size.x = scroll_diff_sprite->outer_width;
                    size.y = scroll_diff_sprite->outer_height;
                    DrawSpriteDiffNormal(source_x + plan_width, sprite->y - v68, size, Scroll_Masque[i + 1],
                                         draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
            }
        }

    } else {
        // type_scroll == 1
        for (i = 0; i < NbBande; ++i) {
            bande_t* bande = Bande + i;
            s16 v24_4 = plan_height - bande->field_6 - h1;
            if (bande->offset + bande->length > w1 && bande->offset < w2) {
                if (bande->length != 0) {
                    s32 v67 = bande->length;
                    u8* source_buffer_pos = bande->source_buffer_pos + bande->field_6 * plan_width + h1 * plan_width;
                    u8* dest_buffer_pos = bande->draw_buffer_pos + SCREEN_WIDTH * h1;
                    s32 v14;
                    s32 v15;
                    s32 v18;
                    if (bande->offset + bande->length <= w2) {
                        if (bande->offset < w1) {
                            source_buffer_pos += w1 - bande->offset;
                            dest_buffer_pos += w1 - bande->offset;
                            v67 = bande->length + bande->offset - w1;
                        }
                    } else if (bande->offset >= w1) {
                        v67 = w2 - bande->offset;
                    } else {
                        source_buffer_pos += w1 - bande->offset;
                        dest_buffer_pos += w1 - bande->offset;
                        v67 = w2 - w1;
                    }
                    v14 = h2 - h1;
                    v15 = plan_height - bande->field_6 - h1;
                    if (v15 > v14) {
                        v15 = v14;
                    }
                    if (v15 <= 0) {
                        source_buffer_pos -= plan_width * v15;
                        v18 = v14;
                    } else {
                        Copy_Plan0_To_Buf(source_buffer_pos, dest_buffer_pos + BufferNormalDeplt, plan_width, v15, v67);
                        source_buffer_pos = bande->source_buffer_pos;
                        dest_buffer_pos = bande->draw_buffer_pos + SCREEN_WIDTH * h1 + SCREEN_WIDTH * v15;
                        v18 = v14 - v15;
                    }
                    if (bande->offset < w1) {
                        source_buffer_pos += w1 - bande->offset;
                        dest_buffer_pos += w1 - bande->offset;
                    }
                    if (v14 < v18) {
                        v18 = v14;
                    }
                    if (v18 > 0) {
                        Copy_Plan0_To_Buf(source_buffer_pos, dest_buffer_pos + BufferNormalDeplt, plan_width, v18, v67);
                    }
                }
            }
        }
        for (i = 0; i < NbSprite; ++i) {
            def_sprite_t* sprite = Sprite + i;
            bande_t* bande = Bande + sprite->bande_index;
            if (bande->length != 0) {
                s32 source_y = sprite->y - bande->field_6;
                sprite_t* scroll_diff_sprite;
                if (source_y > SCREEN_HEIGHT) {
                    source_y -= plan_height;
                }
                scroll_diff_sprite = ScrollDiffSprites->sprites + (i + 1);
                if (source_y + scroll_diff_sprite->outer_height > h1) {
                    vec2b_t size;
                    size.x = scroll_diff_sprite->outer_width;
                    size.y = scroll_diff_sprite->outer_height;
                    DrawSpriteDiffNormal(sprite->x, source_y, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
                // Draw a second sprite further to the right if >1 fit on screen
                if (source_y + plan_height < h2) {
                    vec2b_t size;
                    size.x = scroll_diff_sprite->outer_width;
                    size.y = scroll_diff_sprite->outer_height;
                    DrawSpriteDiffNormal(sprite->x, source_y + plan_height, size, Scroll_Masque[i + 1],
                                         draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
            }
        }
    }
}

void Display_Sprite_On_Front(s32 plan_width, s32 plan_height, s32 w1, s32 h1, s32 w2, s32 h2) {
    s32 i;
    s32 y_scroll;
    u8 type_scroll;

    type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
    y_scroll = 2 * ymapmax - 2 * ymap + SCREEN_HEIGHT;
    plan_width /= 2;
    if (type_scroll == 0 || type_scroll == 2) {
        for (i = 0; i < NbSprite; ++i) {
            def_sprite_t *sprite;
            bande_t *bande;
            sprite_t *scroll_diff_sprite;
            s32 source_x;
            s32 source_y;
            vec2b_t size;

            sprite = Sprite + i;
            bande = Bande + sprite->bande_index;
            if (bande->length == 0) {
                source_x = sprite->x - bande->field_6;
                if (source_x > SCREEN_WIDTH) {
                    source_x -= plan_width;
                }
                scroll_diff_sprite = ScrollDiffSprites->sprites + i + 1;
                source_y = sprite->y - y_scroll;
                size.x = scroll_diff_sprite->outer_width;
                size.y = scroll_diff_sprite->outer_height;
                if (source_x + size.x > w1) {
                    DrawSpriteDiffNormal(source_x, source_y, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
                if (source_x + plan_width < w2) {
                    DrawSpriteDiffNormal(source_x + plan_width, source_y, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
            }
        }
    } else {
        for (i = 0; i < NbSprite; ++i) {
            def_sprite_t *sprite;
            bande_t *bande;
            sprite_t *scroll_diff_sprite;
            s32 source_y;
            vec2b_t size;

            sprite = Sprite + i;
            bande = Bande + sprite->bande_index;
            if (bande->length != 0) {
                source_y = sprite->y - bande->field_6;
                if (source_y > SCREEN_HEIGHT) {
                    source_y -= plan_height;
                }
                scroll_diff_sprite = ScrollDiffSprites->sprites + i + 1;
                size.x = scroll_diff_sprite->outer_width;
                size.y = scroll_diff_sprite->outer_height;
                if (source_y + size.y > h1) {
                    DrawSpriteDiffNormal(sprite->x, source_y, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
                if (source_y + plan_height < h2) {
                    DrawSpriteDiffNormal(sprite->x, source_y + plan_height, size, Scroll_Masque[i + 1], draw_buffer,
                                         ScrollDiffSprites->img_buffer + scroll_diff_sprite->offset_in_atlas);
                }
            }
        }
    }
}

void Calcul_Deplacement_Bande(s16 x, s32 plan_width, s32 plan_height) {
    s32 i;
    u8 type_scroll = Type_Scroll[10 * (num_world - 1) + Num_Fond];
    if (type_scroll != 0) {
        if (type_scroll == 1) {
            for (i = 0; i < NbBande; ++i) {
                bande_t* bande = Bande + i;
                if (bande->length != 0) {
                    if (bande->field_4 != 0) {
                        if (bande->field_4 == 1 || bande->field_4 == 4) {
                            bande->field_14 = (bande->field_9 + bande->field_14) % (16 * plan_height);
                            if (bande->field_4 != 1) {
                                bande->field_6 = (ymap + plan_height * bande->field_8) / bande->field_8 % plan_height;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_height;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        } else if (bande->field_4 == 2 || bande->field_4 == 5) {
                            if (bande->field_9 > bande->field_14) {
                                bande->field_14 += 16 * plan_height;
                            }
                            bande->field_14 -= bande->field_9;
                            if (bande->field_4 != 2) {
                                bande->field_6 = (ymap + plan_height * bande->field_8) / bande->field_8 % plan_height;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_height;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        }
                    } else {
                        bande->field_6 = (ymap + plan_height * bande->field_8) / bande->field_8 % plan_height;
                    }
                } else {
                    bande->field_6 = (2 * ymap) % plan_height;
                }
            }
        }
    } else {
        // type_scroll == 0
        plan_width /= 2;
        if (NbBande != 0) {
            for (i = 0; i < NbBande; ++i) {
                bande_t* bande = Bande + i;
                if (bande->length != 0) {
                    if (bande->field_4 != 0) {
                        if (bande->field_4 == 1 || bande->field_4 == 4) {
                            bande->field_14 = (bande->field_9 + bande->field_14) % (16 * plan_width);
                            if (bande->field_4 != 1) {
                                bande->field_6 = (x + plan_width * bande->field_8) / bande->field_8 % plan_width;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_width;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        } else if (bande->field_4 == 2 || bande->field_4 == 5) {
                            if (bande->field_9 > bande->field_14) {
                                bande->field_14 += 16 * plan_width;
                            }
                            bande->field_14 -= bande->field_9;
                            if (bande->field_4 != 2) {
                                bande->field_6 = (x + plan_width * bande->field_8) / bande->field_8 % plan_width;
                                bande->field_6 += bande->field_14 >> 4;
                                bande->field_6 = bande->field_6 % plan_width;
                            } else {
                                bande->field_6 = bande->field_14 >> 4;
                            }
                        }
                    } else {
                        bande->field_6 = (x + plan_width * bande->field_8) / bande->field_8 % plan_width;
                    }
                } else {
                    bande->field_6 = (2 * x) % plan_width;
                }
            }
        }
        if (num_world == world_5_cave && num_level == 10) {
            Do_Effet_Chaleur(plan_width, plan_height);
        }
    }
}

void Init_Effet_Chaleur(s32 width, s16 height, u8* source_buf, u8* dest_buf) {
    s32 i;

    NbBande = height;
    for (i = 0; i < NbBande; ++i) {
        bande_t *bande = Bande + i;
        bande->length = 1;
        bande->field_8 = 2;
        bande->field_9 = 0;
        bande->field_4 = 0;
        bande->field_6 = 0;
        bande->field_14 = 0;
        bande->offset = i;
        bande->source_buffer_pos = source_buf + i * width;
        bande->draw_buffer_pos = dest_buf + i * SCREEN_WIDTH;
    }
    NbSprite = 0;
}

void Do_Effet_Chaleur(s16 width, s16 height) {
    s32 start_y;
    s32 y;
    s32 counter;
    s32 phase;

    start_y = jump_time >> 2;
    if (start_y > height - 201) {
        start_y = height - 201;
    }
    start_y += Get_PosLave_Y() - ymap - 20;
    if (gele == 0) {
        ++data_22d948;
        if (data_22d948 == 16) {
            data_22d948 = 0;
        }
    }
    phase = data_22d948;
    counter = 90;
    for (y = start_y; y < height + 90; ++y) {
        --counter;
        if (counter < 16) {
            counter = 16;
        }
        if (y > 0) {
            Bande[y].field_6 = (Bande[y].field_6 + width + SinTabFeu[phase] * 16 / counter) % width;
        }
        ++phase;
        if (phase == 16) {
            phase = 0;
        }
    }
}


void swap(s32 ind_1, s32 ind_2)
{
    u8 temp = ecroule_rubis_list[ind_1];
    ecroule_rubis_list[ind_1] = ecroule_rubis_list[ind_2];
    ecroule_rubis_list[ind_2] = temp;
}

void set_rubis_list(void)
{
    s32 rubis_ind;
    s32 i;
    obj_t *cur_obj;
    s32 nb_objs;
    u16 type;
    s32 swapped;
    s32 j;
    s16 *mp_height;
    u8 *cur_lave_obj;

    rubis_list_calculated = 1;
    rubis_ind = 0;
    sko_nb_lave = 0;
    i = 0;
    cur_obj = &level.objects[i];
    nb_objs = level.nb_objects;
    while (i < nb_objs) {
        type = cur_obj->type;
        if (type == TYPE_RUBIS) {
            ecroule_rubis_list[rubis_ind] = cur_obj->id;
            rubis_ind++;
        } else if (type == TYPE_SKO_PINCE) {
            sko_pince_obj_id = i;
        } else if (type == TYPE_LAVE) {
            sko_lave_obj[sko_nb_lave] = i;
            sko_nb_lave++;
        }
        i++;
        cur_obj++;
    }

    do {
        swapped = 0;
        for (j = 0; j < 7; j++) {
            if (level.objects[ecroule_rubis_list[j]].x >
                level.objects[ecroule_rubis_list[j + 1]].x) {
                swap(j, j + 1);
                swapped = 1;
            }
        }
    } while (swapped);

    if (ModeDemo != 0 || data_23af64 != 0) {
        ecroule_plat_index = 8;
    } else {
        ecroule_plat_index = (s16)myRand(177) % 3 << 3;
    }
    sko_ecroule_plat = ecroule_rubis_list[ecroule_rubis_order[ecroule_plat_index]];
    sko_enfonce_enable = 0;

    mp.height = 32;
    ymapmax = 272;
    scroll_end_y = 272;
    sko_ecran_tremble = 0;
    if (sko_nb_lave != 0) {
        mp_height = &mp.height;
        cur_lave_obj = &sko_lave_obj[0];
        do {
            level.objects[*cur_lave_obj].init_y = *mp_height * 16 - 60;
            level.objects[*cur_lave_obj].y = level.objects[*cur_lave_obj].init_y;
            cur_lave_obj++;
        } while (cur_lave_obj < &sko_lave_obj[sko_nb_lave]);
    }
}

void allocate_rayon(s16 x, s16 y)
{
    s16 found;
    s16 i;
    obj_t *cur_obj;
    u16 nb_objs;

    found = 0;
    i = 0;
    cur_obj = &level.objects[i];
    nb_objs = level.nb_objects;
    do {
        if (cur_obj->type == TYPE_RAYON && cur_obj->is_active == 0)
            found = 1;
        cur_obj++;
        i++;
    } while (!found && i < nb_objs);
    if (found) {
        cur_obj--;
        cur_obj->flags.alive = 1;
        cur_obj->is_active = 1;
        add_alwobj(cur_obj);
        cur_obj->flags.flag_0x40 = 0;
        cur_obj->x = x;
        cur_obj->y = y;
        cur_obj->speed_x = 0;
        cur_obj->speed_y = 0;
        set_main_etat(cur_obj, 3);
        set_sub_etat(cur_obj, 0);
    }
}

void allocate_8_petits_rayons(s16 x, s16 y)
{
    obj_t *found_obj;
    s16 i;
    obj_t *rayon_obj;

    found_obj = allocateNOVA();
    if (found_obj != 0) {
        found_obj->x = x + 64;
        found_obj->y = y + 64;
        found_obj->display_prio = 1;
        found_obj->init_x = 0;
        found_obj->is_active = 1;
        calc_obj_pos(found_obj);
    }
    start_pix_gerbe(x + 140, y + 32);
    found_obj = findfirstObject(TYPE_RAYON);
    i = 0;
    rayon_obj = &found_obj[10];
    while (i < 8) {
        rayon_obj->x = x + 8;
        rayon_obj->y = y;
        rayon_obj->main_etat = 3;
        rayon_obj->sub_etat = 2;
        rayon_obj->flags.alive = 1;
        rayon_obj->is_active = 1;
        rayon_obj->speed_x = rayon_speed_x[i];
        rayon_obj->speed_y = rayon_speed_y[i];
        rayon_obj->anim_frame = 2;
        i++;
        rayon_obj++;
    }
}

void do_sko_rayon(void)
{
    obj_t *ecroule_obj;
    s16 diff_x;
    s16 diff_y;
    s16 dist;
    s16 unk_1;

    if (sko_rayon_on != 0) {
        D_801E4E10 = 3;
        sko_rayon_on--;
        ecroule_obj = &level.objects[sko_ecroule_plat];
        diff_y = sko_final_y - sko_rayon_y;
        diff_x = sko_final_x - sko_rayon_x;
        dist = ABS(diff_x) + ABS(diff_y);
        if (dist < 8) {
            set_main_etat(ecroule_obj, 2);
            set_sub_etat(ecroule_obj, 2);
        }
        if (dist > 0) {
            diff_x = ashl16(diff_x, D_801E4E10) / dist;
            diff_y = ashl16(diff_y, D_801E4E10) / dist;
        }
        unk_1 = horloge[4] == 0 && sko_rayon_on < 50;
        if (unk_1) {
            sko_rayon_dx += SGN(diff_x - sko_rayon_dx);
            sko_rayon_dy += SGN(diff_y - sko_rayon_dy);
        }
        sko_rayon_x += sko_rayon_dx;
        sko_rayon_y += sko_rayon_dy;
        if (horloge[3] == 0 && dist != 0)
            allocate_rayon(sko_rayon_x, sko_rayon_y);
    }
}

void do_sko_rayon2(void)
{
    obj_t *poing_obj;
    s16 horl_zero;
    s16 diff_x;
    s16 diff_y;
    s16 dist;

    if (sko_rayon_on != 0) {
        sko_rayon_on--;
        if (poing.is_active) {
            D_801E4E10 = 3;
            poing_obj = &level.objects[poing_obj_id];
            sko_final_x = poing_obj->x + poing_obj->offset_bx - 104;
            sko_final_y = poing_obj->y + poing_obj->offset_hy - 120;
            horl_zero = horloge[2] == 0;
        } else {
            D_801E4E10 = 3;
            sko_final_x = ray.x + ray.offset_bx - 120;
            sko_final_y = ray.y + ray.offset_by - 140;
            horl_zero = horloge[8] == 0;
        }
        diff_y = sko_final_y - sko_rayon_y;
        diff_x = sko_final_x - sko_rayon_x;
        dist = ABS(diff_x) + ABS(diff_y);
        if (dist > 0) {
            diff_x = ashl16(diff_x, (u32)D_801E4E10) / dist;
            diff_y = ashl16(diff_y, (u32)D_801E4E10) / dist;
        }
        if (horl_zero) {
            sko_rayon_dx += SGN(diff_x - sko_rayon_dx);
            sko_rayon_dy += SGN(diff_y - sko_rayon_dy);
        }
        sko_rayon_x += sko_rayon_dx;
        sko_rayon_y += sko_rayon_dy;
        if (sko_rayon_x < -150) {
            sko_rayon_dx = -sko_rayon_dx;
            sko_rayon_x += sko_rayon_dx;
            sko_rayon_x += sko_rayon_dx;
        }
        horl_zero = horloge[3] == 0;
        if (horl_zero && dist != 0)
            allocate_rayon(sko_rayon_x, sko_rayon_y);
        if (sko_rayon_on == 0) {
            sko_rayon_x = -32000;
            sko_rayon_y = -32000;
        }
    }
}

void start_sko_rayon(s16 obj_x, s16 obj_y)
{
    obj_t *ecroule_obj;

    ecroule_plat_index++;
    sko_ecroule_plat = ecroule_rubis_list[ecroule_rubis_order[ecroule_plat_index]];
    ecroule_obj = &level.objects[sko_ecroule_plat];
    if (sko_rayon_on != 0xff)
        sko_rayon_on = 60;
    sko_rayon_x = obj_x - 80;
    sko_rayon_dx = -4;
    sko_rayon_y = obj_y - 10;
    sko_rayon_dy = 4;
    sko_final_x = ecroule_obj->offset_bx + ecroule_obj->x - 120;
    sko_final_y = ecroule_obj->offset_hy + ecroule_obj->y - 120;
    allocate_rayon(sko_rayon_x, sko_rayon_y);
}

void start_sko_rayon2(s16 obj_x, s16 obj_y)
{
    obj_t *final_obj;

    if (poing.is_active)
        final_obj = &level.objects[poing_obj_id];
    else
        final_obj = &ray;
    sko_rayon_on = 120;
    sko_rayon_x = obj_x - 80;
    sko_rayon_dx = -4;
    sko_rayon_y = obj_y - 10;
    sko_rayon_dy = 2;
    sko_final_x = final_obj->offset_bx + final_obj->x - 120;
    sko_final_y = final_obj->offset_hy + final_obj->y - 120;
    allocate_rayon(sko_rayon_x, sko_rayon_y);
}

void lance_pince(obj_t *sko_obj)
{
    obj_t *pince_obj;

    pince_obj = &level.objects[sko_pince_obj_id];
    pince_obj->flags.alive = 1;
    pince_obj->x = sko_obj->x;
    pince_obj->y = sko_obj->y;
    pince_obj->speed_x = -128;
    pince_obj->speed_y = 0;
    calc_obj_pos(pince_obj);
    add_alwobj(pince_obj);
}

s32 sko_get_eject_sens(void)
{
    ray.iframes_timer = 40;
    return -1;
}

void DO_SOL_ENFONCE(void)
{
    s32 i;

    if (ray_mode == 3 ||
        (ray.main_etat == 2 && ray.sub_etat == 9) ||
        (ray.main_etat == 3 && (ray.sub_etat == 23 || ray.sub_etat == 52))) {
        sko_enfonce_enable = 0;
        pixels_enfonce = 0;
    }
    if (sko_ecran_tremble != 0) {
        --sko_ecran_tremble;
        if (horloge[4] >= 2) {
            xmap += 2;
        } else {
            xmap -= 2;
        }
    }
    if (!(pixels_enfonce < 96 && sko_enfonce_enable == 1)) {
        if (pixels_enfonce < 196 && sko_enfonce_enable == 2) {
            goto block_19;
        }
    } else if (horloge[2] != 0) {
block_19:
        if (sko_enfonce_enable == 2 && horloge[4] == 0) {
            goto block_21;
        }
    } else {
        if (horloge[4] >= 2) {
block_21:
            xmap += 3;
        } else {
            xmap -= 3;
        }
        --ymapmax;
        --scroll_end_y;
        ++pixels_enfonce;
        if ((pixels_enfonce & 15) == 0) {
            --mp.height;
        }
        for (i = 0; i < sko_nb_lave; ++i) {
            obj_t *lave_obj;

            lave_obj = &level.objects[sko_lave_obj[i]];
            --lave_obj->y;
            --lave_obj->init_y;
            if (sko_enfonce_enable == 1 && horloge[10] == 0) {
                --lave_obj->y;
                --lave_obj->init_y;
            }
        }
    }
    if (pixels_enfonce == 96 && sko_enfonce_enable == 1) {
        pixels_enfonce = 0;
        sko_enfonce_enable = 0;
        sko_ecran_tremble = 60;
    }
    if (pixels_enfonce == 196 && sko_enfonce_enable == 2) {
        pixels_enfonce = 0;
        sko_enfonce_enable = 0;
        sko_ecran_tremble = 60;
    }
}

void DO_SKO_PHASE_0(obj_t *sko_obj)
{
    s16 sko_sub_etat;
    obj_t *ecroule_obj = &level.objects[sko_ecroule_plat];
    if (sko_obj->main_etat == 0) {
        sko_sub_etat = sko_obj->sub_etat;
        if (sko_sub_etat >= 2) {
            if (sko_sub_etat >= 4) {
                if (sko_sub_etat == 4 && sko_obj->anim_frame == 27 && screen_trembling == 0) {
                    screen_trembling = 1;
                    sko_nb_frap++;
                    if (ecroule_obj->sub_etat == 7) {
                        set_main_etat(ecroule_obj, 2);
                        set_sub_etat(ecroule_obj, 2);
                        ecroule_plat_index++;
                        sko_ecroule_plat = ecroule_rubis_list[ecroule_rubis_order[ecroule_plat_index]];
                    }
                    if (sko_nb_frap == sko_sub_etat)
                        skipToLabel(sko_obj, 4, 1);
                    if (sko_nb_frap == 5) {
                        sko_nb_frap = 0;
                        sko_nb_hit = 0;
                        sko_phase++;
                        skipToLabel(sko_obj, 5, 1);
                        ecroule_plat_index--;
                    }
                }
            } else if (sko_last_action == 4) {
                set_sub_etat(sko_obj, 8);
                sko_last_action = 8;
            } else {
                set_sub_etat(sko_obj, 4);
                skipToLabel(sko_obj, 3, 1);
                sko_last_action = 4;
            }
        }
    }
}

void DO_SKO_PHASE_1(obj_t *obj)
{
    if (obj->main_etat == 0 && obj->sub_etat == 4 &&
        obj->anim_frame == 27 && screen_trembling == 0)
    {
        screen_trembling = 1;
        sko_nb_frap++;
        if (ray.main_etat == 5)
        {
            set_main_etat(&ray, 2);
            set_sub_etat(&ray, 2);
        }
        if (sko_nb_frap == 3)
        {
            sko_rayon_on = 0;
            sko_phase++;
            skipToLabel(obj, 6, 1);
            sko_nb_frap = 0;
            obj->anim_frame = 0;
        }
    }
}

void DO_SKO_PHASE_2(obj_t *obj)
{
    s16 main_etat = obj->main_etat;
    s16 sub_etat = obj->sub_etat;
    do_sko_rayon();
    switch (main_etat)
    {
    case 0:
        switch (sub_etat)
        {
        case 4:
            switch (sko_nb_frap)
            {
            case 0:
                if (obj->anim_frame == 27)
                {
                    sko_nb_frap++;
                    sko_enfonce_enable = 1;
                }
                break;
            case 1:
                if (obj->anim_frame == 27)
                {
                    sko_nb_frap++;
                    screen_trembling = 1;
                }
                break;
            }
            break;
        case 2:
        case 3:
            set_sub_etat(obj, 5);
            break;
        case 7:
            if (obj->anim_frame == 9 && sko_rayon_on == 0)
            {
                start_sko_rayon(obj->x, obj->y);
                if (++sko_nb_frap == 5)
                {
                    skipToLabel(obj, 7, 1);
                    sko_nb_frap = 0;
                    sko_phase++;
                }
            }
            break;
        }
        break;
    default:
        if (
            main_etat == 1 && sub_etat == 15 &&
            obj->nb_cmd == sko_nb_hit
        )
            obj->nb_cmd = 0;
        break;
    }
}

void DO_SKO_PHASE_3(obj_t *obj)
{
    s16 main_etat = obj->main_etat;
    s16 sub_etat = obj->sub_etat;
    do_sko_rayon();
    switch (main_etat)
    {
    case 0:
        switch (sub_etat)
        {
        case 4:
            if (obj->anim_frame == 27 && sko_nb_frap == 1)
            {
                sko_nb_frap++;
                sko_enfonce_enable = 2;
            }
            break;
        case 2:
            if (sko_nb_frap == 0)
            {
                sko_nb_frap++;
                skipToLabel(obj, 11, 1);
            }
            else if (sko_nb_frap == 2)
            {
                set_main_etat(obj, 1);
                set_sub_etat(obj, 15);
                obj->speed_x = 3;
            }
            break;
        }
        break;
    }
}

void DO_SKO_PINCE(obj_t *obj)
{
    u8 sub_etat;
    s32 pince_id;
    obj_t *pince_obj;

    if (obj->main_etat == 0) {
        sub_etat = obj->sub_etat;
        if (sub_etat == 8) {
            if (obj->anim_frame == 65) {
                lance_pince(obj);
                set_sub_etat(obj, 9);
                skipToLabel(obj, 9, 1);
            }
        } else if (sub_etat == 9) {
            pince_id = sko_pince_obj_id;
            pince_obj = &level.objects[pince_id];
            pince_obj->speed_x += 2;
            if (-pince_obj->speed_x == -110) {
                set_sub_etat(obj, 10);
            }
        } else if (sub_etat == 10 && obj->anim_frame == 7) {
            pince_id = sko_pince_obj_id;
            pince_obj = &level.objects[pince_id];
            pince_obj->flags.alive = 0;
            pince_obj->x = -32000;
            pince_obj->y = -32000;
        }
    }
}

void DO_SCORPION_COLLISION(obj_t *obj)
{
    s16 iVar1 = -1;
    s16 rayon_x = sko_rayon_x + 120;
    s16 rayon_y = sko_rayon_y + 120;

    s16 unk_1 = 11;
    if (!(obj->main_etat == 0 && (obj->sub_etat == unk_1 || obj->sub_etat == 12)))
    {
        iVar1 = BOX_IN_COLL_ZONES(
            TYPE_SCORPION,
            rayon_x, rayon_y,
            16, 16,
            obj
        );
    }

    if (iVar1 != -1)
    {
        set_sub_etat(obj, 11);
        allocate_8_petits_rayons(sko_rayon_x, sko_rayon_y);
        sko_rayon_x = 33536;
        sko_rayon_y = 33536;
        sko_rayon_on = 0;
        obj->hit_points--;
        if (obj->hit_points == 1)
            skipToLabel(obj, 13, 1);
        if (obj->hit_points == 0)
            set_sub_etat(obj, 12);
    }
}

void DO_SCORPION_MORT(obj_t *obj)
{
    finBosslevel.mr_skops = 1;
    if (
        obj->main_etat == 0 && obj->sub_etat == 12 &&
        obj->anim_frame >= 127
    )
    {
        obj->anim_frame = 129;
        fin_boss = 1;
        TEST_SIGNPOST();
    }
}

void DO_SKO(obj_t *obj)
{
    s16 main_etat = obj->main_etat;
    s16 sub_etat = obj->sub_etat;
    obj_t *cur_obj;
    s16 i;
    u8 nb_objs;

    if (main_etat == 0 && sub_etat == 1) {
        if (obj->anim_frame == 0) {
            PlaySnd(172, obj->id);
        }
        if (obj->anim_frame == 50) {
            PlaySnd(173, obj->id);
        }
        if (obj->anim_frame == 100) {
            PlaySnd(177, obj->id);
        }
    }
    if (num_level == 10) {
        DO_ONE_CMD(obj);
        if (!rubis_list_calculated) {
            set_rubis_list();
        }
        DO_SKO_PINCE(obj);
        DO_SOL_ENFONCE();
        if (obj->x + obj->offset_bx < ray.x && ray_mode != 3) {
            RAY_HIT(1, obj);
        }
        switch (sko_phase) {
        case 0:
            DO_SKO_PHASE_0(obj);
            break;
        case 1:
            DO_SKO_PHASE_1(obj);
            break;
        case 2:
            DO_SKO_PHASE_2(obj);
            break;
        case 3:
            DO_SKO_PHASE_3(obj);
            break;
        }
    } else if (num_level == 11) {
        if (!rubis_list_calculated) {
            scrollLocked = 1;
            obj->hit_points = obj->init_hit_points;
            cur_obj = level.objects;
            i = 0;
            nb_objs = level.nb_objects;
            while (i < nb_objs) {
                if (cur_obj->type == TYPE_SKO_PINCE) {
                    sko_pince_obj_id = i;
                    break;
                }
                ++cur_obj;
                ++i;
            }
            skipToLabel(obj, 12, 1);
            set_main_etat(obj, 0);
            set_sub_etat(obj, 5);
            rubis_list_calculated = 1;
            sko_phase = 4;
        }
        DO_ONE_CMD(obj);
        do_sko_rayon2();
        DO_SKO_PINCE(obj);
        DO_SCORPION_COLLISION(obj);
        DO_SCORPION_MORT(obj);
        if (main_etat == 0) {
            switch (sub_etat) {
            case 7:
                if (obj->anim_frame == 9 && sko_rayon_on == 0) {
                    sko_rayon_on = 0xff;
                    start_sko_rayon2(obj->x, obj->y);
                }
                break;
            case 2:
            case 3:
                if (sko_last_action == 5) {
                    set_sub_etat(obj, 8);
                    sko_last_action = 8;
                } else {
                    set_sub_etat(obj, 5);
                    sko_last_action = 5;
                }
                break;
            }
        }
    }
}

void SKO_ray_in_zone(obj_t *obj)
{
    s16 main_etat = obj->main_etat;
    s16 sub_etat = obj->sub_etat;
    switch (sko_phase)
    {
    case 0:
        if (main_etat == 0)
        {
            switch (sub_etat)
            {
            case 0:
                set_sub_etat(obj, 1);
                skipToLabel(obj, 2, 1);
                sko_nb_frap = 0;
                break;
            case 2:
            case 3:
                set_sub_etat(obj, 4);
                skipToLabel(obj, 3, 1);
                break;
            }
        }
        break;
    case 1:
        break;
    case 2:
    case 3:
        break;
    }
}

void DO_SKO_HIT(obj_t *obj)
{
    s16 main_etat;
    s16 sub_etat;
    s16 phase;

    main_etat = obj->main_etat;
    phase = sko_phase;
    sub_etat = obj->sub_etat;
    switch (phase)
    {
    case 0:
        if (main_etat == 0)
        {
            switch (sub_etat)
            {
            case 0:
                set_sub_etat(obj, 1);
                skipToLabel(obj, 2, 1);
                obj->change_anim_mode = ANIMMODE_NONE;
                obj->anim_frame = 81;
                sko_nb_frap = 0;
                break;
            case 1:
                MAX_2(obj->anim_frame, 81);
                break;
            case 2:
            case 3:
                set_sub_etat(obj, 4);
                skipToLabel(obj, 3, 1);
                break;
            }
        }
        break;
    case 1:
        if (main_etat == 0)
        {
            obj->x += 2;
            sko_nb_hit++;
            switch (sub_etat)
            {
            case 2:
            case 3:
                obj->nb_cmd = 0;
                break;
            }
        }
        break;
    case 2:
    case 3:
        break;
    }
}

s16 Get_PosLave_Y(void)
{
    return PosLave_Y;
}

