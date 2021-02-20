#include "process.h"

process_t* new_process(char* program){
    
    process_t* p = (process_t*) malloc (sizeof(process_t));
    p->pc = 0;
    p->var = 0;
    p->cpu_usage = 0;
    p->instruction = new_instructions(program);
    
    return p;
}

void cp_process(process_t* copy, process_t* p){
    
    copy->id = (int)p->id;
    copy->pid = (int)p->pid;
    copy->start = (int)p->start;
    copy->pc = (int)p->pc;
    copy->var = (int)p->var;
    copy->cpu_usage = (int)p->cpu_usage;
    copy->instruction = (instruction_t**) p->instruction;
}

instruction_t** new_instructions(char* program){
    
    instruction_t** instruction;
    instruction = (instruction_t**) malloc (INSTRUCTION_N * sizeof(instruction_t*));
    for(int i = 0; i < INSTRUCTION_N; i++){
        instruction[i] = (instruction_t*) malloc (sizeof(instruction_t));
        instruction[i]->value = (char*) malloc (INSTRUCTION_SIZE * sizeof(char));
    }
    
    printf("%s <---\n", program);   // dbg
    FILE* file = fopen(program, "rt");
	if(file == NULL) {
		printf("PROBLEMA EM ABRIR O ARQUIVO\n");  // dbg
		exit(EXIT_FAILURE);
	} else printf("ABRIU O ARQUIVO!\n");   // dbg

    int i = 0;
	char c, line[INSTRUCTION_SIZE];
	while(!feof(file)){
        strcpy(line, "");
        
        fscanf(file, "%c ", &c);
        if(!(c == 'B' || c == 'E'))
            fscanf(file, "%s ", line);
        
        instruction[i]->type = c;
        strcpy(instruction[i]->value, line);
        
	    i++;
	}
	fclose(file);
    
    return instruction;
}

void atualize_var(int value, process_t* p){
    p->var = value;
}

void add_var(int value, process_t* p){
    p->var+=value;
}

void sub_var(int value, process_t* p){
    p->var-=value;
}




















