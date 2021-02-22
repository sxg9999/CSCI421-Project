/*
 *
 */
#ifndef TABLE_MANAGER_H
#define TABLE_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include "table.h"

// PLACEHOLDER
typedef struct HashTable{
    int current_size;
} HashTable;

///
/// PLACEHOLDERS
int Hash_get_item(HashTable* self, int key, void* item);

int Hash_remove_item(HashTable* self, int key);

int Hash_add_item(HashTable* self, int key, void* item);

int Hash_destroy(HashTable* self);

int Hash_put_int(HashTable* self, int key, void* item);
///
///

typedef struct {
    char* db_loc;
    int page_size;
    int* table_ids;
    HashTable* tables;
    int num_tables;
    int buffer_size;
} TableManager;

int TM_write_meta(TableManager* self);

buffer_manager* TM_read_meta(char* db_loc, TableManager* , int* ret);

TableManager* init_table_manager(char* db_loc, int page_size, int buffer_size);

int TM_get_table(TableManager* self, int table_id, Table* table);

void get_table_dir(int table_id, char* db_loc, char* dirStr);

int TM_add_table(TableManager* self, int* data_types, int* key_indices, int data_types_size, int key_indices_size);

int TM_add_old_table(TableManager* self, Table* old_table);

int TM_save(TableManager* self);

void TM_destroy(TableManager* self);

int TM_clear_table(TableManager* self, int table_id);

int TM_drop_table(TableManager* self, int table_id);

#endif