#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// le entradas ate que seja uma valida
char read_command();
int verify_input(char);


int main(){

    int fd[2];      // pipe
    char c;         // entrada teclado
    char* args[] = {"./manager", NULL}; // execv
    pid_t pid;     // pid

    if(pipe(fd) < 0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){   // filho
        
        close(fd[1]);
        
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        
        execv(args[0], args);
        
    } else {    // pai
        
        close(fd[0]);
        
        do {
            c = read_command();
//             sleep(1);
            write(fd[1], &c, 1);
        }while (c != 'T');
        
        close(fd[1]);
    }

    return 0;
}

char read_command(){

    char c = '\0';
    
    while(!verify_input(toupper(c)))
        scanf("%c", &c);

    return toupper(c);
}

int verify_input(char c){
    return (c == 'P' || c == 'Q' || c == 'U' || c == 'T');
}
