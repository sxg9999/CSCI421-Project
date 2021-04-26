
#ifndef PARSE_DELETE_STMT_H
#define PARSE_DELETE_STMT_H



/**
 * Parses, validates, and executes a potential delete stmt.
 * 
 * 
 * Format:
 * 'delete from' - All DML statements that start with this will be considered to be trying
 * to delete data from a table. 
 * '<name>' - is the name of the table to delete from. All table names are unique.
 * 'where <condition>' - A condition where a tuple should deleted. If this evaluates to
 * true the tuple is remove; otherwise it remains. See below for evaluating conditionals. If
 * there is no where clause it is considered to be a where true and all tuples get deleted.
 * 
 * Format:
 * 'delete from foo where bar = 10;'
 * 'delete from foo where bar > 10 and foo = "baz";'
 * 'delete from foo where bar != bazzle;'
 * 
 * 
 * 
 * @param delete_stmt - the potential statement to be parsed and run 
 * @return 0 if delete statement was successful, else -1 for failure
 */
int parse_delete_stmt(char* delete_stmt);




/**
 * Paers, validates, and executes a potential update statement.
 * 
 * 
 * Format:
 * 'update' - All DML statements that start with this will be considered 
 * to be trying to update data in a table.
 * '<name>' - is the name of the table to update in. All table names are unique.
 * 'set <column 1> = <value>, ..., <column N> = <value>' - Sets the columns 
 * to the provided values.
 * '<value>' - attribute name, constant value, or a mathematical operation.
 * 'where <condition>' - A condition where a tuple should updated. 
 * If this evaluates to true the tuple is updated; otherwise it remains the same. 
 * See below for evaluating conditionals. 
 * If there is no where clause it is considered to be a where true and all tuples get updated.
 * 
 * Examples: 
 * 'update foo set bar = 5 where baz < 3.2;'
 * 'update foo set bar = bar + 1, baz = 1.1 where a = "foo";'
 * 'update foo set bar = bar + 1, baz = 1.1 where a = "foo" and bar > 2;'
 * 
 * 
 */
int parse_update_stmt(char* input_stmt);




void lowercase(char* str);

#endif
