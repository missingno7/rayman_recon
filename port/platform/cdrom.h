/* Virtual MSCDEX 2.21: one drive, a read-only single-file CUE/BIN medium.
   Device addresses remain DOS tokens; they are never host pointers. */
#ifndef PORT_CDROM_H
#define PORT_CDROM_H
#include <stdint.h>
#include <stddef.h>
struct dos_regs;

int port_cd_init(const char *cue, char letter);
void port_cd_shutdown(void);
int port_cd_drive(void);
int port_cd_present(void);
/* The inserted disc's ISO 9660 file system: the file `name` in the root directory's subdirectory `dir` (NULL or "" =
   the root), case-insensitive; its first sector and size. 0 when there is no disc or no such file. */
int port_cd_file(const char *dir, const char *name, uint32_t *lba, uint32_t *size);
/* Copy such a file to a host file (written as host_path.part, then renamed). */
int port_cd_copy_file(uint32_t lba, uint32_t size, const char *host_path);
/* The host drive letter when the disc is in a host drive (a real CD or a mounted image), 0 for a CUE/BIN image. */
char port_cd_host_drive(void);
int port_cd_path(const char *path);
uint16_t port_cd_allocate(uint16_t bytes, uint32_t *address);
uint8_t port_cd_free(uint16_t handle);
uint8_t *port_cd_buffer(uint32_t offset, size_t bytes);
int port_cd_interrupt(struct dos_regs *regs);
void port_cd_request(unsigned drive, uint8_t *request, size_t capacity);
uint32_t port_cd_msf(uint32_t lba);
int port_cd_lba(uint32_t msf, uint32_t *lba);
uint32_t port_cd_position(void);
void port_cd_pump(void);
/* Read CD-DA source samples, after channel routing/gain. Used by the stream and tests. */
size_t port_cd_pcm(uint64_t sample, int16_t *out, size_t frames);
#endif
