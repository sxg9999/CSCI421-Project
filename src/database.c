#include <stdio.h>
#include <stdlib.h>

#include "../include/database.h"
#include "../include/ddl_parser.h"

int main(int argc, char* argv[] ) {
    char test_string[] = "DROP TABLE DRO;";
    printf("Input: %s\n", test_string);
    parse_ddl_statement(test_string);
}