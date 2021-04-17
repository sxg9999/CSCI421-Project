//
// Created by SGuan on 4/10/2021.
//

#include <stdio.h>
#include <stdlib.h>

#include "../../include/hash_collection/hash_collection.h"
#include "../../include/catalog/catalog_io.h"
#include "../../include/catalog/catalog_io_helper.h"

int read_catalog(char* file_path, struct hashtable** table_ht){
    printf("reading catalog ... \n");
    FILE* catalog_file = fopen(file_path, "rb");

    int num_of_tables;
    int capacity;        // capacity of the hashtable that contains the table information
    int base_multiplier;

    fread(&num_of_tables, sizeof(int), 1, catalog_file);
    fread(&capacity, sizeof(int), 1, catalog_file);
    fread(&base_multiplier, sizeof(int), 1, catalog_file);

    *table_ht = ht_create(capacity,0.75);
    (*table_ht)->base_multiplier = base_multiplier;

    for(int i = 0; i < num_of_tables; i++){
        struct catalog_table_data* t_data;
        int result = catalog_read_table(catalog_file, &t_data);

        if(result == 0){
            sv_ht_add(*table_ht, t_data->table_name, t_data);
        }else{
            fprintf(stderr, "(catalog_io.c/read_catalog) %s",
                    "Expected to read in a table but cannot read it!!!\n");
        }

    }
    fclose(catalog_file);

    printf("reading catalog completed\n");

    return 0;

}

int write_catalog(char* file_path, struct hashtable* table_ht){
    printf("writing catalog ...\n");
    FILE* catalog_file = fopen(file_path, "wb");

    printf("table size is : %d\n", table_ht->size);
    /* write out the hashtable's size (amount of tables currently in the hashtable) */
    fwrite(&(table_ht->size), sizeof(int), 1, catalog_file);

    if(table_ht->size == 0){
        printf("There are no tables ... writing nothing to file\n");
        return 0;
    }

    printf("table size is : %d\n", table_ht->size);

    /* write out the hashtable's (table_ht) capacity*/
    fwrite(&(table_ht->capacity), sizeof(int), 1, catalog_file);
    /* write out the hashtable's base multiplier */
    fwrite(&(table_ht->base_multiplier), sizeof(int), 1, catalog_file);


    struct ht_node** table_nodes = table_ht->node_list;

    for(int i = 0; i < table_ht->size; i++){
        struct catalog_table_data* t_data = table_nodes[i]->value->v_ptr;
        catalog_write_table(catalog_file, t_data);

    }

    fclose(catalog_file);
    printf("writing catalog completed...\n");
    return 0;
}