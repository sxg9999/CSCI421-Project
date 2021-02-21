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
#include "table.h"


// PLACEHOLDER
typedef struct {

} HashTable;

typedef struct {
    char* db_loc;
    int* page_size;
    HashTable* tables;
} TableManager;

int table_add(int* data_types, int* key_indices, int data_types_size, int key_indices_size);

Table* getTables();

/*
 * Open (or create) TABLES_META_DATA, add newTable to it.
 * Create new dir for this table.
 */
int table_save(Table* newTable);
/*
 * Deletes data from table, but keeps the table meta data.
 */

int table_clear(int table_id);

/*
 * Deletes a table from existance.
 */
int table_drop(int table_id);

#endif TABLES_H