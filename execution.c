#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "execution.h"

void execute(char **args){
    int pid, status;
    pid = fork();
    if (pid < 0){
        printf("error forking...");
    }else if(pid == 0){
        //child
        execvp(*args, args);
        _exit(0);            
    }else{
        //parent
        wait(&status);
    }
}