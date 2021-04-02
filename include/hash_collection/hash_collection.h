#ifndef HASH_COLLECTION_H
#define HASH_COLLECTION_H

#include "ht_structs.h"

struct hashtable* ht_create(int capacity, double load_factor);

#endif