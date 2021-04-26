#ifndef STACK_H
#define STACK_H

struct stack_str
{
    char** elements;
    int capacity;
    int top;
    int bottom;
};
struct stack_str* init_stack();
void push_stack(struct stack_str* stack, char* element);
char* pop_stack(struct stack_str* stack);
char* peek_stack(struct stack_str* stack);
void resize_stack(struct stack_str* stack);
int size_stack(struct stack_str* stack);
void print_stack(struct stack_str* stack);
int is_empty_stack(struct stack_str* stack);
#endif