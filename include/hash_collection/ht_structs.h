//
// Created by SGuan on 3/17/2021.
//

#ifndef HT_STRUCTS_H
#define HT_STRUCTS_H

union ht_node_value{
    int i;
    double d;
    void* v_ptr;
};

struct ht_node;

struct ht_node{
    int table_index;
    int list_index;
    char* key;
    unsigned long long hash_code;
    union ht_node_value* value;
    struct ht_node* next_node;

};

struct hashtable{
    int size;
    int capacity;
    int base_multiplier;
    double load_factor;
    struct ht_node** table;
    struct ht_node** node_list;
};

#endif //IMPLEMENTATION_1_HT_STRUCTS_H
