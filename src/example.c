//
// Created by SGuan on 4/18/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/catalog/catalog_structs.h"
#include "../include/storage_mediator/storage_mediator.h"
#include "../include/hash_collection/hash_table.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/hash_collection/sv_ht.h"

#include "../../include/example.h"

static int table_count = 0;

struct catalog_table_data* get_department_table_data(){
    struct catalog_table_data* depart_t_data = malloc(sizeof(struct catalog_table_data));
    depart_t_data->table_num = table_count;
    table_count += 1;

    char t_name[] = "department";
    depart_t_data->table_name = malloc(strlen(t_name) + 1);
    strncpy(depart_t_data->table_name, (char*)t_name, strlen(t_name)+1);

    struct hashtable* attr_ht = ht_create(12, 0.75);

    struct attr_data* id_attr = malloc(sizeof(struct attr_data));
    id_attr->index = 0;
    id_attr->attr_size = sizeof(int);
    id_attr->type = INT;
    id_attr->attr_name = malloc(3);
    strncpy(id_attr->attr_name, "id", 3);
    id_attr->num_of_constr = 0;
    id_attr->constr = NULL;

    struct attr_data* dept_name_attr = malloc(sizeof(struct attr_data));
    dept_name_attr->index = 1;
    dept_name_attr->attr_size = 20;
    dept_name_attr->type = CHAR;
    char dept_name_str[] = "dept_name";
    dept_name_attr->attr_name = malloc(strlen(dept_name_str) + 1);
    strncpy(dept_name_attr->attr_name, dept_name_str, strlen(dept_name_str) + 1);
    dept_name_attr->num_of_constr = 0;
    dept_name_attr->constr = NULL;

    sv_ht_add(attr_ht, "id", id_attr);
    sv_ht_add(attr_ht, "dept_name", dept_name_attr);

    depart_t_data->attr_ht = attr_ht;

    depart_t_data->p_key_len = 2;
    char** p_key_attrs = malloc(sizeof(char*) * 2);
    p_key_attrs[0] = malloc(3);
    strncpy(p_key_attrs[0], "id", 3);
    p_key_attrs[1] = malloc(strlen(dept_name_str) + 1);
    strncpy(p_key_attrs[1], dept_name_str, strlen(dept_name_str) + 1);
    depart_t_data->primary_key_attrs = p_key_attrs;

    depart_t_data->num_of_f_key = 0;
    depart_t_data->f_key_arr_size = 0;
    depart_t_data->f_keys = NULL;
    depart_t_data->num_of_childs = 0;
    depart_t_data->child_arr_size = 0;
    depart_t_data->childs = NULL;

    return depart_t_data;

}


struct catalog_table_data* get_student_table_data(){
    struct catalog_table_data* student_t_data = malloc(sizeof(struct catalog_table_data));
    student_t_data->table_num = table_count;
    table_count += 1;

    char t_name[] = "student";
    char s_id[] = "s_id";
    char s_name[] = "s_name";
    char s_dept_id[] = "s_dept_id";
    char s_dept_name[] = "s_dept_name";

    student_t_data->table_name = malloc(strlen(t_name) + 1);
    strncpy(student_t_data->table_name, t_name, strlen(t_name) + 1);

    struct hashtable* attr_ht = ht_create(12, 0.75);

    struct attr_data* s_id_attr = malloc(sizeof(struct attr_data));
    s_id_attr->index = 0;
    s_id_attr->attr_size = sizeof(int);
    s_id_attr->type = INT;
    s_id_attr->attr_name = malloc(strlen(s_id) + 1);
    strncpy(s_id_attr->attr_name, s_id, strlen(s_id) + 1);
    s_id_attr->num_of_constr = 0;
    s_id_attr->constr = NULL;

    struct attr_data* s_name_attr = malloc(sizeof(struct attr_data));
    s_name_attr->index = 1;
    s_name_attr->attr_size = strlen(s_name);
    s_name_attr->type = CHAR;
    s_name_attr->attr_name = malloc(strlen(s_name) + 1);
    strncpy(s_name_attr->attr_name, s_name, strlen(s_name) + 1);
    s_name_attr->num_of_constr = 0;
    s_name_attr->constr = NULL;

    struct attr_data* s_dept_id_attr = malloc(sizeof(struct attr_data));
    s_dept_id_attr->index = 2;
    s_dept_id_attr->attr_size = sizeof(int);
    s_dept_id_attr->type = INT;
    s_dept_id_attr->attr_name = malloc(strlen(s_dept_id) + 1);
    strncpy(s_dept_id_attr->attr_name, s_dept_id, strlen(s_dept_id) + 1);
    s_dept_id_attr->num_of_constr = 0;
    s_dept_id_attr->constr = NULL;

    struct attr_data* s_dept_name_attr = malloc(sizeof(struct attr_data));
    s_dept_name_attr->index = 3;
    s_dept_name_attr->attr_size = strlen(s_dept_name);
    s_dept_name_attr->type = CHAR;
    s_dept_name_attr->attr_name = malloc(strlen(s_dept_name) + 1);
    strncpy(s_dept_name_attr->attr_name, s_dept_name, strlen(s_dept_name) + 1);
    s_dept_name_attr->num_of_constr = 0;
    s_dept_name_attr->constr = NULL;


    sv_ht_add(attr_ht, s_id, s_id_attr);
    sv_ht_add(attr_ht, s_name, s_name_attr);
    sv_ht_add(attr_ht, s_dept_id, s_dept_id_attr);
    sv_ht_add(attr_ht, s_dept_name, s_dept_name_attr);

    student_t_data->attr_ht = attr_ht;

    student_t_data->p_key_len = 2;
    char** p_key_attrs = malloc(sizeof(char*) * 2);
    p_key_attrs[0] = malloc(strlen(s_id) + 1);
    strncpy(p_key_attrs[0], s_id, strlen(s_id) + 1);
    p_key_attrs[1] = malloc(strlen(s_name) + 1);
    strncpy(p_key_attrs[1], s_name, strlen(s_name) + 1);
    student_t_data->primary_key_attrs = p_key_attrs;

    student_t_data->num_of_f_key = 1;
    student_t_data->f_key_arr_size = 6;
    student_t_data->f_keys = malloc(sizeof(struct foreign_key_data*) * student_t_data->f_key_arr_size);

    struct foreign_key_data* f_key_data = malloc(sizeof(struct foreign_key_data));
    char p_table_name[] = "department";
    f_key_data->parent_table_name = malloc(strlen(p_table_name) + 1);
    strncpy(f_key_data->parent_table_name, p_table_name, strlen(p_table_name) + 1);

    struct hashtable* f_key_attr_ht = ht_create(12, 0.75);
    char* dept_id = strdup("dept_id");
    char* dept_name = strdup("dept_name");
    sv_ht_add(f_key_attr_ht, s_dept_id, dept_id);
    sv_ht_add(f_key_attr_ht, s_dept_name, dept_name);
    f_key_data->f_keys = f_key_attr_ht;

    student_t_data->f_keys[0] = f_key_data;


    student_t_data->num_of_childs = 0;
    student_t_data->child_arr_size = 0;
    student_t_data->childs = NULL;

    return student_t_data;

}
