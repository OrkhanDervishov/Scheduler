#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.h"

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
void schedule(ProcessManager* pm);

/* Algorithms */
void round_robin(ProcessManager* pm, int quantum);
void priority_scheduling(ProcessManager* pm);
void multilevel_queue(ProcessManager* pm);

/* Utilities */
void calculate_times(ProcessManager* pm);
void print_results(ProcessManager* pm);

#endif