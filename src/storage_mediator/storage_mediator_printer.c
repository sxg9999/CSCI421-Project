//
// Created by SGuan on 4/15/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/hash_table.h"
#include "../../include/db_types.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/storagemanager.h"
#include "../../include/stringify_record.h"

#include "../../include/storage_mediator/storage_mediator_printer.h"
#include "../../include/helper_module/helper_function.h"


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
void sm_print_catalog_unique_groups(int unique_group_count, struct unique_group** unique_group_arr);
int sm_print_storage_manager_t_data(struct table_data* t_data);
void sm_print_storage_manager_attr_types(int num_attr, int* attr_types);
void sm_print_key_attrs_indices(int num_key_attr, int* key_indices);

/**
 * Verifies that the catalog table meta data have a counter part in storagemanager
 */
int sm_verify_tables(struct hashtable* table_ht, struct table_data** storagemanager_t_data, int storagemanager_t_data_size){
    char func_str[] = "(storage_mediator_printer.c/sm_verify_tables)";
    printf("%s %s\n", func_str, "Verifying catalog's table metadata and storagemanager's metadata");
    printf("%s %s %d\n", func_str, "Catalog's table metadata size:", table_ht->size);
    printf("%s %s %d\n", func_str, "Storagemanager's table metadata size:", storagemanager_t_data_size);

    if(table_ht->size != storagemanager_t_data_size){
        printf("%s %s\n", func_str, "Catalog's table metadata size does not match Storagemanager's table metadata size");
        return -1;
    }

    printf("%s %s\n", func_str, "Printing storagemanager's table metadata's content");

    for(int i = 0; i < storagemanager_t_data_size; i++){
        printf("...Table: %d, num_arr = %d\n", storagemanager_t_data[i]->table_num, storagemanager_t_data[i]->num_attr);
    }

    return 0;
}

void sm_print_all_table_meta_datas(){
    char func_str[] = "(storage_mediator_printer.c/sm_print_all_table_meta_datas)";
    struct hashtable* table_ht = catalog_get_ht();                          //catalog's hashtable that contains table meta data
    struct table_data** table_data = storage_get_table_meta_datas();        //storage managers table meta datas
    int storagemanager_t_data_size = storage_get_num_of_tables();

    int verification_err = sm_verify_tables(table_ht, table_data, storagemanager_t_data_size);

    if(verification_err == -1){
        printf("%s %s\n", func_str, "Table metadata verification failed");
        return;
    }

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
        sm_print_catalog_unique_groups(c_t_data->unique_group_count, c_t_data->unique_group_arr);
//        printf("\n");
//        printf("Table num is : %d\n", c_t_data->table_num);

        int error_code = sm_print_storage_manager_t_data(table_data[c_t_data->table_num]);

        if(error_code == -1){
            printf("%s %s \"%s\"\n", func_str, "Cannot print storage managers table data:", c_t_data->table_name);
        }


        printf("\n\n");
    }
}

int sm_print_storage_manager_t_data(struct table_data* t_data){

    if(t_data == NULL){
        printf("(storage_mediator_printer.c/storage_manager_t_data) %s\n",
                "t_data is NULL!!!");
        return -1;
    }

    printf("(storage manager)\n");
    printf("table_size = %d\n", t_data->table_size);
    printf("num_attr = %d\n", t_data->num_attr);
    sm_print_storage_manager_attr_types(t_data->num_attr, t_data->attr_types);
    printf("num_key_attr = %d\n", t_data->num_key_attr);
    sm_print_key_attrs_indices(t_data->num_key_attr, t_data->key_indices);

    return 0;
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

//    sv_ht_print(attr_ht);

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
        printf("\n");
    }
}

void sm_print_catalog_constr(int constr_count, struct attr_constraint** constr){

    for(int i = 0; i < constr_count; i++){
        struct attr_constraint* a_constr = constr[i];
        char* constr_type_str = type_to_str(a_constr->type);

        printf("%s, ", constr_type_str);
    }

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


            printf("- (foreign_key_%d) ref_table=%s : ( ", i, referenced_table_name);

            for(int j = 0; j < num_of_mapping; j++){
                printf("%s=%s, ", f_key_attrs[j]->key, (char*)(f_key_attrs[j]->value->v_ptr));
            }

            printf(" )\n");

        }
    }

}

void sm_print_catalog_unique_groups(int unique_group_count, struct unique_group** unique_group_arr){
    if(unique_group_count == 0){
        printf("unique group: <none>\n");
    }else{
        printf("unique group: %d\n", unique_group_count);
        for(int i = 0; i < unique_group_count; i++){
            struct unique_group* unique_group = unique_group_arr[i] ;
            char** unique_attrs_names = unique_group->unique_attr_names;

            int num_of_unique_attr = unique_group->size;
            char* result_str = str_concat(unique_attrs_names, num_of_unique_attr, ',');
            char* buffer = malloc(strlen(result_str) + 50);
            buffer[0] = 0;

            sprintf(buffer, "- unique_%d( %s )", i, result_str);
            printf("%s\n", buffer);
            free(buffer);
            free(result_str);
        }
    }
}



void sm_print_table_records(char* table_name){
    int table_id = catalog_get_table_num(table_name);
    union record_item** records = NULL;

    int num_of_records = get_records(table_id, &records);
    printf("There is a total of %d records\n", num_of_records);

    struct attr_data** attr_data_arr = NULL;
    int num_of_attributes = catalog_get_attr_data(table_name, &attr_data_arr);

    char* table_header_str = catalog_get_table_header(table_name);
    printf("%s\n", table_header_str);

    if(num_of_records == 0){
        printf("( <no record> )\n");
    }else{
        char** record_str_arr = record_to_str_n(attr_data_arr, records, num_of_attributes, num_of_records);
        for(int i = 0; i < num_of_records; i++){
            printf("%d : %s\n",i, record_str_arr[i]);
        }
    }

}
