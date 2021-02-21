#include "../include/table_manager.h"

///
/// PLACEHOLDERS
int Hash_get_item(HashTable* self, int key, void* item) {
    return 0;
}

int Hash_remove_item(HashTable* self, int key) {
    return 0;
}

int Hash_add_item(HashTable* self, int key, void* item) {
    return 0;
}
///
///




void TM_write_meta(TableManager* self) {

}

void TM_read_meta(char* tm_meta_path) {

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
    self->num_tables += 1;
    Hash_add_item(self->tables, newTable->table_id, newTable);

    return table_id;
}

void TM_save_meta(TableManager* self) {
    FILE* fp;
    char filepath[255] = NULL;
    strcpy(filepath, self->db_loc);
    strcat(filepath, TM_META_FILE);

    fp = fopen(filepath, "wb+");

    fwrite(self, sizeof(TableManager), 1, fp);
    fclose(fp);

    TM_destroy(self);
}

void TM_save_tables(TableManager* self) {
    Table** tables;
    get_all_items(self->tables, tables);
    for(int t = 0; t < self->num_tables; t++) {
        Table* table = tables[t];
        Table_save_meta(table);
    }

    TM_save_meta(self);
}

void TM_destroy(TableManager* self) {
    Hash_destroy(self->tables);
    free(self);
}

int TM_clear_table(TableManager* self, int table_id)
{
    Table* table;
    if(TM_get_table(self, table_id, table) == -1) {
        return -1;
    }
    Table_clear(table);

    return 0;
}

int TM_drop_table(TableManager* self, int table_id)
{
    char* table_meta_path, table_path;
    Table* table;
    if(TM_get_table(self, table_id, table) == -1) {
        return -1;
    }

    // update list of tables

    Hash_remove_item(self->tables, table_id);

    self->num_tables -= 1;

    // destroy table and its folder
    Table_destroy(table);

    char* tableDir = get_table_dir(table_id, self->db_loc);
    rmdir(tableDir);

    return 0;
}