

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/parse_insert_stmt.h"

#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_attr_helper.h"
#include "../../include/storagemanager.h"
#include "../../include/helper_module/helper_function.h"


int parse_tuple_str(char* str_of_tuple, char*** tuple_str_arr, int* num_of_tuple);

int get_converted_tuples(char* table_name, char** tuple_str_arr, int num_of_tuple,
                         union record_item*** record_arr, int* num_of_records);

/**
 * Verify that the parameter for the function convert_to_records is valid
 * @param table_name : name of the table
 * @param tuple_str_arr : an array that stores the tuple strings
 * @param num_of_tuple : number of tuples
 * @param record_arr : a array of records
 * @param num_of_records : number of records
 * @return 0 if success and -1 if failed
 */

int verify_build_records_parameters(char* table_name, char** tuple_str_arr, int num_of_tuple);

/**
 * Verify that the attribute metadata nodes are valid
 * @param attr_data_arr : a array of attribute metadata nodes
 * @param num_of_attr : the number of attributes
 * @return 0 for success and -1 for fail
 */
int verify_attr_meta_data_nodes(struct attr_data** attr_data_arr, int num_of_attr);

/**
 * Convert the tuple strings into record_item equivalent
 * @param table_name : name of the table
 * @param tuple_str_arr : an array that stores the tuple strings
 * @param num_of_tuple : number of tuples
 * @param record_arr : a array of records
 * @param num_of_records : number of records
 * @return 0 if success and -1 if failed
 */
int build_records(char* table_name, char** tuple_str_arr, int num_of_tuple,
                       union record_item*** record_arr, int* num_of_records);


/**
 * Verify that the parsed value strings exist and that the total number of
 * value strings parsed is equal to the number of attributes
 * @param value_str_arr : a array that stores value strings
 * @param num_of_vals : the number of value strings
 * @param num_of_attrs : the number of attributes
 * @return 0 for success and -1 for failed
 */
int verify_parsed_val_str(char** value_str_arr, int num_of_vals, int num_of_attrs);

/**
 * Convert a tuple string to record_items
 * @param t_data : the table meta data
 * @param attr_data_arr : an array of attribute meta data
 * @param num_of_attr : the number of attributes
 * @param tuple_str : the tuple string
 * @param record: a record (tuple of attributes)
 * @return 0 for success and -1 for fail
 */
int convert_to_record(struct catalog_table_data* t_data, struct attr_data** attr_data_arr,
        int num_of_attr, char* tuple_str,  union record_item** record);

int convert_and_verify_foreign_key(struct catalog_table_data* t_data, union record_item** record);

/**
 * Parse all the values of the tuple_str and store it in value_arr
 * @param tuple_str : the tuple string
 * @param value_arr : a array that stores value strings
 * @param num_of_values : the number of value strings
 * @return 0 for success and -1 for fail
 */
int get_tuple_values(char* tuple_str, char*** value_arr, int* num_of_values);

/**
 * Get the next value in the tuple_str without parenthesis
 * @param str_ptr : a pointer to the tuple string without parenthesis
 * @param value_str : the value string
 * @return 0 if successful and -1 if failed
 */
int get_next_value(char** str_ptr, char** value_str);


/**
 * Check if a notnull attribute constraint exist or not
 * @param attr_constr_arr : an array of attribute constraint
 * @param num_of_constr : the number of attribute constraint
 * @return 1 for true and 0 for false
 */
int notnull_constr_exist(struct attr_constraint** attr_constr_arr, int num_of_constr);

int parse_insert_stmt(char* insert_stmt) {
    char func_loc_str[] = "(parse_insert_stmt.c/parse_insert_stmt)";
    char* stmt_cpy = strdup(insert_stmt);

    //skip "insert" keyword
    stmt_cpy = stmt_cpy + 7;

    printf("%s %s\n", func_loc_str, "Parsing the insert statement");

    // parse the into keyword
    printf("...Parsing keyword \"into\"\n");
    char* into_kw = strtok(stmt_cpy, " ");
    if(into_kw == NULL){
        printf("%s %s\n",func_loc_str, "Error: Failed to parsing keyword \"into\"");
        return -1;
    }
    printf("...Obtained keyword \"into\"\n");

    // parse the table name
    printf("...Parsing table name\n");
    char* table_name = strtok(NULL, " ");
    if(table_name == NULL){
        printf("%s %s\n",func_loc_str, "Error: Failed to parsing table name");
        return -1;
    }
    printf("...Obtained table \"%s\"\n", table_name);

    // verify that the table exist
    printf("...Verifying that table \"%s\" exist\n", table_name);
    if(catalog_contains(table_name) == 0){
        printf("%s Verification failed: Table \"%s\" does not exist\n",func_loc_str, table_name);
        return -1;
    }
    printf("...Verification success: Table \"%s\" exist \n", table_name);


    // parse values keyword
    printf("...Parsing keyword \"values\"\n");
    char* values_kw = strtok(NULL, " ");
    if(values_kw == NULL){
        printf("%s %s\n",func_loc_str, "Error: Failed to parse keyword \"values\"");
        return -1;
    }
    printf("...Obtained keyword \"values\"\n");

    /*Parse Tuples*/
    char* str_of_tuple = strtok(NULL, ";");
    remove_ending_spaces(str_of_tuple);
    int* num_of_tuple;
    char** tuple_str_arr = NULL;
    int parse_tuple_result = parse_tuple_str(str_of_tuple, &tuple_str_arr, &num_of_tuple);
    if(parse_tuple_result == -1){
        printf("%s %s\n", func_loc_str, "Failed to parse tuples");
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

int build_records(char* table_name, char** tuple_str_arr, int num_of_tuple,
                       union record_item*** record_arr, int* num_of_records){
    char func_loc_str[] = "(parse_insert_stmt.c/build_records)";
    verify_build_records_parameters(table_name, tuple_str_arr, num_of_tuple);

    /* Get the table meta data*/
    struct catalog_table_data* t_data = catalog_get_table_metadata(table_name);
    if(t_data == NULL){
        fprintf(stderr, "%s Table \"%s\" does not exist\n", func_loc_str, table_name);
        return -1;
    }

    /*Allocate memory for record_arr*/
    *record_arr = malloc(sizeof(union record_item*) * num_of_tuple);
    *num_of_records = 0;

    /*Get the attribute metadata nodes*/

    struct attr_data** attr_data_arr = NULL;
    int num_of_attr = sv_ht_values(t_data->attr_ht, &attr_data_arr);

    verify_attr_meta_data_nodes(attr_data_arr, num_of_attr);

    printf("%s %s\n", func_loc_str, "Converting tuple strings to records");
    for(int i = 0; i < num_of_tuple; i++){
        int convert_rec_err = convert_to_record(t_data, attr_data_arr, num_of_attr, tuple_str_arr[i],
                          &((*record_arr)[i]));
        if(convert_rec_err == -1){
            printf("%s Error: Failed to parse tuple string \'%s\'\n", func_loc_str, tuple_str_arr[i]);
//            if(*num_of_records == 0){
//                free(*record_arr);
//            }else{
//                for(int i = 0; i < )
//            }
            return -1;
        }

    }
    printf("%s %s\n", func_loc_str, "Finished converting tuple strings to records");


    return 0;
}

int convert_to_record(struct catalog_table_data* t_data, struct attr_data** attr_data_arr,
                      int num_of_attr, char* tuple_str,  union record_item** record){
    char func_loc_str[] = "(parse_insert_stmt.c/convert_to_records)";

    char* tuple_str_copy = strdup(tuple_str);
    /*Removed the parenthesis for the tuple_str*/
    tuple_str_copy[strlen(tuple_str_copy)-1] = 0;
    tuple_str_copy = tuple_str_copy + 1;

    /*Removing leading and trailing spaces*/
    remove_leading_spaces(tuple_str_copy);
    remove_ending_spaces(tuple_str_copy);


    printf("%s Getting ready to parse \'%s\'\n", func_loc_str, tuple_str_copy);

    printf("%s Allocating memory to record\n", func_loc_str);
    /*Allocate memory to record*/
    *record = malloc(sizeof(union record_item) * num_of_attr);


    char* ptr = tuple_str_copy;
    char** value_arr = NULL;
    int num_of_values = 0;
    int get_tuple_val_err = get_tuple_values(ptr, &value_arr, &num_of_values);

    if(get_tuple_val_err == -1){
        return -1;
    }

    printf("%s Obtained %d values\n", func_loc_str, num_of_values);
    int val_str_verification_err = verify_parsed_val_str(value_arr, num_of_values, num_of_attr);
    if(val_str_verification_err == -1){
        printf("%s %s\n", func_loc_str, "Freeing value_arr");
        free_2d_char(value_arr, num_of_values);
        free(tuple_str_copy);
        printf("%s %s\n", func_loc_str, "Freed value_arr");
        return -1;
    }

    printf("%s %s\n", func_loc_str, "Building a record from the parsed value string");
    for(int i = 0; i < num_of_attr; i++){
        struct attr_data* a_data = attr_data_arr[i];
        if(a_data == NULL){
            fprintf(stderr, "%s %s\n", func_loc_str, "Error: a_data is NULL");
            exit(0);
        }
        char* value_copy = strdup(value_arr[i]);
        str_lower(value_copy, value_copy, strlen(value_copy));

        if(strncmp(value_copy, "null", 4) == 0){
            //if true then value is null
            /*Check if there is a not null constraint*/
            if(attr_has_notnull(a_data)){
                //attribute has notnull constraint
                printf("%s Error: Attribute \"%s\" cannot be null\n", func_loc_str, a_data->attr_name);
                free_2d_char(value_arr, num_of_values);
                free(tuple_str_copy);
                printf("%s %s\n", func_loc_str, "Freed value_arr");
                return -1;
            }
        }

        switch(a_data->type){
            case INT:
                break;
            case DOUBLE:
                break;
            case BOOL:
                break;
            case CHAR:
                break;
            case VARCHAR:
                break;
            default:
                printf("%s %s\n", func_loc_str, "Irrecoverable Error: Attribute data type is not valid");
                exit(0);
        }

    }

    return 0;

}

int get_tuple_values(char* tuple_str, char*** value_arr, int* num_of_values){
    char func_loc_str[] = "(parse_insert_stmt.c/get_tuple_values)";
    if(tuple_str == NULL){
        fprintf(stderr, "%s %s\n", func_loc_str, "tuple_str is NULL");
        exit(0);
    }

    /*Allocate memory to the value_arr*/
    int value_arr_size = 12;
    *value_arr = malloc(sizeof(char*) * value_arr_size);
    for(int i = 0; i < value_arr_size; i++){
        (*value_arr)[i] = NULL;
    }

    *num_of_values = 0;

    /*Get a copy of the tuple_str*/
    char* tuple_str_copy = strdup(tuple_str);

    printf("%s %s \'%s\'\n", func_loc_str, "Parsing tuple values for tuple", tuple_str);
    char* ptr = tuple_str_copy;
    char* value_str = NULL;
    int get_val_str_err = get_next_value(&ptr, &value_str);
    if(get_val_str_err == -1 || value_str == NULL){
        fprintf(stderr, "%s Cannot parse tuple values for \"%s\"\n", func_loc_str, tuple_str);
        free(tuple_str_copy);
        free(value_str);
        free(*value_arr);
        return -1;
    }

    /*Add the first value_str to the value_arr*/
    (*value_arr)[*num_of_values] = value_str;
    *num_of_values += 1;
    printf("...Parsed value %s\n", value_str);

    while(ptr != NULL){
        value_str = NULL;
        get_val_str_err = get_next_value(&ptr, &value_str);
        if(get_val_str_err == -1 || value_str == NULL){
            printf(stderr, "%s Cannot parse tuple values for \"%s\"\n", func_loc_str, tuple_str);
            free(tuple_str_copy);
            if(value_str != NULL){
                free(value_str);
            }
            free(*value_arr);
            return -1;
        }

        /*Add the value_str to the value_arr*/
        (*value_arr)[*num_of_values] = value_str;
        *num_of_values += 1;
        printf("...Parsed value %s\n", value_str);
    }

    printf("%s %s\n", func_loc_str, "Finished parsing tuple values");

    return 0;

}


int get_next_value(char** str_ptr, char** value_str){
    char func_loc_str[] = "(parse_insert_stmt.c/get_next_value)";
//    printf(".../str_ptr is %s\n", *str_ptr);
    if(*str_ptr == NULL){
        fprintf(stderr,"%s %s\n", func_loc_str, "str_ptr is NULL");
        exit(0);
    }

    char* space_ptr = strchr(*str_ptr, ' ');
    char* quote_ptr = strchr(*str_ptr, '"');

    if(space_ptr == NULL && quote_ptr == NULL){
        // there is just one value left
        *value_str = strdup(*str_ptr);
        *str_ptr = NULL;
        return 0;
    }

    if(space_ptr == NULL && quote_ptr != NULL){
        printf("part 1\n");
        char* closing_quote_ptr = strchr((*str_ptr) + 1, '"');
        if(closing_quote_ptr == NULL){
            fprintf(stderr,"%s %s\n", func_loc_str,
                    "Error: Expected a closing quote but got none. Insert statement is invalid");
            return -1;
        }
        int open_quote_index = quote_ptr - (*str_ptr);
        int closing_quote_index = closing_quote_ptr - (*str_ptr);
        int end_index = closing_quote_index - 1;
        *value_str = substring(*str_ptr, 1, end_index);

        if((*str_ptr)[closing_quote_index + 1] == '\0'){
            *str_ptr = NULL;
        }else if((*str_ptr)[closing_quote_index + 1] != ' '){
            printf("%s %s \'%s\'\n", func_loc_str, "Error: Expected a space but there is none:", *str_ptr);
            return -1;
        }else{
            *str_ptr = closing_quote_ptr + 1;
            remove_leading_spaces(*str_ptr);
//            printf("...remaining \'%s\'\n", *str_ptr);
        }
        return 0;

    }else if(space_ptr != NULL && quote_ptr == NULL){
        printf("part 2\n");
        int space_index = space_ptr - (*str_ptr);
        int end_index = space_index - 1;
        *value_str = substring(*str_ptr, 0, end_index);

        if((*str_ptr)[space_index + 1] == '\0'){
            *str_ptr = NULL;
        }else{
            *str_ptr = space_ptr + 1;
            remove_leading_spaces(*str_ptr);
        }

        return 0;

    }else if(space_ptr != NULL && quote_ptr != NULL){
        printf("part 3\n");
        int space_index = space_ptr - (*str_ptr);
        int quote_index = quote_ptr - (*str_ptr);


        if(space_index < quote_index){
            /*space comes before the quotes so you can use the space delim
             * i.e., id_123 "Professor Bob"  can parse out id_123
             */
            int end_index = space_index - 1;
            *value_str = substring(*str_ptr, 0, end_index);

            if((*str_ptr)[space_index + 1] == '\0'){
                *str_ptr = NULL;
            }else{
                *str_ptr = space_ptr + 1;
                remove_leading_spaces(*str_ptr);
            }

            return 0;
        }else if(space_index > quote_index){
//            printf("... Parsing a string\n");
            /*quotes comes before space so you have to parse the content between the
             * quotes
             * i.e., "Professor Bob"  can parse out Professor Bob
             */
            char* closing_quote_ptr = strchr((*str_ptr) + 1, '"');
            if(closing_quote_ptr == NULL){
                fprintf(stderr,"%s %s\n", func_loc_str,
                        "Error: Expected a closing quote but got none. Insert statement is invalid");
                return -1;
            }
            int closing_quote_index = closing_quote_ptr - (*str_ptr);
            int end_index = closing_quote_index - 1;
            *value_str = substring(*str_ptr, 1, end_index);

            if((*str_ptr)[closing_quote_index + 1] == '\0'){
                *str_ptr = NULL;
            }else if((*str_ptr)[closing_quote_index + 1] != ' '){
                printf("%s %s \'%s\'\n", func_loc_str, "Error: Expected a space but there is none:", *str_ptr);
                return -1;
            }else{
//                *str_ptr = (*str_ptr) + 1;
                *str_ptr = closing_quote_ptr + 1;
                remove_leading_spaces(*str_ptr);
//                printf("...remaining \'%s\'\n", *str_ptr);
            }

            return 0;
        }
    }

    fprintf(stderr, "%s %s\n", func_loc_str, "Cannot parse value string at all.");
    return -1;

}

int convert_tuple_str(char* tuple_str, enum db_type* attr_types, int num_of_attrs){
    char func_loc_str[] = "(parse_insert_stmt.c/convert_tuple_str)";

    if(tuple_str == NULL || tuple_str[0]=='\0'){
        printf("%s %s\n", func_loc_str, "Error: tuple_str is invalid (it is NULL)");
        exit(0);
    }

    if(attr_types == NULL){
        printf("%s %s\n", func_loc_str, "Error: attr_type is invalid (it is NULL)");
        exit(0);
    }

    if(num_of_attrs <= 0){
        printf("%s %s %d\n", func_loc_str, "Error: Expected atleast 1 attribute but num_of_attr is", num_of_attrs);
        exit(0);
    }


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

