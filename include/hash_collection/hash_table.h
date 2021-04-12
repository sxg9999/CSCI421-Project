//
// Created by SGuan on 3/17/2021.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "ht_structs.h"

int ht_add(struct hashtable* ht, char* key, union ht_node_value* value);

union ht_node_value* ht_get(struct hashtable* ht, char* key);

union ht_node_value* ht_remove(struct hashtable* ht, char* key);

int ht_resize(struct hashtable* ht);

/**
 * checks if the key exist in the hashtable
 * @param ht
 * @param key
 * @return 1 for exist, 0 for does not exist
 */
int ht_contains(struct hashtable* ht, char* key);

void ht_destroy(struct hashtable* ht);

void ht_print(struct hashtable* ht);


#endif //
