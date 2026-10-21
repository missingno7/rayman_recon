/* ray_pc: worker tu_pcmisc (1:61920 ray.c PC part / 1:6f3b0 sysutils.c, misc_source.c). Names RAYVERSE/ANDROID. */
#include "obj.h"
#include "game.h"
#include <string.h>

extern mem_t main_mem_tmp;
extern u8 VGA_FREQ;
extern u8 current_pal_id;
extern u8 *DrawBufferNormal;
extern u8 *display_buffer;
extern u8 *PLAN2BIT;
extern u8 *PLAN3BIT;
extern s32 plan2bit_length;
extern s32 plan3bit_length;
extern rgb_palette_t rvb_pres;
extern rgb_palette_t current_rvb;
extern rgb_palette_t rvb_plan3;

u32 init_memory(mem_t *mem, u32 capacity);
void free_main_mem(mem_t *mem);
s16 InitModeNormalWithFrequency(u8 frequency);
void SWAP_BUFFERS(void);
void start_fade_in(s16 speed, rgb_palette_t *palette);
void synchro(void);
void endsynchro(void);
void do_fade(rgb_palette_t *source, rgb_palette_t *dest);
void DisplayBufferModeNormal(u8 *source, u8 *dest);
#pragma aux DisplayBufferModeNormal parm [esi] [edi]
void readinput(void);
u8 TOUCHE(u8 key);
s16 but0pressed(void);
s16 but1pressed(void);
void fade_out(s16 speed, rgb_palette_t *palette);
void LoadPlan2InVignet(mem_t *mem, s16 plan);
void INIT_FADE_IN(void);
void SetCompteurTrameAudio(void);
void start_cd_ubi_soft(void);
void stop_cd(void);

void INIT_PC(void)
{
    init_memory(&main_mem_tmp, 0x2EE00);
}

void FIN_PC(void)
{
    free_main_mem(&main_mem_tmp);
}

void updateLogo(s16 fade_duration, s16 first_count, s16 second_count)
{
    s16 i;

    InitModeNormalWithFrequency(VGA_FREQ);
    SWAP_BUFFERS();
    start_fade_in(2, &rvb_pres);
    synchro();
    memcpy(DrawBufferNormal, PLAN2BIT, 64000);
    DisplayBufferModeNormal(DrawBufferNormal, display_buffer);

    i = 0;
    while (first_count != -1) {
        endsynchro();
        synchro();
        do_fade(&rvb_pres, &current_rvb);
        ++i;
        if (i == fade_duration) {
            first_count--;
            i = 0;
        }
    }

    readinput();
    i = 0;
    for (;;) {
        if (TOUCHE(0x39) || but0pressed() || but1pressed())
            break;
        if (second_count == -1)
            break;
        endsynchro();
        synchro();
        do_fade(&rvb_pres, &current_rvb);
        readinput();
        ++i;
        if (i == fade_duration) {
            second_count--;
            i = 0;
        }
    }
    fade_out(2, &rvb_pres);
}

void LOAD_SCREEN(void)
{
    LoadPlan2InVignet(&main_mem_tmp, 29);
}

void sub_71A84(void)
{
    LoadPlan2InVignet(&main_mem_tmp, 31);
}

void sub_71A98(void)
{
    u8 temp[0x300];
    u8 *temp_ptr;
    s32 temp_size;

    temp_ptr = PLAN2BIT;
    PLAN2BIT = PLAN3BIT;
    PLAN3BIT = temp_ptr;
    temp_size = plan2bit_length;
    plan2bit_length = plan3bit_length;
    plan3bit_length = temp_size;

    memcpy(temp, &rvb_pres, 0x300);
    memcpy(&rvb_pres, &rvb_plan3, 0x300);
    memcpy(&rvb_plan3, temp, 0x300);
}

void DO_UBI_LOGO(void)
{
    INIT_PC();
    SetCompteurTrameAudio();
    current_pal_id = 0;
    LOAD_SCREEN();
    INIT_FADE_IN();
    start_cd_ubi_soft();
    updateLogo(60, -1, 8);
    FIN_PC();
    stop_cd();
}
