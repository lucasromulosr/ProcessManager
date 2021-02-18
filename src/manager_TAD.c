#include "manager.h"

process_t* create_process(int *id, int pid, int timer, char* program){

    
    process_t* process = new_process();

    process->id = *id;
	process->pid = pid;
	process->pc = 0;
	process->var = -55;
	process->start = timer;
    process->instruction = new_instructions(program);

	(*id)++;

	return process;
}

process_t** new_pcb_table(){

    process_t** pcb_table = (process_t**) malloc (PROCESS_N * sizeof(process_t*));
    for(int i = 0; i < PROCESS_N; i++)
        pcb_table[i] = (process_t*) malloc (sizeof(process_t));
    
    return pcb_table;
}

process_list_t* new_process_list(){
    
    process_list_t* list = (process_list_t*) malloc (sizeof(process_list_t));
    list->process_id = (int*) malloc (PROCESS_N * sizeof(int));
    for(int i = 0; i < PROCESS_N; i++)
        list->process_id[i] = -1;
    list->next = 0;
    
    return list;
}

void unlock_process(process_list_t* locked, process_list_t* ready){

    if(locked->process_id[0] != -1){
        
        ready->process_id[ready->next] = locked->process_id[0];
        (ready->next)++;
        
        for(int i = 0; i < locked->next; i++)
            locked->process_id[i] = locked->process_id[i+1];
        (locked->next)--;
    }
}



void reporter(int timer, process_t* cpu, process_t** pcb_table, process_list_t* locked, process_list_t* ready){
    
//  pid, ppid, /prioridade/, valor, tempo inicio, /CPU usada ate agora/
    
    int id;
    process_t* p;
    
    printf("***********************************\n");
    printf("Estado do sistema:\n");
    printf("***********************************\\\\\n");
    printf("TEMPO ATUAL: %d\n", timer);
    
    printf("BLOQUEADO:\n");
    for(int i = 0; i < locked->next; i++){
        id = locked->process_id[i];
        p = pcb_table[id];
        printf("%d %d %d %d \n", p->id, p->pid, p->var, p->start);
    }

    printf("PRONTO:\n");
    for(int i = 0; i < ready->next; i++){
        id = ready->process_id[i];
        p = pcb_table[id];
        printf("%d %d %d %d \n", p->id, p->pid, p->var, p->start);
    }
    
    printf("EXECUTANDO:\n");
        printf("%d %d %d %d \n", cpu->id, cpu->pid, cpu->var, cpu->start);
}



void remove_process(int id, int* next_id, process_t** pcb_table){
    
    int match = -1;
    
    for(int i = 0; i < *next_id; i++)
        if(pcb_table[i]->id == id)
            match = i;
        
    if(match != -1){

        free(pcb_table[match]);

        for(int i = match; i < *next_id; i++)
            pcb_table[i] = pcb_table[i+1];

        (*next_id)--;        
    }
}
