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

#endif
