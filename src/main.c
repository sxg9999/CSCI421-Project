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
#include "page_meta_data.h"

int Page_test(){

	// Page meta information
	PageMeta page_meta;

	char db_dir_path[] = "Test_DB_Loc/";
	char page_file_name[] = "page_1";
	int page_id = 1;
	int page_size = 10000;

	page_meta.db_dir_path = db_dir_path;
	page_meta.page_file_name = page_file_name;
	page_meta.page_id = page_id;
	page_meta.page_size = page_size;


	// Record meta information
	RecordMeta record_meta;

	int record_item_size = 255;
	int num_of_attribute = 3;
	int column_attributes[3] = {0,0,0};


	Page* page1 = Page_create(&page_meta, &record_meta);

	printf("Page.page_file_path : %s\n", page1->page_file_path);

	int* attributes = page1->column_attributes;

	int i;

	// printf("[");
	
	// for(i=0; i<page1->num_of_attributes; i++){
	// 	printf("%d ",attributes[i]);
	// }


	// printf("]");

	Page_destroy(page1);
	

	printf("Page Test done\n");
	


	return 0;
}



int main() {
	printf("test\n");
	Page_test();
	
	return 0;
}