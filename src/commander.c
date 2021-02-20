#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

char read_command();
int verify_input(char);

int main(){

    int fd[2];
    char c;
    char* args[] = {"./manager", NULL};
    pid_t cpid;

    if(pipe(fd) < 0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if((cpid = fork()) == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(cpid == 0){
        
        close(fd[1]);
        
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        
        execv(args[0], args);
        
    } else {
        
        close(fd[0]);
        
        do {
            c = read_command();
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
