/* 
 * Author: Nicolas Chaudron
 * Behavior related to tables.
 * Essentially a Table class.
 */

#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include "storagemanager.h"
#include "table_meta_data.h"
#include "page.h"
#include "buffer_manager.h"

typedef struct{
    int table_id; // unique id
    union record_item*** records; // pointer to list of all tuples
    int num_records; // number of tuples
    int* column_types; // data_types of attributes
    int* key_indices; // which columns are keys
    int num_columns;
    int num_keys;
    int num_pages;
} Table;

union record_item* row;
union record_item** rows;
union record_item*** pointer_to_all_rows;

// typedef struct{

//     int num_of_pages;
//     char table_name[255];
//     void *** table;
//     // Hash_Table* hash_table;
    
// }Table;

void table_init();

Table* create_table(int table_id, int* data_types, int* key_indices, int num_cols, int num_keys);

union record_item*** getTable(int table_id);

union record_item*  getRecord_with_PrimaryKey(int table_id, union record_item* primary_key);


#endif