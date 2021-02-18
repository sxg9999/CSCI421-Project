/*
 * Author: Steven Guan
 * Header file for a Page class
 */



#ifndef PAGE_H
#define PAGE_H

#include "meta_struct.h"

typedef struct{

    int id;                                //the page_id
    int num_of_records;                    //the amount of record
    int max_num_of_records;
    int num_of_attributes;
    int record_size;                       //the size of a record (size of record_item * col)
    int* column_attributes;                //a pointer to a array of column attribute values
    FILE* fp;                              //a pointer to the page file
    char* page_file_path;
    union record_item** records;           //the an array of records ([])

}Page;


/*
Create and return a pointer to a Page struct
@param records - a array of records thats contains record_items
@returns a pointer to a page struct
*/
Page* Page_create(char* file_path, char* file_name, int* column_attributes, 
            int page_id, int page_size, int record_item_size, int num_of_attributes);

/*
Returns the records with the specified record_id in the table

@param record_id - the id of the record in the table
@returns an pointer to a array of record_items
*/
union record_item* get_record(int record_id);


/*
Inserts the provided record into the table witht he provided id.
@param record - the record to be inserted to the page, an array of record_items
@return 0 if record is successfully removed, -1 otherwise
*/
int insert_record(union record_item* record);


/*
Updates the provided record into the page with the provided page_id
@param record_id
@param record
@return 0 if record is successfully removed, -1 otherwise
*/
int update_record(int record_id, union record_item* record);



/*
Removes the record specified by the key_values
@param record_id
@return 0 if record is successfully removed, -1 otherwise
*/
int remove_record(int record_id);


/*
 * Write the page back to storage
 */ 
int write(Page* page);


#endif