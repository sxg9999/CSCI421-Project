#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "../include/database.h"
#include "../include/helper_module/multiline_input.h"
#include "../include/catalog.h"
#include "../include/ddl_parser.h"
#include "../include/types/statement_type.h"
#include "../include/types/attr_type.h"
#include "../include/helper_module/helper_function.h"
#include "../include/hash_collection/hash_table.h"
#include "../include/hash_collection/si_ht.h"


int get_query_type(char* key_word){
    return 0;
}


int execute_create_table(char* statement){
    char** stmt_arr;
    int num_of_parts = split_n(&stmt_arr, statement, ' ', 4);

    char* table_name = stmt_arr[2];
    char* stmt_body = stmt_arr[3];
    char* stmt_body_unwrapped = substring(stmt_body, 1, strlen(stmt_body)-4);

    char** body_arr;
    int body_arr_count = split(&body_arr, stmt_body_unwrapped, ',');
    printf("body arr count = %d\n", body_arr_count);

    for(int i = 0; i < body_arr_count; i++){
        char** body_content;
        int bc_count = split(&body_content, body_arr[i], ' ');
        printf("content_%d:\n", i);
        for(int j = 0; j < bc_count; j++){
            printf("%s\n",body_content[j]);
            free(body_content[j]);
        }
        printf("\n\n");
        free(body_content);
    }

}



int execute_drop_table(char* statement){
    statement = statement+11;               //11th index contains the key word
    char* table_name =(char*)malloc(strlen(statement)+2);
    strncpy(table_name, statement, strlen(statement)+1);    //add a 1 for null term
    table_name[strlen(table_name)-1] = 0;

//    int table_num = catalog.get_table_num(table_name);
//    drop_table(table_num);
//    catalog.remove_table(table_name);
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





int un_init(){
    statement_type_close();
    attr_type_close();
}

/**
 * Free up memory
 */
int shutdown_database(){
    free_input();
    un_init();
//    catalog.close();
    terminate_database();
    return 0;
}



void test_create_table(){
    char statement[] = "Create table student ("\
                        "ID varchar(5), "\
                        "name varchar(20) not null, "\
                        "primary key (ID), "\
                        "foreign key (dept_name) references department );";

    printf("Statement:\n%s\n", statement);

    execute_create_table(statement);



    exit(0);
}



/**
 *  Program ran as ./database <db_loc> <page_size> <buffer_size>
 */

int main(int argc, char* argv[] ) {
    init_statement_types();
    init_attr_types();


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
        mkdir(db_loc, 0777);
//#ifdef __linux__
//        mkdir(db_loc, 0777);
//#else
//        _mkdir(db_loc, 0777);
//#endif
    }


    char* db_loc_path = (char*)malloc(sizeof(char)*strlen(db_loc)+2);
    db_loc_path[0] = 0;
    strncpy(db_loc_path, db_loc, strlen(db_loc)+2);

    if(db_loc_path[strlen(db_loc_path)-1] != '/'){
        db_loc_path[strlen(db_loc_path)] = '/';
    }


    create_database(db_loc_path, page_size, buffer_size, exist);
    init_statement_types();                         //initiates the statement type class
//    init_catalog(db_loc_path);                      //initates the catalog
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