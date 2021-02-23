#include "manager.h"

#include <unistd.h>

int main(){

    process_manager_t* pm;
    pm = initialize_process_manager();

    char c; // entrada
    do {
        scanf("%c", &c);
        
        switch( c ){
            case 'Q': execute(pm); break;
            case 'U': unblock_process(pm); break;
            case 'P': reporter(pm); break;
            case 'T': reporter(pm); break;
        }

        pm->timer++;
    } while (c != 'T');

    free(pm);

    return 0;
}
