//
// Created by SGuan on 4/26/2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../include/storage_mediator/storage_mediator.h"
#include "../../include/storage_mediator/storage_mediator_printer.h"
#include "../../include/database_util/db_process_non_sql_statements.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/example.h"


int process_non_sql_statements(char* stmt){
    int ret_val = 0;
    char* stmt_cpy = strdup(stmt);

    char* command = strtok(stmt_cpy, " ");
    char command_lower[10];
    str_lower(command_lower, command, strlen(command));
    command_lower[strlen(command)] = 0;

    char* table_name = strtok(NULL, " ");
    table_name[strlen(table_name)-1] = 0;

    if(strncmp(command, "add", 3) == 0){
        ret_val = add_example_table(table_name);

    }else if(strncmp(command, "get", 3) == 0){
         ret_val = print_table_records(table_name);
    }

    free(stmt_cpy);
    return ret_val;
}

int add_example_table(char* table_name){
    int ret_val = 0;
    if(strncmp(table_name, "department", 10) == 0){
        sm_add_table(get_department_table_data());
        ret_val = 1;
    }else if(strncmp(table_name, "student", 7) == 0){
        sm_add_table(get_student_table_data());
        ret_val = 1;
    }

    return ret_val;
}


int print_table_records(char* table_name){
    sm_print_table_records(table_name);
    return 1;
}