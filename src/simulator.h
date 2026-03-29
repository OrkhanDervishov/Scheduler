#ifndef SIMULATION_H
#define SIMULATION_H

#include "process.h"
#include "scheduler.h"

typedef struct {
    int context_switch_time;
} SimulationConfig;

void run_simulation(ProcessManager* pm, SimulationConfig config);

#endif