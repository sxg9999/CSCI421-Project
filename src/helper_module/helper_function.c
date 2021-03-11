//
// Created by SGuan on 3/10/2021.
//

#include "helper_module/helper_function.h"
#include <stdio.h>
#include <string.h>


void clear_buffer(char* buffer, int length){
    for(int i = 0; i < length; i++){
        buffer[i] = 0;
    }
}

void str_lower(char* buffer, char* str, int length){
    for(int i = 0; i < length; i++){
        buffer[i] = tolower(str[i]);
    }
}




