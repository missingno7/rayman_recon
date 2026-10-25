/*
 * File service: maps the DOS program's paths onto the host.
 *
 *  - Reads resolve against the save directory first (files the port wrote: saves, config), then against the
 *    player's original game-data directory (never written).
 *  - Writes always go to the save directory.
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

/* Resolve a DOS path. for_write = 0: an existing file (save dir, then data dir); returns 0 if none exists.
   for_write = 1: the save-dir path (parent directories created). Returns 1 on success. */
int port_files_resolve(const char *dos_path, int for_write, char *out, size_t out_size);

const char *port_files_data_dir(void);
const char *port_files_save_dir(void);

#ifdef __cplusplus
}
#endif

#endif
