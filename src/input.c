#include <stdio.h>
#include <stdlib.h>
#include "input.h"


void input_processes_manual(ProcessManager* pm, int count)
{
    if (!pm) return;

    pm->process_count = 0;

    for (int i = 0; i < count; i++) {
        char name[MAX_PROCESS_NAME_LENGTH];
        int at, bt, pr;

        printf("Process %d\n", i + 1);

        printf("Name: ");
        scanf("%s", name);

        printf("Arrival Time: ");
        scanf("%d", &at);

        printf("Burst Time: ");
        scanf("%d", &bt);

        printf("Priority: ");
        scanf("%d", &pr);

        Process p = create_process(i + 1, name, at, bt, pr);
        add_process(pm, p);
    }
}



int input_processes_from_file(ProcessManager* pm, const char* filename)
{
    if (!pm || !filename) return -1;

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    pm->process_count = 0;

    int count;
    if (fscanf(file, "%d", &count) != 1) {
        printf("Invalid file format!\n");
        fclose(file);
        return -1;
    }

    for (int i = 0; i < count; i++) {
        int at, bt, pr;

        if (fscanf(file, "%d %d %d", &at, &bt, &pr) != 3) {
            printf("Invalid process data at line %d\n", i + 2);
            fclose(file);
            return -1;
        }

        Process p = create_process(i + 1, "", at, bt, pr);
        add_process(pm, p);
    }

    fclose(file);
    return 0;
}