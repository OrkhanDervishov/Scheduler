#include <stdio.h>

#include "output.h"

static const int TYPE_COLUMN_WIDTH = 12;

static const char* scheduling_to_string(Scheduling type)
{
    switch (type) {
        case MLQ_SCHEDULING: return "Multilevel Queue";
        case ROUND_ROBIN_SCHEDULING: return "Round Robin";
        case PRIORITY_SCHEDULING: return "Priority";
        case SJF_SCHEDULING: return "SJF";
        case FCFS_SCHEDULING: return "FCFS";
        default: return "Unknown";
    }
}

void output_print_gantt_timeline(const ProcessManager* pm)
{
    int picked[1000] = {0};

    printf("\nGantt-style Timeline (completion order approximation):\n");

    for (int i = 0; i < pm->process_count; i++) {
        int idx = -1;
        clock_t best_completion = 0;

        for (int j = 0; j < pm->process_count; j++) {
            const Process* p = &pm->processes[j];
            if (picked[j]) continue;

            if (idx == -1 || p->completion_time < best_completion) {
                idx = j;
                best_completion = p->completion_time;
            }
        }

        if (idx != -1) {
            const Process* p = &pm->processes[idx];
            printf("| P%d ", p->pid);
            picked[idx] = 1;
        }
    }
    printf("|\n");

    for (int i = 0; i < pm->process_count; i++) picked[i] = 0;

    for (int i = 0; i < pm->process_count; i++) {
        int idx = -1;
        clock_t best_completion = 0;

        for (int j = 0; j < pm->process_count; j++) {
            const Process* p = &pm->processes[j];
            if (picked[j]) continue;

            if (idx == -1 || p->completion_time < best_completion) {
                idx = j;
                best_completion = p->completion_time;
            }
        }

        if (idx != -1) {
            const Process* p = &pm->processes[idx];
            printf("%d\t", (int)p->completion_time);
            picked[idx] = 1;
        }
    }
    printf("\n");
}

void output_print_comparison_table(const MetricsSummary* summary, const TypeMetrics type_metrics[PROCESS_TYPE_COUNT])
{
    printf("\nOverall Comparison Table:\n");
    printf("%-20s %s\n", "Metric", "Value");
    printf("%-20s %.2f\n", "Avg Waiting Time", summary->avg_waiting_time);
    printf("%-20s %.2f\n", "Avg Turnaround Time", summary->avg_turnaround_time);
    printf("%-20s %.2f\n", "Avg Response Time", summary->avg_response_time);
    printf("%-20s %.4f proc/unit\n", "Throughput", summary->throughput);
    printf("%-20s %.2f%%\n", "CPU Utilization", summary->cpu_utilization);
    printf("%-20s %.2f\n", "Fairness Dispersion", summary->fairness_dispersion);
    printf("%-20s %d\n", "Starvation Count", summary->starvation_count);
    printf("%-20s %u\n", "Context Switches", summary->context_switches);

    printf("\nPer-Type Metrics:\n");
    printf("Type\t\tCount\tAvg WT\tAvg TAT\n");
    for (int i = 0; i < PROCESS_TYPE_COUNT; i++) {
        if (type_metrics[i].count == 0) {
            printf("%-*s\t%d\tN/A\tN/A\n",
                   TYPE_COLUMN_WIDTH,
                   metrics_process_type_to_string((ProcessType)i),
                   type_metrics[i].count);
            continue;
        }

        printf("%-*s\t%d\t%.2f\t%.2f\n",
               TYPE_COLUMN_WIDTH,
               metrics_process_type_to_string((ProcessType)i),
               type_metrics[i].count,
               type_metrics[i].avg_waiting_time,
               type_metrics[i].avg_turnaround_time);
    }
}

void output_print_results(const ProcessManager* pm, SchedulerConfig config)
{
    MetricsSummary summary;
    TypeMetrics type_metrics[PROCESS_TYPE_COUNT];
    const char* algorithm_name = scheduling_to_string(config.type);

    metrics_calculate_summary(pm, &summary);
    metrics_calculate_type_metrics(pm, type_metrics);

    printf("\n=== Scheduling Metrics Summary ===\n");
    if (config.type == ROUND_ROBIN_SCHEDULING) {
        printf("Algorithm: %s (Q=%d)\n", algorithm_name, config.quantum);
    } else {
        printf("Algorithm: %s\n", algorithm_name);
    }
    printf("\nPID\tAT\tBT\tFRT\tRT\tCT\tWT\tTAT\tType\n");

    for (int i = 0; i < pm->process_count; i++) {
        const Process* p = &pm->processes[i];
        ProcessType type = metrics_classify_process_type(p);

        if (p->first_run_time == UNINITIALIZED_TIME || p->response_time == UNINITIALIZED_TIME) {
            printf("%d\t%d\t%d\tN/A\tN/A\t%d\t%ld\t%ld\t%s\n",
                   p->pid,
                   (int)p->arrival_time,
                   (int)p->burst_time,
                   (int)p->completion_time,
                   p->waiting_time,
                   p->turnaround_time,
                   metrics_process_type_to_string(type));
        } else {
            printf("%d\t%d\t%d\t%d\t%ld\t%d\t%ld\t%ld\t%s\n",
                   p->pid,
                   (int)p->arrival_time,
                   (int)p->burst_time,
                   (int)p->first_run_time,
                   p->response_time,
                   (int)p->completion_time,
                   p->waiting_time,
                   p->turnaround_time,
                   metrics_process_type_to_string(type));
        }
    }

    output_print_gantt_timeline(pm);
    output_print_comparison_table(&summary, type_metrics);
}
