/*
 * Author: Dylan Cuprewich (dxc2961)
 * Source file for DDL Parser
 * Implements the public functionality for the DDL Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser_helper.h"
#include "../include/ddl_parser.h"

int parse_ddl_statement( char* statement ) {
    char* token;
    const char delimiter[2] = " ";
    char* str = statement;
    char c = *(str++);
    const char* test = "test";
    const char* test2 = "tested"; 
    
    // get first word in statement
    token = strtok(statement, delimiter);
    if (token == NULL) {
        fprintf(stderr, "%s: '%s'\n", "Invalid DDL statement", statement);
        return -1;
    }
    printf("%s\n", token);
    token = "poop";
    // make the token all lowercase
    while(*token) {
        // check if char is a letter
        if (!isalpha(*token)) {
            fprintf(stderr, "%s: '%s'\n", 
               "Invalid DDL statement", statement);
            return -1;
        }
        *token = tolower(*token);
        token++;
    }

    printf("%s\n", token);
    // check first word 

    // check second word
    // call one of the other parse funcs
    // int ret = strncmp(test, test2, strlen(test));
    // printf("%d", ret);


    // while (c != '\0') {
    //     printf("%c\n", c);
    //     c = *(str++);
    // }

    return 0;
}

int parse_create_table_stmt( char* statement ) {
    return 0;
}

int parse_drop_table_stmt( char* statement ) {
    return 0;
}

int parse_alter_table_stmt( char* statement ) {
    return 0;
}