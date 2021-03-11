/**
 * Note: Case does not matter
 */

#include "catalog.h"
#include "helper_module/hash_function.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>



struct i_node;

struct i_node{
    char* key;
    char* hash_code_str;
    int value;
    struct i_node* next_node;
};


static char* catalog_loc;
static double load_factor = 0.75;

static int table_map_capacity;
static int table_map_multiplier;
static int table_map_size;
static struct i_node** table_map = NULL;          //a hashtable that maps a table name to a table num


/*
 *  Private function predefintion 
 */
int init_mapping(int capacity, bool catalog_exist);
int table_map_resize();
int read_catalog();



/*
 * db_path contains a ending '/'
 */
int init_catalog(char* db_path){
    //check if catalog_exist then call init_mapping
    //to be done
    //catalog.data

    int length = strlen(db_path)+12;
    catalog_loc = malloc(sizeof(char)*length);
    snprintf(catalog_loc, length + 1, "%s%s", db_path, "catalog.data");
    
    struct stat s;
    bool catalog_exist = false;
    if(stat(catalog_loc, &s)==0){
        catalog_exist = true;
    }else{
        printf("Catalog does not exist : %s\n", catalog_loc);
    }
    
    init_mapping(5, catalog_exist);
    return 0;
}



int init_mapping(int capacity, bool catalog_exist){
    
    if(catalog_exist==true){
        //load the capacity of the table_map, its multiplier, its size
        //and the table_map content
        printf("reading catalog file...");
        read_catalog();
    }else{
        table_map_capacity = capacity;
        table_map_size = 0;
        table_map_multiplier = capacity;
        table_map = (struct i_node**)malloc(sizeof(struct i_node*)*table_map_capacity);
        for(int i =  0; i < table_map_capacity; i++){
            table_map[i] = NULL;
        }
    }
    printf("catalog initiated...\n");
    return 0;
}


int catalog_get_table_num(char* table_name){
    char* hash_code_str = hash_str(table_name);            //compute the hash_code
    int index = hash_compute_index(hash_code_str, table_map_capacity);

    struct i_node* curr = table_map[index];
    int value = -1;
    while(curr!=NULL){
        if(strncmp(curr->key, table_name, strlen(table_name)) == 0){
            value = curr->value;
            break;
        }
        curr = curr->next_node;
    }
    free(hash_code_str);
    return value;
}

void catalog_print_table_map_content(){
    

    if(table_map_size == 0){
        printf("Table_Map's Content: {}\n");
        return;
    }
    printf("Table_Map's Content: {\n");
    for(int i = 0; i < table_map_capacity; i++){
        if(table_map[i]!=NULL){
            struct i_node* curr = table_map[i];
            while(curr!=NULL){
                printf("\tindex : %d, Key : %s, Value: %d\n", i, curr->key, curr->value);
                curr = curr->next_node;
            }
        }
    }
    printf("}\n");
}

void catalog_print_table_map_info(){
    printf("size : %d, capacity : %d, multiplier : %d,  load_factor : %lf, threshold : %lf\n", 
                table_map_size, table_map_capacity, table_map_multiplier, load_factor, ceil(table_map_capacity*load_factor));

}

void catalog_test_print(){
    printf("test");
}

int catalog_table_mapping_add(char* table_name, int num){

    //check if resize is needed
    table_map_resize();

    // char* hash_code_str2 = hash_str(table_name);            //compute the hash_code
    // printf("add here\n");
    // int index = hash_compute_index(hash_code_str, table_map_capacity);
    
    printf("Before computing index : %s\n", table_name);
    char hash_code_str[] = "123445666";
    int index = 0;
    struct i_node* curr = table_map[index];

    if(curr==NULL){
        
        table_map[index] = malloc(sizeof(struct i_node));
        // struct i_node* new_node = malloc(sizeof(struct i_node));
        table_map[index]->key = malloc(sizeof(char)*strlen(table_name)+1);
        
        strncpy(table_map[index]->key, table_name, strlen(table_name)+2);

        table_map[index]->hash_code_str = hash_code_str;
        table_map[index]->value = num;
        table_map[index]->next_node = NULL;
    }else{
        //check if a node with the key already exist and replace the value
        while(curr!=NULL){
            
            if(strncmp(curr->key, table_name, strlen(table_name))==0){
                printf("Already exist : %s\n", table_name);
                curr->value = num;
                return 0;
            }
            curr = curr->next_node;
        }
        
        printf("collision : %s\n", table_name);
        struct i_node* new_node = (struct i_node*)malloc(sizeof(struct i_node));
        printf("after allocating new node\n");
        new_node->key = (char*)malloc(sizeof(char)*strlen(table_name)+1);
        printf("allocating mem in key \n");

        strncpy(new_node->key, table_name, strlen(table_name)+2);

        new_node->hash_code_str = hash_code_str;
        new_node->value = num;
        new_node->next_node = table_map[index];
        table_map[index] = new_node;
        printf("done adding after collision\n");

    }

    
    table_map_size++;
    printf("successfully added : %s\n", table_name);
    return 0;

}

int table_map_resize(){
    //check if resizing is needed
    int threshold = ceil(table_map_capacity * load_factor);
    if(table_map_size + 1 >= threshold){
        //resize table_map
        int new_capacity = table_map_capacity * table_map_multiplier;
        struct i_node** new_table_map = (struct i_node**)malloc(sizeof(struct i_node*)*new_capacity);
        for(int i = 0; i < new_capacity; i++){
            new_table_map[i] = NULL;
        }
        //remap all the nodes in the old table map to the new table map
        for(int i = 0; i < table_map_capacity; i++){
            if(table_map[i]!=NULL){
                struct i_node* curr = table_map[i];
                struct i_node* tmp_node = NULL;
                //for all next_nodes map them to the new table
                while(curr != NULL){
                    tmp_node = curr;
                    curr = curr->next_node;
                    
                    int new_index = hash_compute_index(tmp_node->hash_code_str, new_capacity);
                    
                   if(new_table_map[new_index]==NULL){
                       //let new_tabl_map[new_index] point to the node with
                       //its next_node NULL;
                       tmp_node->next_node = NULL;
                       new_table_map[new_index] = tmp_node;
                   }else{
                       tmp_node->next_node = new_table_map[new_index];
                       new_table_map[new_index] = tmp_node;
                   }
                }
                
            }
        }      
        free(table_map);            //free the old table map
        table_map = new_table_map;  //point table_map to the new table_map
        table_map_capacity = new_capacity;
    }
    return 0;

}



int catalog_table_mapping_remove(char* table_name){
    char* hash_code_str = hash_str(table_name);
    int index = hash_compute_index(hash_code_str, table_map_size);

    struct i_node* curr = table_map[index];
    struct i_node* prev = NULL;
    while(curr!=NULL){
        if(strncmp(curr->key, table_name, strlen(table_name)) == 0){
            //remove this mapping
            if(prev==NULL){
                //the head of the list is the node to be removed
                table_map[index] = curr->next_node;
            }else{
                prev->next_node = curr->next_node; 
            }
            free(curr->key);
            free(curr->hash_code_str);
            free(curr);
            return 0;

        }
        prev = curr;
        curr = curr->next_node;
    }
    return -1;
}

int read_catalog(){
    //do reading
    return 0;
}

int write_catalog(){
    //do writing
    return 0;
}

int catalog_close(){
    //write everything to storage
    return 0;
}



