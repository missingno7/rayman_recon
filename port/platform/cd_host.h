/* A CD in one of the host's drives (a real disc or a mounted image) as the medium of the virtual MSCDEX drive
   (platform/cdrom.c). Read-only device access; Windows only, elsewhere every call reports "no drive". */
#ifndef PORT_CD_HOST_H
#define PORT_CD_HOST_H
#include <stdint.h>

typedef struct port_cd_host_track { uint32_t start; uint8_t data; } port_cd_host_track;

/* The first CD drive whose medium is labelled RAYMAN; 0 when there is none. */
char port_cd_host_find(void);
/* Open drive `letter` and read its table of contents: tracks[1..*last] (index 0 unused, capacity 100) and the
   lead-out, as LBAs. Returns 0 when the drive has no readable disc. */
int port_cd_host_open(char letter, port_cd_host_track *tracks, unsigned *first, unsigned *last, uint32_t *leadout);
/* One sector: 2048 user bytes of a data sector (raw = 0) or 2352 bytes (raw = 1; audio sectors are the disc's
   samples, data sectors get a synthesized header and no EDC/ECC). */
int port_cd_host_read(uint32_t lba, int raw, int data_track, uint8_t *out);
void port_cd_host_close(void);
int port_cd_host_is_open(void);
#endif
