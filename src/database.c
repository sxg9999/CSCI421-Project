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

int execute_drop_table(char* statement){
    char* table_name;
    int table_num = catalog.get_table_num(table_name);
    drop_table(table_num);
    catalog.remove_table(table_name);
    return 0;
}

int execute_non_query(char * statement){
    int result = parse_ddl_statement(statement);
    if(result==0){
        char* key_word;
        int query_type = get_query_type(key_word);

        switch(query_type){
            case 0:
                //assuming case 0 is drop table
                execute_drop_table(statement);
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
        }
        return 0;
    }
    return -1;

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



void catalog_test(){
    init_catalog("./db/");                      //initates the catalog
    catalog.add_table("bob",1);
    catalog.add_table("tree",2);
    catalog.add_table("bill",3);
    catalog.add_table("haa",4);
    catalog.print_table_map();
    catalog.print_table_names();

    printf("\n\nAfter removing bob\n");
    catalog.remove_table("bob");
    catalog.print_table_map();
    catalog.print_table_names();
    exit(0);
}


/**
 *  Program ran as ./database <db_loc> <page_size> <buffer_size>
 */

int main(int argc, char* argv[] ) {
//    catalog_test();
    char* test0 = "DROP TABLE NAME1";
    char* test1 = "CREATE TABLE BAZZLE( BAZ INTEGER );";
    char* test2 = "create table foo( baz integer, bar Double notnull, primarykey( bar baz ), foreignkey( bar ) references bazzle( baz ));";
    parse_ddl_statement(test1);
    return 0;
    
    
    
    
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
        mkdir(db_loc,0777);
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
        statement = get_input();
        if(strncmp(statement, "quit", 4)==0){
            printf("Program Ended\n");
            break;
        }
        printf("%s\n", statement);
        
        /*process the statement and state if the statement
        was parsed and executed successful or not*/
        result = process_statement(statement);     //parse and execute the statement
        if(result == 0){
            printf("SUCCESS\n");
        }else{
            printf("ERROR\n");
        }
    }

    shutdown_database();                         // a function that handles cleaning and frees up memory
}