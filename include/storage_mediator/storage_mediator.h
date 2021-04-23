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
 * Inserts records to the table
 * @return
 */
int sm_insert_records(char* table_name, union record_item** records, int num_of_records);


#endif
