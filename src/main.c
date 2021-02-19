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
#include <string.h>
#include "page.h"
#include "page_params.h"



union record_item {
	int i;				//0
	double d;			//1
	bool b;				//2
	char c[255];		//3
	char v[255];		//4
};


int Page_test_print_all_records(Page* page){
	printf("Num of records : %d\n", page->num_of_records);
	printf("Max num of records : %d\n", page->max_num_of_records);
	
	printf("records:\n");
	printf("---------------------------\n");

	union record_item** records = page->records;

	int i;
	int j;

	int int_val;
	int double_val;
	char* bool_val;
	char* char_val;
	char* var_char_val;

	for(i=0; i<page->num_of_records; i++){
		printf("record_%d : [ ",i);
		for(j=0; j<page->num_of_attributes; j++){
			// printf("string len: %d\n",strlen(records[i][j].c));
			switch(page->column_attributes[j]){
				case 0:
					int_val = records[i][j].i;
					printf(" %d", int_val);
					break;
				case 1:
					double_val = records[i][j].d;
					printf(" %d", double_val);
					break;
				case 2:
					bool_val = (char*)records[i][j].b ? "true": "false";
					printf(" %s", bool_val);
					break;
				case 3:
					char_val = records[i][j].c;
					printf(" %s", char_val);
					break;
				case 4:
					var_char_val = records[i][j].v;
					printf(" %s", var_char_val);
					break;
				default:
					printf(" error ");
					break;
			}
		}
		printf(" ]\n");
	}

	return 0;
}

int Page_test_print_fields(Page* page){
	printf("Page.page_file_path : %s\n", page->page_file_path);
	printf("Page.max_num_of_record: %d\n", page->max_num_of_records);
	printf("Page.num_of_attributes : %d\n", page->num_of_attributes);
	printf("Page.num_of_records : %d\n", page->num_of_records);
	printf("Page.record_size : %d\n", page->record_size);
	printf("Record_item_size : %d\n", page->record_size/page->num_of_attributes);
	printf("\n");
	

	int* attributes = page->column_attributes;

	int i;

	printf("Column_Attributes : [");
	
	for(i=0; i<page->num_of_attributes; i++){
		switch(page->column_attributes[i]){
			case 0:
				printf("<int>");
				break;
			case 1:
				printf("<double>");
				break;
			case 2:
				printf("<bool>");
				break;
			case 3:
				printf("<char>");
				break;
			case 4:
				printf("<varchar>");
				break;
			default:
				printf("<error>");
				break;
		}
	}

	printf("]\n\n");
	return 0;
}


int Page_test_inserting_a_record(Page* page){
	// Test inserting records
	union record_item record_item1;
	union record_item record_item2;
	union record_item record_item3;

	union record_item* record = (union record_item*)malloc(sizeof(union record_item)*3);

	record[0].i = 2;
	record[1].b = true;
	char str[] = "Hello world";
	record[2].c[0] = 0;
	strncpy(record[2].c,str,strlen(str));

	int page_insert_result = Page_insert_record(page, record);

	if(page_insert_result == 0){
		printf("Insert was successful\n");
	}else{
		printf("Insert failed\n");
	}

	printf("\n");

	//free up the memory
	free(record);
}


int Page_test(){

	// Page meta information
	

	char db_dir_path[] = "Test_DB_Loc/";
	char page_file_name[] = "page_1";
	int page_id = 1;
	int page_size = 10000;
	int record_item_size = 255;
	int num_of_attribute = 3;
	int column_attributes[3] = {0,2,3};			//{int, bool, char}

	PageParams page_params = {
		.db_dir_path = db_dir_path,
		.page_file_name = page_file_name,
		.page_id = page_id,
		.page_size = page_size,
		.record_item_size = record_item_size,
		.num_of_attributes = num_of_attribute,
		.column_attributes = column_attributes
	};

	Page* page1 = Page_create(&page_params);

	Page_test_print_fields(page1);
	
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);

	Page_test_print_all_records(page1);

	//another round of inserts
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	Page_test_inserting_a_record(page1);
	
	
	//write the page back to disk and destroy the page
	Page_write(page1);


	printf("Page Test done\n");
	


	return 0;
}



int main() {
	printf("test\n");
	Page_test();
	
	return 0;
}