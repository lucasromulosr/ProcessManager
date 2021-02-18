#ifndef __PROCESS__
#define __PROCESS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSTRUCTION_N 100
#define INSTRUCTION_SIZE 20

typedef struct instruction_t{
	char type;
	char* value;
} instruction_t;

typedef struct process_t{
	int id, pid, pc;
	int var, start;
    int cpu_usage;
 	instruction_t** instruction;
} process_t;

process_t* new_process();
instruction_t** new_instructions(char*);

void execute(); // START THAT SHIT

#endif
