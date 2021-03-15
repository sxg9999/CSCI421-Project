//
// Created by SGuan on 3/14/2021.
//

#ifndef HT_STR_INT_H
#define HT_STR_INT_H

#include "ht_structs.h"



struct ht_str_int;

struct ht_str_int{
    int (*add)(struct ht_container*, char*, int);
    int (*get)(struct ht_containert*, char*);
    int (*remove)(struct ht_container*, char*);
    int (*contains)(struct ht_container*, char*);
    int (*resize)(struct ht_node**, int*, int, int, double);
    int (*close)(struct ht_container*);
    void (*print)(struct ht_container*);
    void (*print_test)();
};

struct ht_str_int* get_ht_str_int();


#endif
