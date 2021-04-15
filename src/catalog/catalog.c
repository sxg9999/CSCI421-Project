/**
 * Note: Case does not matter
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "../../include/catalog/catalog_io.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/db_types.h"
#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/si_ht.h"
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/storagemanager.h"
#include "../../include/file_sys/file_sys.h"

#include "hash_collection.h"




static char catalog_file_name[] = "catalog.data";
static char* catalog_loc;

static struct hashtable* table_ht;


int init_catalog(char* db_path){
    //check if catalog_exist then call init_mapping
    //catalog.data
    int length = strlen(db_path)+strlen(catalog_file_name);
    catalog_loc = malloc(sizeof(char)*length+1);
    snprintf(catalog_loc, length + 1, "%s%s", db_path, catalog_file_name);

    if(file_exist(catalog_loc)){
        //catalog exist
        printf("catalog exist\n");
        read_catalog(catalog_loc, &table_ht);
    }else{
        //catalog does not exist
        printf("Catalog does not exist : %s\n", catalog_loc);
        table_ht = ht_create(12, 0.75);
    }

    return 0;
}

int catalog_add_table(struct catalog_table_data* t_data){

    sv_ht_add(table_ht, t_data->table_name, t_data);

    return 0;

}

int catalog_remove_table(char* table_name){

    int table_exist = sv_ht_contains(table_ht, table_name);
    if(table_exist==0){
        fprintf(stderr, "(catalog.c/catalog_remove_table) Table does not exist or it has been removed!!!\n");
        return -1;
    }

//    struct catalog_table_data* t_data = sv_ht_remove(table_ht, table_name);
//    drop_table(t_data->table_num);
    //handing the removal of foreign keys will be done.
    return 0;
}

int catalog_contains(char* table_name){
    int exist = sv_ht_contains(table_ht, table_name);
    return exist;
}


int catalog_get_data_types(struct catalog_table_data* t_data, int** data_types){
    int num_of_attrs = t_data->attr_ht->size;
    *data_types = malloc(sizeof(int) * num_of_attrs);

    struct ht_node** val_nodes = t_data->attr_ht->node_list;
    for(int i = 0; i < num_of_attrs; i++){
        struct attr_data* a_data = val_nodes[i]->value->v_ptr;
        enum db_type a_type = a_data->type;
        int type_int_val = (int)a_type;

        (*data_types)[i] = type_int_val;
    }
    return num_of_attrs;
}



int catalog_get_p_key_indices(struct catalog_table_data* t_data, int** p_key_indices){
    int p_key_len = t_data->p_key_len;
    *p_key_indices = malloc(sizeof(int) * p_key_len);

    struct hashtable* attr_ht = t_data->attr_ht;
    char** p_key_attrs = t_data->primary_key_attrs;

    for(int i = 0; i < p_key_len; i++){
        struct attr_data* a_data = sv_ht_get(attr_ht, p_key_attrs[i]);
        (*p_key_indices)[i] = a_data->index;
    }
    return p_key_len;
}

struct hashtable* catalog_get_ht(){
    return table_ht;
}

void catalog_close(){
    write_catalog(catalog_loc, table_ht);

}












