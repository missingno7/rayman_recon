/* Device tests plus integration through the historical packet builders/TOC parser.
   The player's image is read-only input. Test files live in the CTest build directory. */
#include "platform/cdrom.h"
#include "dos_regs.h"
#include "platform/clock.h"
#include "platform/files.h"
#include "cd_local.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

extern u8 MusicCdActive, JeuCracker, byte_CFA29, drm_byte_CFA27;
extern u16 word_DE8BC, cd_drive_handle;
extern u32 cd_offset_840;
extern dos_cd_disc_info_t cd_disc_info;
u8 sub_19790(void), AudioStatusByte(void);
u32 sub_19b34(u8,u8,u8), sub_19c0c(u32), sub_19c40(u32), AudioStatusCheck(u8), sub_194f4(u8);
void sub_19b9c(s32,u8*,u8*,u8*), InitMusic(void), check_cd_for_save(void);
u16 sub_19c74(u8), sub_19cc0(u8), sub_19ce4(u8), sub_19d30(u8,u8,u32,u32);
u16 sub_19d88(u8), sub_19df0(u8), sub_19e58(u8), sub_19eb4(u8);
u16 sub_19f10(u8,u8*,u8*,u32*), sub_19fa0(u8,u8,u32*,u8*);
u16 sub_1a02c(u8,u8*,u8*,u8*,u8*), sub_1a0e4(u8,u32*);
u16 sub_1a158(u8,u8*,u8*,u8*,u8*), sub_1a1e8(u8,u8,u8,u8,u8);
void AudioTrackCommand(u8), AudioStopCommand(u8);
void sub_2748(u32 *);
time_t port_time(time_t *);
FILE *port_fopen(const char*,const char*);
int port_open(const char*,int,...), port_unlink(const char*);

static unsigned checks, failures;
#define CHECK(c) do { checks++; if (!(c)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#c); failures++; } } while(0)
static unsigned rd16(const u8 *p) { return p[0] | p[1]<<8; }
static void wr16(u8 *p,unsigned v) { p[0]=(u8)v;p[1]=(u8)(v>>8); }
static void wr32(u8 *p,u32 v) { wr16(p,v);wr16(p+2,v>>16); }

static void advance(unsigned frames)
{
    port_clock_wait_until(port_clock_ns()+(uint64_t)frames*1000000000/75+1);
}

static int cue_bin_path(const char *cue, char *out, size_t cap)
{
    char line[2048],name[1024],*slash,*back;
    FILE *f=fopen(cue,"r");
    if(!f)return 0;
    while(fgets(line,sizeof line,f)) {
        char *p=line;while(*p==' ')p++;
        if(sscanf(p,"FILE \"%1023[^\"]\"",name)==1) {
            char parent[2048];
            snprintf(parent,sizeof parent,"%s",cue);
            slash=strrchr(parent,'/');back=strrchr(parent,'\\');
            if(!slash || (back && back>slash))slash=back;
            if(slash)slash[1]=0;else parent[0]=0;
            snprintf(out,cap,"%s%s",parent,name);fclose(f);return 1;
        }
    }
    fclose(f);return 0;
}

static void test_transport(void)
{
    dos_regs_t r={0};
    u8 packet[32]={13,0,0xfe};
    u32 address;
    u32 tick, tick2;
    uint64_t now=port_clock_ns();
    time_t epoch=port_time(NULL), again;
    CHECK(epoch!=(time_t)-1 && port_time(&again)==epoch && again==epoch);
    sub_2748(&tick); sub_2748(&tick2);
    CHECK(tick==tick2 && port_clock_ns()==now);
    advance(75); sub_2748(&tick2);
    CHECK(tick2==18 && tick==0);
    CHECK(port_time(NULL)==epoch+1);
    r.ax=0x1500; CHECK(port_cd_interrupt(&r)); CHECK(r.bx==1 && r.cx==3);
    r.ax=0x150c; CHECK(port_cd_interrupt(&r)); CHECK(r.bx==0x0215);
    r.ax=0x150b; r.cx=3; port_cd_interrupt(&r); CHECK(r.ax==1 && r.bx==0xadad);
    r.ax=0x150b; r.cx=2; port_cd_interrupt(&r); CHECK(r.ax==0);
    r.ax=0x1510; r.es=0xffff; r.bx=0xffff; port_cd_interrupt(&r); CHECK((r.flags&1) && r.ax==5);
    port_cd_request(3,packet,sizeof packet); CHECK(rd16(packet+3)==0x8103);
    port_cd_request(2,packet,sizeof packet); CHECK(rd16(packet+3)==0x8101);
    packet[0]=1; port_cd_request(3,packet,sizeof packet); CHECK(rd16(packet+3)==0x8105);
    CHECK(port_cd_allocate(8192,&address)==0x1000 && address==0x10000000);
    CHECK(!port_cd_buffer(8191,2)); CHECK(!port_cd_buffer(0xffffffff,1));
    CHECK(port_cd_allocate(10,&address)==0);
    CHECK(port_cd_free(0)==1); CHECK(port_cd_free(0x1000)==0); CHECK(port_cd_free(0x1000)==1);
}

static void test_synthetic_pcm(void)
{
    FILE *f;
    u8 sector[2352], *r, *b;
    int16_t out[4];
    u32 address;
    unsigned i,j;
    f=fopen("cd-fixture.cue","w"); CHECK(f!=NULL); if(!f)return;
    fputs("FILE \"cd-fixture.bin\" BINARY\n TRACK 01 MODE1/2352\n INDEX 01 00:00:00\n TRACK 02 AUDIO\n INDEX 00 00:00:01\n INDEX 01 00:00:02\n TRACK 03 AUDIO\n INDEX 00 00:00:03\n INDEX 01 00:00:04\n",f);fclose(f);
    f=fopen("cd-fixture.bin","wb"); CHECK(f!=NULL); if(!f)return;
    for(j=0;j<588;j++){ wr16(sector+j*4,12000);wr16(sector+j*4+2,(uint16_t)-10000); }
    for(i=0;i<6;i++)CHECK(fwrite(sector,1,sizeof sector,f)==sizeof sector);
    fclose(f);
    CHECK(port_cd_init("cd-fixture.cue",'D'));
    CHECK(port_cd_pcm(2*588,out,2)==2 && out[0]==12000 && out[1]==-10000);
    CHECK(port_cd_allocate(256,&address)!=0);
    r=port_cd_buffer(0,26);b=port_cd_buffer(64,11);
    r[0]=13;r[2]=12;wr16(r+14,64);wr16(r+16,0x1000);wr16(r+18,9);
    b[0]=3;b[1]=1;b[2]=128;b[3]=0;b[4]=0;b[5]=2;b[6]=255;b[7]=3;b[8]=255;
    port_cd_request(3,r,26);CHECK(rd16(r+3)==0x100);
    CHECK(port_cd_pcm(2*588,out,2)==2 && out[0]==-10000*128/255 && out[1]==0);
    b[1]=4;port_cd_request(3,r,26);CHECK(rd16(r+3)==0x8103);
    CHECK(port_cd_pcm(2*588,out,2)==2 && out[0]==-10000*128/255 && out[1]==0);
    memset(r,0,26);r[0]=13;r[2]=0x84;wr32(r+14,2);wr32(r+18,4);
    port_cd_request(3,r,26);CHECK(rd16(r+3)==0x300);
    advance(1);
    r[2]=3;wr16(r+14,64);wr16(r+16,0x1000);wr16(r+18,11);b[0]=12;
    port_cd_request(3,r,26);
    CHECK(rd16(r+3)==0x300 && b[2]==3 && b[3]==0 && b[4]==0 && b[5]==0 && b[6]==1);
    CHECK(b[8]==0 && b[9]==2 && b[10]==3);
    port_cd_shutdown();
    f=fopen("cd-fixture.cue","a");CHECK(f!=NULL);if(f){fputs("PREGAP 00:02:00\n",f);fclose(f);}
    CHECK(!port_cd_init("cd-fixture.cue",'D'));
    remove("cd-fixture.cue");remove("cd-fixture.bin");
}

static void test_toc(const char *cue)
{
    FILE *f=fopen(cue,"r");
    char line[2048];
    unsigned track=0, idx,m,s,fr, count=0;
    u32 start,leadout,lba,flags32;
    u8 first,last,flags;
    CHECK(f!=NULL);
    if (!f) return;
    MusicCdActive=1;
    InitMusic();
    CHECK(MusicCdActive==1); CHECK(AudioStatusByte()==3);
    CHECK(cd_disc_info.first_track==1 && cd_disc_info.last_track==26 && cd_disc_info.track_count==26);
    CHECK(sub_19f10(3,&first,&last,&leadout)==0x100 && first==1 && last==26);
    CHECK(sub_1a0e4(3,&flags32)==0x100 && !(flags32&0x800) && (flags32&0x110)==0x110);
    while (fgets(line,sizeof line,f)) {
        char *p=line; while (*p==' ') p++;
        if (sscanf(p,"TRACK %u",&track)==1) continue;
        if (sscanf(p,"INDEX %u %u:%u:%u",&idx,&m,&s,&fr)==4 && idx==1) {
            u32 expected=m*4500+s*75+fr;
            CHECK(sub_19fa0(3,(u8)track,&start,&flags)==0x100);
            CHECK(port_cd_lba(start,&lba) && lba==expected);
            CHECK(sub_19c40(start)==expected && cd_disc_info.track_start[track]==start);
            CHECK(flags==(track==1?0x40:0));
            CHECK(port_cd_msf(expected)==start);
            count++;
        }
    }
    fclose(f);
    CHECK(count==26);
    CHECK(port_cd_lba(leadout,&lba));
    /* The supplied fixture BIN's length independently fixes the last extent. */
    {
        char path[2048]; FILE *image;
        CHECK(cue_bin_path(cue,path,sizeof path));
        image=fopen(path,"rb");CHECK(image!=NULL);
        if(image) { CHECK(!fseek(image,0,SEEK_END));CHECK((u32)(ftell(image)/2352)==lba);fclose(image); }
    }
    CHECK(cd_disc_info.track_length[26]==lba-sub_19c40(cd_disc_info.track_start[26]));
    CHECK(sub_19fa0(3,99,&start,&flags)==8);
    for (m=0;m<100;m++) {
        u8 a,b,c;
        u32 n=m*3001+150;
        sub_19b9c((s32)n,&a,&b,&c);
        CHECK(sub_19b34(a,b,c)==n-150);
        CHECK(sub_19c40(sub_19c0c(n))==n-150);
    }
    CHECK(!port_cd_lba(0,&lba)); CHECK(!port_cd_lba(0x026000,&lba));
}

static void test_playback(void)
{
    u32 lba=sub_19c40(cd_disc_info.track_start[12]);
    u8 t,m,s,f,a,b,c,d;
    unsigned i;
    CHECK(sub_19d30(3,1,cd_disc_info.track_start[1],10)==7); /* data */
    CHECK(sub_19d30(3,2,0,1)==8);
    CHECK(sub_19d30(3,0,sub_19c40(cd_disc_info.leadout_lba)-3,10)==8);
    CHECK(sub_19d30(3,0,lba,150)==0x100);
    CHECK(AudioStatusCheck(3)==1);
    for (i=0;i<100;i++) CHECK(port_cd_position()==lba); /* polls never advance deterministic time */
    advance(75); CHECK(port_cd_position()==lba+75);
    CHECK(sub_1a02c(3,&t,&m,&s,&f)==0x100 && t==12 && m==0 && s==1 && f==0);
    CHECK(sub_19c74(3)==0x100); CHECK(AudioStatusCheck(3)==0 && sub_194f4(3)==1);
    advance(75); CHECK(port_cd_position()==lba+75);
    CHECK(sub_19ce4(3)==0x100 && AudioStatusCheck(3)==1);
    advance(75); CHECK(port_cd_position()==lba+150 && AudioStatusCheck(3)==0 && sub_194f4(3)==0);
    AudioTrackCommand(12); CHECK(AudioStatusCheck(3)==1);
    AudioStopCommand(3); CHECK(AudioStatusCheck(3)==0 && sub_194f4(3)==0);
    CHECK(sub_19ce4(3)==2); /* double stop discarded resume */
    CHECK(sub_1a1e8(3,10,20,30,40)==0x100);
    CHECK(sub_1a158(3,&a,&b,&c,&d)==0x100 && a==10 && b==20 && c==30 && d==40);
    CHECK(sub_19df0(3)==0x100 && sub_19e58(3)==2); /* locked tray */
    CHECK(sub_19d88(3)==0x100 && sub_19e58(3)==0x100);
    CHECK(!port_cd_present() && sub_19d30(3,0,lba,1)==2 && sub_19ce4(3)==2);
    CHECK(sub_19eb4(3)==0x100 && port_cd_present());
    CHECK(sub_19cc0(3)==0x100);
}

static void test_packets_and_read(void)
{
    u8 *r,*b;
    u32 address;
    dos_regs_t regs={0};
    CHECK(port_cd_free(cd_drive_handle)==0);
    CHECK(port_cd_allocate(8192,&address)!=0);
    r=port_cd_buffer(0,32); b=port_cd_buffer(128,4096);
    memset(r,0,32); r[0]=13;r[2]=3; wr16(r+16,0x1000);wr16(r+14,128);wr16(r+18,1);b[0]=6;
    port_cd_request(3,r,32);CHECK(rd16(r+3)==0x8105);
    b[0]=0xff;port_cd_request(3,r,32);CHECK(rd16(r+3)==0x8103);
    wr16(r+14,8191);wr16(r+18,5);port_cd_request(3,r,32);CHECK(rd16(r+3)==0x8105);
    wr16(r+14,128);wr16(r+18,1);r[2]=12;b[0]=0xff;port_cd_request(3,r,32);CHECK(rd16(r+3)==0x8103);
    b[0]=3;port_cd_request(3,r,32);CHECK(rd16(r+3)==0x8105);
    memset(r,0,32);r[0]=13;r[2]=0x80;wr16(r+14,128);wr16(r+16,0x1000);wr16(r+18,1);wr32(r+20,16);
    regs.ax=0x1510;regs.cx=3;regs.es=0x1000;port_cd_interrupt(&regs);
    CHECK(rd16(r+3)==0x100 && !memcmp(b+1,"CD001",5));
    for(unsigned k=0;k<3;k++) {
        u8 expected[38];unsigned len=37;
        memcpy(expected,b+702+k*37,37);
        while(len && (expected[len-1]==' ' || expected[len-1]==0))len--;
        expected[len]=0;
        regs.ax=(u16)(0x1502+k);regs.bx=128;regs.cx=3;regs.es=0x1000;
        CHECK(port_cd_interrupt(&regs) && !(regs.flags&1));
        CHECK(!strcmp((char*)b,(char*)expected));
        /* Restore the independent PVD for the next expected identifier. */
        port_cd_request(3,r,32);
    }
    regs.ax=0x15ff;CHECK(port_cd_interrupt(&regs) && (regs.flags&1) && regs.ax==1);
    r[13]=1;wr32(r+20,port_cd_msf(16));r[24]=1;port_cd_request(3,r,32);
    CHECK(rd16(r+3)==0x100 && !memcmp(b+17,"CD001",5));
    wr32(r+20,0);port_cd_request(3,r,32);CHECK(rd16(r+3)==0x8108);
    CHECK(port_cd_free(0x1000)==0);
}

static void test_files(void)
{
    FILE *f;
    CHECK(port_files_init(".","cd-test-save"));
    CHECK(!port_fopen("D:\\probe.tmp","wb") && errno==EROFS);
    CHECK(!port_fopen("d:probe.tmp","r+b") && errno==EROFS);
    CHECK(port_open("D:probe.tmp",0x21,0600)==-1 && errno==EROFS);
    CHECK(port_unlink("D:\\probe.tmp")==-1 && errno==EROFS);
    f=fopen("CDREAD.TMP","wb");CHECK(f!=NULL);if(f){fputs("data",f);fclose(f);}
    f=port_fopen("D:\\CDREAD.TMP","rb");CHECK(f!=NULL);if(f){CHECK(fgetc(f)=='d');fclose(f);}
    byte_CFA29=1;drm_byte_CFA27='D';JeuCracker=0;
    check_cd_for_save(); CHECK(JeuCracker==0);
    remove("CDREAD.TMP");
}

static void test_pcm(const char *cue)
{
    char path[2048];
    FILE *f;
    u8 raw[4]; int16_t out[2];
    u32 lba=sub_19c40(cd_disc_info.track_start[12]);
    CHECK(cue_bin_path(cue,path,sizeof path));
    f=fopen(path,"rb");CHECK(f!=NULL);if(!f)return;
    CHECK(fseek(f,(long)(lba*2352+400),SEEK_SET)==0 && fread(raw,1,4,f)==4);fclose(f);
    sub_1a1e8(3,128,64,255,255);
    CHECK(port_cd_pcm((uint64_t)lba*588+100,out,1)==1);
    CHECK(out[0]==(int16_t)rd16(raw)*128/255 && out[1]==(int16_t)rd16(raw+2)*64/255);
    CHECK(port_cd_pcm(0,out,1)==0); /* data track never becomes noise */
}

int main(int argc,char **argv)
{
    u8 first,last;u32 lead,status;
    if(argc!=2)return 2;
    port_clock_init(PORT_CLOCK_DETERMINISTIC);
    CHECK(port_cd_init(NULL,'D'));
    test_transport();
    test_synthetic_pcm();
    CHECK(port_cd_init(argv[1],'D'));
    test_toc(argv[1]);test_playback();test_pcm(argv[1]);test_files();test_packets_and_read();
    CHECK(port_cd_init(NULL,'D'));
    CHECK(!sub_19790());
    CHECK(sub_1a0e4(3,&status)==0x100 && (status&0x800));
    CHECK(sub_19f10(3,&first,&last,&lead)==2);
    CHECK(sub_19c74(3)==2 && sub_19ce4(3)==2 && sub_19d30(3,0,0,1)==2);
    CHECK(port_cd_init(NULL,'E'));CHECK(port_cd_path("e:probe") && !port_cd_path("D:probe"));
    CHECK(!port_cd_init(NULL,'!'));
    port_cd_shutdown();
    printf("CD tests: %u checks, %u failures\n",checks,failures);
    return failures?1:0;
}
