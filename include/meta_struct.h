/*
 * Author: Steven Guan
 * 
 * Header file for meta structs
 *
 */

#ifndef PARAMS_H
#define PARAMS_H



/*
 * A structs that holds meta informations related to a Page
 * : db_dir_path, page_file_name, page_id, page_size
 */
typedef struct{
    char* db_dir_path;
    char* page_file_name;
    int page_id;
    int page_size;
}PageMeta;



/*
 *A struct that holds meta informations related to a Record
 * : record_item_size, column_attributes, num_of_attributes
*/
typedef struct{
    int record_item_size;
    int num_of_attributes;
    int* column_attributes;
}RecordMeta

#endif
 