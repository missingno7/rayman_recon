#include "type_island_abi.h"
/* save object (DOS v1.21, 1:63b80..1:65837; following zeros are linker padding).
 * Names RAYVERSE/ANDROID; tu_save/gfin_save/h_save reconstruction, d_save layout pass.
 * See NOTES.md for verified results and remaining differences.
 */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
extern save_state_t save1;
extern u8 save_zone[2592];
extern s16 num_world;
extern s16 num_level;
extern u8 nb_levels_in_world[7];
extern status_bar_t status_bar;
extern s16 xmap, ymap;
extern obj_t ray;
extern rayevts_t RayEvts;
extern s16 *link_init;
extern level_t level;
extern s16 nb_floc[8];
extern s16 VENT_X, VENT_Y;
extern poing_t poing;
extern u8 dead_time, current_pal_id;
extern world_info_t t_world_info[24];
extern world_info_save_t wi_save_zone[24];
extern obj_t rms;
extern obj_t *poing_obj;
extern s16 decalage_en_cours, ray_wind_force;
extern u8 gerbe;
extern s16 h_scroll_speed, v_scroll_speed;
/* DOS CheminSauvegarde, Rayverse data.c //CCFDC; config's old label is reused
   at the same start, with its true 100-byte character-array type. */
extern char demo_video_mem[100];
extern char save_ray[3][4];
extern u8 nb_continue;
extern u8 bonus_perfect[24];
extern u8 save_ray_copy[4];
extern loadinforay_t LoadInfoRay[3];
extern u8 language, NumCard, Mode_Pad, Port_Pad, P486, ScrollDiffOn;
extern u8 is_joypad_mode, something_with_cfg_25_default_00_joy;
extern u8 dans_la_map_monde, JeuCracker;
extern s16 num_world_choice;
extern finbosslevel_t finBosslevel;
extern s32 Port, Irq, Dma, Param, DeviceID;
extern u8 ALL_WORLD;
extern u8 world_index;
extern s16 xwldmapsave, ywldmapsave;
extern options_jeu_t options_jeu;
extern s32 *GetPort(void);
extern s32 *GetIrq(void);
extern s32 *GetDma(void);
extern s32 *GetParam(void);
extern s32 *GetDeviceID_Ray(void);
void sub_21b48(FILE *source, FILE *destination);
void sub_21d28(FILE *source, FILE *destination);
u8 sub_21840(void);
extern u8 *tab_key[7];
extern s16 notbut[4];
extern s16 xpadmax, xpadmin, ypadmax, ypadmin, xpadcentre, ypadcentre;
extern u8 GameModeVideo, Frequence, BackgroundOptionOn;
extern u16 fixon;
extern s16 SizeScreen;
extern u16 tRefRam2VramNormalFix[2][4];
extern u16 tRefRam2VramNormal[2][4];
extern u16 tRefTransFondNormal[2][4];
extern u16 tRefSpriteNormal[2];
extern u16 tRefRam2VramX[2];
extern u16 tRefVram2VramX[2];
extern s16 tRefSpriteX[2];
u8 SaveGameOnDisk(u8 which_save);
u8 LoadGameOnDisk(u8 which_save);
u8 LoadInfoGame(u8 which_save);
u8 SaveOptionsOnDisk(void);
u8 LoadOptionsOnDisk(void);
void actualize_palette(u8 pal_id);
s16 get_offset_in_safe_zone(s16 event_index);
void initGameSave(void);
void doneGameSave(void);
void reset_save_zone_level(void);
void take_bonus(s16 event_index);
u8 bonus_taken(s16 event_index);

void initGameSave(void) {
    save1.is_just_saved = 0;
}

void doneGameSave(void) {
	save1.is_just_saved = 0;
}

void saveGameState(obj_t *save_obj, save_state_t *save_state)
{
    s16 j;
    obj_t *cur_obj;
    u8 i;
    s16 nb_objs;
    s32 unk_2;
    s16 unk_1;

    save_state->status_bar_tings = status_bar.num_wiz;
    save_state->x_map = xmap;
    save_state->y_map = ymap;
    save_state->ray_x_pos = (s16)ray.x;
    save_state->ray_y_pos = (s16)ray.y;
    save_state->ray_screen_x = ray.screen_x;
    save_state->ray_screen_y = ray.screen_y;
    save_state->ray_flip_x = ray.flags.flip_x;
    save_state->rayevts_reverse = RayEvts.reverse;
    save_state->rayevts_super_helico = RayEvts.super_helico;
    save_state->rayevts_poing = RayEvts.poing;

    if (save_obj) {
        save_state->save_obj_id = save_obj->id;
        save_state->save_obj_x_pos = (s16)save_obj->x;
        save_state->save_obj_y_pos = (s16)save_obj->y;
        save_state->save_obj_detect_zone_flag = save_obj->detect_zone_flag;
        save_state->save_obj_flag_1 = save_obj->flags.hurtbyfist;
    } else {
        save_state->save_obj_id = -1;
    }

    memcpy(save_state->link_init, link_init, level.nb_objects * sizeof(u16));
    for (i = 0; i < 5U; i++)
        save_state->ray_coll_btype[i] = ray.btypes[i];

    save_state->ray_anim_index = ray.anim_index;
    save_state->ray_anim_frame = ray.anim_frame;
    save_state->ray_main_etat = ray.main_etat;
    save_state->ray_sub_etat = ray.sub_etat;
    for (i = 0; i < 8; i++)
        save_state->nb_floc[i] = nb_floc[i];

    save_state->vent_x = VENT_X;
    save_state->vent_y = VENT_Y;
    save_state->poing_sub_etat = poing.sub_etat;

    j = 0;
    cur_obj = &level.objects[0];
    /* COMPILER_STEERED: the count copy and loop rotation act together. */
    {
        s16 object_count;
        object_count = level.nb_objects;
        nb_objs = object_count;
    }
    if (nb_objs > 0) {
        do {
            if (cur_obj->type == 141 || cur_obj->type == 164 ||
                cur_obj->type == 179 || cur_obj->type == 242) {
                unk_1 = j >> 5;
                unk_2 = 1U << (j & 0x1f);
                if (cur_obj->flags.alive)
                    save_state->triggered_objects[unk_1] |= unk_2;
                else
                    save_state->triggered_objects[unk_1] &= ~unk_2;
            }
            j++;
            cur_obj++;
        } while (j < nb_objs);
    }

    save_state->is_just_saved = 1;
    save_state->dead_time = dead_time;
    save_state->current_pal_id = current_pal_id;
}

void restoreGameState(save_state_t *save_state)
{
    s16 obj_id;
    obj_t *obj;
    s16 i;
    s32 bit;
    s16 index;
    s16 nb_objs;
    obj_t *save_obj;
    s16 saved_id;

    if (save_state->is_just_saved) {
        if (RayEvts.tiny) {
            ray = rms;
            RayEvts.tiny = 0;
        }

        status_bar.num_wiz = save_state->status_bar_tings;
        xmap = save_state->x_map;
        ymap = save_state->y_map;
        ray.x = save_state->ray_x_pos;
        ray.y = save_state->ray_y_pos;
        ray.screen_x = save_state->ray_screen_x;
        ray.screen_y = save_state->ray_screen_y;
        ray.flags.flip_x = save_state->ray_flip_x;
        RayEvts.super_helico = save_state->rayevts_super_helico;

        if (num_world == 6 && num_level == 3) {
            RayEvts.poing = save_state->rayevts_poing;
            RayEvts.reverse = save_state->rayevts_reverse;
        }

        saved_id = save_state->save_obj_id;
        if (saved_id != -1) {
            save_obj = level.objects + saved_id;
            save_obj->x = save_state->save_obj_x_pos;
            save_obj->y = save_state->save_obj_y_pos;
            save_obj->detect_zone_flag = save_state->save_obj_detect_zone_flag;
            save_obj->flags.hurtbyfist = save_state->save_obj_flag_1;
        }

        if (save_state == (save_state_t *)&save1) {
            for (i = 0; i < level.nb_objects; i++) {
                link_init[i] = save_state->link_init[i];
                if (link_init[i] != i)
                    level.objects[i].link_has_gendoor = 1;
                else
                    level.objects[i].link_has_gendoor = 0;
            }
        }

        for (i = 0; i < 5U; i++)
            ray.btypes[i] = save_state->ray_coll_btype[i];

        ray.anim_index = save_state->ray_anim_index;
        ray.anim_frame = save_state->ray_anim_frame;
        ray.main_etat = save_state->ray_main_etat;
        ray.sub_etat = save_state->ray_sub_etat;

        if (save_state == (save_state_t *)&save1) {
            switch (save_state->poing_sub_etat) {
            case 5:
            case 12:
                save_state->poing_sub_etat = 3;
                break;
            case 1:
            case 3:
            case 8:
            case 10:
                save_state->poing_sub_etat = 1;
                break;
            }
        }

        poing_obj->init_sub_etat = save_state->poing_sub_etat;
        poing.sub_etat = save_state->poing_sub_etat;
        dead_time = save_state->dead_time;
        i = 0;  /* DOS 1:64046: initialize before the two reset stores. */
        decalage_en_cours = 0;
        ray_wind_force = 0;
        for (; i < 8; i++)
            nb_floc[i] = save_state->nb_floc[i];
        VENT_X = save_state->vent_x;
        ray.speed_x = 0;
        ray.speed_y = 0;
        ray.iframes_timer = -1;
        ray.is_active = 1;
        VENT_Y = save_state->vent_y;
        ray.flags.alive = 1;

        h_scroll_speed = 0;
        v_scroll_speed = 0;
        obj_id = 0;
        obj = level.objects;
        /* SYNTHETIC: local copies steer allocation; neither address escapes. */
        {
            s16 object_count;
            s16 *count_slot;
            count_slot = &object_count;
            *count_slot = level.nb_objects;
            nb_objs = object_count;
        }
        while (obj_id < nb_objs) {
            if (obj->type == 3 || obj->type == 13 || obj->type == 15 || obj->type == 59 ||
                obj->type == 36 || obj->type == 66 || obj->type == 62 || obj->type == 99) {
                obj->flags.alive = 0;
            } else if (obj->type == 141 || obj->type == 164 ||
                       ((obj->type == 179 || obj->type == 242) ? (obj->field_1C = 1) : 0)) {

                index = obj_id >> 5;
                {
                    s32 object_bit;
                    s32 *bit_slot;
                    bit_slot = &object_bit;
                    *bit_slot = 1U << (obj_id & 0x1f);
                    bit = object_bit;
                }
                if ((bit & save_state->triggered_objects[index]) != 0)
                    obj->flags.alive = 1;
                else
                    obj->flags.alive = 0;
            }
            obj_id++;
            obj++;
        }

        gerbe = 0;
        current_pal_id = save_state->current_pal_id + 1;
        actualize_palette(save_state->current_pal_id);
    }
}

s16 get_offset_in_safe_zone(s16 event_index)
{
    s16 offset = 0;  /* DOS 1:641f6 zeroes BH, retaining a narrow accumulator. */
    s16 i;

    for (i = 1; i < num_world; i++)
        offset += nb_levels_in_world[i] * 32;
    for (i = 1; i < num_level; i++)
        offset += 32;
    offset += ashr16(event_index, 3);
    return offset;
}

void reset_save_zone_level(void)
{
    memset(&save_zone[get_offset_in_safe_zone(0)], 0, 32);
}

void take_bonus(s16 event_index)
{
    save_zone[get_offset_in_safe_zone(event_index)] |=
        ashr16(0x80, (u8)event_index & 7);
}

u8 bonus_taken(s16 event_index)
{
    return save_zone[get_offset_in_safe_zone(event_index)] &
        ashr16(0x80, (u8)event_index & 7);
}

void storeWorldInfoAcces(void)
{
    s16 i;
    for (i = 0; i < 24; i++) {
        wi_save_zone[i].is_unlocked = t_world_info[i].is_unlocked;
        wi_save_zone[i].is_unlocking = t_world_info[i].is_unlocking;
        wi_save_zone[i].nb_cages = t_world_info[i].nb_cages;
    }
}

void retrieveWorldInfoAccess(void)
{
    s16 i;
    for (i = 0; i < 24; i++) {
        t_world_info[i].is_unlocked = wi_save_zone[i].is_unlocked;
        t_world_info[i].is_unlocking = wi_save_zone[i].is_unlocking;
        t_world_info[i].nb_cages = wi_save_zone[i].nb_cages;
    }
}

s32 file_size(char *filename)
{
    FILE *fp;
    s32 size;

    fp = fopen(filename, "r");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fclose(fp);
        return size;
    }
    return 0;
}

u8 SaveGameOnDisk(u8 which_save)
{
    char temporary[116];
    char filename[120];
    char temp_name[L_tmpnam];
    s16 fd;
    FILE *source;
    FILE *destination;
    u8 error;

    strcpy(filename, demo_video_mem);
    strcat(filename, "RAYMAN");
    itoa(which_save, filename + strlen(filename), 10);
    strcat(filename, ".SAV");

    tmpnam(temp_name);
    strcpy(temporary, demo_video_mem);
    strcat(temporary, temp_name);

    fd = (s16)open(temporary, O_RDWR | O_CREAT | O_BINARY,
                   S_IREAD | S_IWRITE);
    if (fd != -1) {
        write(fd, save_ray[which_save - 1], 4);
        write(fd, &nb_continue, 1);
        storeWorldInfoAcces();
        write(fd, wi_save_zone, 24);
        write(fd, &RayEvts, 2);
        write(fd, &poing, 20);
        write(fd, &status_bar, 10);
        write(fd, &ray.hit_points, 1);
        write(fd, save_zone, 2592);
        write(fd, bonus_perfect, 24);
        if (dans_la_map_monde)
            write(fd, &num_world_choice, 2);
        else
            write(fd, &world_index, 2);
        write(fd, &finBosslevel, 2);
        close(fd);
        if (which_save > 0) {
            source = fopen(temporary, "rb");
            if (source != NULL) {
                destination = fopen(filename, "wb");
                if (destination != NULL) {
                    sub_21b48(source, destination);
                    error = sub_21840();
                    fclose(destination);
                } else {
                    error = 1;
                }
                fclose(source);
            } else {
                error = 1;
            }
        } else {
            error = 1;
        }
    } else {
        error = 1;
    }

    destination = fopen(temporary, "wb");
    if (destination != NULL) {
        fwrite(&ray, 0xa7a, 1, destination);
        fclose(destination);
    }
    unlink(temporary);
    return error;
}

u8 LoadGameOnDisk(u8 which_save)
{
    char temporary[116];
    char filename[120];
    char temp_name[L_tmpnam];
    s16 fd;
    u8 error;
    FILE *source;
    FILE *destination;

    strcpy(filename, demo_video_mem);
    strcat(filename, "RAYMAN");
    itoa(which_save, filename + strlen(filename), 10);
    strcat(filename, ".SAV");

    tmpnam(temp_name);
    strcpy(temporary, demo_video_mem);
    strcat(temporary, temp_name);
    source = fopen(filename, "rb");
    if (source != NULL) {
        destination = fopen(temporary, "wb");
        if (destination != NULL) {
            sub_21d28(source, destination);
            error = sub_21840();
            fclose(destination);
        } else {
            error = 1;
        }
        fclose(source);
    } else {
        error = 1;
    }

    if (error == 0) {
        if (file_size(temporary) == 0xa7a) {
        } else {
            error = 1;
        }
    }

    if (error == 0) {
        fd = (s16)open(temporary, O_RDONLY | O_BINARY,
                       S_IREAD | S_IWRITE);
        if (fd != -1) {
            read(fd, save_ray[which_save - 1], 4);
            read(fd, &nb_continue, 1);
            read(fd, wi_save_zone, 24);
            read(fd, &RayEvts, 2);
            read(fd, &poing, 20);
            read(fd, &status_bar, 10);
            read(fd, &ray.hit_points, 1);
            read(fd, save_zone, 2592);
            read(fd, bonus_perfect, 24);
            read(fd, &world_index, 2);
            read(fd, &finBosslevel, 2);
            close(fd);
            retrieveWorldInfoAccess();
            xwldmapsave = 0;
            ywldmapsave = 0;
        } else
            error = 1;
    } else
        error = 1;

    destination = fopen(temporary, "wb");
    if (destination != NULL) {
        fwrite(&ray, 0xa7a, 1, destination);
        fclose(destination);
    }
    unlink(temporary);

    return error;
}

u8 LoadInfoGame(u8 which_save)
{
    char temporary[116];
    char filename[120];
    char temp_name[L_tmpnam];
    s16 fd;
    FILE *destination;
    u8 error;
    s32 i;

    strcpy(filename, demo_video_mem);
    strcat(filename, "RAYMAN");
    itoa(which_save, filename + strlen(filename), 10);
    strcat(filename, ".SAV");

    tmpnam(temp_name);
    strcpy(temporary, demo_video_mem);
    strcat(temporary, temp_name);
    {
        FILE *source;
        source = fopen(filename, "rb");
        if (source != NULL) {
            destination = fopen(temporary, "wb");
            if (destination != NULL) {
                sub_21d28(source, destination);
                error = sub_21840();
                fclose(destination);
            } else {
                error = 1;
            }
            fclose(source);
        } else {
            error = 1;
        }
    }
    if (error == 0) {
        if (file_size(temporary) != 0xa7a)
            error = 1;
    }
    if (error == 0) {
        fd = (s16)open(temporary, O_RDONLY | O_BINARY,
                       S_IREAD | S_IWRITE);
        if (fd == -1) {
            error = 1;
        } else {
            read(fd, save_ray[which_save - 1], 4);
            read(fd, &nb_continue, 1);
            read(fd, wi_save_zone, 24);
            read(fd, &RayEvts, 2);
            read(fd, &poing, 20);
            read(fd, &status_bar, 10);
            close(fd);

            LoadInfoRay[which_save - 1].continues = nb_continue;
            LoadInfoRay[which_save - 1].cages = 0;
            for (i = 0; i < 24; i++)
                LoadInfoRay[which_save - 1].cages += wi_save_zone[i].nb_cages;
            LoadInfoRay[which_save - 1].lives = (u8)status_bar.lives;
            LoadInfoRay[which_save - 1].tings = status_bar.num_wiz;
        }
    } else {
        error = 1;
    }

    destination = fopen(temporary, "wb");
    if (destination != NULL) {
        fwrite(&ray, 0xa7a, 1, destination);
        fclose(destination);
    }
    unlink(temporary);

    if (JeuCracker)
        save_ray[which_save - 1][0] = '\0';
    return error;
}

u8 SaveOptionsOnDisk(void)
{
    char filename[120];
    s16 fd;
    u8 saved_frequency;
    u8 i;

    saved_frequency = Frequence;
    strcpy(filename, demo_video_mem);
    strcat(filename, "RAYMAN.CFG");
    fd = (s16)open(filename, O_RDWR | O_CREAT | O_BINARY,
                   S_IREAD | S_IWRITE);
    if (fd == -1)
        return 0;

    if (write(fd, &language, 1) == -1) return 0;
    if (write(fd, GetPort(), 4) <= 0) return 0;
    if (write(fd, GetIrq(), 4) <= 0) return 0;
    if (write(fd, GetDma(), 4) <= 0) return 0;
    if (write(fd, GetParam(), 4) <= 0) return 0;
    if (write(fd, GetDeviceID_Ray(), 4) <= 0) return 0;
    if (write(fd, &NumCard, 1) <= 0) return 0;
    if (write(fd, &options_jeu.jump, 2) <= 0) return 0;
    if (write(fd, &options_jeu.fist, 2) <= 0) return 0;
    if (write(fd, &options_jeu.field_14, 2) <= 0) return 0;
    if (write(fd, &options_jeu.action, 2) <= 0) return 0;
    if (write(fd, &options_jeu.music_enabled, 2) <= 0) return 0;
    if (write(fd, &options_jeu.sound_volume, 2) <= 0) return 0;
    if (write(fd, &options_jeu.is_stereo, 2) <= 0) return 0;
    if (write(fd, &is_joypad_mode, 1) <= 0) return 0;
    if (write(fd, &something_with_cfg_25_default_00_joy, 1) <= 0) return 0;
    if (write(fd, &xpadmax, 2) <= 0) return 0;
    if (write(fd, &xpadmin, 2) <= 0) return 0;
    if (write(fd, &ypadmax, 2) <= 0) return 0;
    if (write(fd, &ypadmin, 2) <= 0) return 0;
    if (write(fd, &xpadcentre, 2) <= 0) return 0;
    if (write(fd, &ypadcentre, 2) <= 0) return 0;
    for (i = 0; i < 4; i++)
        if (write(fd, &notbut[i], 1) <= 0) return 0;
    for (i = 0; i < 7; i++)
        if (write(fd, tab_key[i], 1) <= 0) return 0;
    if (write(fd, &GameModeVideo, 1) <= 0) return 0;
    if (write(fd, &P486, 1) <= 0) return 0;
    if (write(fd, &SizeScreen, 1) <= 0) return 0;
    if (saved_frequency != 1 && saved_frequency != 0 && saved_frequency != 2)
        saved_frequency = 0;
    if (write(fd, &saved_frequency, 1) <= 0) return 0;
    if (write(fd, &fixon, 1) <= 0) return 0;
    if (write(fd, &BackgroundOptionOn, 1) <= 0) return 0;
    if (write(fd, &ScrollDiffOn, 1) <= 0) return 0;
    if (write(fd, tRefRam2VramNormalFix, 16) <= 0) return 0;
    if (write(fd, tRefRam2VramNormal, 16) <= 0) return 0;
    if (write(fd, tRefTransFondNormal, 16) <= 0) return 0;
    if (write(fd, tRefSpriteNormal, 4) <= 0) return 0;
    if (write(fd, tRefRam2VramX, 4) <= 0) return 0;
    if (write(fd, tRefVram2VramX, 4) <= 0) return 0;
    if (write(fd, tRefSpriteX, 4) <= 0) return 0;
    close(fd);
    return 1;

}

u8 LoadOptionsOnDisk(void)
{
    char filename[120];
    s16 fd;
    s32 length;
    u8 i;

    strcpy(filename, demo_video_mem);
    strcat(filename, "RAYMAN.CFG");
    fd = (s16)open(filename, O_RDONLY | O_BINARY, S_IREAD | S_IWRITE);
    if (fd == -1)
        return 0;

    length = lseek(fd, 0, SEEK_END);
    if (length != 0x84)
        return 0;
    lseek(fd, 0, SEEK_SET);

    if (read(fd, &language, 1) == -1) return 0;
    if (read(fd, &Port, 4) == -1) return 0;
    if (read(fd, &Irq, 4) == -1) return 0;
    if (read(fd, &Dma, 4) == -1) return 0;
    if (read(fd, &Param, 4) == -1) return 0;
    if (read(fd, &DeviceID, 4) == -1) return 0;
    if (read(fd, &NumCard, 1) == -1) return 0;
    if (read(fd, &options_jeu.jump, 2) <= 0) return 0;
    if (read(fd, &options_jeu.fist, 2) <= 0) return 0;
    if (read(fd, &options_jeu.field_14, 2) <= 0) return 0;
    if (read(fd, &options_jeu.action, 2) <= 0) return 0;
    if (read(fd, &options_jeu.music_enabled, 2) <= 0) return 0;
    if (read(fd, &options_jeu.sound_volume, 2) <= 0) return 0;
    if (read(fd, &options_jeu.is_stereo, 2) <= 0) return 0;
    if (read(fd, &Mode_Pad, 1) <= 0) return 0;
    if (read(fd, &Port_Pad, 1) <= 0) return 0;
    if (read(fd, &xpadmax, 2) <= 0) return 0;
    if (read(fd, &xpadmin, 2) <= 0) return 0;
    if (read(fd, &ypadmax, 2) <= 0) return 0;
    if (read(fd, &ypadmin, 2) <= 0) return 0;
    if (read(fd, &xpadcentre, 2) <= 0) return 0;
    if (read(fd, &ypadcentre, 2) <= 0) return 0;
    for (i = 0; i < 4; i++) {
        if (read(fd, &notbut[i], 1) <= 0) return 0;
    }
    for (i = 0; i < 7; i++)
        if (read(fd, tab_key[i], 1) <= 0) return 0;
    if (read(fd, &GameModeVideo, 1) <= 0) return 0;
    if (read(fd, &P486, 1) <= 0) return 0;
    if (read(fd, &SizeScreen, 1) <= 0) return 0;
    if (Frequence != 70) {
        if (read(fd, &Frequence, 1) <= 0) return 0;
    } else if (read(fd, &fixon, 1) <= 0) {
        return 0;
    }
    if (read(fd, &fixon, 1) <= 0) return 0;
    if (read(fd, &BackgroundOptionOn, 1) <= 0) return 0;
    if (read(fd, &ScrollDiffOn, 1) <= 0) return 0;
    if (read(fd, tRefRam2VramNormalFix, 16) <= 0) return 0;
    if (read(fd, tRefRam2VramNormal, 16) <= 0) return 0;
    if (read(fd, tRefTransFondNormal, 16) <= 0) return 0;
    if (read(fd, tRefSpriteNormal, 4) <= 0) return 0;
    if (read(fd, tRefRam2VramX, 4) <= 0) return 0;
    if (read(fd, tRefVram2VramX, 4) <= 0) return 0;
    if (read(fd, tRefSpriteX, 4) <= 0) return 0;
    close(fd);
    return 1;

}
