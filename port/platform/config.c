/* Player settings file (platform/config.h): a flat JSON object. Unknown settings are ignored, so files written
   by a later version still load. */
#include "platform/config.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void port_config_defaults(port_config *c)
{
    memset(c, 0, sizeof *c);
    strcpy(c->cd, "auto");
    c->vsync = 1;
}

static void skip_space(const char **p)
{
    while (isspace((unsigned char)**p)) ++*p;
}

/* A JSON string into out (truncation is an error). Unknown escapes keep their backslash, so a Windows path typed
   with single backslashes ("D:\Games") still reads as intended unless it forms a real escape. */
static int parse_string(const char **p, char *out, size_t size)
{
    size_t n = 0;
    const char *s = *p;
    if (*s++ != '"') return 0;
    while (*s != '"') {
        char ch = *s++;
        if (!ch) return 0;
        if (ch == '\\') {
            ch = *s++;
            switch (ch) {
            case '"': case '\\': case '/': break;
            case 'b': ch = '\b'; break;
            case 'f': ch = '\f'; break;
            case 'n': ch = '\n'; break;
            case 'r': ch = '\r'; break;
            case 't': ch = '\t'; break;
            case 0: return 0;
            default:
                if (out) { if (n + 1 >= size) return 0; out[n] = '\\'; }
                n++;
            }
        }
        if (out) { if (n + 1 >= size) return 0; out[n] = ch; }
        n++;
    }
    if (out) out[n] = 0;
    *p = s + 1;
    return 1;
}

static int skip_value(const char **p, int depth)
{
    skip_space(p);
    if (depth > 32) return 0;
    if (**p == '"') return parse_string(p, NULL, 0);
    if (**p == '{' || **p == '[') {
        char close = **p == '{' ? '}' : ']';
        int object = **p == '{';
        ++*p;
        skip_space(p);
        if (**p == close) { ++*p; return 1; }
        for (;;) {
            if (object) {
                skip_space(p);
                if (!parse_string(p, NULL, 0)) return 0;
                skip_space(p);
                if (*(*p)++ != ':') return 0;
            }
            if (!skip_value(p, depth + 1)) return 0;
            skip_space(p);
            if (**p == ',') { ++*p; continue; }
            if (**p == close) { ++*p; return 1; }
            return 0;
        }
    }
    if (!strncmp(*p, "true", 4)) { *p += 4; return 1; }
    if (!strncmp(*p, "false", 5)) { *p += 5; return 1; }
    if (!strncmp(*p, "null", 4)) { *p += 4; return 1; }
    if (**p == '-' || isdigit((unsigned char)**p)) {
        char *end;
        strtod(*p, &end);
        if (end == *p) return 0;
        *p = end;
        return 1;
    }
    return 0;
}

static int parse_bool(const char **p, int *out)
{
    if (!strncmp(*p, "true", 4)) { *p += 4; *out = 1; return 1; }
    if (!strncmp(*p, "false", 5)) { *p += 5; *out = 0; return 1; }
    return 0;
}

static int parse_int(const char **p, int *out)
{
    char *end;
    long v = strtol(*p, &end, 10);
    if (end == *p || v < -100000 || v > 100000) return 0;
    *p = end;
    *out = (int)v;
    return 1;
}

static int parse_dirs(const char **p, port_config *c)
{
    c->extra_data_dir_count = 0;
    if (*(*p)++ != '[') return 0;
    skip_space(p);
    if (**p == ']') { ++*p; return 1; }
    for (;;) {
        skip_space(p);
        if (c->extra_data_dir_count == PORT_CONFIG_EXTRA_DIRS) return 0;
        if (!parse_string(p, c->extra_data_dirs[c->extra_data_dir_count], sizeof c->extra_data_dirs[0])) return 0;
        c->extra_data_dir_count++;
        skip_space(p);
        if (**p == ',') { ++*p; continue; }
        if (**p == ']') { ++*p; return 1; }
        return 0;
    }
}

static int parse(const char *text, port_config *c, unsigned *seen)
{
    static const char *const keys[] = {"widescreen", "fullscreen", "window_scale", "cd", "save_dir", "extra_data_dirs",
                                        "vsync"};
    const char *p = text;
    char key[64];
    unsigned k;
    *seen = 0;
    if ((unsigned char)p[0] == 0xEF && (unsigned char)p[1] == 0xBB && (unsigned char)p[2] == 0xBF) p += 3;
    skip_space(&p);
    if (*p++ != '{') return 0;
    skip_space(&p);
    if (*p != '}') for (;;) {
        int ok;
        skip_space(&p);
        if (!parse_string(&p, key, sizeof key)) return 0;
        skip_space(&p);
        if (*p++ != ':') return 0;
        skip_space(&p);
        for (k = 0; k < sizeof keys / sizeof keys[0] && strcmp(key, keys[k]); k++) {}
        switch (k) {
        case 0: ok = parse_bool(&p, &c->widescreen); break;
        case 1: ok = parse_bool(&p, &c->fullscreen); break;
        case 2: ok = parse_int(&p, &c->window_scale) && c->window_scale >= 0 && c->window_scale <= 64; break;
        case 3: ok = parse_string(&p, c->cd, sizeof c->cd); break;
        case 4: ok = parse_string(&p, c->save_dir, sizeof c->save_dir); break;
        case 5: ok = parse_dirs(&p, c); break;
        case 6: ok = parse_bool(&p, &c->vsync); break;
        default: ok = skip_value(&p, 0); break;
        }
        if (!ok) return 0;
        if (k < 32) *seen |= 1u << k;
        skip_space(&p);
        if (*p == ',') { ++p; continue; }
        if (*p == '}') break;
        return 0;
    }
    ++p;
    skip_space(&p);
    if (*p) return 0;
    *seen &= (1u << (sizeof keys / sizeof keys[0])) - 1;
    return *seen == (1u << (sizeof keys / sizeof keys[0])) - 1 ? 1 : 2;
}

int port_config_load(const char *path, port_config *config)
{
    FILE *f = fopen(path, "rb");
    char *text;
    long size;
    unsigned seen;
    int result;
    port_config loaded;
    if (!f) return 0;
    if (fseek(f, 0, SEEK_END) || (size = ftell(f)) < 0 || size > 1 << 20 || fseek(f, 0, SEEK_SET) ||
        !(text = malloc((size_t)size + 1))) {
        fclose(f);
        port_config_defaults(config);
        return -1;
    }
    size = (long)fread(text, 1, (size_t)size, f);
    fclose(f);
    text[size] = 0;
    port_config_defaults(&loaded);
    result = (long)strlen(text) == size ? parse(text, &loaded, &seen) : 0;
    free(text);
    if (!result) {
        port_config_defaults(config);
        return -1;
    }
    *config = loaded;
    return result;
}

static void write_string(FILE *f, const char *s)
{
    fputc('"', f);
    for (; *s; s++) {
        if (*s == '"' || *s == '\\') fputc('\\', f);
        if (*s == '\n') fputs("\\n", f);
        else if (*s == '\r') fputs("\\r", f);
        else if (*s == '\t') fputs("\\t", f);
        else fputc(*s, f);
    }
    fputc('"', f);
}

int port_config_save(const char *path, const port_config *c)
{
    FILE *f = fopen(path, "wb");
    int i, ok;
    if (!f) return 0;
    fprintf(f, "{\n  \"widescreen\": %s,\n  \"fullscreen\": %s,\n  \"vsync\": %s,\n  \"window_scale\": %d,\n  \"cd\": ",
            c->widescreen ? "true" : "false", c->fullscreen ? "true" : "false", c->vsync ? "true" : "false",
            c->window_scale);
    write_string(f, c->cd);
    fputs(",\n  \"save_dir\": ", f);
    write_string(f, c->save_dir);
    fputs(",\n  \"extra_data_dirs\": [", f);
    for (i = 0; i < c->extra_data_dir_count; i++) {
        if (i) fputs(", ", f);
        write_string(f, c->extra_data_dirs[i]);
    }
    fputs("]\n}\n", f);
    ok = !ferror(f);
    return fclose(f) == 0 && ok;
}
