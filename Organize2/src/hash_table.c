#include "common.h"
#include "hash_table.h"

HashEntry** hash_table = NULL;

unsigned int hash(const char *name) {
    unsigned int hash_value = 0;
    while (*name) {
        hash_value = (hash_value * 31) + *name;
        name++;
    }
    return hash_value % HASH_SIZE;
}

void init_hash_table() {
    hash_table = malloc(HASH_SIZE * sizeof(HashEntry*));
    for (int i = 0; i < HASH_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

void hash_table_insert(char *name, int id) {
    unsigned int index = hash(name);
    
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    new_entry->name = malloc(strlen(name) + 1);
    strcpy(new_entry->name, name);
    new_entry->id = id;
    new_entry->next = NULL;
    
    if (hash_table[index] == NULL) {
        hash_table[index] = new_entry;
    } else {
        new_entry->next = hash_table[index];
        hash_table[index] = new_entry;
    }
}

int hash_table_search(const char *name) {
    unsigned int index = hash(name);
    HashEntry *current = hash_table[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->id;  
        }
        current = current->next;
    }
    return -1;  
}

void free_hash_table() {
    if (!hash_table) return;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = hash_table[i];
        while (entry) {
            HashEntry *temp = entry;
            entry = entry->next;
            free(temp->name);
            free(temp);
        }
    }
    free(hash_table);
}