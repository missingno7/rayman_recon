/*
 * CD audio control (GOG v1.21 object 1:0cee0-1:0d070, 400 bytes). File name invented (Rayverse audiocd.c).
 * Names: Rayverse/Android; AudioTrackCommand, AudioStatusByte, AudioStopCommand, AudioStatusCheck are INVENTED
 * (callees not yet reconstructed). All 12 functions STRICT with worker symbols.json; contiguous _TEXT.
 * start_cd_continue (PS1/Android name) is an empty DOS entry at 1:0d024, called at 1:5a7ea.
 * Watcom shares its aligned return with start_cd_gameover, inserting the two-byte alignment no-op.
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

/* DOS: MAIN_CONTINUE_PRG calls this empty entry at 1:0d024 (call at 1:5a7ea). */
void start_cd_continue(void)
{
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
