/*
 * Watcom 10.6 <fcntl.h> / <sys/stat.h> flag values (C:	ools\watcom-10.6\H) for the historical units.
 *
 * Deliberately WITHOUT an include guard and included LAST by port_watcom.h: the host headers define their own
 * values (MinGW: O_CREAT 0x100, O_BINARY 0x8000; in MSVCRT 0x200 is O_TRUNC), and MinGW's <fcntl.h> pulls in
 * <io.h> and then redefines them, so these must be applied after every host header. The historical sources pass
 * the Watcom encoding, sometimes numerically (movie.c opens with the literal 0x200 = O_BINARY); port_open()
 * always decodes it. Getting this wrong opened RAYMAN.CFG in text mode (CR inserted, 134 bytes): the game then
 * rejected its own configuration on the next start and skipped the sound-driver setup.
 */
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
