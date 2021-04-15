//
// Created by SGuan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/catalog/catalog_structs.h"
#include "../../include/catalog/catalog.h"
#include "../../include/storagemanager.h"

#include "../../include/storage_mediator/storage_mediator.h"
#include "../../include/storage_mediator/storage_mediator_helper.h"


int sm_add_table(struct catalog_table_data* t_data){
    /* Get attribute data types  */
    int* data_types = NULL;
    int data_type_size = catalog_get_data_types(t_data, &data_types);

    /* Get primary key indices */
    int* p_key_indices = NULL;
    int p_key_size = 0;

    /* Add the table to storage manager */
    int table_num = add_table(data_types, p_key_indices, data_type_size, p_key_size);
    t_data->table_num = table_num;

    /* Add the table to catalog */
    catalog_add_table(t_data);

    return 0;
}

int sm_drop_table(char* table_name){
    catalog_remove_table(table_name);
    return 0;
}

int sm_alter_table();
