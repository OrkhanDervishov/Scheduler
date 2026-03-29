#include "process.h"

//===============================================================
// Initialization

void init_process_manager(ProcessManager* pm, uint32_t max_capacity)
{
    pm->process_count = 0;
    pm->capacity = max_capacity;
    pm->processes = (Process*)malloc(sizeof(Process) * max_capacity);

    if (!pm->processes) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
}


void free_process_manager(ProcessManager* pm)
{
    free(pm->processes);
    pm->processes = NULL;
    pm->process_count = 0;
}

//===============================================================
// Manipulation

int add_process(ProcessManager* pm, Process p)
{
    if (!pm) return -1;
    if(pm->process_count >= pm->capacity) return -2;
    
    pm->processes[pm->process_count] = p;
    pm->process_count++;
    
    return 0;
}

int delete_process_by_pid(ProcessManager* pm, pid_t pid)
{
    if (!pm || pm->process_count == 0) return -1;

    int index = -1;

    // Find process
    for (int i = 0; i < pm->process_count; i++) {
        if (pm->processes[i].pid == pid) {
            index = i;
            break;
        }
    }

    if (index == -1) return -2;

    // Shift elements left
    for (int i = index; i < pm->process_count - 1; i++) {
        pm->processes[i] = pm->processes[i + 1];
    }

    pm->process_count--;

    return 0;
}

//===============================================================
// Visualization

void print_process(const Process* p)
{
    printf("PID: %d | Name:%s | AT: %zu | BT: %zu | RT: %zu | PR: %u | STATE: %d\n",
           p->pid,
           p->name,
           (long)p->arrival_time,
           (long)p->burst_time,
           (long)p->remaining_time,
           p->priority,
           p->state);
}

void print_all_processes(const ProcessManager* pm)
{
    for (int i = 0; i < pm->process_count; i++) {
        print_process(&pm->processes[i]);
    }
}

