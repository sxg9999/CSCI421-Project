//
// Created by SGuan on 4/14/2021.
//

#ifndef CATALOG_PARSING_H
#define CATALOG_PARSING_H

#include "../db_types.h"
#include "catalog_structs.h"

struct catalog_table_data* catalog_get_table_data_struct(char* table_name, char* data_str);

int catalog_add_attributes(struct catalog_table_data* t_data, char** data_str_arr, int data_str_size);

union default_value* get_attr_constr_default_val(char** str, enum db_type attr_type);

union default_value* get_attr_constr_type_val(char** str, char* attr_name, enum db_type attr_type, enum db_type* constr_type);

struct attr_constraint** get_attr_constraints(int* count, char* attr_name, enum db_type attr_type, char* constr_str);

char** get_primary_key_attrs(int* p_key_len, char* data_str);

int catalog_add_primary_key(struct catalog_table_data* t_data, char** data_str_arr, int data_str_size);

int compare_attr_data(struct attr_data* a_data_1, struct attr_data* a_data_2);

int check_if_attr_matches(int count, struct hashtable* attr_ht, struct hashtable* ref_attr_ht,
                          char** foreign_key_attrs, char** ref_table_attrs);

struct foreign_key_data* create_foreign_key(int attr_count, char* ref_table_name, char** foreign_key_attrs, char** ref_attrs);

struct foreign_key_data* get_foreign_key(int num_of_f_key_attr, int num_of_ref_attr, char** foreign_key_attrs,
                                         char** ref_table_attrs, char* ref_table_name, struct hashtable* table_attr_ht, int* result);

char* get_foreign_key_attrs(char* data_str, char*** foreign_key_attrs, int* num_of_f_key_attr);

char* get_ref_table_name(char* data_str, char** ref_table_name);

char* get_ref_table_attrs(char* data_str, char*** ref_table_attrs, int* num_of_ref_attr);

int table_add_child(char* parent_name, char* child_name);

int catalog_add_foreign_keys(struct catalog_table_data* t_data, char** data_str_arr, int data_str_size);

int catalog_add_table_to_storage_manager(struct catalog_table_data* t_data);




#endif
