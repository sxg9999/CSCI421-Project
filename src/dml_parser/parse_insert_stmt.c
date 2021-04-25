

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../include/parse_insert_stmt.h"
#include "../../include/parse_insert_stmt_helpers.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_attr_helper.h"
#include "../../include/storagemanager.h"
#include "../../include/helper_module/helper_function.h"



int parse_tuple_str(char* str_of_tuple, char*** tuple_str_arr, int* num_of_tuple);

int parse_insert_stmt_kw_into(char* func_loc_str, char* stmt);
int parse_insert_stmt_kw_value(char* func_loc_str);
int parse_insert_stmt_table_name(char* func_loc_str, char** table_name);
int parse_insert_stmt_tuple(char* func_loc_str, int* num_of_tuple, char*** tuple_str_arr);


int parse_insert_stmt(char* insert_stmt) {
    char func_loc_str[] = "(parse_insert_stmt.c/parse_insert_stmt)";
    char* stmt_cpy = strdup(insert_stmt);
    char* stmt_ptr = stmt_cpy + 7; //skip "insert" keyword
    printf("%s %s\n", func_loc_str, "Parsing the insert statement");

    char* table_name = NULL;
    char** tuple_str_arr = NULL;
    int num_of_tuple = 0;


    if(parse_insert_stmt_kw_into(func_loc_str, stmt_ptr) != 0 ||
       parse_insert_stmt_table_name(func_loc_str, &table_name) != 0 ||
       parse_insert_stmt_kw_value(func_loc_str) != 0 ||
       parse_insert_stmt_tuple(func_loc_str, &num_of_tuple, &tuple_str_arr)){
        if(table_name != NULL){
            free(table_name);
            free(stmt_cpy);
        }
        return -1;

    }

    union record_item** records = NULL;
    int num_of_records = 0;
    int build_rec_err = build_records(table_name, tuple_str_arr, num_of_tuple,
                       &records, &num_of_records);

    if(build_rec_err != 0){
        printf("%s %s\n", func_loc_str, "Failed to convert tuples to records");
        return -1;
    }

    return 0;
}

int parse_insert_stmt_kw_into(char* func_loc_str, char* stmt){
    // parse the into keyword
    printf("...Parsing keyword \"into\"\n");
    char* into_kw = strtok(stmt, " ");
    char into_kw_lower[10];
    str_lower(into_kw_lower, into_kw, strlen(into_kw));

    if(into_kw == NULL || strncmp(into_kw_lower, "into", 4) != 0){
        printf("%s (%s)\n","Error: Failed to parsing keyword \"into\"", func_loc_str);
        return -1;
    }
    printf("...Obtained keyword \"into\"\n");
    return 0;
}

int parse_insert_stmt_kw_value(char* func_loc_str){
    // parse values keyword
    printf("...Parsing keyword \"values\"\n");
    char* values_kw = strtok(NULL, " ");
    char values_kw_lower[10];
    str_lower(values_kw_lower, values_kw, strlen(values_kw));

    if(values_kw == NULL || strncmp(values_kw_lower, "values", 6) != 0){
        printf("%s (%s)\n","Error: Failed to parse keyword \"values\"", func_loc_str);
        return -1;
    }
    printf("...Obtained keyword \"values\"\n");
    return 0;
}

int parse_insert_stmt_table_name(char* func_loc_str, char** table_name){
    // parse the table name
    printf("...Parsing table name\n");
    char* t_name = strtok(NULL, " ");
    if(t_name == NULL){
        printf("%s (%s)\n", "Error: Failed to parsing table name", func_loc_str);
        return -1;
    }
    printf("...Obtained table \"%s\"\n", t_name);
    *table_name = strdup(t_name);
    return 0;
}

int parse_insert_stmt_tuple(char* func_loc_str, int* num_of_tuple, char*** tuple_str_arr){
    /*Parse Tuples*/
    char* str_of_tuple = strtok(NULL, ";");
    remove_ending_spaces(str_of_tuple);
    int parse_tuple_result = parse_tuple_str(str_of_tuple, tuple_str_arr, num_of_tuple);
    if(parse_tuple_result == -1){
        printf("%s %s\n", func_loc_str, "Failed to parse tuples");
        return -1;
    }

    return 0;
}

int parse_tuple_str(char* str_of_tuple, char*** tuple_str_arr, int* num_of_tuple){
    char func_loc_str[] = "(parse_insert_stmt.c/parse_tuple_str)";

    /*Remove leading and trailing spaces */
    remove_leading_spaces(str_of_tuple);
    remove_ending_spaces(str_of_tuple);

    printf("%s Parsing a string of tuple : \"%s\"\n", func_loc_str, str_of_tuple);

    int tuple_str_arr_size = 5;
    *tuple_str_arr = malloc(sizeof(char*) * tuple_str_arr_size);
    *num_of_tuple = 0;

    /* Check if there is only one tuple */
    char* ptr = strchr(str_of_tuple, ')');
    if(ptr == NULL){
        printf("%s %s \"%s\"\n", func_loc_str,
               "Cannot parse str_of_tuple:", str_of_tuple);
        return -1;
    }

    int close_param_index = ptr - str_of_tuple;
    ptr = ptr + 1;
    int open_param_index = 0;
    if(str_of_tuple[open_param_index] != '('){
        printf("%s %s \"%s\"\n", func_loc_str, "Error: Expected '(' but got none :", str_of_tuple);
        return -1;
    }

    if(close_param_index + 1 == strlen(str_of_tuple)){
        printf("...There is only one tuple\n");
        char* tuple_str = substring(str_of_tuple, open_param_index, close_param_index);
        (*tuple_str_arr)[0] = tuple_str;
        *num_of_tuple = 1;
        printf("...Parsed tuple \"%s\"\n", tuple_str);
        return 0;
    }

    char* prev_ptr;
    /* Else parse all the tuple in a loop */
    while(1){
//        printf("%s %s %d\n", func_loc_str, "Current number of tuple:", *num_of_tuple);
        /*Resize tuple_str_arr if needed*/
        if(*num_of_tuple == tuple_str_arr_size){
            printf("!Resizing tuple_str_arr\n");
            int old_tuple_str_arr_size = tuple_str_arr_size;
            tuple_str_arr_size = tuple_str_arr_size * 5;
            *tuple_str_arr = realloc(*tuple_str_arr, sizeof(char*) * tuple_str_arr_size);
            printf("!Resized tuple_str_arr from size of %d to size %d\n", old_tuple_str_arr_size, tuple_str_arr_size);
        }

        if(close_param_index + 1 == strlen(str_of_tuple)){
            char* tuple_str = substring(str_of_tuple, open_param_index, close_param_index);
            (*tuple_str_arr)[*num_of_tuple] = tuple_str;
            *num_of_tuple = (*num_of_tuple) + 1;
            str_of_tuple = NULL;
            printf("...Parsed tuple \"%s\"\n", tuple_str);
            break;
        }


        /*Remove leading spaces from the string ptr*/
        remove_leading_spaces(ptr);

        if(ptr[0] != ','){
            printf("%s %s\n", func_loc_str, "Error: Expected a comma after the closing paren but got none");

            if(*num_of_tuple != 0){
                printf("%s %s\n", func_loc_str, "Freeing non empty tuple_str_arr");
                free_2d_char(*tuple_str_arr, *num_of_tuple);
                printf("%s %s\n", func_loc_str, "Freed non empty tuple_str_arr");

            }else{
                printf("%s %s\n", func_loc_str, "Freeing empty tuple_str_arr");
                free(*tuple_str_arr);
                printf("%s %s\n", func_loc_str, "Freed empty tuple_str_arr");
            }
            return -1;
        }

        char* tuple_str = substring(str_of_tuple, open_param_index, close_param_index);
        (*tuple_str_arr)[*num_of_tuple] = tuple_str;
        *num_of_tuple = (*num_of_tuple) + 1;

        printf("...Parsed tuple \'%s\'\n", tuple_str);

        prev_ptr = ptr + 1;
        ptr = strchr(ptr + 1, '(');
        //check if it is an open param index
        if(ptr == NULL){
            printf("%s %s \'%s\'\n", func_loc_str, "Error: Expected '(' but got none :", prev_ptr);
            if(*num_of_tuple != 0){
                free_2d_char(tuple_str_arr, *num_of_tuple);
            }else{
                free(tuple_str_arr);
            }
            return -1;
        }
        open_param_index = ptr - str_of_tuple;
        ptr = ptr + 1;

        ptr = strchr(ptr + 1, ')');

        if(ptr == NULL){
            printf("%s %s \"%s\"\n", func_loc_str,
                   "Cannot parse str_of_tuple:", str_of_tuple);
            return -1;
        }
        close_param_index = ptr - str_of_tuple;
        ptr = ptr + 1;

    }
    printf("%s %s(%d tuples)\n", func_loc_str, "Successfully parsed all tuples", *num_of_tuple);

    return 0;
}









