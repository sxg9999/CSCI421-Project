/*
 * Author: Dylan Cuprewich (dxc2961)
 * Header file for CSCI421 Group Project DDL Parser Helpers
 * Outlines the public functionality for the DML Parser
 */

#ifndef DDL_PARSER_HELPER_H
#define DDL_PARSER_HELPER_H

#define CREATE_START "create table"
#define DROP_START "drop table"
#define ALTER_START "alter table"

/*
  * This function handles the parsing of create table statments
  * that return nothing
  *
  * @param statement - the create table statement to execute
  * @return 0 on sucess; -1 on failure
  */
int parse_create_table_stmt( char* statement );

/*
  * This function handles the parsing of drop table statments
  * that return nothing
  *
  * @param statement - the drop table statement to execute
  * @return 0 on sucess; -1 on failure
  */
int parse_drop_table_stmt( char* statement );

/*
  * This function handles the parsing of alter table statments
  * that return nothing
  *
  * @param statement - the alter table statement to execute
  * @return 0 on sucess; -1 on failure
  */
int parse_alter_table_stmt( char* statement );

#endif
