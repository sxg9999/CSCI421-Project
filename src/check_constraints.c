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
