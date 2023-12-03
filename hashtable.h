#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

#define HASH_SIZE 100

typedef struct Node {
    uintptr_t key;
    char* value;
    struct Node* next;
} Node;

typedef struct HashTable {
    Node* nodes[HASH_SIZE];
} HashTable;

//HashTable* create_hashtable();
//void destroy_hashtable(HashTable* hashtable);
//void insert(HashTable* hashtable, uintptr_t key, char* value);
//char* find(HashTable* hashtable, uintptr_t key);
//
#endif //HASHTABLE_H