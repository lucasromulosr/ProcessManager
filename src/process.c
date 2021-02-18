#include "process.h"

process_t* new_process(){
    return (process_t*) malloc (sizeof(process_t));
}

instruction_t** new_instructions(char* program){
    
    instruction_t** instruction;
    instruction = (instruction_t**) malloc (INSTRUCTION_N * sizeof(instruction_t*));
    for(int i = 0; i < INSTRUCTION_N; i++){
        instruction[i] = (instruction_t*) malloc (sizeof(instruction_t));
        instruction[i]->value = (char*) malloc (INSTRUCTION_SIZE * sizeof(char));
    }
    
    printf("%s <---\n", program);
    FILE* file = fopen(program, "rt");
	if(file == NULL) {
		printf("PROBLEMA EM ABRIR O ARQUIVO\n");
		exit(EXIT_FAILURE);
	} else printf("ABRIU O ARQUIVO!\n");   // debug - tirar cmm dps

    int i = 0;
	char c, line[INSTRUCTION_SIZE];
	while(!feof(file)){
		fscanf(file, "%c %s\n", &c, line);
        
        instruction[i]->type = c;
        strcpy(instruction[i]->value, line);
        
	    i++;
	}
	fclose(file);
    
    return instruction;
}
