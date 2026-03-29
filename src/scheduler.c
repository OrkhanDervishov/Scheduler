#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

/* ===================== GLOBAL CONFIG ===================== */

static SchedulerConfig current_config;

/* ===================== UTILS ===================== */

void calculate_times(ProcessManager* pm)
{
    for (int i = 0; i < pm->process_count; i++) {
        Process* p = &pm->processes[i];

        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
    }
}

void print_results(ProcessManager* pm)
{
    float total_wt = 0, total_tat = 0;

    printf("\nPID\tAT\tBT\tCT\tWT\tTAT\n");

    for (int i = 0; i < pm->process_count; i++) {
        Process* p = &pm->processes[i];

        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p->pid,
               (int)p->arrival_time,
               (int)p->burst_time,
               (int)p->completion_time,
               p->waiting_time,
               p->turnaround_time);

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / pm->process_count);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / pm->process_count);
}

/* ===================== CONFIG ===================== */

void set_scheduling_algorithm(SchedulerConfig config)
{
    current_config = config;
}

/* ===================== SCHEDULER DISPATCH ===================== */

void schedule(ProcessManager* pm)
{
    switch (current_config.type) {

        case ROUND_ROBUN__SCHEDULING:
            round_robin(pm, current_config.quantum);
            break;

        case PRIORITY_SCHEDULING:
            priority_scheduling(pm);
            break;

        case MLQ_SCHEDULING:
            multilevel_queue(pm);
            break;

        case FCFS_SCHEDULING:
        case SJF_SCHEDULING:
        default:
            printf("Scheduling type not fully implemented yet.\n");
            break;
    }
}

/* ===================== ROUND ROBIN ===================== */

void round_robin(ProcessManager* pm, int quantum)
{
    int time = 0;
    int completed = 0;

    /* Initialize remaining times */
    for (int i = 0; i < pm->process_count; i++) {
        pm->processes[i].remaining_time = pm->processes[i].burst_time;
    }

    while (completed < pm->process_count) {

        int progress = 0;

        for (int i = 0; i < pm->process_count; i++) {

            Process* p = &pm->processes[i];

            if (p->remaining_time > 0) {
                progress = 1;

                if (p->remaining_time > quantum) {
                    time += quantum;
                    p->remaining_time -= quantum;
                } else {
                    time += p->remaining_time;
                    p->remaining_time = 0;
                    p->completion_time = time;
                    completed++;
                }
            }
        }

        if (!progress) break;
    }

    calculate_times(pm);
    print_results(pm);
}

/* ===================== PRIORITY SCHEDULING ===================== */

void priority_scheduling(ProcessManager* pm)
{
    int time = 0;
    int completed = 0;

    int visited[MAX_PROCESSES] = {0};

    while (completed < pm->process_count) {

        int idx = -1;
        int best_priority = 9999;

        for (int i = 0; i < pm->process_count; i++) {

            Process* p = &pm->processes[i];

            if (!visited[i] && p->arrival_time <= time) {

                if (p->priority < best_priority) {
                    best_priority = p->priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            Process* p = &pm->processes[idx];

            time += p->burst_time;
            p->completion_time = time;

            visited[idx] = 1;
            completed++;
        } else {
            time++;
        }
    }

    calculate_times(pm);
    print_results(pm);
}

/* ===================== MULTILEVEL QUEUE ===================== */

void multilevel_queue(ProcessManager* pm)
{
    int time = 0;
    int quantum = 2;

    /* Reset remaining times */
    for (int i = 0; i < pm->process_count; i++) {
        pm->processes[i].remaining_time = pm->processes[i].burst_time;
    }

    /* High priority queue (RR-like) */
    for (int i = 0; i < pm->process_count; i++) {

        Process* p = &pm->processes[i];

        if (p->priority <= 2) {

            int rem = p->remaining_time;

            while (rem > 0) {
                if (rem > quantum) {
                    time += quantum;
                    rem -= quantum;
                } else {
                    time += rem;
                    rem = 0;
                    p->completion_time = time;
                }
            }

            p->remaining_time = 0;
        }
    }

    /* Low priority queue (FCFS-like) */
    for (int i = 0; i < pm->process_count; i++) {

        Process* p = &pm->processes[i];

        if (p->priority > 2) {
            time += p->burst_time;
            p->completion_time = time;
            p->remaining_time = 0;
        }
    }

    calculate_times(pm);
    print_results(pm);
}