#ifndef __MANAGER__
#define __MANAGER__

#include "process.h"

#define PROCESS_N 100    // up

typedef struct process_manager_t{
    int timer;
    int next_id, next_pcb;          // pcb_table
    int next_ready, next_locked;    // lines
    process_t* cpu;
    process_t** pcb_table;
    int* ready;
    int* locked;
} process_manager_t;


/* -- REPORTER --*/
void reporter(process_manager_t*);

/* -- INITIALIZATION --*/
process_manager_t* initialize_process_manager();
process_t* first_process(int*, int, int, char*);
process_t** new_pcb_table();


/* -- working on -- */
void execute(process_manager_t*);
void scheduler(process_manager_t*);





/* -- PROCESS -- */
void ready_to_cpu(process_manager_t*);
void lock_process(process_manager_t*);  // OK
void unlock_process(process_manager_t*);  // OK
void remove_process(process_manager_t*);    // not OK
void child_process(int, process_manager_t*);
void change_image(char*, process_t*);

/* -- LINES -- */
int* new_line();
void move_line(int*, int*);
void add_line(int, int*, int*);


#endif
