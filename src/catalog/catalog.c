///**
// * Note: Case does not matter
// */
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/db_types.h"
#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/si_ht.h"
#include "../../include/hash_collection/sv_ht.h"




static char catalog_file_name[] = "catalog.data";
static char* catalog_loc;

static struct hashtable* table_ht;


int init_catalog(char* db_path){
    //check if catalog_exist then call init_mapping
    //catalog.data
    int length = strlen(db_path)+strlen(catalog_file_name);
    catalog_loc = malloc(sizeof(char)*length+1);
    snprintf(catalog_loc, length + 1, "%s%s", db_path, catalog_file_name);
    struct stat s;

    if(stat(catalog_loc, &s)==0){
        //catalog exist
        printf("catalog exist\n");
    }else{
        //catalog does not exist
        printf("Catalog does not exist : %s\n", catalog_loc);
        table_ht = ht_create(12, 0.75);
    }

    return 0;
}



/**
 * get the default value for the attribute and assign it to "val"
 * @param str
 * @param attr_type
 * @param val
 * @return returns the pointer to the remaining string
 */
union default_value* get_attr_constr_default_val(char** str, enum db_type attr_type){
    union default_value* val = malloc(sizeof(union default_value));
    int start_index = 0;
    int end_index = 0;
    char* ptr;

    //check if str = "constr_val" <constraints>
    if(str[0] != '"'){
        // this means str = constr_val <constraints>
        ptr = strchr((*str)+1, ' ');

        if(ptr == NULL){
            end_index = strlen(*str) - 1;
        }else{
            end_index = ptr - *str - 1;
            ptr = ptr + 1;
        }

        char* val_str = substring(*str, start_index, end_index);
        int i_val;
        double d_val;

        switch(attr_type){
            case INT:
                sscanf(val_str, "%d", &i_val);
                val->i = i_val;
                break;
            case DOUBLE:
                sscanf(val_str, "%lf", &d_val);
                val->d = d_val;
                break;
            case BOOL:
                if(strncmp(val_str, "true", 4) == 0){
                    val->b = true;
                }else{
                    val->b = false;
                }
                break;
            default:
                fprintf(stderr, "Error: cannot match constraint type!!!\n");
                break;
        }
        free(val_str);
    }else{
        //str = "constraint val" <constraints>
        ptr = strchr((*str) + 1, '"');     //ptr currently points to '" <constrains>'

        if(ptr == NULL){
            fprintf(stderr, "(catalog.c/get_attr_constr_default_val) Expected '\"' but could not find any!!!\n");
            fprintf(stderr, "str = >%s<\n", *str);
            exit(0);
        }

        end_index = ptr - *str - 1;
        start_index = 1;
        ptr = ptr + 2;          //ptr now currently points to "<constraints>..."

        char* val_str = substring(*str, start_index, end_index);

        if(attr_type == CHAR){
            strncpy(val->c, val_str, strlen(val_str) + 1);
        }else{
            strncpy(val->v, val_str, strlen(val_str) + 1);
        }
        free(val_str);
    }
    *str = ptr;

    return val;
}

/**
 * get the constraint type and values for the attribute
 * @param str
 * @param attr_type
 * @param constr_type
 * @param val
 * @return
 */
union default_value* get_attr_constr_type_val(char** str, enum db_type attr_type, enum db_type* constr_type){
    int start_index = 0;
    int end_index = 0;
    char* ptr;
    union default_value* val;
    val = NULL;

    ptr = strchr(*str, ' ');         //i.e "notnull" -> null b/c there is no space afterward
    if(ptr == NULL){
        end_index = strlen(*str) - 1;
        char* constr_name = substring(*str, start_index, end_index);
        enum db_type tmp_type = typeof_kw(constr_name);


        if(tmp_type == UNKNOWN || tmp_type == DEFAULT){
            fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Type of constraint name is not the following: notnull !!!\n");
            fprintf(stderr, "constraint name is >%s<\n", constr_name);
            exit(0);
        }
        free(constr_name);
        *constr_type = tmp_type;
        *str = ptr;

        return NULL;
    }

    //else there is more than one constraint i.e. "notnull default ..." -> " default ..."
    end_index = ptr - *str - 1;
    char* constr_name = substring(*str, start_index, end_index);
    enum db_type tmp_type = typeof_kw(constr_name);
    free(constr_name);
    ptr = ptr + 1;

    if(tmp_type == UNKNOWN){
        fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Type of constraint name is unknown!!!\n");
        fprintf(stderr, "constraint name is >%s<\n", constr_name);
        exit(0);
    }

    if(tmp_type == DEFAULT){
        *constr_type = tmp_type;
        val = get_attr_constr_default_val(&ptr, attr_type);
        *str = ptr;
        return val;
    }

    //else it has to be NotNULL
    *constr_type = tmp_type;
    val = NULL;
    *str = ptr;
    return val;


}



struct attr_constraint** get_attr_constraints(int* count, enum db_type attr_type, char* constr_str){
    printf("constraints are : >%s<\n", constr_str);
    struct attr_constraint** constr = malloc(sizeof(struct attr_constraint*)*4);
    *count = 0;

    int constr_index = 0;
    char* ptr = constr_str;
    enum db_type constr_type;


    while(1){
        union default_value* default_val = get_attr_constr_type_val(&ptr, attr_type, &constr_type);

        if(constr_type == UNKNOWN){
            // just a precaution
            fprintf(stderr, "(catalog.c/get_attr_constraints) returned constr_type is unknown (this is IMPOSSIBLE)!!!\n");
            exit(0);
        }

        constr[constr_index] = malloc(sizeof(struct attr_constraint));
        constr[constr_index]->type = constr_type;
        constr[constr_index]->value = default_val;
        constr_index += 1;

        if(ptr == NULL){
            break;
        }
    }

    *count = constr_index;
    return constr;
}

/**
 * Get the attribute name and store it in attr_name
 * @param str
 * @return a ptr to the remaining part of the string
 */
char* get_attr_name(char* str, char** attr_name){
    int start_index = 0;
    int end_index = 0;
    char* ptr;

    ptr = strchr(str, ' ');

    if(ptr == NULL){
        fprintf(stderr, "(catalog.c/get_attr_name) Can't get attribute name!!!\n");
        fprintf(stderr, "Either attribute dont have a data type or there are no space!!!\n");
        exit(0);
    }

    end_index = ptr - str - 1;
    *attr_name = substring(str, start_index, end_index);

    return ptr + 1;             //returns "<data_type> ..."
}

/**
 * get the size of the attribute type
 * @param type
 * @return the size of the attribute type
 */
int get_attr_size(enum db_type type){
    if(type == UNKNOWN){
        fprintf(stderr, "(catalog.c/get_attr_size) Type is unknown when it should be either: int, double, or bool\n");
        exit(0);
    }

    int size = 0;

    switch(type){
        case INT:
            size = sizeof(int);
            break;
        case DOUBLE:
            size = sizeof(double);
            break;
        case BOOL:
            size = sizeof(bool);
            break;
    }

    return size;

}

/**
 * get the size of a char type or a varchar type attribute
 * @param str the string that contains the size string i.e. "(23)" = size of 23
 * @return the size
 */
int get_attr_char_size(char** str){
    int size;
    int start_index = 1;
    int end_index = 1;
    char* ptr = strchr(*str, ')');      // ptr = "(23) <constraint>" -> ") <constraint>"

    end_index = ptr - *str - 1;
    char* attr_size_str = substring(*str, start_index, end_index);

    if(is_int(attr_size_str) == 0){
        //if attr_size_str is not a int in string form then give a error msg
        fprintf(stderr, "(catalog.c/get_attr_char_size)attr_size_str is not a valid int in string form!!\n");
        fprintf(stderr, "attr_size_str = >%s<\n", attr_size_str);
        exit(0);
    }
    sscanf(attr_size_str, "%d", &size);
    free(attr_size_str);

    ptr = strchr(ptr, ' ');
    if(ptr == NULL){
        //means that there are not constraints so set the ptr or in this case *str to NULL
        *str = NULL;
    }else{
        *str = ptr + 1;         //*str now points to "<constraints>"
    }

    return size;
}


char* get_attr_type_n_size(char* str, enum db_type* type, int* size){
    int start_index = 0;
    int end_index = 0;
    char* ptr;

    ptr = strchr(str, '(');     //i.e., "char(23) <constraints>"  -> "(23) <constraints>"

    if(ptr == NULL){
        printf("Not a char or a Varchar\n");

        ptr = strchr(str, ' '); //i.e., "int <constraints>" -> " <constraints>"

        if(ptr == NULL){
            printf("No constraints in %s\n", str);
            end_index = strlen(str) - 1;
            char* attr_type_str = substring(str, start_index, end_index);
            enum db_type tmp_type = typeof_kw(attr_type_str);
            free(attr_type_str);

            *size = get_attr_size(tmp_type);
            *type = tmp_type;

            return NULL;            //No contraints to parse so return NULL
        }

        //else this means ptr currently points to " <constraints>"
        end_index = ptr - str - 1;
        char* attr_type_str = substring(str, start_index, end_index);
        enum db_type tmp_type = typeof_kw(attr_type_str);
        free(attr_type_str);

        *size = get_attr_size(tmp_type);
        *type = tmp_type;

        return ptr + 1;              //returns "<constraints>" to be parsed later (notice that there are no leading space)
    }

    //else this means ptr currently points to "(23) <constraints>"
    end_index = ptr - str - 1;
    char* attr_type_str = substring(str, start_index, end_index);
    enum db_type tmp_type = typeof_kw(attr_type_str);
    free(attr_type_str);

    *size = get_attr_char_size(&ptr);       //ptr will become either NULL or points to the head of the constraints string
    *type = tmp_type;

    if(ptr == NULL){
        //there are no constraint
        printf("No constraints in %s\n", str);
        return NULL;
    }
    return ptr;

}

struct attr_data* get_attr(char* attr_data_str){
    struct attr_data* a_data = malloc(sizeof(struct attr_data));

    int attr_size;
    enum db_type type;
    char* attr_name;

    int num_of_constr;
    struct attr_constraint** constr;

    char* ptr;
    ptr = get_attr_name(attr_data_str, &attr_name);       //returned ptr should point to the begining of the data type
    ptr = get_attr_type_n_size(ptr, &type, &attr_size);   //can return NULL or a pointer to the head of the constraints


    if(ptr == NULL){
        //No constraints
        num_of_constr = 0;
        constr = NULL;
    }else{
//        constr = get_attr_constr(&num_of_constr, type, attr_size, ptr);
        constr = get_attr_constraints(&num_of_constr, type, ptr);
//        if(constr == NULL){
//            fprintf(stderr, "(catalog.c/get_attr) (struct attr_constraint**)constr is unexpectedly NULL\n");
//            exit(0);
//        }
        printf("nothing hahaha\n");
    }
    a_data->attr_size = attr_size;
    a_data->type = type;
    a_data->attr_name = attr_name;
    a_data->num_of_constr = num_of_constr;
    a_data->constr = constr;

//    printf("a_name=%s, a_type=%d, a_size=%d, a_constr_count=%d\n", a_data->attr_name,
//           (int)(a_data->type), a_data->attr_size, a_data->num_of_constr);


    return a_data;
}

int catalog_add_attributes(struct catalog_table_data* t_data, char* data_str){
    struct hashtable* attr_ht = ht_create(12, 0.75);
    char** str_arr;
    int count = split(&str_arr, data_str, ',');

    char* key_word;
    enum db_type type;

    for(int i = 0; i < count; i++){

        char* kw_end_ptr = strchr(str_arr[i], '(');

        if(kw_end_ptr != NULL){
            int kw_end_index = kw_end_ptr - str_arr[i] - 1;
            int kw_start_index = 0;
            key_word = substring(str_arr[i], kw_start_index, kw_end_index);
            /*check if the s_str(sub string) is a keyword*/
            type = typeof_kw(key_word);
            free(key_word);

            if(type == UNKNOWN) {
                //if the keyword is UNKNOWN/invalid then it has to be a attribute
                struct attr_data* a_data = get_attr(str_arr[i]);
                sv_ht_add(attr_ht, a_data->attr_name, a_data);
//                printf("create attr_data\n");

            }
        }else{
            //if there are no parentheses then it is an attribute
            struct attr_data* a_data = get_attr(str_arr[i]);
//            printf("create attr_data\n");
//            struct attr_data* a_data = get_attr(str_arr[i]);
            sv_ht_add(attr_ht, a_data->attr_name, a_data);
        }

    }
    free_2d_char(str_arr, count);
    t_data->attr_ht = attr_ht;

}

int catalog_add_table(int table_num, char* table_name, char* data_str){
    printf("data str is : %s\n", data_str);
    struct catalog_table_data* t_data = malloc(sizeof(struct catalog_table_data));
    t_data->table_num = table_num;

    int table_name_len = strlen(table_name);
    t_data->table_name = malloc(sizeof(table_name_len));
    strncpy(t_data->table_name, table_name, table_name_len + 1);

    /*Add the attributes*/
    catalog_add_attributes(t_data, data_str);

    sv_ht_add(table_ht, table_name, t_data);

    printf("\n\nprint the tables\n");
    struct catalog_table_data* t_data_2 = sv_ht_get(table_ht, table_name);
    printf("table=%s, table_num=%d\n", t_data_2->table_name, t_data_2->table_num );

    struct ht_node** node_list = t_data_2->attr_ht->node_list;
    for(int i = 0; i < t_data_2->attr_ht->size; i++){
        struct attr_data* attr_data = node_list[i]->value->v_ptr;
        printf("a_name=%s, a_type=%d, a_size=%d, a_constr_count=%d\n", attr_data->attr_name,
               (int)(attr_data->type), attr_data->attr_size, attr_data->num_of_constr);
    }
}




int catalog_remove_table(char* table_name){

    return 0;
}

void catalog_print_tables(){
    struct ht_node** node_list = table_ht->node_list;

    for(int i = 0; i < table_ht->size; i++){
        struct catalog_table_data* t_data = node_list[i]->value->v_ptr;
        printf("Table:%d - %s\n", t_data->table_num, t_data->table_name);
        printf("\n\n");
    }
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


