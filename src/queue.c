
#include "../include/queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct queue_str* init_queue() {
    struct queue_str* new_queue = (struct queue_str*) malloc(sizeof(struct queue_str*));
    new_queue->capacity = 0;
    new_queue->front = 0;
    new_queue->end = 0;
    resize(new_queue);
    return new_queue;
}

void push(struct queue_str* queue, char* element) {
    if (size(queue) < 2) {
        resize(queue);
    }
    queue->elements[queue->end] = (char*) malloc( strlen(element) );
    strcpy(queue->elements[queue->end], element);
    queue->end += 1;
    return;
}

char* pop(struct queue_str* queue) {
    queue->front += 1;
    return queue->elements[(queue->front) - 1];
}

char* peek(struct queue_str* queue) {
    return queue->elements[(queue->front)];
}


void resize(struct queue_str* queue) {
    queue->capacity += 5;
    queue->elements = (char**) realloc(queue->elements, (queue->capacity+1) * sizeof(char*));
    return;
}

int size(struct queue_str* queue) {
    return queue->end - queue->front;
}

int is_empty(struct queue_str* queue) {
    return (size(queue) == 0);
}


void print_queue(struct queue_str* queue) {
    printf("Queue:\n");
    for (int i = queue->front; i < queue->end; i++ ) {
        printf("\t%d: '%s'\n", i, queue->elements[i]);
    }  
    return;
}
