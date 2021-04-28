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

int where_tree(char* where_clause, struct catalog_table_data* table) {
    printf("Constructing operation tree from where clause '%s'...\n", where_clause);
    struct queue_str* output_queue;
    int rpn_success = get_rpn(where_clause, &output_queue);
    if (rpn_success != 0 || output_queue == NULL) {
        fprintf(stderr, "Invalid where clause '%s': failed to get rpn form\n", where_clause);
        return -1;
    }
    print_array(output_queue);
    struct queue_str* reverse_output = init_queue();
    reverse_queue(output_queue, reverse_output);
    print_queue(reverse_output);
    output_queue = reverse_output;
    int tree_success = build_tree(output_queue, table);
    if (tree_success != 0) {
        fprintf(stderr, "Invalid where clause syntax: '%s'\n", where_clause);
        return -1;
    }


    return 0;
}


int get_rpn(char* input, struct queue_str** output) {
    printf("Getting RPN form of where clause...\n");
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
    *output = output_queue;

    return 0;
}
 
int build_tree(struct queue_str* output, struct catalog_table_data* table) {
    printf("Building operation tree from RPN form...\n");
    struct shunt_node* root_node = init_shunt_node();
    printf("Top: '%s'\n", peek_queue(output));
    char* top = strdup(pop_queue(output));
    enum where_op node_op = typeof_op(top);
    root_node->node_op = node_op;


    root_node->left_child = init_shunt_node();
    root_node->right_child = init_shunt_node();

    continue_tree(root_node->right_child, output, table);
    continue_tree(root_node->left_child, output, table);
    print_shunt_tree(root_node);


    if (!is_empty_queue(output)) {
        fprintf(stderr, "Queue not empty: ");
        print_array(output);
        return -1;
    }
    return 0;
}

int continue_tree(struct shunt_node* node, struct queue_str* output, struct catalog_table_data* table) {
    printf("Top: '%s'\n", peek_queue(output));
    char* top = strdup(pop_queue(output));
    enum where_op node_op = typeof_op(top);
    // check if leaf node or expressions
    if (!is_op(top)) {
        node->node_op = LEAF;
        return 1;
    }
    node->node_op = node_op;
    node->node_value = (union shunt_value*) malloc(sizeof(union shunt_value));
    strcpy(node->node_value->c, top);

    node->left_child = init_shunt_node();
    node->right_child = init_shunt_node();

    continue_tree(node->right_child, output, table);
    continue_tree(node->left_child, output, table);

    return 0;
}

struct shunt_node* init_shunt_node() {
    struct shunt_node* new_node;
    new_node = (struct shunt_node*) calloc(1, sizeof(struct shunt_node));

    return new_node;
}

void print_shunt_tree(struct shunt_node* node) {
    if (node->node_op != LEAF) {
        print_shunt_tree(node->left_child);
    }
    printf("(%d), ", node->node_op);
    if (node->node_op != LEAF) {
        print_shunt_tree(node->right_child);
    }
}
