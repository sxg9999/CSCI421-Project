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

int get_unique_group_constr_indices(char* table_name, int*** unique_group_attr_indices_arr,
                                    int** unique_group_size_arr){
    char func_str_loc[] = "(catalog_unique_constraint.c/get_unique_group_constr_indices)";
//    struct catalog_table_data* t_data = catalog_get_table_metadata(table_name);
//
//    int num_of_unique_group = t_data->num_of_unique;
//    if(num_of_unique_group == 0){
//        return 0;
//    }
//
//    verify_unique_group_variables(t_data);
//
//    struct hashtable* attr_ht = t_data->attr_ht;
//    char*** unique_groups = t_data->unique_attrs;
//    *unique_group_attr_indices_arr = malloc(sizeof(int*) * num_of_unique_group);
//    *unique_group_size_arr = malloc(sizeof(int) * num_of_unique_group);
//
//    for(int i = 0; i < num_of_unique_group; i++){
//        int unique_group_size = t_data->unique_group_sizes[i];
//        if(unique_group_size <= 0){
//            printf("Error: unique group size <= 0. %s\n", func_str_loc);
//            exit(0);
//        }
//        /*Add the unique group size to the array that stores a unique group size*/
//        (*unique_group_size_arr)[i] = unique_group_size;
//
//        char** unique_group = unique_groups[i];
//        if(unique_group == NULL){
//            printf("Error: unique_group_%d is NULL. %s\n", i, func_str_loc);
//            exit(0);
//        }
//
//        /*
//         * Allocate memory to an array for storing the indices of attributes in a
//         * unique group
//         */
//        int* unique_group_attr_indices = malloc(sizeof(int) * unique_group_size);
//
//        for(int j = 0; j < unique_group_size; j++){
//            char* attr_name = unique_group[j];
//            if(attr_name == NULL){
//                printf("Error: attr_%d's name is NULl for unique_group_%d. %s\n",
//                       j, i, func_str_loc);
//            }
//            struct attr_data* attr_data = sv_ht_get(attr_ht, attr_name);
//            int attr_index = attr_data->index;
//            unique_group_attr_indices[j] = attr_index;
//        }
//
//        /*Store the unique_group_indices in the array that stores it*/
//        (*unique_group_attr_indices_arr)[i] = unique_group_attr_indices;
//    }

//    return num_of_unique_group;
    return 0;



}

//int verify_unique_group_variables(struct catalog_table_data* t_data){
//    char func_str_loc[] = "(catalog_unique_constraint.c/verify_unique_group_variables)";
//    int num_of_unique_group = t_data->num_of_unique;
//    if(num_of_unique_group < 0){
//        printf("Error: num_of_unique is < 0. %s\n", func_str_loc);
//        exit(0);
//    }
//
//    char*** unique_groups = t_data->unique_attrs;
//    if(unique_groups == NULL){
//        printf("Error: Expected at least one unique attribute group but unique attribute group array is NULL. %s\n",
//               func_str_loc);
//        exit(0);
//    }
//
//    if(t_data->unique_group_sizes == NULL){
//        printf("Error: Unique group size array is NULL. %s\n", func_str_loc);
//        exit(0);
//    }
//}