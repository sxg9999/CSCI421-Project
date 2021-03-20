///**
// * Note: Case does not matter
// */
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "catalog.h"
#include "helper_module/helper_function.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/hash_collection/si_ht.h"
#include "../include/hash_collection/sv_ht.h"


struct attr_data{
    int int_val;             //the data_type of the attribute
    int attr_index;          //index of the attributes in a row/tuple
    int* constraints;        //a array of constraints in int form

};

struct table_data{
    int table_num;
    struct hashtable* attr_ht;             //a hash table of <attr_name,attr_data> pairs
    struct hashtable* foreign_key_ht;      //a hash table of <foreign_key, reference_table_name> pairs
    struct hashtable* primary_key_ht;         //a hash table of <attr_name, attr_index> pairs

};

static char catalog_file_name[] = "catalog.data";
static char* catalog_loc;
static struct hashtable* table_ht;


int init_hash_table(int catalog_exist);
int read_catalog();


int init_catalog(char* db_path){
    //check if catalog_exist then call init_mapping
    //catalog.data
    int length = strlen(db_path)+strlen(catalog_file_name);
    catalog_loc = malloc(sizeof(char)*length+1);
    snprintf(catalog_loc, length + 1, "%s%s", db_path, catalog_file_name);
    struct stat s;
    int catalog_exist = 0;   //0 is false, 1 is true
    if(stat(catalog_loc, &s)==0){
        catalog_exist = 1;
    }else{
        printf("Catalog does not exist : %s\n", catalog_loc);
    }

    init_hash_table(catalog_exist);
    return 0;
}

int init_hash_table(int catalog_exist){
    int init_result = -1;

    if(catalog_exist==1){
        //do reading
        printf("read\n");
    }else{
        //init a new hashtable
        table_ht = ht_create(12, 0.75);
        init_result = 0;
    }

    return init_result;
}

int catalog_add(int table_num, char* table_name, struct attribute_params* attrParams,
                struct primary_key_params* pk_param, struct foreign_key_params* fk_params){
    return 0;
}



///*
// *  Private function predefintion
// */
//int init_mapping(int capacity, bool catalog_exist);
//int init_table_names_list();
//int table_map_resize();
//int table_reorder(int removed_table_num);
//int read_catalog();
//void free_attr_ht_node(struct ht_node* n);
//void free_attr_ht(struct hashtable* ht);
//void free_non_deep_ht_node(struct ht_node* n);
//
//
//
///*
// * db_path contains a ending '/'
// */

//
//
//
//int init_mapping(int capacity, bool catalog_exist){
//
//    if(catalog_exist==true){
//        read_catalog();
//    }else{
//        table_ht = ht_create(capacity, 0.75);
//
//    }
//    return 0;
//}
//
//
///**
// * returns the table num
// * @param table_name
// * @return returns table num if the table exist and returns -1 if the table doesn't exist
// */
//int catalog_get_table_num(char* table_name){
//
//    struct table_data* t_data = sk_ht_func->get(table_ht, table_name);
//
//    if(t_data != NULL){
//        return t_data->table_num;
//    }
//
//    return -1;
//}
//
///*
// *Checks to see if table_name is already in the table_map
// *
// * returns: 1 if exist and 0 if it does not exist;
// */
//
//int catalog_table_mapping_contains(char* table_name){
//
//    int result = sk_ht_func->contains(table_ht, table_name);
//    return result;
//}
//
//
//int catalog_table_mapping_add(char* table_name, int table_num, char** attr_names, int** attr_int_vals,
//                              char** foreign_keys){
//
//    struct table_data* t_data = (struct table_data*)malloc(sizeof(struct table_data));
//    t_data->table_num = table_num;
//    t_data->attr_ht = ht_create(10, 0.75);
//    t_data->foreign_key_ht = ht_create(10,0.75);
//    t_data->primary_keys_ht = ht_create(10, 0.75);
//
//    sk_ht_func->add(table_ht, table_name, t_data);
//    return 0;
//
//}
//
//int catalog_table_mapping_remove(char* table_name){
//
//     struct table_data* t_data = sk_ht_func->remove(table_ht, table_name);
//
//    //clean up by freeing up the memory
//    free_attr_ht(t_data->attr_ht);
//    sk_ht_func->close(t_data->foreign_key_ht);
//    ik_ht_func->close(t_data->primary_keys_ht);
//
//    free(t_data);
//
//    return 0;
//}
//
//
//int read_catalog(){
//    //do reading
////    int map_size;
////    int capacity;
////    int base_multiplier;
////    int size;
////
////    FILE* catalog_file = fopen(catalog_loc, "rb");
////
////    fread(&capacity, sizeof(int), 1, catalog_file);
////    fread(&base_multiplier, sizeof(int), 1, catalog_file);
////    fread(&map_size, sizeof(int), 1, catalog_file);
////
////    //initialize the table_map
////    ht = ht_create(capacity, 0.75);
//////    table_map = (struct i_node**)malloc(sizeof(struct i_node*)*table_map_capacity);
////
////    //initialize the table_name arr
//////    init_table_names_list();
////
//////    for(int i = 0; i <table_map_capacity; i++){
//////        table_map[i] = NULL;
//////    }
////
////    //read all entries
////    //format: <len of table name> <table_name> <table_num>
////    char buffer[255];
////    buffer[0] = 0;
//////    printf("size of map is : %d\n", map_size);
////    for(int i = 0; i < map_size; i++){
////        int table_name_len;
////        int table_num;
////        fread(&table_name_len, sizeof(int), 1, catalog_file);
////        fread(buffer, sizeof(char), table_name_len, catalog_file);
////        buffer[table_name_len]=0;
////        fread(&table_num, sizeof(int), 1, catalog_file);
////
////        catalog_table_mapping_add(buffer,table_num);
//////        printf("name is: %s\n", buffer);
////        clear_buffer(buffer, table_name_len+2);
////    }
////
////    fclose(catalog_file);
//    return 0;
//}
//
//int write_catalog(){
////    //do writing
////    FILE* catalog_file = fopen(catalog_loc, "wb");
////
////    //write table_map_capacity
////    fwrite(&table_map_capacity, sizeof(int), 1, catalog_file);
////    //write table_map_multiplier
////    fwrite(&table_map_multiplier, sizeof(int), 1, catalog_file);
////    //write table_map_size
////    fwrite(&table_map_size, sizeof(int), 1, catalog_file);
////
////    //write all the mapping pairs to the file
////    //format: <len of table name> <table_name> <table_num>
////    for(int i = 0; i < table_map_capacity; i++){
////        if(table_map[i]!=NULL){
////            struct i_node* curr = table_map[i];
////            while(curr!=NULL){
////                int table_name_length = strlen(curr->key);
////                int table_num = curr->value;
////                fwrite(&table_name_length, sizeof(int), 1, catalog_file);
////                fwrite(curr->key, sizeof(char), table_name_length, catalog_file);
////                fwrite(&table_num, sizeof(int), 1, catalog_file);
////                curr = curr->next_node;
////            }
////        }
////    }
////    fclose(catalog_file);
//    return 0;
//}
//
//void catalog_recur_free_i_node(struct i_node* n){
//    if(n != NULL){
//        catalog_recur_free_i_node(n->next_node);
//        free(n->key);
//        free(n->hash_code_str);
//        free(n);
//        n=NULL;
//    }
//}
//
//
//
//void free_attr_ht_node(struct ht_node* n){
//    if(n!=NULL){
//        free_attr_ht_node(n->next_node);        //recursively free
//        free(n->key);
//        free(n->hash_code);
//        struct attr_data* val = n->value->v_ptr;
//        free(val->constraints);
//        free(val);
//        free(n->value);
//        free(n);
//        n = NULL;
//    }
//}
//
//void free_attr_ht(struct hashtable* ht){
//    if(ht){
//        for(int i = 0; i < ht->capacity; i++){
//            struct ht_node* curr = ht->table[i];
//            if(curr!=NULL){
//                free_attr_ht_node(curr);
//            }
//        }
//        free(ht->table);
//        free(ht->keys);
//        free(ht);
//    }
//}
//
//
//void free_non_deep_ht_node(struct ht_node* n){
//    if(n!=NULL){
//        free_non_deep_ht_node(n->next_node);        //recursively free
//        free(n->key);
//        free(n->hash_code);
//        free(n->value);
//        free(n);
//    }
//}
//
///**
// * frees hashtable that don't have a value to be freed
// * @param ht
// * @return
// */
//int free_non_deep_ht(struct hashtable* ht){
//    if(ht){
//        for(int i = 0; i < ht->capacity; i++){
//            struct ht_node* curr = ht->table[i];
//            if(curr!=NULL){
//                free_non_deep_ht_node(curr);
//            }
//        }
//        free(ht->table);
//        free(ht->keys);
//        free(ht);
//    }
//}
//
//int catalog_close(){
//    //write everything to storage and then free everything
////    write_catalog();
////    catalog_free_table_map();
////    free(table_names);
////    free(catalog_loc);
//
////    printf("catalog closed...\n");
//
//    return 0;
//}
//
//
///*
// * Below are functions used to help with debugging catalog
// */
//
//void catalog_print_table_map_content(){
//
////    if(table_map_size == 0){
////        printf("Table_Map's Content: {}\n");
////        return;
////    }
////    printf("Table_Map's Content: {\n");
////    for(int i = 0; i < table_map_capacity; i++){
////        if(table_map[i]!=NULL){
////            struct i_node* curr = table_map[i];
////            while(curr!=NULL){
////                printf("\tindex : %d, Key : %s, Value: %d\n", i, curr->key, curr->value);
////                curr = curr->next_node;
////            }
////        }
////    }
//    printf("}\n");
//}
//
//void catalog_print_table_map_info(){
////    printf("size : %d, capacity : %d, multiplier : %d,  load_factor : %lf, threshold : %lf\n",
////           table_map_size, table_map_capacity, table_map_multiplier, load_factor, ceil(table_map_capacity*load_factor));
//
//}
//
//void catalog_print_table_names(){
////    if(table_map_size == 0){
////        printf("Table_name's content = {}\n");
////    }
////    printf("Table_names's Content: {\n");
////    for(int i = 0; i < table_map_size; i++){
////        printf("table_num: %d, table_name: %s\n", i+1, table_names[i]);
////    }
////    printf("}\n");
//
//
//}
//
//void catalog_test_print(){
//    printf("test");
//}


