#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/shunting_yard.h"
#include "../include/queue.h"


struct op_pair{
    char type_name[15];
    enum db_type type;
};

static struct op_pair op_pairs[] = {
        {.type_name = "or", .type = OR_OP}, 
        {.type_name = "and", .type = AND_OP}, 
        {.type_name = "=", .type = EQUAL_OP}, 
        {.type_name = ">", .type = GREATER_OP},
        {.type_name = "<", .type = LESS_OP},
        {.type_name = ">=", .type = GREATER_EQ_OP},
        {.type_name = "<=", .type = LESS_EQ_OP},
        {.type_name = "+", .type = ADD_OP},
        {.type_name = "-", .type = SUB_OP},
        {.type_name = "*", .type = MULT_OP},
        {.type_name = "/", .type = DIV_OP},
};

static int OP_COUNT = 9;

enum where_op typeof_op(char* op){

    for(int i = 0; i < OP_COUNT; i++){
        char* t_name = op_pairs[i].type_name;
        if(strncmp(t_name, op, strlen(op)) == 0 ){
            return op_pairs[i].type;
        }
    }

    return UNDEFINED_OP;
}

enum where_op typeof_cond_op(char* op){
    enum where_op result = typeof_op(op);
    if (AND_OP < result && result > ADD_OP) {
        return result; 
    }
    return UNDEFINED_OP;
}

enum where_op typeof_logical_op(char* op) {
    enum where_op result = typeof_op(op);
    if (0 < result && result < EQUAL_OP ) {
        return UNDEFINED_OP; 
    }
    return UNDEFINED_OP;
}


enum where_op typeof_math_op(char* op){
    enum where_op result = typeof_op(op);
    if (result > GREATER_EQ_OP) {
        return result; 
    }
    return UNDEFINED_OP;
}

int is_op(char* op) {
    int op_type = typeof_op(op);
    return (op_type > 0);
}

int a_higher_than_b(char* op_a, char* op_b) {
    enum where_op a = typeof_op(op_a);
    enum where_op b = typeof_op(op_b);
    return (a - b) > 10;
}


int is_cond_op(char* op) {
    int op_type = typeof_cond_op(op);
    return (0 < op_type);
}

int is_logical_op(char* op) {
    int op_type = typeof_logical_op(op);
    return (0 < op_type);
}

int is_math_op(char* op) {
    int op_type = typeof_math_op(op);
    return (0 < op_type);
}


int build_tree(char* input, struct catalog_table_data* table) {
    char* where_clause = (char*) malloc( strlen(input) );
    strcpy(where_clause, input);
    printf("Where clause: '%s'\n", where_clause);
    char* token;
    // get and check 'where' in where clause
    token = strtok(where_clause, " ");
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    if (strcmp(token, "where") != 0) {
        fprintf(stderr, "Missing 'where' keyword: '%s'\n", input);
        return -1;
    }

    // build op tree
    struct queue_str* output_queue = init_queue();
    struct queue_str* op_queue = init_queue();
    while( (token = strtok(NULL, " ")) ) {
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        printf("Token: '%s'\n", token);
        printf("type: '%d'\n", typeof_op(token));
        if (!is_op(token)) { // not op
            push(output_queue, token);
        } 
        else { // token is operator
            while ( !is_empty(op_queue) &&  a_higher_than_b(peek(op_queue), token) > 0) {
                push(output_queue, pop(op_queue));
            }
            push(op_queue, token);
        }
        print_queue(output_queue);
        print_queue(op_queue);
    }
    printf("Ordering remaining ops...\n");
    print_queue(output_queue);
    print_queue(op_queue);
    printf("------\n");
    while (!is_empty(op_queue)) {
        push(output_queue, pop(op_queue));
        print_queue(output_queue);
        print_queue(op_queue);
    }
    return 0;
}

int build_node(struct shunt_node* left_child, enum db_type left_type, 
            struct shunt_node* right_child, enum db_type right_type, enum where_op node_op);