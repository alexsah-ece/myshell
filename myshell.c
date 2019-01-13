#include "execution.h"
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 512

char input[512];

void run(char* filename){
   if (filename == '\0'){
      while(1){
         printf("sahinis_8906> ");
         fgets(input, MAX_INPUT, stdin);

         //Substitute '\n' character with NULL
         char* c = (strchr(input, '\n'));
         if (c != NULL) *c = '\0';

         execute_line(input);  
      }
   }else{
      FILE *fptr;
      fptr = fopen(filename, "r");
      if(fptr == NULL){
         printf("No such file or directory\n");
      }else{
         while(1){
            if(fgets(input, MAX_INPUT, fptr) == NULL) break;
            
            //Substitute '\n' character with NULL
            char* c = (strchr(input, '\n'));
            if (c != NULL) *c = '\0';

            execute_line(input);
         }
      }
   }
}

int main(int argc, char **argv){

   char *filename;
   filename = (argc > 0) ? argv[1]: '\0';
   run(filename);
   return 0;
}