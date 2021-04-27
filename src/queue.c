
#include "../include/queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct queue_str* init_queue() {
    struct queue_str* new_queue = (struct queue_str*) malloc(sizeof(struct queue_str*));
    new_queue->capacity = 0;
    new_queue->front = 0;
    new_queue->end = 0;
    resize_queue(new_queue);
    return new_queue;
}

void push_queue(struct queue_str* queue, char* element) {
    if (size_queue(queue) < 2) {
        resize_queue(queue);
    }
    queue->elements[queue->end] = (char*) malloc( strlen(element) );
    strcpy(queue->elements[queue->end], element);
    queue->end += 1;
    return;
}

char* pop_queue(struct queue_str* queue) {
    queue->front += 1;
    return queue->elements[(queue->front) - 1];
}

char* end_pop(struct queue_str* queue) {
    queue->end -= 1;
    return queue->elements[(queue->end)];
}

char* peek_queue(struct queue_str* queue) {
    return queue->elements[(queue->front)];
}


void resize_queue(struct queue_str* queue) {
    queue->capacity += 5;
    queue->elements = (char**) realloc(queue->elements, (queue->capacity+1) * sizeof(char*));
    return;
}

int size_queue(struct queue_str* queue) {
    return queue->end - queue->front;
}

int is_empty_queue(struct queue_str* queue) {
    return (size_queue(queue) == 0);
}

void reverse_queue(struct queue_str* queue, struct queue_str* reverse) {
    int q_size = size_queue(queue);
    for (int i = 0; i < q_size; i++) {
        push_queue(reverse, end_pop(queue));
    }
    return;
}


void print_queue(struct queue_str* queue) {
    printf("Queue:\n");
    for (int i = queue->front; i < queue->end; i++ ) {
        printf("\t%d: '%s'\n", i, queue->elements[i]);
    }  
    return;
}

void print_array(struct queue_str* queue) {
    printf("Queue: ");
    for (int i = queue->front; i < queue->end; i++ ) {
        printf("%s ", queue->elements[i]);
    }  
    printf("\n");
    return;
}
