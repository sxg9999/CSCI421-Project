#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "ht_structs.h"

struct hashtable* ht_create(int capacity, double load_factor);

#endif