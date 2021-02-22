#include <stdlib.h>
#include <string.h>

#include "../include/table.h"

Page* new_page(Table* self);
Page* new_existing_page(Table* self, int page_id);


int Table_write_meta(Table* self, FILE* fp) {
    int rc;
    // write table id
    rc = ec( fwrite(&(self->table_id), sizeof(int), 1, fp) );

    // write # of attributes
    rc = ec( fwrite(&(self->num_columns), sizeof(int), 1, fp) );

    // write type of each attribute
    rc = ec( fwrite(self->columns, sizeof(int), self->num_columns, fp) );

    // write # of attributes in primary key
    rc = ec( fwrite(&(self->num_keys), sizeof(int), 1, fp));

    // write each attribute of primary key
    rc = ec( fwrite(self->primary_key, sizeof(int), self->num_keys, fp) );

    // write # of records in table
    rc = ec( fwrite(&(self->num_records), sizeof(int), 1, fp) );

    // write # of pages in table
    rc = ec( fwrite(&(self->num_pages), sizeof(int), 1, fp) );

    // write each page #
    rc = ec( fwrite(self->page_ids, sizeof(int), self->num_pages, fp) );

    if (rc == -1) {
        fprintf(stderr, "Problem writing table data.");
    }
    return rc;
}

int Table_read_meta(Table* newTable, FILE* fp) {
    int rc;

    // read table id
    int table_id;
    rc = ec( fread(&table_id, sizeof(int), 1, fp) );

    // read # of attributes
    int num_columns;
    rc = ec( fread(&num_columns, sizeof(int), 1, fp) );

    // read type of each attribute
    int* column_types = malloc(num_columns * sizeof(int));
    rc = ec( fread(column_types, sizeof(int), num_columns, fp) );

    // read # of attributes in primary key
    int num_keys;
    rc = ec( fread(&num_keys, sizeof(int), 1, fp));

    // read each attribute of primary key
    int* primary_key = malloc(num_keys * sizeof(int));
    rc = ec( fread(primary_key, sizeof(int), num_keys, fp) );

    // read # of records in table
    int num_records;
    rc = ec( fread(&num_records, sizeof(int), 1, fp) );

    // read # of pages in table
    int num_pages;
    rc = ec( fread(&num_pages, sizeof(int), 1, fp) );

    // read each page #
    int* page_ids = malloc(num_pages * sizeof(int));
    rc = ec( fread(page_ids, sizeof(int), num_pages, fp) );

    newTable->table_id = table_id;
    newTable->num_columns = num_columns;
    newTable->columns = column_types;
    newTable->num_keys = num_keys;
    newTable->primary_key = primary_key;
    newTable->num_records = num_records;
    newTable->num_pages = num_pages;
    newTable->page_ids = page_ids;

    if (rc == -1) {
        fprintf(stderr, "Problem reading table data.");
    }
    return rc;
}


void Table_init(Table* self, TableParams* params) {
    self->table_id = params->table_id;
    self->page_size = params->page_size;
    self->num_pages = 0;
    self->page_ids = NULL;
    self->num_columns = params->data_types_size;
    self->num_keys = params->key_indices_size;
    self->columns = params->data_types;
    self->primary_key = params->key_indices;
    self->loc = params->loc;
}

Table* Table_create(TableParams* params) {
    Table* table = malloc(sizeof(Table));
    Table_init(table, params);
    return table;
}


int Table_insert_record(Table* self, buffer_manager* bm, union record_item* record) {
    Page* page;
    int last_page_id;
    if(self->num_pages == 0) { // no pages in table
        new_page(page);
        add_page(bm, page);
        
    } else if(get_buffer_page(bm, last_page_id = self->page_ids[self->num_pages - 1], page) != 0) { // page not in buffer
        new_existing_page(page, last_page_id);
        add_page(bm, page);
    }
    // page now in buffer

    if(Page_insert_record(page, record) == -1) {
        return -1;
    }

    return 0;
}

int Table_update_record(Table* self, buffer_manager* bm, union record_item* record) {
    Page* page;
    int last_page_id;
    if(self->num_pages == 0) { // no pages in table
        page = new_page(self);
        add_page(bm, page);
        
    } else if(get_buffer_page(bm, last_page_id = self->page_ids[self->num_pages - 1], page) != 0) { // page not in buffer
        page = new_existing_page(self, last_page_id);
        add_page(bm, page);
    }
    // page now in buffer

    /// RECORD_ID =?????
    if(Page_update_record(page, 0, record) == -1) {
        return -1;
    }

    return 0;
}



/*
 *
 */
void Table_clear(Table* self) {
    char page_file[255];
    char* page_id_str;

    // delete all page files
    for(int p = 0; p < self->page_ids; p++) {
        tostring(self->page_ids[p], page_id_str);
        strcpy(page_file, self->loc);
        strcat(page_file, page_id_str);

        remove(page_file);
    }

    self->num_pages = 0;
}

void Table_destroy(Table* self) {
    Table_clear(self);

    strcat(self->loc, TABLE_META_DATA_FILENAME);
    remove(self->loc);

    free(self->page_ids);
    free(self->columns);
    free(self->primary_key);
    free(self->loc);

    free(self);
}

// returns the new page id
Page* new_page(Table* self) {
    srand(time());
    int page_id = rand();
    Page* page = new_existing_page(self, page_id);

    // add page id to list
    int* new_ids, old_ids;

    self->num_pages += 1;

    new_ids = malloc(self->num_pages * sizeof(int));
    old_ids = self->page_ids;

    // append new page_id
    memcpy(new_ids, old_ids, (self->num_pages-1)*sizeof(int));
    new_ids[self->num_pages-1] = page_id;

    free(self->page_ids);
    self->page_ids = new_ids;

    return page;
}

Page* new_existing_page(Table* self, int page_id) {
    char page_file_name[255] = "page_";
    tostring(page_id, page_file_name+5);
    strcat(page_file_name, ".oo");

    // create Page "object"
    PageParams pp = (PageParams) {  self->loc,
                                    page_file_name,
                                    page_id,
                                    self->page_size,
                                    sizeof(union record_item),
                                    self->num_columns,
                                    self->columns};

    return Page_create(&pp);    
}

void tostring(int num, char str[]) {
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}