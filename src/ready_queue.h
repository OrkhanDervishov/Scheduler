#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "process.h"

typedef struct QueueNode{
    Process* process;
    struct QueueNode *next;
} QueueNode;

typedef struct{
    size_t size;
    QueueNode *head;
    QueueNode *tail;
} ReadyQueue;


void init_queue(ReadyQueue* q);
int is_empty(ReadyQueue* q);
void enqueue(ReadyQueue* q, Process* p);
Process* dequeue(ReadyQueue* q);
Process* peek(ReadyQueue* q);
void clear_queue(ReadyQueue* q);
int remove_process(ReadyQueue* q, Process* target);
void print_queue(ReadyQueue* q);

#endif