#include "manager.h"

/* ***** REPORTER ***** */
void reporter(process_manager_t* pm){
    
//  pid, ppid, /prioridade/, valor, tempo inicio, CPU usada ate agora
    
    int id;
    process_t* p;
    
    printf("***********************************\n");
    printf("Sys status:\n");
    printf("***********************************\\\\\n");
    printf("TIME RUNNING: \t%d\n", pm->timer);
    
    printf("|pid\t|ppid\t|var\t|start\t|cpu_usage\n");
    
    printf("|RUNNING PROCESS:\n");
        if((p = pm->cpu) != NULL)
        printf("|%d \t|%d \t|%d \t|%d \t|%d \n", 
               p->id, p->pid, p->var, p->start, p->cpu_usage);
    
    printf("|LOCKED PROCESSES:\n");
    for(int i = 0; i < pm->next_locked; i++){
        id = pm->locked[i];
        p = pm->pcb_table[id];
        printf("|%d \t|%d \t|%d \t|%d \t|%d \n", 
               p->id, p->pid, p->var, p->start, p->cpu_usage);
    }

    printf("|READY PROCESSES:\n");
    for(int i = 0; i < pm->next_ready; i++){
        id = pm->ready[i];
        p = pm->pcb_table[id];
        printf("|%d \t|%d \t|%d \t|%d \t|%d \n", 
               p->id, p->pid, p->var, p->start, p->cpu_usage);
    }
}

/* ***** INITIALIZATION ***** */
// initializes process manager
process_manager_t* initialize_process_manager(){
    
    // creates process manager
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
    
    // creates first process
    // increments next_id and next_pcb
    pm->pcb_table[pm->next_pcb++] = 
        first_process(&pm->next_id, 0, pm->timer, "../init.txt");
    
    // adds process to pcb table
    pm->cpu = pm->pcb_table[0];
    
    return pm;
}

// creates first process from init file
process_t* first_process(int *id, int pid, int timer, char* program){

    // creates process from init file
    process_t* process = new_process(program);

    process->id = *id;
	process->pid = pid;
	process->start = timer;

    // increments next_id
	(*id)++;

	return process;
}

// creates pcb table
process_t** new_pcb_table(){

    process_t** pcb_table = (process_t**) malloc (PROCESS_N * sizeof(process_t*));
    for(int i = 0; i < PROCESS_N; i++)
        pcb_table[i] = (process_t*) malloc (sizeof(process_t));
    
    return pcb_table;
}

/* *********** EDITING *********** */


void execute(process_manager_t* pm){

    if(pm->cpu == NULL){
        printf("_/> Theres no process running now.\n");
        return;
    }
    
    process_t* cpu = pm->cpu;
    instruction_t* instruction;
    instruction = cpu->instruction[cpu->pc++];
    
    printf("p:%d instruct %c %s\n", cpu->id, instruction->type, instruction->value);
    
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
    
    scheduler(pm);
}


void scheduler(process_manager_t* pm){

    int id = -1;
    if(pm->cpu != NULL)
        for(int i = 0; i < pm->next_pcb; i++)
            if(pm->cpu->id == pm->pcb_table[i]->id)
                id = i;
    
    if(id != -1)
        add_line(id, pm->ready, &pm->next_ready);
    
    ready_to_cpu(pm);
}


/* *********** END EDITING *********** */




/* ***** PROCESS ***** */
// moves the first precess in ready line
// to the cpu
void ready_to_cpu(process_manager_t* pm){
    
    if(pm->next_ready > 0){
        pm->cpu = pm->pcb_table[pm->ready[0]];
        move_line(pm->ready, &pm->next_ready);
    }
    else pm->cpu = NULL;
}

// moves the current cpu process
// to the locked line
void lock_process(process_manager_t* pm){
    
    int match = -1;
    for(int i = 0; i < pm->next_pcb; i++)
        if(pm->cpu->id == pm->pcb_table[i]->id)
            match = i;
    
    pm->locked[pm->next_locked++] = match;
    
    if(pm->ready)
    
    pm->cpu = NULL;
}

// moves the first process in the locked line
// to the end of the ready line
void unlock_process(process_manager_t* pm){

    if(pm->locked[0] != -1){
        
        if(pm->cpu == NULL)
            pm->cpu = pm->pcb_table[pm->locked[0]];
        else
            add_line(pm->locked[0], pm->ready, &pm->next_ready);  
        
        move_line(pm->locked, &pm->next_locked);
        
    }
}

// current process finished (E instructio)
// removes process from pcb table
// atualizes index (w/ pcb) in ready and locked lines
void remove_process(process_manager_t* pm){
    
    int id = -1;
    
    for(int i = 0; i < pm->next_pcb; i++)
        if(pm->cpu->id == pm->pcb_table[i]->id)
            id = i;
        
    free(pm->pcb_table[id]);
    
    for(int i = id; i < pm->next_pcb-1; i++){
   
        *(pm->pcb_table[i]) = (process_t) *(pm->pcb_table[i+1]);
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

// creates a copy of the current process
// sets child process id, pid, start time, cpu_usage
// adds new process to pcb table and ready line
void child_process(int value, process_manager_t* pm){
    
    process_t* cpu = pm->cpu;
    process_t* child = (process_t*) malloc (sizeof(process_t));
    
    *child = (process_t) *cpu;
    child->id = pm->next_id++;
    child->pid = cpu->id;
    child->start = pm->timer;
    child->cpu_usage = 0;
    
    cpu->pc+=value;
    
    pm->ready[pm->next_ready++] = pm->next_pcb;
    pm->pcb_table[pm->next_pcb++] = child;
}

// changes current process image
void change_image(char* program, process_t* p){
    
    p->pc = 0;
    p->var = 0;
    p->instruction = new_instructions(program);
    
}

/* ***** LINES ***** */
// creates an empty line
int* new_line(){
    
    int* line;
    line = (int*) malloc (PROCESS_N * sizeof(int));
    
    for(int i = 0; i < PROCESS_N; i++)
        line[i] = -1;
    
    return line;
}

// adds an element to the end of the line
void add_line(int x, int* line, int* size){
    line[(*size)++] = x;
}

// moves line forwards
// first element out
void move_line(int* line, int* size){
    for(int i = 0; i < *size; i++)
        line[i] = line[i+1];
    (*size)--;
}
