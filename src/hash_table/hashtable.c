//
// Created by SGuan on 3/13/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/hash_table/hashtable.h"
#include "../../include/helper_module/hash_function.h"
#include "../../include/hash_table/ht_str_int.h"
#include "../../include/hash_table/ht_str_void.h"




//struct ht_str_int* create_ht_str_int(int capacity, double load_factor);
int ht_str_int_add(struct ht_str_int* ht, char* key, int val);
int ht_str_int_get(struct ht_str_int* ht, char* key);
int ht_str_int_remove(struct ht_str_int* ht, char* key);
int ht_str_int_contains(struct ht_str_int* ht, char* key);
void print_ht_str_int_content(struct ht_str_int* ht);
void ht_free_ht_str_int(struct ht_str_int* ht);


void recur_free(struct ht_node* n, enum d_type val);
void ht_free_table(struct ht_node** table, int capacity, enum d_type val_type);
int ht_resize(struct ht_node*** table, int* capacity, int size, int multiplier, double load_factor);
void ht_close(void* ht, enum ht_type type);




void* ht_create(struct ht_container** content, enum d_type key, enum d_type val, int capacity, double load_factor){
    *content = (struct ht_container*)malloc(sizeof(struct ht_container));
    (*content)->size = 0;
    (*content)->capacity = capacity;
    (*content)->base_multiplier = capacity;
    (*content)->load_factor = load_factor;
    (*content)->table = (struct ht_node**)malloc(sizeof(struct ht_node*)*capacity);

    for(int i = 0; i < capacity; i++){
        (*content)->table[i] = NULL;
    }

    switch(key){
        case INT:
            switch(val){
                case INT:
                    break;
                case STRING:
                    break;
                default:
                    fprintf(stderr, "Invalid val type : %d\n", val);
                    break;
            }
            break;
        case STRING:
            switch(val){
                case INT:
                    return (void*)get_ht_str_int();
                    break;
                case STRING:
                    break;
                case VOID_PTR:
                    return (void*)get_ht_str_void();
                default:
                    fprintf(stderr, "Invalid val type : %d\n", val);
                    break;
            }
            break;
        default:
            fprintf(stderr, "Invalid key type : %d\n", key);
            break;
    }
}


//
//struct ht_str_int* create_ht_str_int(int capacity, double load_factor){
//
//    struct ht_str_int* ht = (struct ht_str_int*)malloc(sizeof(struct ht_str_int));
//
//    ht->size = 0;
//    ht->capacity = capacity;
//    ht->base_multiplier = capacity;
//    ht->load_factor = load_factor;
//    ht->table_type = HT_STR_INT;
//    ht->val_type = INT;
//    ht->table = (struct ht_node**)malloc(sizeof(struct ht_node*)*capacity);
//    for(int i = 0; i < capacity; i++){
//        ht->table[i] = NULL;
//    }
//
//    //link the functions
//    ht->add = ht_str_int_add;
//    ht->get = ht_str_int_get;
//    ht->remove = ht_str_int_remove;
//    ht->contains = ht_str_int_contains;
//    ht->close = ht_free_ht_str_int;
//    ht->resize = ht_resize;
//    ht->print = print_ht_str_int_content;
//
////    ht->load_factor = 1.01;
//
//    return ht;
//
//}
//
//int ht_str_int_add(struct ht_str_int* ht, char* key, int val){
//
//    ht_resize(&(ht->table), &(ht->capacity), ht->size, ht->base_multiplier, ht->load_factor);
//
//    char* hash_code_str = hash_str(key);
//    int index = hash_compute_index(hash_code_str, ht->capacity);
//
//    int add_successful = -1;
//    struct ht_node** table = ht->table;
//    struct ht_node* curr = table[index];
//    if(curr==NULL){
//
//        table[index] = (struct ht_node*)malloc(sizeof(struct ht_node));
//        table[index]->key = (char*)malloc(sizeof(char)*strlen(key)+1);
//        strncpy(table[index]->key, key, strlen(key)+1);
//        table[index]->hash_code_str = hash_code_str;
//        table[index]->value = (union ht_node_value*)malloc(sizeof(union ht_node_value));
//        table[index]->value->i = val;
//        table[index]->next_node = NULL;
//        add_successful = 0;
//    }else{
//        //check if a node with the key already exist and replace the value
//        while(curr!=NULL){
//            if(strncmp(curr->key, key, strlen(key))==0){
//                curr->value->i = val;
//                add_successful = 0;
//                return add_successful;
//            }
//        }
//
//        struct ht_node* new_node = (struct ht_node*)malloc(sizeof(struct ht_node));
//        new_node->key = (char*)malloc(sizeof(char)*strlen(key)+1);
//        strncpy(new_node->key, key, strlen(key)+1);
//        new_node->hash_code_str = hash_code_str;
//        new_node->value = (union ht_node_value*)malloc(sizeof(union ht_node_value));
//        new_node->value->i = val;
//        new_node->next_node = table[index];
//        table[index] = new_node;
//        add_successful = 0;
//    }
//
//    ht->size++;
//
//    return add_successful;
//}
//
//int ht_str_int_get(struct ht_str_int* ht, char* key){
//    char* hash_code_str = hash_str(key);
//    int index = hash_compute_index(hash_code_str, ht->capacity);
//
//    struct ht_node* curr = ht->table[index];
//    int value = -1;
//    while(curr!=NULL){
//        if(strncmp(curr->key, key, strlen(key)) == 0){
//            value = curr->value->i;
//            break;
//        }
//        curr = curr->next_node;
//    }
//    free(hash_code_str);
//    return value;
//}
//
//int ht_str_int_remove(struct ht_str_int* ht, char* key){
//    char* hash_code_str = hash_str(key);
//    int index = hash_compute_index(hash_code_str, ht->capacity);
//
//    int result = -1;
//    struct ht_node* curr = ht->table[index];
//    struct ht_node* prev = NULL;
//    while(curr!=NULL){
//        if(strncmp(curr->key, key, strlen(key))==0){
//            if(prev==NULL){
//                //remove the head
//                ht->table[index] = curr->next_node;
//            }else{
//                prev->next_node = curr->next_node;
//            }
//            result = 0;
//            break;
//        }
//        prev = curr;
//        curr = curr->next_node;
//    }
//
//    //clean up
//    free(hash_code_str);
//    free(curr->key);
//    free(curr->hash_code_str);
//    free(curr->value);
//    free(curr);
//
//    ht->size--;
//
//    return result;
//}
///**
// * check if the key exist in the hash table
// * @param ht
// * @param key
// * @return returns 1 if exist and 0 if it does not
// */
//int ht_str_int_contains(struct ht_str_int* ht, char* key){
//    char* hash_code_str = hash_str(key);
//    int index = hash_compute_index(hash_code_str, ht->capacity);
//
//    int result = 0;
//    struct ht_node* curr = ht->table[index];
//    while(curr!=NULL){
//        if(strncmp(curr->key, key, strlen(key))==0){
//            result = 1;
//            break;
//        }
//        curr = curr->next_node;
//    }
//    free(hash_code_str);
//    return result;
//}
//
//void ht_free_ht_str_int(struct ht_str_int* ht){
//    if(ht){
//
//        int size = ht->capacity;
//        for(int i = 0; i < size; i++){
//            struct ht_node* curr = ht->table[i];
//            if(curr != NULL){
//                recur_free(curr, ht->val_type);
//            }
//        }
//        free(ht->table);
//        free(ht);
//    }
//}
//
//
//void print_ht_str_int_content(struct ht_str_int* ht){
//
//    int capacity = ht->capacity;
//    struct ht_node** table = ht->table;
//    for(int i = 0; i < capacity; i++){
//        if(table[i]!=NULL){
//            struct ht_node* curr = table[i];
//            while(curr != NULL){
//                printf("index_%d key : %s, val : %d\n", i, curr->key, curr->value->i);
//                curr = curr->next_node;
//            }
//
//        }
//    }
//}
//
//
//
//
//
//
//int ht_resize(struct ht_node*** table, int* capacity, int size, int multiplier, double load_factor){
//
//    int threshold = ceil(*capacity * load_factor);
//    printf("threshold is %d\n", threshold);
//
//    if(size + 1 >= threshold){
////        printf("true\n");
////        printf("size is : %d\n", *size);
//        int new_capacity = (*capacity) * multiplier;
//        struct ht_node** new_table = (struct ht_node**)malloc(sizeof(struct ht_node*)*new_capacity);
//        for(int i = 0; i < new_capacity; i++){
//            new_table[i] = NULL;
//        }
//        //remap all the nodes
//        for(int i = 0; i < *capacity; i++){
//            if((*table)[i]!=NULL){
//                struct ht_node* curr = (*table)[i];
//                struct ht_node* tmp_node = NULL;
//
//                while(curr!=NULL){
//                    tmp_node = curr;
//                    curr = curr->next_node;
//
//                    int new_index = hash_compute_index(tmp_node->hash_code_str, new_capacity);
//
//                    if(new_table[new_index] == NULL){
//                        tmp_node->next_node = NULL;
//                        new_table[new_index] = tmp_node;
//                    }else{
//                        tmp_node->next_node = new_table[new_index];
//                        new_table[new_index] = tmp_node;
//                    }
//                }
//            }
//        }
//        printf("done resizing\n");
//        free(*table);
//        *table = new_table;
//        *capacity = new_capacity;
//
//    }
//
//
//
//    return 0;
//}
//
//
//void recur_free(struct ht_node* n, enum d_type val){
//    if(n != NULL){
//        recur_free(n->next_node, val);
//        free(n->key);
//        free(n->hash_code_str);
//        if(val == STRING){
//            free(n->value->c);
//        }
//        free(n->value);
//        free(n);
//        n = NULL;
//    }
//}
//
//void ht_free_table(struct ht_node** table, int capacity, enum d_type val_type){
////    printf("capacity is : %d\n", capacity);
//    for(int i = 0; i < capacity; i++){
//        struct ht_node* curr = table[i];
////        printf("in free val is : %d\n", table[i]->value->i);
////        printf("freeing\n");
//        if(curr != NULL){
//
//            recur_free(curr,val_type);
//        }
//    }
////    printf("free is done\n");
//}
//
//
//
//void ht_close(void* ht, enum ht_type type){
//    switch(type){
//        case HT_STR_INT:
//            ht_free_ht_str_int((struct ht_str_int*)ht);
//            break;
//        default:
//            fprintf(stderr, "Invalid ht_type!\n");
//            break;
//    }
//}