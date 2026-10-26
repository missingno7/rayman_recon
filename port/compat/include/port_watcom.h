/*
 * Pre-included (-include) into every historical translation unit compiled by the native port.
 *
 * It makes the Watcom 10.6 / DOS dialect of ../../src acceptable to a modern C compiler WITHOUT editing the
 * historical files. Everything here is either a pure spelling adapter (keywords with no meaning in a flat
 * native address space) or a redirection of a DOS/Watcom C-library service to an explicit port service.
 * Rules and rationale: port/docs/ARCHITECTURE.md ("Compiling the historical C").
 */
#ifndef PORT_WATCOM_H
#define PORT_WATCOM_H

/* Standard headers the historical units use, included BEFORE the redirections below so that their own
   declarations are never renamed. Later #includes in the units are no-ops (include guards). */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Watcom segment/calling keywords. The port is a flat 32/64-bit program: a __far pointer is an ordinary
   pointer and an __interrupt handler is an ordinary function called by the platform layer. */
#define __far
#define __near
#define __huge
#define __interrupt
#define __loadds
#define __saveregs
#define _WCRTLINK

/* Watcom C library extensions and DOS services, implemented by the port (compat/src/dos_clib.c). */
#include "port_dos.h"

#define fopen(path, mode) port_fopen((path), (mode))
#define open port_open
#define read port_read
#define write port_write
#define close port_close
#define lseek port_lseek
#define unlink port_unlink
#define remove port_remove
#define filelength port_filelength
#define itoa port_itoa
#define ltoa port_ltoa
#define strupr port_strupr
#define kbhit port_kbhit
#define getch port_getch
#define getche port_getche
#define sleep port_sleep
#define tmpnam port_tmpnam
#define malloc port_malloc
#define time port_time
#define rand port_rand
#define srand port_srand

/* Last: Watcom flag values over every host definition (port_watcom_fcntl.h). */
#include "port_watcom_fcntl.h"

#endif
