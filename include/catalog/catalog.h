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
 * Gets the table metadata
 * @param table_name : name of the table
 * @return
 */
struct catalog_table_data* catalog_get_table_metadata(char* table_name);

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


/**
 * Returns an array of the attribute types in enums
 * @param table_name : name of the table
 * @param attr_types : a array of attribute types
 * @return the size of the array that stores the attribute types
 */
int catalog_get_attr_types(char* table_name, enum db_type** attr_types);

/**
 * Returns an array of all the attribute's meta data in the table
 * @param table_name : the name of the table
 * @param attr_data_arr : an array that stores the attribute meta data
 * @param num_of_attr : the number of attributes
 * @return size of the array that stores the attribute or for error -1.
 */
int catalog_get_attr_data(char* table_name, struct attr_data*** attr_data_arr);


/**
 * Get all the foreign key indices of the table
 * @param table_name : name of the table
 * @param foreign_key_indices : a 2d array that stores the all the foreign keys
 * @param foreign_key_lens: an array of foreign key lens (corresponds with the foreign key indices array).
 * @return the number of foreign keys
 */
int catalog_get_foreign_key_indices(char* table_name, int*** foreign_key_indices, int** foreign_key_lens);

/**
 * Return the header for the table.
 *
 * i.e., Table 0 : department ( <id>, <dept_name> )
 * @param table_name
 * @return
 */
char* catalog_get_table_header(char* table_name);


/**
 * Checks if the catalog's table metadata is valid
 * @param t_data : the catalog's table metadata
 * @return 1 for true, 0 for false
 */
int catalog_table_data_is_valid(struct catalog_table_data* t_data);

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