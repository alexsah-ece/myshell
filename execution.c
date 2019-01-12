#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "execution.h"

void execute(char **args){
    int pid;
    pid = fork();
    if (pid < 0){
        printf("error forking...");
    }else if(pid == 0){

        execvp(*args, args);
        perror(*args);
        _exit(1);            
    
    }else{
        wait(&status);
    }
}

void execute_input_redirect(char **args, char *filename){
    int pid;
    pid = fork();
    if (pid < 0){
        printf("error forking...");
    }else if(pid == 0){
        FILE *fptr;
        
        fptr = fopen(filename,"r");
        dup2(fileno(fptr), STDIN_FILENO);
        fclose(fptr);
        
        execvp(*args, args);
        perror(*args);
        _exit(1);           

    }else{
        wait(&status);
    }
}

void execute_output_redirect(char **args, char *filename){
    int pid;
    pid = fork();
    if (pid < 0){
        printf("error forking...");
    }else if(pid == 0){
    
        FILE *fptr;
        fptr = fopen(filename,"w");
        dup2(fileno(fptr), STDOUT_FILENO);
        dup2(fileno(fptr), STDERR_FILENO);
        fclose(fptr);

        execvp(*args, args);
        perror(*args);
        _exit(1);           

    }else{
        wait(&status);
    }
}