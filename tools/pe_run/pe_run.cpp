/* pe_run: runs a 32-bit Windows console PE (the Watcom BINNT stubs) over win32_layer, no Wine.
 * W32L_PE = the executable; argv[1..] = its command line. */
#include "platform.h"

#include <asm/ldt.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef MAP_FIXED_NOREPLACE
#define MAP_FIXED_NOREPLACE 0x100000
#endif

/* GetModuleFileNameA answers the loaded PE's own path as C:\<native path>; with no W32L_DRIVE_C the drive letter drops. */
static char module_path[4096] = "C:\\pe_run\\pe_run.exe";
extern "C" const W32L_AppInfo w32l_app_info = {"pe_run", "1", "org.retrofoundry.pe_run", module_path};

namespace {

struct FileHeader { uint16_t Machine, NumberOfSections; uint32_t Time, SymTab, NumSyms; uint16_t OptSize, Chars; };
struct DataDir { uint32_t va, size; };
struct OptHeader {
    uint16_t Magic; uint8_t MajL, MinL;
    uint32_t SizeOfCode, SizeOfInit, SizeOfUninit, Entry, BaseOfCode, BaseOfData, ImageBase;
    uint32_t SectAlign, FileAlign;
    uint16_t v[6];
    uint32_t Win32Ver, SizeOfImage, SizeOfHeaders, CheckSum;
    uint16_t Subsystem, DllChars;
    uint32_t stack[2], heap[2], LoaderFlags, NumDirs;
    DataDir dir[16];
};
struct Section {
    char Name[8];
    uint32_t VirtualSize, VirtualAddress, RawSize, RawPtr, Relocs, Lines;
    uint16_t NumRelocs, NumLines;
    uint32_t Chars;
};
struct Import { uint32_t OrigFirstThunk, Time, Forwarder, Name, FirstThunk; };

void die(const char *fmt, ...) __attribute__((noreturn, format(printf, 1, 2)));
void die(const char *fmt, ...)
{
    va_list ap;
    fputs("pe_run: ", stderr);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(1);
}

/* fs:[0] is the SEH chain head; fs:[0x18] the TEB itself. */
void teb_attach(void)
{
    static __thread int done;
    uint32_t *teb;
    struct user_desc d;

    if (done)
        return;
    done = 1;
    teb = (uint32_t *)calloc(1024, sizeof *teb);
    if (!teb)
        die("no memory for a TEB");
    teb[0] = 0xFFFFFFFFu;
    teb[6] = (uint32_t)(uintptr_t)teb;
    memset(&d, 0, sizeof d);
    d.entry_number = (unsigned)-1;
    d.base_addr = (unsigned)(uintptr_t)teb;
    d.limit = 0xFFFFF;
    d.seg_32bit = 1;
    d.limit_in_pages = 1;
    d.useable = 1;
    if (syscall(SYS_set_thread_area, &d) != 0)
        die("set_thread_area: %s", strerror(errno));
    uint16_t sel = (uint16_t)(d.entry_number * 8 + 3);
    __asm__ volatile("mov %0, %%fs" : : "r"(sel));
}

__attribute__((constructor)) void init(void)
{
    teb_attach();
    w32l_host_environment = 1;
    w32l_thread_attach = teb_attach;
    const char *pe = getenv("W32L_PE");
    if (pe && pe[0] == '/') {
        size_t n = snprintf(module_path, sizeof module_path, "C:%s", pe);
        for (char *c = module_path; c < module_path + n; c++)
            if (*c == '/')
                *c = '\\';
    }
}

[[noreturn]] void missing(const char *what) { die("the program called %s, which win32_layer does not provide", what); }

const void *make_stub(const char *module, const char *name)
{
    static uint8_t *page;
    static size_t used;
    char *what = (char *)malloc(strlen(module) + strlen(name) + 2);
    if (!page) {
        page = (uint8_t *)mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (page == MAP_FAILED)
            die("cannot map the import stubs");
    }
    if (!what || used + 16 > 4096)
        die("too many missing imports");
    sprintf(what, "%s!%s", module, name);
    uint8_t *s = page + used;
    used += 16;
    s[0] = 0x68;
    memcpy(s + 1, &what, 4);
    s[5] = 0xE8;
    uint32_t rel = (uint32_t)((uintptr_t)&missing - (uintptr_t)(s + 10));
    memcpy(s + 6, &rel, 4);
    s[10] = 0xCC;
    return s;
}

unsigned bind_imports(uint8_t *image, const DataDir &dir)
{
    unsigned nmissing = 0;
    for (const Import *imp = (const Import *)(image + dir.va); dir.va && imp->Name; imp++) {
        const char *module = (const char *)(image + imp->Name);
        const uint32_t *names = (const uint32_t *)(image + (imp->OrigFirstThunk ? imp->OrigFirstThunk : imp->FirstThunk));
        uint32_t *slots = (uint32_t *)(image + imp->FirstThunk);
        for (unsigned i = 0; names[i]; i++) {
            char ord[16];
            const char *name;
            if (names[i] & 0x80000000u) {
                snprintf(ord, sizeof ord, "#%u", names[i] & 0xFFFFu);
                name = ord;
            } else {
                name = (const char *)(image + names[i] + 2);
            }
            const void *fn = w32l_export_lookup(module, name);
            if (!fn) {
                fprintf(stderr, "pe_run: import %s!%s is not provided\n", module, name);
                fn = make_stub(module, name);
                nmissing++;
            }
            slots[i] = (uint32_t)(uintptr_t)fn;
        }
    }
    return nmissing;
}

} // namespace

extern "C" int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdline, int32_t);
extern "C" int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdline, int32_t)
{
    const char *path = getenv("W32L_PE");
    if (!path)
        die("W32L_PE is not set");
    FILE *f = fopen(path, "rb");
    if (!f)
        die("cannot open %s", path);
    fseek(f, 0, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *file = (uint8_t *)malloc(size);
    if (!file || fread(file, 1, size, f) != size)
        die("cannot read %s", path);
    fclose(f);

    uint32_t lfanew;
    if (size < 0x40 || file[0] != 'M' || file[1] != 'Z')
        die("%s is not a PE image", path);
    memcpy(&lfanew, file + 0x3C, 4);
    if (lfanew + 4 + sizeof(FileHeader) + sizeof(OptHeader) > size || memcmp(file + lfanew, "PE\0\0", 4))
        die("%s has no PE header", path);
    const FileHeader *fh = (const FileHeader *)(file + lfanew + 4);
    const OptHeader *oh = (const OptHeader *)(fh + 1);
    if (fh->Machine != 0x14C || oh->Magic != 0x10B)
        die("%s is not an i386 PE32 image", path);

    uint8_t *image = (uint8_t *)mmap((void *)(uintptr_t)oh->ImageBase, oh->SizeOfImage, PROT_READ | PROT_WRITE | PROT_EXEC,
                                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (image == MAP_FAILED || image != (uint8_t *)(uintptr_t)oh->ImageBase)
        die("cannot map the image at 0x%08x: %s", (unsigned)oh->ImageBase, strerror(errno));
    memcpy(image, file, oh->SizeOfHeaders < size ? oh->SizeOfHeaders : size);
    const Section *sec = (const Section *)((const uint8_t *)oh + fh->OptSize);
    for (unsigned i = 0; i < fh->NumberOfSections; i++) {
        uint32_t n = sec[i].RawPtr ? sec[i].RawSize : 0;
        if (sec[i].RawPtr + n > size || sec[i].VirtualAddress + n > oh->SizeOfImage)
            die("section %.8s is out of range", sec[i].Name);
        memcpy(image + sec[i].VirtualAddress, file + sec[i].RawPtr, n);
    }
    void (*entry)(void) = (void (*)(void))(uintptr_t)(oh->ImageBase + oh->Entry);
    if (bind_imports(image, oh->dir[1]))
        fprintf(stderr, "pe_run: continuing with missing imports stubbed\n");
    free(file);
    HeapFree(GetProcessHeap(), 0, HeapAlloc(GetProcessHeap(), 0, 16)); /* maps the fixed heap */
    /* The stub copies the compiler payload into LocalAlloc memory and runs it there. */
    if (mprotect(w32l_fixed_heap_base(), w32l_fixed_heap_limit(), PROT_READ | PROT_WRITE | PROT_EXEC))
        die("cannot make the heap executable: %s", strerror(errno));
    (void)cmdline;
    entry();
    return 0;
}
