//
// Created by SGuan on 3/13/2021.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../data_types.h"
#include "ht_str_int.h"
#include "ht_str_void.h"

enum ht_type {HT_STR_INT, HT_INT_INT};

void ht_init();
void* ht_create(struct ht_container** content, enum d_type key, enum d_type val, int capacity, double load_factor);
void ht_close();





#endif
