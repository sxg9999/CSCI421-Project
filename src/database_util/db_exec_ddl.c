//
// Created by SGuan on 4/4/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/helper_module/helper_function.h"
#include "../../include/database_util/db_exec_ddl.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_parsing.h"
#include "../../include/storage_mediator/storage_mediator.h"


static enum db_type type;
//static enum db_type sub_type;
static char* keyword_str;
static char* table_name;
static char* data_str;

int kw_str_len = 10;
int t_name_len = 50;
int data_str_len = 250;


int init_ddl_exec_ddl(){
    keyword_str = malloc(kw_str_len);
    keyword_str[0] = 0;

    table_name = malloc(t_name_len);
    table_name[0] = 0;

    data_str = malloc(data_str_len);
    data_str[0] = 0;

    return 0;
}


int get_create_stmt_parts(char* str){
    char func_str[] = "(db_exec_ddl.c/get_create_stmt_parts)";
    int start_index = 0;
    int end_index = 0;

    char* ptr = strchr(str, '(');

    if(ptr == NULL){
        fprintf(stderr, "(db_exec_ddl.c/ddl_get_create_stmt_parts) Can't get the table name!\n");
        return -1;
    }

    end_index = ptr - str - 1;
    table_name[0] = 0;
    sub_cpy(&table_name, str, start_index, end_index);


    start_index = end_index + 2;        //ptr + start_index = "<attr> <attr> <others> );"
    end_index = strlen(str) - 2;   //the last char is ')' or a ' ';

    if(data_str_len < (int)(strlen(ptr) + 1)){
        data_str_len = strlen(ptr) + 1;
        data_str = realloc(data_str, data_str_len);

    }

    data_str[0] = 0;
    sub_cpy(&data_str, str, start_index, end_index);

//    printf("%s %s",func_str, "Printing data_str\n");
//    printf("...data_str = \"%s\"\n", data_str);


    start_index = 0;
    end_index = strlen(data_str) - 1;
    if(data_str[end_index] == ')'){
        data_str[end_index - 1] = 0;


    }else{
        remove_ending_spaces(data_str);
        end_index = strlen(data_str) - 2;
        data_str[end_index] = 0;
    }

    remove_leading_spaces(data_str);
    remove_ending_spaces(data_str);

//    printf("finished getting all the create parts\n");

    return 0;

}

int get_drop_stmt_parts(char* str){
    int start_index = 0;
    int end_index = strlen(str) - 2;                            //"<table_name>" (without the semicolon)

    if(t_name_len < (int)(strlen(str) + 1)){
        t_name_len = strlen(str) +  1;
        table_name = realloc(table_name, t_name_len);
    }

    table_name[0] = 0;
    sub_cpy(&table_name, str, start_index, end_index);
    remove_ending_spaces(table_name);

    printf("---------------------------------------------------\n");
    printf("(drop table) table name is : >%s<\n", table_name);

    return 0;
}

int get_ddl_stmt_parts(char* stmt){
    int result = 0;
    int start_index = 0;
    int end_index;

    char* ptr = strchr(stmt, ' ');
    end_index = ptr - stmt - 1;

    keyword_str[0] = 0;
    sub_cpy(&keyword_str, stmt, start_index, end_index);

//    printf("key word str is :>%s<\n", keyword_str);
    type = typeof_kw(keyword_str);

    ptr = ptr + 7;
    switch(type){
        case CREATE:
//            printf("in get stmt parts create\n");
            result = get_create_stmt_parts(ptr);
            break;
        case DROP:
            result = get_drop_stmt_parts(ptr);
            break;
        case ALTER:
            break;
        default:
            break;
    }

    return result;
}

int execute_ddl_statement(){
    int err;
    switch(type){
        case CREATE:
            err = execute_create_table();
            break;
        case DROP:
            err = execute_drop_table();
            break;
        default:
            break;
    }
    return err;
}

int execute_create_table(){
    char func_str[] = "(db_exec_ddl.c/execute_create_table)";
//    printf("Executing create table\n");
//    printf("table=%s, data_str=>%s<\n", table_name, data_str);
    struct catalog_table_data* t_data = catalog_get_table_data_struct(table_name, data_str);
//    printf("Got the table structs\n");

    if(t_data == NULL){
        fprintf(stderr, "(db_exec_ddl.c/execute_create_table) %s\n", "Table data struct is NULL.\n");
        return -1;
    }else{
        printf("%s %s\n",func_str, "Not NULL");
    }
    return sm_add_table(t_data);

}

int execute_drop_table(){
    return sm_drop_table(table_name);

}

int execute_alter_table(enum db_type alter_type, char* data_str);

void db_exec_ddl_close(){
    free(keyword_str);
    free(table_name);
    free(data_str);
}