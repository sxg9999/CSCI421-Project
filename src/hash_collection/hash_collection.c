//
// Created by SGuan on 3/17/2021.
//

#include "../../include/hash_collection/hash_collection.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct hashtable* ht_create(int capacity, double load_factor){
    struct hashtable* ht = malloc(sizeof(struct hashtable));
    ht->size = 0;
    ht->capacity = capacity;
    ht->base_multiplier = capacity;
    ht->load_factor = load_factor;

    struct ht_node** table = malloc(sizeof(struct ht_node*) * capacity);
    for(int i = 0; i < capacity; i++){
        table[i] = NULL;
    }
    ht->table = table;

    int node_list_len = (int)ceil(capacity*load_factor) + 1;
    struct ht_node** node_list = malloc(sizeof(struct ht_node*) * node_list_len);
    for(int i = 0; i < node_list_len; i++){
        node_list[i] = NULL;
    }

    ht->node_list = node_list;

    return ht;


}

