
#include "../include/stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct stack_str* init_stack() {
    struct stack_str* new_stack = (struct stack_str*) malloc(sizeof(struct stack_str*));
    new_stack->capacity = 0;
    new_stack->top = 0;
    new_stack->bottom = 0;
    resize_stack(new_stack);
    return new_stack;
}

void push_stack(struct stack_str* stack, char* element) {
    if (size_stack(stack) < 2) {
        resize_stack(stack);
    }
    stack->elements[stack->top] = (char*) malloc( strlen(element) );
    strcpy(stack->elements[stack->top], element);
    stack->top += 1;
    return;
}

char* pop_stack(struct stack_str* stack) {
    stack->top -= 1;
    return stack->elements[stack->top];
}

char* peek_stack(struct stack_str* stack) {
    return stack->elements[(stack->top) - 1];
}


void resize_stack(struct stack_str* stack) {
    stack->capacity += 5;
    stack->elements = (char**) realloc(stack->elements, (stack->capacity+1) * sizeof(char*));
    return;
}

int size_stack(struct stack_str* stack) {
    return stack->top - stack->bottom;
}

int is_empty_stack(struct stack_str* stack) {
    return (size_stack(stack) == 0);
}


void print_stack(struct stack_str* stack) {
    printf("stack:\n");
    for (int i = stack->bottom; i < stack->top; i++ ) {
        printf("\t%d: '%s'\n", i, stack->elements[i]);
    }  
    return;
}
