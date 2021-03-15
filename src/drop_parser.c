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


int parse_drop_table_stmt( char* input_statement ) {
    // copy string
    char* statement = strdup(input_statement);
    // move input string pointer to skip beginning
    statement += strlen(DROP_START) + 1 +
                        strlen(TABLE) + 1;

    // printf("Drop STMT: %s\n", statement);
    const char delimiter[2] = " \n";
    char* table_name;
    // whether or not ';' directly follows <NAME> '<NAME>; vs <NAME> ;'
    // 1 = space between <NAME> and ';'
    int name_space_semi = 1;

    // get table name parameter
    table_name = strtok(statement, delimiter);
    if (table_name[strlen(table_name)-1] == STMT_END_CHAR) {
        table_name[strlen(table_name)-1] = '\0';
        name_space_semi = 0;
    }

    char lower_char;
    for (int i = 0; table_name[i] != '\0'; i++) {
        lower_char = tolower(table_name[i]);
        table_name[i] = lower_char;
    }

    // check if table name is not a keyword
    if (is_keyword(table_name)) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid table name; table name is a keyword", table_name);
        return -1;
    } 

    char* check_rest = strtok(NULL, delimiter);
    if (check_rest != NULL && check_rest[0] != STMT_END_CHAR && name_space_semi) {

        fprintf(stderr, "%s: '%s'\n", 
            "Invalid table name", table_name);
        return -1;
    }

    printf("Valid Drop Table statement: '%s'\n", input_statement);
    return 0;
}