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
#include "../include/database_util/db_process_non_sql_statements.h"
#include "../include/db_types.h"
#include "../include/dml_parser.h"
#include "../include/shunting_yard.h"




/**
 * Executes a non query statement
 * @param statement
 * @return 0 if successful and -1 if failed
 */
int execute_non_query(char * statement){
    char func_loc_str[] = "(database.c/execute_non_query)";
    int exec_result = -1;
    int error;

    error = 0;
    enum db_type stmt_type = typeof_s(statement);

    if(stmt_type == DDL){
        printf("%s %s\n", func_loc_str, "Statement is of type DDL");
        error = parse_ddl_statement(statement);
    }else if(stmt_type == DML){
        printf("%s %s\n", func_loc_str, "Statement is of type DML");
        error = parse_dml_statement(statement);
    }else{
        fprintf(stderr, "%s %s\n", func_loc_str,
                "Expected either DDL or DML but received neither");
        exit(0);
    }


    return error;

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

    }else if(query_type == NON_QUERY){
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
//    free_input();
//    terminate_database();
    db_close();
    return 0;
}

/**
 *  Program ran as ./database <db_loc> <page_size> <buffer_size>
 */

int main(int argc, char* argv[] ) {
    /*
    char* where1 = "where foo > 123";
    char* where2 = "where foo < \"foo\"";
    char* where3 = "where foo > 123 and baz < \"foo\"";
    char* where4 = "where foo = true";
    char* where5 = "where 2 + 3 * 4";
    char* where6 = "where foo > 123 or baz < \"foo\" and bar = 2.1";
    char* where7 = "where A + B * C - D";
    //where_tree(where1, NULL);
    //where_tree(where3, NULL);
    //where_tree(where7, NULL);
    //return 0;
    */
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
        }else if(process_non_sql_statements(statement)){
            continue;
        }


        result = execute(statement);

        if(result == 0){
            printf("SUCCESS\n");
        }else{
            printf("ERROR\n");
        }
    }

    shutdown_database();                         // a function that handles cleaning and frees up memory
}