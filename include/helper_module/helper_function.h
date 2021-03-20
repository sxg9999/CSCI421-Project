//
// Created by SGuan on 3/10/2021.
//

#ifndef HELPER_FUNCTION_H
#define HELPER_FUNCTION_H

void clear_buffer(char* buffer, int length);
void str_lower(char* buffer, char* str, int length);
void remove_leading_spaces(char* src);
void remove_ending_spaces(char* src);
char* substring(char*src, int start_index, int end_index);
int split(char*** dest, char* str, char delim);
int split_n(char*** dest, char* str, char delim, int n);
void free_2d_char(char** char_2d, int length);

#endif
