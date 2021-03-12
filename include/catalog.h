/*
 * Author: Steven Guan
 * 
 * Header file for db_tables which keeps track of all the tables in a db
 * Outlines the functionality for db_tables
 * 
 */

#ifndef CATALOG_H
#define CATALOG_H

#include <stdlib.h>


int init_catalog(char* db_path);

int catalog_get_table_num(char* table_name);
int catalog_table_mapping_add(char* table_name, int num);
int catalog_table_mapping_remove(char* table_name);
int catalog_table_mapping_contains(char* table_name);
int catalog_close();
void catalog_test_print();
void catalog_print_table_map_info();
void catalog_print_table_map_content();
void catalog_print_table_names();


struct catalog_func{
    int (*add_table)(char*, int);
    int (*remove_table)(char*);
    int (*get_table_num)(char*);
    int (*table_contains)(char*);
    int (*close)();
    void (*test_print)();
    void (*print_table_info)();
    void (*print_table_map)();
    void (*print_table_names)();
};

static struct catalog_func catalog = {
    .add_table = catalog_table_mapping_add,
    .remove_table = catalog_table_mapping_remove,
    .get_table_num = catalog_get_table_num,
    .table_contains = catalog_table_mapping_contains,
    .close = catalog_close,
    .test_print = catalog_test_print,
    .print_table_info = catalog_print_table_map_info,
    .print_table_map = catalog_print_table_map_content,
    .print_table_names = catalog_print_table_names
};



#endif