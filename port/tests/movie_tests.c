/* Exercise the actual derived TU, including its private native records. The archive's movie.o is not pulled
   in: all public entries are supplied here. Palette/synchro observers check fade steps; input observers
   check poll order and end a loop, with a configured no-input fixture in place of the game entry setup. */
#define write_movie_palette observed_palette
#define synchro observed_synchro
#define readinput observed_readinput
#define TOUCHE observed_TOUCHE
#include PORT_MOVIE_SOURCE
#undef write_movie_palette
#undef synchro
#undef readinput
#undef TOUCHE
#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "platform/files.h"
#include "platform/host.h"
#include "platform/cdrom.h"
#include "platform/hmi.h"
#include "video/vga.h"

void write_movie_palette(u8 *);
void synchro(void);
void readinput(void);
extern void (*Read_Input_Func)(void);
extern u8 input_mode;
static void no_input(void) {}
u8 TOUCHE(u8);
static movie_work_t *input_work;
static unsigned input_reads, input_stop, input_indices[250], entry_reads;
void observed_readinput(void)
{
    if (input_work) {
        if (input_reads < 250) input_indices[input_reads] = input_work->frame_index;
        ++input_reads;
    } else ++entry_reads;
    readinput();
}
u8 observed_TOUCHE(u8 key)
{
    return key == 0x39 && input_work && input_stop && input_reads >= input_stop ? 1 : TOUCHE(key);
}
static unsigned checks, failures, fade_steps, fade_syncs;
static int in_fade;
static u8 fade_initial[768];
#define CHECK(c) do { ++checks; if (!(c)) { ++failures; fprintf(stderr,"FAIL %d: %s\n",__LINE__,#c); } } while (0)
void observed_synchro(void) { if (in_fade) ++fade_syncs; synchro(); }
void observed_palette(u8 *pal)
{
    unsigned i;
    if (in_fade) {
        ++fade_steps;
        CHECK(fade_syncs == fade_steps);
        for (i=0;i<768;++i) {
            int c = (int)fade_initial[i] - 2*(int)fade_steps;
            CHECK(pal[i] == (c < 0 ? 0 : c));
        }
    }
    write_movie_palette(pal);
}
static u8 pixels[64000], reference[64000], refpal[768], scratch_frame[262144];
static unsigned chunks_seen[19];
static unsigned le16(const u8 *p) { return p[0] | p[1]<<8; }
static u32 le32(const u8 *p) { return le16(p) | (u32)le16(p+2)<<16; }
static void put16(u8 *p,unsigned n) { p[0]=(u8)n;p[1]=(u8)(n>>8); }
static void put32(u8 *p,u32 n) { put16(p,n);put16(p+2,n>>16); }

/* Independent scalar reference for the MOVIE_FORMAT contract. No native decoder helpers or records used. */
static void reference_chunk(unsigned type,const u8 *p,int w,int h)
{
    int x0=(320-w)/2,y0=(200-h)/2,x,y,n,k,lines,packets;
    switch(type) {
    case 4: case 11:
        packets=le16(p);p+=2;x=0;
        while(packets--) {
            x+=*p++;n=*p++;if(!n)n=256;
            for(k=0;k<n*3;++k)refpal[x*3+k]=type==4 ? p[k]>>2 : p[k];
            p+=n*3;x+=n;
        }
        break;
    case 13: memset(reference,0,sizeof reference);break;
    case 16:
        for(y=y0;y<y0+h;++y) { memcpy(reference+y*320+x0,p,w);p+=w; }
        break;
    case 15:
        for(y=y0;y<y0+h;++y) {
            ++p;x=x0;
            while(x<x0+w) {
                n=(int8_t)*p++;
                if(n>=0) { memset(reference+y*320+x,*p++,n);x+=n; }
                else { n=-n;memcpy(reference+y*320+x,p,n);p+=n;x+=n; }
            }
        }
        break;
    case 12:
        y=y0+le16(p);lines=le16(p+2);p+=4;
        while(lines--) {
            packets=*p++;x=x0;
            while(packets--) {
                x+=*p++;n=(int8_t)*p++;
                if(n<0) { n=-n;memset(reference+y*320+x,*p++,n); }
                else { memcpy(reference+y*320+x,p,n);p+=n; }
                x+=n;
            }
            ++y;
        }
        break;
    case 7:
        lines=le16(p);p+=2;y=y0;
        while(lines) {
            packets=(int16_t)le16(p);p+=2;
            if(packets<0) {
                if(packets&0x4000) { y-=packets;continue; }
                /* DOS odd-byte control intentionally swaps the conventional coordinates. */
                reference[(x0+w-1)*320+(u8)packets]=(u8)y;
                packets=(int16_t)le16(p);p+=2;
            }
            x=x0;
            while(packets--) {
                x+=*p++;n=(int8_t)*p++;
                if(n<0) {
                    n=-n;for(k=0;k<n;++k) {reference[y*320+x++]=p[0];reference[y*320+x++]=p[1];}p+=2;
                } else { memcpy(reference+y*320+x,p,n*2);p+=n*2;x+=n*2; }
            }
            ++y;--lines;
        }
        break;
    default: break; /* Includes 18: skipped, with next chunk located by its total size. */
    }
}
static void synthetic_chunks(void)
{
    movie_work_t w={0};movie_surface_t s={pixels,320,200,0};
    movie_frame_header_t f={0};u8 buf[1024];unsigned i;
    w.width=4;w.height=2;sub_3AFB4(&w,&s);
    CHECK(w.x_offset==158 && w.y_offset==99 && w.surface_offset==99*320+158);
    for(i=0;i<10;++i) {
        static const int dims[][2]={{320,200},{240,150},{1,1},{319,199},{2,2},{0,0},{21,17},{100,50},{318,198},{7,9}};
        w.width=dims[i][0];w.height=dims[i][1];sub_3AFB4(&w,&s);
        CHECK(w.x_offset==(320-w.width)/2 && w.y_offset==(200-w.height)/2);
    }
    w.width=4;w.height=2;sub_3AFB4(&w,&s);
    /* RGB8 packet run 0 means 256, skipping index 0. */
    memset(buf,0,sizeof buf);put16(buf,1);
    for(i=0;i<768;++i)buf[4+i]=(u8)i;
    sub_3A6D0(buf,&w,&s);for(i=0;i<768;++i)CHECK(movie_video_buffer[i]==(u8)i>>2);
    CHECK(movie_palette_xmin==0 && movie_palette_xmax==256);
    /* RGB6, packet skips and two runs. */
    { const u8 pal[]={2,0,2,1,1,2,3,3,1,7,8,9};
      sub_3A6E0(pal,&w,&s);CHECK(movie_video_buffer[6]==1 && movie_video_buffer[18]==7);
      CHECK(movie_palette_xmin==2 && movie_palette_xmax==7); }
    /* BLK clears the entire 320x200 surface, including outside the centred movie. */
    memset(pixels,0xee,sizeof pixels);sub_3A9F4(NULL,&w,&s);
    for(i=0;i<sizeof pixels;++i)if(pixels[i])break;CHECK(i==sizeof pixels);
    /* Raw, BRUN (ignored stored packet count), byte delta, word delta and unknown chunk. */
    {
        const u8 raw[]={1,2,3,4,5,6,7,8};
        const u8 brun[]={99,2,8,254,2,3,0,252,4,5,6,7};
        const u8 byte_delta[]={0,0,1,0,2,0,254,9,0,2,7,6};
        const u8 word_delta[]={1,0,2,0,0,255,1,2,0,1,3,4};
        const u8 word_skip[]={1,0,255,255,1,0,0,255,11,12};
        const u8 word_odd[]={1,0,7,128,0,0};
        const u8 skip[]={10,0,0,0,18,0,0xff,0xff,0xff,0xff};
        memset(reference,0,sizeof reference);memset(pixels,0,sizeof pixels);
        reference_chunk(16,raw,4,2);sub_3AA08(raw,&w,&s);CHECK(!memcmp(pixels,reference,sizeof pixels));
        reference_chunk(15,brun,4,2);sub_3A6F0(brun,&w,&s);CHECK(!memcmp(pixels,reference,sizeof pixels));
        reference_chunk(12,byte_delta,4,2);sub_3A7B4(byte_delta,&w,&s);CHECK(!memcmp(pixels,reference,sizeof pixels));
        reference_chunk(7,word_delta,4,2);sub_3A8A0(word_delta,&w,&s);CHECK(!memcmp(pixels,reference,sizeof pixels));
        reference_chunk(7,word_skip,4,2);sub_3A8A0(word_skip,&w,&s);CHECK(!memcmp(pixels,reference,sizeof pixels));
        reference_chunk(7,word_odd,4,2);sub_3A8A0(word_odd,&w,&s);CHECK(!memcmp(pixels,reference,sizeof pixels));
        CHECK(pixels[161*320+7]==99); /* DOS's unusual odd-byte coordinates */
        f.chunks=1;movie_audio_enabled=0;movie_audio_track=0;MusicCdActive=0;
        CHECK(sub_3ABE4(&w,&f,skip,&s,&s.timer)==0);CHECK(!memcmp(pixels,reference,sizeof pixels));
    }
}
static void corpus(const char *name,int count,const unsigned expected[19])
{
    movie_work_t w;movie_surface_t s={pixels,320,200,0};FILE *f;u8 header[16];
    unsigned frames=0,n,size,i,j;int mismatches=0;
    CHECK(sub_3AA70(&w,(char *)name)==0);
    CHECK(w.file_size==12 && w.file_magic==0xaf12 && w.frame_count==count && w.width==240 && w.height==150);
    sub_3AFB4(&w,&s);CHECK(w.x_offset==40 && w.y_offset==25 && w.surface_offset==8040);
    memset(pixels,0,sizeof pixels);memset(reference,0,sizeof reference);
    memset(movie_video_buffer,0,768);memset(refpal,0,768);memset(chunks_seen,0,sizeof chunks_seen);
    f=fopen(name,"rb");CHECK(f!=NULL);if(!f)return;fseek(f,12,SEEK_SET);
    while(fread(header,1,16,f)==16) {
        CHECK(le16(header+4)==0xf1fa);size=le32(header)-16;CHECK(size<=sizeof scratch_frame);
        if(size>sizeof scratch_frame)break;
        CHECK(fread(scratch_frame,1,size,f)==size);n=le16(header+6);j=0;
        for(i=0;i<n;++i) {
            unsigned len=le32(scratch_frame+j),type=le16(scratch_frame+j+4);
            CHECK(len>=6 && j+len<=size);if(len<6 || j+len>size)break;
            if(type<19)chunks_seen[type]++;
            reference_chunk(type,scratch_frame+j+6,240,150);j+=len;
        }
        /* Decode every frame with the actual file/stream path; pacing covered separately. */
        movie_audio_enabled=0;movie_audio_track=0;w.sound_mode=0;
        CHECK(sub_3AF20(&w,&s,&s.timer)==0);++frames;
        if(memcmp(pixels,reference,sizeof pixels) || memcmp(movie_video_buffer,refpal,768))++mismatches;
        CHECK(w.frame_index==(int)frames);
        if(frames==(unsigned)count)CHECK(port_lseek(w.fd,0,1)==ftell(f));
    }
    CHECK(frames==(unsigned)count+1 && mismatches==0);
    for(i=0;i<19;++i)CHECK(chunks_seen[i]==expected[i]);
    printf("%s: header=%d EOF=%u compared=%u decoder_mismatches=%d chunks 4=%u 7=%u 12=%u 13=%u 15=%u 16=%u 18=%u\n",
           name,count,frames,frames,mismatches,chunks_seen[4],chunks_seen[7],chunks_seen[12],chunks_seen[13],chunks_seen[15],chunks_seen[16],chunks_seen[18]);
    fclose(f);sub_3AA4C(&w);
}
static void files_and_errors(const char *data,const char *save)
{
    FILE *f;int fd;u8 b[128]={0};u32 actual;movie_work_t w;movie_surface_t s;
    uint64_t before=port_clock_ns();
    CHECK(playVideo(".\\","not-present.dat",20)==-5 && port_clock_ns()==before);
    f=fopen("READ.TST","wb");CHECK(f!=NULL);if(f){fwrite("abc",1,3,f);fclose(f);}
    fd=port_open("READ.TST",0x200);CHECK(fd>=0);
    actual=99;CHECK(_dos_read(fd,b,100,&actual)==0 && actual==3 && !memcmp(b,"abc",3));
    CHECK(_dos_read(fd,b,100,&actual)==0 && actual==0);
    actual=19;CHECK(_dos_read(-1,b,10,&actual)==6 && actual==19);
    CHECK(sub_3A55C(-1,b,10)==-6);port_close(fd);
    /* Synthetic headers are written only in the test save directory. */
    put32(b,12);put16(b+4,0xaf12);put16(b+6,1);put16(b+8,321);put16(b+10,150);
    f=fopen("HEADER.TST","wb");fwrite(b,1,128,f);fclose(f);CHECK(sub_3AA70(&w,"HEADER.TST")==-3);
    put16(b+8,240);put16(b+4,0x1234);
    f=fopen("HEADER.TST","wb");fwrite(b,1,128,f);fclose(f);CHECK(sub_3AA70(&w,"HEADER.TST")==-11);
    put16(b+4,0xaf11);
    f=fopen("HEADER.TST","wb");fwrite(b,1,128,f);fclose(f);CHECK(sub_3AA70(&w,"HEADER.TST")==0);
    CHECK(w.data_offset==12 && w.speed==5*1000/70);sub_3AA4C(&w);
    put16(b+4,0xaf12);
    f=fopen("HEADER.TST","wb");fwrite(b,1,128,f);fclose(f);CHECK(sub_3AA70(&w,"HEADER.TST")==0);
    CHECK(w.speed==30 && w.data_offset==12);CHECK(sub_3AF20(&w,&s,&s.timer)==-4);sub_3AA4C(&w);
    put32(b,128);put16(b+4,0xaf12);put32(b+0x50,128);put32(b+16,1234);
    f=fopen("HEADER.TST","wb");fwrite(b,1,128,f);fclose(f);CHECK(sub_3AA70(&w,"HEADER.TST")==0);
    CHECK(w.data_offset==128 && w.speed==1234 && port_lseek(w.fd,0,1)==128);sub_3AA4C(&w);
    put16(b+4,0xaf11);put32(b+16,5);
    f=fopen("HEADER.TST","wb");fwrite(b,1,128,f);fclose(f);CHECK(sub_3AA70(&w,"HEADER.TST")==0);
    CHECK(w.data_offset==128 && w.speed==5*1000/70 && port_lseek(w.fd,0,1)==128);sub_3AA4C(&w);
    TheProcesseur=3;CHECK(sub_3A5B4(&s)==-13);TheProcesseur=5;
    /* Search roots: save first, then the player's roots in order; no virtual-CD lookup. */
    CHECK(port_files_init(save,save));CHECK(port_files_add_data(data));
    { char path[1024];CHECK(port_files_resolve("intro.dat",0,path,sizeof path));CHECK(strstr(path,"INTRO.DAT") || strstr(path,"intro.dat")); }
    {
        char a[1024],bdir[1024],overlay[1024];int c;
        snprintf(a,sizeof a,"%s/root-a",save);snprintf(bdir,sizeof bdir,"%s/root-b",save);
        snprintf(overlay,sizeof overlay,"%s/overlay",save);
        CHECK(port_files_init(data,a));f=fopen("ORDER.TST","wb");fputc('A',f);fclose(f);
        CHECK(port_files_init(data,bdir));f=fopen("ORDER.TST","wb");fputc('B',f);fclose(f);
        f=fopen("ONLY-B.TST","wb");fputc('b',f);fclose(f);
        CHECK(port_files_init(a,overlay));port_unlink("ORDER.TST");CHECK(port_files_add_data(bdir));
        f=fopen("order.tst","rb");CHECK(f!=NULL);if(f){c=fgetc(f);CHECK(c=='A');fclose(f);}
        f=fopen("only-b.tst","rb");CHECK(f!=NULL);if(f){CHECK(fgetc(f)=='b');fclose(f);}
        f=fopen("ORDER.TST","wb");fputc('S',f);fclose(f);
        f=fopen("order.tst","rb");CHECK(f!=NULL);if(f){CHECK(fgetc(f)=='S');fclose(f);}
        CHECK(port_files_init(a,overlay)); /* init clears root-b */
        f=fopen("only-b.tst","rb");CHECK(f==NULL);if(f)fclose(f);
        CHECK(!port_files_add_data("not-a-directory"));
    }
    CHECK(port_files_init(data,save));
}
static void pit_and_fade(void)
{
    u32 base,t;uint64_t start;unsigned lo,hi,after;union REGS r={0};s32 timer;
    CHECK(sub_3A1EC(0x13)==1 && sub_3A214()==0x13);
    outp(0x43,0x34);outp(0x40,0);outp(0x40,0);base=port_pc_bios_ticks();start=port_pc_pit_ticks();
    port_clock_wait_until(((start+1000)*1000000000ULL+PORT_PIT_HZ-1)/PORT_PIT_HZ);
    outp(0x43,0);lo=inp(0x40);hi=inp(0x40);
    CHECK((lo | hi<<8)==65536-1000);
    port_clock_wait_until(((start+66000)*1000000000ULL+PORT_PIT_HZ-1)/PORT_PIT_HZ);
    r.h.ah=0;int386(0x1a,&r,&r);t=(u32)r.w.cx<<16|r.w.dx;CHECK(t==base+1);
    outp(0x43,0x34);outp(0x40,0x00);outp(0x40,0x10);base=port_pc_bios_ticks();start=port_pc_pit_ticks();
    port_clock_wait_until(((start+8192)*1000000000ULL+PORT_PIT_HZ-1)/PORT_PIT_HZ);
    CHECK(port_pc_bios_ticks()==base+2);outp(0x43,0);lo=inp(0x40);
    port_clock_wait_until(port_clock_ns()+1000000);hi=inp(0x40);CHECK((lo|hi<<8)==4096 || (lo|hi<<8)==4095);
    /* Reload restores 65536 without losing whole BIOS ticks. */
    base=port_pc_bios_ticks();outp(0x43,0x36);outp(0x40,0);outp(0x40,0);CHECK(port_pc_bios_ticks()==base);
    sub_3A488((u32 *)&timer);CHECK(timer==4608);
    t=sub_3A4B8(&timer);after=sub_3A518(60,&timer);CHECK(after-t==276 || after-t==277);
    t=sub_3A4B8(&timer);after=sub_3A518(125,&timer);CHECK(after-t==576 || after-t==577);
    sub_3A4A4(&timer);
    for(t=0;t<768;++t)movie_video_buffer[t]=fade_initial[t]=(u8)(t%64);
    fade_steps=fade_syncs=0;in_fade=1;display_movie_frames();in_fade=0;
    CHECK(fade_steps==32 && fade_syncs==32);
    for(t=0;t<768;++t)CHECK(movie_video_buffer[t]==0 && port_vga_dac[t]==0);
}
static void playback_order(void)
{
    movie_work_t w;movie_surface_t s;unsigned i;
    DrawBufferNormal=pixels;MusicCdActive=0;
    for(i=0;i<2;++i) {
        TheProcesseur=i?4:5;
        CHECK(sub_3A5B4(&s)==0 && s.pixels==pixels && s.timer==4608);
        CHECK(sub_3AA70(&w,"conclu.dat")==0);sub_3AFB4(&w,&s);
        input_work=&w;input_reads=input_stop=0;
        CHECK(sub_3B014(&w,&s)==0 && w.frame_index==242 && input_reads==242);
        CHECK(input_indices[0]==0 && input_indices[241]==241); /* input before decode */
        /* Exactly the EOF frame remains unread after one-shot playback. */
        { movie_frame_header_t eof;CHECK(sub_3A55C(w.fd,&eof,16)==0 && eof.magic==0xf1fa); }
        sub_3AA4C(&w);sub_3A634(&s);
    }
    TheProcesseur=5;CHECK(sub_3A5B4(&s)==0);
    CHECK(sub_3AA70(&w,"conclu.dat")==0);sub_3AFB4(&w,&s);
    input_work=&w;input_reads=0;input_stop=243;
    CHECK(sub_3B138(&w,&s)==0 && w.frame_index==2 && input_reads==243);
    CHECK(input_indices[0]==2 && input_indices[241]==243 && input_indices[242]==2);
    /* First decoded once, then header_count frames through EOF, then second frame on the next loop. */
    sub_3AA4C(&w);sub_3A634(&s);input_work=NULL;input_reads=input_stop=0;
}

/* Force the documented CD thresholds with the real device. Deterministic full-speed decoding need not reach
   a threshold within its pause window; these tests cover both branches without changing movie policy. */
void InitMusic(void);
extern dos_sound_config_t sound_config;
extern s32 sound_driver_handle, sound_detect_handle;
static void hmi_suspend_resume(const char *cue)
{
    s32 hw[6]={0},h,slot;unsigned count=0;int token;
    CHECK(sosDIGIInitSystem(0,0,0)==0);
    CHECK(sosDIGIInitDriver(0,10000,hw,&sound_config,&sound_driver_handle)==0 && sound_config.callback);
    CHECK(sosTIMERInitSystem(0xff00,0)==0);
    CHECK(sosTIMERRegisterEvent(0x28,sound_config.callback,&sound_detect_handle)==0);
    h=sound_detect_handle;CarteSonAutorisee=1;
    sub_3B260();sub_3B274();CHECK(sound_detect_handle==h);
    CHECK(port_cd_init(cue,'D'));MusicCdActive=1;InitMusic();
    entry_reads=0;
    CHECK(playVideo(".\\","conclu.dat",21)==0 && entry_reads==242);
    CHECK(!playing_intro_video && movie_draw_buffer==NULL && sound_detect_handle==h);
    port_cd_shutdown();MusicCdActive=0;
    /* Resume registered one event again, while BIOS chaining occupies slot 15. */
    while(sosTIMERRegisterEvent(0x28,&token,&slot)==0)++count;
    CHECK(count==14);
    port_hmi_shutdown();CarteSonAutorisee=0;
}
static void cd_rows(const char *cue)
{
    movie_work_t w={0};movie_surface_t s={pixels,320,200,4608};movie_frame_header_t f={0};
    unsigned row;u8 track,m,sec,frac;uint64_t now;
    static const unsigned increments_ms[]={15000,16000,30534,18467};
    CHECK(port_cd_init(cue,'D'));MusicCdActive=1;InitMusic();CHECK(MusicCdActive && AudioStatusByte()==3);
    playing_intro_video=1;w.sound_mode=20;w.width=4;w.height=2;sub_3AFB4(&w,&s);
    sub_1a5b8(AudioStatusByte());sub_1a4ac(AudioStatusByte(),20,0,0);effect_set_audio_status(AudioStatusByte());
    movie_audio_counter=0;movie_audio_state=1;movie_audio_track=1;movie_audio_enabled=0;w.frame_index=1;
    CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_state==0 && movie_audio_counter==0);
    CHECK(sub_1a02c(AudioStatusByte(),&track,&m,&sec,&frac)==0x100 && track==20);
    for(row=0;row<4;++row) {
        now=port_clock_ns();port_clock_wait_until(now+(uint64_t)increments_ms[row]*1000000);
        w.frame_index=movie_audio_limits[row][2];movie_audio_enabled=0;
        CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_counter==(int)row+1 && movie_audio_state==1);
        now=port_cd_position();port_clock_wait_until(port_clock_ns()+100000000);
        CHECK(port_cd_position()==now); /* paused */
        w.frame_index=movie_audio_limits[row+1][1]-1;movie_audio_enabled=0;
        CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_state==1);
        ++w.frame_index;movie_audio_enabled=0;
        CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_state==0);
        now=port_cd_position();port_clock_wait_until(port_clock_ns()+100000000);
        CHECK(port_cd_position()>now); /* resumed */
    }
    /* Row 4's 0xffffff threshold cannot be reached by a valid MSF position. Exact end advances to -1. */
    w.frame_index=1800;movie_audio_enabled=0;
    CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_counter==4 && movie_audio_state==0);
    w.frame_index=1805;movie_audio_enabled=0;
    CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_counter==-1);
    /* Exact end may advance each row without a pause. */
    for(row=0;row<5;++row) {
        movie_audio_counter=(s8)row;movie_audio_state=0;w.frame_index=movie_audio_limits[row][3];movie_audio_enabled=0;
        CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_counter==(row==4?-1:(int)row+1));
    }
    playing_intro_video=0;w.sound_mode=21;
    sub_1a5b8(AudioStatusByte());sub_1a4ac(AudioStatusByte(),21,0,0);effect_set_audio_status(AudioStatusByte());
    movie_audio_counter=0;movie_audio_state=1;movie_audio_enabled=0;w.frame_index=1;
    CHECK(sub_3ABE4(&w,&f,NULL,&s,&s.timer)==0 && movie_audio_state==0);
    CHECK(sub_1a02c(AudioStatusByte(),&track,&m,&sec,&frac)==0x100 && track==21);
    sub_1a5b8(AudioStatusByte());port_cd_shutdown();MusicCdActive=0;
    printf("CD movie rows: initial resume, pauses 0..3, resumes 1..4, exact-end 0..4, -1 sentinel; conclusion track 21 resume\n");
}
int main(int argc,char **argv)
{
    static const unsigned intro[19]={[4]=1774,[7]=1408,[15]=1,[16]=365,[18]=1};
    static const unsigned conclu[19]={[4]=243,[7]=40,[15]=1,[16]=201,[18]=1};
    port_host_options opts={1,0,0};
    if(argc!=4)return 2;
    port_clock_init(PORT_CLOCK_DETERMINISTIC);CHECK(port_host_init(&opts));
    Read_Input_Func=no_input;input_mode=0;
    CHECK(port_files_init(argv[1],argv[2]));TheProcesseur=5;WindowsLance=0;MusicCdActive=0;
    EffetBufferNormal=malloc(sizeof scratch_frame);CHECK(EffetBufferNormal!=NULL);
    synthetic_chunks();corpus("intro.dat",1804,intro);corpus("conclu.dat",242,conclu);
    files_and_errors(argv[1],argv[2]);pit_and_fade();playback_order();cd_rows(argv[3]);hmi_suspend_resume(argv[3]);
    free(EffetBufferNormal);port_host_shutdown();
    printf("movie: %u checks, %u failures\n",checks,failures);return failures!=0;
}
