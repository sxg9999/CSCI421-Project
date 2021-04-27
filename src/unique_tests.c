
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
#include "../include/parse_select_stmt.h"

int unique_tests();
int test1();
int test2();
int test3();
int test4();
int test5();
int test6();
int test7();
int test8();



int unique_tests() {
    char* unique_table0 = "CREATE TABLE MALL( A CHAR(10) PRIMARYKEY, B INTEGER UNIQUE );";
    char* unique_table1 = "CREATE TABLE DESERT( A CHAR(10) PRIMARYKEY, B INTEGER, UNIQUE( B ) );";
    char* unique_table2 = "CREATE TABLE JOKER( A CHAR(10) PRIMARYKEY, B INTEGER, C BOOLEAN, UNIQUE( B C ) );";
    
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();

    shutdown_database();                         // a function that handles cleaning and frees up memory
    return 0;
}

int test(char* test) {
    execute(test);
    print_tables();
    return 0;
}

int test1() {
    printf("-------------------------------------\n");
    printf("DOUBLE ATTRIBUTE UNIQUE, 0 DIFFER TEST 1\n");

    char* unique_table2 = "CREATE TABLE foot( A CHAR(10) PRIMARYKEY, B INTEGER, C INTEGER, UNIQUE( B C ) );";
    execute(unique_table2);
    char* insert_test1 = "insert into foot values (\"poop\" 1 2);";
    printf("FIRST INSERT T1:\n");
    execute(insert_test1);
    char* dup_insert = "insert into foot values (\"SHIT\" 1 2);";
    printf("SECOND INSERT T1:\n");
    execute(dup_insert);
    printf("RESULTS T1:\n");
    process_non_sql_statements("get foot;");
    return 0;
}

int test2() {
    printf("-------------------------------------\n");
    printf("TWO ATTRIBUTE UNIQUE, 1 DIFFER TEST 2");
    char* unique_table2 = "CREATE TABLE head( A CHAR(10) PRIMARYKEY, B INTEGER, C BOOLEAN, UNIQUE( B C ) );";
    execute(unique_table2);
    char* insert_test1 = "insert into head values (\"poop\" 1 true);";
    printf("FIRST INSERT T2:\n");
    execute(insert_test1);
    char* dup_insert = "insert into head values (\"lol\" 2 true);";
    printf("SECOND INSERT T2:\n");
    execute(dup_insert);
    printf("RESULTS T2:\n");

    process_non_sql_statements("get head;");
    return 0;
}

int test3() {
    printf("-------------------------------------\n");
    printf("TWO UNIQUE, MULTIPLE TUPLE INSERT TEST 3\n");
    char* unique_table2 = "CREATE table crow( A CHAR(10) PRIMARYKEY, B INTEGER, C integer, UNIQUE( B C ) );";
    execute(unique_table2);
    char* insert_test1 = "insert into crow values (\"yellow\" 1 2);";
    printf("FIRST INSERT T3:\n");
    execute(insert_test1);
    char* dup_insert = "insert into crow values (\"blue\" 2 3), (\"purple\" 3 4), (\"white\" 4 5);";
    printf("SECOND INSERT T3:\n");
    execute(dup_insert);
    printf("RESULTS T3:\n");
    process_non_sql_statements("get crow;");
    return 0;
}

int test4() {
    printf("-------------------------------------\n");
    printf("TWO UNIQUE, MULTIPLE TUPLE 1 ATTR DIFFER TEST 4\n");
    char* unique_table2 = "CREATE table bird( A CHAR(10) PRIMARYKEY, B INTEGER, C integer, UNIQUE( B C ) );";
    execute(unique_table2);
    char* insert_test1 = "insert into bird values (\"yellow\" 1 2);";
    printf("FIRST INSERT T4:\n");
    execute(insert_test1);
    char* dup_insert = "insert into bird values (\"blue\" 1 3), (\"purple\" 1 4), (\"white\" 1 5);";
    printf("SECOND INSERT T4:\n");
    execute(dup_insert);
    printf("RESULTS T4:\n");

    process_non_sql_statements("get bird;");
    return 0;
}

int test5() {
    printf("-------------------------------------\n");
    printf("TWO UNIQUE, MULTIPLE TUPLE 1 ATTR DIFFER DUPS TEST 5\n");
    char* unique_table2 = "CREATE table cat( A CHAR(10) PRIMARYKEY, B INTEGER, C integer, UNIQUE( B C ) );";
    execute(unique_table2);
    char* insert_test1 = "insert into cat values (\"yellow\" 1 2);";
    printf("FIRST INSERT T5:\n");
    execute(insert_test1);
    char* dup_insert = "insert into cat values (\"blue\" 1 2), (\"purple\" 1 2), (\"white\" 1 5);";
    printf("SECOND INSERT T5:\n");
    execute(dup_insert);
    printf("RESULTS T5:\n");
    process_non_sql_statements("get cat;");
    return 0;
}

int test6() {
    printf("-------------------------------------\n");
    printf("ONE UNIQUE NOT GROUP, MULTIPLE TUPLE 0 ATTR DIFFER DUPLICATES TEST 6\n");
    char* unique_table2 = "CREATE table mouse( A CHAR(10) PRIMARYKEY, B INTEGER UNIQUE, C integer );";
    execute(unique_table2);
    char* insert_test1 = "insert into mouse values (\"yellow\" 1 2);";
    printf("FIRST INSERT T6:\n");
    execute(insert_test1);
    char* dup_insert = "insert into mouse values (\"blue\" 1 2), (\"purple\" 2 2), (\"white\" 2 5);";
    printf("SECOND INSERT T6:\n");
    execute(dup_insert);
    printf("RESULTS T6:\n");
    process_non_sql_statements("get mouse;");
    return 0;
}

int test7() {
    printf("-------------------------------------\n");
    printf("ONE UNIQUE NOT GROUP, MULTIPLE TUPLE  1 TUPLE WITH 0 ATTR DIFFER DUPLICATES TEST 7\n");
    char* unique_table2 = "CREATE table dog( A CHAR(10) PRIMARYKEY, B INTEGER UNIQUE, C integer );";
    execute(unique_table2);
    char* insert_test1 = "insert into dog values (\"yellow\" 1 2);";
    printf("FIRST INSERT T7:\n");
    execute(insert_test1);
    char* dup_insert = "insert into dog values (\"blue\" 2 2), (\"purple\" 3 2), (\"white\" 1 5);";
    printf("SECOND INSERT T7:\n");
    execute(dup_insert);
    printf("RESULTS T7:\n");
    process_non_sql_statements("get dog;");
    return 0;
}