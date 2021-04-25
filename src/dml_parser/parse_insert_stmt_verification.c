//
// Created by SGuan on 4/24/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../include/parse_insert_stmt_verification.h"
#include "../../include/catalog/catalog.h"

int verify_table_exist(char* func_loc_str, char* table_name){
    // verify that the table exist
    printf("...Verifying that table \"%s\" exist\n", table_name);
    if(catalog_contains(table_name) == 0){
        printf("Verification failed: Table \"%s\" does not exist (%s)\n", table_name, func_loc_str);
        return -1;
    }
    printf("...Verification success: Table \"%s\" exist \n", table_name);

    return 0;
}

int verify_build_records_parameters(char* table_name, char** tuple_str_arr, int num_of_tuple){
    char func_loc_str[] = "(parse_insert_stmt.c/build_records)";

    /*Verify that the table exist*/
    if(catalog_contains(table_name) == 0){
        printf("%s %s \"%s\"\n", func_loc_str, "Irrecoverable error: Cannot find table", table_name);
        exit(0);
    }

    /*Verify that the num_of_tuple is greater than 0*/
    if(num_of_tuple < 1){
        printf("%s %s\n", func_loc_str, "Irrecoverable error: Expected atleast one tuple but received none");
        printf("%s %s : %d\n", func_loc_str, "Number of tuple", num_of_tuple);
        exit(0);
    }

    /*Verify that the tuple_str_arr is not NULL */
    if(tuple_str_arr == NULL){
        printf("%s %s\n", func_loc_str, "Irrecoverable error: tuple_str_arr is NULL");
        exit(0);
    }

    return 0;
}

int verify_record_is_valid(struct catalog_table_data* t_data, union record_item* record){
    /*
     * Things to check
     * - if a attribute/column/value have a unique constraint
     * - if the table has foreign keys
     */

    return 0;
}

int verify_attr_meta_data_nodes(struct attr_data** attr_data_arr, int num_of_attr){
    char func_loc_str[] = "(parse_insert_stmt.c/build_records)";

    /*Verify that num_of_attr is not <= 0*/
    if(num_of_attr <= 0){
        fprintf(stderr,"%s %s %d\n", func_loc_str, "Error: Expected atleast one attribute but got", num_of_attr);
        exit(0);
    }

    if(attr_data_arr == NULL){
        fprintf(stderr,"%s %s\n", func_loc_str, "Error: Expected a non null attr_data_arr but got null");
        exit(0);
    }

    /*Print out the attribute names*/
    printf("%s Verifying attributes\n", func_loc_str);
    for(int i = 0; i < num_of_attr; i++){
        struct attr_data* a_data = attr_data_arr[i];
        printf("%s ...Verified attribute \"%s\"\n", func_loc_str, a_data->attr_name);
    }
    printf("%s Successfully verified all the attributes\n", func_loc_str);


    return 0;
}

int verify_parsed_val_str(char** value_str_arr, int num_of_vals, int num_of_attrs){
    char func_loc_str[] = "(parse_insert_stmt.c/convert_to_records)";
    if(value_str_arr == NULL){
        fprintf(stderr, "%s %s\n", func_loc_str, "Error: value_str_arr is NULL");
        exit(0);
    }

    if(num_of_vals != num_of_attrs){
        printf( "%s Error: number of value strings (%d count) does not match number of attributes (%d count)\n", func_loc_str,
                num_of_vals, num_of_attrs);
        return -1;
    }

    return 0;

}


int verify_attr_types_exist(enum db_type* attr_types, int num_of_attrs){
    char func_loc_str[] = "(parse_insert_stmt.c/verify_attr_types_exist)";
    if(attr_types == NULL){
        printf("%s %s\n", func_loc_str, "Error: attr_type is invalid (it is NULL)");
        exit(0);
    }

    if(num_of_attrs <= 0){
        printf("%s %s %d\n", func_loc_str, "Error: Expected atleast 1 attribute but num_of_attr is", num_of_attrs);
        exit(0);
    }
    printf("%s %s\n", func_loc_str, "Printing attribute types");
    for(int i = 0; i < num_of_attrs; i++){
        switch(attr_types[i]){
            case INT:
                printf("...attr_%d = int\n", i);
                break;
            case DOUBLE:
                printf("...attr_%d = double\n", i);
                break;
            case BOOL:
                printf("...attr_%d = bool\n", i);
                break;
            case CHAR:
                printf("...attr_%d = char\n", i);
                break;
            case VARCHAR:
                printf("...attr_%d = varchar\n", i);
                break;
            default:
                printf("%s %s \"%s\"\n", func_loc_str, "Error: Invalid attribute type:",type_to_str(attr_types[i]));
                exit(0);
        }
    }

}
