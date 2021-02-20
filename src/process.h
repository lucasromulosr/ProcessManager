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
	int id, pid, start;        // from PM
	int pc, var, cpu_usage;    // initialized 0
 	instruction_t** instruction;
} process_t;

/* -- PROCESS -- */
process_t* new_process(char*);
void cp_process(process_t*, process_t*);

/* -- INSTRUCTION -- */
instruction_t** new_instructions(char*);

/* -- VAR -- */
void atualize_var(int, process_t*);
void add_var(int, process_t*);
void sub_var(int, process_t*);

#endif
