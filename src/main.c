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
#include "page.h"
#include "page_params.h"

int Page_test(){

	// Page meta information
	

	char db_dir_path[] = "Test_DB_Loc/";
	char page_file_name[] = "page_1";
	int page_id = 1;
	int page_size = 10000;
	int record_item_size = 255;
	int num_of_attribute = 3;
	int column_attributes[3] = {0,0,0};

	PageParams page_params = {
		.db_dir_path = db_dir_path,
		.page_file_name = page_file_name,
		.page_id = page_id,
		.record_item_size = record_item_size,
		.num_of_attributes = num_of_attribute,
		.column_attributes = column_attributes
	};


	// Record meta information
	

	Page* page1 = Page_create(&page_params);

	printf("Page.page_file_path : %s\n\n", page1->page_file_path);

	int* attributes = page1->column_attributes;

	int i;

	printf("[\n");
	
	for(i=0; i<page1->num_of_attributes; i++){
		printf("Attribute %d : %d\n", i, page1->column_attributes[i]);
	}

	printf("]\n");

	Page_destroy(page1);
	

	printf("Page Test done\n");
	


	return 0;
}



int main() {
	printf("test\n");
	Page_test();
	
	return 0;
}