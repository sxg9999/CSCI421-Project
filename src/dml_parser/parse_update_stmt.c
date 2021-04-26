#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../include/parse_dml_stmt.h"
#include "../../include/db_types.h"
#include "../../include/catalog/catalog.h"


int parse_update_stmt(char* input_stmt) {
    printf("Attempting to parse '%s' as an update statement...\n", input_stmt);
    // copy the string locally for strtok
    char* update_stmt = (char *)malloc( strlen(input_stmt) );
    strcpy(update_stmt, input_stmt);
    printf("TEST: '%s'\n", update_stmt);

    // get the update keyword
    char* token = strtok(update_stmt, " ");
    lowercase(token);
    //printf("First token: '%s'\n", token);
    enum db_type first_word_type = typeof_kw(token);
    if (first_word_type != UPDATE) {
        fprintf(stderr, "Missing 'update' keyword: '%s' in '%s'\n", token, input_stmt);
        return -1;
    }
    printf("'update' keyword detected...\n");
    
    // get the table name
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

    // get the set keyword
    token = strtok(NULL, " ");
    lowercase(token);
    //printf("Second token: '%s'\n", token);
    if (strcmp(token, "set") != 0) {
        fprintf(stderr, "Missing 'set' keyword: '%s'\n", input_stmt);
        return -1;
    }
    printf("'set' keyword detected...\n");
    
    // parse the sets to get the list of changes
    
    // get the where keyword (if there)
    bool where_clause;
    token = strtok(NULL, " ");
    if (token == NULL) {
        printf("No where clause\n");
        where_clause = false;
    }
    else {
        printf("Where clause present\n");
        where_clause = true;
    }


    // parse the where clause and make the tree
    
    // get all the records for the table
    
    // for each record send it to the where tree
        // if true mark it for update
        // if false ignore it

    // make the updates

    // send the updates to the storagemanger update

    return 0;
}
    

