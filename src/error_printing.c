/*
 * Author: Dylan Cuprewich (dxc2961)
 * Source file for CSCI421 Group Project DDL Parser Helpers
 * Implements the public functionality Printing Errors.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int null_check_str_statement(char* words, char* source_text) {
    if ( words == NULL) {
        fprintf(stderr, "%s: '%s'\nUsage:\n%s\n", 
            "Invalid Create statement", source_text,
            "create table <name>( "
            "<a_name> <a_type> <constraint_1> ... <constraint_N>, "
            "[primarykey( <a_1> ... <a_N> ),] "
            "[unique( <a_1> ... <a_N> ),] "
            "[foreignkey( <a_1> ... <a_N> ) "
            "references <r_name>( <r_1> ... <r_N> )] "
            ");");
        return -1;
    }

    return 0;
}

int int_equality_check(int a, int b, char* source_text) {
    if ( a != b ) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", source_text);
        return -1;
    }

    return 0;
}

