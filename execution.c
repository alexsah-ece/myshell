#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "execution.h"

void execute(char **args){
    int pid;
    pid = fork();
    if (pid < 0){
        printf("error forking...");
    }else if(pid == 0){
        //child
        execvp(*args, args);
        perror(*args);
        _exit(1);            
    }else{
        //parent
        wait(&status);
    }
}