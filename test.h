#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Argの定義。フィールドはRustのArgと一致させる必要があります。
typedef struct {
    bool is_arg;
    char* name;
    int64_t location;
    char* type_name;
    uint64_t bytes_cnt;
} Arg;

// Rustの関数のプロトタイプ
//extern void* get_func2args(const char* file_path);
//extern size_t get_arg_count(void* map, const char* key);
extern Arg get_ith_arg(void* map, const char* key, size_t i);
//extern void free_map(void* map);
extern char* get_funcname(void* map, uintptr_t arg);
extern void* get_addr2func(const char* file_path);
extern size_t get_arg_cnt_from_func_addr(void* map, uintptr_t func_addr);
extern Arg get_ith_arg_from_func_addr(void* map, uintptr_t func_addr, size_t i);
