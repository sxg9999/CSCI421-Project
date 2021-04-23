//  Hash table functions for accepting strings as keys and returns void
// Created by SGuan on 3/17/2021.
//

#ifndef SV_HT_H
#define SV_HT_H
#include "ht_structs.h"

/**
 * Add the <String,void*> pair to the hashtable ht
 * @param ht : the hashtable struct
 * @param key
 * @param value
 * @return : 0 for success and -1 for failed
 */
int sv_ht_add(struct hashtable* ht, char* key, void* value);


/**
 * Get the value of the key from the hashtable
 * @param ht
 * @param key
 * @return : the value of the key (warning: it is a void ptr so you have to cast to use it)
 */
void* sv_ht_get(struct hashtable* ht, char* key);


/**
 * remove the key and its value from the hash table
 * @param ht
 * @param key
 * @return the value of the key (warning: it is a void ptr so you have to cast to use it)
 */
void* sv_ht_remove(struct  hashtable* ht, char* key);

/**
 * check if the key exist in the table
 * @param ht
 * @param key
 * @return 1 if exist, 0 if does not exist, -1 if an error occurred
 */
int sv_ht_contains(struct hashtable* ht, char* key);



/**
 * Just prints the hash table's content (testing purposes only)
 * @param ht
 */
void sv_ht_print(struct hashtable* ht);


/**
 * Free the hashtable from memory
 *
 * @param ht
 */
void destroy_sv_ht(struct hashtable* ht);


/**
 * gets all the values of the hashtable and store it in
 * value_ptrs and return the number of values
 *
 * value_arr : a pointer that points to an array of values (void ptrs in this case)
 * @param ht
 * @return
 */
int sv_ht_values(struct hashtable* ht, void*** value_arr);

#endif
