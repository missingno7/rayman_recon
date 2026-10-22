/*
 * Bonus levels and timing (GOG v1.21 object 1:13a00-1:13f30). File name invented (Rayverse bonus.c).
 * Names: Rayverse/Android.
 * All eight functions STRICT (calc_left_time, do_perfect_bonus solved by build/workers/near_leftovers).
 * Note: the DOS ending test is `loop_time == 500` (Rayverse writes `> 500`).
 */
#include "obj.h"
#include "game.h"

/* Memory block descriptor: 12-byte DOS layout evidenced by this unit (init_memory/free_main_mem callers);
   Rayverse's mem_t is a port-specific variant. Local until a second unit confirms it. */
extern s16 allowed_time[192];
extern u8 bonus_perfect[24];
extern s16 num_world;
extern s16 num_level;
extern s16 bonus_map;
extern s32 map_time;
extern s32 left_time;
extern s32 nb_wiz_save;
extern u8 nb_wiz;
extern u8 nb_wiz_collected;
extern s16 departlevel;
extern u8 horloge[25];
extern s16 loop_time;
extern s16 id_Cling_1up;
extern u16 nb_fade;
extern u8 CarteSonAutorisee;
extern u8 ModeVideoActuel;
extern u8 VGA_FREQ;
extern mem_t main_mem_tmp;
extern status_bar_t status_bar;
extern obj_t ray;
extern obj_t *dword_D0028;
extern char bonus_time_display_text[20];
extern char *language_txt[];
extern rgb_palette_t rvb_plan3;

#define MODE_X 1

extern void fix_numlevel(obj_t *obj);
extern void DO_FADE_OUT(void);
extern u8 getbit(u8 *bits, u16 bit);
extern void setbit(u8 *bits, u16 bit);
extern s16 NOVA_STATUS_BAR(void);
extern void Add_One_RAY_lives(void);
extern void horloges(u8 ticks);
extern void readinput(void);
extern void INIT_FADE_IN(void);
extern void cyclage_palette(s16 first, s16 low, s16 high);
extern void DISPLAY_FOND3(void);
extern void display_text(const char *text, s16 x, s16 y, u8 font_size, s8 color);
extern obj_t *allocateNOVA(void);
extern void DO_ANIM(obj_t *obj);
extern void switchOff(obj_t *obj);
extern void display2(obj_t *obj);
extern s16 but0pressed(void);
extern s16 but1pressed(void);
extern s16 but2pressed(void);
extern s16 but3pressed(void);
extern s16 InitModeNormalWithFrequency(u8 frequency);
extern s16 InitModeXWithFrequency(u8 frequency);
extern void init_memory(mem_t *mem, s32 size);
extern void LOAD_PERFTIME_SCREEN(mem_t *mem);
extern void LOAD_PERFECT_SCREEN(mem_t *mem);
extern void SAVE_PALETTE(rgb_palette_t *palette);
extern void EFFACE_VIDEO(void);
extern void start_cd_gros_rayman(void);
extern void stop_all_snd(void);
extern void PlayTchatchVignette(s32 sound);
extern void SYNCHRO_LOOP(s16 (*scene)(u32));
extern void FreeTchatchVignette(void);
extern void RESTORE_PALETTE(void);
extern void free_main_mem(mem_t *ptr);
extern int sprintf(char *dst, const char *fmt, ...);

s16 get_allowed_time(void)
{
    s32 level_index;
    s32 world_index;

    level_index = num_level;
    world_index = num_world;
    --level_index;
    world_index = (world_index << 5) - 32;
    return allowed_time[world_index + level_index];
}

void calc_left_time(void)
{
    if (bonus_map) {
        s32 world_index;
        s32 level_index;
        s32 index;
        world_index = num_world;
        level_index = num_level;
        world_index = (world_index << 5) - 32;
        --level_index;
        index = level_index + world_index;
        if (map_time == 1) {
            left_time = allowed_time[index];
            if (left_time != -2)
                left_time *= 60;
        }
        if (allowed_time[index] != -2 && left_time != 0 && map_time > 120) {
            ray.is_active = 1;
            left_time = 60 * allowed_time[index] - (map_time - 120);
            if (left_time == 0 && nb_wiz != 0) {
                status_bar.num_wiz = (u8)nb_wiz_save;
                departlevel = 0;
                fix_numlevel(&ray);
                DO_FADE_OUT();
            }
        }
    }
}

u8 get_bonus_map_complete(s32 world, s32 lvl)
{
    return getbit(bonus_perfect, (u16)(lvl - 1 + ((world - 1) << 5)));
}

void set_bonus_map_complete(s32 world, s32 lvl)
{
    setbit(bonus_perfect, (u16)(lvl - 1 + ((world - 1) << 5)));
}

void DO_WIZ_AFTER_BONUS_MAP(void)
{
    if (nb_wiz_collected > 0 && !bonus_map && horloge[4] == 0) {
        nb_wiz_collected--;
        status_bar.num_wiz++;
        if (status_bar.num_wiz > 99) {
            status_bar.num_wiz -= 100;
            id_Cling_1up = NOVA_STATUS_BAR();
            if (id_Cling_1up == -1)
                Add_One_RAY_lives();
        }
    }
}

void init_bonus_perfect(void)
{
    s32 i;

    i = 0;
    do {
        ++i;
        bonus_perfect[i - 1] = 0;
    } while (i < 24);
}

s16 do_perfect_bonus(u32 a1)
{
    obj_t *nova;
    s16 done;

    horloges(1);
    readinput();

    if (loop_time == 0) {
        s16 index;
        INIT_FADE_IN();
        index = (s16)(((num_world - 1) * 32) + (num_level - 1));
        sprintf(bonus_time_display_text, "/%2d : %02d/", allowed_time[index] - 1 - left_time / 60, (map_time - 120) % 60);
    }

    if (get_bonus_map_complete(num_world, num_level)) {
        if (nb_fade == 0 && horloge[4] == 0)
            cyclage_palette(0, 128, 191);
        if (nb_fade == 0 && horloge[6] == 0)
            cyclage_palette(0, 192, 239);
        DISPLAY_FOND3();
        display_text(language_txt[157], 160, 60, 0, 5);
        display_text(language_txt[158], 160, 110, 2, 1);
        display_text(bonus_time_display_text, 160, 150, 0, 5);
    } else {
        if (nb_fade == 0 && horloge[4] == 0)
            cyclage_palette(0, 128, 191);
        if (nb_fade == 0 && horloge[6] == 0)
            cyclage_palette(0, 192, 239);
        DISPLAY_FOND3();
        if (loop_time == 15) {
            nova = allocateNOVA();
            nova->screen_x = 75;
            nova->screen_y = 50;
            dword_D0028 = nova;
            nova->is_active = 1;
            DO_ANIM(nova);
            Add_One_RAY_lives();
        }
        if (loop_time > 20) {
            DO_ANIM(dword_D0028);
            switchOff(dword_D0028);
            if (dword_D0028->is_active)
                display2(dword_D0028);
        }
    }

    done = 0;
    if (loop_time == 500 || ((but0pressed() || but1pressed() || but2pressed() || but3pressed()) && loop_time > 50)) {
        done = 1;
        fix_numlevel(&ray);
    }

    ++loop_time;
    return done;
}

void DO_PERFECT_BONUS_MAP(void)
{
    s32 is_mode_x = 0;
    if (loop_time < 0) {
        ++loop_time;
        if (loop_time == -1) {
            departlevel = 0;
            ++loop_time;
            if (ModeVideoActuel == MODE_X) {
                is_mode_x = 1;
                InitModeNormalWithFrequency(VGA_FREQ);
            }
            init_memory(&main_mem_tmp, 64000);
            if (get_bonus_map_complete(num_world, num_level))
                LOAD_PERFTIME_SCREEN(&main_mem_tmp);
            else
                LOAD_PERFECT_SCREEN(&main_mem_tmp);
            SAVE_PALETTE(&rvb_plan3);
            EFFACE_VIDEO();
            start_cd_gros_rayman();
            if (CarteSonAutorisee) {
                stop_all_snd();
                PlayTchatchVignette(6);
            }
            SYNCHRO_LOOP(do_perfect_bonus);
            if (CarteSonAutorisee)
                FreeTchatchVignette();
            DO_FADE_OUT();
            status_bar.num_wiz = (u8)nb_wiz_save;
            nb_wiz_save = 0;
            set_bonus_map_complete(num_world, num_level);
            if (is_mode_x)
                InitModeXWithFrequency(VGA_FREQ);
            EFFACE_VIDEO();
            RESTORE_PALETTE();
            free_main_mem(&main_mem_tmp);
        }
    }
}
