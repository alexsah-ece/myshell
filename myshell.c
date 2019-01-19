#include "execution.h"
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 512

char input[512];

/* Detects the mode {interactive, batch},
 * reads the input and executes the commands
 * with the help of execut_line()
 */
void run(char* filename){
   if (filename == NULL){
      //interactive mode
      while(1){
         printf("sahinis_8906> ");
         fgets(input, MAX_INPUT, stdin);
         //Substitute '\n' character with NULL
         char* c = (strchr(input, '\n'));
         if (c != NULL) *c = '\0';
         //check for empty input
         if(input[0] == '\0') continue;
         //execute line (if quit was inserted, run() returns)
         if (execute_line(input)) return;  
      }
   }else{
      //batch mode
      FILE *fptr;
      fptr = fopen(filename, "r");
      if(fptr == NULL){
         printf("No such file or directory\n");
      }else{
         while(1){
            //read line (if EOF is reached, run() returns)
            if(fgets(input, MAX_INPUT, fptr) == NULL){
               printf("quiting...\n");
               return;
            }             
            //Substitute '\n' character with NULL
            char* c = (strchr(input, '\n'));
            if (c != NULL) *c = '\0';
            //execute line (if quit was inserted, run() returns)
            if (execute_line(input)) return;           }
      }
   }
}

int main(int argc, char **argv){

   char *filename;
   filename = (argc > 0) ? argv[1] : NULL;
   run(filename);
   return 0;
}