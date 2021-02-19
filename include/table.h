#ifndef TABLE_H
#define TABLE_H


typedef struct{

    int num_of_pages;
    char table_name[255];
    void *** table;
    // Hash_Table* hash_table;
    
}Table;



Table* Table_create(union record_item*** table);

union record_item*** getTable();

/*
Primary key = [union, union, union...]
*/
union record_item*  getRecord_with_PrimaryKey(union record_item* primary_key);

union record_item* getRecord_with_ID(int id);



#endif