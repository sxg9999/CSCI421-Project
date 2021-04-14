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
    catalog_add_table(t_data);
}

int sm_drop_table();

int sm_alter_table();
