/*
 * File service: maps the DOS program's paths onto the host.
 *
 *  - Reads resolve against the save directory first (files the port wrote: saves, config), then against the
 *    player's ordered game-data directories (never written).
 *  - Writes go to the save directory, except the virtual CD drive, which returns EROFS.
 *  - CD reads use the ordered data directories directly (no save overlay), only while a disc is present.
 *  - DOS spelling is accepted: '\\' or '/', any letter case, a leading drive ("C:"), a leading "..\\" (the
 *    original ran either from the game directory or one below it; both name the data root here).
 */
#ifndef PORT_PLATFORM_FILES_H
#define PORT_PLATFORM_FILES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* data_dir: the player's Rayman directory (with RAY.LNG, PCMAP\...). save_dir: writable; created if missing. */
int port_files_init(const char *data_dir, const char *save_dir);
/* Append a read root (maximum 32), first match wins. init resets the list. */
int port_files_add_data(const char *data_dir);

/* Resolve a DOS path. for_write = 0: an existing file (save dir, then ordered data dirs); returns 0 if none exists.
   for_write = 1: the save-dir path (parent directories created). Returns 1 on success.
   Virtual CD paths: read-only alias of the data search path; writes return 0 with errno=EROFS. */
int port_files_resolve(const char *dos_path, int for_write, char *out, size_t out_size);

const char *port_files_data_dir(void);
const char *port_files_save_dir(void);

#ifdef __cplusplus
}
#endif

#endif
