//
// Created by SGuan on 4/26/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/catalog/catalog_structs.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_unique_constraint.h"
#include "../../include/hash_collection/hash_table.h"
#include "../../include/hash_collection/sv_ht.h"

int verify_unique_group_variables(struct catalog_table_data* t_data);

int get_unique_group_constr_indices(char* table_name, int*** unique_attr_indices_arr,
                                    int** unique_group_size_arr){
    char func_str_loc[] = "(catalog_unique_constraint.c/get_unique_group_constr_indices)";

    struct catalog_table_data* t_data = catalog_get_table_metadata(table_name);
//
    int unique_group_count = t_data->unique_group_count;
    if(unique_group_count == 0){
        return 0;
    }

    verify_unique_group_variables(t_data);

    struct hashtable* attr_ht = t_data->attr_ht;
    struct unique_group** unique_group_arr = t_data->unique_group_arr;
    *unique_attr_indices_arr= malloc(sizeof(int*) * unique_group_count);
    *unique_group_size_arr = malloc(sizeof(int) * unique_group_count);

    for(int i = 0; i < unique_group_count; i++){
        struct unique_group* unique_group = unique_group_arr[i];
        if(unique_group == NULL){
            printf("Error: unique_group_%d is NULL. %s\n", i, func_str_loc);
            exit(0);
        }

        int num_of_attr = unique_group->size;
        if(num_of_attr <= 0){
            printf("Error: Number of unique attributes in the group is <= 0. %s\n", func_str_loc);
            exit(0);
        }
        /*Add the unique group size to the array that stores a unique group size*/
        (*unique_group_size_arr)[i] = num_of_attr;


        char** unique_attr_names = unique_group->unique_attr_names;

        /*
         * Allocate memory to an array for storing the indices of attributes in a
         * unique group
         */
        int* unique_attr_indices = malloc(sizeof(int) * num_of_attr);

        for(int j = 0; j < num_of_attr; j++){
            char* attr_name = unique_attr_names[j];
            if(attr_name == NULL){
                printf("Error: attr_%d's name is NULl for unique_group_%d. %s\n",
                       j, i, func_str_loc);
            }
            struct attr_data* attr_data = sv_ht_get(attr_ht, attr_name);
            int attr_index = attr_data->index;
            unique_attr_indices[j] = attr_index;
        }

        /*Store the unique_group_indices in the array that stores it*/
        (*unique_attr_indices_arr)[i] = unique_attr_indices;
    }

    return unique_group_count;
}

int verify_unique_group_variables(struct catalog_table_data* t_data){
    char func_str_loc[] = "(catalog_unique_constraint.c/verify_unique_group_variables)";
    int unique_group_count = t_data->unique_group_count;
    if(unique_group_count < 0){
        printf("Error: unique_group_count is < 0. %s\n", func_str_loc);
        exit(0);
    }

    struct unique_group** unique_group_arr = t_data->unique_group_arr;
    if(unique_group_arr == NULL){
        printf("Error: Expected at least one unique group but unique group array is NULL. %s\n",
               func_str_loc);
        exit(0);
    }

}