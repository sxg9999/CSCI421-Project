
#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include "db_types.h"
#include "catalog/catalog_structs.h"


enum where_op {
    UNDEFINED_OP = -1, // ???

    // LOGICAL OPS
    OR_OP = 1, // OR
    AND_OP = 2, //


    // CONDITIONAL OPS
    EQUAL_OP = 101, // =
    LESS_OP, // <
    LESS_EQ_OP, // <=
    GREATER_OP, // >
    GREATER_EQ_OP, // >=
    

    // MATHEMATICAL OPS
    ADD_OP = 201, // +
    SUB_OP, // -
    MULT_OP = 301, // *
    DIV_OP, // /
};

/**
 * <left_child> <op> <right_child> = result
 */
struct shunt_node {
    struct shunt_node* left_child;     // left expression
    struct shunt_node* right_child;    // right expression
    enum db_type left_type;     // type of the left attribute
    enum db_type right_type;    // type of the right attribute
    enum where_op node_op;      // operation done at node

}shunt_node;

enum where_op typeof_op(char* op);
enum where_op typeof_cond_op(char* op);
enum where_op typeof_logical_op(char* op);
enum where_op typeof_math_op(char* op);
int is_op(char* op);
int is_logic_op(char* op);
int is_math_op(char* op);
int a_higher_than_b(char* op_a, char* op_b);

int eval_tuple();

int build_tree(char* where_clause, struct catalog_table_data* table );

int build_node(struct shunt_node* left_child, enum db_type left_type, 
            struct shunt_node* right_child, enum db_type right_type, enum where_op node_op);

#endif