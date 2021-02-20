#include "manager.h"

void reporter(process_manager_t* pm){
    
//  pid, ppid, /prioridade/, valor, tempo inicio, CPU usada ate agora
    
    int id;
    process_t* p;
    
    printf("***********************************\n");
    printf("Estado do sistema:\n");
    printf("***********************************\\\\\n");
    printf("TEMPO ATUAL: %d\n", pm->timer);
    
    printf("TABELA PCB:\n");
    for(int i = 0; i < pm->next_pcb; i++){
        p = pm->pcb_table[i];
        printf("%d %d %d %d %d \n", p->id, p->pid, p->var, p->start, p->cpu_usage);
    }
    
    printf("BLOQUEADO:\n");
    for(int i = 0; i < pm->next_locked; i++){
        printf("%d - ", pm->locked[i]);   // dbg
        id = pm->locked[i];
        p = pm->pcb_table[id];
        printf("%d %d %d %d %d \n", p->id, p->pid, p->var, p->start, p->cpu_usage);
    }

    printf("PRONTO:\n");
    for(int i = 0; i < pm->next_ready; i++){
        printf("%d - ", pm->ready[i]);   // dbg
        id = pm->ready[i];
        p = pm->pcb_table[id];
        printf("%d %d %d %d %d \n", p->id, p->pid, p->var, p->start, p->cpu_usage);
    }
    
    printf("EXECUTANDO:\n");
        if((p = pm->cpu) != NULL)
        printf("%d %d %d %d %d \n", p->id, p->pid, p->var, p->start, p->cpu_usage);
}

// TIRAR DBG
process_manager_t* initialize_process_manager(){
    
    process_manager_t* pm;
    pm = (process_manager_t*) malloc (sizeof(process_manager_t));
    pm->pcb_table = new_pcb_table();
    pm->ready = new_line();
    pm->locked = new_line();
    
    pm->timer = 0;
    pm->next_id = 0;
    pm->next_pcb = 0;
    pm->next_ready = 0;
    pm->next_locked = 0;
    
    pm->pcb_table[pm->next_pcb++] = 
    create_process(&pm->next_id, -1, pm->timer, "../init.txt");
    
    pm->cpu = pm->pcb_table[0];
    
    return pm;
}

process_t* create_process(int *id, int pid, int timer, char* program){

    process_t* process = new_process(program);

    process->id = *id;
	process->pid = pid;
	process->start = timer;

	(*id)++;

	return process;
}

process_t** new_pcb_table(){

    process_t** pcb_table = (process_t**) malloc (PROCESS_N * sizeof(process_t*));
    for(int i = 0; i < PROCESS_N; i++)
        pcb_table[i] = (process_t*) malloc (sizeof(process_t));
    
    return pcb_table;
}

/* *********** EDITING *********** */


void execute(process_manager_t* pm){

    if(pm->cpu == NULL){
        printf("NULL CPU <<<----\n");
        return;
    }
    
    process_t* cpu = pm->cpu;
    instruction_t* instruction;
    instruction = cpu->instruction[cpu->pc++];
    
    char c = instruction->type;
    int value;
    if(c == 'S' || c== 'A' || c == 'D' || c == 'F')
        value = atoi(instruction->value);
    
    cpu->cpu_usage++;
    switch( c ){
        case 'S': atualize_var(value, cpu); break;
        case 'A': add_var(value, cpu); break;
        case 'D': sub_var(value, cpu); break;
        case 'B': lock_process(pm); break;
        case 'E': remove_process(pm); break;
        case 'F': child_process(value, pm); break;
        case 'R': change_image(instruction->value, cpu); break;
    }
    
    printf("instruct %c %s\n", instruction->type, instruction->value);
    scheduler(pm);
}


void child_process(int value, process_manager_t* pm){
    
    process_t* cpu = pm->cpu;
    process_t* child = (process_t*) malloc (sizeof(process_t));
    
    cp_process(child, cpu);
    child->id = pm->next_id++;
    child->pid = cpu->id;
    child->start = pm->timer;
    child->cpu_usage = 0;
    
    cpu->pc+=value;
    
    pm->ready[pm->next_ready++] = pm->next_pcb;
    pm->pcb_table[pm->next_pcb++] = child;
}

void change_image(char* program, process_t* p){
    
    p->pc = 0;
    p->var = 0;
    p->instruction = new_instructions(program);
    
}






/* *********** END EDITING *********** */




void scheduler(process_manager_t* pm){

    int id = -1;
    if(pm->cpu != NULL)
        for(int i = 0; i < pm->next_pcb; i++)
            if(pm->cpu->id == pm->pcb_table[i]->id)
                id = i;
    printf("ID FOUND %d <<<---- \n", id);   // dbg
    
    if(id != -1)
        add_line(id, pm->ready, &pm->next_ready);
    
    ready_to_cpu(pm);
}

void ready_to_cpu(process_manager_t* pm){
    
    if(pm->next_ready > 0){
        pm->cpu = pm->pcb_table[pm->ready[0]];
        move_line(pm->ready, &pm->next_ready);
    }
    else pm->cpu = NULL;
}

void lock_process(process_manager_t* pm){
    
    int match = -1;
    for(int i = 0; i < pm->next_pcb; i++)
        if(pm->cpu->id == pm->pcb_table[i]->id)
            match = i;
        
    printf("MATCH FOUND %d <<----\n", match);   // dbg
    
    pm->locked[pm->next_locked++] = match;
    
    if(pm->ready)
    
    pm->cpu = NULL;
}

void unlock_process(process_manager_t* pm){

    if(pm->locked[0] != -1){
        
        if(pm->cpu == NULL)
            pm->cpu = pm->pcb_table[pm->locked[0]];
        else
            add_line(pm->locked[0], pm->ready, &pm->next_ready);  
        
        move_line(pm->locked, &pm->next_locked);
        
    }
}

void remove_process(process_manager_t* pm){   // remover processo da CPU
    
//     process_t* p;
    int id = -1;
    
    for(int i = 0; i < pm->next_pcb; i++)
        if(pm->cpu->id == pm->pcb_table[i]->id)
            id = i;
        
    free(pm->pcb_table[id]);
    
    for(int i = id; i < pm->next_pcb-1; i++){
        
        cp_process(pm->pcb_table[i], pm->pcb_table[i+1]);
        free(pm->pcb_table[i+1]);
    }
    
    for(int i = 0; i < pm->next_ready; i++)
        if(pm->ready[i] > id)
            pm->ready[i]--;
    
    for(int i = 0; i < pm->next_locked; i++)
        if(pm->locked[i] > id)
            pm->locked[i]--;
        
    pm->next_pcb--;
    pm->cpu = NULL;
}

/* ******** LINES ******** */
int* new_line(){
    
    int* line;
    line = (int*) malloc (PROCESS_N * sizeof(int));
    
    for(int i = 0; i < PROCESS_N; i++)
        line[i] = -1;
    
    return line;
}

void move_line(int* line, int* size){
    for(int i = 0; i < *size; i++)
        line[i] = line[i+1];
    (*size)--;
}

void add_line(int x, int* line, int* size){
    line[(*size)++] = x;
}
