#include "process.h"

/* ***** PROCESS ***** */
// creates a process and its intruction array
process_t* new_process(char* program){
    
    process_t* p = (process_t*) malloc (sizeof(process_t));
    p->pc = 0;
    p->var = 0;
    p->cpu_usage = 0;
    p->instruction = new_instructions(program);
    p->priority = set_priority(p->instruction);
    p->priority_i = p->priority;
    
    return p;
}

// priority = number of 'B' instructions
int set_priority(instruction_t** instruction){
    
    int priority = 0;
    char c;     // instruction type
    int i = 0;

    while((c = instruction[i++]->type) != 'E')
        if(c == 'B') priority++;
    
    return priority;
}

/* ***** INSTRUCTION ***** */
// creates a instruction array from the program file 
// its formated as "type value", ex "A 30"
// B and E instructions are just "type"
instruction_t** new_instructions(char* program){
    
    char file_name[] = "../programs/";
    strcat(file_name, program);
    
    instruction_t** instruction;
    instruction = (instruction_t**) malloc (INSTRUCTION_N * sizeof(instruction_t*));
    for(int i = 0; i < INSTRUCTION_N; i++){
        instruction[i] = (instruction_t*) malloc (sizeof(instruction_t));
        instruction[i]->value = (char*) malloc (INSTRUCTION_SIZE * sizeof(char));
    }
    
    FILE* file = fopen(file_name, "rt");
	if(file == NULL) {
		printf("_/> Theres was a problem opening file. Please (t)erminate. _");  // dbg
		exit(EXIT_FAILURE);
    }
    
    int i = 0;  // index
	char type, value[INSTRUCTION_SIZE];
    
    // goes through extracting instructions
	while(!feof(file)){
        
        strcpy(value, "");                  // "NULL"s value 
        
        fscanf(file, "%c ", &type);         // gets type
        if(!(type == 'B' || type == 'E'))
            fscanf(file, "%s ", value);     // gets value
        
        // save into array
        instruction[i]->type = type;
        strcpy(instruction[i]->value, value);
        
	    i++;
	}
	
	fclose(file);
    
    return instruction;
}

/* ***** VAR ***** */
void set_var(int value, process_t* p){
    p->var = value; // set var
}

void add_var(int value, process_t* p){
    p->var+=value;  // adds to var
}

void dec_var(int value, process_t* p){
    p->var-=value;  // decreases from var
}




















