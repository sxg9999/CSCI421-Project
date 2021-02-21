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


#define TABLE_META_DATA_FILENAME "table_meta"

// PLACEHOLDER
typedef struct HashTable{
    int current_size;
} HashTable;

typedef struct {
    char* db_loc;
    int page_size;
    int* table_ids;
    HashTable* tables;
    int num_tables;
} TableManager;

Table* getTables();

/*
 * Writes the meta data about each of the tables to a file. 
 * @param self - the table manager to be written out to a file
 */
void TM_write_meta(TableManager* self);

/*
 * Reads and reconstructs the tables from the meta data file.
 */
void TM_read_meta();

/*
 *
 * 
 */
TableManager* init_table_manager(char* db_loc, int page_size);

/*
 * Create a brand new
 * 
 */
int TM_add_table(TableManager* self, int* data_types, int* key_indices, int data_types_size, int key_indices_size);

/* 
 * 
 * 
 */
int TM_add_old_table(TableManager* self, Table* old_table);

/*
 *
 */
void TM_save_tables(TableManager* self);

/*
 *
 * 
 */
int TM_drop_table(TableManager* self, int table_id);


/*
 *
 * 
 */
char* get_table_dir(int table_id, char* db_loc);

#endif