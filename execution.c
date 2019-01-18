#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "execution.h"
#include "parse.h"


/* Executes a line, consisting of commands, seperated by ";" 
 * and "&&". If "quit" was inserted, it returns 1. 
 * Otherwise, it executes the commands and returns 0.
 */
int execute_line(char *input){
   int command_count = split_commands(input);
   for(int i = 0; i < command_count; i++){
      //execute command
      first = 1;    
      execute_command(commands[i], 0, 0);
      //check exit status
      if(status >= 0){
         if(status == 2){
            printf("quiting...\n");
            return 1;
         }
         //skip execution of next commands, if delim is && and exit_status !=0
         if (delimiters[i] == '&' && status != 0) break;
      }else{
         //unexpected exit
         printf("unexpected error\n");
         break;
      }
   }
   return 0;
}

/* Executes a single command. Input and output redirection is implemented,
 * as well as multiple pipes. If redirection is  needed, the filename is passed. 
 * Else, filename is set to NULL. Piping is implemented with the use of temp files
 * using the flags input_pipe and output_pipe.
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

/* After checking for built-in commands, executes a simple command, 
 * through creating a child process. Syntax error detection is enabled 
 * with perror() and _exit().
 */ 
void execute(char **args, char *input_filename, char *output_filename){
   //Check if built-in command. If so, execute it and return. 
   if(execute_built_in(args)){
      return;
   }
   int pid = fork();
   if (pid < 0){
      printf("error forking...\n");
      _exit(1);
   }else if(pid == 0){   
      input_redirect(input_filename);
      output_redirect(output_filename);
      execvp(*args, args);
      perror(*args);
      _exit(1);            
   }else{
      wait(&status);
      if(WIFEXITED(status)){
         status = WEXITSTATUS(status);
      }else{
         status = -1;
      }
   }
}

int execute_built_in(char **args){
   status = -1;
   if (strcmp(*args, "quit") == 0){
      status = 2;
   }else if(strcmp(*args, "cd") == 0){
      if(chdir(args[1])!=0){
         perror("cd");
         status = 1;
      }else{
         status = 0;
      }
   }
   return (status == -1)? 0: 1;
}

void input_redirect(char *input_filename){
   if(input_filename != NULL){
      FILE *fptr;
      fptr = fopen(input_filename,"r");
      if (fptr == NULL){
         printf("%s: No such file or directory\n", input_filename);
         _exit(1);
      }
      dup2(fileno(fptr), STDIN_FILENO);
      fclose(fptr);
   }
}

void output_redirect(char *output_filename){
   if(output_filename != NULL){
      FILE *fptr;
      fptr = fopen(output_filename,"w");
      dup2(fileno(fptr), STDOUT_FILENO);
      dup2(fileno(fptr), STDERR_FILENO);
      fclose(fptr);
   }
}