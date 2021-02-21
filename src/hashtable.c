#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hashtable.h"




void HashTable_record_int(HashTable* self, int capacity, int load_factor, int* attr_data_types, int len_of_data_types_arr){
	self->capacity = capacity;
	self->load_factor = load_factor;
	self->len_of_data_types_arr = len_of_data_types_arr;
	self->table = (Node*)malloc(sizeof(Node)*self->capacity);
	self->attr_data_types = (int*)malloc(sizeof(int)*len_of_data_types_arr);

	int i;

	for(i=0; i<len_of_data_types_arr; i++){
		self->attr_data_types[i] = attr_data_types[i];
	}
}


HashTable* HashTable_record_create_param(int capacity, int load_factor, int* attr_data_types, int len_of_data_types_arr){
	
	HashTable* record_hash_table = (HashTable*)malloc(sizeof(HashTable));
	HashTable_record_int(record_hash_table, capacity, load_factor, attr_data_types, len_of_data_types_arr);

	return record_hash_table;
}

HashTable* HashTable_record_create(int* attr_data_types, int len_of_data_types_arr){

	int capacity = 12;
	int load_factor = .75;
	
	HashTable* record_hash_table = (HashTable*)malloc(sizeof(HashTable));
	HashTable_record_int(record_hash_table, capacity, load_factor, attr_data_types, len_of_data_types_arr);

	return record_hash_table;
}



int get_record(HashTable* self, union record_item* key, int key_length){


}
int compute_dec_vals(char* str, int length){
	int result = 0;
	
	int i;
	for(i=0; i<length; i++){
		result += (int)str[i];
	}

	return result;
}

void Ht_clear_n_buffer(char* buffer, int end_of_buffer){
    int i;
    for(i=0; i<end_of_buffer; i++){
        buffer[i]=0;
    }
}

int compute_hash_code_record(HashTable* self, union record_item* key, int record_length){
	
	int* attr_data_types = self->attr_data_types;
	
	int hash_code = 0;

	char buffer[100];
	//parse the key

	int i;
	int j;
	for(i=0; i<record_length; i++){

		switch(attr_data_types[i]){
				case 0:
					//int
					sprintf(buffer, "%d", key[i].i);
					hash_code += compute_dec_vals(buffer, strlen(buffer));
					break;
				case 1:
					//double
					sprintf(buffer, "%d", (int)key[i].d);
					hash_code += compute_dec_vals(buffer, strlen(buffer));
					break;
				case 2:
					//bool
					if(key[i].b==true){
						hash_code += compute_dec_vals("true", 4);
					}else{
						hash_code += compute_dec_vals("false", 5);
					}
					break;
				case 3:
					hash_code += compute_dec_vals(key[i].c, strlen(key[i].c));
					//char
					break;
				case 4:
					//varchar
					hash_code += compute_dec_vals(key[i].c, strlen(key[i].c));
					break;
				default:
					break;
		}
		Ht_clear_n_buffer(buffer, strlen(buffer)+1);
	}

	return hash_code;
	
}



int compute_index(HashTable* self, int hash_code){
	return hash_code % hash_code;
}

int resize(HashTable* table);

void HashTable_destroy();



