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
 * Build a record of only the attributes in a unique attribute group
 * @param table_name : name of the table
 * @param record : a row of record in the table
 * @param unique_attr_indices : an array of the unique group attribute's indices
 * @param unique_group_size : the size of the unique group
 * @param unique_record : a record to store all the unique group attribute values
 * @return 0 for success and -1 for fail
 */
int build_unique_record(char* table_name, union record_item* record, int* unique_attr_indices,
                        int unique_group_size, union record_item** unique_record);
#endif
