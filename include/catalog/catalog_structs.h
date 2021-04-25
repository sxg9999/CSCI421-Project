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



/**
 * A union for storing the default values
 * of a default constraint
 */
union default_value{
    int i;
    double d;
    bool b;
    char c[255];
    char v[255];
};

/**
 * A struct for storing a attribute
 * constraint information.
 *
 * type - (required)  could be notnull, default, unique, primarykey
 * value - (not required) for storing the default values of a default constraint
 */
struct attr_constraint{
    enum db_type type;
    union default_value* value;
};


/**
 * A struct for storing a attribute information
 *
 * index - the column index of the attribute
 * attr_size - size of the attribute ( int = 4, bool= 1, char(1) = 1, char(5) = 5, char(25) = 25,  etc)
 * type - the attribute type (INT, BOOL, CHAR, etc)
 * attr_name - the string name of the attribute
 *
 * num_of_constr - number of constraints for the attribute
 * constr - a array of attribute constraints (attr_constraint struct)
 *
 */
struct attr_data{
    int index;
    int attr_size;          //used to determine the length of char and varchar
    enum db_type type;
    char* attr_name;

    int num_of_constr;
    struct attr_constraint** constr;
};


/**
 * A struct for storing foreign key data
 *
 * parent_table_name - the name of the table that the foreign key references
 * f_keys - a hash table that stores foreign key and reference tables primary key pairings
 *          ex: foreign key = (f1,f2,f3) ref table's primary key = (p1,p2,p3)
 *              hashtable = [(f1,p1), (f2,p2), (f3,p3)]
 */
struct foreign_key_data{
    char* parent_table_name;
    struct hashtable* f_keys;               //maps the foreign key to the parent table's primary key
                                            //attribute

};


/**
 * A struct that holds the table information
 *
 * attr_ht - A hashtable that stores the name of the attributes with the attributes data (attr_data struct) of the table
 *           ex: [ (attribute_name_1, attr_data_1), (attribute_name_2, attr_data_2) ...]
 */
struct catalog_table_data{
    int table_num;                         // The table number
    char* table_name;                      // The name of the table
    struct hashtable* attr_ht;             //A hashtable that stores the attributes data (attr_data struct) of the table


    int p_key_len;                         // Size of the primary key (amount of attributes that makes up a primary key)
    char** primary_key_attrs;              // A array of attributes that makes up a primary key (in their string form)

    int num_of_f_key;                      // Number of foreign keys in the table
    int f_key_arr_size;                    // The size of the array that contains the foreign keys
    struct foreign_key_data** f_keys;      // A array of foreign keys of the table

    int num_of_childs;                     // The number of tables that references this tables primary key as a foreign key
    int child_arr_size;                    // The size of the array that stores the child tables (other tables that references this table)
    char** childs;                         // A array of child table names


};

//struct child_tables{
//    int count;
//    char** t_names;
//};

#endif
