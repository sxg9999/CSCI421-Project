//
// Created by SGuan on 4/24/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../include/parse_insert_stmt_helpers.h"
#include "../../include/parse_insert_stmt_verification.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_attr_helper.h"
#include "../../include/storagemanager.h"
#include "../../include/stringify_record.h"
#include "../../include/helper_module/helper_function.h"

void test_print_records(union record_item** records, int num_of_records){
    printf("Printing %d amount of records\n", num_of_records);

    for(int i = 0; i < num_of_records; i++){
        printf("(%d, \"%s\") \n", records[i][0].i, records[i][1].c);
    }
}


int build_record(char* table_name, char* tuple_str, union record_item** record){
    char func_loc_str[] = "(parse_insert_stmt.c/build_record)";

    struct catalog_table_data* t_data = catalog_get_table_metadata(table_name);
    struct attr_data** attr_data_arr = NULL;
    int num_of_attr = catalog_get_attr_data(table_name, &attr_data_arr);

    return convert_to_record_new(t_data, attr_data_arr, num_of_attr, tuple_str, record);

}

int convert_to_record_new(struct catalog_table_data* t_data, struct attr_data** attr_data_arr,
                          int num_of_attr, char* tuple_str, union record_item** record){
    char func_loc_str[] = "(parse_insert_stmt_helpers.c/convert_to_record)";

    char* tuple_str_copy = strdup(tuple_str);
    /*Removed the parenthesis for the tuple_str*/
    tuple_str_copy[strlen(tuple_str_copy)-1] = 0;
    tuple_str_copy = tuple_str_copy + 1;

    /*Removing leading and trailing spaces*/
    remove_leading_spaces(tuple_str_copy);
    remove_ending_spaces(tuple_str_copy);

    /*Allocate memory to record*/
    *record = malloc(sizeof(union record_item) * num_of_attr);

    char* ptr = tuple_str_copy;
    char** value_arr = NULL;
    int num_of_values = 0;
    int get_tuple_val_err = get_tuple_values(ptr, &value_arr, &num_of_values);

    if(get_tuple_val_err == -1){
        return -1;
    }

    int err = 0;
    for(int i = 0; i < num_of_attr; i++){
        struct attr_data* a_data = attr_data_arr[i];
        if(a_data == NULL){
            fprintf(stderr, "%s %s\n", func_loc_str, "Error: a_data is NULL");
            exit(0);
        }

        char* value_copy = strdup(value_arr[i]);
        str_lower(value_copy, value_copy, strlen(value_copy));
        if(strncmp(value_copy, "null", 4) == 0 && attr_has_notnull(a_data)){
            err = -1;
            printf("Error: Attribute \"%s\" cannot be null. %s\n", a_data->attr_name,
                   func_loc_str);
            break;

        }else if(strncmp(value_copy, "null", 4) == 0 && !attr_has_notnull(a_data)){
            err = get_record_value_null(&((*record)[i]), a_data->type);

        }else{
            err = get_record_value_notnull(value_arr[i], &((*record)[i]),
                                           a_data->type);
        }
        free(value_copy);
        if(err == -1){
            break;
        }
    }
    free_2d_char(value_arr, num_of_values);
    free(tuple_str_copy);


    return err;


}

int get_record_value_notnull(char* record_value_str, union record_item* record_value,
                             enum db_type attr_type){
    char func_loc_str[] = "(parse_insert_stmt.c/get_record_value_notnull)";
    char* value_copy = NULL;
    char* ptr = NULL;

    int int_tmp;
    double double_tmp;
    switch(attr_type){
        case INT:
            if(is_int(record_value_str)){
                sscanf(record_value_str, "%d", &int_tmp);
                (*record_value).i = int_tmp;
                printf("...converted str to int (%d)\n", (*record_value).i);
            }else{
                printf("Error: Expected a integer value. %s\n", func_loc_str);
                return -1;
            }
            break;
        case DOUBLE:
            if(is_num(record_value_str)){
                sscanf(record_value_str, "%lf", &double_tmp);
                (*record_value).d = double_tmp;
                printf("...converted str to double\n");
            }else{
                printf("Error: Expected a double value. %s\n", func_loc_str);
                return -1;
            }
            break;
        case BOOL:
            value_copy = strdup(record_value_str);
            str_lower(value_copy, value_copy, strlen(value_copy));
            if(strncmp(value_copy, "true", 4) == 0){
                (*record_value).b = true;
                printf("...converted str to bool true\n");
            }else if(strncmp(value_copy, "false", 5) == 0){
                (*record_value).b = false;
                printf("...converted str to bool false\n");
            }else{
                printf("Error: %s is not a valid bool value %s\n", value_copy, func_loc_str);
                free(value_copy);
                return -1;
            }
            free(value_copy);
            break;
        case CHAR:
            if(record_value_str[0] != '"' || record_value_str[strlen(record_value_str)-1] != '"'){
                printf("Error: Expected a string value. %s\n", func_loc_str);
                return -1;
            }
            value_copy = strdup(record_value_str);
            ptr = value_copy;
            ptr[strlen(value_copy) - 1] = '\0';
            ptr = ptr + 1;


            (*record_value).c[0] = 0;
            strncpy((*record_value).c, ptr, strlen(ptr) + 1);
            printf("...converted str to chars (%s)\n", (*record_value).c);
            free(value_copy);
            break;
        case VARCHAR:
            if(record_value_str[0] != '"' || record_value_str[strlen(record_value_str)-1] != '"'){
                printf("Error: Expected a string value. %s\n", func_loc_str);
                return -1;
            }

            value_copy = strdup(record_value_str);
            ptr = value_copy;
            ptr[strlen(value_copy) - 1] = '\0';
            ptr = ptr + 1;

            (*record_value).v[0] = 0;
            strncpy((*record_value).v, ptr, strlen(ptr) + 1);
            printf("...converted str to varchars\n");
            free(value_copy);
            break;
        default:
            printf("%s %s\n", func_loc_str, "Irrecoverable Error: Attribute data type is not valid");
            exit(0);
    }
    return 0;
}

int get_record_value_null(union record_item* record_value,
                          enum db_type attr_type){
    switch(attr_type){
        case INT:
            (*record_value).i = 0;
            break;
        case DOUBLE:
            (*record_value).d = 0.0;
            break;
        case BOOL:
            (*record_value).b = false;
            break;
        case CHAR:
            (*record_value).c[0] = 0;
            strncpy((*record_value).c, "null", 5);
            break;
        case VARCHAR:
            (*record_value).v[0] = 0;
            strncpy((*record_value).v, "null", 5);
            break;
        default:
            printf("Critical Error: Attribute data type is not valid. %s\n",
                   "(parse_insert_stmt_helpers.c/get_record_value_null)");
            exit(0);
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
//    int num_of_attr = sv_ht_values(t_data->attr_ht, &attr_data_arr);
    int num_of_attr = catalog_get_attr_data(table_name, &attr_data_arr);

    verify_attr_meta_data_nodes(attr_data_arr, num_of_attr);

    printf("%s %s\n", func_loc_str, "Converting tuple strings to records");
    for(int i = 0; i < num_of_tuple; i++){
        union record_item* record = NULL;
        int convert_rec_err = convert_to_record_new(t_data, attr_data_arr, num_of_attr, tuple_str_arr[i],
                                                &record);
        if(convert_rec_err == -1){
            printf("%s Error: Failed to parse tuple string \'%s\'\n", func_loc_str, tuple_str_arr[i]);
            if(*num_of_records == 0){
                free(*record_arr);
            }else if(*num_of_records > 0){
                for(int j = 0; j < *num_of_records; j++){
                    free((*record_arr)[j]);
                }
                free(*record_arr);
            }
            free(record);

            return -1;
        }else{
            (*record_arr)[i] = record;
            *num_of_records = (*num_of_records) + 1;
        }

        if(verify_record_is_valid(t_data, (*record_arr)[i]) == 0 ){
            printf("Record is valid %s\n", func_loc_str);
        }

    }

    printf("%s %s\n", func_loc_str, "Finished converting tuple strings to records");

//    test_print_records(*record_arr, *num_of_records);

    char** record_str_arr = record_to_str_n(attr_data_arr, *record_arr, num_of_attr, *num_of_records);
    if(record_str_arr == NULL){
        printf("Error: Array of record string is NULL. (parse_insert_stmt.c/build_records)\n");
        exit(0);
    }
//
    parse_insert_print_records(record_str_arr, *num_of_records);

    return 0;
}


void parse_insert_print_records(char** record_str_arr, int num_of_records){
    printf("Printing out records that was parsed and build\n");

    for(int i = 0; i < num_of_records; i++){
        printf("%s\n", record_str_arr[i]);
    }

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
                free(value_copy);
                printf("%s %s\n", func_loc_str, "Freed value_arr");
                return -1;
            }else{
                switch(a_data->type){
                    case INT:
                        (*record)[i].i = 0;
                        break;
                    case DOUBLE:
                        (*record)[i].d = 0.0;
                        break;
                    case BOOL:
                        (*record)[i].b = false;
                        break;
                    case CHAR:
                        (*record)[i].c[0] = 0;
                        strncpy((*record)[i].c, "null", 5 );
                        break;
                    case VARCHAR:
                        (*record)[i].v[0] = 0;
                        strncpy((*record)[i].v, "null", 5 );
                        break;
                    default:
                        printf("%s %s\n", func_loc_str, "Irrecoverable Error: Attribute data type is not valid");
                        exit(0);
                }
            }
        }else{
            int int_tmp;
            double double_tmp;
            switch(a_data->type){
                case INT:
                    if(is_num(value_arr[i])){
                        sscanf(value_arr[i], "%d", &int_tmp);
                        (*record)[i].i = int_tmp;
                        printf("...converted str to int (%d)\n", (*record)[i].i);
                    }else{
                        printf("Error: Expected a integer value. %s\n", func_loc_str);
                        return -1;
                    }
                    break;
                case DOUBLE:
                    if(is_num(value_arr[i])){
                        sscanf(value_arr[i], "%lf", &double_tmp);
                        (*record)[i].d = double_tmp;
                        printf("...converted str to int\n");
                    }
                    break;
                case BOOL:
                    if(strncmp(value_copy, "true", 4) == 0){
                        printf("...converted str to bool true\n");
                    }else if(strncmp(value_copy, "false", 5) == 0){
                        printf("...converted str to bool false\n");
                    }else{
                        printf("Error: %s is not a valid bool value %s", value_copy, func_loc_str);
                        free(tuple_str_copy);
                        free(value_copy);
                        free_2d_char(value_arr, num_of_values);
                        return -1;
                    }
                    break;
                case CHAR:
                    (*record)[i].c[0] = 0;
                    strncpy((*record)[i].c, value_arr[i], strlen(value_arr[i]) + 1);
                    printf("...converted str to chars (%s)\n", (*record)[i].c);
                    break;
                case VARCHAR:
                    (*record)[i].v[0] = 0;
                    strncpy((*record)[i].v, value_arr[i], strlen(value_arr[i]) + 1);
                    printf("...converted str to varchars\n");
                    break;
                default:
                    printf("%s %s\n", func_loc_str, "Irrecoverable Error: Attribute data type is not valid");
                    exit(0);
            }
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
    int get_val_str_err = get_next_value_new(&ptr, &value_str);
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
        get_val_str_err = get_next_value_new(&ptr, &value_str);
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

int get_next_value_new(char** str_ptr, char** value_str){
    char func_loc_str[] = "(parse_insert_stmt.c/get_next_value)";
    if(*str_ptr == NULL){
        fprintf(stderr,"%s %s\n", func_loc_str, "str_ptr is NULL");
        exit(0);
    }

    char* space_ptr = strchr(*str_ptr, ' ');
    char* quote_ptr = strchr(*str_ptr, '"');

    if(space_ptr == NULL && quote_ptr == NULL){
        // just a non string value left
        *value_str = strdup(*str_ptr);
        *str_ptr = NULL;
        return 0;

    }else if(space_ptr == NULL && quote_ptr != NULL){
        return get_next_val_string(quote_ptr, str_ptr, value_str);

    }else if(space_ptr != NULL && quote_ptr == NULL){
        return get_next_val_non_string(space_ptr, str_ptr, value_str);

    }else if(space_ptr != NULL && quote_ptr != NULL){
        int space_index = space_ptr - (*str_ptr);
        int quote_index = quote_ptr - (*str_ptr);

        if(space_index < quote_index){
            /*space comes before the quotes so you can use the space delim
             * i.e., id_123 "Professor Bob"  can parse out id_123
             */
            return get_next_val_non_string(space_ptr, str_ptr, value_str);
        }else if(space_index > quote_index){
            /*quotes comes before space so you have to parse the content between the
             * quotes
             * i.e., "Professor Bob"  can parse out Professor Bob
             */
            return get_next_val_string(quote_ptr, str_ptr, value_str);
        }
    }

    fprintf(stderr, "%s %s\n", func_loc_str, "Cannot parse value string at all.");
    return -1;

}

int get_next_val_non_string(char* space_ptr, char** str_ptr, char** value_str){
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
}

int get_next_val_string(char* open_quote_ptr, char** str_ptr, char** value_str){
    char func_loc_str[] = "(parse_insert_stmt_helper.c/get_next_val_string)";
    char* closing_quote_ptr = strchr((*str_ptr) + 1, '"');
    if(closing_quote_ptr == NULL){
        printf("Error: Expected a closing quote but got none. Tuple is invalid. %s\n",
               func_loc_str);
        return -1;
    }
    int open_quote_index = open_quote_ptr - (*str_ptr);
    int closing_quote_index = closing_quote_ptr - (*str_ptr);

    *value_str = substring(*str_ptr, open_quote_index, closing_quote_index);

    if((*str_ptr)[closing_quote_index + 1] == '\0'){
        *str_ptr = NULL;
    }else if((*str_ptr)[closing_quote_index + 1] != ' '){
        printf("Error: Expected a space but there is none: \'%s\'. %s\n", *str_ptr,
               func_loc_str);
        return -1;
    }else{
        *str_ptr = closing_quote_ptr + 1;
        remove_leading_spaces(*str_ptr);
    }
    return 0;
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