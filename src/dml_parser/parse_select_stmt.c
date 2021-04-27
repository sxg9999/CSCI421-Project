

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../../include/database.h"
#include "../../include/parse_insert_stmt.h"
#include "../../include/parse_insert_stmt_helpers.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_attr_helper.h"
#include "../../include/storagemanager.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/storage_mediator/storage_mediator.h"
#include "../../include/shunting_yard.h"

int parse_select_ids(char** token, char** column_names, int* column_count, int* star_select);
int parse_select_tables(char** token, char** table_names, int* table_count);
int parse_select_where(char** token, char** where_clause, int* order_clause_present);
int parse_select_order_by();
int test_select_example();

int test_select_example() {
    char* create_table = "CREATE TABLE BAZZLE( baz integer PRIMARYKEY );";
    char* insert_tuple = "insert into bazzle values (1);";
    char* get_bazzle_tuples = "get bazzle;";
    char* select_test = "select baz \
from bazzle \
where baz > 5 \
order by baz;";
    char* select_test1 = "select baz \
from bazzle \
where baz > 5;";

    execute(create_table);
    execute(insert_tuple);
    execute(get_bazzle_tuples);
    execute(select_test1);
    return 0;
}

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
        fprintf(stderr, "Invalid: missing 'select' keyword: '%s'\n", select_stmt);
        return -1;
    }

    // parse the select attributes (or the *)
    char** column_names = (char**) calloc(1, sizeof(char*));
    int column_count = 0;
    int star_select = 0;
    int select_success = parse_select_ids(&token, column_names, &column_count, &star_select);
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
    char** table_names = (char**) calloc(1, sizeof(char*));
    int table_count = 0;
    int from_success = parse_select_tables(&token, table_names, &table_count);
    if (from_success != 0) {
        fprintf(stderr, "Invalid: error getting select tables: '%s'\n", select_stmt);
        return -1;
    }
    if (table_count == 0) {
        fprintf(stderr, "Invalid: No table names given in select statement: '%s'\n", select_stmt);
        return -1;
    }

    // show tables found
    printf("Tables '%d':\n", table_count);
    for (int i = 0; i < table_count; i++) {
        printf("\t%d: '%s' ", i, table_names[i]);
        // check if tables exist
        if ( catalog_contains(table_names[i]) == 0) { // table doesn't exist
            printf("doesn't exists\n");

            fprintf(stderr, "Invalid: table '%s' does not exist\n", table_names[i]);
            return -1;
        }
        printf("exists\n");
    }
    
    // TODO: check if ids exist in their table
    // TODO: check for multiple tables

    // check for 'where' key word
    int where_clause_present = 0;
    int order_clause_present = 0;
    char* where_clause = (char*) calloc(1, sizeof(char));
    char* order_clause = (char*) calloc(1, sizeof(char));

    if (strcmp(token, "where") == 0) {
        where_clause_present = 1;
    }    

    
    // TODO: check if the columns are in the table
    // look at catalog.c

    // get the where keyword (if there)
    if (where_clause_present == 0 ) { // default where_clause
        where_clause = "where true";
    } 
    else { // get where clause
        where_clause = "where ";
        int where_success  = parse_select_where(&token, &where_clause, &order_clause_present);
        if (where_success != 0) {
            fprintf(stderr, "Invalid: error getting where clause: '%s'\n", select_stmt);
            return -1;
        }
        
    }    
    if (strcmp(where_clause, "where ") == 0) { // check for empty where clause
        fprintf(stderr, "Invalid empty where clause: '%s'\n", select_stmt);
        return -1;
    }
    if (where_clause[strlen(where_clause) - 1] == ';') { // remove possible ending ';'
        where_clause[strlen(where_clause) - 1] = '\0';
    }
    printf("Where clause: '%s'\n", where_clause);

    // TODO: get order by clause
    if ( order_clause_present == 1) {
        int order_success = parse_select_order_by();
    }
    // TODO: check if order by empty

    // TODO: parse the where clause and make the tree
    
    // TODO: get records for table
    // TODO: for each record for the table
        // TODO: if true place in output
        // TODO: if false ignore it

    // TODO: in the output remove the attributes not in the select
    // TODO: create a temp copy of table
    // TODO: drop some of the columns

    printf("Select stmt '%s' was successful!\n", select_stmt);
    return 0;
}

/**
 * Parse the column names/ids from the select statement.
 * @param token ptr to strtok of select stmt
 * @param column_names list of column names
 * @param column_count ptr to count of column names
 * @param star_select ptr to whether there is a '*' present (0 for no, 1 for yes)
 * @return 0 if parse was successful, else -1 for failure
 */
int parse_select_ids(char** token, char** column_names, int* column_count, int* star_select) {
    while(((*token) = strtok(NULL, ", ")) != NULL) {
        for (int i = 0; (*token)[i] != '\0'; i++) {
            if ( isalpha((*token)[i]) ) {
                (*token)[i] = tolower((*token)[i]);
            }
        }

        // check for from keyword
        if (strcmp(*token, "from") == 0) {
            return 0;
        }
        if (strcmp(*token, "*") == 0) {
            *star_select = 1;
        }
        // check if name is valid
        //printf("column name: '%s'\n", *token);
        *column_count += 1;
        column_names = (char**) realloc(column_names, (*column_count) * sizeof(char*));
        column_names[(*column_count)-1] = (char*) calloc(strlen((*token)), sizeof(char) );
        strcpy(column_names[(*column_count)-1], (*token));
    }
    
    return 0;
}

/**
 * Parse the table names from the select statement.
 * @param token ptr to strtok of select stmt
 * @param table_names list of table names
 * @param table_count ptr to count of table names
 * @return 0 if parse was successful, else -1 for failure
 */
int parse_select_tables(char** token, char** table_names, int* table_count) {
    while(((*token) = strtok(NULL, ", ")) != NULL) {
        for (int i = 0; (*token)[i] != '\0'; i++) { // lowercase token
            if ( isalpha((*token)[i]) ) {
                (*token)[i] = tolower((*token)[i]);
            }
        }
        //printf("table name Token: '%s'\n", (*token));
        if (strcmp((*token), "where") == 0) { // check for from keyword
            break;
        }

        // add table name to table_names
        (*table_count) += 1;        // incr table count
        table_names = (char**) realloc(table_names, (*table_count) * sizeof(char*)); // allot space for 1 more char*
        table_names[(*table_count)-1] = (char*) calloc( strlen( (*token)), sizeof(char) );     // allot space of token
        strcpy(table_names[(*table_count)-1], (*token));                        // copy token to table_names
    }

    return 0;
}

/**
 * Parse the where clause from the select statement.
 * @param token ptr to strtok of select stmt
 * @param where_clause ptr to where_clause string
 * @param order_clause_present ptr to whether there 'order' keyword present (0 for no, 1 for yes)
 * @return 0 if parse was successful, else -1 for failure
 */
int parse_select_where(char** token, char** where_clause, int* order_clause_present) {
    char* temp_where;
    while ( ((*token) = strtok(NULL, " ")) ) {
        for (int i = 0; (*token)[i] != '\0'; i++) {
            if ( isalpha((*token)[i]) ) { // lower token
                (*token)[i] = tolower((*token)[i]);
            }
        }
        //printf("Where Token: '%s'\n", (*token));
        if (strcmp((*token), "order") == 0) { // check if 'order' keyword
            *order_clause_present = 1;
            return 0;
        }

        // append token to where_clause
        temp_where = (char*) calloc(strlen((*where_clause)) + strlen((*token)) + 1 + 1, sizeof(char));
        strcat(temp_where, (*where_clause));    // copy current where clause
        strcat(temp_where, " ");                // add a space
        strcat(temp_where, (*token));           // add new token
        *where_clause = temp_where;             // set where clause to temp
        //printf("WHERE CLAUSE: '%s'\n", *where_clause);
    }
    return 0;
}

int parse_select_order_by() {
    return 0;
}