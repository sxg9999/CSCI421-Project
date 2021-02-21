#ifndef PAGE_PARAMS_H
#define PAGE_PARAMS_H


/*
 * A structs that holds the Page class's required parameters
 * 
 */
typedef struct{
    char* table_dir;               //path to the table dir
    char* page_file_name;          //name of the page
    int page_id;                   //id of the page
    int page_size;                 //size of the page in bytes
    int record_item_size;          //size of an record_item in bytes
    int num_of_attributes;         //number of attributes in a row
    int* attr_data_types;          //the data types of each attributes/columns in order
}PageParams;


#endif