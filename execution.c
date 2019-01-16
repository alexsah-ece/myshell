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
      first = 1;    
      execute_command(commands[i], 0, 0);
      //check exit status
      if(WIFEXITED(status)){
         int exit_status = WEXITSTATUS(status);
         if(exit_status == 2){
            printf("quiting...\n");
            return 1;
         }
         //skip execution of next commands, if delim is && and exit_status !=0
         if (delimiters[i] == '&' && exit_status != 0) break;
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
void execute_command(char* commands, int input_pipe, int output_pipe){
   char *output_redirection = strchr(commands, '>');
   char *input_redirection = strchr(commands, '<');
   char *pipe = strchr(commands, '|');
   char *min;
   if(pipe != NULL){
      *pipe = '\0';
      if(first){
         execute_command(commands, 0, 1);
         first = !first;
      }else{
         execute_command(commands, 1, 1);
      }
      execute_command(pipe + 1, 1, 0);
      return;
   }
   if (output_redirection != NULL || input_redirection != NULL){
      if((output_redirection < input_redirection && output_redirection != NULL) || input_redirection == NULL){
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
   if(input_pipe){
      input_redirection = "in_temp";
   }
   if(output_pipe){
      output_redirection = "out_temp";
   }
   parse_command(commands, args);
   execute(args, input_redirection, output_redirection);
   //write output to the in_temp file, for the next command to use
   if(output_pipe){
      FILE *fptr_out, *fptr_in;
      char ch;

      fptr_out = fopen("out_temp","r");
      fptr_in = fopen("in_temp", "w");

      while((ch = fgetc(fptr_out)) != EOF){
         //printf("%c", ch);
         fputc (ch, fptr_in);
      }
      fclose(fptr_in);
      fclose(fptr_out);
   }
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