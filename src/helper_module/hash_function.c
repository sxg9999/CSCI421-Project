#include "helper_module/hash_function.h"
#include <stdio.h>
#include <string.h>

const long long int FNV_OFFSET_BASIS = 14695981039346656037;
const long long int FNV_PRIME_VALUE  = 1099511628211;

long long unsigned FNV_1A_Hash_Algorithm(char* string_to_be_hashed){

    long long unsigned hash_code = FNV_OFFSET_BASIS;
    int len_of_str = strlen(string_to_be_hashed);
    
    for(int i = 0; i < len_of_str; i++){
        hash_code = hash_code ^ (int)string_to_be_hashed[i];
        hash_code = hash_code * FNV_PRIME_VALUE;
    }

    printf("The hash code is : %llu\n", hash_code);
    return hash_code;
}


int hash_compute_index(char* hash_code_str, int mod_num){
    
    int len_of_str = strlen(hash_code_str);
    int mod_result = 0;
    for(int i = 0; i < len_of_str; i++){
        mod_result = (mod_result*10 + (int)hash_code_str[i] - '0') % mod_num;
    }
    return mod_result;
}

char* hash_str(char* str_val){
    long long unsigned hash_code = FNV_1A_Hash_Algorithm(str_val);
    char hash_code_str[22];
    sprintf(hash_code_str, "%llu", hash_code);

    return hash_code_str;
}

char* hash_int(int int_val){
    char* str_val = (char*)malloc(sizeof(char)*int_val+2);
    spritnf(str_val, "%d", int_val);
    
    return hash_str(str_val);
}