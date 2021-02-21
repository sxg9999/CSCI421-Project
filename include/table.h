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
    int num_records; // number of tuples
    int* column_types; // data_types of attributes
    int* key_indices; // which columns are keys
    int num_columns;
    int num_keys;
    int num_pages;
    int* page_ids;
} Table;

Page* new_page(Table* self);
Page* new_existing_page(Table* self, int page_id);


int Table_write_meta(Table* self, FILE* fp);

int Table_read_meta(Table* newTable, FILE* fp);


void Table_init(Table* self, TableParams* params);

Table* Table_create(TableParams* params);


int Table_insert_record(Table* self, buffer_manager* bm, union record_item* record);

int Table_update_record(Table* self, buffer_manager* bm, union record_item* record);



/*
 *
 */
void Table_clear(Table* self);

void Table_destroy(Table* self);

// returns the new page id
Page* new_page(Table* self);

Page* new_existing_page(Table* self, int page_id);


#endif