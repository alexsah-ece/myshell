#include "execution.h"
#include "input.h"
#include <stdio.h>
#include <string.h>

void execute_line(char* commands){
   char *x = strchr(commands, '>');
   if(x!= NULL){
      *x = '\0';
      parse_command(commands, args);
      char *filename = extract_filename(x+1);
      execute_output_redirect(args, filename);
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

void run(char* filename){
   if (filename == '\0'){
      while(1){
         printf("sahinis_8906> ");
         get_input();
         if (strcmp(input, "quit") == 0) break;
         int delim_count = split_commands(input) - 1;
         for(int i=-1; i < delim_count; i++){
            if(i == -1){
               execute_line(*commands);
            }else{
               if ((delimiters[i] == 1 && status == 0) || delimiters[i] == 0) execute_line(commands[i+1]);
            }
         }     
      }
   }else{
      FILE *fptr;
      fptr = fopen(filename, "r");
      if(fptr == NULL){
         printf("No such file or directory\n");
      }else{
         while(1){
            if(fgets(input, MAX_INPUT, fptr) == NULL) break;
            printf("\tINPUT:%s\n", input);
            char* c = (strchr(input, '\n'));
            if (c != NULL) *c = '\0';
            //printf("\tINPUT:%s\n", input);
            if (strcmp(input, "quit") == 0) break;
            int delim_count = split_commands(input) - 1;
            for(int i=-1; i < delim_count; i++){
               if(i == -1){
                  execute_line(*commands);
               }else{
                  if ((delimiters[i] == 1 && status == 0) || delimiters[i] == 0) execute_line(commands[i+1]);
               }
            }
         }
      }
   }
}

int main(int argc, char **argv){

   char *filename;
   if (argc > 0){
      filename = argv[1];
   }else{
      filename = '\0';
   }
   run(filename);
   return 0;
}