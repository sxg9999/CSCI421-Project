//
// Created by SGuan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/catalog/catalog_structs.h"
#include "../../include/catalog/catalog.h"
#include "../../include/storagemanager.h"

#include "../../include/storage_mediator/storage_mediator.h"
#include "../../include/storage_mediator/storage_mediator_helper.h"


int sm_add_table(struct catalog_table_data* t_data){
    char func_str[] = "(storage_mediator.c/sm_add_table)";

    /* check if the table already exist */
    if(catalog_contains(t_data->table_name)){
        printf("%s %s \"%s\" %s\n", func_str, "Table", t_data->table_name, "already exist.");
        printf("%s %s \"%s\"\n", func_str, "Cannot add table", t_data->table_name);
        return -1;
    }

    /* Get attribute data types  */
    int* data_types = NULL;
    int data_type_size = catalog_get_data_types(t_data, &data_types);

    if(data_types == NULL){
        fprintf(stderr, "%s %s\n", "(storage_mediator.c/sm_add_table)",
                "Cannot obtain data_type int vals.");
        return -1;
    }else{
        printf("%s %s\n", func_str, "Obtained data_type int vals.");
    }

    /* Get primary key indices */
    int* p_key_indices = NULL;
    int p_key_size = catalog_get_p_key_indices(t_data, &p_key_indices);\


    if(p_key_size == -1){
        printf("%s %s\n", "(storage_mediator.c/sm_add_table)",
               "p_key_indices is NULL");
        return -1;
    }else if(p_key_size >= 0 && p_key_indices != NULL){
        printf("%s %s\n", func_str, "Obtained p_key_indices.");
    }

    /* Add the table to storage manager */
    int table_num = add_table(data_types, p_key_indices, data_type_size, p_key_size);
    t_data->table_num = table_num;

    /* Add the table to catalog */
    int err_code = catalog_add_table(t_data);
    if(err_code == -1){
        printf("%s %s table = %s\n","(storage_mediator.c/sm_add_table)",
                "Cannot add table to table_ht.", t_data->table_name);

        /* Roll back changes to storagemanager */
        printf("%s %s\n", func_str, "Reverting changes to storagemanager.");
        drop_table(table_num);
        printf("%s %s\n", func_str, "Reverted changes to storagemanager.");
        return -1;
    }else{
        printf("%s %s \"%s\"\n", func_str, "Successfully added table", t_data->table_name);
    }


    return 0;
}

int sm_drop_table(char* table_name){
    char func_str[] = "(storage_mediator.c/sm_drop_table)";

    if(catalog_contains(table_name) == 0){
        printf("%s %s \"%s\".\n", func_str, "Unexpected Error: Cannot find table", table_name);
        printf("%s %s \"%s\".\n", func_str, "Cannot drop table", table_name);
        return -1;
    }


    int table_id = catalog_get_table_num(table_name);
    printf("%s %s %d\n", func_str, "Obtained table_id:", table_id);

    int catalog_remove_err = catalog_remove_table(table_name);
    if(catalog_remove_err == -1){
        printf("%s %s \"%s\" %s.\n", func_str, "Cannot remove table", table_name, "from catalog" );
        printf("%s %s \"%s\".\n", func_str, "Cannot drop table", table_name);
        return -1;
    }

    drop_table(table_id);

    storage_print_t_datas();
    return 0;
}

int sm_alter_table();
