#ifndef INPUT_H
#define INPUT_H

#include "process.h"

// Manual input
void input_processes_manual(ProcessManager* pm, int count);

// File input
int input_processes_from_file(ProcessManager* pm, const char* filename);

#endif