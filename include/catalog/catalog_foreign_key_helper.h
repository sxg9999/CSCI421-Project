//
// Created by SGuan on 4/24/2021.
//

#ifndef CATALOG_FOREIGN_KEY_HELPER_H
#define CATALOG_FOREIGN_KEY_HELPER_H

#include "catalog_structs.h"

/**
 * Checks if the table have a foreign key
 * @param table_name : name of the table
 * @return 1 if true and 0 if false
 */
int table_has_foreign_key(char* table_name);

/**
 * Get an array of foreign keys
 * - attr_data** stores a foreign key
 * - attr_data*** stores a array of foreign key
 *
 * @param table_name : the name of the table
 * @param foreign_keys : an array for storing the foreign key
 * @param key_count : the number of foreign key
 * @return 0 for success, -1 for fail
 */
int get_all_foreign_key(char* table_name, struct attr_data**** foreign_keys,
        struct attr_data**** parent_p_keys, int* key_count, int** key_attr_count);

/**
 * Checks if the foreign key data struct is valid and without errors
 *
 * @param f_key_data : the foreign key data struct
 * @return
 */
void verify_foreign_key_data_struct(struct foreign_key_data* f_key_data);
#endif
