#include <stdlib.h>
#include "../include/table.h"

/*
 * Filler function.
 */
Table* find_table(int table_id)
{
    return NULL;
}

Table* create_table(int table_id, int* data_types, int* key_indices, int num_cols, int num_keys)
{
    Table* newTable = malloc(sizeof(Table));
    *newTable = (Table) {table_id,
                        NULL,
                        0,
                        data_types,
                        key_indices,
                        num_cols,
                        num_keys,
                        0};

    return newTable;

}

union record_item*** getTable(int table_id)
{
    return find_table(table_id)->records;
}

/*
Primary key = [union, union, union...]
*/

/*
 * Find out if record items are equal.
 */
int records_equal(union record_item* item1, union record_item* item2, int data_type)
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