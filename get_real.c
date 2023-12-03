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

#define ELF_ST_TYPE(i) ((i) & 0xf)
#define ELF_R_SYM(i) ((i) >> 32)

Elf64_Addr main_addr = 0;

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
        printf("\t[%d]\t%s\n", i, sname);
        if (!strcmp(sname, ".strtab")) str = shdr;
    }

    printf("Segments:\n");
    for (i = 0; i < ehdr->e_phnum; i++) {
        phdr = (Elf64_Phdr *)(head + ehdr->e_phoff + ehdr->e_phentsize * i);
        printf("\t[%d]\t", i);
        for (j = 0; j < ehdr->e_shnum; j++) {
            shdr = (Elf64_Shdr *)(head + ehdr->e_shoff + ehdr->e_shentsize * j);
            size = (shdr->sh_type != SHT_NOBITS) ? shdr->sh_size : 0;
            if (shdr->sh_offset < phdr->p_offset) continue;
            if (shdr->sh_offset + size > phdr->p_offset + phdr->p_filesz) continue;
            sname = (char *)(head + shstr->sh_offset + shdr->sh_name);
            printf("%s ", sname);
        }
        printf("\n");
    }

    printf("Symbols:\n");
    for (i = 0; i < ehdr->e_shnum; i++) {
        shdr = (Elf64_Shdr *)(head + ehdr->e_shoff + ehdr->e_shentsize * i);
        if (shdr->sh_type != SHT_SYMTAB) continue;
        sym = shdr;
        for (j = 0; j < sym->sh_size / sym->sh_entsize; j++) {
            symp = (Elf64_Sym *) (head + sym->sh_offset + sym->sh_entsize * j);
            if (!symp->st_name) continue;
            char *symname = (char*)(head + str->sh_offset + symp->st_name);
            printf("symname: %s\n", symname);
            if (strcmp(symname, "main") == 0) {
                printf("address found!!!\n");
                printf("Runtime address of the function: %lx\n", symp->st_value);
                main_addr = symp->st_value;
            }
            printf("\t[%d]\t%d\t%d\t%s\n", j, (int)ELF_ST_TYPE(symp->st_info), symp->st_size, (char*)(head + str->sh_offset + symp->st_name));
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

void __attribute__((no_instrument_function))
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
    printf("enter: %p\n", (int *)this_fn);
    printf("[+]\n");
}

void __attribute__((no_instrument_function))
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
    printf("[-]\n");
}

int main() {
    int fd;
    struct stat sb;
    char *head;

//    char path1[PATH_MAX];
//    ssize_t len1 = readlink("/proc/self/exe", path1, sizeof(path1)-1);
//    printf("%s\n", path1);






    // Get the filename of the executable
    char path[256], exe[256];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path)-1);
    if (len != -1) {
        path[len] = '\0';
    }
    strncpy(exe, basename(path), sizeof(exe));

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("%d\n", fd);
    };
    fstat(fd, &sb);
    printf("before mmap\n");
    head = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

    elfdump(head);

    FILE *maps = fopen("/proc/self/maps", "r");
    if (maps == NULL) {
        perror("fopen");
        return 1;
    }

    uintptr_t load_addr = main_addr;
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

    // Calculate the runtime address of the function
    uintptr_t func_addr = load_addr + main_addr;

    printf("Base address of the function: %lx\n", load_addr);
    printf("main offset: %lx\n__cyg_profile_func_enter", main_addr);
    printf("Runtime address of the function: %lx\n", func_addr);

    return 0;
}
