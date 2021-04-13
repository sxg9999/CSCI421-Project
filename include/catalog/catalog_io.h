//
// Created by SGuan on 4/7/2021.
//

#ifndef CATALOG_IO_H
#define CATALOG_IO_H

#include "../hash_collection/hash_table.h"


int read_catalog(char* file_path, struct hashtable** table_ht);

int write_catalog(char* file_path, struct hashtable* table_ht);

#endif
