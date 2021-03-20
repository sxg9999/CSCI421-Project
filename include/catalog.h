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



/**
 * params related to attributes
 */
struct attribute_params{
    char** attrs;
    int* attr_types;
    int* attr_indices;
    int* attr_constraints;
    int num_of_attr;
};

/**
 * params related to primary key
 */
struct primary_key_params{
    char** primary_key;
    int* pk_types;
    int* pk_indices;
    int pk_len;
};

struct foreign_key_params{
    int num_of_fk;
    char*** foreign_keys;       //[0][0]= foreign_key_1's keys/attributes
                                //[0][1] = foreign_key_1's reference table
                                //[1][0] = foreign_key_2's keys/attributes
                                //[1][1] = foreign_key_2's reference table
};



//int init_catalog(char* db_path);
//
//int catalog_get_table_num(char* table_name);
//int catalog_table_mapping_add(char* table_name, int table_num, char** attr_names, int** attr_int_vals,
//                              char** foreign_keys);
//int catalog_table_mapping_remove(char* table_name);
//int catalog_table_mapping_contains(char* table_name);
//int catalog_close();
//void catalog_test_print();
//void catalog_print_table_map_info();
//void catalog_print_table_map_content();
//void catalog_print_table_names();
int init_catalog(char* db_path);
int catalog_add(int table_num, char* table_name, struct attribute_params* attrParams,
                struct primary_key_params* pk_param, struct foreign_key_params* fk_params);
int write_catalog();


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

//static struct catalog_func catalog = {
//    .add_table = catalog_table_mapping_add,
//    .remove_table = catalog_table_mapping_remove,
//    .get_table_num = catalog_get_table_num,
//    .table_contains = catalog_table_mapping_contains,
//    .close = catalog_close,
//    .test_print = catalog_test_print,
//    .print_table_info = catalog_print_table_map_info,
//    .print_table_map = catalog_print_table_map_content,
//    .print_table_names = catalog_print_table_names
//};



#endif