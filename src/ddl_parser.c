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
    enum statement_type stmt_type;
    char* token;
    const char delimiter[2] = " ";
    char* str = statement;
    char c = *(str++);
    //const char* test = "test";
    //const char* test2 = "tested"; 
    
    // get first word in statement
    token = strtok(statement, delimiter);
    if (token == NULL) {
        fprintf(stderr, "%s: '%s'\n", "Invalid DDL statement", statement);
        return -1;
    }
    printf("Old: %s\n", token);
    char* first_word = (char *)malloc(strlen(token));
    char a;
    // make the token all lowercase
    for (int i = 0; token[i] != '\0'; i++) {
        a = tolower(token[i]);
        first_word[i] = a;
    }
    printf("New: %s\n", first_word);

    // check for statement type 
    char* check_type;
    if (strlen(first_word) == DROP) {
        check_type = DROP_START;
    } else if (strlen(first_word) == ALTER) {
        check_type = ALTER_START;
    } else if (strlen(first_word) == CREATE) {
        check_type = CREATE_START;
    } else {
        fprintf(stderr, "%s: '%s'\n", "Invalid DDL statement", statement);
        return -1;
    }
    int valid_first = strncmp(first_word, check_type, strlen(first_word));
    printf("valid: %d\n", valid_first==0);

    
    


    // check for table second word
    // call one of the other parse funcs


    // int ret = strncmp(test, test2, strlen(test));
    // printf("%d", ret);


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