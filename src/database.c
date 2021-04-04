/**
 * Some things to note:
 * - Creating table is fine but can't drop it due to some memory issue
 * - Saving Catalog is not implemented
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/database.h"
#include "../include/helper_module/multiline_input.h"
#include "../include/ddl_parser.h"
#include "../include/helper_module/helper_function.h"
#include "../include/database_helper.h"





char* parse_table_name(char* data_str, char** table_name){

    int start_index = 0;
    int end_index = 0;
    data_str = data_str + 13;  // data_str currently points to <table_name>( <content>, <content>, ...)

    char* ptr = strchr(data_str, '(');
    end_index = ptr - data_str - 1;

    *table_name = substring(data_str, start_index, end_index);

//    printf("table name is : >%s<\n", *table_name);
//    printf("remaining str is : >%s<\n", ptr);

    start_index = end_index + 1;
    end_index = strlen(data_str) - 2;

    char* remaining_str_with_closing_braces = substring(data_str, start_index, end_index);
    remove_ending_spaces(remaining_str_with_closing_braces);

    end_index = strlen(remaining_str_with_closing_braces) - 2;

    char* remaining_str_clean = substring(remaining_str_with_closing_braces, 2, end_index);
    free(remaining_str_with_closing_braces);
    remove_leading_spaces(remaining_str_clean);
    remove_ending_spaces(remaining_str_clean);
//    printf("(actual)remaining str is : >%s<\n", remaining_str_clean);

    return remaining_str_clean;
}




int execute_create_table(char* statement){

//    if(parse_ddl_statement(statement) == -1){
//        return -1;    //-1 for false
//    }
    int table_num = catalog_get_next_table_num();
    char* table_name;
    char* remaining_str = parse_table_name(statement, &table_name);

    catalog_add_table(table_num, table_name, remaining_str);



    return 0;

}



int execute_drop_table(char* statement){
    statement = statement+11;               //11th index contains the key word
    char* table_name =(char*)malloc(strlen(statement)+1);
    strncpy(table_name, statement, strlen(statement)+1);    //add a 1 for null term

    /*calls the remove table method in catalog that will remove the table
     * in catalog and call the drop_table method in storagemanager.c*/
    catalog_remove_table(table_name);

    return 0;
}

/**
 * Executes a non query statement
 * @param statement
 * @return 0 if successful and -1 if failed
 */
int execute_non_query(char * statement){

    int start_index = 0;
    int end_index = 0;
    char* ptr = strchr(statement, ' ');

    end_index = ptr - statement - 1;
    char* key_word = substring(statement, start_index, end_index);
//    printf("key word is >%s<:\n", key_word);

    if(strncmp(key_word, "create", 6) == 0){
        execute_create_table(statement);
        return 0;
    }else if(strncmp(key_word, "drop", 4) == 0){
        execute_drop_table(statement);
        return 0;
    }else if(strncmp(key_word, "alter", 5) == 0){
        fprintf(stderr, "Sorry could not get alter to work !!!\n");
    }
    return 0;


}

int execute_query(char * query, union record_item *** result){
    return 0;
}

/**
 * Free up memory
 */
int shutdown_database(){
    free_input();
    terminate_database();
    return 0;
}


/**
 *  Program ran as ./database <db_loc> <page_size> <buffer_size>
 */

int main(int argc, char* argv[] ) {
    get_cl_args(argc, argv);
    init_db();

    char* statement;
    int result;                                     // the result of processing the statement (parsing, execution, etc)
    while(true){
        printf("Enter a statement: \n");
        statement = get_input();
        if(strncmp(statement, "quit", 4)==0){
            printf("Program Ended\n");
            break;
        }
        printf("%s\n", statement);
        
        /*process the statement and state if the statement
        was parsed and executed successful or not*/
        result = execute_non_query(statement);     //parse and execute the statement
        if(result == 0){
            printf("SUCCESS\n");
        }else{
            printf("ERROR\n");
        }
    }

    shutdown_database();                         // a function that handles cleaning and frees up memory
}