#include <stdlib.h>
#include <string.h>

#include "../include/table.h"
#include "../include/page.h"

#define ce(X) (((X) < 0) ? (X) : 0)

Page* new_page(Table* self);
Page* new_existing_page(Table* self, int page_id);


int Table_write_meta(Table* self, FILE* fp) {
    int rc;
    // write table id
    rc = ce( fprintf(fp, self->table_id) );
    // write # of attributes
    rc = ce( fprintf(fp, self->num_columns) );
    // write type of each attribute
    for(int a = 0; a < self->num_columns; a++) {
        rc = ce( fprintf(fp, self->column_types[a]) );
    }
    // write # of attributes in primary key
    rc = ce( fprintf(fp, self->num_keys));
    // write each attribute of primary key
    for(int k = 0; k < self->num_keys; k++) {
        rc = ce( fprintf(fp, self->key_indices[a]) );
    }
    // write # of records in table
    rc = ce( fprintf(fp, self->num_records) );
    // write # of pages in table
    rc = ce( fprintf(fp, self->num_pages) );
    // write each page #
    for(int p = 0; p < self->num_pages; p++) {
        rc = ce( fprintf(fp, self->page_ids[p]) );
    }

    if (rc == -1) {
        frpintf(stderr, "Problem writing table data.");
    }
    return rc;
}

int Table_read_meta(Table* newTable, FILE* fp) {
    int rc;

    // read table id
    int table_id;
    rc = ce( fscanf(fp, &table_id) );

    // read # of attributes
    int num_columns;
    rc = ce( fscanf(fp, &num_columns) );

    // read type of each attribute
    int* column_types = malloc(num_columns * sizeof(int));
    for(int a = 0; a < num_columns; a++) {
        rc = ce( fscanf(fp, column_types + a*sizeof(int)) );
    }

    // read # of attributes in primary key
    int num_keys;
    rc = ce( fscanf(fp, &num_keys));

    // read each attribute of primary key
    int* key_indicess = malloc(num_keys * sizeof(int));
    for(int k = 0; k < num_keys; k++) {
        rc = ce( fscanf(fp, key_indices + k*sizeof(int)) );
    }

    // read # of records in table
    int num_records;
    rc = ce( fscanf(fp, &num_records) );

    // read # of pages in table
    int num_pages;
    rc = ce( fscanf(fp, &num_pages) );

    // read each page #
    int* page_ids = malloc(num_pages * sizeof(int));
    for(int p = 0; p < &num_pages; p++) {
        rc = ce( fscanf(fp, page_ids + p*sizeof(int)) );
    }

    newTable->table_id = table_id;
    newTable->num_columns = num_columns;
    newTable->column_types = column_types;
    newTable->num_keys = num_keys;
    newTable->key_indices = key_indices;
    newTable->num_records = num_records;
    newTable->num_pages = num_pages;
    newTable->page_ids = pages_ids;

    if (rc == -1) {
        frpintf(stderr, "Problem reading table data.");
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
        itoa(page_id_str, self->page_ids[p], 10);
        strcpy(page_file, self->loc);
        strcat(page_file, page_id_str);

        remove(page_file);
    }

    self->num_pages = 0;
}

void Table_destroy(Table* self) {
    table_clear(self);

    strcat(self->loc, TABLE_META);
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
    srand(time(NULL));
    int page_id = rand();

    char page_file_name[255] = 'page_';
    itoa(page_id, page_file_name+6, 10);
    strcat(page_file_name, ".oo");

    // create Page "object"
    PageParams pp = (PageParams) {  self->loc,
                                    page_file_name,
                                    page_id,
                                    self->page_size,
                                    sizeof(record_item),
                                    self->num_columns,
                                    self->columns};

    return Page_create(pp);    
}