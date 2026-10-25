/* API expectations read from src/hmi: ACTIVE-only allocation, no-stop flag,
   16-bit previous values; PCM/loops/rate from PLATFORM_OPEN item 3. */
#include "platform/hmi.h"
#include "platform/hmi_pcm.h"
#include "platform/clock.h"
#include "platform/host.h"
#include "platform/audio.h"
#include "platform/files.h"
#include "compat/src/pc_hardware.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
extern archive_header_t snd8b_header_infos[7], snd8b_data_infos[7], sndvig_infos[9];
extern bnk_header_t bnkHeaderFixe[128];
extern unsigned char *bnkDataFixe;
int LoadBnkFile(int,int,bnk_header_t *,unsigned char **);
void FreeTchatchVignette(void);
extern unsigned char *ptrTchatch;
extern short sound_voice_id;
extern int sound_driver_handle;
static unsigned checks, failures;
#define CHECK(c) do { checks++; if(!(c)) {fprintf(stderr,"HMI FAIL line %d: %s\n",__LINE__,#c);failures++;} } while(0)
static unsigned char pcm[20000];
static void setup(void)
{
    dos_sound_config_t cfg={0}; int handle=-1;
    port_hmi_shutdown(); port_clock_init(PORT_CLOCK_DETERMINISTIC);
    CHECK(sosDIGIInitSystem(0,0,0)==0);
    CHECK(sosDIGIInitDriver(0,10000,NULL,&cfg,&handle)==0 && handle==0 && cfg.callback!=NULL);
}
static dos_sound_request_t request(unsigned size, unsigned flags)
{
    dos_sound_request_t r={0};r.data=pcm;r.size=size;r.flags=flags;
    r.volume=0x1234;r.pan=0x8000;r.sample_rate=65536;r.field_18[0]=1;
    return r;
}
static void api(void)
{
    int i; dos_sound_request_t r;
    setup(); r=request(sizeof pcm,0x720);
    for(i=0;i<32;i++) CHECK(sosDIGIStartSample(0,&r)==i);
    CHECK(sosDIGIStartSample(0,&r)==-1);
    CHECK(sosDIGISamplesPlaying(0)==32);
    CHECK(sosDIGISampleDone(0,0)==0);
    CHECK(sosDIGISetSampleVolume(0,3,0x7fff)==0x1234);
    CHECK(sosDIGISetSampleVolume(0,3,0x12345678)==0x7fff);
    CHECK(sosDIGISetSampleVolume(0,3,0x8765)==0x5678);
    CHECK(sosDIGISetSampleVolume(0,3,0)==(int16_t)0x8765);
    CHECK(sosDIGISetPanLocation(0,3,0xffff)==-32768);
    CHECK(sosDIGISetPanLocation(0,3,0x12345678)==-1);
    CHECK(sosDIGISetPanLocation(0,3,0)==0x5678);
    CHECK(sosDIGIStopSample(0,3)==0);
    CHECK(sosDIGISampleDone(0,3)==1 && sosDIGISamplesPlaying(0)==31);
    CHECK(sosDIGISetSampleVolume(0,3,12)==0 && sosDIGISetPanLocation(0,3,12)==0);
    CHECK(sosDIGIStartSample(0,&r)==3);
    CHECK(sosDIGIStopSample(0,32)==10 && sosDIGIStopSample(0,-1)==10);
    CHECK(sosDIGISampleDone(0,32)==10 && sosDIGISampleDone(0,-1)==10);
    CHECK(sosDIGIUnInitDriver(0,1,1)==0 && sosDIGIUnInitDriver(0,1,1)==1);
    CHECK(sosDIGISamplesPlaying(0)==0);
    setup();r=request(10,0x1720);CHECK(sosDIGIStartSample(0,&r)==0);
    CHECK(sosDIGIStopSample(0,0)==0 && sosDIGISampleDone(0,0)==0);
    CHECK(port_hmi_flags(0)&0x1000);
    port_clock_wait_until(1000000);
    CHECK(sosDIGISampleDone(0,0)==1); /* no-stop still ends naturally */
    CHECK(sosDIGIDetectInit(0,0)!=0 && sosDIGIDetectFindFirst(NULL,NULL)!=0 && sosDIGIDetectGetSettings(NULL)!=0);
    CHECK(sosDIGIDetectUnInit()==0);
}
/* The reference functions are generated from the original library bodies at build time. */
void hmi_reference_reset(void);
unsigned hmi_reference_flags(unsigned);
int hmi_reference_start(unsigned char *,unsigned,unsigned,unsigned);
int ref_sosDIGIStopSample(unsigned,unsigned);
unsigned ref_sosDIGISampleDone(unsigned,unsigned),ref_sosDIGISamplesPlaying(unsigned);
int ref_sosDIGISetSampleVolume(unsigned,unsigned,int),ref_sosDIGISetPanLocation(unsigned,unsigned,int);
static void reference_api(void)
{
    uint32_t rng=0x3e780;
    unsigned i,n,value,op;
    setup();hmi_reference_reset();
    for(i=0;i<2048;i++) {
        rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;
        n=(rng>>8)&31;value=rng&65535;op=rng%6;
        if(op==0) {
            unsigned flags=0x33330720|((rng&0x10000)?0x1000:0);
            dos_sound_request_t r=request(20000,flags);r.volume=value;r.pan=value;
            CHECK(sosDIGIStartSample(0,&r)==hmi_reference_start(pcm,flags,value,value));
        } else if(op==1) CHECK(sosDIGIStopSample(0,n)==ref_sosDIGIStopSample(0,n));
        else if(op==2) CHECK(sosDIGISetSampleVolume(0,n,value)==ref_sosDIGISetSampleVolume(0,n,value));
        else if(op==3) CHECK(sosDIGISetPanLocation(0,n,value)==ref_sosDIGISetPanLocation(0,n,value));
        else if(op==4) CHECK(sosDIGISampleDone(0,n)==(int)ref_sosDIGISampleDone(0,n));
        else CHECK(sosDIGISamplesPlaying(0)==(int)ref_sosDIGISamplesPlaying(0));
        CHECK(port_hmi_flags(n)==hmi_reference_flags(n));
    }
    CHECK(sosDIGISampleDone(0,32)==(int)ref_sosDIGISampleDone(0,32));
}
static int irq_count, injected;
void _dos_setvect(int,void (*)(void));
void (*_dos_getvect(int))(void);
static void poll_irq(void) { irq_count++; }
static void poll_hook(void)
{
    if (!injected && port_clock_ns()>=50000) {
        port_host_inject_scancode(0x1e); injected=1;
    }
}
static void timing(void)
{
    dos_sound_request_t r; unsigned i; unsigned steps[]={32768,65536,98304,131072,0xffffffffu};
    for(i=0;i<sizeof steps/sizeof steps[0];i++) {
        uint64_t deadline=(100ULL*65536*100000+steps[i]-1)/steps[i];
        setup();r=request(100,0x720);r.sample_rate=(int)steps[i];CHECK(sosDIGIStartSample(0,&r)==0);
        CHECK(port_hmi_frame(0,deadline-1)!=UINT64_MAX);
        CHECK(port_hmi_frame(0,deadline)==UINT64_MAX);
        port_clock_wait_until(deadline);CHECK(!(port_hmi_flags(0)&0x8000));
    }
    setup();r=request(200,0x180);r.sample_rate=123;CHECK(sosDIGIStartSample(0,&r)==0);
    CHECK(port_hmi_frame(0,9999999)==99 && port_hmi_frame(0,10000000)==UINT64_MAX);
    setup();r=request(6928,0x4760);r.field_30=2621;r.field_34=3199;r.field_0c=-1;
    CHECK(sosDIGIStartSample(0,&r)==0);
    CHECK(port_hmi_frame(0,2620ULL*100000)==2620);
    CHECK(port_hmi_frame(0,2621ULL*100000)==2621);
    CHECK(port_hmi_frame(0,5819ULL*100000)==5819);
    CHECK(port_hmi_frame(0,5820ULL*100000)==2621);
    CHECK(port_hmi_frame(0,(2621ULL+3199*1000000ULL)*100000)==2621);
    CHECK(sosDIGIStopSample(0,0)==0 && sosDIGISampleDone(0,0)==1);
    setup();r=request(200,0x180);sound_voice_id=sosDIGIStartSample(0,&r);
    sound_driver_handle=0;ptrTchatch=NULL;
    /* Exercise the actual game's busy wait, bounded by the model's poll cost. */
    {
        void (*previous)(void)=_dos_getvect(9);
        irq_count=injected=0;_dos_setvect(9,poll_irq);port_host_set_pump_hook(poll_hook);
        FreeTchatchVignette();
        port_host_set_pump_hook(NULL);_dos_setvect(9,previous);
        CHECK(irq_count==1 && injected);
    }
    CHECK(port_clock_ns()>=10000000 && port_clock_ns()<=10001000);
    port_hmi_pump(); CHECK(port_audio_rate()==0); /* no device in deterministic/headless mode */
}
static void mixer(void)
{
    dos_sound_request_t r;float out[12];
    CHECK(port_hmi_pcm((const uint8_t *)"\x00",0)==-32768);
    CHECK(port_hmi_pcm((const uint8_t *)"\x80",0)==0);
    CHECK(port_hmi_pcm((const uint8_t *)"\xff",0)==32512);
    CHECK(port_hmi_pcm((const uint8_t *)"\x00\x80",1)==-32768);
    CHECK(port_hmi_pcm((const uint8_t *)"\xff\x7f",1)==32767);
    CHECK(port_hmi_pcm((const uint8_t *)"\xff\xff",1)==-1);
    setup();memset(pcm,255,sizeof pcm);r=request(100,0x720);r.volume=0x7fff;r.pan=0;
    CHECK(sosDIGIStartSample(0,&r)==0);port_hmi_mix(out,6,10000,0);
    CHECK(out[0]==32512.0f/32768 && out[1]==0);
    CHECK(sosDIGISetPanLocation(0,0,0xffff)==0);port_hmi_mix(out,6,10000,0);
    CHECK(out[0]==0 && out[1]==32512.0f/32768);
    sosDIGISetPanLocation(0,0,0x8000);port_hmi_mix(out,6,10000,0);
    CHECK(out[0]==out[1] && out[0]>0.99f);
    sosDIGISetSampleVolume(0,0,0);port_hmi_mix(out,6,10000,0);CHECK(out[0]==0 && out[1]==0);
    setup();r=request(8,0x180);r.volume=0x7fff;
    memcpy(pcm,"\x00\x80\xff\x7f\x00\x00\xff\xff",8);
    CHECK(sosDIGIStartSample(0,&r)==0);port_hmi_mix(out,6,10000,0);
    CHECK(out[0]==-1 && out[1]==-1);
    CHECK(out[2]==32767.0f/32768 && out[2]==out[3]);
    CHECK(out[4]==0 && out[5]==0 && out[6]==-1.0f/32768 && out[6]==out[7]);
    CHECK(out[8]==0 && out[9]==0); /* last speech frame consumed */
}
static void timers(void)
{
    int i,h=-1;setup();CHECK(sosTIMERInitSystem(0xff00,0)==0);
    for(i=0;i<15;i++) {CHECK(sosTIMERRegisterEvent(40,(void *)&checks,&h)==0);CHECK(h==i);}
    CHECK(sosTIMERRegisterEvent(40,(void *)&checks,&h)==11);
    CHECK(sosTIMERRemoveEvent(4)==0 && sosTIMERRegisterEvent(40,(void *)&checks,&h)==0 && h==4);
    CHECK(sosTIMERUnInitSystem(0)==0 && sosDIGIUnInitSystem()==0);
}
static unsigned char *decode(const char *data,const char *name,const archive_header_t *a)
{
    char path[1024]; FILE *f; unsigned i,sum=0;unsigned char *b=malloc(a->size);
    snprintf(path,sizeof path,"%s/%s",data,name);f=fopen(path,"rb");
    CHECK(f!=NULL && b!=NULL);if(!f||!b)exit(2);
    CHECK(fseek(f,a->offset,SEEK_SET)==0 && fread(b,1,a->size,f)==a->size);fclose(f);
    for(i=0;i<a->size;i++) {sum+=b[i];b[i]^=a->xor_byte;}
    CHECK((sum&255)==a->checksum_byte);
    return b;
}
static void assets(const char *data,const char *save)
{
    unsigned bank,j,k,count=0,loops=0;
    CHECK(port_files_init(data,save));
    for(bank=0;bank<7;bank++) {
        unsigned char *h=decode(data,"SNDH8B.DAT",&snd8b_header_infos[bank]);
        unsigned char *d=decode(data,"SNDD8B.DAT",&snd8b_data_infos[bank]);
        CHECK(LoadBnkFile(bank,bank,bnkHeaderFixe,&bnkDataFixe)==0);
        for(j=0;j<128;j++) {
            unsigned off=port_hmi_le32(h+j*16),sz=port_hmi_le32(h+j*16+4);
            unsigned l=port_hmi_le32(h+j*16+8),n=port_hmi_le32(h+j*16+12);
            CHECK(bnkHeaderFixe[j].offset==bnkDataFixe+off && bnkHeaderFixe[j].size==sz);
            CHECK(bnkHeaderFixe[j].field_8==l && bnkHeaderFixe[j].field_C==n);
            if(!sz)continue;
            count++;CHECK((uint64_t)off+sz<=snd8b_data_infos[bank].size && (uint64_t)l+n<=sz);
            CHECK(memcmp(bnkDataFixe+off,d+off,sz)==0);
            for(k=0;k<sz;k++) {
                int16_t expected=(int16_t)((int)d[off+k]*256-32768);
                if(port_hmi_pcm(bnkHeaderFixe[j].offset+k,0)!=expected){CHECK(0);break;}
            }
            if(n) {
                dos_sound_request_t r={0};int v;
                loops++;r.data=d+off;r.size=sz;r.flags=0x4760;r.field_0c=-1;
                r.field_30=l;r.field_34=n;r.sample_rate=65536;
                v=sosDIGIStartSample(0,&r);CHECK(v>=0);
                if(v>=0){uint64_t now=port_clock_ns();CHECK(port_hmi_frame(v,now+(uint64_t)(l+n)*100000)==l);sosDIGIStopSample(0,v);}
            }
        }
        free(h);free(d);
    }
    for(bank=0;bank<9;bank++) {
        unsigned char *b=decode(data,"SNDVIG.DAT",&sndvig_infos[bank]);
        CHECK(sndvig_infos[bank].size%2==0);
        for(k=0;k<sndvig_infos[bank].size;k+=2) {
            int expected=(int)b[k]+256*(int)b[k+1];if(expected>=32768)expected-=65536;
            if(port_hmi_pcm(b+k,1)!=expected){CHECK(0);break;}
        }
        free(b);
    }
    CHECK(count==216 && loops==13);
    printf("PCM: %u shipped effect spans, 9 speech chunks, 23 archive checksums, %u loops\n",count,loops);
}
void LoadTchatchPerdu(void),PlayTchatch(int,int);
void JumelleFreeArray(int **);
int sub_3E780(void);
extern dos_sound_config_t sound_config;
extern unsigned char *TchachPerduPtr;
static void speech_requests(const char *data)
{
    static const unsigned banks[]={8,5,2,0,3,7,1,6,3};
    unsigned sound;
    setup();CHECK(sub_3E780()==0);
    CHECK(sound_config.timeout==10000 && sound_config.callback!=NULL);
    LoadTchatchPerdu();CHECK(TchachPerduPtr!=NULL);
    for(sound=1;sound<=9;sound++) {
        unsigned bank=banks[sound-1];
        unsigned char *decoded=decode(data,"SNDVIG.DAT",&sndvig_infos[bank]);
        uint64_t now, duration=(uint64_t)(sndvig_infos[bank].size/2)*100000;
        PlayTchatch(sound,127);
        CHECK(sound_voice_id>=0 && sound_voice_id<32);
        CHECK((port_hmi_flags(sound_voice_id)&0x180)==0x180); /* actual derivative flag write */
        CHECK(memcmp(bank==6?TchachPerduPtr:ptrTchatch,decoded,sndvig_infos[bank].size)==0);
        now=port_clock_ns();
        CHECK(port_hmi_frame(sound_voice_id,now+duration-1)==sndvig_infos[bank].size/2-1);
        CHECK(port_hmi_frame(sound_voice_id,now+duration)==UINT64_MAX);
        port_clock_wait_until(now+duration);FreeTchatchVignette();
        CHECK(ptrTchatch==NULL && sosDIGISampleDone(0,sound_voice_id)==1);
        free(decoded);
    }
    JumelleFreeArray((int **)&TchachPerduPtr);
    JumelleFreeArray((int **)&bnkDataFixe);
}
int main(int argc,char **argv)
{
    port_host_options host={1,1,0};
    if(argc!=3)return 2;
    port_clock_init(PORT_CLOCK_DETERMINISTIC);CHECK(port_host_init(&host));
    api();reference_api();timing();mixer();timers();setup();assets(argv[1],argv[2]);speech_requests(argv[1]);
    port_clock_init(PORT_CLOCK_REALTIME);port_hmi_pump();
    CHECK(port_audio_rate()==0 && port_hmi_audio_queued()==-1); /* headless real time */
    port_hmi_shutdown();port_host_shutdown();
    printf("HMI: %u checks, %u failures\n",checks,failures);
    return failures?1:0;
}
