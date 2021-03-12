//
// Created by SGuan on 3/11/2021.
//

#ifndef STATEMENT_TYPE_H
#define STATEMENT_TYPE_H

/**
 * Initiates the statement_types
 * @return 0 if success and -1 if failed
 */
int init_statement_types();

/**
 * Gets the int value of the key_word
 * @param key_word
 * @return the int value of the key_word
 */
int typeof_s(char* key_word);

/**
 * checks if the key_word is a query
 * @param key_word
 * @return 1 if true, 0 if false
 */
int is_query(char* key_word);

/**
 * checks if the key_word is not a query
 * @param key_word
 * @return 1 if true, 0 if false
 */
int is_non_query(char* key_word);

void statement_print_table_content();
#endif
