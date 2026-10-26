/*
 * DOS / Watcom C library services used by the historical units, as explicit port functions.
 * Implemented in compat/src/dos_clib.c on top of the platform file/input services. port/docs/BOUNDARY.md lists
 * every replaced service and its contract.
 */
#ifndef PORT_DOS_H
#define PORT_DOS_H

#include <stdio.h>
#include <stdint.h>

/* The Watcom <fcntl.h>/<sys/stat.h> flag values are applied by port_watcom_fcntl.h (see there). */

#ifdef __cplusplus
extern "C" {
#endif

/* Files: DOS path spelling ('\\', any case, optional "..\\" prefix, drive letters) is resolved against the
   player's game-data directory (reads) and the port's save directory (writes). */
FILE *port_fopen(const char *path, const char *mode);
int port_open(const char *path, int watcom_flags, ...);
int port_read(int fd, void *buf, unsigned count);
/* DOS success (0) includes a short read or EOF; actual is written on success. */
int32_t _dos_read(int32_t fd, void *buf, uint32_t count, uint32_t *actual);
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

/* Heap: zero-filled blocks (port decision, docs/DIVERGENCES.md). The DOS program read some freshly allocated
   memory before writing it; fresh DPMI pages were zero on a booted machine, and zero-filling makes the port
   deterministic instead of dependent on host heap layout. */
void *port_malloc(size_t size);

/* time(): wall clock in real time; in deterministic mode the port clock from a fixed epoch (MakeMyRand seeds the
   game's random table with srand(time(NULL)), src/rand.c). */
#include <time.h>
time_t port_time(time_t *out);
void port_set_time_epoch(long long seconds);

/* BIOS keyboard buffer (INT 16h as seen through Watcom kbhit/getch). */
int port_kbhit(void);
int port_getch(void);
int port_getche(void);

#ifdef __cplusplus
}
#endif

#endif
