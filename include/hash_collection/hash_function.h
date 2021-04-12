//
// Created by SGuan on 3/17/2021.
//

#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H


unsigned long long hash_str(char* str_val);
unsigned long long hash_int(int int_val);
int hash_compute_index(unsigned long long hash_code, int mod_num);

#endif
