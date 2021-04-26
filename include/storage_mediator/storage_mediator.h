//
// Created by SGuan on 4/14/2021.
//

/**
 * Header file for declaring functionalities that involves accessing
 * both catalog and storagemanager.
 *
 * i.e., create table statement, drop table, alter table, etc
 */

#ifndef STORAGE_MEDIATOR_H
#define STORAGE_MEDIATOR_H

#include <string.h>

#include "../../include/catalog/catalog_structs.h"
#include "../../include/storagemanager.h"

/**
 * Adds a table to both catalog and storage manager
 * @param t_data : a struct that contains table data
 * @return : 0 if it is successful and -1 if it failed
 */
int sm_add_table(struct catalog_table_data* t_data);

/**
 * Drops a table and reflect the changes in both catalog and storagemanager
 * @return : 0 if it is successful and -1 if it failed
 */
int sm_drop_table(char* table_name);

/**
 * Alter a table and reflect the changes in both catalog and storagemanager
 * @return : 0 if it is successful and -1 if it failed
 */
int sm_alter_table();

/**
 * Attempt to insert a record to the database
 * @param table_name : name of the table
 * @param record : record to be inserted
 * @return 0 for success and -1 for fail
 */
int sm_insert_record(char* table_name, union record_item* record);
/**
 * Inserts records to the table
 * @return
 */
int sm_insert_records(char* table_name, union record_item** records, int num_of_records);


/**
 * Checks if a record with the given key exist
 * @param table_id : the table_id
 * @param key_values : the primary key values
 * @return 1 for true, 0 for false
 */
int sm_record_exist(int table_id, union record_item* key_values);

/**
 * Check to see if the record_value exist or not
 * @param table_name : name of the table
 * @param record_value : the record value to be checked
 * @param attr_index : index in the record that the record value belongs to
 * @param attr_type : the attribute type of the record value
 * @return 1 for exist and 0 for does not exist
 */
int sm_record_value_exist(char* table_name, union record_item record_value,
        int attr_index, enum db_type attr_type);



/**
 * Check to see if the a array of record value exist or not
 * @param table_name : name of the table
 * @param record_value : an array of record values to checked
 * @param attr_index_arr : an array of indices that corresponds with the record values
 * @param attr_type_arr : an array of attribute types that corresponds with the record values
 * @param num_of_values : number of record values
 * @return 1 for exist and 0 for does not exist
 */
int sm_record_values_exist(char* table_name, union record_item* record_values,
                              int* attr_index_arr, enum db_type* attr_type_arr, int num_of_values);



#endif
