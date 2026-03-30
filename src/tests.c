#include "tests.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "process.h"
#include "input.h"
#include "generator.h"
#include "simulator.h"


void test_process_creation() {

    Process p = create_process("test_process", 0, 10, 2);

    assert(p.arrival_time == 0);
    assert(p.burst_time == 10);
    assert(p.remaining_time == 10);
    assert(p.priority == 2);
    assert(p.state == PROCESS_NEW);

    printf("Process creation test passed\n");
}

void test_process_manager_add_delete() {
    ProcessManager pm;
    init_process_manager(&pm, 10);

    Process p1 = create_process("test_process_0", 0, 5, 1);
    Process p2 = create_process("test_process_1", 1, 3, 2);

    add_process(&pm, p1);
    add_process(&pm, p2);

    print_all_processes(&pm);
    assert(pm.process_count == 2);

    delete_process_by_pid(&pm, 1);
    assert(pm.process_count == 1);
    // assert(pm.processes[0].pid == 2);

    free_process_manager(&pm);

    printf("Process manager add/delete test passed\n");
}

void test_input_manual_simulation() {

    ProcessManager pm;
    init_process_manager(&pm, 10);

    add_process(&pm, create_process("test_process_0", 0, 5, 1));
    add_process(&pm, create_process("test_process_0", 1, 3, 2));

    assert(pm.process_count == 2);

    printf("Manual input simulation test passed\n");

    free_process_manager(&pm);
}

void test_generator_cpu_bound() {
    ProcessManager pm;
    init_process_manager(&pm, 10);

    generate_workload(&pm, 5, WORKLOAD_CPU_BOUND);

    assert(pm.process_count == 5);

    for (int i = 0; i < pm.process_count; i++) {
        assert(pm.processes[i].burst_time >= 20);
    }

    printf("CPU-bound generator test passed\n");

    free_process_manager(&pm);
}

void test_generator_interactive() {
    ProcessManager pm;
    init_process_manager(&pm, 10);

    generate_workload(&pm, 5, WORKLOAD_INTERACTIVE);

    assert(pm.process_count == 5);

    for (int i = 0; i < pm.process_count; i++) {
        assert(pm.processes[i].burst_time <= 5);
    }

    printf("Interactive generator test passed\n");

    free_process_manager(&pm);
}

void test_generator_priority_heavy() {
    ProcessManager pm;
    init_process_manager(&pm, 100);

    generate_workload(&pm, 20, WORKLOAD_PRIORITY_HEAVY);

    assert(pm.process_count == 20);

    int high_priority_count = 0;

    for (int i = 0; i < pm.process_count; i++) {
        if (pm.processes[i].priority <= 2)
            high_priority_count++;
    }

    assert(high_priority_count > 0); // should have some high priority tasks

    printf("Priority-heavy generator test passed\n");

    free_process_manager(&pm);
}

/* ===================== FILE INPUT TEST ===================== */

void test_file_input() {
    ProcessManager pm;
    init_process_manager(&pm, 100);


    int result = input_processes_from_file(&pm, "input_files/input0.txt");

    if (result == 0) {
        assert(pm.process_count > 0);
        printf("File input test passed\n");
        free_process_manager(&pm);
    } else {
        printf("File input test skipped (file not found)\n");
    }
}

void test_simulator(){
    ProcessManager pm;
    init_process_manager(&pm, 10);

    srand(time(NULL));
    generate_workload(&pm, 5, WORKLOAD_MIXED);

    SchedulerConfig sched = {
        .type = ROUND_ROBIN_SCHEDULING,
        .quantum = 2
    };

    set_scheduling_algorithm(sched);

    SimulationConfig sim = {
        .context_switch_time = 1
    };  

    run_simulation(&pm, sim);

    free_process_manager(&pm);
    return;
}