//
// Created by SGuan on 3/11/2021.
//

#ifndef STATEMENT_TYPE_H
#define STATEMENT_TYPE_H

enum stmt_type {NO_STMT_TYPE = 0, CREATE = 1, DROP = 2, ALTER = 3,
                INSERT = 4, UPDATE = 5, DELETE = 6, SELECT = 10};

/**
 * Initiates the statement_types
 * @return 0 if success and -1 if failed
 */
int init_statement_types();

/**
 * Gets the statement type
 * @param key_word
 * @return the enum value of the key_word
 */
enum stmt_type typeof_s(char* statement);

/**
 * check if the statement is a ddl statement
 * @param key_word
 * @return 1 if true, 0 if false
 */
int is_ddl(char* statement);

/**
 * check if the statement is a dml statement
 * @param key_word
 * @return 1 if true, 0 if false
 */

int is_dml(char* statement);
/**
 * checks if the statement is a query statement
 * @param key_word
 * @return 1 if true, 0 if false
 */
int is_query(char* statement);

/**
 * checks if the statement is a non query statement
 * @param key_word
 * @return 1 if true, 0 if false
 */
int is_non_query(char* statement);

void statement_print_table_content();

void statement_type_close();
#endif
