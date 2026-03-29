#include "ready_queue.h"

void init_queue(ReadyQueue* q)
{
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
}


int is_empty(ReadyQueue* q)
{
    return q->size == 0;
}


void enqueue(ReadyQueue* q, Process* p)
{
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) return;

    node->process = p;
    node->next = NULL;

    if (q->tail == NULL) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }

    q->size++;
}


Process* dequeue(ReadyQueue* q)
{
    if (is_empty(q)) return NULL;

    QueueNode* temp = q->head;
    Process* p = temp->process;

    q->head = q->head->next;

    if (q->head == NULL)
        q->tail = NULL;

    free(temp);
    q->size--;

    return p;
}


Process* peek(ReadyQueue* q)
{
    if (is_empty(q)) return NULL;
    return q->head->process;
}


void clear_queue(ReadyQueue* q)
{
    while (!is_empty(q)) {
        dequeue(q);
    }
}


int remove_process(ReadyQueue* q, Process* target)
{
    if (is_empty(q)) return -1;

    QueueNode *dummy = q->head, *prev = NULL;

    while (dummy) {
        if (dummy->process == target) {

            if (prev == NULL) {
                q->head = dummy->next;
            } else {
                prev->next = dummy->next;
            }

            if (dummy == q->tail) {
                q->tail = prev;
            }

            free(dummy);
            q->size--;
            return 0;
        }

        prev = dummy;
        dummy = dummy->next;
    }

    return -1;
}


void print_queue(ReadyQueue* q)
{
    QueueNode* dummy = q->head;

    printf("ReadyQueue: ");
    while (dummy) {
        printf("P%d -> ", dummy->process->pid);
        dummy = dummy->next;
    }
    printf("NULL\n");
}
