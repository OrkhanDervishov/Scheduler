#ifndef OUTPUT_H
#define OUTPUT_H

#include "process.h"
#include "scheduler.h"
#include "metrics.h"

void output_print_results(const ProcessManager* pm, SchedulerConfig config);
void output_print_gantt_timeline(const ProcessManager* pm);
void output_print_comparison_table(const MetricsSummary* summary, const TypeMetrics type_metrics[PROCESS_TYPE_COUNT]);

#endif
