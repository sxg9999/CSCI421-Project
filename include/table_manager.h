/*
 *
 */
#ifndef TABLES_H
#define TABLES_H

#include <stdlib.h>
#include <stdio.h>
#include "table.h"

#define TABLES_META_DATA "tables_meta_data.oo"


int t_add_table(int* data_types, int* key_indices, int data_types_size, int key_indices_size);

int save_table(Table* newTable);

int clear_table(int table_id);

#endif TABLES_H