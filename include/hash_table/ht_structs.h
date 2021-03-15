//
// Created by SGuan on 3/14/2021.
//

#ifndef HT_STRUCTS_H
#define HT_STRUCTS_H

union ht_node_value{
    int i;
    double d;
    char* c;
    char** c_2d;
    void* v_ptr;
};

struct ht_node{
    char* key;
    char* hash_code_str;
    union ht_node_value* value;
    struct ht_node* next_node;

};

struct ht_container{
    int size;
    int capacity;
    int base_multiplier;
    double load_factor;
    struct ht_node** table;
};


#endif
