#include <stdlib.h>
#include "../include/table.h"

private Page* new_page(Table* self);
private Page* new_existing_page(Table* self, int page_id);


void Table_write_meta(Table* self) {

}

void Table_read_meta(char* table_meta_path) {
    
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
        page = new_page(self, page);
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
private Page* new_page(Table* self) {
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

private Page* new_existing_page(Table* self, int page_id) {
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

bool records_equal(union record_item* item1, union record_item* item2, int data_type)
{
    switch (data_type)
    {
    case 0:
        return item1->i == item2->i;
        break;
    case 1:
        return item1->d == item2->d;
        break;
    case 2:
        return item1->b == item2->b;
        break;
    case 3:
        return strcmp(item1->c, item2->c) == 0;
        break;
    case 4:
        return strcmp(item1->v, item2->v) == 0;
        break;
    default:
        return false;
        break;
    }
}

union record_item* getRecord_with_PrimaryKey(int table_id, union record_item* primary_key)
{
    Table* table = find_table(table_id);
    union record_item*** records = table->records;

    // very rudamentary search
    for(int r = 0; r < table->num_records; r++) // iterate thru rows
    {
        union record_item* row = *records[r];
        for(int k = 0; k < table->num_keys; k++) // iterate thru keys
        {
            int key_index = table->key_indices[k];
            union record_item row_val = row[key_index];
            union record_item key_val = primary_key[k];
            int data_type = table->column_types[key_index];
            if(records_equal( &row_val, &key_val, data_type) ) // check row against given key
            {
                if(k == table->num_keys - 1) {
                    return row;
                } else {
                    continue;
                }
            } else {
                break;
            }
        }

    }
}