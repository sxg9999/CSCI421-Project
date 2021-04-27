//
// Created by SGuan on 4/27/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/catalog/catalog.h"
#include "../include/catalog/catalog_attr_helper.h"
#include "../include/catalog/catalog_unique_constraint.h"
#include "../include/storage_mediator/storage_mediator.h"

#include "../include/check_constraints.h"


int single_unique_constraint_met(char* table_name, union record_item* record){
    char func_loc_str[] = "(check_constraint.c/single_unique_constraint_met)";

    struct attr_data** attr_data_arr = NULL;
    int num_of_attr = catalog_get_attr_data(table_name, &attr_data_arr);


    for(int i = 0; i < num_of_attr; i++){
        struct attr_data* attr_data = attr_data_arr[i];
        if(attr_data == NULL){
            printf("Error: attr_data is NULL. %s\n", func_loc_str);
            exit(0);
        }
        if(attr_has_unique(attr_data)){
            if(sm_record_value_exist(table_name, record[i], attr_data->index, attr_data->type)){

                if(attr_data_arr){
                    free(attr_data_arr);
                }

                return 0;
            }
        }
    }
    if(attr_data_arr){
        free(attr_data_arr);
    }

    return 1;
}

int group_unique_constraint_met(char* table_name, union record_item* record,
                                int** unique_attr_indices_arr, int* unique_group_size_arr, int unique_group_count){
    char func_loc_str[] = "(check_constraint.c/group_unique_constraint_met)";

    if(unique_group_count == 0){
        return 1;
    }

    if(unique_attr_indices_arr == NULL || unique_group_size_arr == NULL || unique_group_count < 0){
        printf("Error: Incorrect parameters. %s\n", func_loc_str);
        exit(0);
    }


    enum db_type* attr_types = NULL;
    int num_of_attr = catalog_get_attr_types(table_name, &attr_types);


    for(int i = 0; i < unique_group_count; i++){
        union record_item* unique_record = NULL;
        int build_unique_err = build_sub_record(table_name, record, unique_attr_indices_arr[i],
                                                   unique_group_size_arr[i], &unique_record);

        if(build_unique_err == -1 || unique_record == NULL){
            printf("Error: Cannot create an unique record. %s\n", func_loc_str);
            exit(0);
        }

        if(sm_record_values_exist(table_name, unique_record, unique_attr_indices_arr[i],
                                 attr_types, unique_group_size_arr[i])){
            free(unique_record);
            return 0;
        }

        free(unique_record);

    }

    if(attr_types){
        free(attr_types);
    }

    return 1;

}

int foreign_key_constraint_met(char* table_name, union record_item* record,
                               char** parent_names, int** foreign_key_indices_arr, int* foreign_key_lens,
                               int f_key_count){
    char func_loc_str[] = "(check_constraint.c/foreign_key_constraint_met)";

    if(f_key_count == 0){
        return 1;
    }


    if(foreign_key_indices_arr == NULL || foreign_key_lens == NULL || f_key_count < 0){
        printf("Error: Incorrect parameters. %s\n", func_loc_str);
        exit(0);
    }

    enum db_type* attr_types = NULL;
    int num_of_attr = catalog_get_attr_types(table_name, &attr_types);


    for(int i = 0; i < f_key_count; i++){
        union record_item* foreign_key = NULL;
        int build_sub_err = build_sub_record(table_name, record, foreign_key_indices_arr[i],
                                             foreign_key_lens[i], &foreign_key);

        if(build_sub_err == -1 || foreign_key == NULL){
            printf("Error: Cannot create an sub record foreign key. %s\n", func_loc_str);
            exit(0);
        }
        int table_id = catalog_get_table_num(parent_names[i]);
        if(sm_record_exist(table_id, foreign_key)){
            if(foreign_key){
                free(foreign_key);
            }

            if(attr_types){
                free(attr_types);
            }
            return 1;
        }
        if(foreign_key){
            free(foreign_key);
        }
    }

    if(attr_types){
        free(attr_types);
    }

    return 0;


}

int build_sub_record(char* table_name, union record_item* record, int* attr_indices,
                        int num_of_values, union record_item** sub_record){
    char func_loc_str[] = "(parse_insert_stmt_helpers.c/build_unique_record)";
    if(attr_indices == NULL){
        printf("Error: unique_group_attr_indices is NULL. %s\n", func_loc_str);
        exit(0);
    }
    if(record == NULL){
        printf("Error: record is NULL. %s\n", func_loc_str);
        exit(0);
    }
    if(num_of_values <= 0){
        printf("Error: number of values <= 0. %s\n", func_loc_str);
        exit(0);
    }
    enum db_type* attr_types = NULL;
    int num_of_attr = catalog_get_attr_types(table_name, &attr_types);

    *sub_record = malloc(sizeof(union record_item) * num_of_values);
    for(int i = 0; i < num_of_values; i++){
        int attr_index = attr_indices[i];
        enum db_type attr_type = attr_types[attr_index];
        switch(attr_type){
            case INT:
                (*sub_record)[i].i = record[attr_index].i;
                break;
            case DOUBLE:
                (*sub_record)[i].d = record[attr_index].d;
                break;
            case BOOL:
                (*sub_record)[i].b = record[attr_index].b;
                break;
            case CHAR:
                (*sub_record)[i].c[0] = 0;
                strncpy((*sub_record)[i].c, record[attr_index].c, strlen(record[attr_index].c) + 1);
                break;
            case VARCHAR:
                (*sub_record)[i].v[0] = 0;
                strncpy((*sub_record)[i].v, record[attr_index].v, strlen(record[attr_index].v) + 1);
                break;
            default:
                printf("Error: Attribute data type is invalid. %s\n", func_loc_str);
                exit(0);
        }
    }

}
