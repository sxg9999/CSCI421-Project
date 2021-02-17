#ifndef PAGE_H
#define PAGE_H


typedef struct{
    int id;
    int num_of_records;
    int max_size;
    int current_size;
    union record_item** records;

}Page;


Page* Page_create(union record_item** records);

#endif