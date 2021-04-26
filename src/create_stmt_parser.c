/*
 * Author: Dylan Cuprewich (dxc2961), Robert Gandley ()
 * Source file for parsing Create statements
 * Implements the public functionality for the Create Statement Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/create_stmt_parser.h"
#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"
#include "../include/error_printing.h"
#include "../include/catalog/catalog_structs.h"
#include "../include/catalog/catalog.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/db_types.h"
#include "../include/catalog/catalog_printer.h"
#include "../include/storage_mediator/storage_mediator.h"





int parse_create_table_stmt( char* input_statement ) {

    char* statement = (char* )malloc( strlen( input_statement ) + 1);
    strcpy(statement, input_statement); 
    printf("Parsing Create STMT: '%s'\n", statement);
    
    // skip "create table " part of the create stmt
    statement += strlen(CREATE_START) + 1 
                + strlen(TABLE) + 1;

    //Create Table setup
    char* table_name;
    // attribute strings
    char** attributes = malloc(10 * sizeof(char*));
    // names of individual attributes
    char** attr_names = malloc(10 * sizeof(char*));
    // # of attributes
    int attrCount = 0;
    int name_count = 0;
    
    // new table struct
    struct catalog_table_data* new_table = 
            (struct catalog_table_data*) malloc(sizeof(struct catalog_table_data));
    // initialize the table
    new_table->table_name = NULL;
    new_table->attr_ht = ht_create(12, 0.75);
    // unique constraint fields
    new_table->num_of_unique = 0;
    new_table->unique_group_sizes = NULL;
    // notnull constraint fields
    new_table->num_of_notnull = 0;
    new_table->notnull_attrs = NULL;
    // primary key fields
    new_table->p_key_len = 0;
    new_table->primary_key_attrs = NULL;
    // foreign key fields
    new_table->num_of_f_key = 0;
    new_table->f_key_arr_size = 0;
    new_table->f_keys = NULL;
    // foreign table fields
    new_table->num_of_childs = 0;
    new_table->child_arr_size = 0;
    new_table->childs = NULL;

    //get everything to left of first open parenthesis
    table_name = strtok(statement, "(");
    if ( null_check_str_create_stmt(table_name, input_statement) ) {
        fprintf(stderr, "Invalid statement\n");
        return -1;
    }
    for (int i = 0; table_name[i] != '\0'; i++) {
        if ( isalpha(table_name[i]) ) {
            table_name[i] = tolower(table_name[i]);
        }
    }
    // set table name 
    new_table->table_name = (char *)malloc( strlen(table_name) + 1 );
    strcpy(new_table->table_name, table_name); 

    // check if table exists
    /*
    if (catalog_contains(new_table->table_name)) {
        fprintf(stderr, "Table '%s' already exists!\n");
        return -1;
    }*/
    printf("Table '%s' doesn't already exists!\n", new_table->table_name);


    // get all attributes
    int attr_success = get_attrs_from_stmt(statement, attributes, &attrCount);
    if ( attr_success == -1 ) {
        fprintf(stderr, "Failed to get attributes from create statement: '%s'\n", statement);
        return -1;
    }

    // check if each attribute is form properly
    printf("Checking attributes and constraints validity...\n");
    for (int i = 0; i < attrCount; i++) {
        char* currentAttr = attributes[i];
        // check if attribute is a valid attribute/constraint
        int valid_attr = attr_form_check(currentAttr, attr_names, &name_count, i, new_table);
        if (valid_attr == -1) {
            fprintf(stderr, "Invalid attribute: '%s'\n", currentAttr);
            return -1;
        } 
    }

    // check if table has a primarykey
    if (new_table->p_key_len == 0) {
        fprintf(stderr, "No primarykey specified in '%s'\n", input_statement);
        return -1;
    }

    // add table to catalog
    new_table->table_name = (char *)malloc( strlen(table_name) + 1 );
    for (int i = 0; table_name[i] != '\0'; i++) {
        if ( isalpha(table_name[i]) ) {
            table_name[i] = tolower(table_name[i]);
        }
    }
    strcpy(new_table->table_name, table_name); 
    printf("Adding table '%s' to catalog...\n", new_table->table_name);
    int add_to_catalog_success = sm_add_table(new_table);
    if (add_to_catalog_success == -1) {
        fprintf( stderr, "Error trying to add table '%s' to the catalog.\n", new_table->table_name);
        return -1;
    }
    printf("Successfully created table '%s'!\n", input_statement);
    catalog_print_tables();

    free(attributes);
    return 0;
}

/**
 * Get the attributes/contraints from a create stmt.
 * ex. "CREATE TABLE BAZZLE( baz double PRIMARYKEY );" -> ["baz double PRIMARYKEY"]
 * @param stmt - the original create statement
 * @param attributes - 2d array of attribute/constraint strings
 * @param attrCount - ptr to the total count of attributes in the `stmt`
 * 
 * @return 0 on Success, -1 on failure
 */
int get_attrs_from_stmt(char* stmt, char** attributes, int* attrCount) {
    printf("Getting attributes from create statement...\n");
    int is_last_attr = 0;

    char* newAtrribute;
    while (is_last_attr == 0) {
            // get next attribute
        newAtrribute = strtok(NULL, ",");
        // check if there is any attributes
        if ( null_check_str_create_stmt(newAtrribute, stmt) ) {
            fprintf(stderr, "Invalid token '%s' in stmt '%s'.\n", newAtrribute, stmt);
            return -1;
        }
        // check if last attribute
        if (  newAtrribute[strlen(newAtrribute) - 1] == ';' ) {
            newAtrribute[strlen(newAtrribute) - 1] = '\0';
            is_last_attr = 1;
        }
        printf("Potential attribute/contraint: '%s'\n", newAtrribute);
        // save attribute
        attributes[(*attrCount)] = newAtrribute;
        (*attrCount) += 1;
        if ((*attrCount)%10 == 0) {
            attributes = realloc(attributes, ((*attrCount)+10) * sizeof(int*));
        }
    }

    return 0;
}

/**
 * Checks if an attribute/constraint is valid, and creates it.
 * @param currentAttr the current attribute/constraints being validated
 * @param attr_names a list of attribute names
 * @param name_count total of attributes
 * @param is_first_attr whether or not this is the first attribute
 * @return 0 on success, -1 on failure
 */
int attr_form_check(char* currentAttr, char** attr_names, int* name_count, int attr_num,
        struct catalog_table_data* new_table) 
{
    char* token;
    char* currentAttr_copy = (char*)malloc( strlen( currentAttr ) + 1);
    strcpy(currentAttr_copy, currentAttr);
    token = strtok(currentAttr_copy, " ");
    
    // force attr lower lowercase
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    // check if possible attribute or constraint name
    if ( is_keyword(token) == 0) { // for constraints 
        int constraint_success = constraint_check(currentAttr, token, attr_names, name_count, 
                attr_num, new_table);
        if (constraint_success == -1) {
            fprintf(stderr, "Invalid constraint: '%s'\n", currentAttr);
            return -1;
        }
    } 
    else { // for attributes
        int attribute_success = attribute_check(currentAttr, token, attr_names, 
                name_count, new_table);
        if (attribute_success == -1) {
            fprintf( stderr, "Invalid attribute: '%s'\n", currentAttr);
            return -1;
        }
    } 

    return 0;
}
