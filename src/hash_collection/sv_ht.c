//
// Created by SGuan on 3/17/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/hash_collection/hash_table.h"

int sv_ht_add(struct hashtable* ht, char* key, void* value){
    union ht_node_value* node_value = malloc(sizeof(union ht_node_value));
    node_value->v_ptr = value;
    int result = ht_add(ht, key, node_value);

    if(result == -1){
        //add somehow failed
        free(node_value);
    }

    return result;
}

void* sv_ht_get(struct hashtable* ht, char* key){
    union ht_node_value* value = ht_get(ht, key);


    if(value == NULL){
        return NULL;
    }

    return value->v_ptr;
}

void* sv_ht_remove(struct  hashtable* ht, char* key){
    union ht_node_value* value = ht_remove(ht, key);

    if(value == NULL){
        return NULL;
    }

    void* ret_val = value->v_ptr;
    printf("in remove\n");
    printf("the ret_val is : %s\n", ret_val);
    free(value);
    return ret_val;
}

int sv_ht_contains(struct hashtable* ht, char* key){
    return ht_contains(ht, key);
}

void sv_ht_print(struct hashtable* ht){
    struct ht_node** node_list = ht->node_list;
    int size = ht->size;
    printf("\nHash Table Content:\n");
    for(int i = 0; i < size; i++){
        printf("index=%d, key=%s, hash_code=%llu\n", node_list[i]->table_index, node_list[i]->key, node_list[i]->hash_code);
    }
    printf("\n");
}

void destroy_sv_ht(struct hashtable* ht){
    if(ht->size != 0 && ht->node_list[0]->value->v_ptr != NULL){
        fprintf(stderr, "(Error in sv_ht.c/destroy_sv_ht) HashTable<string, void*> cannot be freed before its values!\n");
        exit(0);
    }
    ht_destroy(ht);
}

int sv_ht_values(struct hashtable* ht, void*** value_ptrs){
    *value_ptrs = malloc(sizeof(void*)*ht->size);
    struct ht_node** node_list = ht->node_list;
    for(int i = 0; i < ht->size; i++){
        (*value_ptrs)[i] = node_list[i]->value->v_ptr;
    }
    return ht->size;
}

