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
   int j = split_commands(input);
   for(int i = 0; i < j; i++){
      //execute command  
      execute_command(commands[i]);
      //check exit status
      if(status >= 0){
         if(status == 2){
            printf("quiting...\n");
            return 1;
         }
         if(status == 3){
            printf("syntax: syntax error\n");
            break;
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

/* Takes a command an an input, which might contain a pipe. It handles the pipes
 * occured, executing the commands.
 */
void execute_command(char *cmd){
   int pipe_count, cmd_count, pid, input_pipe, output_pipe;
   pipe_count = split_pipes(piped_commands, cmd);
   cmd_count = pipe_count + 1;

   //check for empty command insertion, while delimiters have been inserted
   if(cmd_count == 0){
      status = 3;
      return;
   }
   int pd[pipe_count*2];

   //creating all necessary pipes
   for(int i=0; i < pipe_count*2; i++){
      if(pipe(pd + i*2) < 0){
         perror("pipe");
         status = 1;
         return;
      }
   }

   int j = 0;
   while(j < cmd_count){
      //double parsing on the same string leads to bug, so strdup is used
      if(execute_built_in(strdup(piped_commands[j]))){
         j++;
         continue;
      }
      pid = fork();
      if(pid == 0){
         input_pipe = 0; output_pipe = 0;
         if(j != 0){
            //if not first, get input from previous
            if(dup2(pd[(j-1) * 2], STDIN_FILENO) < 0){
               perror("dup2");
               _exit(1);
            };
            input_pipe = 1;
         }
         if(j != cmd_count - 1){
            //if not last command, pipe the output
            if(dup2(pd[j*2 + 1], 1) < 0){
               perror("dup2");
               _exit(1);
            }
            output_pipe = 1;
         }
         //close all pipes in child
         for(int i = 0; i < pipe_count*2; i++){
            close(pd[i]);
         }
         execute(piped_commands[j], input_pipe, output_pipe);
      }else if(pid > 0){
         close(pd[(j-1) * 2]);
         close(pd[j*2 + 1]);
         wait(&status);
         status = (WIFEXITED(status)) ? WEXITSTATUS(status) : -1;
            j++;
      }else{
         perror("fork");
         status = 1;
      }
   }
}

/* Executes built-in commands, the ones not supported natively by execvp.
 */
int execute_built_in(char *cmd){
   parse_command(cmd, args);
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

/* Executes a single command. Input and output redirection is implemented.
 * If redirection is  needed, the filename is passed. Else, filename is set 
 * to NULL. Input and output redirection is permitted if the command doesn't 
 * belong to a pipe where it redirects its intput and output correspondingly. 
 * This information is passed to input_pipe and output_pipe function args.
 */
void execute(char* command, int input_pipe, int output_pipe){
   char *output_redirection = strchr(command, '>');
   char *input_redirection = strchr(command, '<');

   if(output_redirection != NULL && output_pipe == 0){
      *output_redirection = '\0';
      output_redirection = extract_filename(output_redirection + 1);
      output_redirect(output_redirection);
   }
   if(input_redirection != NULL && input_pipe == 0){
      *input_redirection = '\0';
      input_redirection = extract_filename(input_redirection + 1);
      input_redirect(input_redirection);

   }
   parse_command(command, args);
   execvp(*args, args);
   perror(*args);
   _exit(1);   
}

/* Redirects input to the given filename.
 */
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

/* Redirects output to the given filename.
 */
void output_redirect(char *output_filename){
   if(output_filename != NULL){
      FILE *fptr;
      fptr = fopen(output_filename,"w");
      dup2(fileno(fptr), STDOUT_FILENO);
      dup2(fileno(fptr), STDERR_FILENO);
      fclose(fptr);
   }
}