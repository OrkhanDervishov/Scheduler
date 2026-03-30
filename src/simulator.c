#include <stdio.h>
#include <limits.h>

#include "simulator.h"
#include "ready_queue.h"
#include "scheduler.h"

/* ===================== SIMULATION ===================== */

void run_simulation(ProcessManager* pm, SimulationConfig config)
{
    int time = 0;
    int completed = 0;

    ReadyQueue rq;
    init_queue(&rq);

    Process* current = NULL;
    int quantum_counter = 0;

    printf("\n===== Simulation Start =====\n");

    while (completed < pm->process_count) {

        /* ===================== ARRIVALS ===================== */
        for (int i = 0; i < pm->process_count; i++) {
            Process* p = &pm->processes[i];

            if (p->arrival_time <= time && p->state == PROCESS_NEW) {
                p->state = PROCESS_READY;
                enqueue(&rq, p);
            }
        }

        /* ===================== DISPATCH ===================== */
        if (current == NULL && !is_empty(&rq)) {
            current = dequeue(&rq);

            current->state = PROCESS_RUNNING;
            quantum_counter = 0;

            /* Context switch overhead */
            time += config.context_switch_time;
        }

        /* ===================== EXECUTION ===================== */
        if (current != NULL) {

            printf("Time %d: Running P%d\n", time, current->pid);

            current->remaining_time--;
            quantum_counter++;

            /* FINISH */
            if (current->remaining_time == 0) {
                current->completion_time = time + 1;
                current->state = PROCESS_TERMINATED;

                current = NULL;
                completed++;
            }
            /* ROUND ROBIN PREEMPTION */
            else if (current_config.type == ROUND_ROBIN_SCHEDULING &&
                     quantum_counter >= current_config.quantum) {

                current->state = PROCESS_READY;
                enqueue(&rq, current);

                current = NULL;
            }
        }
        else {
            /* ===================== IDLE HANDLING ===================== */

            int next_arrival = INT_MAX;

            for (int i = 0; i < pm->process_count; i++) {
                if (pm->processes[i].state == PROCESS_NEW) {
                    if (pm->processes[i].arrival_time < next_arrival)
                        next_arrival = pm->processes[i].arrival_time;
                }
            }

            if (next_arrival == INT_MAX) {
                break; /* nothing left */
            }

            printf("Time %d → %d: CPU Idle\n", time, next_arrival);
            time = next_arrival;
            continue;
        }

        time++;
    }

    printf("===== Simulation End =====\n");

    calculate_times(pm);
    print_results(pm);

    clear_queue(&rq);
}