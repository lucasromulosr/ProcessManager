#include "manager.h"
#include <stdio.h>
#include <unistd.h>

void get_process_manager(process_manager_t*);

int main(){
    
    process_manager_t* pm;
	pm = malloc (sizeof(process_manager_t));
    get_process_manager(pm);
    
    int id;         // pcb_table index
    process_t* p;   // points to process to print
    
    printf("******************************************\n");
    printf("Sys status:\n");
    printf("******************************************\\\\\n");
    printf("TIME RUNNING: \t%d\n", pm->timer);
    
    printf("|pid\t|ppid\t|prior.\t|var\t|start\t|cpu_usage\n");
    
    // prints cpu process
    printf("|RUNNING PROCESS:\n");
        if((p = pm->cpu) != NULL)
        printf("|%d \t|%d \t|%d \t|%d \t|%d \t|%d \n", 
               p->id, p->pid, p->priority, p->var, p->start, p->cpu_usage);
    
    // gits the index from the queue;
    // ponits to process in pcb;
    // prints p
    printf("|BLOCKED PROCESSES:\n");
    for(int i = 0; i < pm->next_blocked; i++){
        id = pm->blocked[i];
        p = pm->pcb_table[id];
        printf("|%d \t|%d \t|%d \t|%d \t|%d \t|%d \n", 
               p->id, p->pid, p->priority, p->var, p->start, p->cpu_usage);
    }

    printf("|READY PROCESSES:\n");
    for(int i = 0; i < pm->next_ready; i++){
        id = pm->ready[i];
        p = pm->pcb_table[id];
        printf("|%d \t|%d \t|%d \t|%d \t|%d \t|%d \n", 
               p->id, p->pid, p->priority, p->var, p->start, p->cpu_usage);
    }
    
    free(pm);
    return 0;
}

//  gets the struct from manager TAD
void get_process_manager(process_manager_t* pm){

    read(0, &pm->timer, sizeof(int));
    read(0, &pm->next_id, sizeof(int));
    read(0, &pm->next_pcb, sizeof(int));
    read(0, &pm->next_ready, sizeof(int));
    read(0, &pm->next_blocked, sizeof(int));
        
    pm->cpu = (process_t*) malloc (sizeof(process_t));
    pm->pcb_table = (process_t**) malloc (pm->next_pcb * sizeof(process_t*));
    for(int i = 0; i < pm->next_pcb; i++)
        pm->pcb_table[i] = (process_t*) malloc (sizeof(process_t));
    
    pm->ready = (int*) malloc (pm->next_ready * sizeof(int));
    pm->blocked = (int*) malloc (pm->next_blocked * sizeof(int));
    
    // gets a signal that checks if the cpu is empty
    // cpu empty = 1, not empty = 0
    int null_cpu_signal = 0;
    read(0, &null_cpu_signal, sizeof(int));
    
    // checks the signal to receive or not
    if(!null_cpu_signal)
        read(0, pm->cpu, sizeof(process_t));
    else pm->cpu = NULL;
    
    for(int i = 0; i < pm->next_pcb; i++)
        read(0, pm->pcb_table[i], sizeof(process_t));
    
    read(0, pm->ready, pm->next_ready * sizeof(int));
    read(0, pm->blocked, pm->next_blocked * sizeof(int));
 
}
