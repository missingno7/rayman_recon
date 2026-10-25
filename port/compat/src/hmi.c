/* HMI API model. Contracts: src/hmi/{hmi_samples,hmi_active,hmi_status,hmi_voice_params}.c.
   Playback is clock driven; the SDL queue is presentation only. Main thread owns all state/data. */
#include "platform/hmi.h"
#include "platform/hmi_pcm.h"
#include "platform/audio.h"
#include "platform/clock.h"
#include "platform/host.h"
#include "compat/src/pc_hardware.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define ACTIVE 0x8000u
#define LOOPING 0x4000u
#define STAGE_LOOP 0x40u
#define PITCH_SHIFT 0x400u
#define PANNING 0x200u
#define VOLUME 0x100u
#define SPEECH 0x80u
#define POLL_NS 1000ULL /* one microsecond per status read, like an ISA game-port read */
static struct voice {
    dos_sound_request_t request;
    uint64_t started;
    uint32_t flags, step;
    uint16_t volume, pan, id;
    int bank, program;
} voices[32];
static int initialized, driver_live;
static unsigned char timer_events[16];
static SDL_AudioStream *stream;
static uint64_t next_ns;
static FILE *trace;
extern bnk_header_t bnkHeaderFixe[128], bnkHeaderWorld[128];
extern archive_header_t sndvig_infos[9];

static void trace_event(const char *event, int n)
{
    struct voice *v = n >= 0 && n < 32 ? &voices[n] : NULL;
    if (trace) {
        fprintf(trace,"%s %llu %d %d %d %u %u %u\n",event,
                (unsigned long long)port_clock_ns(), n, v?v->bank:-1, v?v->program:-1,
                v?v->request.size:0, v?v->step:0, v?v->flags:0);
        fflush(trace);
    }
}
/* Split before multiplying so a long-running clock cannot overflow nanoseconds*Q16. */
static uint64_t consumed(const struct voice *v, uint64_t ns)
{
    uint64_t dt = ns > v->started ? ns - v->started : 0;
    uint64_t ticks = dt / 100000ULL; /* 10000 Hz reference ticks */
    return (ticks / 65536) * v->step +
        ((ticks % 65536) * v->step + (dt % 100000) * v->step / 100000) / 65536;
}
uint64_t port_hmi_frame(unsigned n, uint64_t ns)
{
    const struct voice *v;
    uint64_t frame, length, start, span;
    unsigned width;
    int16_t count;
    if (n >= 32 || !(voices[n].flags & ACTIVE)) return UINT64_MAX;
    v = &voices[n];
    width = (v->flags & SPEECH) ? 2 : 1;
    frame = consumed(v, ns);
    length = v->request.size / width;
    if (v->flags & LOOPING) {
        start = (v->flags & STAGE_LOOP) ? v->request.field_30 / width : 0;
        span = (v->flags & STAGE_LOOP) ? v->request.field_34 / width : length;
        count = (int16_t)v->request.field_0c;
        if (span && start <= length && span <= length - start && frame >= start) {
            uint64_t after = frame - start;
            if (count == -1 || (count > 0 && after / span < (unsigned)count))
                return start + after % span;
            if (count > 0) frame -= (uint64_t)count * span;
        }
    }
    return frame < length ? frame : UINT64_MAX;
}
static void update(void)
{
    unsigned i;
    uint64_t now = port_clock_ns();
    for (i=0; i<32; ++i)
        if ((voices[i].flags & ACTIVE) && port_hmi_frame(i,now)==UINT64_MAX) {
            voices[i].flags &= ~ACTIVE;
            trace_event("Done",i);
        }
}
static void invalidate(void)
{
    if (stream) SDL_ClearAudioStream(stream);
    next_ns = port_clock_ns();
}
static void status_poll(void)
{
    if (port_clock_mode() == PORT_CLOCK_DETERMINISTIC)
        port_clock_wait_until(port_clock_ns()+POLL_NS);
    port_pc_poll(); /* includes host keyboard IRQ delivery, CD and HMI pumping */
    update();
}
s32 sosDIGIInitSystem(s32 a, s32 b, s32 c)
{
    const char *path = getenv("RAYMAN_SOUND_TRACE");
    (void)a; (void)b; (void)c;
    initialized = 1;
    if (!trace && path && *path) trace = fopen(path,"w");
    return 0;
}
s32 sosDIGIUnInitSystem(void) { initialized=0; return 0; }
s32 sosDIGIInitDriver(s32 id, s32 rate, s32 *hardware, void *config, void *handle)
{
    dos_sound_config_t *cfg = config;
    (void)id; (void)rate; (void)hardware;
    if (!initialized) return 1;
    memset(voices,0,sizeof voices);
    driver_live=1;
    /* The DOS callback is its loaded driver's mixer service, not game code.
       A nonnull native token preserves registration; port_hmi_pump replaces its work. */
    if (cfg) cfg->callback = &driver_live;
    if (handle) *(s32 *)handle=0;
    invalidate();
    trace_event("Init",-1);
    port_hmi_pump(); /* Start native presentation as DOS InitDriver started its DMA service. */
    if (trace) {
        SDL_AudioSpec spec;
        if (port_audio_spec(&spec)) {
            fprintf(trace,"Device %d %d %u %d\n",spec.freq,spec.channels,spec.format,port_hmi_audio_queued());
            fflush(trace);
        }
    }
    return 0;
}
s32 sosDIGIUnInitDriver(s32 driver,s32 enabled,s32 flags)
{
    (void)enabled; (void)flags;
    if (driver!=0 || !driver_live) return 1;
    memset(voices,0,sizeof voices); driver_live=0; invalidate(); return 0;
}
/* No hardware probing on this host. sub_3E710 is orphaned on supported paths;
   never copy the DOS 106-byte capabilities record into current_device_name[100]. */
s32 sosDIGIDetectInit(s32 a,s16 b) { (void)a;(void)b; return 1; }
s32 sosDIGIDetectUnInit(void) { return 0; }
s32 sosDIGIDetectFindFirst(const char *name,void *out) { (void)name;(void)out;return 1; }
s32 sosDIGIDetectGetSettings(s32 *out) { (void)out;return 1; }
s32 sosDIGIStartSample(s32 driver,dos_sound_request_t *request)
{
    int i,j;
    struct voice *v;
    if (driver!=0 || !driver_live || !request) return -1;
    update();
    for (i=0;i<32;i++) if (!(voices[i].flags & ACTIVE)) break;
    if (i==32) { trace_event("Full",-1); return -1; }
    v=&voices[i];
    v->request=*request; v->started=port_clock_ns();
    v->flags=(uint16_t)request->flags|0xa000;
    v->step=(request->flags&PITCH_SHIFT)?(uint32_t)request->sample_rate:65536;
    v->volume=(uint16_t)request->volume; v->pan=(uint16_t)request->pan;
    v->id=(uint16_t)request->field_18[0];
    v->bank=-1; v->program=-1;
    if (request->flags&SPEECH) {
        v->bank=2;
        for (j=0;j<9;j++) if (sndvig_infos[j].size==request->size) v->program=j;
    } else for (j=0;j<128;j++) {
        if (bnkHeaderFixe[j].size && bnkHeaderFixe[j].offset==request->data) {v->bank=0;v->program=j;break;}
        if (bnkHeaderWorld[j].size && bnkHeaderWorld[j].offset==request->data) {v->bank=1;v->program=j;break;}
    }
    trace_event("StartSample",i); invalidate();
    return i;
}
s32 sosDIGIStopSample(s32 driver,s16 n)
{
    (void)driver;
    if ((unsigned)n>=32) return 10;
    update(); trace_event("StopSample",n);
    if ((voices[n].flags&ACTIVE) && !(voices[n].flags&0x1000)) {
        voices[n].flags &= ~ACTIVE; voices[n].id=0; invalidate();
    }
    return 0;
}
s32 sosDIGISampleDone(s32 driver,s16 n)
{
    (void)driver;
    if ((unsigned)n>=32) return 10;
    status_poll();
    return (voices[n].flags&ACTIVE)?0:1;
}
s32 sosDIGISamplesPlaying(s32 driver)
{
    unsigned i,count=0; (void)driver; status_poll();
    for (i=0;i<32;i++) if (voices[i].flags&ACTIVE) count++;
    return count;
}
s32 sosDIGISetSampleVolume(s32 driver,s16 n,s32 volume)
{
    uint16_t old; (void)driver; update();
    if ((unsigned)n>=32 || !(voices[n].flags&ACTIVE)) return 0;
    old=voices[n].volume; voices[n].volume=(uint16_t)volume; invalidate(); return (int16_t)old;
}
s32 sosDIGISetPanLocation(s32 driver,s16 n,s32 pan)
{
    uint16_t old; (void)driver; update();
    if ((unsigned)n>=32 || !(voices[n].flags&ACTIVE)) return 0;
    old=voices[n].pan; voices[n].pan=(uint16_t)pan; invalidate(); return (int16_t)old;
}
s32 sosTIMERInitSystem(s32 period,s32 flags)
{ if (period && !(flags&1)) timer_events[15]=1; return 0; }
s32 sosTIMERUnInitSystem(s32 flags) { (void)flags; return 0; }
s32 sosTIMERRegisterEvent(s32 rate,void *callback,void *handle)
{
    int i; (void)rate;
    for (i=0;i<16;i++) if (!timer_events[i]) break;
    if (i==16) return 11;
    timer_events[i]=callback!=NULL;
    if (handle) *(s32 *)handle=i;
    return 0;
}
s32 sosTIMERRemoveEvent(s32 handle)
{ if ((unsigned)handle<16) timer_events[handle]=0; return 0; }
uint32_t port_hmi_flags(unsigned n) { update();return n<32?voices[n].flags:0; }
void port_hmi_mix(float *out,size_t frames,unsigned rate,uint64_t ns)
{
    size_t f; unsigned n;
    memset(out,0,frames*2*sizeof *out);
    if (!rate) return;
    for (n=0;n<32;n++) {
        const struct voice *v=&voices[n];
        unsigned width=(v->flags&SPEECH)?2:1;
        /* Linear balance: full gain in both channels at centre, zero opposite at extremes.
           This is presentation, not the original SB integer mixing arithmetic. */
        unsigned pan=(v->flags&PANNING)?v->pan:0x8000;
        float gain=(v->flags&VOLUME)?(float)(v->volume>0x7fff?0x7fff:v->volume)/32767.0f:1.0f;
        float left=pan<=0x8000?1.0f:(float)(0xffff-pan)/32767.0f;
        float right=pan>=0x8000?1.0f:(float)pan/32768.0f;
        if (!(v->flags&ACTIVE) || !v->request.data) continue;
        for (f=0;f<frames;f++) {
            uint64_t frame=port_hmi_frame(n,ns+f*1000000000ULL/rate);
            if (frame!=UINT64_MAX) {
                float s=(float)port_hmi_pcm(v->request.data+frame*width,width==2)/32768.0f*gain;
                out[2*f]+=s*left; out[2*f+1]+=s*right;
            }
        }
    }
    for (f=0;f<frames*2;f++) { if(out[f]>1)out[f]=1; if(out[f]<-1)out[f]=-1; }
}
void port_hmi_pump(void)
{
    float pcm[1024*2]; unsigned rate; size_t frames;
    uint64_t now=port_clock_ns();
    update();
    if (!driver_live || port_host_headless() || port_clock_mode()==PORT_CLOCK_DETERMINISTIC) return;
    if (!stream) { stream=port_audio_stream(SDL_AUDIO_F32,2,0); next_ns=now; }
    if (!stream) return;
    rate=(unsigned)port_audio_rate();
    if (next_ns<now || next_ns>now+100000000ULL) {SDL_ClearAudioStream(stream);next_ns=now;}
    while (next_ns<now+20000000ULL && SDL_GetAudioStreamQueued(stream)<(int)sizeof pcm*2) {
        frames=rate/100; if(frames>1024)frames=1024;
        port_hmi_mix(pcm,frames,rate,next_ns);
        if (!SDL_PutAudioStreamData(stream,pcm,(int)(frames*2*sizeof *pcm))) break;
        next_ns+=frames*1000000000ULL/rate;
    }
}
int port_hmi_audio_queued(void) { return stream ? SDL_GetAudioStreamQueued(stream) : -1; }
void port_hmi_shutdown(void)
{
    if (stream) SDL_DestroyAudioStream(stream);
    stream=NULL;
    if (trace) fclose(trace);
    trace=NULL;
    memset(voices,0,sizeof voices); memset(timer_events,0,sizeof timer_events);
    initialized=driver_live=0;
}
