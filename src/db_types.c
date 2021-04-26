//
// Created by SGuan on 3/20/2021.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/db_types.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/hash_collection/sv_ht.h"
#include "../include/helper_module/helper_function.h"


struct pair{
    char type_name[15];
    enum db_type type;
};


static struct pair type_pairs[] = {
        {.type_name = "notnull", .type = NOT_NULL},
        {.type_name = "unique", .type = UNIQUE},
        {.type_name = "primarykey", .type = PRIMARY_KEY},
        {.type_name = "foreignkey", .type = FOREIGN_KEY},
        {.type_name = "default", .type = DEFAULT},
        {.type_name = "integer", .type = INT},
        {.type_name = "double", .type = DOUBLE},
        {.type_name = "boolean", .type = BOOL},
        {.type_name = "char", .type = CHAR},
        {.type_name = "varchar", .type = VARCHAR},
        {.type_name = "create", .type = CREATE},
        {.type_name = "drop", .type = DROP},
        {.type_name = "add", .type = ADD},
        {.type_name = "alter", .type = ALTER},
        {.type_name = "insert", .type = INSERT},
        {.type_name = "update", .type = UPDATE},
        {.type_name = "delete", .type = DELETE},
        {.type_name = "select", .type = SELECT}
};


static int num_of_types = 18;


enum db_type typeof_kw(char* key_word){

    char* kw_lower = malloc(strlen(key_word) + 1);
    str_lower(kw_lower, key_word, strlen(key_word));
    kw_lower[strlen(key_word)] = 0;

    for(int i = 0; i < num_of_types; i++){
        char* t_name = type_pairs[i].type_name;
        if(strncmp(t_name, kw_lower, strlen(kw_lower)) == 0){
            return type_pairs[i].type;
        }
    }

    return UNKNOWN;
}

enum db_type typeof_s(char* statement){
    char* ptr = strchr(statement, ' ');
    int end_index = ptr - statement - 1;
    char* key_word = substring(statement,0,end_index);

    enum db_type type = typeof_kw(key_word);
    int type_val = (int)type;
    if(type_val >= 20 && type_val < 30){
        return DDL;
    }else if(type_val >= 30 && type_val <= 40){
        return DML;
    }


    return UNKNOWN;
}

enum db_type typeof_q(char* statement){
    char* ptr = strchr(statement, ' ');

    if(ptr == NULL){
        fprintf(stderr, "(db_types.c/typeof_q) Not a valid statement!!!\n");
        exit(0);
    }

    int end_index = ptr - statement - 1;
    char* key_word = substring(statement,0,end_index);

    enum db_type type = typeof_kw(key_word);

    if(type == UNKNOWN){
        return UNKNOWN;
    }

    int type_val = (int)type;
    if(type_val >= 40 && type_val < 50){
        return QUERY;
    }

    return NON_QUERY;
}

int is_attr(char* key_word){
    enum db_type type = typeof_kw(key_word);
    return type_is_attr(type);
}

int is_constr(char* key_word){
    enum db_type type = typeof_kw(key_word);
    return type_is_constr(type);
}


int type_is_attr(enum db_type type){
    int t_val = (int)type;
    if(t_val >= 10 && t_val < 20){
        return 1;
    }
    return 0;
}

int type_is_constr(enum db_type type){
    int t_val = (int)type;
    if(t_val >= 1 && t_val < 10){
        return 1;
    }
    return 0;
}

char* type_to_str(enum db_type type){
    for(int i = 0; i < num_of_types; i++){
        if(type_pairs[i].type == type){
            return (char*) (&(type_pairs[i].type_name));
        }
    }
    return NULL;
}

void db_type_print_all(){
    for(int i = 0; i < num_of_types; i++){
        printf("type_name=%s, type_val=%d\n", type_pairs[i].type_name, type_pairs[i].type);
    }
}











