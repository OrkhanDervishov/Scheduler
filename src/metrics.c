#include "metrics.h"

static const clock_t INTERACTIVE_BURST_THRESHOLD = 5;
static const clock_t CPU_BOUND_BURST_THRESHOLD = 20;

void metrics_calculate_process_times(ProcessManager* pm)
{
    for (int i = 0; i < pm->process_count; i++) {
        Process* p = &pm->processes[i];

        p->turnaround_time = p->completion_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = (p->first_run_time == UNINITIALIZED_TIME)
            ? UNINITIALIZED_TIME
            : (p->first_run_time - p->arrival_time);
    }
}

ProcessType metrics_classify_process_type(const Process* p)
{
    if (p->burst_time <= INTERACTIVE_BURST_THRESHOLD) return PROCESS_TYPE_INTERACTIVE;
    if (p->burst_time >= CPU_BOUND_BURST_THRESHOLD) return PROCESS_TYPE_CPU_BOUND;
    return PROCESS_TYPE_MIXED;
}

const char* metrics_process_type_to_string(ProcessType type)
{
    switch (type) {
        case PROCESS_TYPE_INTERACTIVE: return "Interactive";
        case PROCESS_TYPE_CPU_BOUND: return "CPU-bound";
        case PROCESS_TYPE_MIXED: return "Mixed";
        default: return "Unknown";
    }
}

void metrics_calculate_type_metrics(const ProcessManager* pm, TypeMetrics out[PROCESS_TYPE_COUNT])
{
    double total_wt[PROCESS_TYPE_COUNT] = {0};
    double total_tat[PROCESS_TYPE_COUNT] = {0};
    int counts[PROCESS_TYPE_COUNT] = {0};

    for (int i = 0; i < pm->process_count; i++) {
        const Process* p = &pm->processes[i];
        ProcessType type = metrics_classify_process_type(p);

        counts[type]++;
        total_wt[type] += p->waiting_time;
        total_tat[type] += p->turnaround_time;
    }

    for (int i = 0; i < PROCESS_TYPE_COUNT; i++) {
        out[i].count = counts[i];
        out[i].avg_waiting_time = (counts[i] == 0) ? 0.0 : (total_wt[i] / counts[i]);
        out[i].avg_turnaround_time = (counts[i] == 0) ? 0.0 : (total_tat[i] / counts[i]);
    }
}

void metrics_calculate_summary(const ProcessManager* pm, MetricsSummary* summary)
{
    double total_wt = 0;
    double total_tat = 0;
    double total_rt = 0;
    double total_bt = 0;
    clock_t min_arrival = 0;
    clock_t max_completion = 0;
    double mean_wt = 0;
    double sq_diff_sum = 0;
    int starvation = 0;
    int responded_count = 0;

    if (pm->process_count == 0) {
        *summary = (MetricsSummary){0};
        return;
    }

    min_arrival = pm->processes[0].arrival_time;
    max_completion = pm->processes[0].completion_time;

    for (int i = 0; i < pm->process_count; i++) {
        const Process* p = &pm->processes[i];

        if (p->arrival_time < min_arrival) min_arrival = p->arrival_time;
        if (p->completion_time > max_completion) max_completion = p->completion_time;

        total_wt += p->waiting_time;
        total_tat += p->turnaround_time;
        if (p->response_time != UNINITIALIZED_TIME) {
            total_rt += p->response_time;
            responded_count++;
        }
        total_bt += p->burst_time;
    }

    mean_wt = total_wt / pm->process_count;

    for (int i = 0; i < pm->process_count; i++) {
        const Process* p = &pm->processes[i];
        double d = p->waiting_time - mean_wt;

        sq_diff_sum += d * d;
        if (p->waiting_time > (2 * p->burst_time)) {
            starvation++;
        }
    }

    summary->avg_waiting_time = mean_wt;
    summary->avg_turnaround_time = total_tat / pm->process_count;
    summary->avg_response_time = (responded_count > 0) ? (total_rt / responded_count) : 0.0;
    summary->makespan = max_completion - min_arrival;
    summary->throughput = (summary->makespan > 0) ? ((double)pm->process_count / summary->makespan) : 0.0;
    summary->cpu_utilization = (summary->makespan > 0) ? ((total_bt / summary->makespan) * 100.0) : 0.0;
    summary->fairness_dispersion = sq_diff_sum / pm->process_count;
    summary->starvation_count = starvation;
    summary->context_switches = pm->context_switches;
}
