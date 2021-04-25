//
// Created by SGuan on 4/24/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/stringify_record.h"
#include "../include/db_types.h"
#include "../include/helper_module/helper_function.h"


char* record_to_str(struct attr_data** attr_data_arr, union record_item* record, int num_attribute){
    if(attr_data_arr == NULL || record == NULL || num_attribute < 1) {
        printf("Cannot stringify the record\n");
        exit(0);
    }

    char** record_value_str_arr = malloc(sizeof(char*) * num_attribute);
    char buffer[260];
    buffer[0] = 0;
    int char_count = 0;

    for(int i = 0; i < num_attribute; i++){
        struct attr_data* a_data = attr_data_arr[i];
        union record_item tmp_record = record[i];
        switch(a_data->type){
            case INT:
                sprintf(buffer, "%d", record[i].i);
                record_value_str_arr[i] = strdup(buffer);
                buffer[0] = 0;
                break;
            case DOUBLE:
                sprintf(buffer, "%lf", record[i].d);
                record_value_str_arr[i] = strdup(buffer);
                buffer[0] = 0;
                break;
            case BOOL:
                if(record[i].b == false){
                    record_value_str_arr[i] = strdup("false");
                }else if(record[i].b == true){
                    record_value_str_arr[i] = strdup("true");
                }
                break;
            case CHAR:

                sprintf(buffer, "\"%s\"", record[i].c);
                record_value_str_arr[i] = strdup(buffer);
                buffer[0] = 0;
                break;
            case VARCHAR:
                sprintf(buffer, "\"%s\"", record[i].v);
                record_value_str_arr[i] = strdup(buffer);
                buffer[0] = 0;
                break;
            default:
                printf("Error: Cannot determine attribute type. (stringify_record.c/record_to_str)\n");
                exit(0);
        }
        char_count += strlen(record_value_str_arr[i]);
    }

    /* Build the record string */
    char* record_string = malloc(char_count + (num_attribute*3) + 2);

    record_string[0] = '(';
    record_string[1] = ' ';
    int curr_index = 2;

    if(num_attribute == 1){
        char* record_value = record_value_str_arr[0];
        for(int i = 0; i < strlen(record_value); i++){
            record_string[curr_index] = record_value[i];
            curr_index += 1;
        }

        record_string[curr_index] = ' ';
        record_string[curr_index+1] = ')';
        record_string[curr_index+2] = 0;
        free_2d_char(record_value_str_arr, num_attribute);
        return record_string;
    }

    for(int i = 0; i < (num_attribute - 1); i++){
        char* record_value = record_value_str_arr[i];
        int len_of_record_value = strlen(record_value);
        for(int j = 0; j < strlen(record_value); j++){
            record_string[curr_index] = record_value[j];
            curr_index += 1;
        }
        record_string[curr_index] = ',';
        record_string[curr_index + 1] = ' ';
        curr_index += 2;
    }

    char* record_value = record_value_str_arr[num_attribute-1];
    for(int i = 0; i < strlen(record_value); i++){
        record_string[curr_index] = record_value[i];
        curr_index += 1;
    }

    record_string[curr_index] = ' ';
    record_string[curr_index+1] = ')';
    record_string[curr_index+2] = 0;
    free_2d_char(record_value_str_arr, num_attribute);
    return record_string;

}

char** record_to_str_n(struct attr_data** attr_data_arr, union record_item** records,
                       int num_attribute, int num_of_records){
    if(attr_data_arr == NULL || records == NULL || num_attribute < 1){
        printf("Cannot get a array of record string\n");
        exit(0);
    }

    char** record_str_arr = malloc(sizeof(char*) * num_of_records);
    int count = 0;
    for(int i = 0; i < num_of_records; i++){
        char* record_str = record_to_str(attr_data_arr, records[i], num_attribute);
        record_str_arr[i] = record_str;
    }

    return record_str_arr;
}
