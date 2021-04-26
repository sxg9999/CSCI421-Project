#ifndef QUEUE_H
#define QUEUE_H

struct queue_str
{
    char** elements;
    int capacity;
    int front;
    int end;
};
struct queue_str* init_queue();
void push(struct queue_str* queue, char* element);
char* pop(struct queue_str* queue);
char* peek(struct queue_str* queue);
void resize(struct queue_str* queue);
int size(struct queue_str* queue);
void print_queue(struct queue_str* queue);
int is_empty(struct queue_str* queue);
#endif