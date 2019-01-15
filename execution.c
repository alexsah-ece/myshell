#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "execution.h"
#include "parse.h"


/* Executes a line consisting of commands,
 * seperated by ";" and "&&". If "quit" was
 * inserted, it returns 1. Otherwise, it
 * executes the commands and returns 0.
 */
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

/* Executes a single command. Input and output
 * redirection is implemented. If redirection is
 * needed, the filename is passed. Else, filename
 * is set to NULL.
 */
void execute_command(char* commands){
   char *output_redirection = strchr(commands, '>');
   char *input_redirection = strchr(commands, '<');
   char *min;
   if (output_redirection != NULL || input_redirection != NULL){
      if(output_redirection < input_redirection && output_redirection != NULL){
         min = output_redirection;
      }else{
         min = input_redirection;
      }
         //this step helps to extract the args, later on
         *min = '\0';
      }
   if(output_redirection!= NULL){
      output_redirection = extract_filename(output_redirection + 1);
   }
   if(input_redirection!= NULL){
      input_redirection = extract_filename(input_redirection + 1);
   }
   parse_command(commands, args);
   execute(args, input_redirection, output_redirection);
}

/* Executes a simple command, through creating
 * a child process. Syntax error detection is
 * enabled with perror() and _exit()
 */ 
void execute(char **args, char *input_filename, char *output_filename){
   int pid;
   pid = fork();
   if (pid < 0){
      printf("error forking...\n");
      _exit(1);
   }else if(pid == 0){
      //input redirection
      if(input_filename != (char *)NULL){
         FILE *fptr;
         fptr = fopen(input_filename,"r");
         if (fptr == NULL){
            printf("%s: No such file or directory\n", input_filename);
            return;
         }
         dup2(fileno(fptr), STDIN_FILENO);
         fclose(fptr);
      }
      //output redirection
      if(output_filename != (char *)NULL){
         FILE *fptr;
         fptr = fopen(output_filename,"w");
         dup2(fileno(fptr), STDOUT_FILENO);
         dup2(fileno(fptr), STDERR_FILENO);
         fclose(fptr);
      }
      //command execution
      execvp(*args, args);
      perror(*args);
      _exit(1);            
   }else{
      wait(&status);
   }
}