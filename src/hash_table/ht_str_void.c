//
// Created by SGuan on 3/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/helper_module/hash_function.h"
#include "../../include/hash_table/ht_helpers.h"
#include "../../include/hash_table/ht_str_void.h"

int ht_str_void_add(struct ht_container* content, char* key, void* val);
void* ht_str_void_get(struct ht_container* content, char* key);
int ht_str_void_remove(struct ht_container* content, char* key);
int ht_str_void_contains(struct ht_container* content, char* key);
int ht_str_void_free(struct ht_container* content);
void ht_str_void_print_content(struct ht_container* content);

static struct ht_str_void ht = {
    .add = ht_str_void_add,
    .get = ht_str_void_get,
    .remove = ht_str_void_remove,
    .contains = ht_str_void_remove,
    .close = ht_str_void_free,
    .print = ht_str_void_print_content
};

struct ht_str_void* get_ht_str_void(){
    return &ht;
}

int ht_str_void_add(struct ht_container* content, char* key, void* val){
    ht_resize(&(content->table), &(content->capacity), content->size, content->base_multiplier, content->load_factor);
    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);

    int result;
    struct ht_node* node = ht_add(&(content->table), &result, index, hash_code_str);
    if(result==1){
        node->key = (char*)malloc(strlen(key)+1);
        strncpy(node->key, key, strlen(key)+1);
        node->hash_code_str = hash_code_str;
        node->value->v_ptr = val;
        content->size++;
        return 1;
    }else if(result==0){
        node->value->v_ptr = val;
        return 0;
    }
    return -1;
}
void* ht_str_void_get(struct ht_container* content, char* key){
    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);
    int result;
    union ht_node_value* value = ht_get(&(content->table), &result, index, hash_code_str);

    if(result==1){
        return value->v_ptr;
    }
    return NULL;
}
int ht_str_void_remove(struct ht_container* content, char* key){
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



int ht_str_void_contains(struct ht_container* content, char* key){
    char* hash_code_str = hash_str(key);
    int index = hash_compute_index(hash_code_str, content->capacity);
    int result = ht_contains(&(content->table),index, hash_code_str);
    free(hash_code_str);
    return result;
}

void ht_str_void_print_content(struct ht_container* content){
    int capacity = content->capacity;
    struct ht_node** table = content->table;

    if(content->size == 0){
        printf("Table<STRING, VOID*> is Empty\n");
    }else{
        for(int i = 0; i < capacity; i++){
            if(table[i]!=NULL){
                struct ht_node* curr = table[i];
                while(curr!=NULL){
                    printf("index_%d key : %s, val : %s\n", i, curr->key, "void*");
                    curr = curr->next_node;
                }
            }
        }
    }



}

int ht_str_void_recur_free(struct ht_node* n){
    if(n!=NULL){
        ht_str_void_recur_free(n->next_node);
        free(n->key);
        free(n->hash_code_str);
        free(n->value);
        free(n);
        n = NULL;
    }
}
int ht_str_void_free(struct ht_container* content){
    if(content){
        for(int i = 0; i < content->capacity; i++){
            struct ht_node* curr = content->table[i];
            if(curr!=NULL){
                ht_str_void_recur_free(curr);
            }
        }
        free(content->table);
        free(content);
    }
}
