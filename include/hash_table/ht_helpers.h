//
// Created by SGuan on 3/14/2021.
//

#ifndef HT_HELPERS_H
#define HT_HELPERS_H

#include "ht_structs.h"



struct ht_node* ht_add(struct ht_node*** table,int* result, int index, char* hash_code_str);
union ht_node_value* ht_get(struct ht_node*** table, int* result, int index, char* hash_code_str);
struct ht_node* ht_remove(struct ht_node*** table, int* result, int index, char* hash_code_str);
int ht_contains(struct ht_node*** table, int index, char* hash_code_str);
int ht_resize(struct ht_node*** table, int* capacity, int size, int multiplier, double load_factor);

#endif
