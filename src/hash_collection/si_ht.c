//
// Created by SGuan on 3/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/hash_collection/si_ht.h"
#include "../../include/hash_collection/hash_table.h"

int si_ht_add(struct hashtable* ht, char* key, int value){
    union ht_node_value* node_value = malloc(sizeof(union ht_node_value));
    node_value->i = value;
    int result = ht_add(ht, key, node_value);

    if(result == -1){
        //free the node if adding somehow fails
        free(node_value);
    }

    return result;
}

int si_ht_get(struct hashtable* ht, char* key){
    union ht_node_value* node_value = ht_get(ht, key);

    if(node_value == NULL){
        return 0;
    }
    return node_value->i;
}

int si_ht_remove(struct  hashtable* ht, char* key){
    union ht_node_value* node_value = ht_remove(ht, key);

    if(node_value == NULL){
        return 0;
    }

    int ret_val = node_value->i;
    free(node_value);
    return ret_val;
}

int si_ht_contains(struct hashtable* ht, char* key){
    return ht_contains(ht, key);
}

void si_ht_print(struct hashtable* ht){
    struct ht_node** node_list = ht->node_list;
    int size = ht->size;
    printf("\nHash Table Content:\n");
    for(int i = 0; i < size; i++){
        printf("index=%d, key=%s, hash_code=%llu\n", node_list[i]->table_index, node_list[i]->key, node_list[i]->hash_code);
    }
    printf("\n");
}

void destroy_si_ht(struct hashtable* ht){
    ht_destroy(ht);
}