/*
 * Author: Dylan Cuprewich (dxc2961)
 * Source file for parsing drop statements
 * Implements the public functionality for the Drop Statement Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"
#include "../include/error_printing.h"
#include "../include/catalog/catalog.h"

int execute_drop_stmt(char* table_name);

int parse_drop_table_stmt( char* input_statement ) {
    printf("Parsing Drop STMT: '%s'\n", input_statement);

    // copy string
    char* statement = (char* )malloc( strlen( input_statement ) + 1);
    strcpy(statement, input_statement); 
    // move input string pointer to skip beginning
    statement += strlen(DROP_START) + 1 +
                        strlen(TABLE) + 1;

    const char delimiter[2] = " \n";
    char* table_name;

    // get table name parameter
    table_name = strtok(statement, delimiter);
    if (table_name[strlen(table_name)-1] == STMT_END_CHAR) {
        table_name[strlen(table_name)-1] = '\0';
    }

    for(int i = 0; i < strlen(table_name); i++){
        if (isalpha(table_name[i]) ) {
            table_name[i] = tolower(table_name[i]);
        }
    }

    // check if table name is not a keyword
    if (is_keyword(table_name)) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid table name; table name is a keyword", table_name);
        return -1;
    } 

    char* check_rest = strtok(NULL, delimiter);
    if (check_rest != NULL && check_rest[0] != STMT_END_CHAR) {

        fprintf(stderr, "%s: '%s'\n", 
            "Invalid table name", table_name);
        return -1;
    }

    printf("Valid Drop Table statement: '%s'\n", input_statement);

    int drop_success = execute_drop_stmt(table_name);

    return drop_success;
}

int execute_drop_stmt(char* table_name) {
    printf("\nDropping table: '%s'\n", table_name);
    // table is not a keyword
    // check if a table with name exists
    int drop_success = catalog_remove_table(table_name);

    // drop table with name
    if ( drop_success ) {
        printf("\ntable '%s' dropped successfully.\n", table_name);
    }
    return drop_success;
}
