#ifndef TABLE_H
#define TABLE_H


typedef struct{
    int num_of_pages;
    char[] table_name;
    union[] primary_key;
    union*** table;
    Hash_Table* hash_table;

}Table;


Table* Table_create(union*** table, int num_of_pages);

union*** getTable();

union*  getRecord_with_PrimaryKey(union[] primary_key)

union* getRecord_with_ID(int id)



#endif