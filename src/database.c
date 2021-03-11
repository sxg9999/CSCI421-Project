#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "../include/database.h"
#include "helper_module/multiline_input.h"
#include "catalog.h"


/**
 * All parsing, executing related task goes here
 */
int process_statement(char* statement){
    //To be done
    return 0;
}


/**
 * Free up memory
 */
int database_clean_up(){
    free_input();
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
    }
    create_database(db_loc, page_size, buffer_size, exist);

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

    database_clean_up();                           // a function that handles cleaning and frees up memory
}