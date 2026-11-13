#include "platform/files.h"
#include "platform/cdrom.h"

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define port_mkdir(p) _mkdir(p)
#else
#include <dirent.h>
#include <strings.h>
#define port_mkdir(p) mkdir((p), 0777)
#endif

static char data_dirs[32][1024];
static size_t data_count;
#define data_dir data_dirs[0]
static char save_dir[1024];
static int cd_movies = 1;   /* movie_from_cd; --no-cd-movies */

static int is_dir(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR;
}

static int is_file(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0 && (st.st_mode & S_IFMT) == S_IFREG;
}

static void make_dirs(const char *path)
{
    char tmp[1024];
    size_t i;
    snprintf(tmp, sizeof tmp, "%s", path);
    for (i = 1; tmp[i]; i++) {
        if (tmp[i] == '/') {
            tmp[i] = 0;
            if (!is_dir(tmp))
                port_mkdir(tmp);
            tmp[i] = '/';
        }
    }
    if (!is_dir(tmp))
        port_mkdir(tmp);
}

int port_files_init(const char *data, const char *save)
{
    data_count = 0;
    if (!port_files_add_data(data)) return 0;
    snprintf(save_dir, sizeof save_dir, "%s", save);
    make_dirs(save_dir);
    return is_dir(data_dir) && is_dir(save_dir);
}

int port_files_add_data(const char *data)
{
    if (!data || data_count == 32 || strlen(data) >= sizeof data_dirs[0] || !is_dir(data))
        return 0;
    snprintf(data_dirs[data_count++], sizeof data_dirs[0], "%s", data);
    return 1;
}

const char *port_files_data_dir(void) { return data_dir; }
const char *port_files_save_dir(void) { return save_dir; }

/* DOS path -> relative path with '/' separators: drive and "..\" prefixes removed. */
static void normalise(const char *dos, char *rel, size_t n)
{
    size_t o = 0;
    if (dos[0] && dos[1] == ':')
        dos += 2;
    while (*dos == '\\' || *dos == '/')
        dos++;
    while ((dos[0] == '.' && dos[1] == '.' && (dos[2] == '\\' || dos[2] == '/')))
        dos += 3;
    for (; *dos && o + 1 < n; dos++)
        rel[o++] = *dos == '\\' ? '/' : *dos;
    rel[o] = 0;
}

/* base/rel with each component matched case-insensitively (case-sensitive hosts). */
static int find_ci(const char *base, const char *rel, char *out, size_t n)
{
#ifdef _WIN32
    snprintf(out, n, "%s/%s", base, rel);
    return is_file(out);
#else
    char cur[1024], part[256];
    const char *p = rel;
    snprintf(cur, sizeof cur, "%s", base);
    while (*p) {
        size_t k = 0;
        DIR *d;
        struct dirent *e;
        int found = 0;
        while (*p && *p != '/' && k + 1 < sizeof part)
            part[k++] = *p++;
        part[k] = 0;
        while (*p == '/')
            p++;
        if (!(d = opendir(cur)))
            return 0;
        while ((e = readdir(d))) {
            if (strcasecmp(e->d_name, part) == 0) {
                size_t l = strlen(cur);
                snprintf(cur + l, sizeof cur - l, "/%s", e->d_name);
                found = 1;
                break;
            }
        }
        closedir(d);
        if (!found)
            return 0;
    }
    snprintf(out, n, "%s", cur);
    return is_file(out);
#endif
}

static int find_data(const char *rel, char *out, size_t n)
{
    size_t i;
    for (i = 0; i < data_count; ++i)
        if (find_ci(data_dirs[i], rel, out, n)) return 1;
    return 0;
}

/* The movies (INTRO.DAT, CONCLU.DAT) from the inserted Rayman CD, when the game folder has none (user decision
   2026-11-13). The original reads them only from its install folder, where its installer could copy them from the
   CD's RAYMAN folder; the port looks there itself: a disc in a host drive (a real CD or a mounted image) is read
   in place, a CUE/BIN image's file is copied once into the save folder's cd-cache, as the installer would have
   done, so every file call of the game works on it unchanged. Only these two files: the movies play with every
   version's game data, while the CD's other files belong to the CD's own version. */
static int movie_from_cd(const char *rel, char *out, size_t n)
{
    static const char *const movies[] = {"INTRO.DAT", "CONCLU.DAT"};
    const char *name = rel;
    char cache[1024];
    uint32_t lba, size;
    size_t i;
    struct stat st;
    char letter;
    if (name[0] == '.' && name[1] == '/')
        name += 2;
    for (i = 0; i < sizeof movies / sizeof movies[0]; i++) {
        size_t k;
        for (k = 0; movies[i][k] && name[k] && toupper((unsigned char)name[k]) == movies[i][k]; k++) {}
        if (!movies[i][k] && !name[k])
            break;
    }
    if (!cd_movies || i == sizeof movies / sizeof movies[0] || !port_cd_present())
        return 0;
    if ((letter = port_cd_host_drive()) != 0) {
        char root[4] = {letter, ':', '/', 0}, path[64];
        snprintf(path, sizeof path, "RAYMAN/%s", movies[i]);
        return find_ci(root, path, out, n);
    }
    if (!port_cd_file("RAYMAN", movies[i], &lba, &size))
        return 0;
    snprintf(cache, sizeof cache, "%s/cd-cache", save_dir);
    make_dirs(cache);
    snprintf(out, n, "%s/%s", cache, movies[i]);
    if (stat(out, &st) == 0 && (uint32_t)st.st_size == size)
        return 1;
    return port_cd_copy_file(lba, size, out);
}

void port_files_set_cd_movies(int on) { cd_movies = on; }

int port_files_resolve(const char *dos_path, int for_write, char *out, size_t out_size)
{
    char rel[512];
    /* Detect both D:\absolute and D:relative before normalization (the CD save probe uses the latter). */
    if (port_cd_path(dos_path)) {
        if (for_write) { errno = EROFS; return 0; }
        if (!port_cd_present()) { errno = ENOENT; return 0; }
        normalise(dos_path, rel, sizeof rel);
        /* Installed-data fallback: search player-selected roots, never the save overlay or CD image. */
        return find_data(rel, out, out_size);
    }
    normalise(dos_path, rel, sizeof rel);
    if (for_write) {
        char dir[1024];
        char *slash;
        if (find_ci(save_dir, rel, out, out_size))
            return 1;
        snprintf(out, out_size, "%s/%s", save_dir, rel);
        snprintf(dir, sizeof dir, "%s", out);
        slash = strrchr(dir, '/');
        if (slash) {
            *slash = 0;
            make_dirs(dir);
        }
        return 1;
    }
    return find_ci(save_dir, rel, out, out_size) || find_data(rel, out, out_size) ||
           movie_from_cd(rel, out, out_size);
}
