#include <stdio.h>
#include "scheduler.h"
#include "tests.h"
#include "simulator.h"
#include "generator.h"
#include "input.h"

void work_gen(ProcessManager *pm);
void input_file(ProcessManager* pm);
void input_manual(ProcessManager* pm);
void input_processes(ProcessManager* pm);
void run_scheduler(ProcessManager* pm);


int main() {
    ProcessManager pm;
    init_process_manager(&pm, 100);

    printf("===== CPU Scheduling Simulator =====\n");

    input_processes(&pm);

    run_scheduler(&pm);

    free_process_manager(&pm);


    return 0;
}

void input_processes(ProcessManager* pm)
{
    int input_type;
    printf("select input type(manual - 0, file - 1, generate - 2):");
    scanf("%d", &input_type);

    switch (input_type)
    {
    case 0:
        input_manual(pm);
        break;
    case 1:
        input_file(pm);
        break;
    case 2:
        // generate_workload(pm, 3, WORKLOAD_MIXED);
        work_gen(pm);
        break;
    }
}

void work_gen(ProcessManager *pm){
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    WorkloadType type;
    printf("CPU BOUND - 0\nINTERACTIVE - 1\nMIXED - 2\nPRIORITY HEAVY - 3\n");
    printf("Enter workload type: ");
    scanf("%d", &type);

    printf("n:%d type:%d\n", n, type);
    generate_workload(pm, n, type);
}

void input_file(ProcessManager* pm){
    char in_file[512];
    char path[256];
    strcpy(in_file, "input_files/");
    printf("Enter file name: ");
    scanf("%s", path);
    strcat(in_file, path);

    input_processes_from_file(pm, in_file);
}

void input_manual(ProcessManager* pm){
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int at, bt, pr;
        char name[32];

        printf("\nProcess %d:\n", i + 1);

        printf("Name: ");
        scanf("%s", name);

        printf("Arrival Time: ");
        scanf("%d", &at);

        printf("Burst Time: ");
        scanf("%d", &bt);

        printf("Priority: ");
        scanf("%d", &pr);

        add_process(pm, create_process(name, at, bt, pr));
    }
}

void run_scheduler(ProcessManager* pm)
{
    int choice;
    int quantum = 2;

    printf("\nSelect Scheduling Algorithm:\n");
    printf("1. FCFS\n");
    printf("2. SJF\n");
    printf("3. Round Robin\n");
    printf("4. Priority\n");
    printf("Choice: ");
    scanf("%d", &choice);

    SchedulerConfig config;

    switch (choice) {
        case 1:
            config.type = FCFS_SCHEDULING;
            break;

        case 2:
            config.type = SJF_SCHEDULING;
            break;

        case 3:
            config.type = ROUND_ROBIN_SCHEDULING;
            printf("Enter Quantum: ");
            scanf("%d", &quantum);
            config.quantum = quantum;
            break;

        case 4:
            config.type = PRIORITY_SCHEDULING;
            break;

        default:
            printf("Invalid choice!\n");
            return;

            
    }

    set_scheduling_algorithm(config);

    /* Run simulation */
    SimulationConfig sim = {
        .context_switch_time = 0
    };

    run_simulation(pm, sim);
}