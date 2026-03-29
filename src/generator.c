#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "generator.h"


/* Main dispatcher */
void generate_workload(ProcessManager* pm, int count, WorkloadType type)
{
    if (!pm) return;

    pm->process_count = 0;

    switch (type) {
        case WORKLOAD_CPU_BOUND:
            generate_cpu_bound(pm, count);
            break;
        case WORKLOAD_INTERACTIVE:
            generate_interactive(pm, count);
            break;
        case WORKLOAD_MIXED:
            generate_mixed(pm, count);
            break;
        case WORKLOAD_PRIORITY_HEAVY:
            generate_priority_heavy(pm, count);
            break;
    }
}

/* ================= CPU-BOUND =================
   Long burst times, fewer arrivals */
void generate_cpu_bound(ProcessManager* pm, int count)
{
    for (int i = 0; i < count; i++) {
        Process p = create_process(
            i + 1,
            "generated",
            rand_range(0, 5),          // arrival
            rand_range(20, 50),        // long burst
            rand_range(1, 5)
        );

        add_process(pm, p);
    }
}

/* ================= INTERACTIVE =================
   Short burst times, frequent arrivals */
void generate_interactive(ProcessManager* pm, int count)
{
    for (int i = 0; i < count; i++) {
        Process p = create_process(
            i + 1,
            "generated",
            rand_range(0, 10),         // more spread arrivals
            rand_range(1, 5),          // short burst
            rand_range(1, 5)
        );

        add_process(pm, p);
    }
}

/* ================= MIXED ================= */
void generate_mixed(ProcessManager* pm, int count)
{
    for (int i = 0; i < count; i++) {

        int burst = (rand() % 2 == 0)
                    ? rand_range(1, 5)     // short
                    : rand_range(20, 40);  // long

        Process p = create_process(
            i + 1,
            "generated",
            rand_range(0, 10),
            burst,
            rand_range(1, 5)
        );

        add_process(pm, p);
    }
}

/* ================= PRIORITY HEAVY =================
   Many low priority, few high priority */
void generate_priority_heavy(ProcessManager* pm, int count)
{
    for (int i = 0; i < count; i++) {

        uint8_t priority = (rand() % 100 < 70)
                           ? rand_range(3, 5)  // mostly low priority
                           : rand_range(1, 2); // few high priority

        Process p = create_process(
            i + 1,
            "generated",
            rand_range(0, 10),
            rand_range(5, 20),
            priority
        );

        add_process(pm, p);
    }
}