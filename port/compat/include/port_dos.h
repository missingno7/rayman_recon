/*
 * DOS / Watcom C library services used by the historical units, as explicit port functions.
 * Implemented in compat/src/dos_clib.c on top of the platform file/input services. port/docs/BOUNDARY.md lists
 * every replaced service and its contract.
 */
#ifndef PORT_DOS_H
#define PORT_DOS_H

#include <stdio.h>
#include <stdint.h>

/* Watcom 10.6 <fcntl.h> / <sys/stat.h> flag values (C:\tools\watcom-10.6\H). The historical sources pass these
   numerically (e.g. movie.c opens with the literal 0x200 = O_BINARY): the host C library's own values differ
   (in MSVCRT 0x200 is O_TRUNC), so port_open() always decodes the Watcom encoding. */
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR
#undef O_APPEND
#undef O_CREAT
#undef O_TRUNC
#undef O_NOINHERIT
#undef O_TEXT
#undef O_BINARY
#undef O_EXCL
#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002
#define O_APPEND    0x0010
#define O_CREAT     0x0020
#define O_TRUNC     0x0040
#define O_NOINHERIT 0x0080
#define O_TEXT      0x0100
#define O_BINARY    0x0200
#define O_EXCL      0x0400
#undef S_IREAD
#undef S_IWRITE
#define S_IREAD     0000400
#define S_IWRITE    0000200

#ifdef __cplusplus
extern "C" {
#endif

/* Files: DOS path spelling ('\\', any case, optional "..\\" prefix, drive letters) is resolved against the
   player's game-data directory (reads) and the port's save directory (writes). */
FILE *port_fopen(const char *path, const char *mode);
int port_open(const char *path, int watcom_flags, ...);
int port_read(int fd, void *buf, unsigned count);
int port_write(int fd, const void *buf, unsigned count);
int port_close(int fd);
long port_lseek(int fd, long offset, int origin);
long port_filelength(int fd);
int port_unlink(const char *path);
int port_remove(const char *path);
char *port_tmpnam(char *buf);

/* Watcom C library extensions. */
char *port_itoa(int value, char *buf, int radix);
char *port_ltoa(long value, char *buf, int radix);
char *port_strupr(char *s);
/* port_sleep(seconds): the units declare `sleep` themselves (load.c: void sleep(u32)). */
/* Watcom 10.6 rand/srand (linked copy at DOS 1:71396/1:713ba, seed 2:2d9b4 = 1):
   next = next * 1103515245 + 12345; result = (next >> 16) & 0x7fff. */
int port_rand(void);
void port_srand(unsigned seed);

/* BIOS keyboard buffer (INT 16h as seen through Watcom kbhit/getch). */
int port_kbhit(void);
int port_getch(void);
int port_getche(void);

#ifdef __cplusplus
}
#endif

#endif
