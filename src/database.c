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
#include "../include/database_util/database_helper.h"
#include "../include/database_util/db_exec_ddl.h"




/**
 * Executes a non query statement
 * @param statement
 * @return 0 if successful and -1 if failed
 */
int execute_non_query(char * statement){
    int exec_result = -1;
    int error;
    enum db_type stmt_type = typeof_s(statement);

    if(stmt_type == DDL){
        printf("statement is a ddl!!!\n");
        error = get_ddl_stmt_parts(statement);

        if(error != 0){
            fprintf(stderr, "(database.c/execute_non_query) cannot parse ddl statement parts!!!\n");
            fprintf(stderr, "Statement given : >%s<", statement);
            exit(0);
        }

        exec_result = execute_ddl_statement();
    }else if(stmt_type == DML) {
        printf("statement is a dml\n!!!");
    }

    return exec_result;

}

int execute_query(char * query, union record_item *** result);
//int execute_query(char * query, union record_item *** result){
//    return 0;
//}


int execute(char* statement){
    int exec_result;
    enum db_type query_type = typeof_q(statement);

    if(query_type == QUERY){
        printf("it is an query statement\n");
//        union record_item** result;
//        exec_result = execute_query(statement, &result);
        //print out the result

    }else if(query_type == NON_QUERY){
        printf("it is an non query statement\n");
        exec_result = execute_non_query(statement);

    }else{
        exec_result = -1;
    }

    return exec_result;
}


/**
 * Free up memory
 */
int shutdown_database(){
    free_input();
    terminate_database();
    db_close();
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
        }else if(strncmp(statement, "tables", 6) == 0){
            print_tables();
            continue;
        }
        printf("%s\n", statement);

        result = execute(statement);

        if(result == 0){
            printf("SUCCESS\n");
        }else{
            printf("ERROR\n");
        }
    }

    shutdown_database();                         // a function that handles cleaning and frees up memory
}