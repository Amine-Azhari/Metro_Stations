#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashEntry {
    char *name;
    int id;
    struct HashEntry *next;
} HashEntry;

extern HashEntry** hash_table;

void init_hash_table();
void hash_table_insert(char *name, int id);
int hash_table_search(const char *name);
void free_hash_table();

#endif