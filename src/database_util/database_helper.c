//
// Created by SGuan on 4/3/2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/storage_mediator/storage_mediator_printer.h"

#include "../../include/database_util/database_helper.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/file_sys/file_sys.h"
#include "../../include/storagemanager.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_printer.h"
#include "../../include/helper_module/multiline_input.h"



void db_close();

/**
 * Checks if the database exist
 * @param db_loc : location of the database
 * @return 1 for true and 0 for false
 */
int db_exist(char* db_loc);

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

    if(db_exist(db_loc)){
        db_loc_exist = true;
    }else{
        printf("Database does not exist\n");
        db_loc_exist = false;
        if(!dir_exist(db_loc)){
            make_dir(db_loc);
            printf("Created a new database directory \"%s\"\n", db_loc);
        }
        printf("Database directory \"%s\" already exist\n");
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
}

int db_exist(char* db_loc){
    char catalog_file[] = "catalog.data";
    char storage_file[] = "metadata.data";

    if(dir_exist(db_loc)){
        printf("Database directory exist\n");
        printf("Checking database contents\n");


        int catalog_loc_str_size = strlen(db_loc) + strlen(catalog_file) + 2;
        char* catalog_loc = malloc(catalog_loc_str_size);
        snprintf(catalog_loc, catalog_loc_str_size, "%s/%s", db_loc, catalog_file);

        int storage_loc_str_size = strlen(db_loc) + strlen(storage_file) + 2;
        char* storage_loc = malloc(storage_loc_str_size);
        snprintf(storage_loc, storage_loc_str_size, "%s/%s", db_loc, storage_file);

        printf("...Finding \"catalog.data\"");
        if(file_exist(catalog_loc)){
            printf("...Found \"catalog.data\"\n");
        }else{
            printf("...Cannot find \"catalog.data\"\n");
            return 0;
        }

        printf("...Finding \"metadata.data\"");
        if(file_exist(storage_loc)){
            printf("...Found \"metadata.data\"\n");
        }else{
            printf("...Cannot find \"metadata.data\"\n");
            return 0;
        }

        return 1;
    }

    printf("Database does not exist.\n");
    return 0;

}

void print_tables(){
//    catalog_print_tables();
    sm_print_all_table_meta_datas();
}

void free_db_params(){
    free(db_params->db_loc_path);
    free(db_params);
}

void db_close(){
    free_db_params();
    catalog_close();
}