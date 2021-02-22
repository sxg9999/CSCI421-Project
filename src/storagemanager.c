/*
 * Author: Dylan Cuprewich (dxc2961), Steven Guan (sxg8944)
 * Implements functions related to the storage manager
 * for CSCI421 Group Project Storage Manager.
 * Implements "include/storagemanager.h".
 * 
 * See "include/storagemanager.h" for more documentation.
 * Functions w/o documentation here are described in header.
 */
#include <stdlib.h>
#include "../include/storagemanager.h"
#include "../include/table.h"
#include "../include/table_manager.h"
#include "../include/buffer_manager.h"
#include <errno.h>
#include <math.h>

char* DB_LOC;
buffer_manager* bufferManager;
TableManager* tableManager;

int create_database(char* db_loc, int page_size, int buffer_size, bool restart) {
    int rc;

    DB_LOC = db_loc;
    
    if (restart) {
        // simply restart an existing DB
        rc = restart_database(db_loc);
    } else {
        // create a brand new database
        rc = new_database(db_loc, page_size, buffer_size);
    }
    return rc;
}

int restart_database(char* db_loc) {

    tableManager = malloc(sizeof(TableManager));
    return TM_read_meta(db_loc, tableManager);
}

int new_database(char* db_loc, int page_size, int buffer_size) {
    bufferManager = BufferManager_new( floor(buffer_size / page_size) );
    tableManager = init_table_manager(db_loc, page_size);
    return 0;
}

int get_records(int table_id, union record_item*** table) {
    int record_count = 0;

    // "table" is location of 2d array
    // put records from the table with id=table_id into "table"

    // find the table
    // get page_ids from the table
    // get pages with those ids from buffermanager
    // if buffermanager doesn't have them, add them
    


    return record_count;
}

int get_page(int page_id, union record_item*** page) {
    return 0;
}

int insert_record(int table_id, union record_item* record) {
    Table* table = NULL;

    if(TM_get_table(tableManager, table_id, table) == -1) {
        return -1;
    }

    if(Table_insert_record(table, bufferManager, record) == -1) {
        return -1;
    }

    return 0;
}

int update_record(int table_id, union record_item* record) {
    Table* table = NULL;

    if(TM_get_table(tableManager, table_id, table) == -1) {
        return -1;
    }

    if(Table_update_record(table, bufferManager, record) == -1) {
        return -1;
    }

    return 0;
}

int drop_table(int table_id) {
    int ret = TM_drop_table(tableManager, table_id);
    return ret;
}

int clear_table(int table_id) {
    int ret = TM_clear_table(tableManager, table_id);
    return ret;
}

/* 
 * data_types_size: number of columns
 * key_indices_size: number of attributes in primary key
 * 
 */
int add_table(int* data_types, int* key_indices, int data_types_size, int key_indices_size) {

    return TM_add_table(tableManager, data_types, key_indices, data_types_size, key_indices_size);
    
}

int purge_buffer() {
    return 0;
}

int terminate_database() {
    int saved = TM_save(tableManager);

    TM_destroy(tableManager);
    return saved;
}
