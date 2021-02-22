#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hashtable.h"


int compute_dec_vals(char* str, int length);
void Ht_clear_n_buffer(char* buffer, int end_of_buffer);
int compute_hash_code_record(HashTable* self, union record_item* key, int record_length);
int compute_index(HashTable* self, int hash_code);
int compute_hash_code_int(HashTable* self, int key);

void HashTable_record_init(HashTable* self, int capacity, double load_factor, int* attr_data_types, int len_of_data_types_arr){
	self->capacity = capacity;
	self->load_factor = load_factor;
	self->current_size = 0;
	self->base_multiplier = capacity;
	self->len_of_data_types_arr = len_of_data_types_arr;

	//allocate memory for hashtable
	self->table = (struct Node**)malloc(sizeof(struct Node*)*self->capacity);
	int i;
	for(i=0;i<self->capacity;i++){
		// self->table[i] = (Node*)malloc(sizeof(Node));
		self->table[i] = NULL;
	}

	self->attr_data_types = (int*)malloc(sizeof(int)*len_of_data_types_arr);

	for(i=0; i<len_of_data_types_arr; i++){
		self->attr_data_types[i] = attr_data_types[i];
	}
}


HashTable* HashTable_record_create_param(int capacity, double load_factor, int* attr_data_types, int len_of_data_types_arr){
	
	HashTable* record_hash_table = (HashTable*)malloc(sizeof(HashTable));
	HashTable_record_init(record_hash_table, capacity, load_factor, attr_data_types, len_of_data_types_arr);

	return record_hash_table;
}

HashTable* HashTable_record_create(int* attr_data_types, int len_of_data_types_arr){

	int capacity = 12;
	double load_factor = .75;
	
	HashTable* record_hash_table = (HashTable*)malloc(sizeof(HashTable));
	HashTable_record_init(record_hash_table, capacity, load_factor, attr_data_types, len_of_data_types_arr);

	return record_hash_table;
}


int put(HashTable* self, int hash_code, int value){

	int threshold = ceil(self->capacity * self->load_factor);
	
	if(self->current_size >= threshold){
		resize(self);
	}
	
	//compute the index
	int index = hash_code % self->capacity;
	
	//create the node and make it the head
	if(self->table[index] == 0){
		struct Node* head_node = (struct Node*)malloc(sizeof(struct Node));
		head_node->key = hash_code;
		head_node->value = value;
		head_node->next_node = NULL;
		self->table[index] = head_node;
	}else{
		struct Node* old_node = self->table[index];
		struct Node* new_head_node = (struct Node*)malloc(sizeof(struct Node));
		//replace the old_head with the new head
		self->table[index] = new_head_node;
	}
	self->current_size++;
}

int put_record(HashTable* self, union record_item* key, int value, int record_length){
	
	//compute the hash code of the record_key
	int hash_code = compute_hash_code_record(self, key, record_length);
	put(self, hash_code, value);

	return 0;

}

int put_int(HashTable* self, int key, int value){
	
	int hash_code = compute_hash_code_int(self, key);
	
	put(self, hash_code, value);
	return 0;
}

int get_record(HashTable* self, union record_item* key, int key_length){
	//compute the hash_code
	int hash_code = compute_hash_code_record(self, key, key_length);

	int index = hash_code % self->capacity;

	struct Node* current_node = self->table[index];

	//loop while current_node is not null
	while(current_node!=0){
		if(current_node->key = hash_code){
			return current_node->value;
		}
	}
	return -1;

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
int compute_hash_code_int(HashTable* self, int key){
	char buffer[100];
	sprintf(buffer,"%d",key);
	
	int hash_code = compute_dec_vals(buffer,strlen(buffer));
	return hash_code;
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

int resize(HashTable* self){

	int curr_capacity = self->capacity;
	int new_capacity = curr_capacity * self->base_multiplier;

	if(self->table){
		self->table = (struct Node*)realloc(self->table,sizeof(struct Node)*new_capacity);
	}
	self->capacity = new_capacity;
}

void HashTable_destroy(HashTable* hashtable){

	if(hashtable){
		free(hashtable->attr_data_types);

		int i;
		for(i=0; i<hashtable->capacity; i++){
			//if the head_node at index i of the table is
			//not null, free it and its members
			
			if(hashtable->table[i]!=0){
				struct Node* current_node = hashtable->table[i];
				struct Node* prev_node;
				while(current_node!=0){
					prev_node = current_node;
					current_node = current_node->next_node;
					free(prev_node);
				}
				free(hashtable->table[i]);
			}
			
		}
		free(hashtable->table);

		
	}
}



