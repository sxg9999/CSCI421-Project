
int parse_update_stmt(char* update_stmt);


int parse_update_stmt(char* update_stmt) {
    // get the update keyword
    
    // get the table name
    
    // get the set keyword
    
    // parse the sets to get the list of changes
    
    // get the where keyword (if there)
    
    // parse the where clause and make the tree
    
    // get all the records for the table
    
    // for each record send it to the where tree
        // if true mark it for update
        // if false ignore it

    // make the updates

    // send the updates to the storagemanger update

    return 0;
}
