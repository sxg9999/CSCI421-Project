//
// Created by SGuan on 4/26/2021.
//

#ifndef DB_PROCESS_NON_SQL_STATEMENTS_H
#define DB_PROCESS_NON_SQL_STATEMENTS_H

/**
 * Process non sql statements:
 *  - add example table statements
 *  - get table record statements
 * @param stmt
 *
 * @return 1 for processed , 0 for not processed
 */
int process_non_sql_statements(char* stmt);


/**
 * add an example table to the database
 * @param table_name
 *
 * @return 1 for processed , 0 for not processed
 */
int add_example_table(char* table_name);

/**
 * Prints the records of the table
 * @param table_name : name of the table
 *
 * @return 1 for processed , 0 for not processed
 */
int print_table_records(char* table_name);

#endif
