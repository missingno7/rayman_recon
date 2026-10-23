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
#include <stdio.h>
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
    s32 speed;
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
    s16 chunks;
    u8 reserved[8];
} movie_frame_header_t;

typedef struct movie_chunk_t {
    u32 size;
    u16 type;
} movie_chunk_t;

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
/* Signed index/sentinel: DOS 1:2ad12 reads [2:64dc-3], then SAR 24. */
extern s8 movie_audio_counter;
extern s32 movie_audio_limits[5][4];
extern s32 movie_next_tick;

extern void set_vga_frequency(u8 frequency);
extern u8 AudioStatusByte(void);
extern void sub_1a5b8(s32 state);
extern u16 sub_1a02c(u8 command, u8 *track, u8 *minute, u8 *second, u8 *frame);
extern void sub_1a4ac(s32 command, u8 track, u8 minute, u8 second);
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
void sub_3A2D8(movie_surface_t *surface, s32 x, s32 y, const u8 *src, s32 width);
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
void sub_3A664(const u8 *data, movie_work_t *work, movie_surface_t *surface,
               movie_palette_writer_t writer);
void sub_3A6D0(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3A6E0(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3A6F0(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3A7B4(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3A8A0(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3A9F4(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3AA08(const u8 *data, movie_work_t *work, movie_surface_t *surface);
void sub_3AA4C(u8 *work);
s32 sub_3AA70(movie_work_t *work, char *path);
s32 sub_3ABE4(movie_work_t *work, movie_frame_header_t *frame,
              const u8 *chunks, movie_surface_t *surface, s32 *timer);
s32 sub_3AF20(movie_work_t *work, movie_surface_t *surface, s32 *timer);
void sub_3AFB4(movie_work_t *work, movie_surface_t *surface);
s32 sub_3B014(movie_work_t *work, movie_surface_t *surface);
s32 sub_3B0F4(movie_work_t *work);
s32 sub_3B138(movie_work_t *work, movie_surface_t *surface);
void sub_3B260(void);
void sub_3B274(void);


s32 sub_3A1C0(void)
{
    movie_draw_buffer = DrawBufferNormal;
    fill(DrawBufferNormal);
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

void sub_3A2D8(movie_surface_t *surface, s32 x, s32 y, const u8 *src, s32 width)
{
    u8 *dst;
    dst = x + y * 320 + surface->pixels;
    copy(src, dst, width);
}

void sub_3A30C(movie_surface_t *surface, s32 x, s32 y, u8 color, u32 width)
{
    u8 *dst;
    u32 i;
    dst = surface->pixels + (x + y * 64 + y * 256);
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
    dst = (u16 *)(surface->pixels + (x + y * 64 + y * 256));
    i = 0;
    while (i < width) {
        dst[i] = color;
        ++i;
    }
}

void sub_3A3B8(u32 context, s32 x, const u8 *rgb, s32 count)
{
    s32 i;
    s32 end;
    (void)context;
    end = x + count;
    i = x;
    while (i < end) {
        movie_video_buffer[i * 3] = rgb[0] >> 2;
        movie_video_buffer[i * 3 + 1] = rgb[1] >> 2;
        movie_video_buffer[i * 3 + 2] = rgb[2] >> 2;
        rgb += 3;
        ++i;
    }
    if (x < movie_palette_xmin)
        movie_palette_xmin = x;
    if (end > movie_palette_xmax)
        movie_palette_xmax = end;
}

void sub_3A428(u32 context, s32 x, const u8 *rgb, s32 count)
{
    s32 i;
    s32 end;
    (void)context;
    end = x + count;
    i = x;
    while (i < end) {
        movie_video_buffer[i * 3] = rgb[0];
        movie_video_buffer[i * 3 + 1] = rgb[1];
        movie_video_buffer[i * 3 + 2] = rgb[2];
        rgb += 3;
        ++i;
    }
    if (x < movie_palette_xmin)
        movie_palette_xmin = x;
    if (end > movie_palette_xmax)
        movie_palette_xmax = end;
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
  s32 low;
  s32 high;
  s32 upper;
  (void) timer;
  regs.h.ah = 0;
  int386(0x1a, &regs, &regs);
  outp(0x43, 0);
  {
    inp(0x40);
  }
  pit_count = -((u8) inp(0x40));
  {
    s32 perm_value_3;
    s32 *perm_address_2;
    perm_address_2 = &perm_value_3;
    *perm_address_2 = regs.h.dl;
    low = perm_value_3;
  }
  high = (s32) regs.h.dh;
  upper = regs.h.cl;
  return pit_count + ((upper << 24) + (((high << 8) + low) << 8));
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

void sub_3A664(const u8 *data, movie_work_t *work, movie_surface_t *surface,
               movie_palette_writer_t writer)
{
    s16 packets;
    s32 run;
    const u8 *src;
    s32 index;

    index = 0;
    movie_palette_xmax = index;
    movie_palette_xmin = 0x100;
    packets = *(const s16 *)data;
    src = data + 2;
    while (--packets >= 0) {
        index += *src++;
        run = *src++;
        if (run == 0)
            run = 0x100;
        writer((u32)surface, index, src, run);
        src += run * 3;
        index += run;
    }
    (void)work;
}

void sub_3A6D0(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    sub_3A664(data, work, surface, sub_3A3B8);
}

void sub_3A6E0(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    sub_3A664(data, work, surface, sub_3A428);
}

void sub_3A6F0(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    const u8 *src;
    s32 height;
    s32 width;
    s32 x;
    s32 y;
    s32 right;
    s8 count;

    src = data;
    /* DOS 1:2a700..2a720: promote both dimensions before spilling the row count. */
    width = work->width;
    height = work->height;
    right = work->x_offset + width;
    y = work->y_offset;
    while (--height >= 0) {
        count = 0;
        ++src; /* BRUN packet count is not used; decode to the row width. */
        x = work->x_offset;
        while ((x += count) < right) {
            count = (s8)*src++;
            if (count >= 0) {
                sub_3A30C(surface, x, y, *src++, count);
            } else {
                count = -count;
                sub_3A2D8(surface, x, y, src, count);
                src += count;
            }
        }
        ++y;
    }
}

void sub_3A7B4(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
  s32 run;
  const u8 *src;
  s16 lines;
  u8 packets;
  s32 x_base;
  s32 x;
  s32 y;
  s8 count;
  src = data + 4;
  {
    s32 perm_value_0;
    s32 *perm_address_0;
    perm_address_0 = &perm_value_0;
    *perm_address_0 = work->x_offset;
    x_base = perm_value_0;
  }
  y = (*((const s16 *) data)) + work->y_offset;
  {
    data += 2;
  }
  lines = *((const s16 *) data);
  while ((--lines) >= 0)
  {
    x = x_base;
    packets = *(src++);
    while (0 < packets)
    {
      x += *(src++);
      count = (s8) (*(src++));
      if (count < 0)
      {
        {
          s8 perm_value_7;
          perm_value_7 = -count;
          count = perm_value_7;
        }
        run = count;
        sub_3A30C(surface, x, y, *(src++), run);
        --packets;
        x += run;
      }
      else
      {
        run = count;
        sub_3A2D8(surface, x, y, src, run);
        x = x + run;
        --packets;
        src += count;
      }
    }

    ++y;
  }

}

void sub_3A8A0(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
  const u8 *src;
  s32 x_base;
  s32 right;
  s32 x;
  s32 y;
  s16 lines;
  s16 packets;
  s32 run;
  x_base = work->x_offset;
  {
    s32 perm_value_2;
    perm_value_2 = work->y_offset;
    y = perm_value_2;
  }
  right = (x_base + work->width) - 1;
  {
    s16 perm_value_4;
    perm_value_4 = *((const s16 *) data);
    lines = perm_value_4;
  }
  src = data + 2;
  do
  {
    read_packets:
    packets = *((const s16 *) src);

    src += 2;
    if (packets < 0)
    {
      if (packets & 0x4000)
      {
        y -= packets;
        goto read_packets;
      }
      sub_3A2CC(surface, (u8) packets, right, (u8) y);
      packets = *((const s16 *) src);
      src += 2;
      if (packets == 0)
      {
        ++y;
        --lines;
        continue;
      }
    }
    x = x_base;
    do
    {
      x += *(src++);
      run = ((s8) (*(src++))) * 2;
      if (!(run >= 0))
      {
        run = -run;
        sub_3A34C(surface, x, y, *((const u16 *) src), run >> 1);
        src += 2;
        x += run;
      }
      else
      {
        sub_3A2D8(surface, x, y, src, run);
        {
          s32 perm_value_10;
          s32 *perm_address_9;
          perm_address_9 = &perm_value_10;
          *perm_address_9 = x + run;
          x = perm_value_10;
        }
        src += run;
      }
    }
    while ((--packets) != 0);
    ++y;
    --lines;
  }
  while (lines >= 1);
}

void sub_3A9F4(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    fill(surface->pixels);
    (void)data;
    (void)work;
}

void sub_3AA08(const u8 *data, movie_work_t *work, movie_surface_t *surface)
{
    s32 width;
    s32 rows;
    u8 *dst;
    const u8 *src;

    dst = surface->pixels + work->surface_offset;
    rows = work->height;
    width = work->width;
    src = data;
    do {
        memcpy(dst, src, width);
        dst += 320;
        src += width;
    } while (--rows != 0);
}

void sub_3AA4C(u8 *work_bytes)
{
    movie_work_t *work;

    work = (movie_work_t *)work_bytes;
    close(work->fd);
    memset(work_bytes, 0, 0x99);
}

s32 sub_3AA70(movie_work_t *work, char *path)
{
    s32 status;
    memset(work, 0, 0x99);
    status = sub_3A538(&work->fd, path);
    if (status < 0)
        goto open_error;
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
            lseek(work->fd, work->data_offset, 0);
            movie_palette_flag = 1;
        } else {
            work->data_offset = 0x80;
        }
        work->speed = work->speed * 1000 / 70;
        return 0;
    } else {
        goto version_error;
    }

version_error:
    status = -11;
    goto read_error;
size_error:
    status = -3;
read_error:
    sub_3AA4C(work);
open_error:
    return status;
}

s32 sub_3ABE4(movie_work_t *work, movie_frame_header_t *frame,
              const u8 *chunks, movie_surface_t *surface, s32 *timer)
{
    const u8 *chunk;
    u16 query_status;
    u8 second;
    u8 cd_frame;
    s32 i;
    const movie_chunk_t *record;
    u8 audio_device;
    u8 track;
    u8 minute;
    s32 start;

    chunk = chunks;
    i = 0;
    for (; i < frame->chunks; ++i) {
        record = (const movie_chunk_t *)chunk;
        chunk += record->size;
        switch (record->type) {
        case 4:
        case 11:
            movie_playback_state = 1;
            if (record->type == 4)
                sub_3A6D0((const u8 *)record + 6, work, surface);
            else
                sub_3A6E0((const u8 *)record + 6, work, surface);
            break;
        case 7:
        case 12:
            if (record->type == 7)
                sub_3A8A0((const u8 *)record + 6, work, surface);
            else
                sub_3A7B4((const u8 *)record + 6, work, surface);
            break;
        case 13:
            sub_3A9F4((const u8 *)record + 6, work, surface);
            break;
        case 15:
            sub_3A6F0((const u8 *)record + 6, work, surface);
            break;
        case 16:
            sub_3AA08((const u8 *)record + 6, work, surface);
            break;
        }
    }

    if (work->sound_mode > 0 && MusicCdActive && movie_audio_state != 2 && movie_audio_counter != -1) {
        audio_device = AudioStatusByte();
        if (playing_intro_video) {
            if (movie_audio_state == 0) {
                if (work->frame_index >= movie_audio_limits[movie_audio_counter][2] &&
                    work->frame_index < movie_audio_limits[movie_audio_counter][3]) {
                    query_status = sub_1a02c(audio_device, &track,
                                             &minute, &second, &cd_frame);
                    if (query_status == 0x100) {
                        start = ((s32)minute << 16) + ((s32)second << 8) + cd_frame;
                        if (start >= movie_audio_limits[movie_audio_counter][0]) {
                            effect_set_audio_status(audio_device);
                            movie_audio_state = 1;
                            ++movie_audio_counter;
                        }
                    }
                } else if (work->frame_index == movie_audio_limits[movie_audio_counter][3]) {
                    ++movie_audio_counter;
                }
                if (movie_audio_counter == 5)
                    movie_audio_counter = -1;
            } else if (work->frame_index >= movie_audio_limits[movie_audio_counter][1]) {
                effect_restore_audio_status(audio_device);
                movie_audio_state = 0;
            }
        } else if (movie_audio_state != 0) {
            effect_restore_audio_status(audio_device);
            movie_audio_state = 0;
        }
    } else {
        movie_audio_state = 1;
    }

    if (movie_audio_enabled) {
        if (work->frame_index != 1) {
            while ((u32)sub_3A4B8(timer) <= (u32)movie_next_tick) {
            }
        }
        endsynchro();
        synchro();
        if (movie_audio_track != 0)
            movie_next_tick = sub_3A518(60, timer);
        else
            movie_next_tick = sub_3A518(125, timer);
    }

    if (movie_audio_enabled) {
        s32 width;
        s32 rows;
        u8 *dst;
        const u8 *src;
        if (movie_playback_state == 1) {
            write_movie_palette(movie_video_buffer);
            movie_playback_state = 0;
        }
        dst = DisplayBufferNormal + work->surface_offset;
        src = surface->pixels + work->surface_offset;
        width = work->width;
        rows = work->height;
        do {
            memcpy(dst, src, width);
            dst += 320;
            src += 320;
        } while (--rows != 0);
    }
    if (movie_audio_track != 0)
        movie_audio_enabled = 1;
    else
        movie_audio_enabled ^= 1;
    return 0;
}

s32 sub_3AF20(movie_work_t *work, movie_surface_t *surface, s32 *timer)
{
    movie_frame_header_t header;
    s32 size;
    u8 *buffer;
    s32 status;

    ++work->frame_index;
    status = sub_3A55C(work->fd, &header, sizeof(header));
    if (status >= 0) {
        if (header.magic == 0xf1fa) {
            size = header.size - sizeof(header);
            if (size > 0) {
                buffer = EffetBufferNormal;
                status = sub_3A578(work->fd, &buffer, size);
                if (status >= 0)
                    status = sub_3ABE4(work, &header, buffer, surface, timer);
            } else {
                status = sub_3ABE4(work, &header, 0, surface, timer);
            }
        } else {
            status = -4;
        }
    }
    return status;
}

void sub_3AFB4(movie_work_t *work, movie_surface_t *surface)
{
    work->x_offset = (sub_3A2C4(surface) - work->width) / 2;
    work->y_offset = (sub_3A2C8(surface) - work->height) / 2;
    work->surface_offset = work->x_offset + work->y_offset * 320;
}

s32 sub_3B014(movie_work_t *work, movie_surface_t *surface)
{
    s32 *timer;
    s32 status;
    s32 i;

    movie_audio_state = 0;
    if (work->sound_mode > 0 && MusicCdActive) {
        sub_1a5b8(AudioStatusByte());
        sub_1a4ac(AudioStatusByte(), work->sound_mode, 0, 0);
        effect_set_audio_status(AudioStatusByte());
        movie_audio_state = 1;
    }
    timer = (s32 *)((u8 *)surface + 0x11);
    i = 0;
    movie_audio_counter = 0;
    for (; i < work->frame_count; ++i) {
        readinput();
        if (but0pressed() || but1pressed() || but2pressed() || but3pressed() || TOUCHE(0x39))
            return 0;
        status = sub_3AF20(work, surface, timer);
        if (status < 0)
            break;
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
    status = sub_3A55C(work->fd, &header, 16);
    if (status < 0)
        return status;
    work->current_offset = work->data_offset + header.size;
    return 0;
}

s32 sub_3B138(movie_work_t *work, movie_surface_t *surface)
{
    s32 *timer;
    s32 status;
    s32 i;

    sub_3B0F4(work);
    lseek(work->fd, work->data_offset, 0);
    timer = (s32 *)((u8 *)surface + 0x11);
    status = sub_3AF20(work, surface, timer);
    if (status < 0)
        return status;
    for (;;) {
        work->frame_index = 1;
        lseek(work->fd, work->current_offset, 0);
        movie_audio_state = 0;
        if (work->sound_mode > 0 && MusicCdActive) {
            sub_1a5b8(AudioStatusByte());
            sub_1a4ac(AudioStatusByte(), work->sound_mode, 0, 0);
            effect_set_audio_status(AudioStatusByte());
            movie_audio_state = 1;
        }
        movie_audio_counter = 0;
        i = 0;
        for (; i < work->frame_count; ++i) {
            status = sub_3AF20(work, surface, timer);
            if (status < 0)
                return status;
            readinput();
            if (but0pressed() || but1pressed() || but2pressed() || but3pressed() || TOUCHE(0x39))
                return 0;
        }
    }
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

/* Movie playback entry points (1:2b288..1:2b4cb), same translation unit: the DOS start 1:2b288 is not
 * paragraph-aligned, so these functions cannot be a separate object (docs/LINK.md; ev_regional). */


s32 playVideo(const char* path, const char* filename, u8 a2);
s32 playVideo_alt(const char* path, const char* filename, u8 a2);
s32 playVideo2(const char* path, const char* filename, u8 a3, u8 a4);
extern u8 movie_video_buffer[768];
extern void endsynchro(void);
#pragma aux endsynchro parm [] modify exact []
extern void synchro(void);
#pragma aux synchro parm [] modify exact []
extern u8 byte_CFA2A;
extern u8 playing_intro_video;
extern u8 MusicCdActive;
extern u8 movie_playback_state;
extern u8 AudioStatusByte(void);
extern void sub_1a5b8(s32 state);

typedef struct movie_local_t {
    u8 work[0x98];
    u8 sound_mode;
    u8 pad[3];
    char path[0x80];
    u8 info[0x18];
} movie_local_t;


void display_movie_frames(void)
{
    s32 pos;
    s32 count;
    s8 color;

    for (count = 0; count < 0x40; count += 2) {
        for (pos = 0; pos != 0x300; pos += 3) {
            color = movie_video_buffer[pos] - 2;
            if (color < 0)
                color = 0;
            movie_video_buffer[pos] = color;

            color = movie_video_buffer[pos + 1] - 2;
            if (color < 0)
                color = 0;
            movie_video_buffer[pos + 1] = color;

            color = movie_video_buffer[pos + 2] - 2;
            if (color < 0)
                color = 0;
            movie_video_buffer[pos + 2] = color;
        }

        endsynchro();
        synchro();

        write_movie_palette(movie_video_buffer);
    }
}

s32 playVideo2(const char *path, const char *filename, u8 a3, u8 a4)
{
    movie_local_t local;
    FILE *file;
    s32 status;

    movie_playback_state = 0;
    playing_intro_video = (stricmp("intro.dat", filename) == 0);

    if (byte_CFA2A) {
        if (strcmp(path, ".\\") == 0)
            return -5;
        strcpy(local.path, path);
        strcat(local.path, filename);
    } else {
        strcpy(local.path, filename);
    }

    file = fopen(local.path, "rb");
    if (file == NULL)
        return -5;
    fclose(file);

    if (MusicCdActive)
        sub_1a5b8(AudioStatusByte());
    else
        a3 = 0;

    sub_3B260();
    status = sub_3A5B4(local.info);
    if (status >= 0) {
        status = sub_3AA70(local.work, local.path);
        if (status >= 0) {
            local.sound_mode = a3;
            sub_3AFB4(local.work, local.info);
            if (a4)
                status = sub_3B138(local.work, local.info);
            else
                status = sub_3B014(local.work, local.info);
            display_movie_frames();
            sub_3AA4C(local.work);
        }
    }
    sub_3A634(local.info);
    sub_3B274();
    return status;
}

s32 playVideo(const char* path, const char* filename, u8 a2) {
    return playVideo2(path, filename, a2, 0);
}

s32 playVideo_alt(const char* path, const char* filename, u8 a2) {
    return playVideo2(path, filename, a2, 1);
}
