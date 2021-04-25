//
// Created by SGuan on 4/24/2021.
//

#ifndef PARSE_INSERT_STMT_HELPERS_H
#define PARSE_INSERT_STMT_HELPERS_H

#include "storagemanager.h"
#include "catalog/catalog_structs.h"


/**
 * Convert the tuple strings into record_item equivalent
 * @param table_name : name of the table
 * @param tuple_str_arr : an array that stores the tuple strings
 * @param num_of_tuple : number of tuples
 * @param record_arr : a array of records
 * @param num_of_records : number of records
 * @return 0 if success and -1 if failed
 */
int build_records(char* table_name, char** tuple_str_arr, int num_of_tuple,
                  union record_item*** record_arr, int* num_of_records);


/**
 * Print all the records that was parsed
 * @param record_arr : records that was parsed and build
 * @param num_of_records : number of records
 * @return
 */
void parse_insert_print_records(char** record_str_arr, int num_of_records);

/**
 * Convert a tuple string to record_items
 * @param t_data : the table meta data
 * @param attr_data_arr : an array of attribute meta data
 * @param num_of_attr : the number of attributes
 * @param tuple_str : the tuple string
 * @param record: a record (tuple of attributes)
 * @return 0 for success and -1 for fail
 */
int convert_to_record(struct catalog_table_data* t_data, struct attr_data** attr_data_arr,
                      int num_of_attr, char* tuple_str,  union record_item** record);


int convert_and_verify_foreign_key(struct catalog_table_data* t_data, union record_item** record);


/**
 * Parse all the values of the tuple_str and store it in value_arr
 * @param tuple_str : the tuple string
 * @param value_arr : a array that stores value strings
 * @param num_of_values : the number of value strings
 * @return 0 for success and -1 for fail
 */
int get_tuple_values(char* tuple_str, char*** value_arr, int* num_of_values);


/**
 * Get the next value in the tuple_str without parenthesis
 * @param str_ptr : a pointer to the tuple string without parenthesis
 * @param value_str : the value string
 * @return 0 if successful and -1 if failed
 */
int get_next_value(char** str_ptr, char** value_str);



#endif
