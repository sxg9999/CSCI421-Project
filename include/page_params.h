#ifndef PAGE_PARAMS_H
#define PAGE_PARAMS_H


/*
 * A structs that holds meta informations related to Page
 * : db_dir_path, page_file_name, page_id, page_size
 */
typedef struct{
    char* db_dir_path;
    char* page_file_name;
    int page_id;
    int page_size;
    int num_of_records;
    int record_item_size;
    int num_of_attributes;
    int* column_attributes;
}PageParams;


#endif