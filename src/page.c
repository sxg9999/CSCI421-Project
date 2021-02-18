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

void Page_init(Page* self, char* file_path, char* file_name, int page_id, int num_of_attributes,
                                int max_records, int record_size){
    
    // compute the amount of records a page could hold
    self->id = page_id;
    self->record_size = record_size;
    self->max_num_of_records = max_records;

    int page_exist = open_page(self, file_path, file_name);

}


Page* Page_create(char* file_path, char* file_name,int page_id,
                int page_size, int record_item_size, int num_of_attributes){
    

    Page* page = (Page*)malloc(sizeof(Page));
    int max_records = get_max_records(page_size, record_item_size, num_of_attributes);
    int record_size = num_of_attributes*record_item_size;
    Page_init(page, file_path, file_name, page_id, num_of_attributes, max_records, record_size);
    return page;
}


/*
 * assume the first column is the id 
 */

int insert_record(union record_item* record){
    int index = (*record).i;



}





void Page_destroy(Page* page){
    if(page){
        free(page->fp);
        free(page->page_file_path);
        free(page);
    }
}

/*
 * Read all the records from the file and return it as a 2d array, 
 * an array of record_item union.
 */
union record_item** read_all_records(FILE* fp, int* num_of_records, int record_size){

}

int write(Page* page){
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
    float est_max_records = est_max_record_items/(float)num_of_attributes;

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

