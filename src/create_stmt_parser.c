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


/*
 * Most simple: CREATE TABLE BAZZLE( baz double PRIMARYKEY );
 * 
 */

int parse_create_table_stmt( char* input_statement ) {
    printf("Parsing Create STMT: '%s'\n", input_statement);

    char* statement = (char* )malloc( strlen( input_statement ) + 1);
    strcpy(statement, input_statement); 
    
    statement += strlen(CREATE_START) + 1 
                + strlen(TABLE) + 1;

    //Create Table setup
    char* table_name;
    // resize as needed
    char** attributes = malloc(10 * sizeof(char*));
    char** attr_names = malloc(10 * sizeof(char*));
    int attrCount = 0;
    int name_count = 0;
    char* newAtrribute;
    int is_last_attr = 0;
    
    //get everything to left of first open parenthesis
    table_name = strtok(statement, "(");
    if ( null_check_str_create_stmt(table_name, input_statement) ) {
        return -1;
    }

    // get all attributes
    while (is_last_attr == 0) {
            // get next attribute
        newAtrribute = strtok(NULL, ",");
        // check if there is any attributes
        if ( null_check_str_create_stmt(newAtrribute, input_statement) ) {
            return -1;
        }
        // check if last attribute
        if (  newAtrribute[strlen(newAtrribute) - 1] == ';' ) {
            newAtrribute[strlen(newAtrribute) - 1] = '\0';
            is_last_attr = 1;
        }
        // printf("New attribute: '%s'\n", newAtrribute);
        // save attribute
        attributes[attrCount] = strdup(newAtrribute);
        attrCount += 1;
        if (attrCount%10 == 0) {
            attributes = realloc(attributes, (attrCount+10)* sizeof(int*));
        }
    }

    // check if each attribute is form properly
    for (int i = 0; i < attrCount; i++) {
        // check if new attribute, or constraint
        char* currentAttr = attributes[i];
        // printf("Attribute line: '%s'\n", currentAttr);
        char* token;
        token = strtok(currentAttr, " ");
        // force attr lower lowercase
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        // check if valid attribute name
        // printf("Prob: '%s', %d\n", token, is_keyword(token));

        if ( is_keyword(token) ) {
            // if it is check if attribute is constraint
            if ( is_constraint(token) && i == 0) {
                fprintf(stderr, "%s: '%s'\n", 
                    "Invalid attribute definition. Attribute name is a keyword", token);
                return -1;
            }
            // check if foreignkey
            if ( strncmp(FOREIGN_CON, token, strlen(FOREIGN_CON)) == 0 ) {
                // printf("Is foreign constraint: %s\n", token);
                // check for key attribute names
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
        } else { // check if attribute def is valid
            // get attr type
            attr_names[name_count] = strdup(token);
            name_count += 1;
            if (name_count%10 == 0) {
                attr_names = realloc(attr_names, (name_count+10)* sizeof(int*));
            }
            token = strtok(NULL, " ");
            for (int i = 0; token[i] != '\0'; i++) {
                if ( isalpha(token[i]) ) {
                    token[i] = tolower(token[i]);
                }
            }
            // check if valid attribute type
            if ( !is_attr_type(token) ) {
                fprintf(stderr, "%s: '%s'\n", 
                    "Invalid attribute definition. Attribute type is invalid", token);
                return -1;
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
                // printf("Possible constraint: '%s'\n", token);
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
    }
    printf("Valid Create Table statement: '%s'\n", input_statement);

    free(attributes);
    return 0;
}

