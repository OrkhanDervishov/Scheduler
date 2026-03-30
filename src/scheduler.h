#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.h"
#include "ready_queue.h"

#define MAX_PROCESSES 100

typedef enum{
    MLQ_SCHEDULING = 0,
    ROUND_ROBIN_SCHEDULING,
    PRIORITY_SCHEDULING,
    SJF_SCHEDULING,
    FCFS_SCHEDULING
} Scheduling;

typedef struct {
    Scheduling type;
    int quantum;
} SchedulerConfig;

extern SchedulerConfig current_config;

/* Core API */
void set_scheduling_algorithm(SchedulerConfig config);
void schedule(ReadyQueue *rq);

// Algorithms
Process* round_robin_scheduling(ReadyQueue* rq);
Process* sjf_scheduling(ReadyQueue* rq);
Process* priority_scheduling(ReadyQueue* rq);
Process* mlq_scheduling(ReadyQueue* rq);
Process* fcfs_scheduling(ReadyQueue* rq);

/* Utilities */
void calculate_times(ProcessManager* pm);
void print_results(ProcessManager* pm);

#endif