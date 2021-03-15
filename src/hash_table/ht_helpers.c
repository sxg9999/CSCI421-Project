//
// Created by SGuan on 3/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/hash_table/ht_helpers.h"


struct ht_node* ht_add(struct ht_node*** table, int* result, int index, char* hash_code_str){
    *result = -1;     //1 is success, 0 already exist, -1 failed badly
    struct ht_node* curr = (*table)[index];
    if(curr==NULL){
        (*table)[index] = (struct ht_node*)malloc(sizeof(struct ht_node));
        (*table)[index]->value = (union ht_node_value*)malloc(sizeof(union ht_node_value));
        (*table)[index]->next_node = NULL;
        *result = 1;
        return (*table)[index];

    }else{
        //check if a node with the hash_code already exist
        while(curr!=NULL){
            if(strncmp(curr->hash_code_str, hash_code_str, strlen(hash_code_str))==0){
                *result = 0;
                return curr;
            }
            curr = curr->next_node;
        }
        struct ht_node* new_node = (struct ht_node*)malloc(sizeof(struct ht_node));
        new_node->value = (union ht_node_value*)malloc(sizeof(union ht_node_value));
        new_node->next_node = (*table)[index];
        (*table)[index] = new_node;
        *result = 1;
        return (*table)[index];

    }
    return NULL;
}

union ht_node_value* ht_get(struct ht_node*** table, int* result, int index, char* hash_code_str){
    struct ht_node* curr = (*table)[index];
    *result = 0;     //1 is success, 0 is fail
    while(curr!=NULL){
        if(strncmp(curr->hash_code_str, hash_code_str, strlen(hash_code_str)) == 0){
            *result = 1;
            return curr->value;
        }
        curr = curr->next_node;
    }
    return NULL;
}
struct ht_node* ht_remove(struct ht_node*** table, int* result, int index, char* hash_code_str){
    *result = 0;        //1 is success, 0 is fail
    struct ht_node* curr = (*table)[index];
    struct ht_node* prev = NULL;
    while(curr!=NULL){
        if(strncmp(curr->hash_code_str, hash_code_str, strlen(hash_code_str))==0){
            if(prev==NULL){
                (*table)[index] = curr->next_node;
            }else{
                prev->next_node = curr->next_node;
            }
            *result = 1;
            return curr;
        }
        prev = curr;
        curr = curr->next_node;
    }
    return NULL;
}

int ht_contains(struct ht_node*** table, int index, char* hash_code_str){
    int result = 0;     //1 is success, 0 is fail
    struct ht_node* curr = (*table)[index];
    while(curr!=NULL){
        if(strncmp(curr->hash_code_str, hash_code_str, strlen(hash_code_str))==0){
            result = 1;
            break;
        }
        curr = curr->next_node;
    }

    return result;
}

int ht_resize(struct ht_node*** table, int* capacity, int size, int multiplier, double load_factor){
    int threshold = ceil(*capacity * load_factor);
    printf("threshold is %d\n", threshold);

    if(size + 1 >= threshold){
//        printf("true\n");
//        printf("size is : %d\n", *size);
        int new_capacity = (*capacity) * multiplier;
        struct ht_node** new_table = (struct ht_node**)malloc(sizeof(struct ht_node*)*new_capacity);
        for(int i = 0; i < new_capacity; i++){
            new_table[i] = NULL;
        }
        //remap all the nodes
        for(int i = 0; i < *capacity; i++){
            if((*table)[i]!=NULL){
                struct ht_node* curr = (*table)[i];
                struct ht_node* tmp_node = NULL;

                while(curr!=NULL){
                    tmp_node = curr;
                    curr = curr->next_node;

                    int new_index = hash_compute_index(tmp_node->hash_code_str, new_capacity);

                    if(new_table[new_index] == NULL){
                        tmp_node->next_node = NULL;
                        new_table[new_index] = tmp_node;
                    }else{
                        tmp_node->next_node = new_table[new_index];
                        new_table[new_index] = tmp_node;
                    }
                }
            }
        }
        printf("done resizing\n");
        free(*table);
        *table = new_table;
        *capacity = new_capacity;

    }

    return 0;
}