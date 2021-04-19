//
// Created by SGuan on 4/16/2021.
//

#ifndef CATALOG_CLEAN_UP_H
#define CATALOG_CLEAN_UP_H

#include "catalog_structs.h"

int catalog_free_f_key(struct foreign_key_data* f_key_data);

void catalog_free_t_data(struct catalog_table_data* t_data);

#endif
