
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hashtable.h"




HashTable* HashTable_record_create_param(int capacity, int load_factor);

HashTable* HashTable_record_create();



int put(HashTable* self, int key, int value){

}

int put_record(HashTable* self, union record_item* key, int value, int record_length){

	int threshold = ceil(self->capacity * self->load_factor);

	if(self->current_size >= threshold){
		resize(self);
	}

}


int compute_hash_code_record(HashTable* self, union record_item* key, int record_length){
	
	int* attr_data_types = self->attr_data_types;
	int num_of_attr = self->num_of_attr;
	int hash_code = 0;

	// char key_str[]
	//parse the key

	int i;
	int j;
	for(i=0; i<record_length; i++){
		for(j=0; j<num_of_attr; j++){
			switch(attr_data_types[j]){
				case 0:
					//int

					break;
				case 1:
					//double
					break;
				case 2:
					//bool
					break;
				case 3:
					//char
					break;
				case 4:
					//varchar
					break;
				default:
					break;
			}
		}
	}
	
}


int compute_index(int hash_code);

int resize(HashTable* table);

void HashTable_destroy();



// struct Hash_Table * Hash_Table_Creator_With_Values(int capacity, int load_factor, Node arr[])
// {
// 	struct Hash_Table* table = malloc(sizeof(struct Hash_Table));
// 	//table->capacity = capacity;
// 	//table->current_size = 0;
// 	//load_factor-> = load_factor;
// 	//fill with values
// 	//addAll(arr);
// };

// struct Hash_Table * Hash_Table_create()
// {
// 	//initialize empty instance

// 	struct Hash_Table *table = malloc(sizeof(struct Hash_Table));

// 	//table->capacity = 2;
// 	//table->current_size = 0;
// 	//load_factor-> = 2;
	
// 	return table;
// };

// void Hash_Table_Destroy(struct Hash_Table table)
// {
// 	//if table not empty (!= NULL)
// 	//for every item in the table
// 	//if != NULL, free
// };

// int hashcode(struct Hash_Table *table, int key)
// {
// 	return (key % table->capacity);
// };

// int addAll(struct Node arr[])
// {
// 	//add a collection of pages to table
// };

// int add(struct Node node)
// {
// 	//insert new entry into table
// };

// void resize(struct Hash_Table table)
// {
// 	//if size = max size, double capacity and reallocate
// };

// int compute_index(int key, int value)
// {
// 	//What calc to use?
// };











// //tutorial table?
// struct data 
// {
// 	int key;
// 	int value;
// };
 
// struct data *array;
// int capacity = 10;
// int size = 0;





 
// /* this function gives a unique hash code to the given key */
// int hashcode(int key)
// {
// 	return (key % capacity);
// }
 
// /* it returns prime number just greater than array capacity */
// int get_prime(int n)
// {
// 	if (n % 2 == 0) 
//         {
// 		n++;
// 	}
// 	for (; !if_prime(n); n += 2);
 
// 	return n;
// }
 
// /* to check if given input (i.e n) is prime or not */
// int if_prime(int n)
// {
// 	int i;
// 	if ( n == 1  ||  n == 0) 
//         {
// 		return 0;
// 	}
// 	for (i = 2; i < n; i++) 
//         {
// 		if (n % i == 0) 
//                 {
// 			return 0;
// 		}
// 	}
// 	return 1;
// }
 
// void init_array()
// {
// 	int i;
// 	capacity = get_prime(capacity);
// 	array = (struct data*) malloc(capacity * sizeof(struct data));
// 	for (i = 0; i < capacity; i++) 
//         {
// 		array[i].key = 0;
// 		array[i].value = 0;
// 	}
// }
 
// /* to insert a key in the hash table */
// void insert(int key)
// {
// 	int index = hashcode(key);
// 	if (array[index].value == 0) 
//         {
// 		/*  key not present, insert it  */
// 		array[index].key = key;
// 		array[index].value = 1;
// 		size++;
// 		printf("\n Key (%d) has been inserted \n", key);
// 	}
// 	else if(array[index].key == key) 
//         {
// 		/*  updating already existing key  */
// 		printf("\n Key (%d) already present, hence updating its value \n", key);
// 		array[index].value += 1;
// 	}
// 	else
//         {
// 		/*  key cannot be insert as the index is already containing some other key  */
// 		printf("\n ELEMENT CANNOT BE INSERTED \n");
// 	}
// }
 
// /* to remove a key from hash table */
// void remove_element(int key)
// {
// 	int index  = hashcode(key);
// 	if(array[index].value == 0)
//         {
// 		printf("\n This key does not exist \n");
// 	}
// 	else {
// 		array[index].key = 0;
// 		array[index].value = 0;
// 		size--;
// 		printf("\n Key (%d) has been removed \n", key);
// 	}
// }
 
// /* to display all the elements of a hash table */
// void display()
// {
// 	int i;
// 	for (i = 0; i < capacity; i++)
//         {
// 		if (array[i].value == 0)
//                 {
// 			printf("\n Array[%d] has no elements \n");
// 		}
// 		else 
//                 {
// 			printf("\n array[%d] has elements -:\n key(%d) and value(%d) \t", i, array[i].key, array[i].value);
// 		}
// 	}
// }
 
// int size_of_hashtable()
// {
// 	return size;
// }