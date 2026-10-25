/* MSCDEX transport and a small CD-ROM device. Packet offsets/status bits follow
   the Microsoft CD-ROM driver interface; game policy remains in cdaudio.c.
   The BIN is opened once, read-only, and never copied or extracted. */
#include "platform/cdrom.h"
#include "dos_regs.h"
#include "platform/cd_audio.h"
#include "platform/clock.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef _WIN32
#define seek64 _fseeki64
#define tell64 _ftelli64
#else
#define seek64 fseeko
#define tell64 ftello
#endif

typedef struct { uint32_t start, index0; uint8_t data, has_index0; } track_t;
static track_t tracks[100];
static unsigned first_track, last_track;
static uint32_t leadout;
static FILE *bin;
static int drive = 3, door, locked;
static uint8_t volume[4] = {255,255,255,255}, channel[4] = {0,1,2,3};
static union { uint64_t alignment; uint8_t data[65536]; } memory;
static size_t allocated;
static uint32_t start_lba, end_lba, position;
static uint64_t started;
static int playing, paused;
static FILE *trace;

static uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }
static uint32_t rd32(const uint8_t *p) { return rd16(p) | (uint32_t)rd16(p+2) << 16; }
static void wr16(uint8_t *p, uint16_t v) { p[0]=(uint8_t)v; p[1]=(uint8_t)(v>>8); }
static void wr32(uint8_t *p, uint32_t v) { wr16(p,(uint16_t)v); wr16(p+2,(uint16_t)(v>>16)); }

uint32_t port_cd_msf(uint32_t lba)
{
    uint32_t f = lba + 150;
    return (f / 4500 << 16) | ((f / 75 % 60) << 8) | (f % 75);
}

int port_cd_lba(uint32_t msf, uint32_t *lba)
{
    unsigned m = msf >> 16, s = msf >> 8 & 255, f = msf & 255;
    if (m > 255 || s >= 60 || f >= 75 || m * 4500 + s * 75 + f < 150) return 0;
    *lba = m * 4500 + s * 75 + f - 150;
    return 1;
}

static void msf_bytes(uint8_t *p, uint32_t frames)
{
    p[0]=(uint8_t)(frames/4500); p[1]=(uint8_t)(frames/75%60); p[2]=(uint8_t)(frames%75);
}

static unsigned track_at(uint32_t lba)
{
    unsigned t = last_track;
    while (t > first_track && tracks[t].start > lba) --t;
    return t;
}

uint32_t port_cd_position(void)
{
    if (playing) {
        uint64_t elapsed = port_clock_ns() - started;
        uint64_t p = (uint64_t)start_lba + elapsed / 1000000000 * 75 + elapsed % 1000000000 * 75 / 1000000000;
        if (p >= end_lba) { position = end_lba; playing = paused = 0; }
        else position = (uint32_t)p;
    }
    return position;
}

void port_cd_pump(void)
{
    port_cd_position();
    port_cd_audio_pump(playing, position, end_lba);
}

static void stop(void)
{
    port_cd_position();
    paused = playing; /* STOP while paused discards the resume address (the game stops twice). */
    playing = 0;
    port_cd_audio_reset();
}

void port_cd_shutdown(void)
{
    port_cd_audio_shutdown();
    if (bin) fclose(bin);
    if (trace) fclose(trace);
    bin = trace = NULL;
    allocated = 0;
    first_track = last_track = leadout = 0;
    playing = paused = door = locked = 0;
    start_lba = end_lba = position = 0;
}

int port_cd_init(const char *cue, char letter)
{
    FILE *f;
    char line[2048], filename[1024] = "", mode[32], path[2048];
    unsigned t = 0, n, index, m, s, frame, files = 0, seen[100] = {0};
    long long bytes;
    const char *slash, *backslash, *log;
    port_cd_shutdown();
    drive = toupper((unsigned char)letter) - 'A';
    if (drive < 0 || drive >= 26) return 0;
    memset(tracks, 0, sizeof tracks);
    for (n=0;n<4;n++) { volume[n]=255; channel[n]=(uint8_t)n; }
    log = getenv("RAYMAN_CD_TRACE");
    if (log && *log) trace = fopen(log, "w");
    if (!cue || !*cue) return 1; /* Resident driver and closed empty drive. */
    if (!(f = fopen(cue, "r"))) goto bad;
    while (fgets(line, sizeof line, f)) {
        char *p = line;
        while (isspace((unsigned char)*p)) ++p;
        if (!strncmp(p,"FILE ",5)) {
            if (++files > 1 || sscanf(p,"FILE \"%1023[^\"]\" %31s",filename,mode)!=2 || strcmp(mode,"BINARY")) goto bad_cue;
        } else if (!strncmp(p,"TRACK ",6)) {
            if (sscanf(p,"TRACK %u %31s",&n,mode)!=2 || !files || n == 0 || n > 98 || n != t+1) goto bad_cue;
            if (t && !seen[t]) goto bad_cue;
            t=n;
            if (!strcmp(mode,"AUDIO")) tracks[t].data=0;
            else if (!strcmp(mode,"MODE1/2352")) tracks[t].data=0x40;
            else goto bad_cue;
            last_track=t;
        } else if (!strncmp(p,"INDEX ",6)) {
            if (sscanf(p,"INDEX %u %u:%u:%u",&index,&m,&s,&frame)!=4 || !t || s>=60 || frame>=75 || m>255) goto bad_cue;
            if (index==1) {
                if (seen[t]) goto bad_cue;
                tracks[t].start=m*4500+s*75+frame;
                seen[t]=1;
            } else if (index==0 && !seen[t] && !tracks[t].has_index0) {
                tracks[t].index0=m*4500+s*75+frame;
                tracks[t].has_index0=1;
            } else goto bad_cue;
        } else if (!strncmp(p,"PREGAP ",7) || !strncmp(p,"POSTGAP ",8)) goto bad_cue;
    }
    if (ferror(f) || !t || !seen[t] || tracks[1].start != 0) goto bad_cue;
    fclose(f);
    slash=strrchr(cue,'/'); backslash=strrchr(cue,'\\');
    if (!slash || (backslash && backslash > slash)) slash=backslash;
    if (filename[0]=='/' || filename[0]=='\\' || filename[1]==':') snprintf(path,sizeof path,"%s",filename);
    else snprintf(path,sizeof path,"%.*s%s",slash ? (int)(slash-cue+1) : 0,cue,filename);
    if (!(bin=fopen(path,"rb"))) goto bad;
    if (seek64(bin,0,SEEK_END) || (bytes=tell64(bin))<=0 || bytes%2352 || (uint64_t)bytes/2352 > UINT32_MAX-150) goto bad;
    leadout=(uint32_t)(bytes/2352);
    for (n=1;n<=t;n++)
        if (tracks[n].start>=leadout || (n>1 && tracks[n].start<=tracks[n-1].start) ||
            (tracks[n].has_index0 && (tracks[n].index0>tracks[n].start ||
             (n>1 && tracks[n].index0<tracks[n-1].start)))) goto bad;
    first_track=1;
    fprintf(stdout,"Virtual MSCDEX 2.21: %c: %u tracks, lead-out LBA %u\n",'A'+drive,last_track,leadout);
    return 1;
bad_cue:
    fclose(f);
bad:
    fprintf(stderr,"CD image: invalid or unsupported single-file MODE1/2352 + AUDIO CUE/BIN: %s\n",cue ? cue : "");
    port_cd_shutdown();
    return 0;
}

int port_cd_drive(void) { return drive; }
int port_cd_present(void) { return bin != NULL && !door; }
int port_cd_path(const char *p) { return p[0] && p[1]==':' && toupper((unsigned char)p[0])=='A'+drive; }

uint16_t port_cd_allocate(uint16_t bytes, uint32_t *address)
{
    if (!bytes || allocated) { *address=0; return 0; }
    allocated=bytes;
    memset(memory.data,0,sizeof memory.data);
    *address=0x10000000; /* 1000:0000, linear address 10000h */
    return 0x1000;
}
uint8_t port_cd_free(uint16_t handle)
{
    if (handle != 0x1000 || !allocated) return 1;
    allocated=0;
    return 0;
}
uint8_t *port_cd_buffer(uint32_t offset, size_t bytes)
{
    if (offset > allocated || bytes > allocated-offset) return NULL;
    return memory.data+offset;
}
static uint8_t *far_buffer(uint16_t seg, uint16_t off, size_t bytes)
{
    uint32_t linear=(uint32_t)seg*16+off;
    if (linear<0x10000) return NULL;
    return port_cd_buffer(linear-0x10000,bytes);
}

static int read_sector(uint32_t lba, int raw, uint8_t *out)
{
    if (!port_cd_present() || lba>=leadout) return 0;
    if (!raw && !tracks[track_at(lba)].data) return 0;
    return seek64(bin,(uint64_t)lba*2352+(raw?0:16),SEEK_SET)==0 && fread(out,1,raw?2352:2048,bin)==(size_t)(raw?2352:2048);
}

size_t port_cd_pcm(uint64_t sample, int16_t *out, size_t frames)
{
    uint8_t sector[2352];
    size_t done=0;
    if (!port_cd_present()) return 0;
    while (done<frames && sample/588<leadout) {
        unsigned lba=(unsigned)(sample/588), i=(unsigned)(sample%588);
        size_t count=588-i, j;
        if (count>frames-done) count=frames-done;
        if (tracks[track_at(lba)].data || !read_sector(lba,1,sector)) break;
        for (j=0;j<count;j++) {
            int v[4]={(int16_t)rd16(sector+4*(i+j)),(int16_t)rd16(sector+4*(i+j)+2),0,0};
            out[2*(done+j)]=(int16_t)(v[channel[0]&3]*volume[0]/255);
            out[2*(done+j)+1]=(int16_t)(v[channel[1]&3]*volume[1]/255);
        }
        done+=count; sample+=count;
    }
    return done;
}

static unsigned ioctl_input(uint8_t *b, size_t n)
{
    unsigned code=b[0], t, need;
    static const uint8_t sizes[16]={0,0,0,0,9,0,5,0,0,0,7,7,11,0,0,11};
    if (code>=16 || !(need=sizes[code])) return 3;
    if (n<need) return 5;
    if (code==6) {
        wr32(b+1,0x314u | (!locked?2u:0u) | (door?1u:0u) | (!port_cd_present()?0x800u:0u));
    } else if (code==4) {
        for (t=0;t<4;t++) { b[1+2*t]=channel[t]; b[2+2*t]=volume[t]; }
    } else {
        if (!port_cd_present()) return 2;
        switch (code) {
        case 10: b[1]=(uint8_t)first_track; b[2]=(uint8_t)last_track; wr32(b+3,port_cd_msf(leadout)); break;
        case 11:
            t=b[1];
            if (t<first_track || t>last_track) return 8;
            wr32(b+2,port_cd_msf(tracks[t].start)); b[6]=tracks[t].data; break;
        case 12:
            t=last_track;
            while (t>first_track && position<(tracks[t].has_index0?tracks[t].index0:tracks[t].start)) --t;
            b[1]=tracks[t].data; b[2]=(uint8_t)((t/10<<4)|t%10);
            b[3]=(uint8_t)(position>=tracks[t].start);
            msf_bytes(b+4,b[3]?position-tracks[t].start:tracks[t].start-position);
            b[7]=0; msf_bytes(b+8,position+150); break;
        case 15:
            wr16(b+1,(uint16_t)paused); wr32(b+3,port_cd_msf(paused?position:start_lba)); wr32(b+7,port_cd_msf(end_lba)); break;
        }
    }
    return 0;
}

static unsigned ioctl_output(uint8_t *b, size_t n)
{
    unsigned i;
    switch (b[0]) {
    case 0:
        if (locked) return 2;
        stop(); paused=0; door=1; break;
    case 1: if (n<2) return 5; locked=b[1]!=0; break;
    case 3:
        if (n<9) return 5;
        for (i=0;i<4;i++) if (b[1+2*i]>3) return 3;
        for (i=0;i<4;i++) { channel[i]=b[1+2*i]; volume[i]=b[2+2*i]; }
        port_cd_audio_reset(); break;
    case 5: door=0; break;
    default: return 3;
    }
    return 0;
}

void port_cd_request(unsigned requested_drive, uint8_t *r, size_t cap)
{
    unsigned error=0, cmd, code=0xff;
    uint8_t *b;
    uint32_t lba, length;
    size_t count, i;
    port_cd_position();
    if (cap<5) return;
    cmd=r[2];
    if (cap<13 || r[0]<13) error=5;
    else if (requested_drive!=(unsigned)drive || r[1]!=0) error=1;
    else switch (cmd) {
    case 3: case 12:
        if (cap<26) { error=5; break; }
        count=rd16(r+18);
        b=far_buffer(rd16(r+16),rd16(r+14),count);
        if (!b || !count) { error=5; break; }
        code=b[0];
        error=cmd==3?ioctl_input(b,count):ioctl_output(b,count);
        break;
    case 0x84:
        if (cap<22) { error=5; break; }
        if (!port_cd_present()) { error=2; break; }
        lba=rd32(r+14); length=rd32(r+18);
        if (r[13]>1 || (r[13]==1 && !port_cd_lba(lba,&lba))) { error=8; break; }
        if (lba>=leadout || !length || length>leadout-lba) { error=8; break; }
        for (i=track_at(lba); i<=last_track && tracks[i].start<lba+length; i++)
            if (tracks[i].data) { error=7; break; }
        if (error) break;
        position=start_lba=lba; end_lba=lba+length; started=port_clock_ns(); playing=1; paused=0;
        port_cd_audio_reset(); break;
    case 0x85:
        if (!port_cd_present()) error=2; else stop();
        break;
    case 0x88:
        if (!port_cd_present() || !paused) { error=2; break; }
        start_lba=position; started=port_clock_ns(); playing=1; paused=0; port_cd_audio_reset(); break;
    case 0x80: /* Not issued by cdaudio, provided for a complete bounded read transport. */
        if (cap<27) { error=5; break; }
        if (!port_cd_present()) { error=2; break; }
        if (playing) { error=12; break; }
        lba=rd32(r+20); length=rd16(r+18);
        if (r[13]>1 || r[24]>1 || (r[13]==1 && !port_cd_lba(lba,&lba))) { error=8; break; }
        count=(size_t)length*(r[24]?2352:2048);
        b=far_buffer(rd16(r+16),rd16(r+14),count);
        if (!b || lba>=leadout || length>leadout-lba) { error=8; break; }
        for (i=0;i<length;i++) if (!read_sector(lba+(uint32_t)i,r[24],b+i*(r[24]?2352:2048))) { error=11; break; }
        break;
    default: error=3;
    }
    wr16(r+3,(uint16_t)(0x100 | (playing?0x200:0) | (error?0x8000|error:0)));
    if (trace) {
        fprintf(trace,"%llu drive=%c cmd=%02x ioctl=%02x status=%04x pos=%u end=%u\n",
                (unsigned long long)port_clock_ns(),'A'+requested_drive,cmd,code,rd16(r+3),position,end_lba);
        fflush(trace);
    }
    port_cd_pump();
}

int port_cd_interrupt(dos_regs_t *r)
{
    uint8_t *b;
    uint32_t linear;
    if ((r->ax & 0xff00)!=0x1500) return 0;
    if (trace) { fprintf(trace,"%llu INT2F ax=%04x cx=%04x\n", (unsigned long long)port_clock_ns(),r->ax,r->cx); fflush(trace); }
    r->flags &= (uint16_t)~1;
    switch (r->ax) {
    case 0x1500: r->bx=1; r->cx=(uint16_t)drive; break;
    case 0x150b: r->ax=r->cx==drive; r->bx=0xadad; break;
    case 0x150c: r->bx=0x0215; break; /* binary minor 21, not packed BCD */
    case 0x1510:
        linear=(uint32_t)r->es*16+r->bx;
        b=far_buffer(r->es,r->bx,13);
        if (!b) { r->flags|=1; r->ax=5; break; }
        port_cd_request(r->cx,b,allocated-(linear-0x10000)); break;
    case 0x1502: case 0x1503: case 0x1504:
        /* PVD optional copyright/abstract/bibliographic identifiers. */
        if (r->cx!=drive || !port_cd_present()) { r->flags|=1; r->ax=21; break; }
        b=far_buffer(r->es,r->bx,38);
        if (!b) { r->flags|=1; r->ax=5; break; }
        {
            uint8_t pvd[2048]; size_t n=37, off=702+(r->ax-0x1502)*37;
            if (!read_sector(16,0,pvd)) { r->flags|=1; r->ax=21; break; }
            while (n && (pvd[off+n-1]==' ' || !pvd[off+n-1])) --n;
            memcpy(b,pvd+off,n); b[n]=0;
        }
        break;
    default: r->flags|=1; r->ax=1; break;
    }
    return 1;
}
