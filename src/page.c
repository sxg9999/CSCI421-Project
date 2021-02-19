/*
 * Author: Steven Guan
 * Implementation of functions related to the Page class
 */

#include "page.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



union record_item {
	int i;
	double d;
	bool b;
	char c[255];
	char v[255];
};



//constructor

void Page_init(Page* self, PageMeta* page_meta, RecordMeta* record_meta){
    
    int max_records = get_max_records(page_meta->page_size, record_meta->record_item_size, record_meta->num_of_attributes);
    int record_size = record_meta->num_of_attributes*record_meta->record_item_size;

    // compute the amount of records a page could hold
    self->id = page_meta->page_id;
    self->record_size = record_size;
    self->max_num_of_records = max_records;
    self->num_of_attributes = record_meta->num_of_attributes;

    // allocate memory for 2d records
    allocate_memory_for_records(self);

    // allocate memory for column_attributes and copy it
    create_column_attribute_arr(self, record_meta->column_attributes);

    
    int page_exist = open_page(self, page_meta->db_dir_path, page_meta->page_file_name);

    if(page_exist == 0){
        //if the page is an existing page
        //read in the records and update the num_of_records

    }else{
        //if the page is an new page
        //then num_of_records = 0 currently
        self->num_of_records = 0;
    }

}


Page* Page_create(PageMeta* page_meta, RecordMeta* record_meta){
    
    Page* page = (Page*)malloc(sizeof(Page));
    Page_init(page, page_meta, record_meta);
    return page;
}


/*
 * assume the first column is the id 
 */

int Page_insert_record(Page* self, union record_item* record){
    int current_index = 0;


}



void Page_destroy(Page* page){


    if(page){
        free(page->fp);
        free(page->page_file_path);
        free(page->column_attributes);

        //free the records
        int i;
        int row = page->num_of_records;
        for(i=0; i<row; i++){
            free(page->records[i]);
        }
        free(page->records);

        free(page);

    }
}

/*
 * Read all the records from the file and return it as a 2d array, 
 * an array of record_item union.
 */
union record_item** read_all_records(FILE* fp, int* num_of_records, int record_size){

}

int Page_write(Page* page){
    //Write all the records back to storage

    //close file

    //free up memory
    Page_destroy(page);
}


/*
 *Returns the maximum possible number of records 
 *@param record_size - the size of a union record_item
 */
int get_max_records(int page_size, int record_item_size, int num_of_attributes){

    //compute the estmated max # of record_items unions in a page
    float est_max_record_items = (float)page_size/(float)record_item_size;

    //compute the estimated max # of records (array of unions) in a page
    float est_max_records = est_max_record_items/(float)num_of_attributes;      //or you can think of it as max rows of records

    //floor the est_max_records
    int max_records = floor(est_max_records);

    return max_records;

}

/**
 * Attempts to open the page file and returns whether
 * or not the page file existed before.
 * @return - 0 for exist and 1 for does not exist
*/
int open_page(Page* self, char* file_path, char* file_name){

    int file_path_size = strlen(file_path);
    int file_name_size = strlen(file_name);

    self -> page_file_path = (char*)malloc(file_path_size+file_name_size);
    self -> page_file_path[0] = 0;

    strncat(self->page_file_path, file_path, file_path_size);
    strncat(self->page_file_path, file_name, file_name_size);

    struct stat buffer;

    int exist = stat(self->page_file_path, &buffer);

    if(exist == 0){
        //file exists
        self -> fp = fopen(self->page_file_path, "r+");
        return 0;
    }

    //file does not exist
    self -> fp = fopen(self->page_file_path, "w+");

    return 1;
    
}

void allocate_memory_for_records(Page* self){

    int row = self->max_num_of_records;
    int col = self->num_of_attributes;

    self->records = (union record_item**)malloc(row*sizeof(union record_item*));
    
    int i;
    
    for(i=0; i<row; i++){
        self->records[i] = (union record_item*)malloc(col*sizeof(union record_item));
    }

}

/* Allocate memory to page.column_attribute and then
 * copy the given column_attribute (an array of int) to page.column_attribute
 * 
*/
void create_column_attribute_arr(Page* self, int* column_attributes){

    int col = self->num_of_attributes;
    //int* column_attributes = (int*)malloc(column*sizeof(int))
    self->column_attributes = (int*)malloc(col*sizeof(int));

    int i;

    for(i=0; i<col; i++){
        self->column_attributes[i] = column_attributes[i];
    }

}

