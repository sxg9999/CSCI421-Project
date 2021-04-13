#ifndef HASH_COLLECTION_H
#define HASH_COLLECTION_H

#include "ht_structs.h"

/**
 * Initializes and returns a hashtable with the specified capacity
 * and load_factor.
 *
 * Important: you must give a capacity and load_factor. Use ht_create(12, 0.75) as
 * the default values.
 *
 * @param capacity : the capacity of the hashtable
 * @param load_factor : the load_factor of the hashtable
 * @return
 */
struct hashtable* ht_create(int capacity, double load_factor);

#endif