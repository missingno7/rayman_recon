#include "platform/files.h"
#include "platform/cdrom.h"

#include <errno.h>
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

static char data_dir[1024];
static char save_dir[1024];

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
    snprintf(data_dir, sizeof data_dir, "%s", data);
    snprintf(save_dir, sizeof save_dir, "%s", save);
    make_dirs(save_dir);
    return is_dir(data_dir) && is_dir(save_dir);
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

int port_files_resolve(const char *dos_path, int for_write, char *out, size_t out_size)
{
    char rel[512];
    /* Detect both D:\absolute and D:relative before normalization (the CD save probe uses the latter). */
    if (port_cd_path(dos_path)) {
        if (for_write) { errno = EROFS; return 0; }
        if (!port_cd_present()) { errno = ENOENT; return 0; }
        normalise(dos_path, rel, sizeof rel);
        /* Installed-data fallback: CD reads never consult the writable save overlay. */
        return find_ci(data_dir, rel, out, out_size);
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
    return find_ci(save_dir, rel, out, out_size) || find_ci(data_dir, rel, out, out_size);
}
