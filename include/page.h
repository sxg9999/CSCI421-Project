/*
 * Author: Steven Guan
 * Header file for a Page class
 */



#ifndef PAGE_H
#define PAGE_H

#include "page_params.h"
#include "stdio.h"


typedef struct{

    int id;                                //the page_id
    int num_of_records;                    //the amount of record
    int max_num_of_records;
    int num_of_attributes;
    int record_size;                       //the size of a record (size of record_item * col)
    int* attr_data_types;                  //a pointer to a array of attribute data types values
    FILE* fp;                              //a pointer to the page file
    char* page_file_path;
    union record_item** records;           //the an array of records ([])

}Page;


/**
 * Create and return a pointer to a Page struct
 * @param page_params - the parameters for constructing a page class
 * @return page - a pointer to a page struct
 */
Page* Page_create(const PageParams* page_params);


/*
Inserts the provided record into the page
@param record - the record to be inserted to the page, an array of record_items
@return 0 if record is successfully inserted, -1 otherwise
*/
int Page_insert_record(Page* self, union record_item* record);


/* 
 * Returns a record from the page with the provided id
 * @param self - a page of record
 * @param record_id - the index of the record in the page
 * @param data - a pointer to an array of record_item values that represents,
 *               the record that matches the record_id
 * @returns 0 if successful, -1 otherwise
 */
int Page_get_record(Page* self, int record_id, union record_item** data);

/*
Updates the provided record into the page with the provided record_id
@param record_id
@param record
@return 0 if record is successfully updated, -1 otherwise
*/
<<<<<<< HEAD
int Page_update_record(Page* self, union record_item* record_key, union record_item* record);
=======
int Page_update_record(Page* self, int record_id, union record_item* record);
>>>>>>> chaudron_table



/*
Removes the record specified by the record_id
@param record_id
@return 0 if record is successfully removed, -1 otherwise
*/
<<<<<<< HEAD
int Page_remove_record(Page* self, union record_item* record_key);
=======
int Page_remove_record(Page* self, int record_id);
>>>>>>> chaudron_table


/*
 * Write the page back to storage
 * @param page - the page to be written to disk
 */ 
int Page_write(Page* page);


/**
 * Destroy a page
 *@param page - the page to be destroyed
 */
void Page_destroy(Page* page);

#endif