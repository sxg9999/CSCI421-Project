/*
 * Author: Dylan Cuprewich (dxc2961), Robert Gandley ()
 * Source file for parsing Create statements
 * Implements the public functionality for the Create Statement Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"
#include "../include/error_printing.h"
#include "../include/catalog/catalog_structs.h"
#include "../include/catalog/catalog.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/db_types.h"

int contraint_check(char** attr_names, char* contraint_name, int name_count);
int get_attrs_from_stmt(char* stmt, char** attributes, int* attr_count);
int attr_form_check(char* currentAttr, char** attr_names, int* name_count, int is_first_attr,
        struct catalog_table_data* new_table);
int attribute_check(char* currentAttr, char* token, char** attr_names, int* name_count,
        struct catalog_table_data* new_table);
int char_type_check(char* char_attr, struct attr_data* new_attribute, enum db_type attr_type);



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
    
    // catalog hashtable
    struct hashtable* catalog_ht = catalog_get_ht();
    // new table struct
    struct catalog_table_data* new_table = 
            (struct catalog_table_data*) malloc(sizeof(struct catalog_table_data));
    // set table number
    new_table->table_num = catalog_ht->size;
    new_table->attr_ht = ht_create(12, 0.75);


    //get everything to left of first open parenthesis
    table_name = strtok(statement, "(");
    if ( null_check_str_create_stmt(table_name, input_statement) ) {
        fprintf(stderr, "Invalid statement\n");
        return -1;
    }
    printf("Trying to create table with name: '%s'\n", table_name);

    // get all attributes
    int attr_success = get_attrs_from_stmt(statement, attributes, &attrCount);
    if ( attr_success == -1 ) {
        fprintf(stderr, "Failed to get attributes from create statement: '%s'\n", statement);
        return -1;
    }

    // check if each attribute is form properly
    printf("Checking attributes and constraints validity...\n\n");
    for (int i = 0; i < attrCount; i++) {
        char* currentAttr = attributes[i];
        // check if attribute is a valid attribute/constraint
        int valid_attr = attr_form_check(currentAttr, attr_names, &name_count, i, new_table);
        if (valid_attr == -1) {
            fprintf(stderr, "Invalid attribute: '%s'\n", currentAttr);
            return -1;
        } 
        printf("\n");
    }
    printf("Valid Create Table statement: '%s'\n", input_statement);

    // set table name 
    new_table->table_name = (char *)malloc( strlen(table_name) + 1 );
    strcpy(new_table->table_name, table_name); 

    // add table to catalog
    printf("Adding table '%s' to catalog...\n", new_table->table_name);
    int add_to_catalog_success = catalog_add_table(new_table);
    if (add_to_catalog_success == -1) {
        fprintf( stderr, "Error trying to add table '%s' to the catalog.\n", new_table->table_name);
        return -1;
    }

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
        printf("New attribute: '%s'\n", newAtrribute);
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
 * Checks if an attribute is valid, and creates it.
 * @param currentAttr the current attribute being validated
 * @param attr_names a list of attribute names
 * @param name_count total of attributes
 * @param is_first_attr whether or not this is the first attribute
 * @return 0 on success, -1 on failure
 */
int attr_form_check(char* currentAttr, char** attr_names, int* name_count, int is_first_attr,
        struct catalog_table_data* new_table) 
{
    printf("Checking if attribute/contraint '%s' is valid...\n", currentAttr);
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
    // check if valid attribute name
    if ( is_keyword(token) == 0) { // for constraints 
        printf("Checking if '%s' is a constraint...\n", currentAttr);
        // if it is check if attribute is constraint
        if ( is_constraint(token) == 0 && is_first_attr == 0) {
            fprintf(stderr, "%s: '%s'\n", 
                "Invalid attribute definition. Attribute name is a keyword", token);
            return -1;
        }
        // check if foreignkey
        if ( strncmp(FOREIGN_CON, token, strlen(FOREIGN_CON)) == 0 ) {
            printf("Attribute is a foreign constraint: '%s'\n", token);
            // check for key attribute names
            int valid_constraint_name = contraint_check(attr_names, token, *name_count);
            if ( valid_constraint_name == -1 ) {
                fprintf(stderr, "Invalid constraint '%s'\n", token);
                return -1;
            }
            // check for 'references'
            token = strtok(NULL, " ");
            // printf("Token: '%s'\n", token);
            if ( strcmp(REFERENCES_CON, token) != 0) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid constraint definition. Missing 'references' keyword", token);
                    return -1;
            }

            // <relation>( check
            token = strtok(NULL, " ");
            // printf("Token: '%s'\n", token);
            if ( token[strlen(token) - 1] != '(' ) {
                fprintf(stderr, "%s: '%s'\n", 
                    "Invalid constraint definition. Missing '('", token);
                    return -1;
            }

            // <r_1> ... <r_n>
            while ( (token = strtok(NULL, " ")) ) {
                if (token[0] == ')') {
                    break;
                }
                // printf("<r_n>: %s\n", token);
                if ( is_keyword(token) == 0) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid constraint definition. Constraint name is a keyword", token);
                    return -1;
                }
            }
        } else {
            // check if valid constraint def
            while ( (token = strtok(NULL, " ")) ) {
                if (token[0] == ')') {
                    break;
                }
                if ( is_keyword(token) == 0) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid constraint definition. Constraint name is a keyword", token);
                    return -1;
                }
                int is_defined = 0;
                for (int i = 0; i < *name_count; i++){
                    if ( strcmp(attr_names[i], token) == 0 ) {
                        is_defined = 1;
                        break;
                    }
                }
                if ( is_defined == 0 ) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid constraint parameter. Attribute name undefined", token);
                    return -1;
                }
            }
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
    printf("\n");
    return 0;
}


int attribute_check(char* currentAttr, char* token, char** attr_names, int* name_count, 
        struct catalog_table_data* new_table) 
{
    printf("Checking if '%s' is a valid attribute...\n", currentAttr);
    attr_names[*name_count] = (char*)malloc( strlen( token ) + 1);
    strcpy(attr_names[*name_count], token);
    printf("Attribute name: '%s'\n", attr_names[*name_count]);
    // attribute struct
    struct attr_data* new_attribute = 
            (struct attr_data*) malloc(sizeof(struct attr_data));
    // set attribute index
    new_attribute->index = *name_count;
    // set attribute name
    new_attribute->attr_name = attr_names[*name_count];
    // set default num of constraints on attribute to 0
    new_attribute->num_of_constr = 0;
    // TODO: set type of attribute, and size
    // TODO: check for constraint on attribute


    *name_count += 1;
    if ((*name_count)%10 == 0) { 
        // resize attr_name, add 10 slots
        attr_names = realloc(attr_names, ((*name_count)+10)* sizeof(int*));
    }
    
    token = strtok(NULL, " "); // get type of attribute
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    
    // check if valid attribute type
    printf("Checking if attribute type '%s' is valid...\n", token);
    if ( is_attr_type(token) == -1 ) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid attribute definition. Attribute type is invalid", token);
        return -1;
    } 
    printf("'%s' is a valid attribute type.\n", token);
    // get attribute type of attribute
    enum db_type attribute_type = typeof_kw(token);
    if (attribute_type == 0) {
        fprintf( stderr, "Unknown/Invalid attribute type: '%s'! \n", token);
        return -1;
    }
    if (attribute_type == 13 || attribute_type == 14) {
        int char_check_success = char_type_check(token, new_attribute, attribute_type);
        if (char_check_success == -1) {
            fprintf(" vchar/char length check failed for '%s' in '%s'\n", token, currentAttr);
        } 
    }
    // set type of attribute
    new_attribute->type = attribute_type;

    // check for constraints on attribute
    char* constraints_used[3];
    int constraints_count = 0;
    while ( (token = strtok(NULL, " ")) ) {
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        if (token[0] == ')') {
            break;
        }
        if ( is_attr_con(token) == -1 ) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid attribute definition. Attribute constraint is invalid", token);
            return -1;
        } else {
            for (int i = 0; i < constraints_count; i++ ) {
                if (strcmp(token, constraints_used[i]) == 0) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid attribute definition. Attribute constraint already used", token);
                    return -1;
                }
            }
            constraints_used[constraints_count] = (char*)malloc( strlen( token ) + 1 );
            strcpy(constraints_used[constraints_count], token);
            constraints_count += 1;
        }
    }
    printf("'%s' is a valid attribute\n", currentAttr);


    char* msg = "Attribute:\n\tAttribute name: '%s'\n\tIndex: '%d'\n\tAttribute size: '%d'\n\
    Attribute type: '%d' \n\tConstraint count: '%d'";
    printf(msg, new_attribute->attr_name, new_attribute->index, new_attribute->attr_size,
            new_attribute->type, new_attribute->num_of_constr);

    // success
    return 0;
}

/**
 * Check for the specified size of vchar/char attribute.
 */
int char_type_check(char* token, struct attr_data* new_attribute, 
        enum db_type attribute_type) {
    char* char_attr = (char *)malloc( strlen(token) + 1 );
    strcpy(char_attr, token);
    printf("Attribute '%s' is a vchar/char!\n", char_attr);
    
    return 0;
}

/**
 * Check if a contraint is valid
 */
int contraint_check(char** attr_names, char* constraint_name, int name_count) {
    while ( (constraint_name = strtok(NULL, " ")) ) {
        if (constraint_name[0] == ')') {
            break;
        }
        if ( is_keyword(constraint_name) == 0) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid constraint definition. Constraint name is a keyword", constraint_name);
            return -1;
        }
        int is_defined = 0;
        for (int i = 0; i < name_count; i++){
            if ( strcmp(attr_names[i], constraint_name) == 0 ) {
                is_defined = 1;
                break;
            }
        }
        if ( is_defined == 0 ) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid constraint parameter. Attribute name undefined", constraint_name);
            return -1;
        }
    }
    printf("Constraint '%s' is valid!", constraint_name);
    return 0;
}

