//
// Created by SGuan on 4/24/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/catalog/catalog_foreign_key_helper.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/hash_collection/ht_structs.h"


int table_has_foreign_key(char* table_name){
    struct catalog_table_data* t_data = catalog_get_table_metadata(table_name);
    if(t_data->num_of_f_key > 0){
        return 1;
    }
    return 0;
}

int get_foreign_key(char* table_name, struct foreign_key_data* foreign_key_data, struct attr_data*** foreign_key,
        struct attr_data*** parent_p_key, int* f_attr_count){
    *f_attr_count = foreign_key_data->f_keys->size;
    *foreign_key = malloc(sizeof(struct attr_data*) * foreign_key_data->f_keys->size);
    *parent_p_key = malloc(sizeof(struct attr_data*) * foreign_key_data->f_keys->size);

    struct catalog_table_data* child_table_data = catalog_get_table_metadata(table_name);
    struct catalog_table_data* parent_table_data = catalog_get_table_metadata(foreign_key_data->parent_table_name);

    struct hashtable* child_attr_ht = child_table_data->attr_ht;
    struct hashtable* parent_attr_ht = parent_table_data->attr_ht;

    struct ht_node** val_nodes = foreign_key_data->f_keys->node_list;
    for(int i = 0; i < foreign_key_data->f_keys->size; i++){
        char* foreign_attr_name = val_nodes[i]->key;
        char* parent_attr_name = val_nodes[i]->value->v_ptr;

        struct attr_data* f_attr_data = sv_ht_get(child_attr_ht, foreign_attr_name);
        (*foreign_key)[i] = f_attr_data;

        struct attr_data* p_attr_data = sv_ht_get(parent_attr_ht, parent_attr_name);
        (*parent_p_key)[i] = p_attr_data;
    }

    return 0;
}

int get_all_foreign_key(char* table_name, struct attr_data**** foreign_keys, struct attr_data**** parent_p_keys,
        int* key_count, int** key_attr_count){
    struct catalog_table_data* t_data = catalog_get_table_metadata(table_name);

    if(t_data->num_of_f_key == 0){
        return -1;
    }

    if(t_data->num_of_f_key < 0){
        printf("Error: Cannot have a negative amount of foreign key. (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
        exit(0);
    }

    *key_count = t_data->num_of_f_key;
    *key_attr_count = malloc(sizeof(int) * t_data->num_of_f_key);
    *foreign_keys = malloc(sizeof(struct attr_data**) * t_data->num_of_f_key);
    *parent_p_keys = malloc(sizeof(struct attr_data**) * t_data->num_of_f_key);

    for(int i = 0; i < t_data->num_of_f_key; i++){
        struct foreign_key_data* foreign_key_data = t_data->f_keys[i];
        if(foreign_key_data == NULL){
            printf("Error: Cannot have a null foreign key data struct. (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
            exit(0);
        }
        verify_foreign_key_data_struct(foreign_key_data);
        int num_of_attr = 0;
        struct attr_data** foreign_key = NULL;
        struct attr_data** parent_p_key = NULL;
        get_foreign_key(table_name, foreign_key_data, &foreign_key, &parent_p_key, &num_of_attr);

        if(num_of_attr == 0){
            printf("Error: num_of_attr is 0! (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
            exit(0);
        }

        (*key_attr_count)[i] = num_of_attr;
        (*foreign_keys)[i] = foreign_key;
        (*parent_p_keys)[i] = parent_p_key;
    }

}

void verify_foreign_key_data_struct(struct foreign_key_data* f_key_data){
    if(f_key_data == NULL){
        printf("Error: Cannot have a null foreign key data struct. (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
        exit(0);
    }

    if(f_key_data->parent_table_name == NULL){
        printf("Error: Cannot have a null parent table name for foreign key data struct. (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
        exit(0);
    }

    if(catalog_contains(f_key_data->parent_table_name) != 1){
        printf("Error: Parent table does not exist (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
        exit(0);
    }

    if(f_key_data->f_keys == NULL){
        printf("Error: Foreign key attribute hashtable cannot be NULL. (catalog_foreign_key_helper.c/get_all_foreign_key)\n");
        exit(0);
    }
}