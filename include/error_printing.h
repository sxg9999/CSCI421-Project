/*
 * Author: Dylan Cuprewich (dxc2961)
 * Header file for CSCI421 Group Project DDL Parser Helpers
 * Outlines the public functionality Printing Errors.
 */

int null_check_str_create_stmt(char* words, char* stmt);

int null_check_str_word(char* words, char* stmt);

int int_equality_check(int a, int b, char* stmt);
