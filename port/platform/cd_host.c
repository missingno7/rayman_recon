/* Host CD drive backend (platform/cd_host.h). */
#include "platform/cd_host.h"
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>

/* ntddcdrm.h values, spelled out so the build does not depend on the DDK headers. */
#define CD_IOCTL_READ_TOC 0x00024000u
#define CD_IOCTL_RAW_READ 0x0002403Eu
#define CD_TRACK_MODE_CDDA 2
#define CACHE_SECTORS 20

typedef struct { UCHAR reserved, control_adr, number, reserved1, address[4]; } toc_track;
typedef struct { UCHAR length[2], first, last; toc_track track[100]; } toc_t;
typedef struct { LARGE_INTEGER offset; ULONG sectors; ULONG mode; } raw_read_t;

static HANDLE device = INVALID_HANDLE_VALUE;
static uint8_t cache[CACHE_SECTORS * 2352];
static uint32_t cache_lba, cache_count;

char port_cd_host_find(void)
{
    char root[4] = "A:\\", label[64];
    UINT mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    char found = 0;
    for (; root[0] <= 'Z' && !found; root[0]++)
        if (GetDriveTypeA(root) == DRIVE_CDROM &&
            GetVolumeInformationA(root, label, sizeof label, NULL, NULL, NULL, NULL, 0) && !_stricmp(label, "RAYMAN"))
            found = root[0];
    SetErrorMode(mode);
    return found;
}

void port_cd_host_close(void)
{
    if (device != INVALID_HANDLE_VALUE) CloseHandle(device);
    device = INVALID_HANDLE_VALUE;
    cache_count = 0;
}

int port_cd_host_is_open(void) { return device != INVALID_HANDLE_VALUE; }

int port_cd_host_open(char letter, port_cd_host_track *tracks, unsigned *first, unsigned *last, uint32_t *leadout)
{
    char name[8] = "\\\\.\\A:";
    toc_t toc;
    DWORD got;
    unsigned i, count;
    port_cd_host_close();
    name[4] = letter;
    device = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (device == INVALID_HANDLE_VALUE) return 0;
    memset(&toc, 0, sizeof toc);
    if (!DeviceIoControl(device, CD_IOCTL_READ_TOC, NULL, 0, &toc, sizeof toc, &got, NULL) ||
        toc.first < 1 || toc.last > 99 || toc.last < toc.first) goto bad;
    count = toc.last - toc.first + 1u;
    for (i = 0; i <= count; i++) {   /* the entry after the last track is the lead-out */
        const toc_track *t = &toc.track[i];
        uint32_t frames = t->address[1] * 4500u + t->address[2] * 75u + t->address[3];
        if (frames < 150) goto bad;
        if (i == count) *leadout = frames - 150;
        else {
            tracks[toc.first + i].start = frames - 150;
            tracks[toc.first + i].data = (t->control_adr & 4) ? 0x40 : 0;
        }
    }
    *first = toc.first;
    *last = toc.last;
    return 1;
bad:
    port_cd_host_close();
    return 0;
}

int port_cd_host_read(uint32_t lba, int raw, int data_track, uint8_t *out)
{
    DWORD got;
    if (device == INVALID_HANDLE_VALUE) return 0;
    if (data_track) {
        LARGE_INTEGER at;
        uint8_t user[2048];
        uint32_t f = lba + 150;
        at.QuadPart = (LONGLONG)lba * 2048;
        if (!SetFilePointerEx(device, at, NULL, FILE_BEGIN) || !ReadFile(device, user, 2048, &got, NULL) || got != 2048)
            return 0;
        if (!raw) { memcpy(out, user, 2048); return 1; }
        memset(out, 0, 2352);
        memset(out + 1, 0xFF, 10);
        out[12] = (uint8_t)((f / 4500 / 10 << 4) | f / 4500 % 10);
        out[13] = (uint8_t)((f / 75 % 60 / 10 << 4) | f / 75 % 60 % 10);
        out[14] = (uint8_t)((f % 75 / 10 << 4) | f % 75 % 10);
        out[15] = 1;
        memcpy(out + 16, user, 2048);
        return 1;
    }
    if (!raw) return 0;
    if (lba < cache_lba || lba >= cache_lba + cache_count) {
        raw_read_t request;
        ULONG n = CACHE_SECTORS;
        cache_count = 0;
        /* Read ahead; near the end of the disc (or a track the drive refuses to cross) fall back to one sector. */
        for (;;) {
            request.offset.QuadPart = (LONGLONG)lba * 2048;
            request.sectors = n;
            request.mode = CD_TRACK_MODE_CDDA;
            if (DeviceIoControl(device, CD_IOCTL_RAW_READ, &request, sizeof request, cache, n * 2352, &got, NULL) &&
                got == n * 2352) break;
            if (n == 1) return 0;
            n = 1;
        }
        cache_lba = lba;
        cache_count = n;
    }
    memcpy(out, cache + (size_t)(lba - cache_lba) * 2352, 2352);
    return 1;
}

#else

char port_cd_host_find(void) { return 0; }
void port_cd_host_close(void) {}
int port_cd_host_is_open(void) { return 0; }
int port_cd_host_open(char letter, port_cd_host_track *tracks, unsigned *first, unsigned *last, uint32_t *leadout)
{
    (void)letter; (void)tracks; (void)first; (void)last; (void)leadout;
    return 0;
}
int port_cd_host_read(uint32_t lba, int raw, int data_track, uint8_t *out)
{
    (void)lba; (void)raw; (void)data_track; (void)out;
    return 0;
}

#endif
