#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "metrics.h"
#include "output.h"


SchedulerConfig current_config;

void calculate_times(ProcessManager* pm)
{
    metrics_calculate_process_times(pm);
}

void print_results(ProcessManager* pm)
{
    output_print_results(pm, current_config);
}

void set_scheduling_algorithm(SchedulerConfig config)
{
    current_config = config;
}

/* ===================== SCHEDULER DISPATCH ===================== */

void schedule(ReadyQueue *rq){

    switch (current_config.type) {
        case ROUND_ROBIN_SCHEDULING:
            round_robin_scheduling(rq);
            break;
        case PRIORITY_SCHEDULING:
            priority_scheduling(rq);
            break;
        case MLQ_SCHEDULING:
            mlq_scheduling(rq);
            break;
        case FCFS_SCHEDULING:
            fcfs_scheduling(rq);
            break;
        case SJF_SCHEDULING:
            sjf_scheduling(rq);
            break;
    }
}

// ==========================================
// Round Robin

Process* round_robin_scheduling(ReadyQueue* rq)
{
    if (is_empty(rq)) return NULL;

    Process* p = dequeue(rq);

    int quantum = current_config.quantum;

    /* Execute for one quantum */
    int exec_time = (p->remaining_time > quantum)
                    ? quantum
                    : p->remaining_time;

    p->remaining_time -= exec_time;

    /* If finished */
    if (p->remaining_time == 0) {
        return p;  // done
    }

    /* Not finished → requeue */
    enqueue(rq, p);

    return NULL;  // not finished yet
}

// ==========================================
// Priority scheduling

Process* sjf_scheduling(ReadyQueue* rq)
{
    if (is_empty(rq)) return NULL;

    QueueNode *curr = rq->head, *prev = NULL;
    QueueNode *best = curr, *best_prev = NULL;

    clock_t min_burst = curr->process->burst_time;

    while (curr) {
        if (curr->process->burst_time < min_burst) {
            min_burst = curr->process->burst_time;
            best = curr;
            best_prev = prev;
        }

        prev = curr;
        curr = curr->next;
    }

    /* remove best */
    if (best_prev == NULL)
        rq->head = best->next;
    else
        best_prev->next = best->next;

    if (best == rq->tail)
        rq->tail = best_prev;

    Process* p = best->process;
    free(best);
    rq->size--;

    return p;
}


Process* srtf_scheduling(ReadyQueue* rq, Process* current)
{
    Process* best = current;

    QueueNode* curr = rq->head;

    while (curr) {
        if (!best || curr->process->remaining_time < best->remaining_time) {
            best = curr->process;
        }
        curr = curr->next;
    }

    return best;
}

Process* priority_scheduling(ReadyQueue* rq)
{
    if (is_empty(rq)) return NULL;

    QueueNode *curr = rq->head, *prev = NULL;
    QueueNode *best = curr, *best_prev = NULL;

    int best_priority = curr->process->priority;

    while (curr) {
        if (curr->process->priority < best_priority) {
            best_priority = curr->process->priority;
            best = curr;
            best_prev = prev;
        }

        prev = curr;
        curr = curr->next;
    }

    /* remove best */
    if (best_prev == NULL)
        rq->head = best->next;
    else
        best_prev->next = best->next;

    if (best == rq->tail)
        rq->tail = best_prev;

    Process* p = best->process;
    free(best);
    rq->size--;

    return p;
}

Process* mlq_scheduling(ReadyQueue* rq)
{
    if (is_empty(rq)) return NULL;

    QueueNode *curr = rq->head, *prev = NULL;

    QueueNode *high = NULL, *high_prev = NULL;

    /* find high-priority process first */
    while (curr) {
        if (curr->process->priority <= 2) {
            high = curr;
            high_prev = prev;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    QueueNode *target = high ? high : rq->head;
    QueueNode *target_prev = high ? high_prev : NULL;

    /* remove */
    if (target_prev == NULL)
        rq->head = target->next;
    else
        target_prev->next = target->next;

    if (target == rq->tail)
        rq->tail = target_prev;

    Process* p = target->process;
    free(target);
    rq->size--;

    return p;
}

Process* fcfs_scheduling(ReadyQueue* rq)
{
    if (is_empty(rq)) return NULL;

    /* Simply take the first process in queue */
    Process* p = dequeue(rq);

    return p;
}