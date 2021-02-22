#include <time.h>

#include "../include/table_manager.h"

#include "hashtable.h"
int TM_write_meta(TableManager* self) {
    FILE* fp;
    int rc;

    char filename[255];
    strcpy(filename, self->db_loc);
    strcat(filename, TABLE_META_DATA_FILENAME);

    fp = fopen(filename, "wb+");
    // write out length of db_loc
    int len = strlen(self->db_loc);
    rc = ec( fwrite(&len, sizeof(int), 1, fp) );
    // write out db_loc
    rc = ec( fwrite(self->db_loc, len*sizeof(char), 1, fp) );
    // write out page_size
    rc = ec( fwrite(&(self->page_size), sizeof(int), 1, fp) );
    // write out # of tables
    rc = ec( fwrite(&(self->num_tables), sizeof(int), 1, fp) );
    // write out buff size
    rc = ec( fwrite(&(self->buffer_size), sizeof(int), 1, fp));

    // write out each table
    for (int i = 0; i < self->num_tables; i++) {
        int current_table_id = self->table_ids[i];
        Table* current_table = get_void(self->tables, current_table_id);
        
        // write out current table to fp
        Table_write_meta(current_table, fp);
    }

    fclose(fp);

    if(rc == -1) {
        fprintf(stderr, "Problem writing tablemanager data.\n");
    }

    return rc;
}

buffer_manager* TM_read_meta(char* db_loc, TableManager* tm, int* rc) {
    FILE* fp;

    char filename[255];
    strcpy(filename, db_loc);
    strcat(filename, TABLE_META_DATA_FILENAME);

    fp = fopen(filename, "rb+");
    // read length of db_loc
    int db_loc_length; 
    *rc = ec( fread(&db_loc_length, sizeof(int), 1, fp) );
    
    // read page_size
    int page_size;
    *rc = ec( fread(&page_size, sizeof(int), 1, fp) );

    // read # of tables
    int table_count;
    *rc = ec( fread(&table_count, sizeof(int), 1, fp) );

    // read buff size
    int buff_size;
    *rc = ec( fread(&buff_size, sizeof(int), 1, fp) );

    // read in each table
    HashTable* tables = malloc(sizeof(HashTable));
    int* table_ids = malloc(table_count*sizeof(int));
    for (int i = 0; i < table_count; i++) {
        Table* current_table = malloc(sizeof(Table));
        // write out current table to fp
        rc = ec( Table_read_meta(current_table, fp) );
        Hash_put_int(tables, current_table->table_id, (void*) current_table);
        table_ids[i] = current_table->table_id;
    }

    tm->db_loc = db_loc;
    tm->page_size = page_size;
    tm->num_tables = table_count;
    tm->table_ids = table_ids;
    tm->tables = tables;
    tm->buffer_size = buff_size;

    fclose(fp);

    if(*rc == -1) {
        fprintf(stderr, "Error reading tablemanager data.\n");
    }

    return BufferManager_new(buff_size);
}



TableManager* init_table_manager(char* db_loc, int page_size, int buffer_size)
{
    TableManager* tm = (TableManager*) malloc(sizeof(TableManager));
    tm->db_loc = db_loc;
    tm->page_size = page_size;
    tm->tables = (HashTable*) malloc(sizeof(HashTable));
    tm->buffer_size = buffer_size;

    return tm;
}

int TM_get_table(TableManager* self, int table_id, Table* table) {
    if(get_void(self->tables, table_id) == NULL) {
        fprintf(stderr, "Table with ID %d does not exist.\n", table_id);
        return -1;
    }
    
    return 0;
}


void get_table_dir(int table_id, char* db_loc, char* dirStr)
{
    char t[255] = "/Table";
    strcpy(dirStr, db_loc);

    tostring(table_id, t+6);
    strcat(dirStr, t);
    strcat(dirStr, "/");
}


/// A disgusting id generator.
int get_rand_id(TableManager* self) {
    srand(time(NULL));
    int id;
    bool loop = true;
    while(loop) {
        loop = false;
        id = rand();

        for(int i = 0; i < self->num_tables; i++) {
            if(id == self->table_ids[i]) {
                loop = true;
                break;
            }
        }
    }
    return id;
}

int TM_add_table(TableManager* self, int* data_types, int* key_indices, int data_types_size, int key_indices_size)
{
    // NOT GUARANTEED UNIQUE ID, just very unlikely to repeat
    int table_id = get_rand_id(self);

    char* table_dir = malloc(256*sizeof(char));
    get_table_dir(table_id, self->db_loc, table_dir);

    mkdir(table_dir, S_IRWXO);
    free(table_dir);

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
    char* table_dir = malloc(256*sizeof(char));
    get_table_dir(old_table->table_id, self->db_loc, table_dir);

    mkdir(table_dir, S_IRWXO);
    free(table_dir);

    // add table id to list
    self->num_tables += 1;
    Hash_add_item(self->tables, old_table->table_id, old_table);

    return old_table->table_id;
}

int TM_save(TableManager* self) {
    return TM_write_meta(self);
}

void TM_destroy(TableManager* self) {
    for(int i = 0; i < self->num_tables; i++) {
        int id = self->table_ids[i];
        Table* table = get_void(self->tables, id);
        Table_destroy(table);
    }
    Hash_destroy(self->tables);
    free(self->db_loc);
    free(self->table_ids);
    free(self);
}

int TM_clear_table(TableManager* self, int table_id)
{
    Table* table = NULL;
    if(TM_get_table(self, table_id, table) == -1) {
        return -1;
    }
    Table_clear(table);

    return 0;
}

int TM_drop_table(TableManager* self, int table_id)
{
    Table* table = NULL;
    char* table_dir = NULL;

    if(TM_get_table(self, table_id, table) == -1) {
        return -1;
    }

    // update list of tables

    Hash_remove_item(self->tables, table_id);

    self->num_tables -= 1;

    // destroy table and its folder
    Table_destroy(table);

    table_dir = malloc(256*sizeof(char));
    get_table_dir(table_id, self->db_loc, table_dir);
    rmdir(table_dir);
    free(table_dir);

    return 0;
}