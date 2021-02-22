/* 
 * Author: Nicolas Chaudron
 * Behavior related to tables.
 * Essentially a Table class.
 */

#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>

#include "storagemanager.h"
#include "page.h"
#include "buffer_manager.h"

#define ec(X) (((X) < 0) ? (X) : 0)
#define TABLE_META_DATA_FILENAME "table_meta"


typedef struct{
    char* loc;
    int table_id; // unique id
    int num_records; // number of tuples
    int* columns; // data_types of attributes
    int* primary_key; // which columns are keys
    int num_columns;
    int num_keys;
    int num_pages;
    int* page_ids;
    int page_size;
} Table;

typedef struct{
    int table_id;
    int page_size;
    int data_types_size;
    int key_indices_size;
    int* data_types;
    int* key_indices;
    char* loc;
} TableParams;

Page* new_page(Table* self);
Page* new_existing_page(Table* self, int page_id);


int Table_write_meta(Table* self, FILE* fp);

int Table_read_meta(Table* newTable, FILE* fp);


void Table_init(Table* self, TableParams* params);

Table* Table_create(TableParams* params);

int Table_insert_record(Table* self, buffer_manager* bm, union record_item* record);

int Table_update_record(Table* self, buffer_manager* bm, union record_item* record);


void Table_clear(Table* self);

void Table_destroy(Table* self);

// returns the new page id
Page* new_page(Table* self);

Page* new_existing_page(Table* self, int page_id);

void tostring(int num, char str[]);

#endif 