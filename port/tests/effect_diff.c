/* Deterministic host-width differential driver. The effect code comes from rayman_hist;
 * the i686 reference executable supplies ONLY the seven original src objects ahead
 * of that library. No reference effect source is patched or preprocessed specially.
 * Link wraps control environment boundaries, never an effect calculation or pixel.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN64
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "rtypes.h"
#include "effect_types.h"
#include "platform/clock.h"
#include "video/lowlevel_normal.h"

extern u8 *DrawBufferNormal, *DisplayBufferNormal, *draw_buffer, *display_buffer, *EffetBufferNormal;
extern u8 GameModeVideo, CarteSonAutorisee, MusicCdActive, FondAutorise, VGA_FREQ, in_pause;
extern u8 effect_pause_bloque, effect_choice, byte_964C8;
extern s16 effect_speed, effect_pause_count;
extern u16 effect_pause_timer;
extern s32 effect_pause_counter, RandomIndex;
extern s16 RandArray[256];
extern char *language_txt[];
extern effect3d_t *dword_E4DA8;
extern effect_rows_t *dword_E0D14;
extern void *effect5_state, *effect_state_6e, *effect_state_6c, *effect_state_55760;
extern void port_srand(unsigned);
extern void MakeMyRand(int);
extern u8 sub_38208(u8,u8,u8*,u8*), sub_38220(u8,u8,u8*,u8*);
extern void sub_38258(void), sub_38290(void), DoEffectBloodOut(void);
extern void Do_Effect_Pause(void), Do_Effect_Pause_Simple(void);
extern void SaveScreen(u8*,u8*), RestoreScreen(u8*,u8*);
extern s16 Do_Effect_Pause_unknown(u32);
extern void effect_helper_6c3b0(s16,s16,s16,u16);
extern void effect_copy_band(u8*,u8*,u32), effect_copy_pause_band(u8*,u8*,u32);

#define GUARD 64
#define SCREEN 64000
#define TEXTURES 102400
static struct { u8 *raw, *p; size_t n; } allocations[32];
static unsigned allocated, freed, fixed_seed=0x12345678u;
static u8 *a, *b, *tex;
static void *surface_memory[3];
static unsigned surface_count;
static const char *case_name;
static unsigned frame, cheat_calls, loop_limit, frames_seen;
static int pause_mode, blood_mode, blood_pending, input_abort, pause_keys;
static unsigned seen_modes, seen_signs, seen_timer, seen_blocked, seen_return;
static unsigned seen_sphere_modes, seen_sphere_bounce, seen_sphere_drop;
static unsigned seen_wave_active, seen_wave_silent, seen_radial_reverse;
static int previous_vx, previous_vy;
static FILE *logfile;
static void fail(const char *s) { fprintf(stderr,"FAIL %s frame %u: %s\n",case_name?case_name:"setup",frame,s); exit(2); }
static void check_guards(void) {
    for (unsigned j=0;j<allocated;j++) if (allocations[j].raw) {
        u8 *p=allocations[j].raw; size_t n=allocations[j].n;
        for(unsigned i=0;i<GUARD;i++)
            if(p[i]!=0xd3 || p[GUARD+n+i]!=0xd3) fail("allocation redzone overwritten");
    }
    for(unsigned i=0;i<GUARD;i++) {
        if(a[-GUARD+(int)i]!=0xd3 || a[SCREEN+i]!=0xd3 || b[-GUARD+(int)i]!=0xd3 || b[SCREEN+i]!=0xd3 || tex[-GUARD+(int)i]!=0xd3 || tex[TEXTURES+i]!=0xd3) fail("surface redzone overwritten");
    }
}
void *__wrap_sub_6FBB4(u32 n) {
    if(allocated==32) fail("too many allocations");
    u8 *p=malloc(n+2*GUARD); if(!p) fail("malloc");
    memset(p,0xd3,n+2*GUARD); memset(p+GUARD,0,n);
    allocations[allocated++]=(typeof(allocations[0])){p,p+GUARD,n}; return p+GUARD;
}
void __wrap_JumelleFreeArray(void **q) {
    if(!*q) return;
    check_guards();
    for(unsigned i=0;i<allocated;i++) if(allocations[i].p==*q && allocations[i].raw) {
        free(allocations[i].raw); allocations[i].raw=0; *q=0; freed++; return;
    }
    fail("free of unknown allocation");
}
void __real_port_srand(unsigned);
void __wrap_port_srand(unsigned ignored) { (void)ignored; __real_port_srand(fixed_seed); }
static uint64_t hash(const void *p,size_t n) {
    const u8 *s=p; uint64_t h=14695981039346656037ULL;
    for(size_t i=0;i<n;i++) h=(h^s[i])*1099511628211ULL;
    return h;
}
/* Coverage views: only fixed-width prefix fields, or a native pointer-width
 * offset calculated from the declared sphere/wave layouts. No pixel expectations. */
static void coverage(void) {
    if(effect_choice==2 && dword_E0D14) seen_modes |= 1u<<dword_E0D14->mode;
    if(effect_choice==6 && dword_E4DA8) seen_modes |= 1u<<dword_E4DA8->mode;
    if(effect_choice==5 && effect5_state) {
        const u8 *s=effect5_state; unsigned mode=s[13];
        seen_sphere_modes|=1u<<mode;
        size_t k=16+6*sizeof(void*); u16 cy; memcpy(&cy,s+k+2,2);
        if(s[k+14]) seen_sphere_bounce++;
        if(cy>=264) seen_sphere_drop++;
    }
    if(effect_choice==3 && effect_state_6e) {
        /* Pointer area begins at align_up(12, alignof(void*)). */
        const u8 *s=effect_state_6e; size_t k=(12+sizeof(void*)-1)&~(sizeof(void*)-1); k+=5*sizeof(void*);
        int active=0; for(unsigned i=0;i<75;i++) active |= s[k+i];
        if(active) seen_wave_active++; else if(frame>200) seen_wave_silent++;
    }
    if(effect_choice==7 && effect_state_6c) {
        s16 vx,vy; memcpy(&vx,(u8*)effect_state_6c+8,2); memcpy(&vy,(u8*)effect_state_6c+10,2);
        if(frame && (vx!=previous_vx || vy!=previous_vy)) seen_radial_reverse++;
        previous_vx=vx;previous_vy=vy;
    }
    if(pause_mode) {
        seen_signs|=effect_speed<0?1:2;
        seen_timer|=effect_pause_timer!=0;
        seen_blocked|=effect_pause_bloque!=0;
        seen_return|=in_pause==0;
    }
}
static void capture(int result) {
    check_guards(); coverage();
    fprintf(logfile,"%s %u %d %016llx %016llx %016llx %016llx %d\n",case_name,frame++,result,
        (unsigned long long)hash(a,SCREEN),(unsigned long long)hash(b,SCREEN),
        (unsigned long long)hash(tex,TEXTURES),(unsigned long long)hash(RandArray,sizeof(s16)*256),RandomIndex);
    frames_seen++;
}
void __wrap_readinput(void) {}
s16 __wrap_but0pressed(void) { return input_abort, pause_keys; }
s16 __wrap_but1pressed(void) { return 0; }
s16 __wrap_but2pressed(void) { return 0; }
s16 __wrap_but3pressed(void) { return 0; }
u8 __wrap_TOUCHE(u32 key) {
    return pause_mode && pause_keys && (key==0x36 || key==0x2a) && ((frame>=10 && frame<15) || (frame>=700 && frame<706));
}
void __wrap_PC_do_cheats(void) { if(pause_mode && ++cheat_calls>=loop_limit) in_pause=0; }
void __wrap_display_text(const char *text,s16 x,s16 y,u8 font,s8 color) {
    (void)text;(void)x;(void)y;(void)font;(void)color;
    /* Stable text boundary: actual font resources belong to the full-game test. */
    for(unsigned i=0;i<28;i++) DrawBufferNormal[100*320+146+i]=(u8)(0xe0+i%16);
}
void __wrap_SAVE_PALETTE(void *p) { (void)p; }
void __wrap_RESTORE_PALETTE(void) {}
void __wrap_INIT_FADE_IN(void) {}
void __wrap_DO_FADE_OUT(void) {}
void __wrap_EFFACE_VIDEO(void) { memset(a,0,SCREEN);memset(b,0,SCREEN); }
void __wrap_endsynchro(void) {}
void __wrap_synchro(void) {
    if(blood_mode) { if(blood_pending) capture(0); blood_pending=1; }
    else if(pause_mode) { capture(0); if(++cheat_calls>=loop_limit) in_pause=0; }
}
void __wrap_SYNCHRO_LOOP(s16 (*scene)(u32)) {
    if(scene==Do_Effect_Pause_unknown) {
        for(unsigned i=0;i<2000;i++) { s16 done=scene(i); capture(done); if(done) return; }
    } else {
        for(unsigned i=0;i<1500;i++) { s16 done=scene(i); capture(done); if(done) return; }
    }
    fail("scene did not finish");
}
static u8 *surface(size_t n) {
    u8 *p;
#ifdef _WIN64
    /* Force texture/destination pointers above 4 GiB. The second framebuffer
     * also crosses the low-word signed-address boundary used by blood.c. */
    static const uintptr_t address[3]={0x10001000000ULL,0x1007fff0000ULL,0x10002000000ULL};
    size_t offset=surface_count==1?0xf000:0;
    p=VirtualAlloc((void*)address[surface_count],n+2*GUARD+offset,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    if(!p) fail("high-address surface allocation");
    surface_memory[surface_count++]=p;p+=offset;
#else
    p=malloc(n+2*GUARD);surface_memory[surface_count++]=p;
    if(!p) fail("surface allocation");
#endif
    memset(p,0xd3,n+2*GUARD);return p+GUARD;
}
static void begin(const char *name,unsigned seed) {
    if(allocated!=freed) fail("effect allocation leaked");
    allocated=freed=0; case_name=name; frame=cheat_calls=0; input_abort=pause_keys=0; pause_mode=blood_mode=blood_pending=0;
    fixed_seed=seed;port_srand(seed);MakeMyRand(0);
    for(unsigned i=0;i<SCREEN;i++) { a[i]=(u8)((i*37+(i/320)*19+(i>>7))&255);b[i]=(u8)((i*13+(i/320)*11+7)&255); }
    for(unsigned i=0;i<TEXTURES;i++) tex[i]=(u8)(i*17+(i>>8)*3);
    DrawBufferNormal=draw_buffer=a;DisplayBufferNormal=display_buffer=b;EffetBufferNormal=tex;
    GameModeVideo=0;CarteSonAutorisee=MusicCdActive=0;FondAutorise=1;VGA_FREQ=70;
    Bloc_lim_W1=0;Bloc_lim_W2=320;Bloc_lim_H1=0;Bloc_lim_H2=200;default_sprite_clipping();
}
static void end(void) {
    check_guards();if(allocated!=freed) fail("effect allocation leaked");
    fprintf(stderr,"CASE %s frames=%u guards=OK\n",case_name,frame);
}
int main(int argc,char **argv) {
    logfile=argc>1?fopen(argv[1],"w"):stdout;if(!logfile) return 2;
    a=surface(SCREEN);b=surface(SCREEN);tex=surface(TEXTURES);port_clock_init(PORT_CLOCK_DETERMINISTIC);
    fprintf(stderr,"SURFACES bits=%u a=%p b=%p tex=%p\n",(unsigned)(sizeof(void*)*8),(void*)a,(void*)b,(void*)tex);
    language_txt[160]="PAUSE";
    char name[80];
    const unsigned effects[]={1,2,3,4,5,6,7,8};
    for(unsigned e=0;e<sizeof effects/sizeof effects[0];e++) {
        unsigned effect=effects[e],max_mode=(effect==2 || effect==4)?12:effect==6?15:1;
        for(unsigned mode=1;mode<=max_mode;mode++) for(int sign=1;sign>=-1;sign-=2) {
            snprintf(name,sizeof name,"transition_%u_%u_%s",effect,mode,sign>0?"in":"out");begin(name,0x12963);
            if(!(sign>0?sub_38208(effect,mode,a,a):sub_38220(effect,mode,b,a))) fail("initializer rejected");
            sub_38258();sub_38290();end();
        }
    }
    /* Sentinels and random dispatch; each deterministic stream is reproduced. */
    for(unsigned i=0;i<32;i++) {
        snprintf(name,sizeof name,"random_%u",i);begin(name,0x49a0+i*911);
        if(!(i&1?sub_38208(9,100,a,a):sub_38220(9,100,b,a))) fail("random initializer");
        sub_38258();sub_38290();end();
    }
    /* Panel pause: first seven seconds, alternating signs, timer, shift toggle,
     * blocked hold/release and final screen restore. */
    begin("pause",0x19d);pause_mode=1;effect_choice=6;in_pause=1;loop_limit=1400;Do_Effect_Pause();end();
    begin("pause_blocked",0x19d);pause_mode=pause_keys=1;effect_choice=6;in_pause=1;loop_limit=1250;Do_Effect_Pause();end();
    begin("pause_simple",0x19d);pause_mode=1;in_pause=1;loop_limit=35;Do_Effect_Pause_Simple();end();
    begin("pause_simple_fond2",0x19d);pause_mode=1;in_pause=1;loop_limit=35;FondAutorise=2;Do_Effect_Pause_Simple();end();
    /* Explicit helper contract round trip and normal-mode save/restore. */
    begin("band_save_restore",11);effect_copy_band(a,tex,160);effect_copy_band(a+160,tex+51200,160);
    memset(a,0,SCREEN);effect_copy_pause_band(tex,a,160);effect_copy_pause_band(tex+51200,a+160,160);capture(0);
    SaveScreen(a,tex);memset(b,0,SCREEN);RestoreScreen(tex,b);capture(0);end();
    for(unsigned i=0;i<3;i++) {
        snprintf(name,sizeof name,"blood_%u",i);begin(name,0x251+i*111);blood_mode=1;DoEffectBloodOut();
        if(blood_pending) capture(1);end();
    }
    fprintf(stderr,"COVERAGE sphere_modes=%x bounce=%u drop=%u wave_active=%u silent=%u radial_reflections=%u pause_signs=%x timer=%u blocked=%u return=%u frames=%u\n",seen_sphere_modes,seen_sphere_bounce,seen_sphere_drop,seen_wave_active,seen_wave_silent,seen_radial_reverse,seen_signs,seen_timer,seen_blocked,seen_return,frames_seen);
    if(seen_sphere_modes!=7 || !seen_sphere_bounce || !seen_sphere_drop || !seen_wave_active || !seen_wave_silent || !seen_radial_reverse || seen_signs!=3 || !seen_timer || !seen_blocked || !seen_return) fail("phase coverage incomplete");
    if(logfile!=stdout) fclose(logfile);
    for(unsigned i=0;i<surface_count;i++) {
#ifdef _WIN64
        VirtualFree(surface_memory[i],0,MEM_RELEASE);
#else
        free(surface_memory[i]);
#endif
    }
    return 0;
}
