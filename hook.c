#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "test.h"

uintptr_t load_addr;

int called = 0;

__attribute__((no_instrument_function))
int get_load_addr(char* path) {
    FILE *maps = fopen("/proc/self/maps", "r");
    if (maps == NULL) {
        perror("fopen");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), maps) != NULL) {
        uintptr_t start, end;
        char perms[5], offset[9], dev[6], inode[11], pathname[256] = "";
        sscanf(line, "%lx-%lx %4s %8s %5s %10s %[^\n]", &start, &end, perms, offset, dev, inode, pathname);

        // If pathname is not empty and it contains the path of the executable
        if (*pathname && strstr(pathname, path)) {
            load_addr = start;
            break;
        }
    }

    fclose(maps);
}


void __attribute__((no_instrument_function))
dump_func(void* map, uintptr_t func_addr) {
    printf("funcname: %s\n", get_funcname(map, func_addr));
    size_t len = get_arg_cnt_from_func_addr(map, func_addr);

    for (int i = 0; i < len; ++i) {
        Arg arg = get_ith_arg_from_func_addr(map, func_addr, i);
        if (arg.is_arg) {
            printf("=== %d th arg ===\n", i+1);
        } else {
            printf("=== %d th member ===\n", i+1);
        }

        printf("name: %s\n", arg.name);
        printf("bytes_cnt: %llu\n", arg.bytes_cnt);
        printf("location: %lld\n", arg.location);
        printf("type_name: %s\n", arg.type_name);
        printf("\n");
    }
}

void __attribute__((no_instrument_function))
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
    void *frame = __builtin_frame_address(1);
    void *frame5 = frame - 4;
    printf("a=%d\n", *(int*)frame5);
    // dinfoで得られるoffset の値 + 16、ということ？
    // https://qiita.com/mhiramat/items/8df17f5113434e93ff0c

    if (called == 0) {
        called = 1;

        // Get the filename of the executable
        char path[256], exe[256];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path)-1);
        if (len != -1) {
            path[len] = '\0';
        }
        strncpy(exe, basename(path), sizeof(exe));

        get_load_addr(path);
    }
    void *addr2func = get_addr2func("/home/jp31281/ftrace/test");
    dump_func(addr2func, (uintptr_t) this_fn - load_addr);
}

void __attribute__((no_instrument_function))
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
//    printf("[-]\n");
}

