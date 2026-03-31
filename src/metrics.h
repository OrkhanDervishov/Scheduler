#ifndef METRICS_H
#define METRICS_H

#include "process.h"

typedef enum {
    PROCESS_TYPE_INTERACTIVE = 0,
    PROCESS_TYPE_CPU_BOUND,
    PROCESS_TYPE_MIXED,
    PROCESS_TYPE_COUNT
} ProcessType;

typedef struct {
    int count;
    double avg_waiting_time;
    double avg_turnaround_time;
} TypeMetrics;

typedef struct {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
    double throughput;
    double cpu_utilization;
    double fairness_dispersion;
    int starvation_count;
    uint32_t context_switches;
    clock_t makespan;
} MetricsSummary;

void metrics_calculate_process_times(ProcessManager* pm);
ProcessType metrics_classify_process_type(const Process* p);
const char* metrics_process_type_to_string(ProcessType type);
void metrics_calculate_type_metrics(const ProcessManager* pm, TypeMetrics out[PROCESS_TYPE_COUNT]);
void metrics_calculate_summary(const ProcessManager* pm, MetricsSummary* summary);

#endif
