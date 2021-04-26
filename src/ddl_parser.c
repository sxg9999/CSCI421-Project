/*
 * Author: Dylan Cuprewich (dxc2961), Robert Gandley ()
 * Source file for DDL Parser
 * Implements the public functionality for the DDL Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"

int parse_ddl_statement( char* input_statement ) {
    char* statement = (char* )malloc( strlen( input_statement ) + 1);
    strcpy(statement, input_statement); 
//    enum statement_type stmt_type;
    enum db_type stmt_type;
    char* token;
    const char delimiter[] = " ";
    //printf("Parsing: '%s'\n", input_statement);
    
    // get first word in statement
    token = strtok(statement, delimiter);
    if (token == NULL) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement, empty", input_statement);
        return -1;
    }
    char* first_word = (char *)malloc(strlen(token)+1);
    strcpy(first_word, token);
    // make the token all lowercase
    for (int i = 0; first_word[i] != '\0'; i++) {
        if ( isalpha(first_word[i]) ) {
            first_word[i] = tolower(first_word[i]);
        }
    }
    first_word[strlen(first_word)] = '\0';
    //printf("First word lower: '%s'\n", first_word);


    // check for statement type 
    char* check_type;
    if (strlen(first_word) == strlen(DROP_START)) {
        check_type = DROP_START;
        stmt_type = DROP;
    } else if (strlen(first_word) == strlen(ALTER_START)) {
        check_type = ALTER_START;
        stmt_type = ALTER;
    } else if (strlen(first_word) == strlen(CREATE_START)) {
        check_type = CREATE_START;
        stmt_type = CREATE;
    } else {
        fprintf(stderr, "%s: '%s' in '%s'\n", 
            "Invalid DDL statement, invalid/missing statement type", first_word, input_statement);
        return -1;
    }
    int valid_first = strncmp(first_word, check_type, strlen(first_word));
    if (valid_first != 0) {
        fprintf(stderr, "%s: '%s' in '%s'\n", 
            "Invalid DDL statement, invalid/missing statement type", first_word, input_statement);
        return -1;
    }

    // check for 'table' as second word
    token = strtok(NULL, delimiter);

    if ( strlen(token) != strlen(TABLE)) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }

    // make second word all lower case
    char* second_word = (char *)malloc(strlen(token));
    strcpy(second_word, token);
    for (int i = 0; second_word[i] != '\0'; i++) {
        if ( isalpha(second_word[i]) ) {
            second_word[i] = tolower(second_word[i]);
        }
    }
    // check if second word=="table"
    int max_length = (strlen(second_word) > strlen(TABLE)) ? (strlen(second_word)) : (strlen(TABLE));
    int valid_second = strncmp(second_word, TABLE, max_length);
    if (valid_second != 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }

    // call one of the other parse funcs
    int result;

    if (stmt_type == DROP) {
        result = parse_drop_table_stmt(input_statement);
    } else if (stmt_type == ALTER) {
        result = parse_alter_table_stmt(input_statement);
    } else if (stmt_type == CREATE) {
        result = parse_create_table_stmt(input_statement);
    } else {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    if (result < 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
    }
    printf("\n\n");
    return result;
}
