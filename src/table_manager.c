#include "../include/table_manager.h"

#define PATH "../"

// PLACEHOLDER
int get_item(HashTable* self, int key, void* item) {
    return 0;
}

TableManager* init_table_manager(char* db_loc, int page_size)
{
    TableManager* tm = (TableManager*) malloc(sizeof(TableManager));
    tm->db_loc = db_loc;
    tm->page_size = page_size;
    tm->tables = (HashTable*) malloc(sizeof(HashTable));

    return tm;
}

int TM_get_table(TableManager* self, int table_id, Table* table) {
    if(get_item(self->tables, table_id, (void*)table) == -1) {
        printf(stderr, "Table with ID %d does not exist.\n", table_id);
        return -1;
    }
    
    return 0;
}



char* get_table_dir(int table_id, char* db_loc)
{
    char t[255] = "/Table";
    char dirStr[255];
    strcpy(dirStr, db_loc);

    itoa(table_id, t+6, 10);
    strcat(dirStr, t);
    strcat(dirStr, "/");

    return dirStr;
}

int TM_add_table(TableManager* self, int* data_types, int* key_indices, int data_types_size, int key_indices_size)
{
    // NOT GUARANTEED UNIQUE ID, just very unlikely to repeat
    srand(time(NULL));
    int table_id = rand();

    char* table_dir = get_table_dir(table_id, self->db_loc);

    mkdir(table_dir, S_IRWXO);

    TableParams params = (TableParams) {table_id,
                                        self->page_size,
                                        data_types,
                                        key_indices,
                                        data_types_size,
                                        key_indices_size,
                                        table_dir};

    Table* newTable = Table_create(&params);

    if(newTable == NULL) {
        return -1;
    }

    // add table id to list
    int* new_ids, old_ids;

    self->num_tables += 1;
    new_ids = malloc(self->num_tables * sizeof(int));
    old_ids = self->table_ids;

    // append new page_id
    memcpy(new_ids, old_ids, (self->num_tables-1)*sizeof(int));
    new_ids[self->num_tables-1] = table_id;

    free(self->table_ids);
    self->table_ids = new_ids;

    return table_id;
}

void TM_save_meta(TableManager* self) {

}

void TM_save_tables(TableManager* self) {
    for(int t = 0; t < self->num_tables; t++) {
        Table* table;
        get_item(self->tables, self->table_ids[t], table);
        Table_save(table);
    }

    TM_save_meta(self);
}

int table_clear(int table_id)
{
    // char* table_meta_path, table_path;
    // char filename[255];
    // DIR *tableDir;
    // struct dirent *page;

    // // go to table directory
    // table_path = get_table_dir(table_id);
    // tableDir = opendir(table_path);

    
    // if(tableDir == NULL) {
    //     return -1; // error
    // }

    // // iterate through pages
    // // delete all its pages
    // while( page=readdir(tableDir) )
    // {
    //     // skip ".", "..", and table_meta_data.oo files
    //     if(!strcmp(page->d_name, ".") || !strcmp(page->d_name, "..") || !strcmp(page->d_name, TABLE_META_DATA_FILE))
    //     {
    //         continue;
    //     }
    //     strcpy(filename, table_path);
    //     strcat(filename, page->d_name);

    //     // delete each one
    //     if(remove(filename) == -1) {
    //         return -1; // error
    //     }
    // }

    // if(closedir(tableDir) == -1) {
    //     return -1;
    // }
    return 0;
}

int table_drop(int table_id)
{
    // char* table_meta_path, table_path;

    // // clear table
    // table_clear(table_id);

    // // delete meta file
    // table_path = get_table_dir(table_id);

    // strcpy(table_meta_path, table_path);
    // strcat(table_meta_path, TABLE_META_DATA_FILE);
    
    // if(remove(table_meta_path) == -1) {
    //     return -1;
    // }

    // // delete table directory
    // if(rmdir(table_path) == -1) {
    //     return -1;
    // }

    return 0;
}