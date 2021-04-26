#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/shunting_yard.h"
#include "../include/queue.h"
#include "../include/stack.h"


struct op_pair{
    char* type_name;
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

static int OP_COUNT = 11;

enum where_op typeof_op(char* op){
    if (op == NULL) {
        return UNDEFINED_OP;
    }

    for(int i = 0; i < OP_COUNT; i++){
        char* t_name = op_pairs[i].type_name;
        // get length of string that's longer
        int max_length = ( strlen(t_name) > strlen(op) ) ? strlen(t_name) : strlen(op);
        if(strncmp(t_name, op, max_length) == 0 ){
            return op_pairs[i].type;
        }
    }

    return UNDEFINED_OP;
}

enum where_op typeof_cond_op(char* op){
    if (op == NULL) {
        return UNDEFINED_OP;
    }

    enum where_op result = typeof_op(op);
    if (AND_OP < result && result > ADD_OP) {
        return result; 
    }
    return UNDEFINED_OP;
}

enum where_op typeof_logical_op(char* op) {
    if (op == NULL) {
        return UNDEFINED_OP;
    }
    enum where_op result = typeof_op(op);
    if (0 < result && result < EQUAL_OP ) {
        return UNDEFINED_OP; 
    }
    return UNDEFINED_OP;
}


enum where_op typeof_math_op(char* op){
    if (op == NULL) {
        return UNDEFINED_OP;
    }
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
    int diff = (a-b);
    return diff > 10 || ( -10 < diff && diff < 10);
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

void op_test() {
    printf("Char: 'or' type: '%d'\n", typeof_op("or"));
    printf("Char: 'and' type: '%d'\n", typeof_op("and"));
    printf("Char: '=' type: '%d'\n", typeof_op("="));
    printf("Char: '<' type: '%d'\n", typeof_op("<"));
    printf("Char: '<=' type: '%d'\n", typeof_op("<="));
    printf("Char: '>' type: '%d'\n", typeof_op(">"));
    printf("Char: '>=' type: '%d'\n", typeof_op(">="));
    printf("Char: '+' type: '%d'\n", typeof_op("+"));
    printf("Char: '-' type: '%d'\n", typeof_op("-"));
    printf("Char: '*' type: '%d'\n", typeof_op("*"));
    printf("Char: '/' type: '%d'\n", typeof_op("/"));

    return;
}

void presedence_test() {
    printf("Char: 'or', 'and': '%d'\n", a_higher_than_b("or", "and"));
    printf("Char: 'and', 'or': '%d'\n", a_higher_than_b("and", "or"));
    printf("Char: '=', 'or': '%d'\n", a_higher_than_b("=", "or"));
    printf("Char: '<', '*': '%d'\n", a_higher_than_b("<", "*"));
    printf("Char: '<=', '>': '%d'\n", a_higher_than_b("<=", ">"));
    printf("Char: '>', '=': '%d'\n", a_higher_than_b(">", "="));
    printf("Char: '>=', 'and': '%d'\n", a_higher_than_b(">=", "and"));
    printf("Char: '+', '-': '%d'\n", a_higher_than_b("+", "-"));
    printf("Char: '-', 'and': '%d'\n", a_higher_than_b("-", "and"));
    printf("Char: '*', '/': '%d'\n", a_higher_than_b("*", "/"));
    printf("Char: '/', 'or': '%d'\n", a_higher_than_b("/", "or"));

    return;
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
    struct stack_str* op_stack = init_stack();
    while( (token = strtok(NULL, " ")) ) {
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        if (!is_op(token)) { // not op
            push_queue(output_queue, token);
        } 
        else { // token is operator
            while ( !is_empty_stack(op_stack) &&  a_higher_than_b(peek_stack(op_stack), token) > 0) {
                push_queue(output_queue, pop_stack(op_stack));
            }
            push_stack(op_stack, token);
        }
    }
    while (!is_empty_stack(op_stack)) {
        push_queue(output_queue, pop_stack(op_stack));
    }
    print_array(output_queue);

    return 0;
}

int build_node(struct shunt_node* left_child, enum db_type left_type, 
            struct shunt_node* right_child, enum db_type right_type, enum where_op node_op);