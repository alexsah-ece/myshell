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
   int command_count = split_commands(input);
   for(int i = 0; i < command_count; i++){
      //execute command      
      execute_command(commands[i]);
      //check exit status
      if(WIFEXITED(status)){
         int exit_status = WEXITSTATUS(status);
         if(exit_status == 2){
            printf("quiting...\n");
            return 1;
         }
         //skip execution of next commands, if delim is && and exit_status !=0
         if (delimiters[i] == 1 && exit_status != 0) break;
      }else{
         //unexpected exit
         break;
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
      if (args != NULL){
         if(strcmp(*args, "quit") == 0) _exit(2);
      }
      execvp(*args, args);
      perror(*args);
      _exit(1);            
   }else{
      wait(&status);
   }
}