
#ifndef TABLE_META_DATA_H
#define TABLE_META_DATA_H

#define TABLE_META_DATA_FILE '/dtable_meta_data.oo'

/*
 * A structs that holds meta information related to a Table
 * : db_dir_path, page_file_name, page_id, page_size
 */ 

typedef struct{
    int table_id;
    int num_columns;
    int num_keys;
    int *columns;
    int *primary_key; // 2d array because primary key can be a collection of strings
    struct page **pages;

} TableMeta;

#endif