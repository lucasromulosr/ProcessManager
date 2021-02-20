#ifndef __MANAGER__
#define __MANAGER__

#include "process.h"

#define PROCESS_N 10    // up

typedef struct process_manager_t{
    int timer;
    int next_id, next_pcb;          // pcb_table
    int next_ready, next_locked;    // lines
    process_t* cpu;
    process_t** pcb_table;
    int* ready;
    int* locked;
} process_manager_t;

// functions manager


/* -- CREATE --*/
process_manager_t* initialize_process_manager();    // OK - tirar dbg
process_t* create_process(int*, int, int, char*);    // OK
process_t** new_pcb_table();    // OK


void execute(process_manager_t*);
void scheduler(process_manager_t*);


void reporter(process_manager_t*);  // LOOKS OK


void child_process(int, process_manager_t*);
void change_image(char*, process_t*);


/* -- PROCESS -- */
void ready_to_cpu(process_manager_t*);
void lock_process(process_manager_t*);  // OK
void unlock_process(process_manager_t*);  // OK
void remove_process(process_manager_t*);    // not OK

/* -- LINES -- */
int* new_line();    // OK
void move_line(int*, int*); // OK
void add_line(int, int*, int*); // OK


#endif
