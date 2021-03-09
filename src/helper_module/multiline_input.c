
#include "helper_module/multiline_input.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char** buffer;
    char** ptr;
    char* result_str;
    int buffer_depth;
    int buffer_size;
    int base_multiplier;
    int count;
}Input;


static Input* input = NULL; 
int multiline_build_str(char** ptr, char* result_str, int line_count);
int multiline_input_resize();
int line_is_empty(char* ptr, int str_length);

int create_multiline_input(){
    input = (Input*)malloc(sizeof(Input));
    input->buffer_depth = 10;
    input->buffer_size = 1024;
    input->base_multiplier = 2;
    input->count = 0;
    input->buffer = (char**)malloc(sizeof(char*)*input->buffer_depth);
    
    for(int i = 0; i < 10; i++){
        input->buffer[i] = (char*)malloc(sizeof(char)*input->buffer_size);
        input->buffer[i][0] = 0;
    }
    input->ptr = input->buffer;
    input->result_str = (char*)malloc(sizeof(char)*input->buffer_depth*input->buffer_size);

    return 0;
}

/**
 * Loop until user terminates with semicolon
 */
char* get_input(){

    //reset buffer
    for(int i = 0; i < input->buffer_depth; i++){
        input->buffer[i][0] = 0;
    }
    input->result_str[0] = 0;
    input->ptr = input->buffer;
    int count = 0;

    int input_len = 0;
    while(true){
        printf(">");
        fgets(*(input->ptr), input->buffer_size, stdin);
        input_len = strlen(*(input->ptr));

        if(line_is_empty(*(input->ptr), input_len) == 0){
            //if the line is empty skip it
            continue;
        }else{
            count++;
            if(*(*(input->ptr)+(input_len-2))==59){
                break;
            }
            input->ptr++;
        }
 
    }

    //concatenate all the lines into one string and return it
    
    multiline_build_str(input->buffer, input->result_str, count);

    return input->result_str;
}

/**
 * checks if a line of string is empty (contains only spaces, tabs, and new line)
 * and returns 0 if empty and 1 if it is not empty;
 */
int line_is_empty(char* str, int str_length){
    char c;
    for(int i = 0; i < str_length; i++){
        c = str[i];
        if(c != ' ' && c != '\t' && c != '\n'){
            return 1;
        }
    }
    return 0;
}

int multiline_build_str(char** ptr, char* result_str, int line_count){
    int len = 0;
    int result_str_index = 0;
    for(int i = 0; i < line_count; i++){
        len = strlen(ptr[i]);
        for(int j = 0; j < len; j++){
            if(ptr[i][j]=='\t' || ptr[i][j]=='\n'){
                result_str[result_str_index] = ' ';
            }else{
                result_str[result_str_index] = ptr[i][j];
            }
            result_str_index++;
        }
    }

    //check if there is a space before the semicolon and 
    //if there is then move the semicolon to the left
    if(result_str[result_str_index-3]==' '){
        result_str[result_str_index-3] = ';';
        result_str[result_str_index-2] = 0;
    }else{
        result_str[result_str_index-1] = 0;
    }
    
}


int multiline_input_resize(){
    int new_depth = input->buffer_depth*input->base_multiplier;
    input->buffer = (char**)realloc(input->buffer, sizeof(char*)*new_depth);
    input->buffer_depth = new_depth;

    for(int i = 0; i < input->buffer_depth; i++){
        if(input->buffer[i]){
            input->buffer[i][0] = 0;
        }else{
            input->buffer[i] = (char*)malloc(sizeof(char)*input->buffer_size);
            input->buffer[i][0] = 0;
        }
    }

    input->result_str = (char*)realloc(input->result_str, sizeof(char)*input->buffer_depth*input->buffer_size);
    
    return 0;

}

int free_input(){
    if(input){
        for(int i = 0; i<input->buffer_depth; i++){
            free(input->buffer[i]);
        }
        free(input->buffer);
        free(input->result_str);
        free(input);
    }

    return 0;
}