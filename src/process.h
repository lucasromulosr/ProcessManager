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
 	instruction_t** instruction;
} process_t;

/* -- PROCESS -- */
process_t* new_process(char*);

/* -- INSTRUCTION -- */
instruction_t** new_instructions(char*);

/* -- VAR -- */
void set_var(int, process_t*);
void add_var(int, process_t*);
void dec_var(int, process_t*);

#endif

/*
    id, pid, start são definidos pelo manager
    
    pc, var e cpu_usage iniciam em 0
    
    instruction é uma tabela de instrucoes
        extraidas do arquivo (ele n fica aberto).
        ex: {A, "55"}, {E, ""}.
        As instrucoes E e B possuem a segunda
        posicao vazia.
        
 */
