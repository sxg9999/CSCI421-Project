#include<stdio.h>
#include<stdlib.h>
 
struct Hash_Table
{
	int capacity; //2^(times increased+1)
	int current_size;
	int load_factor;
}


struct Node
{
	int key,
	int value,
	Node* next_node;
}

*Hash_Table Hash_Table_Creator_With_Values(int capacity, int load_factor, Node arr[])
{
	//load with initial values
}

*Hash_Table Hash_Table_Creator()
{
	//initialize empty instance
}

addAll()
{
	//add a collection of pages to table
}

add()
{
	//insert new entry into table
}

resize()
{
	//if size = max size, double capacity and reallocate
}

compute_index()
{
	//What calc to use?
}









//tutorial table?
struct data 
{
	int key;
	int value;
};
 
struct data *array;
int capacity = 10;
int size = 0;
 
/* this function gives a unique hash code to the given key */
int hashcode(int key)
{
	return (key % capacity);
}
 
/* it returns prime number just greater than array capacity */
int get_prime(int n)
{
	if (n % 2 == 0) 
        {
		n++;
	}
	for (; !if_prime(n); n += 2);
 
	return n;
}
 
/* to check if given input (i.e n) is prime or not */
int if_prime(int n)
{
	int i;
	if ( n == 1  ||  n == 0) 
        {
		return 0;
	}
	for (i = 2; i < n; i++) 
        {
		if (n % i == 0) 
                {
			return 0;
		}
	}
	return 1;
}
 
void init_array()
{
	int i;
	capacity = get_prime(capacity);
	array = (struct data*) malloc(capacity * sizeof(struct data));
	for (i = 0; i < capacity; i++) 
        {
		array[i].key = 0;
		array[i].value = 0;
	}
}
 
/* to insert a key in the hash table */
void insert(int key)
{
	int index = hashcode(key);
	if (array[index].value == 0) 
        {
		/*  key not present, insert it  */
		array[index].key = key;
		array[index].value = 1;
		size++;
		printf("\n Key (%d) has been inserted \n", key);
	}
	else if(array[index].key == key) 
        {
		/*  updating already existing key  */
		printf("\n Key (%d) already present, hence updating its value \n", key);
		array[index].value += 1;
	}
	else
        {
		/*  key cannot be insert as the index is already containing some other key  */
		printf("\n ELEMENT CANNOT BE INSERTED \n");
	}
}
 
/* to remove a key from hash table */
void remove_element(int key)
{
	int index  = hashcode(key);
	if(array[index].value == 0)
        {
		printf("\n This key does not exist \n");
	}
	else {
		array[index].key = 0;
		array[index].value = 0;
		size--;
		printf("\n Key (%d) has been removed \n", key);
	}
}
 
/* to display all the elements of a hash table */
void display()
{
	int i;
	for (i = 0; i < capacity; i++)
        {
		if (array[i].value == 0)
                {
			printf("\n Array[%d] has no elements \n");
		}
		else 
                {
			printf("\n array[%d] has elements -:\n key(%d) and value(%d) \t", i, array[i].key, array[i].value);
		}
	}
}
 
int size_of_hashtable()
{
	return size;
}