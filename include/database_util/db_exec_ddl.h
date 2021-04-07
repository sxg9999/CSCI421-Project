//
// Created by SGuan on 4/4/2021.
//

#ifndef DB_EXEC_DDL_H
#define DB_EXEC_DDL_H

#include "../db_types.h"



int init_ddl_exec_ddl();

/**
 * Parse and split a ddl statement into useful parts (types, table_name, etc)
 * @param stmt : statement given by user
 * @return result of the parsing and split, 0 for success, -1 for failed
 */
int get_ddl_stmt_parts(char* stmt);

int execute_ddl_statement();

/**
 * Execute create table
 * @param table_name
 * @param data_str
 * @return
 */
int execute_create_table();

/**
 * Execute drop table
 * @param table_name
 * @return
 */
int execute_drop_table();


/**
 * Execute alter table
 * @param alter_type
 * @param data_str
 * @return
 */
int execute_alter_table(enum db_type alter_type, char* data_str);

void db_exec_ddl_close();

#endif
