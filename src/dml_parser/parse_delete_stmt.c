#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../include/parse_dml_stmt.h"
#include "../../include/db_types.h"
#include "../../include/catalog/catalog.h"


int parse_delete_stmt(char* input_stmt) {
    printf("Attempting to parse '%s' as a delete statement...\n", input_stmt);
    // copy the string locally for strtok
    char* delete_stmt = (char *)malloc( strlen(input_stmt) );
    strcpy(delete_stmt, input_stmt);
    printf("TEST: '%s'\n", delete_stmt);
    
    // get delete keyword
    char* token = strtok(delete_stmt, " ");
    lowercase(token);
    //printf("First token: '%s'\n", token);
    enum db_type first_word_type = typeof_kw(token);
    if (first_word_type != DELETE) {
        fprintf(stderr, "Missing 'delete' keyword: '%s' in '%s'\n", token, input_stmt);
        return -1;
    }
    printf("'delete' keyword detected...\n");
    
    // get from keyword
    token = strtok(NULL, " ");
    lowercase(token);
    //printf("Second token: '%s'\n", token);
    if (strcmp(token, "from") != 0) {
        fprintf(stderr, "Missing 'from' keyword: '%s'\n", input_stmt);
        return -1;
    }
    printf("'from' keyword detected...\n");
    
    
    // get table name
    token = strtok(NULL, " ");
    //printf("Third token: '%s'\n", token);
    // verify table name
    int table_exists = catalog_contains(token);
    if (catalog_contains(token) != 1) {
        fprintf(stderr, "Catalog does not contain table '%s'!\n", token);
        return -1;
    }
    // get storagemanager table num
    int table_num = catalog_get_table_num(token);
    printf("Table '%s': table id '%d'\n", token, table_num);


    // get where keyword (if there)
    
    // parse where clause and make tree
    
    // get all records for the table
    
    // for each record send it into the where tree for
        // mark it for deletion if true
        // if false ignore it

    // ask the storage manager to delete the records marked for deletion
    
    return 0;
}

void lowercase(char* token) {
    for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
}
    
