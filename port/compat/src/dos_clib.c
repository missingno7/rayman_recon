/*
 * Watcom 10.6 C-library behaviour the historical units depend on, on top of the platform services.
 * Files go through platform/files.c (DOS paths, read-only game data, separate save directory).
 */
#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "platform/files.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

/* Watcom <fcntl.h> values the units pass (port_dos.h). */
enum {
    W_RDONLY = 0x0000, W_WRONLY = 0x0001, W_RDWR = 0x0002, W_APPEND = 0x0010, W_CREAT = 0x0020,
    W_TRUNC = 0x0040, W_TEXT = 0x0100, W_BINARY = 0x0200, W_EXCL = 0x0400
};

static int wants_write(const char *mode)
{
    return strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+');
}

/* Copy a data-directory file into the save directory before it is opened for update. */
static void copy_up(const char *dos_path, const char *dest)
{
    char src[1024];
    FILE *in, *out;
    char buf[8192];
    size_t n;
    if (!port_files_resolve(dos_path, 0, src, sizeof src) || strcmp(src, dest) == 0)
        return;
    if (!(in = fopen(src, "rb")))
        return;
    if ((out = fopen(dest, "wb"))) {
        while ((n = fread(buf, 1, sizeof buf, in)) > 0)
            fwrite(buf, 1, n, out);
        fclose(out);
    }
    fclose(in);
}

FILE *port_fopen(const char *path, const char *mode)
{
    char host[1024];
    if (!wants_write(mode)) {
        if (!port_files_resolve(path, 0, host, sizeof host)) {
            errno = ENOENT;
            return NULL;
        }
        return fopen(host, mode);
    }
    port_files_resolve(path, 1, host, sizeof host);
    if (mode[0] == 'r')
        copy_up(path, host);
    return fopen(host, mode);
}

int port_open(const char *path, int wflags, ...)
{
    char host[1024];
    int flags = 0, writing = (wflags & (W_WRONLY | W_RDWR | W_CREAT | W_TRUNC | W_APPEND)) != 0;
    int mode = 0;
    if (wflags & W_CREAT) {
        va_list ap;
        va_start(ap, wflags);
        mode = va_arg(ap, int);
        va_end(ap);
    }
    if (writing) {
        port_files_resolve(path, 1, host, sizeof host);
        if (!(wflags & W_TRUNC))
            copy_up(path, host);
    } else if (!port_files_resolve(path, 0, host, sizeof host)) {
        errno = ENOENT;
        return -1;
    }
    flags = (wflags & W_RDWR) ? O_RDWR : (wflags & W_WRONLY) ? O_WRONLY : O_RDONLY;
    if (wflags & W_APPEND) flags |= O_APPEND;
    if (wflags & W_CREAT) flags |= O_CREAT;
    if (wflags & W_TRUNC) flags |= O_TRUNC;
    if (wflags & W_EXCL) flags |= O_EXCL;
#ifdef _WIN32
    flags |= (wflags & W_BINARY) ? O_BINARY : (wflags & W_TEXT) ? O_TEXT : O_BINARY;
    return _open(host, flags, (mode & 0200 ? _S_IWRITE : 0) | _S_IREAD);
#else
    return open(host, flags, mode ? 0666 : 0666);
#endif
}

#ifdef _WIN32
int port_read(int fd, void *buf, unsigned count) { return _read(fd, buf, count); }
int port_write(int fd, const void *buf, unsigned count) { return _write(fd, buf, count); }
int port_close(int fd) { return _close(fd); }
long port_lseek(int fd, long offset, int origin) { return _lseek(fd, offset, origin); }
long port_filelength(int fd) { return _filelength(fd); }
#else
int port_read(int fd, void *buf, unsigned count) { return (int)read(fd, buf, count); }
int port_write(int fd, const void *buf, unsigned count) { return (int)write(fd, buf, count); }
int port_close(int fd) { return close(fd); }
long port_lseek(int fd, long offset, int origin) { return (long)lseek(fd, offset, origin); }
long port_filelength(int fd)
{
    struct stat st;
    return fstat(fd, &st) == 0 ? (long)st.st_size : -1L;
}
#endif

int port_unlink(const char *path)
{
    char host[1024];
    /* Only files the port wrote can be removed; the game data stays untouched. */
    port_files_resolve(path, 1, host, sizeof host);
    return remove(host);
}

int port_remove(const char *path) { return port_unlink(path); }

char *port_tmpnam(char *buf)
{
    static char name[32];
    static unsigned counter;
    char *out = buf ? buf : name;
    snprintf(out, 32, "TMP%05u.TMP", ++counter);
    return out;
}

static char *to_radix(unsigned long v, int neg, char *buf, int radix)
{
    char tmp[40];
    int n = 0, i = 0;
    if (radix < 2 || radix > 36) {
        buf[0] = 0;
        return buf;
    }
    do {
        int d = (int)(v % (unsigned)radix);
        tmp[n++] = (char)(d < 10 ? '0' + d : 'a' + d - 10);
        v /= (unsigned)radix;
    } while (v);
    if (neg)
        buf[i++] = '-';
    while (n)
        buf[i++] = tmp[--n];
    buf[i] = 0;
    return buf;
}

/* Watcom itoa/ltoa: a minus sign only for radix 10; other radices print the unsigned bit pattern. */
char *port_itoa(int value, char *buf, int radix)
{
    if (radix == 10 && value < 0)
        return to_radix(0u - (unsigned)value, 1, buf, radix);
    return to_radix((unsigned)value, 0, buf, radix);
}

char *port_ltoa(long value, char *buf, int radix)
{
    int v = (int)value;   /* Watcom long is 32 bits */
    return port_itoa(v, buf, radix);
}

char *port_strupr(char *s)
{
    char *p;
    for (p = s; *p; p++)
        *p = (char)toupper((unsigned char)*p);
    return s;
}

void port_sleep(unsigned seconds)
{
    port_clock_wait_until(port_clock_ns() + (uint64_t)seconds * 1000000000ULL);
}

/* Watcom delay(): the bound wait_milliseconds at DOS 1:7009e. */
void wait_milliseconds(unsigned ms)
{
    port_clock_wait_until(port_clock_ns() + (uint64_t)ms * 1000000ULL);
    port_pc_poll();
}

/* Watcom memset-shaped helper at DOS 1:70420 (destination, byte value, length). */
void clear_memory(void *destination, int value, int length)
{
    memset(destination, value, (size_t)length);
}

/* Watcom itoa at DOS 1:71256 (value, radix in the second register, destination). */
char *itoa_dos(int value, char *destination, int radix)
{
    return port_itoa(value, destination, radix);
}

/* Watcom 10.6 rand/srand (DOS 1:71396 / 1:713ba, seed at 2:2d9b4, initially 1). */
static uint32_t rand_next = 1;
int port_rand(void)
{
    rand_next = rand_next * 1103515245u + 12345u;
    return (int)((rand_next >> 16) & 0x7fff);
}
void port_srand(unsigned seed) { rand_next = seed; }

/* Watcom acos as linked at DOS 1:75b4a (called effect_math_curve): pi/2' - atan(x / sqrt(1 - x*x)) with the
   library's constant 0x3ff921fb54442d15 (3 ulp below pi/2), 0 or pi at |x| == 1, domain error otherwise. */
double effect_math_curve(double x)
{
    static const union { uint64_t u; double d; } half_pi = {0x3ff921fb54442d15ULL}, pi = {0x400921fb54442d18ULL};
    double r = 1.0 - x * x;
    if (r < 0) {
        errno = EDOM;
        return 0.0;
    }
    if (r == 0)
        return x < 0 ? pi.d : 0.0;
    return half_pi.d - atan(x / sqrt(r));
}

/* BIOS keyboard buffer: the game reads it only while the BIOS INT 9 handler is installed. The port does not
   translate keys for it yet (no caller on the supported paths). */
int port_kbhit(void)
{
    port_pc_poll();
    return 0;
}

int port_getch(void)
{
    port_unimplemented("getch: BIOS keyboard buffer read");
    return 0;
}

int port_getche(void) { return port_getch(); }
