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

#define TM_META_FILE "table_manager_meta_data.oo"


// PLACEHOLDER
typedef struct {

} HashTable;

typedef struct {
    char* db_loc;
    int page_size;
    int* table_ids;
    HashTable* tables;
    int num_tables;
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