#include "manager.h"
#include <unistd.h>

/* ***** REPORTER ***** */
void reporter(process_manager_t* pm){
    
    int fd[2];
    pid_t pid;
    char* args[] = {"./reporter", NULL};
    
    if((pipe(fd)) < 0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) == -1){
            perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if(pid == 0){
        
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execv(args[0], args);
        
    } else {        // send process manager 
                    // values through pipe
        close(fd[0]);
        
        write(fd[1], &pm->timer, sizeof(int));
        write(fd[1], &pm->next_id, sizeof(int));
        write(fd[1], &pm->next_pcb, sizeof(int));
        write(fd[1], &pm->next_ready, sizeof(int));
        write(fd[1], &pm->next_blocked, sizeof(int));
        
        int null_cpu_signal = 0;
        if(pm->cpu == NULL) null_cpu_signal = 1;
        write(fd[1], &null_cpu_signal, sizeof(int));
        
        write(fd[1], pm->cpu, sizeof(process_t));
        
        for(int i = 0; i < pm->next_pcb; i++)
            write(fd[1], pm->pcb_table[i], sizeof(process_t));
        
        write(fd[1], pm->ready, pm->next_ready * sizeof(int));
        write(fd[1], pm->blocked, pm->next_blocked * sizeof(int));

        close(fd[1]);
    }
}

/* ***** INITIALIZATION ***** */
// initializes process manager
process_manager_t* initialize_process_manager(){
    
    // creates process manager
    process_manager_t* pm;
    pm = (process_manager_t*) malloc (sizeof(process_manager_t));
    
    pm->cpu = (process_t*) malloc (sizeof(process_t));
    
    pm->pcb_table = new_pcb_table();
    pm->ready = new_queue();
    pm->blocked = new_queue();
    
    pm->timer = 0;
    pm->next_id = 0;
    pm->next_pcb = 0;
    pm->next_ready = 0;
    pm->next_blocked = 0;
    
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

/* ***** SCHEDULING ***** */
// executes 1 instruction from the process running
void execute(process_manager_t* pm){

    if(pm->cpu == NULL){
        printf("_/> Theres no process running now.\n");
        return;
    }
    
    process_t* cpu = pm->cpu;       // ponits to cpu process
    instruction_t* instruction;     // points to instruction
    instruction = cpu->instruction[cpu->pc++];  // adds pc
    
    printf("_/> pid %d: %c %s\n", cpu->id, instruction->type,
            instruction->value);
    
    // those types of instructions are 'c int';
    // atoi converts a string (value) to int
    char c = instruction->type;
    int value;
    if(c == 'S' || c== 'A' || c == 'D' || c == 'F')
        value = atoi(instruction->value);
    
    // adss cpu usage time to current process
    cpu->cpu_usage++;
    
    switch( c ){
        case 'S': set_var(value, cpu); break;
        case 'A': add_var(value, cpu); break;
        case 'D': dec_var(value, cpu); break;
        case 'B': block_process(pm); break;
        case 'E': exit_process(pm); break;
        case 'F': fork_process(value, pm); break;
        case 'R': run_image(instruction->value, cpu); break;
    }
    
    scheduler(pm);
}

// after the execution, the scheduler takes the
// running process to the end of the ready queue,
// if the process didnt end of blocked;
// the first process in ready queue goes to cpu
void scheduler(process_manager_t* pm){

    int id = -1;
    if(pm->cpu != NULL)
        for(int i = 0; i < pm->next_pcb; i++)
            if(pm->cpu->id == pm->pcb_table[i]->id)
                id = i;
    
    if(id != -1)
        add_queue(id, pm->ready, &pm->next_ready);
    
    ready_to_cpu(pm);
}

/* ***** PROCESS ***** */
// moves the first precess in ready queue
// to the cpu and moves the queue
void ready_to_cpu(process_manager_t* pm){
    
    // pm->next_ready indicates how many processes
    // are in queue
    if(pm->next_ready > 0){
        pm->cpu = pm->pcb_table[pm->ready[0]];
        move_queue(pm->ready, &pm->next_ready);
    }
    else pm->cpu = NULL;
}

// moves the current cpu process
// to the blocked queue
void block_process(process_manager_t* pm){
    
    int match = -1;
    // gets the process index on pcb table
    for(int i = 0; i < pm->next_pcb; i++)
        if(pm->cpu->id == pm->pcb_table[i]->id)
            match = i;
    
    // saves the index in the blocked queue
    pm->blocked[pm->next_blocked++] = match;
    
    pm->cpu = NULL;
}

// moves the first process in the blocked queue
// to the end of the ready queue
void unblock_process(process_manager_t* pm){

    // pcb indice is always >= 0;
    // if queue value is -1, means that its empty;
    // if the 1st position is empty, they all are
    if(pm->blocked[0] != -1){
        
        int index = pm->blocked[0];
        
        // if the cpu is empty, that means that there was
        // no process ready in queue, so the unblocked
        // process goes straight to the cpu
        if(pm->cpu == NULL)
            pm->cpu = pm->pcb_table[index];
        else
            add_queue(index, pm->ready, &pm->next_ready);  
        
        printf("_/> pid %d unblocked.\n", pm->pcb_table[index]->id);
        
        move_queue(pm->blocked, &pm->next_blocked);
    }
    
    else printf("_/> No process to unblock.\n");
}

// current process finished (E instructio)
// removes process from pcb table
// atualizes index (w/ pcb) in ready and blocked queues
void exit_process(process_manager_t* pm){
    
    int id = -1;
    
    // gets the process index in pcb table
    for(int i = 0; i < pm->next_pcb; i++)
        if(pm->cpu->id == pm->pcb_table[i]->id)
            id = i;
    
    // removes the process
    free(pm->pcb_table[id]);
    
    // moves the other process ahead
    for(int i = id; i < pm->next_pcb-1; i++){
        *(pm->pcb_table[i]) = (process_t) *(pm->pcb_table[i+1]);
        free(pm->pcb_table[i+1]);
    }
    
    // decreases all indexes from remove positon
    // until the end of the queue
    for(int i = 0; i < pm->next_ready; i++)
        if(pm->ready[i] > id)
            pm->ready[i]--;
    
    for(int i = 0; i < pm->next_blocked; i++)
        if(pm->blocked[i] > id)
            pm->blocked[i]--;
        
    pm->next_pcb--;
    pm->cpu = NULL;
}

// creates a copy of the current process.
// sets child process id, pid, start time, cpu_usage.
// adds new process to pcb table and ready queue.
// current process jumps 'value' instructions
void fork_process(int value, process_manager_t* pm){
    
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
void run_image(char* program, process_t* p){
    
    p->pc = 0;
    p->var = 0;
    p->instruction = new_instructions(program);
    
}

/* ***** QUEUES ***** */
// creates an empty queue
int* new_queue(){
    
    int* queue;
    queue = (int*) malloc (PROCESS_N * sizeof(int));
    
    for(int i = 0; i < PROCESS_N; i++)
        queue[i] = -1;
    
    return queue;
}

// adds an element to the end of the queue
void add_queue(int x, int* queue, int* size){
    queue[(*size)++] = x;
}

// moves queue forwards
// first element out
void move_queue(int* queue, int* size){
    for(int i = 0; i < *size; i++)
        queue[i] = queue[i+1];
    (*size)--;
}
