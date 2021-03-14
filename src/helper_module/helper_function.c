//
// Created by SGuan on 3/10/2021.
//

#include "helper_module/helper_function.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>



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

/**
 * Allocate enough memory for the substring and
 * copy from src to the substring desc and return
 * the substring
 */
char* substring_copy(char*src, int start_index, int end_index){
    int len = end_index-start_index+1;
    char* dest = malloc(len+1);
    dest[0] = 0;
    strncpy(dest, src+start_index, len);
    dest[len] = 0;
    return dest;
}


void remove_leading_spaces(char* src){

    int count = 0;

    int length = strlen(src);
    while(src[count]==' '){
        count++;
    }

    int curr_index;
    for(curr_index = 0; count < length; curr_index++){
        src[curr_index] = src[count];
        count++;
    }
    src[curr_index] = 0;
}

void remove_ending_spaces(char* src){
    int curr_index = strlen(src)-1;
    while(src[curr_index]==' '){
        src[curr_index] = 0;
        curr_index--;
    }
}
/**
 * returns a array of strings splitted based on the delim
 * @param str
 * @param delim
 * @return
 */
int split(char*** dest, char* str, char delim){


    int arr_size = 5;
    int count = 0;
    *dest = (char**)malloc(sizeof(char*)*arr_size);

    char* ptr = str;
    int start_index = 0;
    int end_index;

    while(1){
        if(count == arr_size){
            arr_size = arr_size*5;
            *dest = realloc(*dest,sizeof(char*)*arr_size);
        }
        ptr = strchr(ptr,delim);

        if(ptr == NULL){
            end_index = strlen(str)-1;
            (*dest)[count] = substring_copy(str, start_index, end_index);
            remove_leading_spaces((*dest)[count]);
            remove_ending_spaces((*dest)[count]);
            count++;
            break;
        }
        end_index = ptr - str - 1;
        (*dest)[count] = substring_copy(str, start_index, end_index);
        remove_leading_spaces((*dest)[count]);
        remove_ending_spaces((*dest)[count]);
        start_index = end_index + 2;
        ptr++;
        count++;
    }



    return count;
}

/**
 * Splits the string into 'n' parts based on the delim
 * @param dest
 * @param str
 * @param delim
 * @param n
 * @return returns the total number of parts
 */
int split_n(char*** dest, char* str, char delim, int n){

    int count = 0;
    *dest = (char**)malloc(sizeof(char*)*n);

    char* ptr = str;
    int start_index = 0;
    int end_index;

    while(count < n-1){
        ptr = strchr(ptr,delim);

        if(ptr == NULL){
            end_index = strlen(str)-1;
            (*dest)[count] = substring_copy(str, start_index, end_index);
            remove_leading_spaces((*dest)[count]);
            remove_ending_spaces((*dest)[count]);
            count++;
            (*dest) = (char**)realloc(*dest, sizeof(char*)*count);
            return count;
        }
        end_index = ptr - str - 1;
        (*dest)[count] = substring_copy(str, start_index, end_index);
        remove_leading_spaces((*dest)[count]);
        remove_ending_spaces((*dest)[count]);
        start_index = end_index + 2;
        ptr++;
        count++;
    }

    //put everything that is left into arr
    end_index = strlen(str)-1;
    (*dest)[count] = substring_copy(str, start_index, end_index);
    count++;
    return count;
}

void free_2d_char(char** char_2d, int length){
    for(int i = 0; i < length; i++){
        if(char_2d[i]!=NULL){
            free(char_2d[i]);
        }
    }
    free(char_2d);
}




