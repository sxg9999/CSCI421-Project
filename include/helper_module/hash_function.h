#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H 


char* hash_str(char* str_val);
char* hash_int(int int_val);
int hash_compute_index(char* hash_code_str, int mod_num);

#endif