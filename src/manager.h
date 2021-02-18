#ifndef __MANAGER__
#define __MANAGER__

#include "process.h"

#define PROCESS_N 10    // up


typedef struct process_list_t{
    int next;
    int* process_id;
} process_list_t;


// functions manager

process_t* create_process(int*, int, int, char*);    // OK
//change_image();
//transicao_estado();
//escalonamento();
//change_context();


// functions for manager
void remove_process(int, int*, process_t**);    // OK
process_t** new_pcb_table();    //OK
process_list_t* new_process_list(); // OK
void unlock_process(process_list_t*, process_list_t*);  // OK
void reporter(int, process_t*, process_t**, process_list_t*, process_list_t*);

#endif
