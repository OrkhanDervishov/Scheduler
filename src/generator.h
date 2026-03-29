#ifndef GENERATOR_H
#define GENERATOR_H

#include "process.h"


typedef enum {
    WORKLOAD_CPU_BOUND,
    WORKLOAD_INTERACTIVE,
    WORKLOAD_MIXED,
    WORKLOAD_PRIORITY_HEAVY
} WorkloadType;



void generate_workload(ProcessManager* pm, int count, WorkloadType type);

/* Individual generators */
void generate_cpu_bound(ProcessManager* pm, int count);
void generate_interactive(ProcessManager* pm, int count);
void generate_mixed(ProcessManager* pm, int count);
void generate_priority_heavy(ProcessManager* pm, int count);

#endif