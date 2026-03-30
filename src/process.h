#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "common.h"

typedef enum{
    PROCESS_NEW = 0,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_WAITING,
    PROCESS_TERMINATED
} ProcessState;

typedef int pid_t;

#define MAX_PROCESS_NAME_LENGTH 32
typedef struct{
    bool empty;
    char name[MAX_PROCESS_NAME_LENGTH];
    pid_t pid;
    clock_t arrival_time;
    clock_t burst_time;
    clock_t remaining_time;
    clock_t completion_time;
    clock_t turnaround_time;
    clock_t waiting_time;
    uint8_t priority;
    ProcessState state;
} Process;

typedef struct{
    
    size_t process_count;
    size_t capacity;
    Process* processes;
} ProcessManager;



static inline Process create_process(const char* name, clock_t arrival, clock_t burst, uint8_t priority)
{
    Process p = {
        .empty = false,
        .pid = 0,
        .arrival_time = arrival,
        .burst_time = burst,
        .remaining_time = burst,
        .completion_time = 0,
        .turnaround_time = 0,
        .waiting_time = 0,
        .priority = priority,
        .state = PROCESS_NEW
    };
    strncpy(p.name, name, MAX_PROCESS_NAME_LENGTH);

    return p;
}

void init_process_manager(ProcessManager* pm, uint32_t max_capacity);
void free_process_manager(ProcessManager* pm);

int add_process(ProcessManager* pm, Process p);
int delete_process_by_pid(ProcessManager* pm, pid_t pid);


void print_process(const Process* p);
void print_all_processes(const ProcessManager* pm);


#endif