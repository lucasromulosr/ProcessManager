#include <stdio.h>
#include <stdlib.h>

int main(){
 

    char c;
    int i = 0;
    
    do {
        scanf("%c", &c);
        printf("LETRA %c\n", c);
        
        switch(c){
            case 'Q': printf("NEXT STUFF <<<----\n"); break;
            case 'U': printf("UNLOCK <<<----\n"); break;
            case 'P': printf("ESTADO <<<----\n"); break;
            case 'T': printf("FIM <<<----\n"); break;
            default : printf("HOW THE HELL DID I GOT HERE <---\n");
        }
        
        printf("%d I <<<----\n", ++i);
    } while (c != 'T');
    
    printf("MANAGER.C SE FOI <--\n");
 
    return 0;
}
