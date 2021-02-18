#ifndef PAGE_META_DATA_H
#define PAGE_META_DATA_H


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


#endif