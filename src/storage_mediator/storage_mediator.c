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
#include "../../include/stringify_record.h"


int sm_add_table(struct catalog_table_data* t_data){
    char func_str[] = "(storage_mediator.c/sm_add_table)";

    if(catalog_table_data_is_valid(t_data)){
        printf("Table meta data struct is valid\n");
    }else{
        printf("Table meta data is invalid\n");
        exit(0);
    }

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

int sm_insert_records(char* table_name, union record_item** records, int num_of_records){
    char func_loc_str[] = "(storage_mediator.c/sm_insert_records)";

    if(records == NULL || num_of_records <= 0){
        printf("Error: Invalid number of records to insert. %s", func_loc_str);
        exit(0);
    }

    int table_id = catalog_get_table_num(table_name);
    int insert_error = 0;

    for(int record_index = 0; record_index < num_of_records; record_index++){
        printf("Inserting record_%d. %s\n", record_index, func_loc_str);
        insert_error = insert_record(table_id, records[record_index]);
        if(insert_error == -1){
            printf("Error: Cannot insert record_%d. %s\n", record_index,  func_loc_str);
            return -1;
        }
    }
    return 0;
}

int sm_record_exist(int table_id, union record_item* key_values){
    char func_loc_str[] = "(storage_mediator.c/sm_record_exist)";
    int rec_exist = 0;      //1 for true and 0 for false
    union record_item* record = NULL;

    printf("%s %s\n",func_loc_str, "Finding a record with the given key");
    int get_record_err = get_record(table_id, key_values, &record);

    if(record != NULL){
        free(record);
    }

    if(get_record_err == -1){
        printf("%s %s\n",func_loc_str, "Error: Cannot find a record with the given key");
    }else if(get_record_err == 0){
        //successfully found a record with the given key
        printf("%s %s\n",func_loc_str, "Found record with the given key");
        rec_exist = 1;
    }

    return rec_exist;
}