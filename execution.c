#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "execution.h"
#include "parse.h"


int execute_line(char *input){
   if (strcmp(input, "quit") == 0){
      return 1;
   }
   int delim_count = split_commands(input) - 1;
   for(int i=-1; i < delim_count; i++){
      if(i == -1){
         execute_command(*commands);
      }else{
         if ((delimiters[i] == 1 && status == 0) || delimiters[i] == 0) execute_command(commands[i+1]);
      }
   }
   return 0;
}

void execute_command(char* commands){
   char *x = strchr(commands, '>');
   if(x!= NULL){
      *x = '\0';
      parse_command(commands, args);
      execute_output_redirect(args, extract_filename(x+1));
      return;
   }
   x = strchr(commands, '<');
   if(x!= NULL){
      *x = '\0';
      parse_command(commands, args);
      execute_input_redirect(args, extract_filename(x+1));
      return;
   }
   parse_command(commands, args);
   execute(args);
}

void execute(char **args){
    int pid;
    pid = fork();
    if (pid < 0){
        printf("error forking...\n");
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
        printf("error forking...\n");
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
        printf("error forking...\n");
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