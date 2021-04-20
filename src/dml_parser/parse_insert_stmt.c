

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/parse_insert_stmt.h"


int parse_insert_stmt(char* insert_stmt) {
    //parse the insert key word
    char* stmt_cpy = strdup(insert_stmt);



    // parse the into keyword
    
    // parse the table name

    // verify the table exists
    
    // get the storage manager table number

    // parse values keyword
    
    // parse the tuples one at a time

    // determine the type of each attribute and convert
        //verify the types match the table types
        // verify you have enough attributes
        // create a union * record

    //insert the tuple using the storagemanager insert

    return 0;
}