
#ifndef PARSE_DELETE_STMT_H
#define PARSE_DELETE_STMT_H



/**
 * Parses, validates, and executes a potential delete stmt.
 * 
 * Format:
 * 'delete from' - All DML statements that start with this will be considered to be trying
 * to delete data from a table. 
 * '<name>' - is the name of the table to delete from. All table names are unique.
 * 'where <condition>' - A condition where a tuple should deleted. If this evaluates to
 * true the tuple is remove; otherwise it remains. See below for evaluating conditionals. If
 * there is no where clause it is considered to be a where true and all tuples get deleted.
 * 
 * @param delete_stmt - the potential statement to be parsed and run 
 * @return 0 if delete statement was successful, else -1 for failure
 */
int parse_delete_stmt(char* delete_stmt);


#endif
