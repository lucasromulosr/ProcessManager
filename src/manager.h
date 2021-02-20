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
void lock_process(process_manager_t*);
void unlock_process(process_manager_t*);
void exit_process(process_manager_t*);
void fork_process(int, process_manager_t*);
void run_image(char*, process_t*);

/* -- LINES -- */
int* new_line();
void move_line(int*, int*);
void add_line(int, int*, int*);

#endif

/* 
    timer = tempo de programa
    
    todos next_ começam em 0
    next_id é o id que o proximo processo recebe.
        ele só é incrementado, msm que um processo
        termine, o id não é reutilizado
    next_(pcb, ready e locked) indicam a ultima
        posicao da fila/tabela
        
    o primeiro processo (init) recebe ppid 0, é
        o unico processo que pid == ppid
        
    a variavel inteira de um processo comeca em 0
        
    cpu aponta para o processo em execucao
    
    todos processos (prontos, bloqueados, execucao)
        se encontram na tabela pcb. Quando um processo
        finaliza (instrucao E), ele sai da tabela e os
        próximos são reposicionados
        
    ready e locked são filas que guardam o indice
        do processo na tabela pcb 
        (não é o id do processo)
        
    cada processo realiza uma instrucao e vai pro
        fim da fila de pronto ou é bloqueado (instrucao).
        vou desenvolver um escalonamento aidna
        
    ainda falta comentarios, mas cada funcao tem
        uma breve explicacao sobre seu funcionamento
 */



