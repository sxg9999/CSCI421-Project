#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../include/parse_delete_stmt.h"
int parse_delete_stmt(char* delete_stmt);

int parse_delete_stmt(char* input_stmt) {
    printf("Attempting to parse '%s' as a delete statement...\n", input_stmt);
    // copy the string locally for strtok
    char* delete_stmt = (char *)malloc( strlen(input_stmt) );
    strcpy(delete_stmt, input_stmt);
    printf("TEST: '%s'\n", delete_stmt);
    
    // get delete keyword

    
    // get from keyword
    // get table name
        // verify table name
        // get storagemanager table num

    // get where keyword (if there)
    
    // parse where clause and make tree
    
    // get all records for the table
    
    // for each record send it into the where tree for
        // mark it for deletion if true
        // if false ignore it

    // ask the storage manager to delete the records marked for deletion
    
    return 0;
}

