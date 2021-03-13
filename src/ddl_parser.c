/*
 * Author: Dylan Cuprewich (dxc2961)
 * Source file for DDL Parser
 * Implements the public functionality for the DDL Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"

int parse_ddl_statement( char* input_statement ) {
    char* statement = strdup(input_statement);
    enum statement_type stmt_type;
    char* token;
    const char delimiter[2] = " ";
    
    // get first word in statement
    token = strtok(statement, delimiter);
    if (token == NULL) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    char* first_word = (char *)malloc(strlen(token));
    char a;
    // make the token all lowercase
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            a = tolower(token[i]);
            first_word[i] = a;
        }
    }
    printf("%s\n",first_word);

    // check for statement type 
    char* check_type;
    if (strlen(first_word) == DROP) {
        check_type = DROP_START;
        stmt_type = DROP;
    } else if (strlen(first_word) == ALTER) {
        check_type = ALTER_START;
        stmt_type = ALTER;
    } else if (strlen(first_word) == CREATE) {
        check_type = CREATE_START;
        stmt_type = CREATE;
    } else {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    int valid_first = strncmp(first_word, check_type, strlen(first_word));
    if (valid_first != 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    free(first_word);
    //printf("valid first word: %s\n", token);


    // check for table second word
    token = strtok(NULL, delimiter);
    if ( strlen(token) != strlen(TABLE)) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    // make second word all lower case
    char* second_word = (char *)malloc(strlen(token));
    char b;
    for (int i = 0; token[i] != '\0'; i++) {
        b = tolower(token[i]);
        second_word[i] = b;
    }
    // check if second word=="table"
    int valid_second = strncmp(second_word, TABLE, 
        strlen(second_word));
    if (valid_second != 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    free(second_word);
    //printf("valid second word: %s\n", token);
    free(statement);
    
    // call one of the other parse funcs
    int result;

    if (stmt_type == DROP) {
        result = parse_drop_table_stmt(input_statement);
    } else if (stmt_type == ALTER) {
        result = parse_alter_table_stmt(input_statement);
    } else if (stmt_type == CREATE) {
        result = parse_create_table_stmt(input_statement);
    } else {
        // printf("%s: %s\n", 
        //    "Something went wrong. Check type", check_type);
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
        return -1;
    }
    if (result < 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid DDL statement", input_statement);
    }
    return result;
}

/*
 * Most simple: CREATE TABLE BAZZLE( baz double PRIMARYKEY );
 * 
 */
int parse_create_table_stmt( char* input_statement ) {
    printf("Create STMT: %s\n", input_statement);

    char* statement = strdup(input_statement);
    statement += strlen(CREATE_START) + 1 
                + strlen(TABLE) + 1;

    //Create Table setup
    char* table_name;
    // resize if needed
    char** attributes = malloc(10 * sizeof(int*));
    int attrCount = 0;
    char* newAtrribute;
    int is_last_attr = 0;
    
    //get everything to left of first open parenthesis
    table_name = strtok(statement, "(");
    printf("Table Name: '%s'\n", table_name);

    // get all attributes
    while (is_last_attr == 0) {
            // get next attribute
        newAtrribute = strtok(NULL, ",");
        // check if there is any attributes
        if (newAtrribute == NULL) {
            fprintf(stderr, "%s: '%s'\n%s\n", 
                "Invalid Create statement", input_statement,
                "create table <name>(\
                    <a_name> <a_type> <constraint_1> ... <constraint_N>,\
                    [primarykey( <a_1> ... <a_N> ),]\
                    [unique( <a_1> ... <a_N> ),]\
                    [foreignkey( <a_1> ... <a_N> ) references <r_name>( <r_1> ... <r_N> )]\
                    );");
            return -1;
        }
        // check if last attribute
        if (  newAtrribute[strlen(newAtrribute) - 1] == ';' ) {
            newAtrribute[strlen(newAtrribute) - 1] = '\0';
            is_last_attr = 1;
        }
        // printf("New attribute: '%s'\n", newAtrribute);
        // save attribute
        attributes[attrCount] = strdup(newAtrribute);
        attrCount += 1;
        if (attrCount%10 == 0) {
            attributes = realloc(attributes, (attrCount+10)* sizeof(int*));
        }
    }

    // check if each attribute is form properly
    for (int i = 0; i < attrCount; i++) {
        // check if new attribute, or constraint
        char* currentAttr = attributes[i];
        printf("Attribute line: '%s'\n", currentAttr);
        char* token;
        token = strtok(currentAttr, " ");
        // force attr lower lowercase
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        // check if valid attribute name
        if ( is_keyword(token) ) {
            // if it is check if attribute is constraint
            if ( is_constraint(token) && i == 0) {
                fprintf(stderr, "%s: '%s'\n", 
                    "Invalid attribute definition. Attribute name is a keyword", token);
                return -1;
            }
            // check if valid constraint def
            while ( (token = strtok(NULL, " ")) ) {
                if (token[0] == ')') {
                    break;
                }
                printf("TOk: '%s'\n", token);

            }
        } else { // check if attribute def is valid
            // get attr type
            token = strtok(NULL, " ");
            for (int i = 0; token[i] != '\0'; i++) {
                if ( isalpha(token[i]) ) {
                    token[i] = tolower(token[i]);
                }
            }
            // check if valid attribute type
            if ( !is_attr_type(token) ) {
                fprintf(stderr, "%s: '%s'\n", 
                    "Invalid attribute definition. Attribute type is invalid", token);
                return -1;
            }
            // check for constraints on attribute
            char* constraints_used[3];
            int constraints_count = 0;
            while ( (token = strtok(NULL, " ")) ) {
                for (int i = 0; token[i] != '\0'; i++) {
                    if ( isalpha(token[i]) ) {
                        token[i] = tolower(token[i]);
                    }
                }
                printf("Possible constraint: '%s'\n", token);
                if (token[0] == ')') {
                    break;
                }
                if ( !is_attr_con(token) ) {
                    fprintf(stderr, "%s: '%s'\n", 
                    "Invalid attribute definition. Attribute constraint is invalid", token);
                    return -1;
                } else {
                    for (int i = 0; i < constraints_count; i++ ) {
                        if (strcmp(token, constraints_used[i]) == 0) {
                            fprintf(stderr, "%s: '%s'\n", 
                            "Invalid attribute definition. Attribute constraint already used", token);
                            return -1;
                        }
                    }

                    constraints_used[constraints_count] = strdup(token);
                    constraints_count += 1;
                }
            }


        } 
    }

    printf("Valid Create Table statement: '%s'\n", input_statement);

    free(attributes);
    return 0;
}

int parse_drop_table_stmt( char* input_statement ) {
    char* statement = strdup(input_statement);
    // "DROP/ALTER/CREATE TABLE <NAME>"
    // move input string pointer to skip beginning
    statement += strlen(DROP_START) + 1 +
                        strlen(TABLE) + 1;

    // printf("Drop STMT: %s\n", statement);
    const char delimiter[2] = " \n";
    char* table_name;
    // whether or not ';' directly follows <NAME> '<NAME>; vs <NAME> ;'
    // 1 = space between <NAME> and ';'
    int name_space_semi = 1;

    // get table name parameter
    table_name = strtok(statement, delimiter);
    if (table_name[strlen(table_name)-1] == STMT_END_CHAR) {
        table_name[strlen(table_name)-1] = '\0';
        name_space_semi = 0;
    }

    char lower_char;
    for (int i = 0; table_name[i] != '\0'; i++) {
        lower_char = tolower(table_name[i]);
        table_name[i] = lower_char;
    }

    // check if table name is not a keyword
    if (is_keyword(table_name)) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid table name; table name is a keyword", table_name);
        return -1;
    } 

    char* check_rest = strtok(NULL, delimiter);
    if (check_rest != NULL && check_rest[0] != STMT_END_CHAR && name_space_semi) {

        fprintf(stderr, "%s: '%s'\n", 
            "Invalid table name", table_name);
        return -1;
    }

    printf("Valid Drop Table statement: '%s'\n", input_statement);
    return 0;
}

int parse_alter_table_stmt( char* input_statement ) {
    char* statement = strdup(input_statement);
    statement += strlen(ALTER_START) + 1 + strlen(TABLE) + 1;

    printf("Alter STMT: %s\n", input_statement);
    char* table_name;
    const char delimiter[2] = " ";


    return 0;
}