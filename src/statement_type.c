//
// Created by SGuan on 3/11/2021.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "statement_type.h"
#include "helper_module/hash_function.h"
#include "helper_module/helper_function.h"

struct node;

struct node{
    char key[10];
    char hash_code_str[30];
    int value;
    struct node* next_node;
};

int add_type(struct node* type);

static struct node* table[25];
static struct node type_create = {.key="create", .value = 0};
static struct node type_drop = {.key="drop", .value = 1};
static struct node type_alter = {.key="alter", .value = 2};

int init_statement_types(){
    for(int i = 0; i < 25; i++){
        table[i] = NULL;
    }
    add_type(&type_create);
    add_type(&type_drop);
    add_type(&type_alter);

    return 0;
}

int add_type(struct node* type){
    char* hash_code_str = hash_str(type->key);
    strncpy(type->hash_code_str, hash_code_str, strlen(hash_code_str)+1);   //add a 1 to include the null terminator
    free(hash_code_str);

    int index = hash_compute_index(type->hash_code_str, 25);
    struct node* curr = table[index];
    if(curr==NULL){
        type->next_node = NULL;
        table[index] = type;

    }else{
        type->next_node = table[index];
        table[index] = type;
    }


    return 0; 
}


int typeof_s(char* key_word){
    char buffer[15];
    str_lower(buffer, key_word, strlen(key_word));

//    printf("buffer is %s\n", buffer);

    char* hash_code_str = hash_str(buffer);
    int index = hash_compute_index(hash_code_str, 25);

    int value = -1;
    struct node* curr = table[index];
    while(curr!=NULL){
        if(strncmp(curr->key, buffer, strlen(buffer))==0){
//            printf("current key is %s\n", curr->key);
            value = curr->value;
            break;
        }
        curr = curr->next_node;
    }
    free(hash_code_str);
    return value;


}

int is_query(char* key_word){
    char buffer[15];                //stores the key_word that is lowercased
    str_lower(buffer, key_word, strlen(key_word));

    char* hash_code_str = hash_str(buffer);
    int index = hash_compute_index(hash_code_str, 25);

    int value = 0;
    struct node* curr = table[index];
    while(curr!=NULL){
        if(strncmp(curr->key, buffer, strlen(buffer))==0){
            if(value >= 10){
                value = 1;
            }
            break;
        }
        curr = curr->next_node;
    }
    free(hash_code_str);
    return value;

}

int is_non_query(char* key_word){
    char buffer[15];                //stores the key_word that is lowercased
    str_lower(buffer, key_word, 10);

    char* hash_code_str = hash_str(buffer);
    int index = hash_compute_index(hash_code_str, 25);

    int value = 0;
    struct node* curr = table[index];
    while(curr!=NULL){
        if(strncmp(curr->key, buffer, strlen(buffer))==0){
            if(value < 10){
                value = 1;
            }
            break;
        }
        curr = curr->next_node;
    }
    free(hash_code_str);
    return value;

}

void statement_print_table_content(){

    for(int i = 0; i < 25; i++){
        if(table[i]!=NULL){
            struct node* curr = table[i];
            while(curr!=NULL){
                printf("\tindex : %d, Key : %s, Value: %d\n", i, curr->key, curr->value);
                curr = curr->next_node;
            }
        }
    }
}



