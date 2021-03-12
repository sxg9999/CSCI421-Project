/*
 * Author: Dylan Cuprewich (dxc2961)
 * Header file for CSCI421 Group Project DDL Parser Helpers
 * Outlines the public functionality for the DML Parser
 */

#ifndef KEYWORDS_HEADER
#define KEYWORDS_HEADER

// COMMAND STARTS
#define CREATE_START "create"
#define DROP_START "drop"
#define ALTER_START "alter"

// CREATE TABLE CONSTRAINTS
#define PRIMARY_KEY_CON "primarykey"
#define UNIQUE_CON "unique"
#define FOREIGN_CON "foreginkey"
#define REFERENCES_CON "references"
#define NOTNULL_CON "notnull"

// ALTER TABLE WORDS
#define ALTER_DROP "drop"
#define ALTER_ADD "add"
#define ALTER_DEFAULT "default"

//
#define TABLE "table"

// TYPES
#define INTEGER_TYPE "integer"
#define DOUBLE_TYPE "double"
#define BOOLEAN_TYPE "boolean"
#define CHAR_TYPE "char"
#define VARCHAR_TYPE "varchar"

//  
#define STMT_END_STR ";"
#define STMT_END_CHAR ';'

/*
 * Checks if a provided word is a keyword.
 * @param word - The string check is a keyword
 * @return 0 if `word` is not a keyword, 1 if `word` is a keyword 
 */   
int is_keyword(char* word);

#endif