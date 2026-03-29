#include <stdio.h>
#include "simulator.h"

typedef struct {
    int items[MAX_PROCESSES];
    int front, rear;
} ReadyQueue;

static void init_queue(ReadyQueue* q) {
    q->front = q->rear = -1;
}

static int is_empty(ReadyQueue* q) {
    return q->front == -1;
}

static void enqueue(ReadyQueue* q, int idx) {
    if (q->rear == MAX_PROCESSES - 1) return;

    if (q->front == -1) q->front = 0;
    q->items[++q->rear] = idx;
}

static int dequeue(ReadyQueue* q) {
    if (is_empty(q)) return -1;

    int val = q->items[q->front];

    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front++;

    return val;
}


void run_simulation(ProcessManager* pm, SimulationConfig config)
{
    int time = 0;
    int completed = 0;

    ReadyQueue rq;
    init_queue(&rq);

    int current = -1; /* index of running process */

    printf("\n--- Simulation Start ---\n");

    while (completed < pm->process_count) {

        /* 1. Check arrivals */
        for (int i = 0; i < pm->process_count; i++) {
            Process* p = &pm->processes[i];

            if (p->arrival_time == time && p->state == PROCESS_NEW) {
                p->state = PROCESS_READY;
                enqueue(&rq, i);
            }
        }

        /* 2. If no running process, dispatch */
        if (current == -1 && !is_empty(&rq)) {

            current = dequeue(&rq);
            Process* p = &pm->processes[current];

            p->state = PROCESS_RUNNING;

            /* Context switch overhead */
            time += config.context_switch_time;
        }

        /* 3. Execute current process */
        if (current != -1) {
            Process* p = &pm->processes[current];

            p->remaining_time--;

            printf("Time %d: Running P%d\n", time, p->pid);

            /* If finished */
            if (p->remaining_time == 0) {
                p->completion_time = time + 1;
                p->state = PROCESS_TERMINATED;

                completed++;
                current = -1;
            }
        } else {
            printf("Time %d: CPU Idle\n", time);
        }

        /* 4. Scheduling decision (policy-based) */
        if (current != -1) {

            if (current_config.type == ROUND_ROBIN_SCHEDULING) {
                /* RR preemption */
                int quantum_counter = 0;
                quantum_counter++;

                if (quantum_counter >= current_config.quantum) {
                    enqueue(&rq, current);
                    pm->processes[current].state = PROCESS_READY;
                    current = -1;
                    quantum_counter = 0;
                }
            }
        }

        time++;
    }

    printf("--- Simulation End ---\n");

    calculate_times(pm);
    print_results(pm);
}