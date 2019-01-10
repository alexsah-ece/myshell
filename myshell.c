#include "execution.h"
#include "input.h"
#include <stdio.h>

void run(){
   while(1){
      printf("sahinis_8906> ");
      int delim_count = get_input() - 1;
      parse_command(*commands, args);
      execute(args);
      for(int i=0; i < delim_count; i++){
         if((delimiters[i] == 1 && status == 0) || delimiters[i] == 0){
            parse_command(commands[i+1], args);
            execute(args);
         }
      }
   }
}

int main(int argc, char **argv){

   run();
   return 0;
}