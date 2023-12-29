#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <elf.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>

#include "test.h"

#define HASH_SIZE 100

typedef struct Node {
    uintptr_t key;
    char* value;
    struct Node* next;
} Node;

typedef struct HashTable {
    Node* nodes[HASH_SIZE];
} HashTable;

#define ELF_ST_TYPE(i) ((i) & 0xf)
#define ELF_R_SYM(i) ((i) >> 32)

HashTable *hashtable = NULL;
HashTable *hashtable_without_base = NULL;

__attribute__((no_instrument_function))
unsigned int hash(uintptr_t key) {
    return key % HASH_SIZE;
}

__attribute__((no_instrument_function))
HashTable* create_hashtable() {
    HashTable* hashtable = calloc(1, sizeof(HashTable));
    if (hashtable == NULL) {
        printf("Failed to allocate memory for hashtable\n");
        exit(1);
    }
    return hashtable;
}

__attribute__((no_instrument_function))
void destroy_hashtable(HashTable* hashtable) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Node* node = hashtable->nodes[i];
        while (node != NULL) {
            Node* next = node->next;
            free(node->value);
            free(node);
            node = next;
        }
    }
    free(hashtable);
}

__attribute__((no_instrument_function))
void insert(HashTable* hashtable, uintptr_t key, char* value) {
    unsigned int index = hash(key);
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = strdup(value);
    node->next = hashtable->nodes[index];
    hashtable->nodes[index] = node;
}

__attribute__((no_instrument_function))
char* find(HashTable* hashtable, uintptr_t key) {
    unsigned int index = hash(key);
    Node* node = hashtable->nodes[index];
    while (node != NULL) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

Elf64_Addr main_addr = 0;
uintptr_t load_addr;

__attribute__((no_instrument_function))
static int elfdump(char *head) {
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shdr, *shstr, *str, *sym, *rel;
    Elf64_Phdr *phdr;
    Elf64_Sym *symp;
    Elf64_Rel *relp;

    int i, j, size;
    char *sname;

    ehdr = (Elf64_Ehdr *)head;

    /* セクション名格納用セクション(.shstrtab)の取得*/
    shstr = (Elf64_Shdr *) (head + ehdr->e_shoff + ehdr->e_shentsize * ehdr->e_shstrndx);

    /* セクション名一覧の表示 */
    for (i = 0; i < ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(head + ehdr->e_shoff + ehdr->e_shentsize * i);
        sname = (char *)(head + shstr->sh_offset + shdr->sh_name);
//        printf("\t[%d]\t%s\n", i, sname);
        if (!strcmp(sname, ".strtab")) str = shdr;
    }

//    printf("Segments:\n");
//    for (i = 0; i < ehdr->e_phnum; i++) {
//        phdr = (Elf64_Phdr *)(head + ehdr->e_phoff + ehdr->e_phentsize * i);
//        printf("\t[%d]\t", i);
//        for (j = 0; j < ehdr->e_shnum; j++) {
//            shdr = (Elf64_Shdr *)(head + ehdr->e_shoff + ehdr->e_shentsize * j);
//            size = (shdr->sh_type != SHT_NOBITS) ? shdr->sh_size : 0;
//            if (shdr->sh_offset < phdr->p_offset) continue;
//            if (shdr->sh_offset + size > phdr->p_offset + phdr->p_filesz) continue;
//            sname = (char *)(head + shstr->sh_offset + shdr->sh_name);
//            printf("%s ", sname);
//        }
//        printf("\n");
//    }

    for (i = 0; i < ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(head + ehdr->e_shoff + ehdr->e_shentsize * i);
        if (shdr->sh_type != SHT_SYMTAB) continue;
        sym = shdr;
        for (j = 0; j < sym->sh_size / sym->sh_entsize; j++) {
            symp = (Elf64_Sym *) (head + sym->sh_offset + sym->sh_entsize * j);
            if (!symp->st_name) continue;
            char *symname = (char*)(head + str->sh_offset + symp->st_name);
            insert(hashtable, symp->st_value + load_addr, symname);
            insert(hashtable_without_base, symp->st_value, symname);

//            printf("\t[%d]\t%d\t%d\t%s\n", j, (int)ELF_ST_TYPE(symp->st_info), symp->st_size, (char*)(head + str->sh_offset + symp->st_name));
        }
    }

//    printf("Relocations:\n");
//    for (i = 0; i < ehdr->e_shnum; i++) {
//        shdr = (Elf64_Shdr *) (head + ehdr->e_shoff + ehdr->e_shentsize * i);
//        if ((shdr->sh_type != SHT_REL) && (shdr->sh_type != SHT_RELA)) continue;
//        rel = shdr;
//        for (j = 0; j < rel->sh_size / rel->sh_entsize; j++) {
//            relp = (Elf64_Rel *)(head + rel->sh_offset + rel->sh_entsize * j);
//            symp = (Elf64_Sym *)(head + sym->sh_offset + (sym->sh_entsize * ELF_R_SYM(relp->r_info)));
//            if (!symp->st_name) continue;
//            printf("\t[%d]\t%d\t%s\n", j, ELF_R_SYM(relp->r_info), (char *)(head + str->sh_offset + symp->st_name));
//        }
//    }

    return 0;
}

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

//void __attribute__((no_instrument_function))
//dump(void* map, char *name) {
//    size_t len = get_arg_count(map, name);
//    printf("len: %d\n", len);
//
//    for (int i = 0; i < len; ++i) {
//        Arg arg = get_ith_arg(map, name, i);
//        if (arg.is_arg) {
//            printf("=== %d th arg ===\n", i+1);
//        } else {
//            printf("=== %d th member ===\n", i+1);
//        }
//
//        printf("name: %s\n", arg.name);
//        printf("bytes_cnt: %llu\n", arg.bytes_cnt);
//        printf("location: %lld\n", arg.location);
//        printf("type_name: %s\n", arg.type_name);
//        printf("\n");
//    }
//}

//void dump_func(void* map, uintptr_t func_addr) {
//    printf("funcname: %s\n", get_funcname(map, func_addr));
//    size_t len = get_arg_cnt_from_func_addr(map, func_addr);
//
//    for (int i = 0; i < len; ++i) {
//        Arg arg = get_ith_arg_from_func_addr(map, func_addr, i);
//        if (arg.is_arg) {
//            printf("=== %d th arg ===\n", i+1);
//        } else {
//            printf("=== %d th member ===\n", i+1);
//        }
//
//        printf("name: %s\n", arg.name);
//        printf("bytes_cnt: %llu\n", arg.bytes_cnt);
//        printf("location: %lld\n", arg.location);
//        printf("type_name: %s\n", arg.type_name);
//        printf("\n");
//    }
//}

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
    printf("[+]\n");
    struct stat sb;
    char *head;
    void* map;
    void *frame = __builtin_frame_address(1);
    void *frame5 = frame - 4;
    printf("a=%d\n", *(int*)frame5);
    // dinfoで得られるoffset の値 + 16、ということ？
    // https://qiita.com/mhiramat/items/8df17f5113434e93ff0c

//    map = get_func2args("/home/jp31281/ftrace/test");

    if (called == 0) {
        called = 1;
        hashtable = create_hashtable();
        hashtable_without_base = create_hashtable();
        // Get the filename of the executable

        char path[256], exe[256];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path)-1);
        if (len != -1) {
            path[len] = '\0';
        }
        strncpy(exe, basename(path), sizeof(exe));
//        map = get_func2args(path);
//        dump(map, "callee");

        get_load_addr(path);


        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            printf("%d\n", fd);
        };
        fstat(fd, &sb);
        head = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

        elfdump(head);
    }

    void *addr2func = get_addr2func("/home/jp31281/ftrace/test");
    dump_func(addr2func, (uintptr_t) this_fn - load_addr);

    uintptr_t ptr = (uintptr_t) this_fn;
    char *fname;
    fname = find(hashtable, ptr);
    if (fname) {
        printf("%s got called\n", fname);
//        dump(map, fname);
    }
//    fname = find(hashtable_without_base, ptr);
//    if (fname) {
//        printf("%s is called\n", fname);
//        dump(map, fname);
//    }
}

void __attribute__((no_instrument_function))
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
//    printf("[-]\n");
}

