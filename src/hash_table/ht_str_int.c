//
// Created by SGuan on 3/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/helper_module/hash_function.h"
#include "../../include/hash_table/ht_helpers.h"
#include "../../include/hash_table/ht_str_int.h"


int ht_str_int_add(struct ht_container* content, char* key, int val);
int ht_str_int_get(struct ht_container* content, char* key);
int ht_str_int_remove(struct ht_container* content, char* key);
int ht_str_int_contains(struct ht_container* content, char* key);
int ht_str_int_recur_free(struct ht_node* n);
int ht_str_int_free(struct ht_container* content);
void ht_str_int_print_content(struct ht_container* content);
void ht_str_int_test();

static struct ht_str_int ht ={
        .add = ht_str_int_add,
        .get = ht_str_int_get,
        .remove = ht_str_int_remove,
        .contains = ht_str_int_contains,
        .close = ht_str_int_free,
        .print = ht_str_int_print_content,
        .print_test = ht_str_int_test,
};

void ht_str_int_test(){
    printf("here in ht_str_int\n");
}

struct ht_str_int* get_ht_str_int(){

    return &ht;
}

/**
 *
 * @param ht
 * @param key
 * @param val
 * @return 1 if successful, 0 if already exist and updated, -1 if failed
 */
int ht_str_int_add(struct ht_container* content, char* key, int val){
    ht_resize(&(content->table), &(content->capacity), content->size, content->base_multiplier, content->load_factor);

    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);

    int result;
    struct ht_node* node = ht_add(&(content->table), &result, index, hash_code_str);
    if(result==1){
        //add value to the node
        node->key = (char*)malloc(strlen(key)+1);
        strncpy(node->key, key, strlen(key)+1);
        node->hash_code_str = hash_code_str;
        node->value->i = val;
        content->size++;
        return 1;
    }else if(result==0){
        node->value->i = val;
        return 0;
    }

    return -1;
}

int ht_str_int_get(struct ht_container* content, char* key){
    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);
    int result;

    union ht_node_value* value = ht_get(&(content->table), &result, index, hash_code_str);

    if(result==1){
        return value->i;
    }
    return 0;
}

/**
 * removes a node in the table
 * @param ht
 * @param key
 * @return returns 1 if success and 0 if failed;
 */
int ht_str_int_remove(struct ht_container* content, char* key){
    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);
    int result;

    struct ht_node* node = ht_remove(&(content->table), &result, index, hash_code_str);

    //clean up
    free(hash_code_str);
    if(result){

        free(node->key);
        free(node->hash_code_str);
        free(node->value);
        free(node);
        content->size--;
    }

    return result;
}

/**
 * check if the key exist in the hash table
 * @param ht
 * @param key
 * @return returns 1 if exist and 0 if it does not
 */
int ht_str_int_contains(struct ht_container* content, char* key){
    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);
    int result = ht_contains(&(content->table),index, hash_code_str);
    free(hash_code_str);
    return result;
}


void ht_str_int_print_content(struct ht_container* content){
    int capacity = content->capacity;
    struct ht_node** table = content->table;
    for(int i = 0; i < capacity; i++){
        if(table[i]!=NULL){
            struct ht_node* curr = table[i];
            while(curr != NULL){
                printf("index_%d key : %s, val : %d\n", i, curr->key, curr->value->i);
                curr = curr->next_node;
            }

        }
    }
}



int ht_str_int_recur_free(struct ht_node* n){
    if(n != NULL){
        ht_str_int_recur_free(n->next_node);
        free(n->key);
        free(n->hash_code_str);
        free(n->value);
        free(n);
        n = NULL;
    }
}

int ht_str_int_free(struct ht_container* content){
    if(content){
        for(int i = 0; i < content->capacity; i++){
            struct ht_node* curr = content->table[i];
            if(curr!=NULL){
                ht_str_int_recur_free(curr);
            }
        }
        free(content->table);
        free(content);
    }
}


