/*
 * Author: Dylan Cuprewich (dxc2961), Steven Guan (sxg8944)
 * Implements functions related to the storage manager
 * for CSCI421 Group Project Storage Manager.
 * Implements "include/storagemanager.h".
 * 
 * See "include/storagemanager.h" for more documentation.
 * Functions w/o documentation here are described in header.
 */

#include "../include/storagemanager.h"

int create_database(char* db_loc, int page_size, int buffer_size, bool restart) {
    int rc;
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
    return 0;
}

int new_database(char* db_loc, int page_size, int buffer_size) {
    return 0;
}

int get_records(int table_id, union record_item*** table) {
    int record_count = 0;

    return record_count;
}

int get_page(int page_id, union record_item*** page) {
    return 0;
}

int insert_record(int table_id, union record_item* record) {
    return 0;
}

int update_record(int table_id, union record_item* record) {
    return 0;
}

int drop_table(int table_id) {
    return 0;
}

int clear_table(int table_id) {
    return 0;
}

int add_table(int* data_types, int* key_indices, int data_types_size, int key_indices_size) {
    int table_id = 0;
    
    return table_id;
}

int purge_buffer() {
    return 0;
}

int terminate_database() {
    return 0;
}
