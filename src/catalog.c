/**
 * Note: Case does not matter
 */

#include "catalog.h"
#include "helper_module/hash_function.h"
#include "helper_module/helper_function.h"
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

static int table_names_arr_cap;                   //capacity of table names arr
static char** table_names;                        //a array of table names

/*
 *  Private function predefintion 
 */
int init_mapping(int capacity, bool catalog_exist);
int init_table_names_list();
int table_map_resize();
int table_reorder(int removed_table_num);
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
    init_table_names_list();
    return 0;
}



int init_mapping(int capacity, bool catalog_exist){

//    printf("initial size : %d\n", table_map_size);

    if(catalog_exist==true){
        //load the capacity of the table_map, its multiplier, its size
        //and the table_map content
//        printf("reading catalog file...\n");
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

int init_table_names_list(){
    if(table_map_size == 0){
        table_names_arr_cap = 10;
    }else{
        table_names_arr_cap = table_map_size * 2;
    }
    table_names = (char**)malloc(sizeof(char*)*table_names_arr_cap);
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

/*
 *Checks to see if table_name is already in the table_map
 *
 * returns: 1 if exist and 0 if it does not exist;
 */

int catalog_table_mapping_contains(char* table_name){
    char* hash_code_str = hash_str(table_name);            //compute the hash_code
    int index = hash_compute_index(hash_code_str, table_map_capacity);

    struct i_node* curr = table_map[index];
    int value = 0;
    while(curr!=NULL){
        if(strncmp(curr->key, table_name, strlen(table_name)) == 0){
            value = 1;
            break;
        }
        curr = curr->next_node;
    }
    free(hash_code_str);
    return value;
}


int catalog_table_mapping_add(char* table_name, int num){
    //check if resize is needed
    table_map_resize();

//    printf("added");
    char* hash_code_str = hash_str(table_name);            //compute the hash_code
    int index = hash_compute_index(hash_code_str, table_map_capacity);
    struct i_node* curr = table_map[index];

    int add_successful = -1;

    if(curr==NULL){
        table_map[index] = malloc(sizeof(struct i_node));
        table_map[index]->key = malloc(sizeof(char)*(strlen(table_name)+1));
        strncpy(table_map[index]->key, table_name, strlen(table_name)+1);
        table_map[index]->hash_code_str = hash_code_str;
        table_map[index]->value = num;
        table_map[index]->next_node = NULL;
        add_successful = 0;
    }else{
        //check if a node with the key already exist and replace the value
        while(curr!=NULL){
            if(strncmp(curr->key, table_name, strlen(table_name))==0){
                curr->value = num;
                return 0;
            }
            curr = curr->next_node;
        }

        struct i_node* new_node = (struct i_node*)malloc(sizeof(struct i_node));
        new_node->key = (char*)malloc(sizeof(char)*strlen(table_name)+1);
        strncpy(new_node->key, table_name, strlen(table_name)+1);
        new_node->hash_code_str = hash_code_str;
        new_node->value = num;
        new_node->next_node = table_map[index];
        table_map[index] = new_node;
        add_successful = 0;
    }

    if(add_successful==0){
        if(table_map_size >= table_names_arr_cap){
            table_names_arr_cap = table_names_arr_cap * 2;
            table_names = (char**)realloc(table_name, sizeof(char*) * table_names_arr_cap);
        }
        //add table_name to table_names
//        printf("here\n");
//        printf("table_name %s\n", table_map[index]->key);
//        printf("table_map_size %d\n", table_map_size);
        table_names[table_map_size] = table_map[index]->key;
    }



    table_map_size++;
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
    int index = hash_compute_index(hash_code_str, table_map_capacity);


    int result = -1;
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
            result = 0;
            break;
        }
        prev = curr;
        curr = curr->next_node;
    }

    table_reorder(curr->value);

    //reorder the tables
    free(hash_code_str);
    free(curr->key);
    free(curr->hash_code_str);
    free(curr);


    //decrement the size
    table_map_size--;
//    printf("size of map is : %d\n", table_map_size);

    return result;
}

int table_reorder(int removed_table_num){

    char* hash_code_str = hash_str(table_names[table_map_size-1]);
    int index = hash_compute_index(hash_code_str, table_map_capacity);


    //table_names[remove_table_num] points to the last table's name
    table_names[removed_table_num-1] = table_map[index]->key;
    printf("table name is %s\n", table_map[index]->key);
    //point the old table name in table_name arr to NULL;
    table_names[table_map[index]->value-1] = NULL;
    //change the table_num of the newly placed table to the removed table's num
    table_map[index]->value = removed_table_num;


    return 0;
}

int read_catalog(){
    //do reading
    FILE* catalog_file = fopen(catalog_loc, "rb");
    int map_size;
    fread(&table_map_capacity, sizeof(int), 1, catalog_file);
    fread(&table_map_multiplier, sizeof(int), 1, catalog_file);
    fread(&map_size, sizeof(int), 1, catalog_file);

    //initialize the table_map
    table_map = (struct i_node**)malloc(sizeof(struct i_node*)*table_map_capacity);
    for(int i = 0; i <table_map_capacity; i++){
        table_map[i] = NULL;
    }
    //read all entries
    //format: <len of table name> <table_name> <table_num>
    char buffer[255];
    buffer[0] = 0;
//    printf("size of map is : %d\n", map_size);
    for(int i = 0; i < map_size; i++){
        int table_name_len;
        int table_num;
        fread(&table_name_len, sizeof(int), 1, catalog_file);
        fread(buffer, sizeof(char), table_name_len, catalog_file);
        buffer[table_name_len]=0;
        fread(&table_num, sizeof(int), 1, catalog_file);
        catalog_table_mapping_add(buffer,table_num);
        printf("name is: %s\n", buffer);
        clear_buffer(buffer, table_name_len+2);
    }

    fclose(catalog_file);
    return 0;
}

int write_catalog(){
    //do writing
    FILE* catalog_file = fopen(catalog_loc, "wb");

    //write table_map_capacity
    fwrite(&table_map_capacity, sizeof(int), 1, catalog_file);
    //write table_map_multiplier
    fwrite(&table_map_multiplier, sizeof(int), 1, catalog_file);
    //write table_map_size
    fwrite(&table_map_size, sizeof(int), 1, catalog_file);

    //write all the mapping pairs to the file
    //format: <len of table name> <table_name> <table_num>
    for(int i = 0; i < table_map_capacity; i++){
        if(table_map[i]!=NULL){
            struct i_node* curr = table_map[i];
            while(curr!=NULL){
                int table_name_length = strlen(curr->key);
                int table_num = curr->value;
                fwrite(&table_name_length, sizeof(int), 1, catalog_file);
                fwrite(curr->key, sizeof(char), table_name_length, catalog_file);
                fwrite(&table_num, sizeof(int), 1, catalog_file);
                curr = curr->next_node;
            }
        }
    }
    fclose(catalog_file);
    return 0;
}

void catalog_recur_free_i_node(struct i_node* n){
    if(n != NULL){
        catalog_recur_free_i_node(n->next_node);
        free(n->key);
        free(n->hash_code_str);
        free(n);
        n=NULL;
    }
}

int catalog_free_table_map(){
    if(table_map){
        for(int i = 0; i < table_map_capacity; i++){
            struct i_node* curr = table_map[i];
            if(curr != NULL){
                catalog_recur_free_i_node(curr);
            }
        }
        free(table_map);
    }
}

int catalog_close(){
    //write everything to storage and then free everything
    write_catalog();
    catalog_free_table_map();
    free(table_names);
    free(catalog_loc);

    printf("catalog closed...\n");

    return 0;
}


/*
 * Below are functions used to help with debugging catalog
 */

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

void catalog_print_table_names(){
    if(table_map_size == 0){
        printf("Table_name's content = {}\n");
    }
    printf("Table_names's Content: {\n");
    for(int i = 0; i < table_map_size; i++){
        printf("table_num: %d, table_name: %s\n", i+1, table_names[i]);
    }
    printf("}\n");


}

void catalog_test_print(){
    printf("test");
}


