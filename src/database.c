#include <stdio.h>
#include <stdlib.h>

#include "../include/database.h"

int main(int argc, char* argv[] ) {
    char test_string[] = "DROP TABLE";
    parse_ddl_statement(test_string);
}