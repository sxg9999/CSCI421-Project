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


void create_column_attribute_arr(Page* self, int* column_attribute);
void allocate_memory_for_records(Page* self);
int open_page(Page* self, char* file_path, char* file_name);
int get_max_records(int page_size, int record_item_size, int num_of_attributes);
void clear_n_buffer(char* buffer, int end_of_buffer);
void remove_trailing_zeros(char* src, int str_len);
int Page_write(Page* self);
int Page_read(Page* self);


/*
 * A initiation function that initiates all the required components of a page
 *
 */ 
void Page_init(Page* self,const PageParams* page_params){
    
    int max_records = get_max_records(page_params->page_size, page_params->record_item_size, page_params->num_of_attributes);
    int record_size = page_params->num_of_attributes*page_params->record_item_size;

    // compute the amount of records a page could hold
    self->id = page_params->page_id;
    self->record_size = record_size;
    self->max_num_of_records = max_records;
    self->num_of_attributes = page_params->num_of_attributes;

    // allocate memory for 2d records
    allocate_memory_for_records(self);
    
    // allocate memory for column_attributes and copy it
    create_column_attribute_arr(self, page_params->column_attributes);
    
    self->num_of_records = 0;

    int page_exist = open_page(self, page_params->db_dir_path, page_params->page_file_name);

    

    if(page_exist == 0){
        //if the page is an existing page
        //read in the records and update the num_of_records
        self->num_of_records = page_params->num_of_records;
        Page_read(self);

    }else{
        //if the page is an new page
        //then num_of_records = 0 currently
        self->num_of_records = 0;
    }

}

/*
 * The constructor for a page class
 * 
 */
Page* Page_create(const PageParams* page_params){
    
    Page* page = (Page*)malloc(sizeof(Page));
    Page_init(page, page_params);


    return page;
}



/*
 * assume the first column is the id 
 * @returns 0 if successful, 1 if page is full, -1 otherwise
 */

int Page_insert_record(Page* self, union record_item* record){

    //check if page is full
    if(self->num_of_records >= self->max_num_of_records){
        return 1;
    }

    int index = self->num_of_records;           //num of records can be used as the index for the next record
    self->num_of_records++;                     //increment the total number of records

    int i;
    int num_of_attributes = self->num_of_attributes;
    int attribute_value;

    //loop through the attributes and insert the attribute
    //in their respective location 
    for(i=0; i<num_of_attributes; i++){
        attribute_value = self->column_attributes[i];

        switch(attribute_value){
            case 0:
                //integer
                self->records[index][i].i = record[i].i;
                break;
            case 1:
                //Double
                self->records[index][i].d = record[i].d;
                break;
            case 2:
                //Boolean
                self->records[index][i].b = record[i].b;
                break;
            case 3:
                //Char
                strncpy(self->records[index][i].c,record[i].c,strlen(record[i].c)+1);
                break;
            case 4:
                //Varchar
                strncpy(self->records[index][i].v,record[i].v,strlen(record[i].v));
                break;
        }
    }

    return 0;
}

/*
 * A function responsible for freeing a page class from memory
 * @page - a page class
 */

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
 * Read all the records from the file and add it to the Page
 */
int Page_read(Page* self){
    int num_of_records = self->num_of_records;
    int num_of_attributes = self->num_of_attributes;

    FILE* fp = self->fp;

    union record_item** records = self->records;
    int* column_attributes = self->column_attributes;

    char buffer[256];
    char *ptr;

    int int_val;
    double double_val;
    char* str_val;

    int i;
    int j;

    for(i=0; i<num_of_records; i++){
        printf("reading in record_%d\n",i);

        for(j=0; j<num_of_attributes; j++){
            fread(buffer, sizeof(char), 256, fp);
            switch(column_attributes[j]){
                case 0:
                    //an integer
                    int_val = strtol(buffer, &ptr, 10);
                    records[i][j].i = int_val;
                    break;
                case 1:
                    //double
                    double_val = strtod(buffer, &ptr);
                    records[i][j].d = double_val;
                    break;
                case 2:
                    //bool value
                    int_val = strtol(buffer, &ptr, 10);
                    if(int_val == 1){
                        records[i][j].b = true;
                    }else{
                        records[i][j].b = false;
                    }
                    break;
                case 3:
                    //char value
                    strncpy(records[i][j].c, buffer, strlen(buffer)+1);
                    printf("length of the str : %d\n",strlen(records[i][j].c));
                    break;
                case 4:
                    strncpy(records[i][j].v, buffer, strlen(buffer)+1);
                    break;
                default:
                    break;
            }
        }
    }


}

int Page_write(Page* self){
    //Write all the records back to storage
    int num_of_records = self->num_of_records;
    int num_of_attributes = self->num_of_attributes;


    //set the file pointer back to the beginning of the file
    FILE* fp = self->fp;
    fseek(fp, 0, SEEK_SET);      //SEEK_SET points to the beginning of the file and 
                                 //an offset of 0 from that position.

    

    
    union record_item** records = self->records;
    int* column_attributes = self->column_attributes;

    char buffer[256];

    int i;
    int j;

    for(i=0; i<num_of_records; i++){
        printf("writing out record_%d\n",i);
        for(j=0; j<num_of_attributes; j++){

            switch(column_attributes[j]){
                case 0:
                    //convert int to string
                    sprintf(buffer, "%d", records[i][j].i);
                    break;

                case 1:
                    //convert double to string
                    sprintf(buffer, "%f", records[i][j].d);
                    //remove trailing zeros from the converted string
                    remove_trailing_zeros(buffer, strlen(buffer));
                    break;

                case 2:
                    strncpy(buffer, records[i][j].b ? "1": "0", 2);
                    break;

                case 3:
                    strncpy(buffer, records[i][j].c, strlen(records[i][j].c)+1);
                    break;

                case 4:
                    strncpy(buffer, records[i][j].v, strlen(records[i][j].v)+1);
                    break;
                
                default:
                    printf(" error ");
                    break;
            }


            
            //write the buffer to the file
            fwrite(buffer, sizeof(char), 256, fp);
            printf("\tBuffer content : %s, len: %d\n", buffer, strlen(buffer));

            //clear the buffer
            // clear_n_buffer(buffer, strlen(buffer));
            clear_n_buffer(buffer, 256);
        }
    }

    //close file
    fclose(self->fp);
    //free up memory
    Page_destroy(self);
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
    self->fp = fopen(self->page_file_path, "w+");

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

/*
 * function for clearing a buffer
 */

void clear_n_buffer(char* buffer, int end_of_buffer){
    int i;
    for(i=0; i<end_of_buffer; i++){
        buffer[i]=0;
    }
}

/*
 * function for remove the trailing zeros in a string
 */
void remove_trailing_zeros(char* str, int len){
    int i;
    for(i=0; i<len; i++){
        if(str[i]=='0'){
            str[i]=0;
        }
    }
}


