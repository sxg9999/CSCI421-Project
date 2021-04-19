//
// Created by SGuan on 4/15/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/hash_table.h"
#include "../../include/db_types.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/storagemanager.h"

#include "../../include/storage_mediator/storage_mediator_printer.h"

//struct to hold metadata about a table
struct table_data{
    int table_num;
    int table_size;
    int tuples_per_page;
    int num_attr;
    int num_key_attr;
    int num_pages;
    int * attr_types;
    int * key_indices;
    int * pages;
};

void sm_print_table_childs(int num_of_childs, char** childs);
void sm_print_catalog_attr_data(struct hashtable* attr_ht);
void sm_print_catalog_constr(int constr_count, struct attr_constraint** constr);
void sm_print_catalog_p_keys(int p_key_len, char** p_key_attrs);
void sm_print_catalog_f_keys(int f_key_count, struct foreign_key_data** f_keys);
void sm_print_storage_manager_t_data(struct table_data* t_data);
void sm_print_storage_manager_attr_types(int num_attr, int* attr_types);
void sm_print_key_attrs_indices(int num_key_attr, int* key_indices);

void sm_print_all_table_meta_datas(){
    struct hashtable* table_ht = catalog_get_ht();                          //catalog's hashtable that contains table meta data
    struct table_data** table_data = storage_get_table_meta_datas();        //storage managers table meta datas

    struct ht_node** val_nodes = table_ht->node_list;
    printf("\n");
    for(int i = 0; i < table_ht->size; i++){
        struct catalog_table_data* c_t_data = val_nodes[i]->value->v_ptr;
        printf("Table:%d - %s\n", c_t_data->table_num, c_t_data->table_name);
        printf("(catalog)\n");

        sm_print_table_childs(c_t_data->num_of_childs, c_t_data->childs);
        sm_print_catalog_attr_data(c_t_data->attr_ht);
        sm_print_catalog_p_keys(c_t_data->p_key_len, c_t_data->primary_key_attrs);
        sm_print_catalog_f_keys(c_t_data->num_of_f_key, c_t_data->f_keys);
        printf("\n");
        sm_print_storage_manager_t_data(table_data[c_t_data->table_num]);
        printf("\n\n");
    }
}

void sm_print_storage_manager_t_data(struct table_data* t_data){
    printf("(storage manager)\n");
    printf("table_size = %d\n", t_data->table_size);
    printf("num_attr = %d\n", t_data->num_attr);
    sm_print_storage_manager_attr_types(t_data->num_attr, t_data->attr_types);
    printf("num_key_attr = %d\n", t_data->num_key_attr);
    sm_print_key_attrs_indices(t_data->num_key_attr, t_data->key_indices);

}

void sm_print_storage_manager_attr_types(int num_attr, int* attr_types){
    printf("attr_types = ( ");
    for(int i = 0; i < num_attr; i++){
        char* type_str = type_to_str(attr_types[i]);
        printf(" %s=%d, ",type_str, attr_types[i]);
    }
    printf(")\n");
}

void sm_print_key_attrs_indices(int num_key_attr, int* key_indices){
    printf("key_indices = ( ");
    for(int i = 0; i < num_key_attr; i++){
        printf(" %d, ", key_indices[i]);
    }
    printf(")\n");
}

void sm_print_table_childs(int num_of_childs, char** childs){

    if(num_of_childs == 0){
        printf("child_tables: <none>\n");
    }else{
        printf("child_tables: [");

        for(int i = 0; i < num_of_childs; i++){
            printf("%s, ", childs[i]);
        }

        printf("]\n");
    }
}

void sm_print_catalog_attr_data(struct hashtable* attr_ht){
    int num_of_attrs = attr_ht->size;
    struct ht_node** val_nodes = attr_ht->node_list;
    printf("attributes(%d):\n", num_of_attrs);
    for(int i = 0; i < num_of_attrs; i++){
        struct attr_data* a_data = val_nodes[i]->value->v_ptr;
        enum db_type type = a_data->type;
        char* type_str = type_to_str(a_data->type);

        if(type != CHAR && type !=VARCHAR){
            printf("- %s %s ", a_data->attr_name, type_str);
        }else{
            printf("- %s %s(%d) ", a_data->attr_name, type_str, a_data->attr_size);
        }

        sm_print_catalog_constr(a_data->num_of_constr, a_data->constr);

    }
}

void sm_print_catalog_constr(int constr_count, struct attr_constraint** constr){
    for(int i = 0; i < constr_count; i++){
        struct attr_constraint* a_constr = constr[i];
        char* constr_type_str = type_to_str(a_constr->type);

        printf("%s, ", constr_type_str);
    }
    printf("\n");
}

void sm_print_catalog_p_keys(int p_key_len, char** p_key_attrs){
    printf("primary_key(%d)=( ", p_key_len);
    for(int i = 0; i < p_key_len; i++){
        printf("%s, ", p_key_attrs[i]);
    }
    printf(")\n");
}

void sm_print_catalog_f_keys(int f_key_count, struct foreign_key_data** f_keys){

    if(f_key_count == 0){
        printf("foreign Keys: <none>\n");
    }else{
        printf("foreign Keys:\n");
        for(int i = 0; i < f_key_count; i++){
            char* referenced_table_name = f_keys[i]->parent_table_name;


            struct hashtable* f_keys_mapping = f_keys[i]->f_keys;
            int num_of_mapping = f_keys_mapping->size;
            struct ht_node** f_key_attrs = f_keys_mapping->node_list;


            printf("- (foreign_key_%d) ref_table=%s", i, referenced_table_name);

            for(int j = 0; j < num_of_mapping; j++){
                printf(", %s=%s", f_key_attrs[i]->key, (char*)(f_key_attrs[i]->value->v_ptr));
            }

            printf("\n");

        }
    }

}