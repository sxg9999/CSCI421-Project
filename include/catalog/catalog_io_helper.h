//
// Created by SGuan on 4/10/2021.
//

#ifndef CATALOG_IO_HELPER_H
#define CATALOG_IO_HELPER_H

#include "stdio.h"
#include "catalog_structs.h"

/**
 *
 * @param catalog_file
 * @param t_data
 * @return
 */

int catalog_write_table(FILE* catalog_file, struct catalog_table_data* t_data);

int catalog_write_attr(FILE* catalog_file, struct hashtable* attr_ht);

int catalog_write_primary_key(FILE* catalog_file, int p_key_len, char** primary_key_attrs);

int catalog_write_foreign_keys(FILE* catalog_file, int num_of_keys, int f_key_arr_size, struct foreign_key_data** f_keys);

int catalog_write_unique_groups(FILE* catalog_file, int unique_group_count,
                                int unique_group_arr_size, struct unique_group** unique_group_arr);

int catalog_write_childs(FILE* catalog_file, int num_of_childs, int child_arr_size, char** childs);



/**
 *
 * @param catalog_file
 * @param t_data
 * @return
 */

int catalog_read_table(FILE* catalog_file, struct catalog_table_data** t_data);

int catalog_read_attr(FILE* catalog_file, struct hashtable** attr_ht);

int catalog_read_primary_key(FILE* catalog_file, int* p_key_len, char*** primary_key_attrs);

int catalog_read_foreign_keys(FILE* catalog_file, int* num_of_keys, int* f_key_arr_size, struct foreign_key_data*** f_keys);

int catalog_read_unique_groups(FILE* catalog_file, int* unique_group_count, int* unique_group_arr_size, struct unique_group*** unique_group_arr);

int catalog_read_childs(FILE* catalog_file, int* num_of_childs, int* child_arr_size, char*** childs);


#endif //CSCI421_PROJECT_CATALOG_IO_HELPER_H
