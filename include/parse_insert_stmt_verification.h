//
// Created by SGuan on 4/24/2021.
//

#ifndef PARSE_INSERT_STMT_VERIFICATION_H
#define PARSE_INSERT_STMT_VERIFICATION_H

#include "catalog/catalog_structs.h"
#include "storagemanager.h"

/**
 * Verify that a table with the table name exist in the database
 * @param func_loc_str : the function name that called this function
 * @param table_name : name of the table to check
 * @return 0 for success and -1 for failed
 */
int verify_table_exist(char* func_loc_str, char* table_name);

/**
 * Verify that the parameter for the function convert_to_records is valid
 * @param table_name : name of the table
 * @param tuple_str_arr : an array that stores the tuple strings
 * @param num_of_tuple : number of tuples
 * @param record_arr : a array of records
 * @param num_of_records : number of records
 * @return 0 if success and -1 if failed
 */

int verify_build_records_parameters(char* table_name, char** tuple_str_arr, int num_of_tuple);

/**
 * Verify that the record is valid to attempt a insert
 * @param t_data : the table meta data
 * @param record : the record to be checked
 * @return 0 if success and -1 if failed
 */
int verify_record_is_valid(struct catalog_table_data* t_data, union record_item* record);
/**
 * Verify that the attribute metadata nodes are valid
 * @param attr_data_arr : a array of attribute metadata nodes
 * @param num_of_attr : the number of attributes
 * @return 0 for success and -1 for fail
 */
int verify_attr_meta_data_nodes(struct attr_data** attr_data_arr, int num_of_attr);

/**
 * Verify that the parsed value strings exist and that the total number of
 * value strings parsed is equal to the number of attributes
 * @param value_str_arr : a array that stores value strings
 * @param num_of_vals : the number of value strings
 * @param num_of_attrs : the number of attributes
 * @return 0 for success and -1 for failed
 */
int verify_parsed_val_str(char** value_str_arr, int num_of_vals, int num_of_attrs);

/**
 * Verify that the array that stores the attr_type is valid
 * @param attr_types
 * @param num_of_attrs
 * @return
 */
int verify_attr_types_exist(enum db_type* attr_types, int num_of_attrs);

#endif
