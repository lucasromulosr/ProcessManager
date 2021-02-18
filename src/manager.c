#include "manager.h"

void printa_tabela(process_t**, int*);

int main(){

    int timer = 0;
    process_t* cpu;
    process_t** pcb_table = new_pcb_table();
    process_list_t* ready_list = new_process_list();
    process_list_t* locked_list = new_process_list();
//     process_list_t* exec_list = new_process_list();
    
    char c;
    int *next_id = (int*) malloc (1* sizeof(int));
    *next_id = 0;

    // cria 1o processo com init
    pcb_table[*next_id] = create_process(next_id, -1, timer, "../init.txt");
    cpu = pcb_table[0];
    pcb_table[*next_id] = create_process(next_id, -1, timer, "../program0.txt");
    ready_list->process_id[0] = 1;
    ready_list->next = 1;
    
  //printa conteudo tabela pcb
    printa_tabela(pcb_table, next_id);

    do {
        scanf("%c", &c);
        printf("LETRA %c\n", c);

        switch(c){
            case 'Q': remove_process(0, next_id, pcb_table); printa_tabela(pcb_table, next_id); break;
            case 'U': unlock_process(locked_list, ready_list); break;
            case 'P': reporter(timer, cpu, pcb_table, locked_list, ready_list); break;
            case 'T': printf("FIM <<<----\n"); break;
            default : printf("HOW THE HELL DID I GOT HERE <---\n");
        }

        timer++;
    } while (c != 'T');

    for(int i = 0; i < PROCESS_N; i++)
        free(pcb_table[i]);
    free(pcb_table);

    printf("MANAGER.C SE FOI <--\n");

    return 0;
}


void printa_tabela(process_t** pcb_table, int* next_id){
        for(int i = 0; i < *next_id; i++){

        printf("PC %d\n", pcb_table[i]->pc);

        for(int j = 0; j < 12; j++)
            printf("%c %s \n", pcb_table[i]->instruction[j]->type, pcb_table[i]->instruction[j]->value);

        printf("%d, %d, nx_id %d \n", pcb_table[i]->id, pcb_table[i]->pid, *next_id);
    }
}
