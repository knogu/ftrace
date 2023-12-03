#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"

unsigned int hash(uintptr_t key) {
    return key % HASH_SIZE;
}

HashTable* create_hashtable() {
    HashTable* hashtable = calloc(1, sizeof(HashTable));
    if (hashtable == NULL) {
        printf("Failed to allocate memory for hashtable\n");
        exit(1);
    }
    return hashtable;
return NULL;
}

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

void insert(HashTable* hashtable, uintptr_t key, char* value) {
    unsigned int index = hash(key);
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = strdup(value);
    node->next = hashtable->nodes[index];
    hashtable->nodes[index] = node;
}

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