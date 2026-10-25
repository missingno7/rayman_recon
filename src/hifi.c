/* hifi.c: split at original object boundary 1:2f040. Names RAYVERSE/ANDROID/PS1. */
#include "obj.h"
#include <stdlib.h>
#include <stdio.h>
#include "hifi_local.h"


extern s32 current_device_id;
extern char current_device_name[100];
extern s32 current_dma;
extern s32 current_irq;
extern s32 current_param;
extern s32 current_port;
extern archive_header_t snd8b_header_infos[7];
extern archive_header_t snd8b_data_infos[7];
extern archive_header_t sndvig_infos[9];
extern bnk_header_t bnkHeaderFixe[128];
extern bnk_header_t bnkHeaderWorld[128];
extern u8 *bnkDataFixe;
extern u8 *bnkDataWorld;
extern u8 *ptrTchatch;
extern u8 *TchachPerduPtr;
extern s32 TchachPerduSize;
extern s16 pitchTab[128];
extern s32 sound_driver_handle;
extern s32 sound_detect_handle;
extern s16 sound_voice_id;
extern dos_sound_config_t sound_config;
extern u8 CarteSonAutorisee;
extern u8 SonLimite;
void stop_cd(void);
void rayman_sound_fatal_error(const char* message);
void SetPort_(u16 port);
void SetIrq(s32 irq);
void SetDma(s32 dma);
void SetParam(s32 param);
void SetDeviceID(s32 device_id);
char* GetDeviceName(void);
s32 *GetPort(void);
s32 *GetIrq(void);
s32 *GetDma(void);
s32 *GetParam(void);
s32 *GetDeviceID_Ray(void);
/* Address-identified routines in the DOS sound and file-driver modules. */
extern s32 sosDIGISamplesPlaying(s32 driver);
extern s32 sosTIMERInitSystem(s32 a1, s32 a2);
extern s32 sosTIMERRegisterEvent(s32 a1, void __far *callback, void __far *a5);
extern s32 sosTIMERUnInitSystem(s32 a1);
extern s32 sosTIMERRemoveEvent(s32 handle);
extern s32 sosDIGIDetectInit(s32 a1, s16 a2);
extern s32 sosDIGIDetectFindFirst(const char __far *name, void __far *buffer);
extern s32 sosDIGIDetectGetSettings(s32 __far *value);
extern s32 sosDIGIDetectUnInit(void);
extern s32 sosDIGIInitSystem(s32 a1, s32 a2, s32 a3);
extern s32 sosDIGIInitDriver(s32 device_id, s32 timeout, s32 __far *port,
    void __far *init_data, void __far *driver_data);
extern s32 sosDIGIUnInitSystem(void);
extern s32 sosDIGIUnInitDriver(s32 driver, s32 enabled, s32 flags);
extern s32 sosDIGIStartSample(s32 driver, dos_sound_request_t __far *request);
extern s32 sosDIGIStopSample(s32 driver, s16 voice);
extern s32 sosDIGISetSampleVolume(s32 driver, s16 voice, s32 volume);
extern s32 sosDIGISetPanLocation(s32 driver, s16 voice, s32 pan);
extern s32 sosDIGISampleDone(s32 driver, s16 voice);
extern void *sub_6FBB4(s32 size);
extern void JumelleFreeArray(s32 **address);
void sub_3E66C(void);
void sub_3E6B0(void);
s32 sub_3E710(void);
s32 sub_3E780(void);
s32 LoadBnkFile(s32 header_index, s32 data_index,
    bnk_header_t *headers, u8 **data);
void LoadBnkFixe(void);
s32 LoadBnkWorld(s16 world);
s16 KeyOn(s32 bank, s32 prog, u8 tone, s16 note, s16 volume, s16 pan);
void KeyOff(s16 voice, u8 bank, u8 prog, u8 tone, u8 note);
void KeyVol(s16 voice, s16 volume, s16 pan);
void DigiMusicDone(void);
void LoadTchatchPerdu(void);
void PlayTchatch(s32 sound, s32 volume);
void FreeTchatchVignette(void);
void FreeDetectDriver(void);

void sub_3E66C(void)
{
    s16 voice;

    for (voice = 0; voice <= 32; voice++)
        KeyOff(voice, 0, 0, 0, 0);
    while (sosDIGISamplesPlaying(sound_driver_handle) != 0)
        ;
    sosTIMERRemoveEvent(sound_detect_handle);
    sosTIMERUnInitSystem(0);
}

void sub_3E6B0(void)
{
    sosTIMERInitSystem(0xff00, 0);
    if (sosTIMERRegisterEvent(0x28, sound_config.callback,
            (void __far *)&sound_detect_handle) != 0)
        sosTIMERUnInitSystem(0);
}

/* INVENTED helper name. ORIGINAL_INLINE: 1:2e6f4, BIOS video service. EAX is its input/output;
 * the DOS caller preserves the message in EDX across this exact INT 10h. */
void sound_set_video_mode(s32 mode);
#pragma aux sound_set_video_mode = "int 10h" parm [eax] modify [eax];

void rayman_sound_fatal_error(const char* message) {
    sound_set_video_mode(3);
    printf("Rayman sound says fatal error :\n%s\n", message);
    exit(-1);
}

/* probe -> detected_handle. See docs/semantics/hifi.json. */
s32 sub_3E710(void)
{
    s32 detected_handle;

    if (sosDIGIDetectInit(0, 0) != 0)
        return 1;
    if (sosDIGIDetectFindFirst((const char __far *)current_device_name,
            (void __far *)&detected_handle) != 0) {
        sosDIGIDetectUnInit();
        return 2;
    }
    if (sosDIGIDetectGetSettings((s32 __far *)&current_port) != 0) {
        sosDIGIDetectUnInit();
        return 3;
    }
    sosDIGIDetectUnInit();
    return 0;
}

s32 sub_3E780(void)
{
    s32 result;

    sosDIGIInitSystem(0, 0, 0);
    sosTIMERInitSystem(0xff00, 0);
    sound_config.timeout = 10000;
    result = sosDIGIInitDriver(current_device_id, sound_config.timeout,
        (s32 __far *)&current_port,
        (void __far *)&sound_config,
        (void __far *)&sound_driver_handle);
    if (result != 0) {
        sosTIMERUnInitSystem(0);
        sosDIGIUnInitSystem();
        return 1;
    }
    result = sosTIMERRegisterEvent(0x28, sound_config.callback,
        (void __far *)&sound_detect_handle);
    if (result != 0) {
        sosTIMERUnInitSystem(0);
        sosDIGIUnInitDriver(sound_driver_handle, 1, 1);
        return 2;
    }
    return 0;
}

s32 LoadBnkFile(s32 header_index, s32 data_index,
    bnk_header_t *headers, u8 **data)
{
    FILE *header_file;
    FILE *data_file;
    u32 size, i;
    u8 check;
    u8 *end, *bytes;
    u8 *sample;
    u8 *new_data;

    stop_cd();
    header_file = fopen("SNDH8B.DAT", "rb");
    if (header_file == 0)
        rayman_sound_fatal_error("Fichier header introuvable (SNDH8B.DAT).\n");
    fseek(header_file, snd8b_header_infos[header_index].offset, SEEK_SET);
    fread(headers, 0x10, 0x80, header_file);
    fclose(header_file);
    bytes = (u8 *)headers;
    check = snd8b_header_infos[header_index].checksum_byte;
    end = bytes + 0x800;
    do {
        check -= *bytes;
        *bytes ^= snd8b_header_infos[header_index].xor_byte;
        bytes++;
    } while (bytes != end);
    if (check != 0)
        rayman_sound_fatal_error("Load error in sndh8b.dat.\n");
    stop_cd();
    data_file = fopen("SNDD8B.DAT", "rb");
    if (data_file == 0)
        rayman_sound_fatal_error("Fichier data introuvable.\n");
    size = snd8b_data_infos[data_index].size;
    if (*data != 0)
        JumelleFreeArray((s32 **)data);
    new_data = sub_6FBB4(size);
    if (new_data == 0)
        rayman_sound_fatal_error("M\x82moire insuffisante pour charger les samples.\n");
    *data = new_data;
    fseek(data_file, snd8b_data_infos[data_index].offset, SEEK_SET);
    fread(new_data, 1, size, data_file);
    fclose(data_file);
    check = snd8b_data_infos[data_index].checksum_byte;
    i = 0;
    if (size > 0) {
        sample = new_data;
        do {
            check -= *sample;
            *sample ^= snd8b_data_infos[data_index].xor_byte;
            sample++;
            i++;
        } while (i < size);
    }
    if (check != 0)
        rayman_sound_fatal_error("Load error in sndd8b.dat.\n");
    bytes = (u8 *)headers;
    end = bytes + 0x800;
    do {
        *(u32 *)bytes += (u32)new_data;
        bytes += 0x10;
    } while (bytes != end);
    return 0;
}

void LoadBnkFixe(void)
{
    if (CarteSonAutorisee && !SonLimite)
        LoadTchatchPerdu();
    LoadBnkFile(2, 2, bnkHeaderFixe, &bnkDataFixe);
}

s32 LoadBnkWorld(s16 world)
{
    switch (world) {
    case 1: return LoadBnkFile(4, 4, bnkHeaderWorld, &bnkDataWorld);
    case 2: return LoadBnkFile(6, 6, bnkHeaderWorld, &bnkDataWorld);
    case 3: return LoadBnkFile(5, 5, bnkHeaderWorld, &bnkDataWorld);
    case 4: return LoadBnkFile(3, 3, bnkHeaderWorld, &bnkDataWorld);
    case 5: return LoadBnkFile(1, 1, bnkHeaderWorld, &bnkDataWorld);
    case 6: return LoadBnkFile(0, 0, bnkHeaderWorld, &bnkDataWorld);
    default: return 0;
    }
}

s16 KeyOn(s32 bank, s32 prog, u8 tone, s16 note, s16 volume, s16 pan)
{
    /* DOS initializer 1:2e580: field_10=2, volume=0x7fff, field_18[0]=1. */
    dos_sound_request_t request = {0, 0, 0, 0, 2, 0x7fff, {1}};
    s16 voice;

    request.volume = ((s32)volume * 0x7fff) / 0x7f;
    request.flags |= 0x720;
    request.pan = ((s32)pan * 0xffff) / 0x7f;
    request.sample_rate = ((s32)pitchTab[note]) << 8;

    if ((s16)bank == 0) {
        request.data = (u8 __far *)bnkHeaderFixe[(s16)prog].offset;
        request.size = bnkHeaderFixe[(s16)prog].size;
        if (bnkHeaderFixe[(s16)prog].field_C != 0) {
            request.flags |= 0x4760;
            request.field_30 = bnkHeaderFixe[(s16)prog].field_8;
            request.field_34 = bnkHeaderFixe[(s16)prog].field_C;
            request.field_0c = -1;
        }
    } else {
        request.data = (u8 __far *)bnkHeaderWorld[(s16)prog].offset;
        request.size = bnkHeaderWorld[(s16)prog].size;
        if (bnkHeaderWorld[(s16)prog].field_C != 0) {
            request.flags |= 0x4760;
            request.field_30 = bnkHeaderWorld[(s16)prog].field_8;
            request.field_34 = bnkHeaderWorld[(s16)prog].field_C;
            request.field_0c = -1;
        }
    }
    if (request.size != 0) {
        voice = sosDIGIStartSample(sound_driver_handle,
            (dos_sound_request_t __far *)&request);
        if (voice != -1 && voice < 0x20)
            return voice;
    }
    return -1;
}

void KeyOff(s16 voice, u8 bank, u8 prog, u8 tone, u8 note)
{
    sosDIGIStopSample(sound_driver_handle, voice);
}

void KeyVol(s16 voice, s16 volume, s16 pan)
{
    s32 scaled_volume;
    s32 scaled_pan;

    scaled_volume = ((s32)volume * 0x7fff) / 0x7f;
    sosDIGISetSampleVolume(sound_driver_handle, voice, scaled_volume);
    scaled_pan = ((s32)pan * 0xffff) / 0x7f;
    sosDIGISetPanLocation(sound_driver_handle, voice, scaled_pan);
}

void DigiMusicDone(void)
{
    s16 voice;

    for (voice = 0; voice <= 32; voice++)
        KeyOff(voice, 0, 0, 0, 0);
    sosTIMERRemoveEvent(sound_detect_handle);
    sosTIMERUnInitSystem(0);
    sosDIGIUnInitDriver(sound_driver_handle, 1, 1);
    sosDIGIUnInitSystem();
}

void LoadTchatchPerdu(void)
{
    FILE *file;
    u32 size;
    u32 saved_size; /* COMPILER_STEERED: retain allocation size for the final result. */
    u32 i;
    u8 check;
    s32 bank = 6;

    stop_cd();
    file = fopen("sndvig.dat", "rb");
    if (file == 0)
        rayman_sound_fatal_error("Fichier Tchach introuvable.\n");
    size = sndvig_infos[bank].size;
    saved_size = size;
    ptrTchatch = sub_6FBB4(size);
    if (ptrTchatch == 0) {
        fclose(file);
        return;
    }
    fseek(file, sndvig_infos[bank].offset, SEEK_SET);
    fread(ptrTchatch, 1, size, file);
    fclose(file);
    check = sndvig_infos[bank].checksum_byte;
    for (i = 0; i < size; i++) {
        check -= ptrTchatch[i];
        ptrTchatch[i] ^= sndvig_infos[bank].xor_byte;
    }
    if (check != 0)
        rayman_sound_fatal_error("Load error in sndvig.dat.\n");
    TchachPerduPtr = ptrTchatch;
    TchachPerduSize = saved_size;
    ptrTchatch = 0;
}

void PlayTchatch(s32 sound, s32 volume)
{
    /* DOS initializer 1:2e5f8: field_10=2, volume=0x7fff, field_18[0]=2. */
    dos_sound_request_t request = {0, 0, 0, 0, 2, 0x7fff, {2}};
    s32 bank;
    FILE *file;
    u32 size;
    u32 saved_size; /* COMPILER_STEERED: retain allocation size for the final result. */
    u32 i;
    u8 check;
    s16 voice;

    request.volume = (volume * 0x7fff) / 0x7f;
    ((u8 *)&request)[0x29] |= 1;
    switch (sound) {
    case 1: bank = 8; break;
    case 6: bank = 7; break;
    case 2: bank = 5; break;
    case 7: bank = 1; break;
    case 4: bank = 0; break;
    case 3: bank = 2; break;
    case 8: bank = 6; break;
    case 5:
    case 9: bank = 3; break;
    }
    if (bank != 6) {
        stop_cd();
        file = fopen("sndvig.dat", "rb");
        if (file == 0)
            rayman_sound_fatal_error("Fichier Tchach introuvable.\n");
        size = sndvig_infos[bank].size;
        saved_size = size;
        if (ptrTchatch != 0)
            JumelleFreeArray((s32 **)&ptrTchatch);
        ptrTchatch = sub_6FBB4(size);
        if (ptrTchatch == 0) {
            fclose(file);
            return;
        }
        fseek(file, sndvig_infos[bank].offset, SEEK_SET);
        fread(ptrTchatch, 1, size, file);
        fclose(file);
        check = sndvig_infos[bank].checksum_byte;
        for (i = 0; i < size; i++) {
            check -= ptrTchatch[i];
            ptrTchatch[i] ^= sndvig_infos[bank].xor_byte;
        }
        if (check != 0)
            rayman_sound_fatal_error("Load error in sndvig.dat.\n");
        request.flags |= 0x80;
        request.data = (u8 __far *)ptrTchatch;
        request.size = saved_size;
    } else {
        request.flags |= 0x80;
        request.data = (u8 __far *)TchachPerduPtr;
        request.size = TchachPerduSize;
    }
    voice = sosDIGIStartSample(sound_driver_handle,
        (dos_sound_request_t __far *)&request);
    sound_voice_id = voice;
}

void FreeTchatchVignette(void)
{
    while (sosDIGISampleDone(sound_driver_handle, sound_voice_id) != 1)
        ;
    JumelleFreeArray((s32 **)&ptrTchatch);
    ptrTchatch = 0;
}

void SetPort_(u16 port) {
    current_port = port;
}

void SetIrq(s32 irq) {
    current_irq = irq;
}

void SetDma(s32 dma) {
    current_dma = dma;
}

void SetParam(s32 param) {
    current_param = param;
}

void SetDeviceID(s32 device_id) {
    current_device_id = device_id;
}

char* GetDeviceName(void) {
    return current_device_name;
}

s32 *GetPort(void) {
    return &current_port;
}

s32 *GetIrq(void) {
    return &current_irq;
}

s32 *GetDma(void) {
    return &current_dma;
}

s32 *GetParam(void) {
    return &current_param;
}

s32 *GetDeviceID_Ray(void) {
    return &current_device_id;
}

void FreeDetectDriver(void)
{
    sosDIGIDetectUnInit();
}

