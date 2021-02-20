#include "../include/table_manager.h"

#define PATH "../"

char* get_table_dir(int table_id)
{
    char t[255] = "/Table";
    char *dirStr = PATH;

    itoa(table_id, t+6, 10);
    strcat(dirStr, t);
    strcat(dirStr, "/");

    return dirStr;
}

int table_add(int* data_types, int* key_indices, int data_types_size, int key_indices_size)
{
    // // NOT GUARANTEED UNIQUE ID, just very unlikely to repeat
    // srand(time(NULL));
    // int table_id = rand();

    // Table* newTable = create_table(table_id, data_types, key_indices, data_types_size, key_indices_size);

    // if(newTable == NULL) {
    //     return -1;
    // }
}

Table* getTables()
{
    // FILE *fileptr;
    // char *buffer;
    // long filelen;

    // fileptr = fopen(TABLES_META_DATA, "rb+");
    // fseek(fileptr, 0, SEEK_END);          
    // filelen = ftell(fileptr);             
    // rewind(fileptr);                      

    // buffer = (Table *)malloc(filelen * sizeof(char));
    // fread(buffer, filelen, 1, fileptr); 

    // return buffer;
}

/*
 * Open (or create) TABLES_META_DATA, add newTable to it.
 * Create new dir for this table.
 */
int table_save(Table* newTable)
{
    // char db_loc[255] = "";
    // char newDir[255] = "/Table";
    // itoa(newTable->table_id, newDir+6, 10);
    // // strcat(newDir, db_loc);

    // FILE *fileptr = fopen(db_loc, "wb+");    
	// printf("%lu\n", fwrite(newTable, sizeof(Table), 1, fileptr) );
	// fclose(fileptr);
	// return 0;
}

Table* read_table_meta(int table_id)
{
    // char* table_meta_file = get_table_dir(table_id);
    // strcat(table_meta_file, TABLE_META_DATA_FILE);

    // FILE *fileptr = fopen(table_meta_file, "rb+");
    // TableMeta tm;
    // fread(&tm, sizeof(TableMeta), 1, fileptr);

    // fclose(fileptr);
    // return &tm;
}

int table_clear(int table_id)
{
    char* table_meta_path, table_path;
    char filename[255];
    DIR *tableDir;
    struct dirent *page;

    // go to table directory
    // delete all pages
    table_path = get_table_dir(table_id);
    tableDir = opendir(table_path);

    
    if(tableDir == NULL) {
        return -1; // error
    }

    
    // delete all its pages
    while( page=readdir(tableDir) )
    {
        // skip ".", "..", and table_meta_data.oo files
        if(!strcmp(page->d_name, ".") || !strcmp(page->d_name, "..") || !strcmp(page->d_name, TABLE_META_DATA_FILE))
        {
            continue;
        }
        strcpy(filename, table_path);
        strcat(filename, page->d_name);

        if(remove(filename) == -1) {
            return -1; // error
        }
    }

    if(closedir(tableDir) == -1) {
        return -1;
    }
}

int table_drop(int table_id)
{
    char* table_meta_path, table_path;

    // clear table
    table_clear(table_id);

    // delete meta file
    table_path = get_table_dir(table_id);

    strcpy(table_meta_path, table_path);
    strcat(table_meta_path, TABLE_META_DATA_FILE);
    
    if(remove(table_meta_path) == -1) {
        return -1;
    }

    // delete table directory
    if(rmdir(table_path) == -1) {
        return -1;
    }

    return 0;
}