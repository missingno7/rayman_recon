/* DOS v1.21 frame/FLC prefix, 1:2a1c0..1:2b287.
 * Address-derived function names are retained until a semantic name is proven.
 */
#include "obj.h"
#include "game.h"
#include "rtypes.h"
#include "inline_helpers.h"
#include <conio.h>
#include <i86.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>

typedef struct movie_surface_t {
    u8 *pixels;
    s32 width;
    s32 height;
} movie_surface_t;

typedef struct movie_work_t {
    u32 file_size;
    u16 file_magic;
    u16 frame_count;
    u16 width;
    u16 height;
    u16 depth;
    u16 flags;
    u32 speed;
    u8 reserved14[0x3c];
    u32 data_offset;
    u32 current_offset;
    u8 reserved58[0x28];
    s32 fd;
    s32 frame_index;
    char *path;
    s32 x_offset;
    s32 y_offset;
    u32 surface_offset;
    u8 sound_mode;
} movie_work_t;

typedef struct movie_frame_header_t {
    u32 size;
    u16 magic;
    u16 chunks;
    u8 reserved[8];
} movie_frame_header_t;

typedef void (*movie_palette_writer_t)(u32, s32, const u8 *, s32);

extern u8 *DrawBufferNormal;
extern u8 *DisplayBufferNormal;
extern u8 *EffetBufferNormal;
extern u8 movie_video_buffer[768];
extern u8 VGA_FREQ;
extern u8 WindowsLance;
extern u8 TheProcesseur;
extern u8 MusicCdActive;
extern u8 CarteSonAutorisee;
extern u8 playing_intro_video;
extern u8 movie_playback_state;
extern u8 movie_saved_mode;
extern u8 movie_saved_frequency;
extern u8 movie_palette_flag;
extern s32 movie_palette_xmin;
extern s32 movie_palette_xmax;
extern u8 *movie_draw_buffer;
extern s8 movie_audio_track;
extern u8 movie_audio_enabled;
extern u8 movie_audio_state;
extern u8 movie_audio_counter;
extern s32 movie_audio_limits[5][4];
extern s32 movie_next_tick;

extern void set_vga_frequency(u8 frequency);
extern u8 AudioStatusByte(void);
extern void sub_1a5b8(s32 state);
extern u16 sub_1a02c(u8 command, u8 *track, u8 *minute, u8 *second, u8 *frame);
extern void sub_1a4ac(u8 command, u8 track, u8 minute, u8 second);
extern void effect_set_audio_status(u8 status);
extern void effect_restore_audio_status(u8 status);
extern void endsynchro(void);
extern void synchro(void);
extern void readinput(void);
extern s16 but0pressed(void);
extern s16 but1pressed(void);
extern s16 but2pressed(void);
extern s16 but3pressed(void);
extern u8 TOUCHE(u8 key);
extern void sub_3E66C(void);
extern void sub_3E6B0(void);

extern s32 open(const char *name, s32 flags, ...);
extern s32 close(s32 fd);
extern s32 _dos_read(s32 fd, void *buffer, u32 count, u32 *actual);
extern long lseek(int fd, long offset, int origin);

s32 sub_3A1C0(void);
void sub_3A1E0(void);
s32 sub_3A1EC(u8 mode);
u8 sub_3A214(void);
s32 sub_3A238(u8 *surface);
void sub_3A2AC(void);
s32 sub_3A2C4(movie_surface_t *surface);
s32 sub_3A2C8(movie_surface_t *surface);
void sub_3A2CC(movie_surface_t *surface, s32 x, s32 y, u8 color);
void sub_3A2D8(movie_surface_t *surface, s32 x, s32 y, const u8 *src, u32 width);
void sub_3A30C(movie_surface_t *surface, s32 x, s32 y, u8 color, u32 width);
void sub_3A34C(movie_surface_t *surface, s32 x, s32 y, u16 color, u32 width);
void sub_3A3B8(u32 context, s32 x, const u8 *rgb, s32 count);
void sub_3A428(u32 context, s32 x, const u8 *rgb, s32 count);
s32 sub_3A488(u32 *timer);
void sub_3A4A4(s32 *timer);
s32 sub_3A4B8(s32 *timer);
s32 sub_3A518(s32 value, s32 *scale);
s32 sub_3A538(s32 *fd, const char *path);
s32 sub_3A55C(s32 fd, void *buffer, u32 count);
s32 sub_3A578(s32 fd, u8 **buffer, u32 count);
s32 sub_3A5B4(u8 *info);
void sub_3A634(u8 *info);
void sub_3A664(const u8 *data, movie_surface_t *surface, s32 x,
               movie_palette_writer_t writer);
void sub_3A6D0(const u8 *data, movie_surface_t *surface, s32 x);
void sub_3A6E0(const u8 *data, movie_surface_t *surface, s32 x);
void sub_3A6F0(const u8 *data, movie_work_t *work, movie_surface_t *surface, s8 x_bias);
void sub_3A7B4(const u8 *data, movie_work_t *work, movie_surface_t *surface, s8 x_bias);
void sub_3A8A0(const u8 *data, movie_work_t *work, movie_surface_t *surface, s8 x_bias);
void sub_3A9F4(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3AA08(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3AA4C(u8 *work);
s32 sub_3AA70(u8 *work, char *path);
s32 sub_3ABE4(movie_work_t *work, movie_frame_header_t *frame,
              const u8 *chunks, movie_surface_t *surface, s32 *timer);
s32 sub_3AF20(movie_work_t *work, movie_surface_t *surface, s32 *timer);
void sub_3AFB4(movie_work_t *work, movie_surface_t *surface);
s32 sub_3B014(u8 *work, u8 *surface);
s32 sub_3B0F4(movie_work_t *work);
s32 sub_3B138(u8 *work, u8 *surface);
void sub_3B260(void);
void sub_3B274(void);

/* ORIGINAL_INLINE: exact port string sequence at DOS 1:2ae90..1:2aeab.
 * The C dialect has no expression for REP OUTSB; the scalar port operations elsewhere use inp/outp.
 */
#pragma aux write_movie_palette = \
    "pushad" \
    "add esi,3" \
    "mov edx,0x3c8" \
    "mov eax,1" \
    "out dx,al" \
    "mov ecx,0x2fd" \
    "mov edx,0x3c9" \
    "rep outsb" \
    "popad" \
    parm [esi] modify exact []
extern void write_movie_palette(u8 *palette);

s32 sub_3A1C0(void)
{
    u32 count;
    u32 *dst;

    dst = (u32 *)DrawBufferNormal;
    movie_draw_buffer = (u8 *)dst;
    count = 0x3e80;
    do {
        *dst++ = 0;
    } while (--count != 0);
    return 0;
}

void sub_3A1E0(void)
{
    movie_draw_buffer = 0;
}

s32 sub_3A1EC(u8 mode)
{
    union REGS regs;
    regs.h.ah = 0;
    regs.h.al = mode;
    int386(0x10, &regs, &regs);
    return 1;
}

u8 sub_3A214(void)
{
    union REGS regs;
    regs.h.ah = 0x0f;
    int386(0x10, &regs, &regs);
    return regs.h.al;
}

s32 sub_3A238(u8 *surface)
{
    u8 old_frequency;

    memset(surface, 0, 0x11);
    *(s32 *)(surface + 4) = 320;
    *(u8 **)(surface + 0) = movie_draw_buffer;
    *(s32 *)(surface + 8) = 200;
    movie_saved_mode = 0x3c;
    old_frequency = VGA_FREQ;
    VGA_FREQ = 0x3c;
    movie_saved_frequency = old_frequency;
    set_vga_frequency(0x3c);
    memset(DisplayBufferNormal, 0, 64000);
    memset(movie_video_buffer, 0, 0x300);
    return 0;
}

void sub_3A2AC(void)
{
    VGA_FREQ = movie_saved_frequency;
    set_vga_frequency(VGA_FREQ);
}

s32 sub_3A2C4(movie_surface_t *surface)
{
    return surface->width;
}

s32 sub_3A2C8(movie_surface_t *surface)
{
    return surface->height;
}

void sub_3A2CC(movie_surface_t *surface, s32 x, s32 y, u8 color)
{
    surface->pixels[y * surface->width + x] = color;
}

void sub_3A2D8(movie_surface_t *surface, s32 x, s32 y, const u8 *src, u32 width)
{
    u8 *dst;
    dst = surface->pixels + y * 320 + x;
    copy(dst, src, width);
}

void sub_3A30C(movie_surface_t *surface, s32 x, s32 y, u8 color, u32 width)
{
    u8 *dst;
    u32 i;
    dst = surface->pixels + y * 320 + x;
    i = 0;
    while (i < width) {
        dst[i] = color;
        ++i;
    }
}

void sub_3A34C(movie_surface_t *surface, s32 x, s32 y, u16 color, u32 width)
{
    u16 *dst;
    u32 i;
    dst = (u16 *)(surface->pixels + (y * 320 + x) * 2);
    i = 0;
    while (i < width) {
        dst[i] = color;
        ++i;
    }
}

void sub_3A3B8(u32 context, s32 x, const u8 *rgb, s32 count)
{
    s32 i;
    s32 offset;
    (void)context;
    offset = x * 3;
    i = 0;
    while (i < count) {
        movie_video_buffer[offset] = rgb[0] >> 2;
        movie_video_buffer[offset + 1] = rgb[1] >> 2;
        movie_video_buffer[offset + 2] = rgb[2] >> 2;
        rgb += 3;
        offset += 3;
        ++i;
    }
    if (x < movie_palette_xmin)
        movie_palette_xmin = x;
    if (x + count > movie_palette_xmax)
        movie_palette_xmax = x + count;
}

void sub_3A428(u32 context, s32 x, const u8 *rgb, s32 count)
{
    s32 i;
    s32 offset;
    (void)context;
    offset = x * 3;
    i = 0;
    while (i < count) {
        movie_video_buffer[offset] = rgb[0];
        movie_video_buffer[offset + 1] = rgb[1];
        movie_video_buffer[offset + 2] = rgb[2];
        rgb += 3;
        offset += 3;
        ++i;
    }
    if (x < movie_palette_xmin)
        movie_palette_xmin = x;
    if (x + count > movie_palette_xmax)
        movie_palette_xmax = x + count;
}

s32 sub_3A488(u32 *timer)
{
    *timer = 0x1200;
    outp(0x43, 0x34);
    outp(0x40, 0);
    outp(0x40, 0);
    return 0;
}

void sub_3A4A4(s32 *timer)
{
    (void)timer;
    outp(0x43, 0x36);
    outp(0x40, 0);
    outp(0x40, 0);
}

s32 sub_3A4B8(s32 *timer)
{
    union REGS regs;
    u8 pit_count;
    (void)timer;
    regs.h.ah = 0;
    int386(0x1a, &regs, &regs);
    outp(0x43, 0);
    inp(0x40);
    pit_count = (u8)inp(0x40);
    return ((s32)regs.h.cl << 24) + ((s32)regs.h.dl << 16) +
           ((s32)regs.h.dh << 8) - pit_count;
}

s32 sub_3A518(s32 value, s32 *scale)
{
    u32 scaled;
    scaled = (u32)(value * *scale) / 1000;
    return sub_3A4B8(scale) + (s32)scaled;
}

s32 sub_3A538(s32 *fd, const char *path)
{
    s32 handle;
    handle = open(path, 0x200);
    *fd = handle;
    if (handle < 0)
        return -5;
    return 0;
}

s32 sub_3A55C(s32 fd, void *buffer, u32 count)
{
    u32 actual;
    actual = count;
    if (_dos_read(fd, buffer, count, &actual) != 0)
        return -6;
    return 0;
}

s32 sub_3A578(s32 fd, u8 **buffer, u32 count)
{
    u8 *dst;
    u32 remain;
    u32 part;
    s32 status;
    dst = *buffer;
    remain = count;
    while (remain != 0) {
        if (remain > 0xfff0)
            part = 0xfff0;
        else
            part = remain;
        status = sub_3A55C(fd, dst, part);
        if (status < 0)
            return status;
        dst += part;
        remain -= part;
    }
    return 0;
}

s32 sub_3A5B4(u8 *info)
{
    s32 status;

    if (TheProcesseur == 4)
        movie_audio_track = 0;
    else if (TheProcesseur < 5)
        return -13;
    else
        movie_audio_track = (WindowsLance == 0);
    memset(info, 0, 0x18);
    status = sub_3A488((u32 *)(info + 0x11));
    if (status < 0)
        goto return_status;
    status = sub_3A1C0();
    if (status < 0)
        goto restore_timer;
    status = sub_3A238(info);
    if (status < 0)
        goto restore_timer;
    return 0;

restore_timer:
    sub_3A4A4((s32 *)(info + 0x11));

return_status:
    return status;
}

void sub_3A634(u8 *info)
{
    sub_3A2AC();
    sub_3A4A4((s32 *)(info + 0x11));
    sub_3A1E0();
    if (MusicCdActive) {
        sub_1a5b8(AudioStatusByte() & 0xff);
    }
    (void)info;
}

void sub_3A664(const u8 *data, movie_surface_t *surface, s32 x,
               movie_palette_writer_t writer)
{
    s16 packets;
    s32 run;
    const u8 *src;
    s32 index;

    movie_palette_xmin = 0x100;
    movie_palette_xmax = 0;
    packets = *(const s16 *)data;
    src = data + 2;
    index = 0;
    while (--packets >= 0) {
        index += src[0];
        run = src[1];
        if (run == 0)
            run = 0x100;
        writer((u32)x, index, src + 2, run);
        src += 2 + run * 3;
        index += run;
    }
    (void)surface;
}

void sub_3A6D0(const u8 *data, movie_surface_t *surface, s32 x)
{
    sub_3A664(data, surface, x, sub_3A3B8);
}

void sub_3A6E0(const u8 *data, movie_surface_t *surface, s32 x)
{
    sub_3A664(data, surface, x, sub_3A428);
}

void sub_3A6F0(const u8 *data, movie_work_t *work, movie_surface_t *surface, s8 x_bias)
{
    const u8 *src;
    s32 rows;
    s32 x;
    s32 y;
    s32 right;
    s32 count;
    u8 color;

    src = data;
    rows = work->height;
    x = work->x_offset + x_bias;
    y = work->y_offset;
    right = work->x_offset + work->width;
    while (--rows >= 0) {
        while (x < right) {
            count = (s8)*src++;
            if (count < 0) {
                count = -count;
                color = *src++;
                sub_3A30C(surface, x, y, color, count);
            } else {
                sub_3A2D8(surface, x, y, src, count);
                src += count;
            }
            x += count;
        }
        ++y;
        x = work->x_offset + x_bias;
    }
}

void sub_3A7B4(const u8 *data, movie_work_t *work, movie_surface_t *surface, s8 x_bias)
{
    const u8 *src;
    s32 lines;
    s32 packets;
    s32 x;
    s32 y;
    s32 count;
    s32 skip;
    u8 value;

    y = work->y_offset + *(const s16 *)data;
    lines = *(const u16 *)(data + 2);
    src = data + 4;
    while (--lines >= 0) {
        packets = (s8)*src++;
        x = work->x_offset + x_bias;
        while (--packets >= 0) {
            skip = *src++;
            x += skip;
            count = (s8)*src++;
            if (count < 0) {
                count = -count;
                value = *src++;
                sub_3A30C(surface, x, y, value, count);
            } else {
                sub_3A2D8(surface, x, y, src, count);
                src += count;
            }
            x += count;
        }
        ++y;
    }
}

void sub_3A8A0(const u8 *data, movie_work_t *work, movie_surface_t *surface, s8 x_bias)
{
    const u8 *src;
    s32 lines;
    s32 packets;
    s32 x;
    s32 y;
    s32 run;
    s32 skip;
    u16 color;

    lines = *(const u16 *)data;
    src = data + 2;
    x = work->x_offset + x_bias;
    y = work->y_offset + work->height - 1;
    while (--lines >= 0) {
        packets = *(const s16 *)src;
        src += 2;
        if (packets < 0) {
            skip = (s8)(packets >> 8);
            if (skip & 0x40) {
                y -= (s8)(packets >> 8);
                continue;
            }
            color = *(const u16 *)src;
            src += 2;
            sub_3A2CC(surface, x, y, (u8)color);
            x += 2;
        } else {
            while (--packets >= 0) {
                skip = *src++;
                x += skip;
                run = (s8)*src++;
                if (run >= 0) {
                    sub_3A2D8(surface, x, y, src, run);
                    src += run;
                } else {
                    run = -run;
                    color = *(const u16 *)src;
                    src += 2;
                    sub_3A34C(surface, x, y, color, run);
                }
                x += run;
            }
        }
        ++y;
        x = work->x_offset + x_bias;
    }
}

void sub_3A9F4(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    u32 count;
    u32 *dst;
    dst = (u32 *)surface->pixels;
    count = 0x3e80;
    do {
        *dst++ = 0;
    } while (--count != 0);
    (void)data;
    (void)work;
}

void sub_3AA08(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    s32 row;
    s32 width;
    s32 height;
    u8 *dst;
    const u8 *src;

    src = data;
    dst = surface->pixels + work->surface_offset;
    width = work->width;
    height = work->height;
    row = 0;
    while (row < height) {
        memcpy(dst, src, width);
        src += width;
        dst += 320;
        ++row;
    }
}

void sub_3AA4C(u8 *work_bytes)
{
    movie_work_t *work;

    work = (movie_work_t *)work_bytes;
    close(work->fd);
    memset(work_bytes, 0, 0x99);
}

s32 sub_3AA70(u8 *work_bytes, char *path)
{
    movie_work_t *work;
    s32 status;

    work = (movie_work_t *)work_bytes;
    memset(work, 0, 0x99);
    status = sub_3A538(&work->fd, path);
    if (status < 0)
        return status;
    status = sub_3A55C(work->fd, work, 0x80);
    if (status < 0)
        goto read_error;
    work->frame_index = 0;
    work->path = (char *)path;
    movie_audio_enabled = movie_audio_track != 0;
    if (work->width > 320 || work->height > 200)
        goto size_error;
    if (work->file_magic == 0xaf12) {
        if (work->file_size == 12) {
            work->speed = 30;
            work->data_offset = 12;
            movie_palette_flag = 1;
        }
        lseek(work->fd, work->data_offset, 0);
        return 0;
    }
    if (work->file_magic == 0xaf11) {
        if (work->file_size == 12) {
            work->data_offset = 12;
            work->speed = 5;
            movie_palette_flag = 1;
            lseek(work->fd, work->data_offset, 0);
        } else {
            work->data_offset = 0x80;
        }
        work->speed = work->speed * 1000 / 70;
        return 0;
    } else {
        goto version_error;
    }

size_error:
    sub_3AA4C(work);
    return -3;
version_error:
    sub_3AA4C(work);
    return -11;
read_error:
    sub_3AA4C(work);
    return status;
}

s32 sub_3ABE4(movie_work_t *work, movie_frame_header_t *frame,
              const u8 *chunks, movie_surface_t *surface, s32 *timer)
{
    const u8 *chunk;
    u32 chunk_count;
    u32 i;
    u16 type;
    u16 size;
    s32 start;
    s32 elapsed;
    s32 audio_device;
    u16 query_status;
    u8 cd_frame;
    u8 second;
    u8 minute;
    u8 track;

    chunk = chunks;
    chunk_count = frame->chunks;
    i = 0;
    while (i < chunk_count) {
        size = *(const u16 *)chunk;
        type = *(const u16 *)(chunk + 4);
        switch (type) {
        case 4:
            movie_playback_state = 1;
            if (size == 4)
                sub_3A6D0(chunk + 6, surface, 0);
            else
                sub_3A6E0(chunk + 6, surface, 0);
            break;
        case 5:
            if (size == 7)
                sub_3A8A0(chunk + 6, work, surface, 0);
            else
                sub_3A7B4(chunk + 6, work, surface, 0);
            break;
        case 6:
            sub_3A9F4(chunk + 6, work, surface);
            break;
        case 7:
            sub_3A6F0(chunk + 6, work, surface, 0);
            break;
        case 8:
            sub_3AA08(chunk + 6, work, surface);
            break;
        }
        chunk += size;
        ++i;
    }

    if (work->sound_mode && MusicCdActive && movie_audio_state != 2 && movie_audio_track != -1) {
        audio_device = AudioStatusByte();
        if (playing_intro_video && movie_audio_state == 0) {
            if (work->frame_index >= movie_audio_limits[(u8)movie_audio_track][2] &&
                work->frame_index < movie_audio_limits[(u8)movie_audio_track][3]) {
                query_status = sub_1a02c((u8)audio_device, &track,
                                         &minute, &second, &cd_frame);
                if (query_status == 0x100) {
                    start = ((s32)minute << 16) + ((s32)second << 8) + cd_frame;
                    if (start >= movie_audio_limits[(u8)movie_audio_track][0]) {
                        effect_set_audio_status((u8)audio_device);
                        movie_audio_state = 1;
                        ++movie_audio_counter;
                    }
                }
            } else if (work->frame_index == movie_audio_limits[(u8)movie_audio_track][3]) {
                ++movie_audio_counter;
            }
        } else if (movie_audio_state != 0 && work->frame_index >= movie_audio_limits[(u8)movie_audio_track][1]) {
            effect_restore_audio_status((u8)audio_device);
            movie_audio_state = 0;
        }
    } else {
        movie_audio_state = 1;
    }

    if (movie_audio_enabled) {
        while (work->frame_index != 1) {
            start = sub_3A4B8(timer);
            if (start > movie_next_tick)
                break;
        }
        endsynchro();
        synchro();
        if (movie_audio_track != 0)
            movie_next_tick = sub_3A518(60, timer);
        else
            movie_next_tick = sub_3A518(125, timer);
    }

    if (movie_playback_state) {
        if (movie_playback_state == 1) {
            movie_playback_state = 0;
            write_movie_palette(movie_video_buffer);
        }
        sub_3AA08(movie_video_buffer, work, surface);
        movie_audio_enabled = movie_audio_track != 0;
    }
    return 0;
}

s32 sub_3AF20(movie_work_t *work, movie_surface_t *surface, s32 *timer)
{
    movie_frame_header_t header;
    u32 size;
    u8 *buffer;
    s32 status;

    ++work->frame_index;
    status = sub_3A55C(work->fd, &header, sizeof(header));
    if (status < 0)
        return status;
    if (header.magic != 0xf1fa)
        return -4;
    size = header.size - sizeof(header);
    if ((s32)size > 0) {
        buffer = EffetBufferNormal;
        status = sub_3A578(work->fd, &buffer, size);
        if (status < 0)
            return status;
        status = sub_3ABE4(work, &header, EffetBufferNormal, surface, timer);
    } else {
        status = sub_3ABE4(work, &header, (const u8 *)&header, surface, timer);
    }
    return status;
}

void sub_3AFB4(movie_work_t *work, movie_surface_t *surface)
{
    s32 dx;
    s32 dy;

    dx = work->width;
    dx = sub_3A2C4(surface) - dx;
    if (dx < 0)
        dx = -dx;
    work->x_offset = dx / 2;
    dy = work->height;
    dy = sub_3A2C8(surface) - dy;
    if (dy < 0)
        dy = -dy;
    work->y_offset = dy / 2;
    work->surface_offset = work->y_offset * 320 + work->x_offset;
}

s32 sub_3B014(u8 *work_bytes, u8 *surface_bytes)
{
    movie_work_t *work;
    movie_surface_t *surface;
    s32 *timer;
    s32 status;
    s32 i;
    s32 key;

    work = (movie_work_t *)work_bytes;
    surface = (movie_surface_t *)surface_bytes;
    timer = (s32 *)(surface_bytes + 0x11);

    movie_audio_state = 0;
    if (work->sound_mode && MusicCdActive) {
        sub_1a5b8(AudioStatusByte());
        sub_1a4ac(AudioStatusByte(), 0, 0, 0);
        effect_set_audio_status(AudioStatusByte());
        movie_audio_state = 1;
    }
    i = 0;
    movie_audio_counter = 0;
    status = 0;
    while (i < work->frame_count) {
        readinput();
        key = but0pressed() || but1pressed() || but2pressed() || but3pressed();
        if (!key)
            key = TOUCHE(0x39);
        if (key)
            return 0;
        status = sub_3AF20(work, surface, timer);
        if (status < 0)
            break;
        ++i;
    }
    return status;
}

s32 sub_3B0F4(movie_work_t *work)
{
    s32 status;
    s32 offset;
    movie_frame_header_t header;

    offset = work->data_offset;
    status = lseek(work->fd, offset, 0);
    if (status < 0)
        return -6;
    status = sub_3A55C(work->fd, &header, 16);
    if (status < 0)
        return status;
    work->current_offset = work->data_offset + header.size;
    return 0;
}

s32 sub_3B138(u8 *work_bytes, u8 *surface_bytes)
{
    movie_work_t *work;
    movie_surface_t *surface;
    s32 *timer;
    s32 status;
    s32 i;
    s32 key;

    work = (movie_work_t *)work_bytes;
    surface = (movie_surface_t *)surface_bytes;
    timer = (s32 *)(surface_bytes + 0x11);

    status = sub_3B0F4(work);
    if (status < 0)
        return status;
    lseek(work->fd, work->data_offset, 0);
    status = sub_3AF20(work, surface, timer);
    if (status < 0)
        return status;
    lseek(work->fd, work->current_offset, 0);
    work->frame_index = 1;
    movie_audio_state = 0;
    if (work->sound_mode && MusicCdActive) {
        sub_1a5b8(AudioStatusByte());
        sub_1a4ac(AudioStatusByte(), 0, 0, 0);
        effect_set_audio_status(AudioStatusByte());
        movie_audio_state = 1;
    }
    movie_audio_counter = 0;
    i = 0;
    while (i < work->frame_count) {
        status = sub_3AF20(work, surface, timer);
        if (status < 0)
            return status;
        readinput();
        key = but0pressed() || but1pressed() || but2pressed() || but3pressed();
        if (!key)
            key = TOUCHE(0x39);
        if (key)
            return 0;
        ++i;
    }
    return status;
}

void sub_3B260(void)
{
    if (!CarteSonAutorisee)
        return;
    sub_3E66C();
}

void sub_3B274(void)
{
    if (!CarteSonAutorisee)
        return;
    sub_3E6B0();
}
