
int parse_select_stmt(char* select_stmt);


int parse_select_stmt(char* select_stmt) {
    // parse the select keyword
    
    // parse the select attributes (or the *)
    
    // parse the from key word
    
    // parse the table name (verify it exists)
    
    // get the tuples for that table
    
    // get the where keyword (if there)
    
    // parse the where clause and make the tree
    
    // for each record for the table
        // if true place in output
        // if false ignore it

    // in the output remove the attributes not in the select

    return 0;
}