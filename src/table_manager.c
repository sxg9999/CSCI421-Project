#include "../include/table_manager.h"
#include "../include/table.h"

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
    FILE* fp;

    fp = fopen(TABLE_META_DATA_FILENAME, "wb+");
    // write out length of db_loc
    fprintf(fp, strlen(self->db_loc));
    // write out db_loc
    fprintf(fp, self->db_loc);
    // write out page_size
    fprintf(fp, self->page_size);
    // write out # of tables
    fprintf(fp, (self->tables)->current_size);

    // write out each table
    for (int i = 0; i < self->num_tables; i++) {
        int current_table_id = self->table_ids[i];
        Table* current_table;
        Hash_get_item(self->tables, current_table_id, current_table);
        // write out current table to fp
        Table_write_meta(current_table, fp);
    }

    fclose(fp);
}

void TM_read_meta() {
    FILE* fp;


    fp = fopen(TABLE_META_DATA_FILENAME, "rb+");
    // read length of db_loc
    int db_loc_length; 
    fscanf(fp, "%d", &db_loc_length);
    
    // read db_loc
    char db_loc[db_loc_length];
    fgets(db_loc, db_loc_length, fp);
    
    // read page_size
    int page_size;
    fscanf(fp, "%d", &page_size);

    // create table manager
    TableManager* tm = init_table_manager(db_loc, page_size);

    // read # of tables
    int table_count;
    fscanf(fp, "%d", &table_count);

    // read in each table
    for (int i = 0; i < tm->num_tables; i++) {
        int current_table_id = tm->table_ids[i];
        Table* current_table;
        Hash_get_item(tm->tables, current_table_id, current_table);
        // write out current table to fp
        Table_write_meta(current_table, fp);
    }

    fclose(fp);
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

    TableParams params;
    params.table_id = table_id;
    params.page_size = self->page_size;
    params.data_types = data_types;
    params.data_types_size = data_types_size;
    params.key_indices = key_indices;
    params.key_indices_size = key_indices_size;
    params.loc = table_dir;

    Table* newTable = Table_create(&params);

    if(newTable == NULL) {
        return -1;
    }

    // add table id to list
    self->num_tables += 1;
    Hash_add_item(self->tables, newTable->table_id, newTable);

    return table_id;
}

int TM_add_old_table(TableManager* self, Table* old_table)
{
    char* table_dir = get_table_dir(old_table->table_id, self->db_loc);

    mkdir(table_dir, S_IRWXO);

    // add table id to list
    self->num_tables += 1;
    Hash_add_item(self->tables, old_table->table_id, old_table);

    return old_table->table_id;
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
    for(int i = 0; i < self->num_tables; i++) {
        int id = self->table_ids[i];
        Table* table;
        Hash_get_item(self->tables, id, (void*) table);
        Table_destroy(table);
    }
    Hash_destroy(self->tables);
    free(self->db_loc);
    free(self->table_ids);
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