#ifndef RECORD_META_DATA_H
#define RECORD_META_DATA_H


/*
 *A struct that holds meta informations related to a Record
 * : record_item_size, column_attributes, num_of_attributes
*/
typedef struct{
    int record_item_size;
    int num_of_attributes;
    int* column_attributes;
}RecordMeta;

#endif