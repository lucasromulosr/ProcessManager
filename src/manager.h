#ifndef __MANAGER__
#define __MANAGER__

#include "process.h"

#define PROCESS_N 100    // number of max simulated processes

typedef struct process_manager_t{
    int timer;
    int next_id, next_pcb;          // pcb_table
    int next_ready, next_blocked;    // queues
    process_t* cpu;
    process_t** pcb_table;
    int* ready;
    int* blocked;
} process_manager_t;

/* -- REPORTER --*/
void reporter(process_manager_t*);
void report2(process_manager_t*);

/* -- INITIALIZATION --*/
process_manager_t* initialize_process_manager();
process_t* first_process(int*, int, int, char*);
process_t** new_pcb_table();

/* -- SCHEDULING -- */
void execute(process_manager_t*);
void scheduler(process_manager_t*);

/* -- PROCESS -- */
void ready_to_cpu(process_manager_t*);
void block_process(process_manager_t*);
void unblock_process(process_manager_t*);
void exit_process(process_manager_t*);
void fork_process(int, process_manager_t*);
void run_image(char*, process_t*);

/* -- QUEUES -- */
int* new_queue();
void move_queue(int*, int*);
void add_queue(int, int*, int*);

#endif
