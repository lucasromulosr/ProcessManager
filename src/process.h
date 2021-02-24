#ifndef __PROCESS__
#define __PROCESS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSTRUCTION_N 100       // n instructions / process
#define INSTRUCTION_SIZE 20     // size of instruction value

typedef struct instruction_t{
    char type;
    char* value;
} instruction_t;

typedef struct process_t{
    int id, pid, start;        // from PM
    int pc, var, cpu_usage;    // initialized 0
    int priority, priority_i;  // priority, counter to schedule
    instruction_t** instruction;
} process_t;

/* -- PROCESS -- */
process_t* new_process(char*);
int set_priority(instruction_t**);

/* -- INSTRUCTION -- */
instruction_t** new_instructions(char*);

/* -- VAR -- */
void set_var(int, process_t*);
void add_var(int, process_t*);
void dec_var(int, process_t*);

#endif
