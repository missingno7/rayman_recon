/*
 * Fairy (Betilla) dialogue and powers (GOG v1.21 object 1:29210-1:2a1c0). File name invented (Rayverse fee.c).
 * Names: Rayverse/Android. Status per function: config/units.json.
 */
#include <string.h>
#include "obj.h"
#include "game.h"
#include "rtypes.h"
#include "fee_types.h"

#define TYPE_238_POING_FEE 238
#define SCREEN_HEIGHT 200
#define Abs(value) ((value) >= 0 ? (value) : -(value))
#define true 1
#define false 0

extern obj_t ray;
extern level_t level;
extern s16 num_world;
extern s16 num_level;
extern u8 ModeDemo;
extern fee_record_t record;
extern char *language_txt[300];
extern fee_options_jeu_t options_jeu;
extern fee_display_item_t text_to_display[11];
extern rayevts_t RayEvts;
extern s16 png_or_fee_id;
extern s16 decalage_en_cours;
extern s16 remoteRayXToReach;
extern s16 xmap;
extern s16 ymap;
extern u8 horloge[25];
extern u8 is_fee;
extern u8 new_txt_fee;
extern u8 old_txt_fee;
extern u8 display_txt_fee;
extern u8 ray_in_fee_zone;

void INIT_TEXT_TO_DISPLAY(void);
void INIT_TXT_BOX(fee_display_item_t *box);
void Deter_Option_Caract(char *text, s16 key, s16 offset);
void add_alwobj(obj_t *obj);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void set_sub_etat(obj_t *obj, u8 sub_etat);
void set_main_and_sub_etat(obj_t *obj, u8 etat, u8 sub_etat);
u8 EOA(obj_t *obj);

//39210
void INIT_TXT_FEE(void) {
    s16 i;
    fee_display_item_t *item;

    for (i = 0; i <= 9; ++i) {
        item = &text_to_display[i + 1];
        if (strlen(item->text) != 0) {
            text_to_display[i + 1].font_size = 2;
            text_to_display[i + 1].ypos = 188;
            text_to_display[i + 1].field_D5 = 0;
            text_to_display[i + 1].is_fond = 0;
            text_to_display[i + 1].xpos = 150;
            item->color = 5;
            INIT_TXT_BOX(item);
            text_to_display[i + 1].xpos = text_to_display[i + 1].centered_x_pos;
            text_to_display[i + 1].text[0] = ' ';
        } else {
            text_to_display[i + 1].field_D5 = 0;
        }
    }
    display_txt_fee = -1;
    old_txt_fee = -1;
}

//392AC
void allocate_poing_or_free(void) {
    obj_t *obj;
    s16 nb_objects;
    s16 i;

    obj = level.objects;
    nb_objects = level.nb_objects;
    i = 0;
    for (; i < nb_objects; ++i, ++obj) {
        if (obj->type == TYPE_238_POING_FEE && !obj->flags.alive) {
            obj->flags.alive = 0;
            obj->is_active = 0;
            add_alwobj(obj);
            obj->speed_x = 8;
            obj->speed_y = 0;
            obj->flags.flip_x = 1;
            obj->anim_frame = 0;
            png_or_fee_id = obj->id;
            return;
        }
    }
}

//3932C
void init_fee(void) {
    display_txt_fee = -1;
    if (!ModeDemo) {
        record.is_playing = 0;
    }
    is_fee = 0;
    INIT_TEXT_TO_DISPLAY();
    switch (num_world) {
    case 1:
        switch (num_level) {
        case 3: {
            is_fee = 1;
            strcpy(text_to_display[1].text, language_txt[44]); // /hello rayman/
            strcpy(text_to_display[2].text, language_txt[45]); // /i'm betilla the fairy/
            strcpy(text_to_display[3].text, language_txt[46]); // /and i'm going to help you/
            strcpy(text_to_display[4].text, language_txt[47]); // /in your quest./
            strcpy(text_to_display[5].text, language_txt[48]); // /i can give you a new power./
            strcpy(text_to_display[6].text, language_txt[49]); // /now you can punch with your fist/
            strcpy(text_to_display[7].text, language_txt[50]); // /good luck!/
            strcpy(text_to_display[8].text, language_txt[51]); // /press the $ button to punch/
            strcpy(text_to_display[9].text, language_txt[52]); // /the longer you press/
            strcpy(text_to_display[10].text, language_txt[53]); // /the further you punch/
            Deter_Option_Caract(text_to_display[8].text, options_jeu.fist, 255);
            break;
        }
        case 8: {
            is_fee = 1;
            strcpy(text_to_display[1].text, language_txt[54]); // /you're courageous rayman/
            strcpy(text_to_display[2].text, language_txt[55]); // /but you will have to keep at it/
            strcpy(text_to_display[3].text, language_txt[56]); // (empty string)
            strcpy(text_to_display[4].text, language_txt[57]); // (empty string)
            strcpy(text_to_display[5].text, language_txt[58]); // (empty string)
            strcpy(text_to_display[6].text, language_txt[59]); // /you now have the power to hang/
            strcpy(text_to_display[7].text, language_txt[60]); // (empty string)
            strcpy(text_to_display[8].text, language_txt[61]); // (empty string)
            strcpy(text_to_display[9].text, language_txt[62]); // (empty string)
            strcpy(text_to_display[10].text, language_txt[63]); // (empty string)
            break;
        }
        case 17: {
            is_fee = 1;
            strcpy(text_to_display[1].text, language_txt[64]); // /a gloomy part of your quest/
            strcpy(text_to_display[2].text, language_txt[65]); // /will start now.../
            strcpy(text_to_display[3].text, language_txt[66]); // /i hope this will help you/
            strcpy(text_to_display[4].text, language_txt[67]); // /to avoid the worst./
            strcpy(text_to_display[5].text, language_txt[68]); // (empty string)
            strcpy(text_to_display[6].text, language_txt[69]); // /now you can grab things/
            strcpy(text_to_display[7].text, language_txt[70]); // /with your fist./
            strcpy(text_to_display[8].text, language_txt[71]); // (empty string)
            strcpy(text_to_display[9].text, language_txt[72]); // /to grab use the $ button/
            strcpy(text_to_display[10].text, language_txt[73]); // (empty string)
            Deter_Option_Caract(text_to_display[8].text, options_jeu.fist, 255);
            break;
        }
        }
        break;
    case 2:
        if (num_level == 11) {
            is_fee = 1;
            strcpy(text_to_display[1].text, language_txt[74]); // /things are not going to get/
            strcpy(text_to_display[2].text, language_txt[75]); // /any easier for you rayman.../
            strcpy(text_to_display[3].text, language_txt[76]); // /but this new power /
            strcpy(text_to_display[4].text, language_txt[77]); // /will make you stronger./
            strcpy(text_to_display[5].text, language_txt[78]); // (empty string)
            strcpy(text_to_display[6].text, language_txt[79]); // (empty string)
            strcpy(text_to_display[7].text, language_txt[80]); // (empty string)
            strcpy(text_to_display[8].text, language_txt[81]); // /now you can use your hair/
            strcpy(text_to_display[9].text, language_txt[82]); // /as a helicopter/
            strcpy(text_to_display[10].text, language_txt[83]); // /to do it press $ twice/
            Deter_Option_Caract(text_to_display[8].text, options_jeu.jump, 255);
        }
        break;
    case 3:
        if (num_level == 11) {
            is_fee = 1;
            strcpy(text_to_display[1].text, language_txt[84]); // /things are going to get/
            strcpy(text_to_display[2].text, language_txt[85]); // /really wild from now on rayman/
            strcpy(text_to_display[3].text, language_txt[86]); // /you'll have to be very quick./
            strcpy(text_to_display[4].text, language_txt[87]); // (empty string)
            strcpy(text_to_display[5].text, language_txt[88]); // (empty string)
            strcpy(text_to_display[6].text, language_txt[89]); // /now you can run faster/
            strcpy(text_to_display[7].text, language_txt[90]); // (empty string)
            strcpy(text_to_display[8].text, language_txt[91]); // /to run press $ while walking/
            strcpy(text_to_display[9].text, language_txt[92]); // (empty string)
            strcpy(text_to_display[10].text, language_txt[93]); // (empty string)
            Deter_Option_Caract(text_to_display[8].text, options_jeu.action, 255);
        }
        break;
    }
    if (is_fee) {
        INIT_TXT_FEE();
        allocate_poing_or_free();
    }
}

//39BD0
void wait_for_dialogue_fee(obj_t* obj, s16 time) {
    /* 4DFD0 801727D0 -O2 -msoft-float */
    if (obj->timer < time) {
        obj->timer++;
    } else {
        obj->timer = 0;
        display_txt_fee++;
    }
}

//39C04
void DO_FEE_ETAPE(obj_t* obj) {
    /* 4E020 80172820 -O2 -msoft-float */
    u8 state = obj->config;
    u8 cur_txt;

    switch (state) {
        case 1:
            wait_for_dialogue_fee(obj, 100);
            cur_txt = display_txt_fee;
            if (cur_txt == 5 || text_to_display[cur_txt + 1].text[0] == '\0')
            {
                skipToLabel(obj, 1, false);
                obj->config = 0;
                display_txt_fee = 255;
                obj->timer = 0;
            }
            break;
        case 2:
            wait_for_dialogue_fee(obj, 100);
            if (display_txt_fee == 7) {
                display_txt_fee = 255;
                obj->config = 3;
                obj->timer = 0;
            }
            break;
        case 3:
            obj->timer++;
            if (obj->timer >= 51) {
                obj->config = 4;
                display_txt_fee = 7;
                obj->timer = 0;
            }
            break;
        case 4:
            if (horloge[2] != 0)
                obj->speed_y = -1;
            else
                obj->speed_y = 0;

            cur_txt = display_txt_fee;
            if (cur_txt < 9 && text_to_display[cur_txt + 1].text[0] != '\0')
                wait_for_dialogue_fee(obj, 120);
            break;
        case 0:
        default:
            display_txt_fee = 255;
            break;
    }
}

//39D20
void fee_gives_super_evts(void) {
    /* 4E1B4 801729B4 -O2 -msoft-float */
    switch (num_world) {
        case 1:
            if (num_level == 3)
                RayEvts.poing = true;
            if (num_level == 8)
                RayEvts.hang = true;
            if (num_level == 17)
                RayEvts.grap = true;
            break;
        case 2:
            if (num_level == 11)
                RayEvts.helico = true;
            break;
        case 3:
            if (num_level == 11)
                RayEvts.run = true;
            break;
    }
}

//39DB0
void DO_FEE(obj_t* obj) {
    /* 4E2D0 80172AD0 -O2 -msoft-float */
    obj_t* png_or_fee;
    u8 old_txt;
    u8 ray_main_etat;
    s16 y;
    s16 y_fee;

    png_or_fee = &level.objects[png_or_fee_id];
    old_txt = display_txt_fee;
    switch (obj->sub_etat)
    {
        case 0:
            if (obj->detect_zone_flag != 0) {
                if (Abs(ray.x + ray.offset_bx - obj->x + 2) < 3) {
                    ray_main_etat = ray.main_etat;
                    if (ray_main_etat < 2 || (ray.main_etat == 3 && (ray.sub_etat == 20 || ray.sub_etat == 21))) {
                        set_sub_etat(obj, 1);
                        skipToLabel(obj, 0, true);
                        set_main_and_sub_etat(&ray, 3, 34);
                        remoteRayXToReach = -32000;
                        ray.flags.flip_x = 1;
                    }
                }
            } else {
                ray_in_fee_zone = false;
            }
            obj->config = 0;
            break;
        case 1:
            if (display_txt_fee == 255) {
                obj->config = 1;
                display_txt_fee = 0;
                obj->timer = 0;
            }
            break;
        case 2:
        case 3:
            ray_main_etat = ray.main_etat;
            if (ray_main_etat < 2 || (ray.main_etat == 3 && ray.sub_etat == 34))
                set_main_and_sub_etat(&ray, 3, 35);
            if (ray.main_etat == 3 && ray.sub_etat == 36)
                ray.anim_frame = -1;
            break;
        case 4:
            if (EOA(obj)) {
                fee_gives_super_evts();
                obj->config = 2;
                display_txt_fee = 5;
                obj->timer = 0;
            }
            else
                obj->config = 0;
            break;
        case 6:
            if (ray.main_etat == 3)
            {
                if (ray.sub_etat == 36)
                    set_sub_etat(&ray, 37);
                if (ray.main_etat == 3 && ray.sub_etat == 20)
                {
                    remoteRayXToReach = ray.x;
                    if (obj->config == 4)
                    {
                        set_main_and_sub_etat(&ray, 0, 0);
                        remoteRayXToReach = -32000;
                    }
                }
            }
            break;
    }
    DO_FEE_ETAPE(obj);
    if (display_txt_fee != old_txt)
    {
        old_txt_fee = old_txt; // added in PC version
        new_txt_fee = 1;
        if (display_txt_fee != 255)
        {
            if (!png_or_fee->is_active) {
                add_alwobj(png_or_fee); // added in PC version
            }
            png_or_fee->active_timer = 0;
            png_or_fee->flags.alive = 1;
            png_or_fee->flags.flip_x = 1;
            png_or_fee->x = xmap - png_or_fee->offset_bx;
            y_fee = (SCREEN_HEIGHT < 240) ? SCREEN_HEIGHT - 10 : SCREEN_HEIGHT - 20; // added to account for PC/PS1 difference
            y = ymap - (png_or_fee->offset_by - y_fee);
            png_or_fee->y = y;
            png_or_fee->screen_x = -png_or_fee->offset_bx;
            png_or_fee->screen_y = y_fee - png_or_fee->offset_by;
            old_txt_fee = old_txt;
            png_or_fee->init_x = png_or_fee->x;
            png_or_fee->init_y = png_or_fee->y;
        }
    }
    if (new_txt_fee != 0) {
        // NOTE: fixed out-of-bounds access: old_txt_fee may be 255!
        if (old_txt_fee != 255 && text_to_display[old_txt_fee].ypos < SCREEN_HEIGHT + 20) {
            text_to_display[old_txt_fee].ypos++;
        } else {
            new_txt_fee = 0;
        }
    }
    if (png_or_fee->flags.alive) {
        //NOTE: below is added in PC version
        if (png_or_fee->active_flag == 2 && !png_or_fee->is_active) {
            png_or_fee->active_timer = 0;
            png_or_fee->active_flag = 0;
            png_or_fee->flags.alive = 0;
        }
        png_or_fee->x += 4;
        png_or_fee->screen_x += 4;
        if (png_or_fee->screen_x > 310) {
            png_or_fee->anim_frame = 0;
            png_or_fee->flags.alive = 0;
            png_or_fee->is_active = 0;
        }
    }
}

//3A118
void DoFeeRaymanZDD(obj_t* obj) {
    u8 sub_etat;
    u8 ray_main_etat;

    sub_etat = obj->sub_etat;
    if (sub_etat != 0) {
        ray_in_fee_zone = 0;
    } else {
        ray_main_etat = ray.main_etat;
        if (ray_main_etat == 2) {
            ray_in_fee_zone = 1;
            ray.speed_x = 0;
            ray.link = 0;
            decalage_en_cours = 0;
        } else if (ray_main_etat == 1 || ray_main_etat == 0 || (ray_main_etat == 3 && ray.sub_etat == 20)) {
            remoteRayXToReach = (obj->x - ray.offset_bx - 2) & ~1;
            if (ray.x & 1) {
                remoteRayXToReach |= 1;
            } else {
                remoteRayXToReach &= ~1;
            }
            ray_in_fee_zone = 0;
        } else {
            ray_in_fee_zone = 0;
        }
    }
}
