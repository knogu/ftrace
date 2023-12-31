#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "test.h"

__attribute__((no_instrument_function))
uintptr_t get_load_addr(char* path) {
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
            fclose(maps);
            return start;
        }
    }
}

void __attribute__((no_instrument_function))
dump_value(Type typ, void* abs_addr) { // abs_addr is the address at which the dumped value is stored
    printf("actual value: ");
    if (strcmp(typ.name, "int") == 0) {
        printf("%d", *((int*) abs_addr));
    } else if (strcmp(typ.name, "long long int") == 0) {
        printf("%lld", *((long long*) abs_addr));
    } else if (strcmp(typ.name, "long int") == 0) {
        printf("%ld", *((long*) abs_addr));
    } else if (strcmp(typ.name, "char") == 0) {
        printf("%s", (char*) abs_addr);
    } else if (strcmp(typ.name, "Ptr") == 0) {
        void** ptr_to_ptr = (void**) abs_addr; // E.g., When Ptr[int], this is a pointer to "pointer to int"
        void* ptr = *ptr_to_ptr; // E.g., When Ptr[int], this is a pointer to int
        printf("%p -> ", (void*)ptr);
        dump_value(*typ.pointed, ptr);
    } else if (strcmp(typ.name, "_Bool") == 0) {
        printf("%d", *((_Bool*)abs_addr));
    } else if (typ.struct_first_field) {
        printf("struct name: %s", typ.name);
        Type* field = typ.struct_first_field;
        while (field) {
            printf("\n");
            printf("field type: %s, ", field->name);
            printf("field name: %s, ", field->field_name);
            dump_value(*field, abs_addr + field->offset);
            field = field->struct_next_field;
        }
    } else {
        printf("%s is not yet to be supported", typ.name);
    }
}

void __attribute__((no_instrument_function))
dump_func(void* map, uintptr_t func_addr, void* frame_addr) {
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
        printf("bytes_cnt: %lu\n", arg.bytes_cnt);
        printf("location: %ld\n", arg.location);
        printf("typ.name: %s\n", arg.typ.name);
        // dinfoで得られるoffset の値 + 16、ということ？
        // https://qiita.com/mhiramat/items/8df17f5113434e93ff0c
        void *abs_addr = frame_addr + 16 + arg.location;
        dump_value(arg.typ, abs_addr);

        printf("\n\n");
    }
}

int called = 0;
uintptr_t load_addr;
char path[256];
void *addr2func;

void __attribute__((no_instrument_function))
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
    if (called == 0) {
        called = 1;

        // Get the filename of the executable
        char exe[256];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path)-1);
        if (len != -1) {
            path[len] = '\0';
        }
        strncpy(exe, basename(path), sizeof(exe));

        load_addr = get_load_addr(path);
        addr2func = get_addr2func(path);
    }

    dump_func(addr2func, (uintptr_t) this_fn - load_addr, __builtin_frame_address(1));
}

void __attribute__((no_instrument_function))
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
//    printf("[-]\n");
}

