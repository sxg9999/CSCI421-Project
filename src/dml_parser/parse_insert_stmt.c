

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/parse_insert_stmt.h"

#include "../../include/catalog/catalog.h"
#include "../../include/helper_module/helper_function.h"


int is_valid_tuples(char* table_name, char** tuple_str_arr, int num_of_tuple){
    char func_loc_str[] = "(parse_insert_stmt.c/is_valid_tuples)";

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



}

int parse_tuple_str(char* str_of_tuple, char*** tuple_str_arr, int* num_of_tuple){
    char func_loc_str[] = "(parse_insert_stmt.c/parse_tuple_str)";
    printf("%s Parsing a string of tuple : \"%s\"\n", func_loc_str, str_of_tuple);

    int tuple_str_arr_size = 12;
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
    int open_param_index = 0;
    if(str_of_tuple[open_param_index] != '('){
        printf("%s %s \"%s\"\n", func_loc_str, "Error: Expected '(' but got none :", str_of_tuple);
        return -1;
    }

    if(close_param_index + 1 == strlen(str_of_tuple)){
        printf("%s %s\n", func_loc_str, "There is only one tuple");
        char* tuple_str = substring(str_of_tuple, open_param_index, close_param_index);
        (*tuple_str_arr)[0] = tuple_str;
        *num_of_tuple = 1;
        printf("%s %s \"%s\"\n", func_loc_str, "Parsed tuple", tuple_str);
        return 0;
    }

    /* Else parse all the tuple in a loop */
    while(1){
        if(close_param_index + 1 == strlen(str_of_tuple)){
            char* tuple_str = substring(str_of_tuple, open_param_index, close_param_index);
            (*tuple_str_arr)[*num_of_tuple] = tuple_str;
            *num_of_tuple = (*num_of_tuple) + 1;
            printf("%s %s \"%s\"\n", func_loc_str, "Parsed tuple", tuple_str);
            break;
        }

        if(str_of_tuple[close_param_index + 1] != ','){
            printf("%s %s\n", func_loc_str, "Error: Expected a comma after the closing paren but got none");

            if(*num_of_tuple != 0){
                free_2d_char(tuple_str_arr, *num_of_tuple);
            }else{
                free(tuple_str_arr);
            }
            return -1;
        }

        char* tuple_str = substring(str_of_tuple, open_param_index, close_param_index);
        (*tuple_str_arr)[*num_of_tuple] = tuple_str;
        *num_of_tuple = (*num_of_tuple) + 1;
        printf("%s %s \"%s\"\n", func_loc_str, "Parsed tuple", tuple_str);

        open_param_index = close_param_index + 3;
        //check if it is an open param index
        if(str_of_tuple[open_param_index] != '('){
            printf("%s %s \"%s\"\n", func_loc_str, "Error: Expected '(' but got none :", ptr + 3);
            if(*num_of_tuple != 0){
                free_2d_char(tuple_str_arr, *num_of_tuple);
            }else{
                free(tuple_str_arr);
            }
            return -1;
        }

        ptr = strchr(ptr + 1, ')');

        if(ptr == NULL){
            printf("%s %s \"%s\"\n", func_loc_str,
                   "Cannot parse str_of_tuple:", str_of_tuple);
            return -1;
        }
        close_param_index = ptr - str_of_tuple;

    }
    printf("%s %s(%d tuples)\n", func_loc_str, "Successfully parsed all tuples", *num_of_tuple);

    return 0;
}

int parse_insert_stmt(char* insert_stmt) {
    char func_loc_str[] = "(parse_insert_stmt.c/parse_insert_stmt)";
    char* stmt_cpy = strdup(insert_stmt);

    //skip "insert" keyword
    stmt_cpy = stmt_cpy + 7;

    // parse the into keyword
    printf("%s %s\n",func_loc_str, "Parsing keyword \"into\"");
    char* into_kw = strtok(stmt_cpy, " ");
    if(into_kw == NULL){
        printf("%s %s\n",func_loc_str, "Error: Failed to parsing keyword \"into\"");
        return -1;
    }
    printf("%s %s\n", func_loc_str, "Obtained keyword \"into\"");

    // parse the table name
    printf("%s %s\n",func_loc_str, "Parsing table name");
    char* table_name = strtok(NULL, " ");
    if(table_name == NULL){
        printf("%s %s\n",func_loc_str, "Error: Failed to parsing table name");
        return -1;
    }
    printf("%s Obtained table \"%s\"\n",func_loc_str, table_name);

    // verify that the table exist
    printf("%s Verifying that table exist \"%s\"\n",func_loc_str, table_name);
    if(catalog_contains(table_name) == 0){
        printf("%s Verification failed: Table \"%s\" does not exist\n",func_loc_str, table_name);
        return -1;
    }
    printf("%s Verification success: Table \"%s\" exist \n",func_loc_str, table_name);


    // parse values keyword
    printf("%s %s\n",func_loc_str, "Parsing keyword \"values\"");
    char* values_kw = strtok(NULL, " ");
    if(values_kw == NULL){
        printf("%s %s\n",func_loc_str, "Error: Failed to parse keyword \"values\"");
        return -1;
    }
    printf("%s %s\n",func_loc_str, "Obtained keyword \"values\"");


    char* str_of_tuple = strtok(NULL, ";");
    remove_ending_spaces(str_of_tuple);
    int* num_of_tuple;
    char** tuple_str_arr = NULL;
    int parse_tuple_result = parse_tuple_str(str_of_tuple, &tuple_str_arr, &num_of_tuple);
    if(parse_tuple_result == -1){
        printf("%s %s\n", func_loc_str, "Failed to parse tuples");
        return -1;
    }




    return 0;
}