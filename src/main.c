#include <stdio.h>
#include "scheduler.h"
#include "tests.h"

int main() {
    printf("Running Tests...\n\n");

    test_process_creation();
    test_process_manager_add_delete();
    test_input_manual_simulation();
    test_generator_cpu_bound();
    test_generator_interactive();
    test_generator_priority_heavy();
    test_file_input();
    test_simulator();
    

    printf("\nAll tests completed successfully\n");

    return 0;
}