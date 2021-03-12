/*
 * Author: Dylan Cuprewich (dxc2961)
 * Source file for DDL Parser
 * Implements the public functionality for the DDL Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"

int parse_ddl_statement( char* input_statement ) {
    char* statement = strdup(input_statement);
    enum statement_type stmt_type;
    char* token;
    const char delimiter[2] = " ";
    
    // get first word in statement
    token = strtok(statement, delimiter);
    if (token == NULL) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    char* first_word = (char *)malloc(strlen(token));
    char a;
    // make the token all lowercase
    for (int i = 0; token[i] != '\0'; i++) {
        a = tolower(token[i]);
        first_word[i] = a;
    }

    // check for statement type 
    char* check_type;
    if (strlen(first_word) == DROP) {
        check_type = DROP_START;
        stmt_type = DROP;
    } else if (strlen(first_word) == ALTER) {
        check_type = ALTER_START;
        stmt_type = ALTER;
    } else if (strlen(first_word) == CREATE) {
        check_type = CREATE_START;
        stmt_type = CREATE;
    } else {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    int valid_first = strncmp(first_word, check_type, strlen(first_word));
    if (valid_first != 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    free(first_word);
    //printf("valid first word: %s\n", token);


    // check for table second word
    token = strtok(NULL, delimiter);
    if ( strlen(token) != strlen(TABLE)) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    // make second word all lower case
    char* second_word = (char *)malloc(strlen(token));
    char b;
    for (int i = 0; token[i] != '\0'; i++) {
        b = tolower(token[i]);
        second_word[i] = b;
    }
    // check if second word=="table"
    int valid_second = strncmp(second_word, TABLE, 
        strlen(second_word));
    if (valid_second != 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    free(second_word);
    //printf("valid second word: %s\n", token);
    free(statement);
    
    // call one of the other parse funcs
    int result;

    if (stmt_type == DROP) {
        result = parse_drop_table_stmt(input_statement);
    } else if (stmt_type == ALTER) {
        result = parse_alter_table_stmt(input_statement);
    } else if (stmt_type == CREATE) {
        result = parse_create_table_stmt(input_statement);
    } else {
        // printf("%s: %s\n", 
        //    "Something went wrong. Check type", check_type);
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    if (result < 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
    }
    return result;
}

int parse_create_table_stmt( char* input_statement ) {
    char* statement = strdup(input_statement);
    statement += strlen(CREATE_START) + 1 + strlen(TABLE) + 1;

    //Create Table setup
    printf("Create STMT: %s\n", statement);
    const char openDelimiter[2] = "(";
    const char closeDelimiter[2] = ")";
    const char attDelimiter[2] = ",";
    const char delimiter[2] = " ";
    char* table_name;
    char* inParen;
    
    //get everything to left of first open parenthesis
    table_name = strtok(statement, openDelimiter);
    //everything (except maybe later constraints?) to the right. Replace with comma delim + skip length of table_name?
    inParen = strtok(NULL, openDelimiter);
    //how to retrieve up until constraints?
    
    table_name = strtok(table_name, delimiter);

    

    char* remainder = strtok(NULL, delimiter);
    if (remainder != NULL)
    return 0;
}

int parse_drop_table_stmt( char* input_statement ) {
    char* statement = strdup(input_statement);
    // "DROP/ALTER/CREATE TABLE <NAME>"
    // move input string pointer to skip beginning
    statement += strlen(DROP_START) + 1 +
                        strlen(TABLE) + 1;

    // printf("Drop STMT: %s\n", statement);
    const char delimiter[2] = " ";
    char* table_name;
    table_name = strtok(statement, delimiter);
    // printf("Table name: %s\n", table_name);


    char* check_rest = strtok(NULL, delimiter);
    if (check_rest != NULL) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid drop table name", input_statement);
        return -1;
    }

    return 0;
}

int parse_alter_table_stmt( char* statement ) {
    printf("Alter STMT: %s\n", statement);
    return 0;
}