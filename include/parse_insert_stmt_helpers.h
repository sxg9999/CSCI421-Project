//
// Created by SGuan on 4/24/2021.
//

#ifndef PARSE_INSERT_STMT_HELPERS_H
#define PARSE_INSERT_STMT_HELPERS_H

#include "stdbool.h"
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
 * Convert the tuple string into record_item equivalent
 * @param table_name : name of the table
 * @param tuple_str : the tuple str
 * @param record : a record for storing the record values
 * @return 0 for success and -1 for fail
 */
int build_record(char* table_name, char* tuple_str, union record_item** record);

/**
 * Build a record of only the attributes in a unique attribute group
 * @param table_name : name of the table
 * @param record : a row of record in the table
 * @param unique_group_attr_indices : an array of the unique group attribute's indices
 * @param unique_group_size : the size of the unique group
 * @param unique_record : a record to store all the unique group attribute values
 * @return 0 for success and -1 for fail
 */
int build_unique_record(char* table_name, union record_item* record, int* unique_group_attr_indices,
        int unique_group_size, union record_item** unique_record);
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

/**
 * Convert a tuple string to record_items
 * @param t_data : the table meta data
 * @param attr_data_arr : an array of attribute meta data
 * @param num_of_attr : the number of attributes
 * @param tuple_str : the tuple string
 * @param record: a record (tuple of attributes)
 * @return 0 for success and -1 for fail
 */
int convert_to_record_new(struct catalog_table_data* t_data, struct attr_data** attr_data_arr,
                int num_of_attr, char* tuple_str, union record_item** record);

/**
 * Convert a string to a record item/record value
 * @param record_value_str : the record value string
 * @param record_value : the record value
 * @param attr_type : attribute type
 * @return 0 for success and -1 for fail
 */
int get_record_value_notnull(char* record_value_str, union record_item* record_value,
        enum db_type attr_type);

/**
 * Returns a record value with a null value that corresponds with its attribute type
 *
 * @param record_value : the record value
 * @param attr_type : attribute type
 * @return 0 for success and -1 for fail
 */
int get_record_value_null(union record_item* record_value,
                             enum db_type attr_type);

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

/**
 * Get the next value in the tuple_str without parenthesis
 * @param str_ptr : a pointer to the tuple string without parenthesis
 * @param value_str : the value string
 * @return 0 if successful and -1 if failed
 */
int get_next_value_new(char** str_ptr, char** value_str);

/**
 * Get the next non string value in the tuple_str without parenthesis
 * @param space_ptr : a pointer to where the space is currently
 * @param str_ptr : a pointer to the tuple string without parenthesis
 * @param value_str : the value string
 * @return 0 if successful and -1 if failed
 */
int get_next_val_non_string(char* space_ptr, char** str_ptr, char** value_str);

/**
 * Get the next string value in the tuple_str without parenthesis
 * @param open_quote_ptr : a pointer to where the open quote is currently
 * @param str_ptr : a pointer to the tuple string without parenthesis
 * @param value_str : the value string
 * @return 0 if successful and -1 if failed
 */
int get_next_val_string(char* open_quote_ptr, char** str_ptr, char** value_str);




#endif
