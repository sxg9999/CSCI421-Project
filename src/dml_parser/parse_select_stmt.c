

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../../include/parse_insert_stmt.h"
#include "../../include/parse_insert_stmt_helpers.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_attr_helper.h"
#include "../../include/storagemanager.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/storage_mediator/storage_mediator.h"
#include "../../include/shunting_yard.h"

int parse_select_ids(char* token, char** column_names, int* column_count, int* star_select);
int parse_select_columns();
int parse_select_where();
int parse_select_order_by();

int parse_select_stmt(char* select_stmt) {
    printf("Parsing select statement '%s'\n", select_stmt);
    char* stmt = strdup(select_stmt);
    // parse the select keyword
    char* token = strtok(stmt, " ");
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    printf("first token: '%s'\n", token);
    if (strcmp(token, "select") != 0) {
        fprintf("Invalid: missing 'select' keyword: '%s'\n", select_stmt);
        return -1;
    }

    // parse the select attributes (or the *)
    char** column_names = (char**) malloc(sizeof(char*));
    int column_count = 0;
    int star_select = 0;
    int select_success = parse_select_ids(token, column_names, &column_count, &star_select);
    if (select_success != 0) {
        fprintf(stderr, "Invalid: error getting select ids: '%s'\n", select_stmt);
        return -1;
    }

    // show columns found
    printf("COLUMNS '%d':\n", column_count);
    for (int i = 0; i < column_count; i++) {
        printf("\t%d: '%s'\n", i, column_names[i]);
    }

    // check if there were any columns
    if (column_count == 0) {
        fprintf(stderr, "Invalid: No column names given in select statement: '%s'\n", select_stmt);
        return -1;
    }
    // check if '*' and any other columns
    if (star_select == 1 && column_count > 1) {
        fprintf(stderr, "Invalid: '*' is only allowed by itself: '%s'\n", select_stmt);
        return -1;
    }

    // check for 'from' key word
    if (strcmp(token, "from") != 0) {
        fprintf(stderr, "Invalid: missing 'from' keyword: '%s'\n", select_stmt);
        return -1;
    }
    
    // parse the table name (verify it exists)
    char** table_names = (char**) malloc(sizeof(char*));
    int table_count = 0;
    while((token = strtok(NULL, ", ")) != NULL) {
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        printf("Token: '%s'\n", token);
        // check for from keyword
        if (strcmp(token, "where") == 0) {
            break;
        }

        table_count += 1;
        table_names = (char**) realloc(table_names, table_count * sizeof(char*));
        table_names[table_count-1] = (char*) malloc( strlen(token) );
        strcpy(table_names[table_count-1], token);
    }
    // show tables found
    printf("Tables '%d':\n", table_count);
    for (int i = 0; i < table_count; i++) {
        printf("\t%d: '%s'\n", i, table_names[i]);
    }
    
    // check for 'where' key word
    int where_clause_present = 0;
    if (strcmp(token, "where") == 0) {
        where_clause_present = 1;
    }    
    
    // check if the columns are in the table

    // get the where keyword (if there)
    char* where_clause = (char**) malloc(sizeof(char*));
    if (where_clause_present == 0 ) {
        where_clause = "where true";
    } 
    else {
        // get the where clause
    }
    
    // parse the where clause and make the tree
    
    // for each record for the table
        // if true place in output
        // if false ignore it

    // in the output remove the attributes not in the select

    return 0;
}

int parse_select_ids(char* token, char** column_names, int* column_count, int* star_select) {
    while((token = strtok(NULL, ", ")) != NULL) {
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        printf("Token: '%s'\n", token);

        // check for from keyword
        if (strcmp(token, "from") == 0) {
            break;
        }
        if (strcmp(token, "*") == 0) {
            star_select = 1;
        }
        // check if name is valid

        *column_count += 1;
        column_names = (char**) realloc(column_names, (*column_count) * sizeof(char*));
        column_names[(*column_count)-1] = (char*) malloc( strlen(token) );
        strcpy(column_names[(*column_count)-1], token);
    }
    
    return 0;
}

int parse_select_columns() {
    return 0;
}

int parse_select_where() {
    return 0;
}

int parse_select_order_by() {
    return 0;
}