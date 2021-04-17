//
// Created by SGuan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/hash_collection/hash_table.h"

#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/catalog/catalog_printer.h"



void catalog_print_tables(){

    struct hashtable* table_ht = catalog_get_ht();

    struct ht_node** node_list = table_ht->node_list;
    for(int i = 0; i < table_ht->size; i++){
        struct catalog_table_data* t_data = node_list[i]->value->v_ptr;
        printf("Table:%d - %s\n", t_data->table_num, t_data->table_name);

        catalog_print_childs(t_data->num_of_childs, t_data->childs);
        //print the attributes and its constraints
        struct ht_node** attr_node_list = t_data->attr_ht->node_list;
        int num_of_attr = t_data->attr_ht->size;

        for(int j = 0; j < num_of_attr; j++){
            struct attr_data* a_data = attr_node_list[j]->value->v_ptr;
            char* type_str = type_to_str(a_data->type);
            int constr_count = a_data->num_of_constr;

            printf("a_name=%s, a_type=%s, a_size=%d, a_constr_count=%d, constraints=[ ", a_data->attr_name,
                   type_str, a_data->attr_size, a_data->num_of_constr);


            for(int k = 0; k < constr_count; k++){
                struct attr_constraint* a_constr = a_data->constr[k];
                char* constr_type_str = type_to_str(a_constr->type);

                if(a_constr->type == DEFAULT){
                    switch(a_data->type){
                        case INT:
                            printf("%s(%d), ", constr_type_str, a_constr->value->i);
                            break;
                        case DOUBLE:
                            printf("%s(%lf), ", constr_type_str, a_constr->value->d);
                            break;
                        case BOOL:
                            if(a_constr->value->b == true){
                                printf("%s(true), ", constr_type_str);
                            }else{
                                printf("%s(false), ", constr_type_str);
                            }
                            break;
                        case CHAR:
                            printf("%s(%s), ", constr_type_str, a_constr->value->c);
                            break;
                        case VARCHAR:
                            printf("%s(%s), ", constr_type_str, a_constr->value->v);
                            break;
                        default:
                            fprintf(stderr, "(catalog.c/catalog_print_table) Got an unexpected constraint type!!!\n");
                            exit(0);
                    }
                }else{
                    printf("%s, ", constr_type_str);
                }

            }
            printf("]\n");

            /*print the primary key*/
        }

        printf("primary_key=[ ");
        for(int j = 0; j < t_data->p_key_len; j++){
            printf("%s, ", t_data->primary_key_attrs[j]);
        }
        printf("]\n");

        /*Print the foreign keys*/
        catalog_print_foreign_key(t_data->num_of_f_key, t_data->f_keys);



        printf("\n\n");
    }


}

void catalog_print_foreign_key(int count, struct foreign_key_data** f_keys){

    printf("foreign Keys:\n");
    if(count == 0){
        printf("(No foreign keys)\n");
    }
    for(int i = 0; i < count; i++){
        char* referenced_table_name = f_keys[i]->parent_table_name;


        struct hashtable* f_keys_mapping = f_keys[i]->f_keys;
        int num_of_mapping = f_keys_mapping->size;
        struct ht_node** f_key_attrs = f_keys_mapping->node_list;


        printf("(foreign_key_%d) ref_table=%s", i, referenced_table_name);

        for(int j = 0; j < num_of_mapping; j++){
            printf(", %s=%s", f_key_attrs[i]->key, (char*)(f_key_attrs[i]->value->v_ptr));
        }

        printf("\n");

    }
}

void catalog_print_childs(int count, char** childs){
    printf("childs: [");
    for(int i = 0; i < count; i++){
        printf("%s, ", childs[i]);
    }
    printf("]\n");
}
