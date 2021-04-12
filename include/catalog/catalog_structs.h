//
// Created by SGuan on 3/22/2021.
//

#ifndef CATALOG_STRUCTS_H
#define CATALOG_STRUCTS_H

#include "../hash_collection/hash_table.h"
#include "../hash_collection/sv_ht.h"
#include "../db_types.h"
#include <stdbool.h>


struct catalog_table_data;
struct attr_constraint;
struct attr_node;
struct foreign_key_node;

union default_value{
    int i;
    double d;
    bool b;
    char c[255];
    char v[255];
};


struct attr_constraint{
    enum db_type type;
    union default_value* value;
};


struct attr_data{
    int index;
    int attr_size;          //used to determine the length of char and varchar
    int num_of_constr;
    enum db_type type;
    char* attr_name;
    struct attr_constraint** constr;
};


struct foreign_key_data{
    char* parent_table_name;
    struct hashtable* f_keys;               //maps the foreign key to the parent table's primary key
                                            //attribute

};


struct catalog_table_data{
    int table_num;
    int p_key_len;
    int num_of_f_key;
    int num_of_childs;
    int child_arr_size;
    char* table_name;
    char** childs;
    char** primary_key_attrs;
    struct foreign_key_data** f_keys;
    struct hashtable* attr_ht;

};

struct child_tables{
    int count;
    char** t_names;
};

#endif
