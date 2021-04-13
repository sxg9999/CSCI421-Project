//
// Created by SGuan on 4/10/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/catalog/catalog_io_helper.h"


int catalog_write_table(FILE* catalog_file, struct catalog_table_data* t_data){
    /* write the table num and table name*/
    fwrite(&(t_data->table_num), sizeof(int), 1, catalog_file);

    int t_name_len = strlen(t_data->table_name);
    fwrite(&t_name_len, sizeof(int), 1, catalog_file );
    fwrite(t_data->table_name, sizeof(char), t_name_len, catalog_file);

    /* write the attributes */
    catalog_write_attr(catalog_file, t_data->attr_ht);

    /* write the primary key */
    catalog_write_primary_key(catalog_file, t_data->p_key_len, t_data->primary_key_attrs);

    /* write the foreign key */
    catalog_write_foreign_keys(catalog_file, t_data->num_of_f_key, t_data->f_keys);

    /* write the child tables*/
    catalog_write_childs(catalog_file, t_data->num_of_childs,  t_data->child_arr_size, t_data->childs);


}

int catalog_write_attr_constr(FILE* catalog_file, int num_of_constr, struct attr_constraint** constr){

    /* write out the total number of constraints */
    fwrite(&(num_of_constr), sizeof(int), 1, catalog_file);

    for(int i = 0; i < num_of_constr; i++){
        enum db_type type = constr[i]->type;
        int type_int_val = (int)type;
        char* type_str = type_to_str(type);

        /* write out the int val of the data type */
        fwrite(&(type_int_val), sizeof(int), 1, catalog_file);
//        if(type == DEFAULT){
//            // if the type is default then write out the default value too
//
//        }
    }
}


int catalog_write_attr(FILE* catalog_file, struct hashtable* attr_ht){
    /* write out the total number of attributes */
    fwrite(&(attr_ht->size), sizeof(int), 1, catalog_file);

    if(attr_ht->size == 0){
        return 0;
    }

    /* write out the hashtable's capacity  and base multiplier*/
    fwrite(&(attr_ht->capacity), sizeof(int), 1, catalog_file);
    fwrite(&(attr_ht->base_multiplier), sizeof(int), 1, catalog_file);

    struct ht_node** attrs = attr_ht->node_list;

    for(int i = 0; i < attr_ht->size; i++){
        struct attr_data* a_data = attrs[i]->value->v_ptr;
        fwrite(&(a_data->index), sizeof(int), 1, catalog_file);
        fwrite(&(a_data->attr_size), sizeof(int), 1, catalog_file);

        int type_int_val = (int)(a_data->type);
        fwrite(&(type_int_val), sizeof(int), 1, catalog_file);

        int a_name_len = strlen(a_data->attr_name);
        fwrite(a_data->attr_name, sizeof(char), a_name_len, catalog_file);

        /* write out the attribute constraints */

        catalog_write_attr_constr(catalog_file, a_data->num_of_constr, a_data->constr);
    }
}



int catalog_write_primary_key(FILE* catalog_file, int p_key_len, char** primary_key_attrs){
    fwrite(&p_key_len, sizeof(int), 1, catalog_file);

    if(p_key_len == 0){
        return 0;
    }

    for(int i = 0; i < p_key_len; i++){
        fwrite(primary_key_attrs[i], sizeof(char), strlen(primary_key_attrs[i]), catalog_file);
    }

    return 0;
}

int catalog_write_foreign_keys(FILE* catalog_file, int num_of_keys, struct foreign_key_data** f_keys){
    fwrite(&num_of_keys, sizeof(int), 1, catalog_file);

    if(num_of_keys == 0){
        return 0;
    }

    for(int i = 0; i < num_of_keys; i++){
        char* parent_table_name = f_keys[i]->parent_table_name;
        fwrite(parent_table_name, sizeof(char), strlen(parent_table_name), catalog_file);

        struct ht_node** key_pairs = f_keys[i]->f_keys->node_list;

        /* Write out the number of pairs */
        fwrite(&(f_keys[i]->f_keys->size), sizeof(int), 1, catalog_file);

        /* Write out the other specs of the hashtable: capacity, base multiplier */
        fwrite(&(f_keys[i]->f_keys->capacity), sizeof(int), 1, catalog_file);
        fwrite(&(f_keys[i]->f_keys->base_multiplier), sizeof(int), 1, catalog_file);


        for(int j = 0; j < f_keys[i]->f_keys->size; j++){
            char* f_key_attr_j = key_pairs[j]->key;
            char* p_key_attr_j = key_pairs[j]->value->v_ptr;

            fwrite(f_key_attr_j, sizeof(char), strlen(f_key_attr_j), catalog_file);
            fwrite(p_key_attr_j, sizeof(char), strlen(p_key_attr_j), catalog_file);
        }
    }
}

int catalog_write_childs(FILE* catalog_file, int num_of_childs, int child_arr_size, char** childs){
    fwrite(&num_of_childs, sizeof(int), 1, catalog_file);

    if(num_of_childs == 0){
        return 0;
    }

    fwrite(&child_arr_size, sizeof(int), 1, catalog_file);

    for(int i = 0; i < num_of_childs; i++){
        fwrite(childs[i], sizeof(char), strlen(childs[i]), catalog_file);
    }
    return 0;
}




int catalog_read_table(FILE* catalog_file, struct catalog_table_data** t_data){
    int result = -1;

    *t_data = malloc(sizeof(struct catalog_table_data));

    int table_num;
    int table_name_len;
    fread(&table_num, sizeof(int), 1, catalog_file);
    fread(&table_name_len, sizeof(int), 1, catalog_file);

    char* table_name = malloc(table_name_len+1);
    fread(table_name, sizeof(char), table_name_len, catalog_file);
    table_name[table_name_len] = 0;

    struct hashtable* attr_ht;
    result = catalog_read_attr(catalog_file, &attr_ht);

    int p_key_len;
    char** primary_key_attrs;
    result = catalog_read_primary_key(catalog_file, &p_key_len, &primary_key_attrs);

    int f_key_nums;
    struct foreign_key_data** f_keys;
    result = catalog_read_foreign_keys(catalog_file, &f_key_nums, &f_keys);

    int num_of_childs;
    int child_arr_size;
    char** childs;
    result = catalog_read_childs(catalog_file, &num_of_childs, &child_arr_size, &childs);

    (*t_data)->table_num = table_num;
    (*t_data)->table_name = table_name;
    (*t_data)->attr_ht = attr_ht;
    (*t_data)->p_key_len = p_key_len;
    (*t_data)->primary_key_attrs = primary_key_attrs;
    (*t_data)->num_of_f_key = f_key_nums;
    (*t_data)->f_keys = f_keys;
    (*t_data)->num_of_childs = num_of_childs;
    (*t_data)->child_arr_size = child_arr_size;
    (*t_data)->childs = childs;



}

int catalog_read_attr_constr(FILE* catalog_file, int* num_of_constr, struct attr_constraint*** constr){

    /* read in the number of constraint */
    int count;
    fread(&count, sizeof(int), 1, catalog_file);

    *constr = malloc(sizeof(struct attr_constraint*) * count);


//    for(int i = 0; i < count; i++){
//
//    }
}

int catalog_read_attr(FILE* catalog_file, struct hashtable** attr_ht){
    

}

int catalog_read_primary_key(FILE* catalog_file, int* p_key_len, char*** primary_key_attrs){
    return 0;
}

int catalog_read_foreign_keys(FILE* catalog_file, int* num_of_keys, struct foreign_key_data*** f_keys){
    return 0;

}

int catalog_read_childs(FILE* catalog_file, int* num_of_childs, int* child_arr_size, char*** childs){
    return 0;
}

