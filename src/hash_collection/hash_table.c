//
// Created by SGuan on 3/17/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../include/hash_collection/hash_table.h"
#include "../../include/hash_collection/hash_function.h"


int ht_add(struct hashtable* ht, char* key, union ht_node_value* value){

    int threshold = ceil(ht->capacity * ht->load_factor);
    if(ht->size + 1 >= threshold){
        printf("resizing : threshold=%d\n", threshold);
        ht_resize(ht);
    }

    unsigned long long hash_code = hash_str(key);
    int index = hash_compute_index(hash_code, ht->capacity);
//    printf("index=%d\n",index);

    struct ht_node* new_node = NULL;
    struct ht_node** table = ht->table;
    if(table[index]==NULL){
        new_node = malloc(sizeof(struct ht_node));
        new_node->key = malloc(sizeof(char*) * strlen(key)+1);
        strncpy(new_node->key, key, strlen(key)+1);
        new_node->hash_code = hash_code;
        new_node->value = value;
        new_node->next_node = NULL;
        new_node->table_index = index;
        new_node->list_index = ht->size;        //the index of the node in the node_list
        ht->node_list[ht->size] = new_node;
        table[index] = new_node;
        ht->size++;
        return 0;
    }else if(table[index]!=NULL){

        struct ht_node* curr = table[index];
        while(curr!=NULL){
            if(curr->hash_code == hash_code){
                free(curr->value);
                curr->value = value;
                return 1;
            }
            curr = curr->next_node;
        }

        //if the key does not exist
        new_node = malloc(sizeof(struct ht_node));
        new_node->key = malloc(sizeof(char*) * strlen(key)+1);
        strncpy(new_node->key, key, strlen(key)+1);
        new_node->hash_code = hash_code;
        new_node->value = value;
        new_node->next_node = table[index];
        new_node->table_index = index;
        new_node->list_index = ht->size;        //the index of the node in the node_list
        ht->node_list[ht->size] = new_node;

        table[index] = new_node;
        ht->size++;
        return 0;
    }

    return -1;
}

union ht_node_value* ht_get(struct hashtable* ht, char* key){
    unsigned long long hash_code = hash_str(key);
    int index = hash_compute_index(hash_code, ht->capacity);

    if(ht->table[index]==NULL){
        return NULL;
    }

    struct ht_node* curr = ht->table[index];

    while(curr!=NULL){
        if(curr->hash_code == hash_code){
            return curr->value;
        }
        curr = curr->next_node;
    }


    return NULL;
}


union ht_node_value* ht_remove(struct hashtable* ht, char* key){
    unsigned long long hash_code = hash_str(key);
    int index = hash_compute_index(hash_code, ht->capacity);
    union ht_node_value* ret_val = NULL;


    if(ht->table[index]!=NULL){
//        printf("key is not null\n");
        struct ht_node* curr = ht->table[index];
        struct ht_node* prev = NULL;

        if(curr->hash_code != hash_code){
            curr = curr->next_node;
            prev = curr;

            while(curr!=NULL){
                if(curr->hash_code == hash_code){
                    break;
                }
                prev = curr;
                curr = curr->next_node;
            }
        }

        if(prev == NULL){
            ht->table[index] = curr->next_node;

        }else{
            prev->next_node = curr->next_node;
        }

        ret_val = curr->value;


        //remove the node from node_list
        int removed_index = curr->list_index;
        int last_index = ht->size - 1;
        struct ht_node* last_node = ht->node_list[last_index];
        last_node->list_index = removed_index;
        ht->node_list[removed_index] = last_node;
        ht->node_list[last_index] = NULL;
        free(curr->key);
        free(curr);


        ht->size--;
    }
    return ret_val;
}

int ht_resize(struct hashtable* ht){
    int size = ht->size;
    int capacity = ht->capacity;
    int multiplier = ht->base_multiplier;
    double load_factor = ht->load_factor;

    int new_capacity = capacity * multiplier;

    //reallocate node_list
    ht->node_list = realloc(ht->node_list, sizeof(struct ht_node*) * (int)ceil(new_capacity * load_factor + 1));

    //reallocate the table
    struct ht_node** new_table = (struct ht_node**)malloc(sizeof(struct ht_node*)*new_capacity);
    for(int i = 0; i < new_capacity; i++){
        new_table[i] = NULL;
    }

    //remap all the nodes
    int new_index;
    unsigned long long hash_code;
    struct ht_node** node_list = ht->node_list;

    for(int i = 0; i < size; i++){
        hash_code = node_list[i]->hash_code;
//        printf("< Before computing : %llu >\n", hash_code);
        new_index = hash_compute_index(hash_code, new_capacity);
        node_list[i]->table_index = new_index;

        if(new_table[new_index] == NULL){
            node_list[i]->next_node = NULL;
            new_table[new_index] = node_list[i];
        }else{
            node_list[i]->next_node = new_table[new_index];
            new_table[new_index] = node_list[i];
        }
    }

    free(ht->table);
    ht->table = new_table;
    ht->capacity = new_capacity;
    printf("after resizing the capacity is : %d\n", ht->capacity);

    return 0;       //0 for successful

}

int ht_contains(struct hashtable* ht, char* key){
    unsigned long long hash_code = hash_str(key);
    int index = hash_compute_index(hash_code, ht->capacity);

    if(ht->table[index]==NULL){
        return 0;
    }else{
        struct ht_node* curr = ht->table[index];
        while(curr!=NULL){
            if(curr->hash_code == hash_code){
                return 1;
            }
            curr = curr->next_node;
        }
    }

    return -1;

}

void ht_destroy(struct hashtable* ht){
    if(ht){
        int size = ht->size;
        for(int i = 0; i < size; i++){
            struct ht_node* node = ht->node_list[i];
            free(node->key);
            free(node->value);
            free(node);
            node = NULL;
        }
        free(ht->table);
        free(ht->node_list);
        free(ht);
    }
}

void ht_print(struct hashtable* ht){
    struct ht_node** node_list = ht->node_list;
    int size = ht->size;

    for(int i = 0; i < size; i++){
        printf("index=%d, key=%s, hash_code=%llu\n", node_list[i]->table_index, node_list[i]->key, node_list[i]->hash_code);
    }
}