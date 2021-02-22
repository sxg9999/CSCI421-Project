
/***
 *  
 *  Function layout for Hash Tables
 *
 */


#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>

union record_item {
	int i;
	double d;
	bool b;
	char c[255];
	char v[255];
};

struct Node;

struct Node{
	int key;
	int value;
	struct Node* next_node;
};





typedef struct
{
	int capacity; //2^(times increased+1)
	double load_factor;
	int current_size;
	int base_multiplier;
    int len_of_data_types_arr;
    int* attr_data_types;
    struct Node** table;
    
}HashTable;




HashTable* HashTable_record_create_param(int capacity, double load_factor, int* attr_data_types, int len_of_data_types_arr);

HashTable* HashTable_record_create(int* attr_data_types, int len_of_data_types_arr);

int put(HashTable* self, int key, int value);

int put_record(HashTable* self, union record_item* key, int value, int record_length);

int put_int(HashTable* self, int key, int value);

int get_record(HashTable* self, union record_item* key, int key_length);

int compute_hash_code_record(HashTable* self, union record_item* key, int record_length);

int compute_index(HashTable* self, int hash_code);

int resize(HashTable* self);


void HashTable_destroy(HashTable* hashtable);


#endif