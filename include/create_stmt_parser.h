
#ifndef CREATE_STMT_PARSER
#define CREATE_STMT_PARSER

#include "../include/create_stmt_parser.h"
#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"
#include "../include/error_printing.h"
#include "../include/catalog/catalog_structs.h"
#include "../include/catalog/catalog.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/db_types.h"

/**
 * Check if an attributes specified in a foreignkey constraint is already defined.
 * @param attr_names list of attr names already defined
 * @param current_attr current attr in foreignkey constraint being check
 * @param attr_count count of attr already defined
 */
int foreign_contraint_name_check(char** attr_names, char* constraint_name, int attr_count);

/**
 * Get the attributes/contraints from a create stmt.
 * ex. "CREATE TABLE BAZZLE( baz double PRIMARYKEY );" -> ["baz double PRIMARYKEY"]
 * @param stmt - the original create statement
 * @param attributes - 2d array of attribute/constraint strings
 * @param attrCount - ptr to the total count of attributes in the `stmt`
 * 
 * @return 0 on Success, -1 on failure
 */
int get_attrs_from_stmt(char* stmt, char** attributes, int* attr_count);

/**
 * Checks if an attribute is valid, and creates it.
 * @param currentAttr the current attribute being validated
 * @param attr_names a list of attribute names
 * @param name_count total of attributes
 * @param is_first_attr whether or not this is the first attribute
 * @return 0 on success, -1 on failure
 */
int attr_form_check(char* currentAttr, char** attr_names, int* name_count, 
                int is_first_attr, struct catalog_table_data* new_table);


int attribute_check(char* currentAttr, char* token, char** attr_names, int* name_count,
                struct catalog_table_data* new_table);

/**
 * Checks for constraints on an attribute is valid.
 * @param token strtok pointing towards possible attribute constraint
 * @param attribute string rep of the attribute
 * @param new_attribute attribute struct
 * @return 0 if success, -1 if failure
 * 
 */
int constraint_check(char* currentAttr, char* token, char** attr_names, int* name_count, 
                int attr_num, struct catalog_table_data* new_table);

/**
 * Check for the specified size of vchar/char attribute.
 * 'varchar(X)' or 'char(X)'.
 * @param token the varchar/char token to get the length from
 * @param new_attribute the attribute struct currently being constructed
 * @param attribute_type the attribute type -- varchar/char
 * @return varchar/char length if successful check, -1 if check failed
 */
int char_type_check(char* char_attr, enum db_type attr_type);

/**
 * Gets the attribute size based on the attribute type
 * @param attribute_type the attribute type for token
 * @param token the attribute type string
 * @param currentAttr the current attribute
 * @return the size of the attribute if successful, else -1 for failure
 */
int attribute_type_size(enum db_type attribute_type, char* token, char* currentAttr);


int attribute_constraint_check(char* token, char* attribute, 
                struct attr_data* new_attribute, struct catalog_table_data* new_table);


int add_attr_constraints(char** constraints, enum db_type* constraint_types, 
                struct attr_data* attribute, struct catalog_table_data* new_table);



#endif