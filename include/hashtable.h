
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

typedef struct
{
	int capacity; //2^(times increased+1)
	int current_size;
	int load_factor;
}HashTable;

typedef struct
{
	int key;
	int value;
	struct Node *next_node;
}Node;


HashTable* HashTable_create_param(int capacity, int load_factor);

HashTable* HashTable_create();

int put(union record_item** key, int value);

int compute_hash_code(HashTable* table, int converted_str);

int compute_index(int hash_code);

int resize(HashTable* table);


void HashTable_destroy();


#endif