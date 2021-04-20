/*
 * Author: Dylan Cuprewich (dxc2961)
 * Source file for DML Parser
 * Implements the public functionality for the DML Parser
 */
#include <stdio.h>
#include <string.h>
#include "../include/dml_parser.h"
#include "../include/db_types.h"

int parse_dml_statement( char* statement) {
    char func_loc_str[] = "(dml_parser.c/parse_dml_statement)";
    int error_code = 0;
    printf("Parsing DML statement: '%s'\n", statement);

    char* ptr = strchr(statement, ' ');
    if(ptr == NULL){
        printf("%s %s\n", func_loc_str, "Invalid statement: Expected a space after the statement keyword");
        return -1;
    }
    int end_index = ptr - statement - 1;
    char* keyword_str = strdup(statement, end_index);

    enum db_type kw_type = typeof_kw(keyword_str);
    switch(kw_type){
        case INSERT:
            break;
        case DELETE:
            break;
        case UPDATE:
            break;
        default:
            printf("%s %s \"%s\" is invalid\n", func_loc_str, "Error: Keyword", keyword_str);
            error_code = -1;
            break;

    }



    return error_code;
}

int parse_dml_query( char* query, union record_item*** result) {
    return 0;
}
 