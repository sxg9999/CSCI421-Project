//
// Created by SGuan on 3/11/2021.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/types/statement_type.h"
#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/hash_table.h"
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/helper_module/helper_function.h"


static enum stmt_type t_create = CREATE;
static enum stmt_type t_drop = DROP;
static enum stmt_type t_alter = ALTER;
static enum stmt_type t_insert = INSERT;
static enum stmt_type t_update = UPDATE;
static enum stmt_type t_delete = DELETE;
static enum stmt_type t_select = SELECT;

static struct hashtable* type_ht;


int init_statement_types(){
    type_ht = ht_create(15, 0.75);
    sv_ht_add(type_ht, "create", &t_create);
    sv_ht_add(type_ht, "drop", &t_drop);
    sv_ht_add(type_ht, "alter", &t_alter);
    sv_ht_add(type_ht, "insert", &t_insert);
    sv_ht_add(type_ht, "update", &t_update);
    sv_ht_add(type_ht, "delete", &t_delete);
    sv_ht_add(type_ht, "select", &t_select);
    return 0;
}

/**
 * gets the keyword of the statement and return it lowercased
 * @param statement
 * @return
 */
char* get_key_word(char* statement){
    char* key_word;
    char* ptr = strchr(statement, ' ');
    if(ptr==NULL){
        key_word = malloc(strlen(statement)+1);
        str_lower(key_word, statement, strlen(statement));
        key_word[strlen(statement)] = 0;
    }else{
        int end_index = ptr - statement - 1;
        key_word = substring(statement, 0, end_index);
        str_lower(key_word, key_word, strlen(key_word));
    }
    return key_word;
}

enum stmt_type typeof_s(char* statement){

    char* key_word = get_key_word(statement);
    printf("key word is : %s\n", key_word);
    enum stmt_type* type_val = sv_ht_get(type_ht, key_word);
    free(key_word);

    if(type_val==NULL){
        //have to deal with NULLs
        return NO_STMT_TYPE;
    }
    return *type_val;

}

int is_ddl(char* statement){
    char* key_word = get_key_word(statement);
    enum stmt_type* type = sv_ht_get(type_ht, key_word);
    free(key_word);

    if(type!=NULL && *type < 4){
        return 1;
    }
    return 0;
}

int is_dml(char* statement){
    char* key_word = get_key_word(statement);
    enum stmt_type* type = sv_ht_get(type_ht, key_word);
    free(key_word);
    if(type!=NULL && *type >= 4){
        return 1;
    }
    return 0;
}


int is_query(char* statement){
    char* key_word = get_key_word(statement);
    enum stmt_type* type = sv_ht_get(type_ht, key_word);
    free(key_word);
    if(type!=NULL && *type >= 10){
        return 1;
    }
    return 0;
}


int is_non_query(char* statement){
    char* key_word = get_key_word(statement);
    enum stmt_type* type = sv_ht_get(type_ht, key_word);
    free(key_word);
    if(type!=NULL && *type < 10){
        return 1;
    }

    return 0;
}

void statement_print_table_content(){

    struct ht_node** node_list = type_ht->node_list;
    int size = type_ht->size;

    for(int i = 0; i < size; i++){
        enum stmt_type* t = node_list[i]->value->v_ptr;
        printf("%d : type_name=%s, type_val=%d\n", i, node_list[i]->key, *t);
    }
}

void statement_type_close(){
    type_ht->node_list[0]->value->v_ptr = NULL;
    destroy_sv_ht(type_ht);
}



