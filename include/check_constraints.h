//
// Created by SGuan on 4/27/2021.
//

#ifndef CHECK_CONSTRAINTS_H
#define CHECK_CONSTRAINTS_H

#include "storagemanager.h"
#include "db_types.h"
#include "catalog/catalog_structs.h"

/**
 * Goes through each record values and checks its corresponding attribute meta data
 * - if it has unique constraint then check if the constraint is met
 *      - if the constraint is not met then return 0
 * - if there are no instances of a record value with unique constraint then return 1
 * @param table_name : name of the table
 * @param record : record to be checked
 * @return 1 if true, 0 if false
 */
int single_unique_constraint_met(char* table_name, union record_item* record);


/**
 * Checks to see if the record satisfies all the unique group constraints
 * @param table_name : name of the table
 * @param record : the record
 * @param unique_attr_indices_arr : array of unique attribute indices array
 * @param unique_group_size_arr : array of unique group sizes
 * @param unique_group_count : number of unique group constraints
 * @return 1 if true, 0 if false
 */
int group_unique_constraint_met(char* table_name, union record_item* record,
        int** unique_attr_indices_arr, int* unique_group_size_arr, int unique_group_count);

/**
 * Checks to see if the record satisfies all the foreign key constraints
 * @param table_name : name of the table
 * @param record : the record
 * @param parent_names : names of parent table
 * @param foreign_key_indices_arr : 2d array of foreign key indicies
 * @param foreign_key_lens : len of the foreign key
 * @param f_key_count : number of foreign keys
 * @return 1 if true, 0 if false
 */
int foreign_key_constraint_met(char* table_name, union record_item* record,
         char** parent_names, int** foreign_key_indices_arr, int* foreign_key_lens,
         int f_key_count);

/**
 * Builds a sub record
 * @param table_name : name of the table
 * @param record : a row of record in the table
 * @param attr_indices : an array  of attribute's indices
 * @param num_of_values : number of values/attributes for the sub record
 * @param sub_record : a record to stores sub record of a record;
 * @return 0 for success and -1 for fail
 */
int build_sub_record(char* table_name, union record_item* record, int* attr_indices,
                        int num_of_attr, union record_item** sub_record);
#endif
