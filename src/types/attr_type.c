//
// Created by SGuan on 3/19/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/types/attr_type.h"
#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/hash_table.h"
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/helper_module/helper_function.h"

static enum attr_type t_int = INT;
static enum attr_type t_double = DOUBLE;
static enum attr_type t_bool = BOOL;
static enum attr_type t_char = CHAR;
static enum attr_type t_varchar = VARCHAR;

static struct hashtable* type_ht;

int init_attr_types(){
    type_ht = ht_create(12, 0.75);
    sv_ht_add(type_ht, "int", &t_int);
    sv_ht_add(type_ht, "double", &t_double);
    sv_ht_add(type_ht, "bool", &t_bool);
    sv_ht_add(type_ht, "char", &t_char);
    sv_ht_add(type_ht, "varchar", &t_varchar);
    return 0;
}

enum attr_type typeof_a(char* key_word){

    if(strlen(key_word) > 7){
        return NO_ATTR_TYPE;
    }

    char key_word_lower[8];
    str_lower(key_word_lower, key_word, strlen(key_word));
    key_word_lower[strlen(key_word)] = 0;

    enum attr_type* type = sv_ht_get(type_ht, key_word_lower);

    if(type == NULL){
        return NO_ATTR_TYPE;
    }
    return *type;

}

void attr_type_print(){
    struct ht_node** node_list = type_ht->node_list;
    int size = type_ht->size;

    for(int i = 0; i < size; i++){
        enum attr_type* t = node_list[i]->value->v_ptr;
        printf("%d : type_name=%s, type_val=%d\n", i, node_list[i]->key, *t);
    }
}

void attr_type_close(){
    type_ht->node_list[0]->value->v_ptr = NULL;
    destroy_sv_ht(type_ht);
}