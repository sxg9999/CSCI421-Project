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

int contraint_check(char** attr_names, char* contraint_name, int name_count);
int get_attrs_from_stmt(char* stmt, char** attributes, int* attr_count);
int attr_form_check(char* currentAttr, char** attr_names, int name_count, int is_first_attr);



/*
 * Most simple: CREATE TABLE BAZZLE( baz double PRIMARYKEY );
 * 
 */

int parse_create_table_stmt( char* input_statement ) {
    printf("Parsing Create STMT: '%s'\n", input_statement);

    char* statement = (char* )malloc( strlen( input_statement ) + 1);
    strcpy(statement, input_statement); 
    
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
    
    //get everything to left of first open parenthesis
    table_name = strtok(statement, "(");
    if ( null_check_str_create_stmt(table_name, input_statement) ) {
        return -1;
    }
    printf("Trying to create table with name: '%s'\n", table_name);

    // get all attributes
    int attr_success = get_attrs_from_stmt(statement, attributes, &attrCount);
    if ( attr_success == -1 ) {
        fprintf("Failed to get attributes from create statement: '%s'\n", statement);
        return -1;
    }

    // check if each attribute is form properly
    printf("Checking attributes and constraints validity...\n\n");
    for (int i = 0; i < attrCount; i++) {
        char* currentAttr = attributes[i];
        // check if attribute is a valid attribute/constraint
        int valid_attr = attr_form_check(currentAttr, attr_names, name_count, i);
        if (valid_attr == -1) {
            fprintf(stderr, "Invalid attribute: '%s'\n", currentAttr);
            return -1;
        } 
        printf("\n");
    }
    printf("Valid Create Table statement: '%s'\n", input_statement);

    free(attributes);
    return 0;
}

/**
 * Get the attributes/contraints from a create stmt.
 * ex. "CREATE TABLE BAZZLE( baz double PRIMARYKEY );" -> ["baz double PRIMARYKEY"]
 * @param stmt - the original create statement
 * @param attributes - 2d array of attribute/constraint strings
 * @param attrCount - ptr to the total count of attributes in the `stmt`
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
            fprintf(stderr, "Invalid token '%s' in stmt '%s'.\n");
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

/*
 * Checks if an attribute is valid
 */
int attr_form_check(char* currentAttr, char** attr_names, int name_count, int is_first_attr) {
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
    if ( is_keyword(token) ) { // for constraints 
        printf("Checking if '%s' is a constraint...\n", currentAttr);
        // if it is check if attribute is constraint
        if ( is_constraint(token) && is_first_attr == 0) {
            fprintf(stderr, "%s: '%s'\n", 
                "Invalid attribute definition. Attribute name is a keyword", token);
            return -1;
        }
        // check if foreignkey
        if ( strncmp(FOREIGN_CON, token, strlen(FOREIGN_CON)) == 0 ) {
            printf("Attribute is a foreign constraint: '%s'\n", token);
            // check for key attribute names
            int valid_constraint_name = contraint_check(attr_names, token, name_count);
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
                if ( is_keyword(token) ) {
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
                if ( is_keyword(token) ) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid constraint definition. Constraint name is a keyword", token);
                    return -1;
                }
                int is_defined = 0;
                for (int i = 0; i < name_count; i++){
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
        printf("Checking if '%s' is an attribute...\n", currentAttr);
        attr_names[name_count] = (char*)malloc( strlen( token ) + 1);
        strcpy(attr_names[name_count], token);
        printf("Attribute name: '%s'\n", attr_names[name_count]);
        
        name_count += 1;
        if (name_count%10 == 0) { 
            // resize attr_name, add 10 slots
            attr_names = realloc(attr_names, (name_count+10)* sizeof(int*));
        }
        
        token = strtok(NULL, " "); // get type of attribute
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        
        // check if valid attribute type
        printf("Checking if type attribute '%s' is valid...\n", token);
        if ( !is_attr_type(token) ) {
            fprintf(stderr, "%s: '%s'\n", 
                "Invalid attribute definition. Attribute type is invalid", token);
            return -1;
        } else {
            printf("'%s' is a valid attribute type.", token);
        }
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
            if ( !is_attr_con(token) ) {
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
                constraints_used[constraints_count] = strdup(token);
                constraints_count += 1;
            }
        }
    } 
    printf("\n");
    return 0;
}


/**
 * 
 */
int contraint_check(char** attr_names, char* contraint_name, int name_count) {
    while ( (contraint_name = strtok(NULL, " ")) ) {
        if (contraint_name[0] == ')') {
            break;
        }
        if ( is_keyword(contraint_name) ) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid constraint definition. Constraint name is a keyword", contraint_name);
            return -1;
        }
        int is_defined = 0;
        for (int i = 0; i < name_count; i++){
            if ( strcmp(attr_names[i], contraint_name) == 0 ) {
                is_defined = 1;
                break;
            }
        }
        if ( is_defined == 0 ) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid constraint parameter. Attribute name undefined", contraint_name);
            return -1;
        }
    }

    return 0;
}

