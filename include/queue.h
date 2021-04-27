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
void push_queue(struct queue_str* queue, char* element);
char* pop_queue(struct queue_str* queue);
char* peek_queue(struct queue_str* queue);
void resize_queue(struct queue_str* queue);
int size_queue(struct queue_str* queue);
void print_queue(struct queue_str* queue);
void print_array(struct queue_str* queue);
int is_empty_queue(struct queue_str* queue);
char* end_pop(struct queue_str* queue);
void reverse_queue(struct queue_str* queue, struct queue_str* reverse);
#endif