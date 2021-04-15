//
// Created by SGuan on 4/3/2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../include/database_util/database_helper.h"
#include "../../include/database_util/db_exec_ddl.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/file_sys/file_sys.h"
#include "../../include/storagemanager.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_printer.h"
#include "../../include/helper_module/multiline_input.h"



void db_close();

static bool db_loc_exist = false;
static struct database_params* db_params;

void get_cl_args(int argc, char* argv[]){
    /*first check if the command line is valid*/
    if(argc < 4 || argc > 4){
        fprintf(stderr, "Invalid Arguments!!!\nExpected: database <db_loc> <page_size> <buffer_size>\n");
        exit(0);
    }

    /*if the command line args are valid store the args (after verifying and converting)*/
    db_params = malloc(sizeof(struct database_params));

    //verify and store the db location path
    char* db_loc = (char*)argv[1];
    int db_loc_str_len = strlen(db_loc);
    char* db_loc_path = malloc(db_loc_str_len + 2);  //one extra character for '/', and another one for null
    db_loc_path[0] = 0;
    strncpy(db_loc_path, db_loc, db_loc_str_len + 2);

    if(dir_exist(db_loc)){
        printf("Database exist\n");
        db_loc_exist = true;
    }else{
        printf("Database does not exist! Creating a database directory...\n");
        db_loc_exist = false;
        make_dir(db_loc);
    }

    if(db_loc_path[db_loc_str_len - 1] != '/'){
        /*if there is no ending '/', add one */
        db_loc_path[db_loc_str_len] = '/';
        db_loc_path[db_loc_str_len + 1] = 0;
    }

    //verify and store the page size and buffer_size
    char* ptr;


    char* page_size_str = (char*) argv[2];
    if(is_int(page_size_str) == 0){
        /* page_size_str is not a valid number str */
        fprintf(stderr, "page_size \"%s\" is not valid!\nExpected: database <db_loc> <page_size> <buffer_size>\n", page_size_str);
        exit(0);
    }
    int page_size = strtol(page_size_str, &ptr, 10);

    char* buffer_size_str = (char*) argv[3];
    if(is_int(buffer_size_str) == 0){
        /* page_size_str is not a valid number str */
        fprintf(stderr, "buffer_size \"%s\" is not valid!\nExpected: database <db_loc> <page_size> <buffer_size>\n", buffer_size_str);
        exit(0);
    }
    int buffer_size = strtol(buffer_size_str, &ptr, 10);

    db_params->db_loc_path = db_loc_path;
    db_params->page_size = page_size;
    db_params->buffer_size = buffer_size;


}

void init_db(){
    create_database(db_params->db_loc_path, db_params->page_size, db_params->buffer_size, db_loc_exist);
    create_multiline_input();
    init_catalog(db_params->db_loc_path);
    init_ddl_exec_ddl();
}


void print_tables(){
    catalog_print_tables();
}

void free_db_params(){
    free(db_params->db_loc_path);
    free(db_params);
}

void db_close(){
    free_db_params();
    db_exec_ddl_close();
    catalog_close();
}