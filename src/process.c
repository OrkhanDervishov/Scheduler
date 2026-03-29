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

int resize_process_manager(ProcessManager* pm, uint32_t new_capacity){
    if(pm->processes){
        init_process_manager(pm, new_capacity);
        return 0;
    }

    Process *processes = (Process*)malloc(sizeof(Process) * new_capacity);
    int process_count = 0;

    for(size_t i = 0; i < pm->capacity; i++){
        processes[i] = pm->processes[i];
        if(processes[i].empty) process_count++;
    }

    free(pm->processes);
    pm->process_count = process_count;
    pm->processes = processes;
    pm->capacity = new_capacity;

    return 0;
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

    for(int i = 0; i < pm->capacity; i++){
        if(pm->processes[i].empty){
            pm->processes[i] = p;
            pm->processes[i].pid = i;
            pm->process_count++;
            return 0;
        }
    }
    
    return 0;
}

int delete_process_by_pid(ProcessManager* pm, pid_t pid)
{
    if (!pm || pm->process_count == 0) return -1;

    if(!pm->processes[pid].empty){
        pm->processes[pid].empty = true;
        return 0;
    }

    // Find process
    // for (size_t i = 0; i < pm->process_count; i++) {
    //     if (pm->processes[i].pid == pid) {
    //         pm->processes[i].empty = true;
    //         pm->process_count--;
    //         return 0;
    //     }
    // }

    return -2;
}

//===============================================================
// Visualization

void print_process(const Process* p)
{
    printf("PID: %d | Name:%s | AT: %zu | BT: %zu | RT: %zu | PR: %u | STATE: %d\n",
        p->pid,
        p->name,
        p->arrival_time,
        p->burst_time,
        p->remaining_time,
        p->priority,
        p->state
    );
}

void print_all_processes(const ProcessManager* pm)
{
    for (int i = 0; i < pm->capacity; i++) {
        if(!pm->processes[i].empty)
            print_process(&pm->processes[i]);
    }
}

