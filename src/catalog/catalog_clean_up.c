//
// Created by SGuan on 4/16/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_collection/sv_ht.h"
#include "../hash_collection/hash_table.h"

#include "../../include/catalog/catalog_clean_up.h"

int catalog_free_f_key(struct foreign_key_data* f_key_data){

    if(f_key_data == NULL){
        printf("...Error: foreign_key_data is NULL\n");
        return -1;
    }

    sv_ht_print(f_key_data->f_keys);

    free(f_key_data->parent_table_name);
    printf("...Freed parent table name for child table.\n");

    if(f_key_data->f_keys == NULL){
        printf("...Error: foreign_key_ht is NULL\n");
        return -1;
    }
    /* Free the hashtable */
    int size = f_key_data->f_keys->size;
    printf("...Size of foreign key hashtable is : %d\n", size);

//    printf("size = %d\n", size);
    printf("...Getting foreign key hashtable's nodes\n");
    struct ht_node** val_nodes = f_key_data->f_keys->node_list;
    printf("...Obtained foreign key hashtable's nodes\n");
    printf("...Freeing foreignkey hashtable's nodes\n");
    for(int i = 0; i < size; i++){
        printf("......Freeing reference to attribute \"%s\"\n", (char*)(val_nodes[i]->value->v_ptr));
        free(val_nodes[i]->value->v_ptr);
        val_nodes[i]->value->v_ptr == NULL;
        f_key_data->f_keys->size = 0;
    }
    printf("...Freed foreign key hashtable's nodes\n");
    printf("...Freeing foreign key hashtable\n");
    destroy_sv_ht(f_key_data->f_keys);
    printf("...Freed foreign key hashtable\n");
    printf("...Freeing foreign key data struct\n");
    free(f_key_data);
    printf("...Freed foreign key data struct\n");
    return 0;

}

void catalog_free_t_data(struct catalog_table_data* t_data){

}