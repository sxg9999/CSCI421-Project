#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "../include/database.h"
#include "../include/helper_module/multiline_input.h"
#include "../include/catalog.h"
#include "../include/ddl_parser.h"
#include "../include/statement_type.h"



int get_query_type(char* key_word){
    return 0;
}


int execute_create_table(char* statement){

    init_statement_types();

    char* statement_copy = (char*)malloc(sizeof(char)*strlen(statement)+3);
    statement_copy[0] = 0;
    strncpy(statement_copy, statement, strlen(statement)+1);


    char delim[2] = " ";
    char* table_name = strtok(statement+13, delim);


    char* table_name_lower = (char*)malloc(sizeof(char)*strlen(table_name)+2);
    table_name_lower[0] = 0;
    str_lower(table_name_lower, table_name, strlen(table_name));


    int attr_end_index = strlen(statement_copy) - 2;
    int attr_start_index = strlen(table_name_lower) + 3;
    statement_copy[attr_end_index] = ',';
    statement_copy[attr_end_index+1] = 0;
    statement_copy = statement_copy + 13 + attr_start_index;
//    char* attr_start = statement + 13 + strlen(table_name) +3;        //pointer to the start of the paren



//    char* curr_str = strtok(statement_copy, ",");

    char** attributes =(char**)malloc(sizeof(char*)*10);

    char* curr = strtok(statement_copy, ",");
    int index = 0;
    while(curr!=NULL){
        char* tmp = malloc(strlen(curr)+2);
        strncpy(tmp, curr, strlen(curr)+1);
        if(tmp[0] == ' '){
            tmp = tmp+1;
        }
        attributes[index] = tmp;
        index++;
        curr = strtok(NULL, ",");
    }

    //filler code
    int data_types[3];
    data_types[0] = 1;
    data_types[1] = 2;
    data_types[2] = 3;

    int key_index[1];
    key_index[0] = 0;

    add_table(data_types, key_index, 3, 1);

    printf("no error");


//    for(int i = 0; i<index;i++){
//
//        char* ptr = attributes[i];
//        for(int j = 0; j < strlen(ptr); j++){
//            if(ptr[i]==' '){
//                printf("space %s\n", ptr);
//            }
//            printf("done here");
//        }
//    }
//    printf("done %s\n", attributes[0]);
//    char* curr_



    return 0;
}

int execute_drop_table(char* statement){
    statement = statement+11;               //11th index contains the key word
    char* table_name =(char*)malloc(strlen(statement)+2);
    strncpy(table_name, statement, strlen(statement)+1);    //add a 1 for null term
    table_name[strlen(table_name)-1] = 0;

    int table_num = catalog.get_table_num(table_name);
    drop_table(table_num);
    catalog.remove_table(table_name);
    return 0;
}

/**
 * Executes a non query statement
 * @param statement
 * @return 0 if successful and -1 if failed
 */
int execute_non_query(char * statement){
    int execute_result = 0;
    int valid = parse_ddl_statement(statement);
    if(valid ==0){
        char delim[2] = " ";
        char* key_word = strtok(statement, delim);
        int query_type = get_query_type(key_word);

        switch(query_type){
            case 0:
                //create table
                execute_create_table(statement);
                break;
            case 1:
                //drop table
                execute_drop_table(statement);
                break;
            case 2:
                //alter table
                break;
            default:
                fprintf(stderr, "Invalid Key Word: %s\n", key_word);
                execute_result = -1;
                break;
        }
    }else{
        execute_result = -1;
    }

    return execute_result;

}

int execute_query(char * query, union record_item *** result){
    return 0;
}


/**
 * All parsing, executing related task goes here
 */
int process_statement(char* statement){
    //To be done

    int result = 0;
//    int result = -1;
//    //determine whether its a non_query or a query
//    if(is_query(statement)){
//        union record_item*** table;
//        result = execute_query(statement, table);
//    }else{
//        result = execute_non_query(statement);
//    }

    return result;
}





/**
 * Free up memory
 */
int shutdown_database(){
    free_input();
    catalog.close();
    terminate_database();
    return 0;
}





/**
 *  Program ran as ./database <db_loc> <page_size> <buffer_size>
 */

int main(int argc, char* argv[] ) {

    char* db_loc = argv[1];
    char* ptr;
    int page_size = strtol(argv[2], &ptr, 10);
    int buffer_size = strtol(argv[3], &ptr, 10);

    if(argc < 4 || argc > 4){
        //print an error message and usage guide and then exit
        fprintf(stderr, "Invalid Arguments!!!\nExpected: database <db_loc> <page_size> <buffer_size>\n"); 
        exit(0); 
    }

    /*check if a database exist at that location (db_loc)*/
    struct stat s;
    bool exist = false;
    if(stat(db_loc, &s) == 0 && (s.st_mode & S_IFDIR)?true:false){
        exist = true;
    }else{
        //if the db dir doesn't exist then create one
#ifdef __linux__
        mkdir(db_loc, 0777);
#else
        _mkdir(db_loc, 0777);
#endif
    }

    char* db_loc_path = (char*)malloc(sizeof(char)*strlen(db_loc)+2);
    strncpy(db_loc_path, db_loc, strlen(db_loc)+1);

    if(db_loc_path[strlen(db_loc_path)-1] != '/'){
        db_loc_path[strlen(db_loc_path)] = '/';
    }

    create_database(db_loc_path, page_size, buffer_size, exist);
    init_statement_types();                         //initiates the statement type class
    init_catalog(db_loc_path);                      //initates the catalog
    create_multiline_input();                       //initiates the structs and fields neccessary for 
                                                    //handling multiline user inputs

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