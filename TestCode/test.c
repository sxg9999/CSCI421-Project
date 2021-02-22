#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "../include/hashtable.h"

typedef struct{
    int id;
    int age;
    char* name;
}Person;


// union record_item {
// 	int i;
// 	double d;
// 	bool b;
// 	char c[255];
// 	char v[255];
// };



typedef union{
    char c[255];
}record_item;

void person_struct_test(){
    Person person;

    person = (Person){.age = 10, .id = 1, .name = "bob"};
    
    printf("Person: %d, %d, %s", person.id, person.age, person.name);
}

void clear_n_buffer(char* buffer, int end_of_buffer){
    int i;
    for(i=0; i<end_of_buffer; i++){
        buffer[i]=0;
    }
}

void remove_trailing_zeros(char* str, int len){
    // printf("length of string is: %d\n", len);
    int i;
    for(i=0; i<len; i++){
        if(str[i]=='0'){
            str[i]=0;
        }
    }
    // printf("result str: %s\n",str);
}

void convert_from_int_to_string(){

    int int_val = 13;

    char buffer[255];

    sprintf(buffer,"%d",int_val);

    printf("(int to str)The buffer contains : %s\n",buffer);
}

void convert_from_double_to_string(){
    
    double double_val = 24.12;

    char buffer[255];
    

    sprintf(buffer,"%.6f",double_val);

    remove_trailing_zeros(buffer, strlen(buffer));

    printf("(double to str)The buffer contains : %s\n",buffer);

}


void test_ternary_operator(){

    char result[255];
    
    strncpy(result, false?"true":"false",60);

    printf("size of result str : %d\n", strlen(result));
    printf("result : %s\n", result);

}


void test_pointing_strings(){

    char buffer[20];
    char name[] ="bobtheknowitall";

    


}

void test_getting_length_from_a_pointer(){
    record_item record_item1;
    strncpy(record_item1.c, "hello", 6);
    
    printf("the length of the record_item : %d\n", strlen(record_item1.c));

}


void test_converting_string_to_any(){

    char val[] = "123 hello";

    // int int_val = atoi(val);

    char* ptr;

    double double_val = strtod(val,&ptr);



    printf("double val is : %f\n",double_val);
    printf("ptr contains : %s\n",ptr);

    int int_val = strtol(val, &ptr, 10);    //base 10 for dec
    printf("int val is : %d\n", int_val);
    printf("ptr contains : %s\n",ptr);
    

}

int test_mod(){

    return 0;
}


int test_randnum_gen(){

    srand(0);

    int upper_limit = 3;
    int lower_limit = 0;

    int count = 10;
    int i;
    int num;
    for(i=0; i<count; i++){
        num = (rand()%(upper_limit-lower_limit+1)) + lower_limit;
        printf("%d ", num);
    }

    return 0;
}


int test_comparing_strs(){

    char str1[] = "Hello";
    char str2[] = "Hello";

    // printf("len of str1 : %d\n", strnlen(str1,6));
    // printf("len of str2 : %d\n", strnlen(str2,6));

    int val;
    if(strncmp(str1,str2, strlen(str1)+1) == 0){
        printf("same\n");
    }else{
        printf("different\n");
    }

    printf("val of cmp is : %d\n",val);
    
}


/**
 * 
 * Result: setting a pointer to point to something else 
 * can acheive the effect of a remove function
 */
int test_moving_records_pointer(){
    union record_item** records = (union record_item **)malloc(sizeof(union record_item*)*5);

    int i;

    for(i=0; i<5; i++){
        records[i] = (union record_item*)malloc(sizeof(union record_item)*4);
    }

    records[0][0].i=2;
    records[1][0].i=3;
    records[2][0].i=4;
    records[3][0].i=5;
    records[3] = records[0];

    printf("before: removing record 2 its int val is: %d\n", records[2][0].i);

    records[2] = records[3];
    printf("After: removing record 2 its int val is: %d\n", records[2][0].i);
    // test_randnum_gen();
    // test_comparing_strs();
    printf("Record 3's int value after setting it to point to record 0 is : %d\n", records[3][0].i);
}




int test_allocating_2d_array(){
    char** char_2d = (char**)malloc(sizeof(char*)*3);

    char_2d[0] = "Helloworld";
    char_2d[1] = "Bob";
    char_2d[2] = "builder";

    // char_2d[0] = "Hello world";

    int i;
    for(i=0; i<3; i++){
        printf("char_2d[%d] is %s\n", i, char_2d[i]);
    }
}



int test_converting_chars_to_int_value(){

    char key_string[] = "hello world";

    int key_len = strlen(key_string);

    int i;

    int resulted_key = 0;

    printf("(str) Key is : ");

    for(i=0; i<key_len; i++){
        printf("%c",key_string[i]);
        resulted_key += (int)key_string[i];
        printf("(int) key is : %d\n", key_string[i]);
    }

    printf("(int) key is : %d\n", resulted_key);

    //hash_code (hash function for the key)
    int hash_code = resulted_key;

    //assume array has length of 10;
    int arr_length = 10;
    int index_of_key = hash_code%arr_length;

    printf("the index of the key is : %d\n", index_of_key);

    printf("5.5/2 is %f\n", 5.5/2);
    printf("the ceil of 5/2 is : %f\n", ceil(5.5/2));

}

void Test_clear_n_buffer(char* buffer, int end_of_buffer){
    int i;
    for(i=0; i<end_of_buffer; i++){
        buffer[i]=0;
    }
}

int test_compute_dec_vals(char* str, int length){
	int result = 0;
	
	int i;
	for(i=0; i<length; i++){
		result += (int)str[i];
	}

	return result;
}

int test_compute_hash_code(union record_item* key, int* attr_data, int record_length){
    int* attr_data_types = attr_data;
	
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
					hash_code += test_compute_dec_vals(buffer, strlen(buffer));
					break;
				case 1:
					//double
					sprintf(buffer, "%d", (int)key[i].d);
					hash_code += test_compute_dec_vals(buffer, strlen(buffer));
					break;
				case 2:
					//bool
					if(key[i].b==true){
						hash_code += test_compute_dec_vals("true", 4);
					}else{
						hash_code += test_compute_dec_vals("false", 5);
					}
					break;
				case 3:
					hash_code += test_compute_dec_vals(key[i].c, strlen(key[i].c));
					//char
					break;
				case 4:
					//varchar
					hash_code += test_compute_dec_vals(key[i].c, strlen(key[i].c));
					break;
				default:
					break;
		}
        printf("hash_code is : %d\n", hash_code);
		Test_clear_n_buffer(buffer, strlen(buffer)+1);
	}
    return hash_code;
}


int test_hash_record(){
    int data_types[] = {0, 2, 3};

    union record_item* record = (union record*)malloc(sizeof(union record_item)*3);
    
    record[0].i=12;
    record[1].b=true;
    record[2].c[0] = 0;
    strncpy(record[2].c, "hello world", 12);

    int hash_code = test_compute_hash_code(record, &data_types, 3);
    printf("The hash code is : %d, index is : %d\n", hash_code, hash_code%10);


    free(record);
}


int test_reallocating_pointers(){
    int* int_arr = (int*)malloc(sizeof(int)*5);

    int_arr[0] = 2012;

    printf("int_arr[0] is %d\n", int_arr[0]);

    int_arr = (int*)realloc(int_arr, sizeof(int)*10);
    printf("int_arr[0] is %d\n", int_arr[0]); // realloc copies over the pointers from the old pointer

    free(int_arr);
}


// int test_checking_for_null(){
//     Node** node_arr = (Node**)malloc(sizeof(Node*)*3);
//     // node_arr[0] = (Node*)malloc(sizeof(Node));
//     node_arr[0] = NULL;
//     // node_arr[1] = (Node*)malloc(sizeof(Node));

//     int* int_arr = (int*)malloc(sizeof(int)*3);

//     int_arr[0] = 3;
//     int_arr[1] = 2;

//     printf("int arr[0] is %d\n", int_arr[0]);

//     free(int_arr);

//     printf("Node is : %d\n", node_arr[0]);

//     if(node_arr[0] == 0){
//         printf("node is null");
//     }

//     int i;

//     for(i=0; i<2; i++){
//         free(node_arr[i]);
//     }

//     free(node_arr);


    
//     return 0;
// }

int get_random_num(int lower, int upper){
    
    int num = (rand()%(upper-lower+1))+lower;
}




union record_item** create_records(int num_of_records){
    union record_item** records = (union record_item**)malloc(sizeof(union record_item*)*num_of_records);

    int num_of_attributes = 2;
    
    

    char random_names[][10] = {
        "bob",
        "ash",
        "mercy",
        "moira",
        "baptise",
    };

    int i;

    for(i=0; i<num_of_records; i++){
        records[i] = (union record_item*)malloc(sizeof(union record_item)* num_of_attributes);
        records[i][0].i = i;
        records[i][1].c[0] = 0;

        // int randnum = get_random_num(0, 1000);
        char combined_str[100];

        char buffer[100];
        
        char name[3] = "bob";

        sprintf(buffer, "%d", i);
        strncat(combined_str, name, 4);
        strncat(combined_str, buffer, sizeof(buffer)+1);

        strncpy(records[i][1].c, combined_str, sizeof(combined_str)+1);
        clear_n_buffer(buffer, 100);
        clear_n_buffer(combined_str, 100);
        printf("name is : %s\n", records[i][1].c);
    }

    return records;
}

int test_hash_table(){

    union record_item** records = create_records(4);

    int attr_data_types[] = {0,3};

    HashTable* hashtable = HashTable_record_create(&attr_data_types, 2);

    put_record(hashtable,records[0],100, 2);

    
    int i;

    for(i=0; i<4; i++){
        printf("name is : %s\n", records[i][1].c);
    }


    for(i=0; i<4; i++){
        free(records[i]);
    }
    free(records);

    // int i;

    // for(i=0; i<5; i++){

    // }

}


int main(){
    srand(0);
    test_hash_table();
    // test_converting_chars_to_int_value();

    // printf("result of null term in dec is : %d", '\0');

    // printf("int casted double value of 2.3 is : %d\n", (int)2.3);

    return 0;
}