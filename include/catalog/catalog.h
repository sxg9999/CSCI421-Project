/*
 * Author: Steven Guan
 * 
 * Header file for db_tables which keeps track of all the tables in a db
 * Outlines the functionality for db_tables
 * 
 */

#ifndef CATALOG_H
#define CATALOG_H

#include "catalog_structs.h"


int init_catalog(char* db_path);


int catalog_get_next_table_num();

int catalog_add_table(struct catalog_table_data* t_data);

/**
 * gets the table number of the table with the table_name
 * @param table_name
 * @return a table number if one is found or -1 if the table doesn't exist
 */
int catalog_get_table_num(char* table_name);

/**
 * remove the table with the table_name
 * @param table_name
 * @return return the table number of the removed table
 */
int catalog_remove_table(char* table_name);

/**
 * checks if the table contains the table already
 * @param table_name
 * @return 1 if true, 0 if false
 */

int catalog_contains(char* table_name);

/**
 * Gets the data/attr types of the attributes in the table
 *
 * @param t_data : a struct that contains the table data
 * @param data_types : the data types of the attributes in the table
 * @return the size of the array that stores the data types (data_types)
 */
int catalog_get_data_types(struct catalog_table_data* t_data, int** data_types);

/**
 * Gets the primary key indices of the table
 *
 * @param t_data : a struct that contains the table data
 * @param p_key_indices : an array of attribute indices that make up the primary key
 * @return the size of the array that stores the primary key indices
 */
int catalog_get_p_key_indices(struct catalog_table_data* t_data, int** p_key_indices);


struct hashtable* catalog_get_ht();

void catalog_close();

//int write_catalog();

void catalog_print_tables();


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