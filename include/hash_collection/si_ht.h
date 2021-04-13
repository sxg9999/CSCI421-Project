// Hash table functions involving a string key and int value
// Created by SGuan on 3/18/2021.
//

#ifndef SI_HT_H
#define SI_HT_H
#include "ht_structs.h"

/**
 * Add the <String, Int> pair to the hashtable
 * @param ht
 * @param key
 * @param value
 * @return
 */
int si_ht_add(struct hashtable* ht, char* key, int value);

/**
 * Get the value of the key
 * @param ht
 * @param key
 * @return  for no such key
 */
int si_ht_get(struct hashtable* ht, char* key);

/**
 * remove the key and its value from the hash table
 * @param ht
 * @param key
 * @return the value of the key
 */
int si_ht_remove(struct  hashtable* ht, char* key);

/**
 * check if the key exist in the table
 * @param ht
 * @param key
 * @return 1 if exist, 0 if does not exist, -1 if an error occurred
 */
int si_ht_contains(struct hashtable* ht, char* key);


void si_ht_print(struct hashtable* ht);

/**
 * Free the hashtable
 * @param ht
 */
void destroy_si_ht(struct hashtable* ht);


#endif
