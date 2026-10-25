/* SDL dummy devices exercise the real stream path without a physical sound card. */
#include "platform/cdrom.h"
#include "platform/cd_audio.h"
#include "platform/clock.h"
#include "platform/host.h"
#include "platform/hmi.h"
#include "platform/audio.h"
#include <SDL3/SDL.h>
#include <stdio.h>

extern unsigned char MusicCdActive;
void InitMusic(void), AudioTrackCommand(unsigned char);
unsigned short sub_19c74(unsigned char), sub_19ce4(unsigned char);

#define CHECK(c) do { if (!(c)) { fprintf(stderr,"CD stream FAIL %d: %s (%s)\n",__LINE__,#c,SDL_GetError()); failed=1; goto finish; } } while(0)
int main(int argc, char **argv)
{
    port_host_options options={0,1,0};
    int failed=0, handle=-1;
    dos_sound_config_t cfg={0};
    if(argc!=2)return 2;
    port_clock_init(PORT_CLOCK_REALTIME);
    CHECK(port_host_init(&options));
    CHECK(sosDIGIInitSystem(0,0,0)==0);
    CHECK(sosDIGIInitDriver(0,10000,NULL,&cfg,&handle)==0 && handle==0);
    port_hmi_pump();
    CHECK(port_audio_rate()>0 && port_hmi_audio_queued()>0);
    CHECK(port_cd_init(argv[1],'D'));
    MusicCdActive=1; InitMusic(); CHECK(MusicCdActive==1);
    AudioTrackCommand(2); CHECK(port_cd_audio_queued()>0);
    CHECK(sub_19c74(3)==0x100 && port_cd_audio_queued()==0);
    CHECK(sub_19ce4(3)==0x100 && port_cd_audio_queued()>0);
    port_cd_shutdown();
    CHECK(port_cd_audio_queued()==-1);
    port_hmi_shutdown(); port_audio_shutdown();
    /* Deterministic mode must not create a stream even with a visible host. */
    port_clock_init(PORT_CLOCK_DETERMINISTIC);
    CHECK(sosDIGIInitSystem(0,0,0)==0);
    CHECK(sosDIGIInitDriver(0,10000,NULL,&cfg,&handle)==0);
    CHECK(port_hmi_audio_queued()==-1 && port_audio_rate()==0);
    CHECK(port_cd_init(argv[1],'D'));
    MusicCdActive=1; InitMusic(); AudioTrackCommand(2);
    CHECK(port_cd_audio_queued()==-1);
finish:
    port_hmi_shutdown(); port_cd_shutdown(); port_host_shutdown();
    printf("CD SDL stream: %s\n",failed?"FAIL":"PASS");
    return failed;
}
