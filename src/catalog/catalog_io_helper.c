//
// Created by SGuan on 4/10/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/hash_collection/hash_collection.h"
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
    catalog_write_foreign_keys(catalog_file, t_data->num_of_f_key, t_data->f_key_arr_size, t_data->f_keys);

    /* write the child tables*/
    catalog_write_childs(catalog_file, t_data->num_of_childs,  t_data->child_arr_size, t_data->childs);

    return 0;
}

int catalog_write_attr_constr(FILE* catalog_file, int num_of_constr, struct attr_constraint** constr){
    char func_loc_str[] = "(catalog_io_helper.c/catalog_write_attr_constr)";
    /* write out the total number of constraints */
    printf("%s writing out %d constraint\n", func_loc_str, num_of_constr);
    fwrite(&num_of_constr, sizeof(int), 1, catalog_file);


    for(int i = 0; i < num_of_constr; i++){
        enum db_type type = constr[i]->type;
        int type_int_val = (int)type;
//        char* type_str = type_to_str(type);

        /* write out the int val of the data type */
        fwrite(&(type_int_val), sizeof(int), 1, catalog_file);
//        if(type == DEFAULT){
//            // if the type is default then write out the default value too
//
//
//        }
    }
    return 0;
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
        fwrite(&a_name_len, sizeof(int), 1, catalog_file);
        fwrite(a_data->attr_name, sizeof(char), a_name_len, catalog_file);

//        /* write out the attribute constraints */
//
        catalog_write_attr_constr(catalog_file, a_data->num_of_constr, a_data->constr);
    }

    return 0;
}



int catalog_write_primary_key(FILE* catalog_file, int p_key_len, char** primary_key_attrs){
    fwrite(&p_key_len, sizeof(int), 1, catalog_file);

    if(p_key_len == 0){
        return 0;
    }

    for(int i = 0; i < p_key_len; i++){
        int a_name_len = strlen(primary_key_attrs[i]);
        fwrite(&a_name_len, sizeof(int), 1, catalog_file);
        fwrite(primary_key_attrs[i], sizeof(char), strlen(primary_key_attrs[i]), catalog_file);
    }

    return 0;
}

int catalog_write_foreign_keys(FILE* catalog_file, int num_of_keys, int f_key_arr_size, struct foreign_key_data** f_keys){
    fwrite(&num_of_keys, sizeof(int), 1, catalog_file);
    fwrite(&f_key_arr_size, sizeof(int), 1, catalog_file);

    if(num_of_keys == 0){
        return 0;
    }

    for(int i = 0; i < num_of_keys; i++){

        char* parent_table_name = f_keys[i]->parent_table_name;
        int p_table_name_len = strlen(parent_table_name);
        fwrite(&p_table_name_len, sizeof(int), 1, catalog_file);
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

            int f_key_attr_j_len = strlen(f_key_attr_j);
            int p_key_attr_j_len = strlen(p_key_attr_j);

            fwrite(&f_key_attr_j_len, sizeof(int), 1, catalog_file);
            fwrite(&p_key_attr_j_len, sizeof(int), 1, catalog_file);

            fwrite(f_key_attr_j, sizeof(char), strlen(f_key_attr_j), catalog_file);
            fwrite(p_key_attr_j, sizeof(char), strlen(p_key_attr_j), catalog_file);
        }
    }
    return 0;
}

int catalog_write_childs(FILE* catalog_file, int num_of_childs, int child_arr_size, char** childs){
    fwrite(&num_of_childs, sizeof(int), 1, catalog_file);
    fwrite(&child_arr_size, sizeof(int), 1, catalog_file);

    if(num_of_childs == 0){
        return 0;
    }


    for(int i = 0; i < num_of_childs; i++){
        int c_name_len = strlen(childs[i]);
        fwrite(&c_name_len, sizeof(int), 1, catalog_file);
        fwrite(childs[i], sizeof(char), c_name_len, catalog_file);
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
    int f_key_arr_size;
    struct foreign_key_data** f_keys;
    result = catalog_read_foreign_keys(catalog_file, &f_key_nums, &f_key_arr_size, &f_keys);

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
    (*t_data)->f_key_arr_size = f_key_arr_size;
    (*t_data)->f_keys = f_keys;
    (*t_data)->num_of_childs = num_of_childs;
    (*t_data)->child_arr_size = child_arr_size;
    (*t_data)->childs = childs;

    return result;

}

int catalog_read_attr_constr(FILE* catalog_file, int* num_of_constr, struct attr_constraint*** constr){
    char func_loc_str[] = "(catalog_io_helper.c/catalog_read_attr_constr)";

    /* read in the number of constraint */
    int count;
    fread(&count, sizeof(int), 1, catalog_file);
    *num_of_constr = count;
    printf("%s Read in %d constraints\n", func_loc_str, *num_of_constr);


    *constr = malloc(sizeof(struct attr_constraint*) * 5);

    if(count == 0){
        return 0;
    }



    for(int i = 0; i < count; i++){
        int type_int_val;
        fread(&type_int_val, sizeof(int), 1, catalog_file);

        enum db_type type = (enum db_type)type_int_val;
        (*constr)[i] = malloc(sizeof(struct attr_constraint));
        (*constr)[i]->type = type;
        (*constr)[i]->value = NULL;
    }

    return 0;
}

int catalog_read_attr(FILE* catalog_file, struct hashtable** attr_ht){
    int num_of_attrs;
    fread(&num_of_attrs, sizeof(int), 1, catalog_file);

    if(num_of_attrs == 0){
        *attr_ht = ht_create(12, 0.75);
        return 0;
    }

    /* Read in the attribute hashtable's capacity and base multiplier */
    int capacity;
    int base_multiplier;
    fread(&capacity, sizeof(int), 1, catalog_file);
    fread(&base_multiplier, sizeof(int), 1, catalog_file);

    /* Initialize the attribute hashtable */
    *attr_ht = ht_create(capacity, 0.75);
    (*attr_ht)->base_multiplier = base_multiplier;

    for(int i = 0; i < num_of_attrs; i++){
        struct attr_data* a_data = malloc(sizeof(struct attr_data));
        fread(&(a_data->index), sizeof(int), 1, catalog_file);
        fread(&(a_data->attr_size), sizeof(int), 1, catalog_file);


        int attr_type_int;
        fread(&attr_type_int, sizeof(int), 1, catalog_file);
        enum db_type attr_type = (enum db_type)attr_type_int;
        a_data->type = attr_type;

        int a_name_len;
        fread(&a_name_len, sizeof(int), 1, catalog_file);
        char* a_name = malloc(a_name_len + 1);
        a_name[0] = 0;
        fread(a_name, sizeof(char), a_name_len, catalog_file);
        a_name[a_name_len] = 0;
        a_data->attr_name = a_name;

        int num_of_constr;
        struct attr_constraint** constr;
        catalog_read_attr_constr(catalog_file, &num_of_constr, &constr);

        a_data->num_of_constr = num_of_constr;
        a_data->constr = constr;

        sv_ht_add(*attr_ht, a_data->attr_name, a_data);
    }

    return 0;

}

int catalog_read_primary_key(FILE* catalog_file, int* p_key_len, char*** primary_key_attrs){
    int len;
    fread(&len, sizeof(int), 1, catalog_file);
    *p_key_len = len;


    *primary_key_attrs = malloc(sizeof(char*) * (*p_key_len));

    if(*p_key_len == 0){

        return 0;
    }


    for(int i = 0; i < *p_key_len; i++){
        int a_name_len;
        fread(&a_name_len, sizeof(int), 1, catalog_file);
        char* p_key_attr = malloc(a_name_len + 1);
        fread(p_key_attr, sizeof(char), a_name_len, catalog_file);
        p_key_attr[a_name_len] = 0;

        (*primary_key_attrs)[i] = p_key_attr;
    }

    return 0;
}

int catalog_read_foreign_keys(FILE* catalog_file, int* num_of_keys, int* f_key_arr_size, struct foreign_key_data*** f_keys){

    fread(&(*num_of_keys), sizeof(int), 1, catalog_file);
    fread(&(*f_key_arr_size), sizeof(int), 1, catalog_file);

    /*Initialize f_keys*/
    *f_keys = malloc(sizeof(struct foreign_key_data*) * (*f_key_arr_size));

    if(*num_of_keys == 0){
        return 0;
    }



    for(int i = 0; i < *num_of_keys; i++){
        (*f_keys)[i] = malloc(sizeof(struct foreign_key_data));

        /* Get parent table */
        int p_table_name_len;
        fread(&p_table_name_len, sizeof(int), 1, catalog_file);

        char* p_table_name = malloc(p_table_name_len + 1);
        fread(p_table_name, sizeof(char), p_table_name_len, catalog_file);
        p_table_name[p_table_name_len] = 0;

        (*f_keys)[i]->parent_table_name = p_table_name;


        /* Get the number of attribute pairs */
        int num_of_pairs;
        fread(&num_of_pairs, sizeof(int), 1, catalog_file);

        /* Get the specs of the hashtable for storing <f_key_attr, p_key_attr> pairs */
        int capacity;
        int base_multiplier;
        fread(&capacity, sizeof(int), 1, catalog_file);
        fread(&base_multiplier, sizeof(int), 1, catalog_file);

        struct hashtable* ht = ht_create(capacity, 0.75);
        ht->base_multiplier = base_multiplier;

        /* Get all the attribute pairs */
        for(int j = 0; j < num_of_pairs; j++){
            int f_key_attr_j_len;
            int p_key_attr_j_len;
            fread(&f_key_attr_j_len, sizeof(int), 1, catalog_file);
            fread(&p_key_attr_j_len, sizeof(int), 1, catalog_file);

            char* f_key_attr_j = malloc(f_key_attr_j_len + 1);
            f_key_attr_j[0] = 0;
            char* p_key_attr_j = malloc(p_key_attr_j_len + 1);
            p_key_attr_j[0] = 0;

            fread(f_key_attr_j, sizeof(char), f_key_attr_j_len, catalog_file);
            f_key_attr_j[f_key_attr_j_len] = 0;

            fread(p_key_attr_j, sizeof(char), p_key_attr_j_len, catalog_file);
            p_key_attr_j[p_key_attr_j_len] = 0;

            sv_ht_add(ht, f_key_attr_j, p_key_attr_j);
            free(f_key_attr_j);
        }

        (*f_keys)[i]->f_keys = ht;

    }

    return 0;

}

int catalog_read_childs(FILE* catalog_file, int* num_of_childs, int* child_arr_size, char*** childs){
    fread(&(*num_of_childs), sizeof(int), 1, catalog_file);
    fread(&(*child_arr_size), sizeof(int), 1, catalog_file);

    *childs = malloc(sizeof(char*) * (*child_arr_size));

    if((int)(*num_of_childs) == 0){
        return 0;
    }

    for(int i = 0; i < (int)(*num_of_childs); i++){
        int c_name_len;
        fread(&c_name_len, sizeof(int), 1, catalog_file);
        char* child_table_name = malloc(c_name_len + 1);
        child_table_name[0] = 0;
        fread(child_table_name, sizeof(char), c_name_len, catalog_file);
        child_table_name[c_name_len] = 0;
        (*childs)[i] = child_table_name;
    }

    return 0;
}

