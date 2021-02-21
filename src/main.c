/*
 * Author: Dylan Cuprewich (dxc2961), Steven Guan (sxg8944)
 * Driver/tester for CSCI421 Group Project Storage Manager
 * Simply to be used for testing functionality or debugging purposes.
 * 
 * NOTE: DO NOT PUT RELEVANT CODE HERE.
 * PUT IMPORTANT CODE IN "storagemanager.c" OR SOME OTHER .c FILE!!!
 * WE ARE NOT SUBMITTING "main.c"!
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "generator/record_generator.h"
#include "test/page_test.h"

// union record_item {
// 	int i;
// 	double d;
// 	bool b;
// 	char c[255];
// 	char v[255];
// };



int main() {
	// printf("here");
	
	RecordGenerator* record_gen = RecordGen_create();
	//id, fname, lname, age, weight, department
	// char attributes[6][10] = {
	// 	"id",
	// 	"fname",
	// 	"age",
	// 	"weight",
	// 	"department"
	// };

	char** attributes = (char**)malloc(sizeof(char*)*5);
	attributes[0] = (char*)malloc(sizeof(char)*10);\
	attributes[0][0] = 0;
	strncpy(attributes[0],"id",3);

	attributes[1] = (char*)malloc(sizeof(char)*10);
	attributes[1][0] = 0;
	strncpy(attributes[1],"fname",6);

	attributes[2] = (char*)malloc(sizeof(char)*10);
	attributes[2][0] = 0;
	strncpy(attributes[2],"age",4);

	attributes[3] = (char*)malloc(sizeof(char)*10);
	attributes[3][0] = 0;
	strncpy(attributes[3],"weight", 8);

	attributes[4] = (char*)malloc(sizeof(char)*10);
	attributes[4][0] = 0;
	strncpy(attributes[4],"department",11);

	int attr_vals[] = {0,3,0,0,3};

	// allocate memory for 2d array of records

	int num_of_attributes = 5;
	int num_of_records = 20;


	union record_item** records = (union record_item**)malloc(num_of_records*sizeof(union record_item*));

	int i;
	for(i=0; i<num_of_records; i++){
		records[i] = (union record_item*)malloc(num_of_attributes*sizeof(union record_item));
	}

	// printf("perhaps here");
	
	//generate all the records
	gen_records_rand(record_gen, records, num_of_records, attributes, num_of_attributes);

	

	print_all_records(records, num_of_records, num_of_attributes, attr_vals);

	// free the records
	for(i=0; i<num_of_records; i++){
		free(records[i]);
	}
	free(records);

	
	for(i=0; i<num_of_attributes; i++){
		free(attributes[i]);
	}
	free(attributes);

	return 0;
}