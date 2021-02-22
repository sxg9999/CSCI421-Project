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



void create_attr_data_type_arr(Page* self, int* attr_data_types);
void allocate_memory_for_records(Page* self);
int open_page(Page* self, char* file_path, char* file_name);
int get_max_records(int page_size, int record_item_size, int num_of_attributes);
void clear_n_buffer(char* buffer, int end_of_buffer);
void remove_trailing_zeros(char* src, int str_len);
int Page_write(Page* self);
int Page_read(Page* self, FILE* fp);


/**
 * A initiation function that initiates all the required components of a page
 *@param self - the current page
 *@param page_params - the parameters for initializing a page class
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
    
    // allocate memory for a array to contain the attribute data types of
    // a record and copy the parameters attribute data type values
    create_attr_data_type_arr(self, page_params->attr_data_types);
    
    self->num_of_records = 0;

    int page_exist = open_page(self, page_params->table_dir, page_params->page_file_name);


}

/**
 * Create and return a pointer to a Page struct
 * @param page_params - the parameters for constructing a page class
 * @return page - a pointer to a page struct
 */
Page* Page_create(const PageParams* page_params){
    
    Page* page = (Page*)malloc(sizeof(Page));
    Page_init(page, page_params);

    return page;
}

/**
 * Returns a record from the page with the provided id
 * @param self - a page of record
 * @param record_id - the index of the record in the page
 * @param data - a pointer to an array of record_item values that represents,
 *               the record that matches the record_id
 * @returns 0 if successful, -1 otherwise
 */

int Page_get_record(Page* self, int record_id, union record_item** data){

    union record_item* record =  self->records[record_id];

    int* attr_data_types = self->attr_data_types;
    int num_of_attributes = self->num_of_attributes;

    int i;
    for(i=0; i<num_of_attributes; i++){
        switch(attr_data_types[i]){
            case 0:
                //integer
                data[0][i].i = record[i].i;
                break;
            case 1:
                //Double
                data[0][i].d = record[i].d;
                break;
            case 2:
                //Boolean
                data[0][i].b = record[i].b;
                break;
            case 3:
                //Char
                strncpy(data[0][i].c, record[i].c, strlen(record[i].c)+1);
                break;
            case 4:
                //Varchar
                strncpy(data[0][i].v, record[i].v, strlen(record[i].v)+1);
                break;
        }
    }
}





/**
 * A function that trys to insert a record into the page
 * @param self - the current page
 * @param record - a record or in otherwords a array of record_items
 * 
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
    
    int* attr_data_types = self->attr_data_types;

    //loop through the attributes and insert the attribute
    //in their respective location 
    for(i=0; i<num_of_attributes; i++){
        
        switch(attr_data_types[i]){
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

int get_record_id(Page* self, union record_item* record){
    return 0;
}


/**
 * Update a record in the page. Primary key of old record isn't altered.
 * Find the record with the same primary key as `record` and replaces it with `record`
 * @param self - the current page
 * @param record - the updated version of the record
 * 
 * @returns 0 if successful, -1 otherwise
 */
int Page_update_record(Page* self, union record_item* record_key, union record_item* record_updated) {

    int record_id = get_record_id(self, record_key);

    union record_item* rc_up = (union record_item*)malloc( 
        self->num_of_attributes * sizeof(union record_item));
    for (int i = 0; i < self->num_of_attributes; i++) {
        switch (self->attr_data_types[i])
        {
        case 0:
            rc_up[i].i = record_updated[i].i;
            break;
        case 1:
            rc_up[i].d = record_updated[i].d;
            break;
        case 2:
            rc_up[i].b = record_updated[i].b;
            break;
        case 3:
            strncpy(rc_up[i].c, record_updated[i].c, strlen(record_updated[i].c)+1);
            break;
        case 4:
            strncpy(rc_up[i].v, record_updated[i].v, strlen(record_updated[i].v)+1);
            break;
        default:
            break;
        }
    }

    self->records[record_id] = rc_up;

    return 0;
}



/**
 * Removes a record given the record_id and update the indexs accordindly
 *
 */

int Page_remove_record(Page* self, union record_item* record_key){

    int record_id = get_record_id(self, record_key);
    //record_id is the index
    int i;
    self->num_of_records--;                 //decrement the number of records in the page
    union record_item** records = self->records;
    for(i=record_id; i<self->num_of_records; i++){
        records[i] = records[i+1];
    }

    return 0;
}


/**
 * A function responsible for freeing a page class from memory
 * @page - a page class to be destroyed
 */

void Page_destroy(Page* page){

    if(page){
        free(page->fp);
        free(page->page_file_path);
        free(page->attr_data_types);

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

/**
 * Read all the records from the file and add it to the Page
 * @param self - the current page that the record is going to be loaded to
 * @returns 0 if successful and -1 otherwise
 */

int Page_read(Page* self, FILE* fp){

    union record_item** records = self->records;
    int* attr_data_types = self->attr_data_types;
    int num_of_attributes = self->num_of_attributes;
    

    char buffer[256];
    char* ptr;
    int int_val;
    double double_val;
    
    int row = 0;
    int col = 0; 

    while(fread(buffer, sizeof(char), 256, fp)==256){
        
        switch(attr_data_types[col]){
            case 0:
                //int
                int_val = strtol(buffer, &ptr, 10);
                records[row][col].i = int_val;
                break;
            case 1:
                //double
                double_val = strtod(buffer, &ptr);
                records[row][col].d = double_val;
                break;
            case 2:
                //bool value : 1 = true, 2 = false
                int_val = strtol(buffer, &ptr, 10);
                if(int_val == 1){
                    records[row][col].b = true;
                }else if(int_val == 2){
                    records[row][col].b = false;
                }
                break;
            case 3:
                //char value
                strncpy(records[row][col].c, buffer, strlen(buffer)+1);
                break;
            case 4:
                //var char value
                strncpy(records[row][col].v, buffer, strlen(buffer)+1);
                break;
            
            default:
                printf("error");
                break;
        }
        
        if(col == num_of_attributes-1){
            col = 0;
            printf("record_%d read\n",row);
            row++;
            
        }else{
            col++;
        }
    }

    self->num_of_records = row;

    
    return 0;
}

/**
 * Function for writing the records on the page back to disk
 * and then destroying the page
 * @param self - the current page
 */

int Page_write(Page* self){
    //Write all the records back to storage
    int num_of_records = self->num_of_records;
    int num_of_attributes = self->num_of_attributes;


    //set the file pointer back to the beginning of the file
    // FILE* fp = self->fp;

    //open
    FILE* fp = fopen(self->page_file_path, "wb");

    fseek(fp, 0, SEEK_SET);      //SEEK_SET points to the beginning of the file and 
                                 //an offset of 0 from that position.

    
    union record_item** records = self->records;
    int* attr_data_types = self->attr_data_types;

    //allocate enough memory to a buffer to read a records
    char buffer[256];

    int i;
    int j;

    for(i=0; i<num_of_records; i++){
        printf("writing out record_%d\n",i);
        for(j=0; j<num_of_attributes; j++){

            switch(attr_data_types[j]){
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
                    printf("error: failed to recognize the data type of the attribute\n");
                    break;
            }

            
            //write the buffer to the file
            fwrite(buffer, sizeof(char), 256, fp);
            
            //clear the buffer
            clear_n_buffer(buffer, strlen(buffer));
            // clear_n_buffer(buffer, 256);
        }
    }

    //close file
    fclose(fp);
}


/**
 *Returns the maximum possible number of records 
 *@param page_size - the size of a page in bytes
 *@param record_item_size - the size of a record_item
 *@param num_of_attributes - number of attributes in a record
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
 * @param self - the current page
 * @param file_path - the table dir path
 * @param file_name - the name of the page file
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
        FILE* fp;
        fp = fopen(self->page_file_path, "rb");
        Page_read(self, fp);
        fclose(fp);
        return 0;
    }
    
    self->num_of_records = 0;

    //file does not exist
    // self->fp = fopen(self->page_file_path, "wb");

    return 1;
    
}

/**
 * A function responsible for allocating enough memory for a 2d array of 
 * records to store all possible records into a page
 * @param self - the current page
 */

void allocate_memory_for_records(Page* self){

    int row = self->max_num_of_records;
    int col = self->num_of_attributes;

    self->records = (union record_item**)malloc(row*sizeof(union record_item*));
    
    int i;
    
    for(i=0; i<row; i++){
        self->records[i] = (union record_item*)malloc(col*sizeof(union record_item));
    }

}

/**
 * Function for allocating memory for a attribute data type array
 * @param self - the page class that is going to store the array
 * @param attr_data_type - the array of attribute data type values
 * 
 */
void create_attr_data_type_arr(Page* self, int* attr_data_types){

    int col = self->num_of_attributes;
    
    self->attr_data_types = (int*)malloc(col*sizeof(int));
    
    int i;

    for(i=0; i<col; i++){
        self->attr_data_types[i] = attr_data_types[i];
    }

}

/**
 * Function for clearing a buffer
 * @param buffer - the buffer or in otherwords a char array/char pointer
 * @param end_of_buffer - the last index (excluded)
 */

void clear_n_buffer(char* buffer, int end_of_buffer){
    int i;
    for(i=0; i<end_of_buffer; i++){
        buffer[i]=0;
    }
}

/**
 * Function for remove the trailing zeros in a string
 * @param str - the string that is going to be worked on
 * @param len - the length of the string excluding the null terminator
 */
void remove_trailing_zeros(char* str, int len){
    int i;
    for(i=0; i<len; i++){
        if(str[i]=='0'){
            str[i]=0;
        }
    }
}


