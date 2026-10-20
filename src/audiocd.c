/*
 * CD audio control (GOG v1.21 object 1:0cee0-1:0d070, 400 bytes). File name invented (Rayverse audiocd.c).
 * Names: Rayverse/Android; AudioTrackCommand, AudioStatusByte, AudioStopCommand, AudioStatusCheck are INVENTED
 * (callees not yet reconstructed). Status per function: config/units.json.
 * OPEN: the original start_cd_gameover has an extra `mov eax,eax` before `ret` that no tested source form
 * reproduces, so it and everything after it in this object sits 4 bytes early in the candidate layout.
 */
extern int CompteurTrameAudio;
extern unsigned char MusicCdActive;
extern unsigned char JeuCracker;
extern unsigned char CarteSonAutorisee;
extern int cdTime;

extern void AudioTrackCommand(int track);
extern unsigned char AudioStatusByte(void);
extern void AudioStopCommand(unsigned char status);
extern unsigned char AudioStatusCheck(unsigned char status);
extern void PlayTchatchVignette(int id);

unsigned char cd_playing(void);
void start_cd_credits(void);
void start_cd_bbdead(void);

void SetCompteurTrameAudio(void)
{
    CompteurTrameAudio = 0;
}

void DoCdCredits(void)
{
    if (CompteurTrameAudio > 180)
        CompteurTrameAudio = 0;
    if (MusicCdActive && CompteurTrameAudio == 0) {
        if (!cd_playing())
            start_cd_credits();
    }
    CompteurTrameAudio++;
}

void start_cd_credits(void)
{
    if (JeuCracker)
        MusicCdActive = 0;
    if (MusicCdActive)
        AudioTrackCommand(18);
}

void start_cd_ubi_soft(void)
{
    if (JeuCracker)
        MusicCdActive = 0;
    if (MusicCdActive)
        AudioTrackCommand(12);
}

void start_cd_gros_rayman(void)
{
}

void start_cd_perdu(void)
{
    if (CarteSonAutorisee)
        PlayTchatchVignette(8);
    else
        start_cd_gros_rayman();
}

void DoCdRap(void)
{
    if (CompteurTrameAudio > 180)
        CompteurTrameAudio = 0;
    if (MusicCdActive && CompteurTrameAudio == 0) {
        if (!cd_playing())
            start_cd_bbdead();
    }
    CompteurTrameAudio++;
}

void start_cd_bbdead(void)
{
    if (JeuCracker)
        MusicCdActive = 0;
    if (MusicCdActive)
        AudioTrackCommand(19);
}

void start_cd_gameover(void)
{
    if (JeuCracker)
        MusicCdActive = 0;
    if (MusicCdActive)
        AudioTrackCommand(10);
}

void stop_cd(void)
{
    if (MusicCdActive)
        AudioStopCommand(AudioStatusByte());
    cdTime = 0;
    SetCompteurTrameAudio();
}

unsigned char cd_playing(void)
{
    if (MusicCdActive)
        return AudioStatusCheck(AudioStatusByte());
    return 0;
}
