#include <stdio.h>
#include <string.h>
#include "input.h"

int get_input(){
   fgets(input, MAX_INPUT, stdin);
   //Substitute '\n' character with NULL
   *(strchr(input, '\n')) = '\0';
   int c = split_commands(input);
   //parse_command(input, args);
   return c;
}

int split_commands(char *buf){
   char *delim;
   *commands = buf;
   int i = 1;
   delim = search_delimiter(buf);
   while (delim != NULL){
      add_delimiter(*delim, i-1);
      if (*delim == '&') *delim++ = '\0';
      *delim++ = '\0';
      commands[i++] = delim;
      delim = search_delimiter(delim);
   }
   return i;
}

void add_delimiter(char c, int n){
   enum delimiter delim;
   switch(c){
      case '&':
         delim = AMBERSAND;
         break;
      case ';':
         delim = SEMICOLON;
         break;
      case '>':
         delim = BIGGER_THAN;
         break;
      case '<':
         delim = LESS_THAN;
   }
   delimiters[n] = delim;
}

char* search_delimiter(char *buf){
   char *delim[5];
   delim[0] = strstr(buf, "&&");
   delim[1] = strchr(buf, ';');
   delim[2] = strchr(buf, '>');
   delim[3] = strchr(buf, '<');
   char *first = delim[0];
   for(int i=1; i < 4; i++)
     if((delim[i] < first) && (delim[i] != NULL) || (first == NULL)) first = delim[i];
   return first; 
}

void parse_command(char *buf, char **args){
   int i=0, j=0;
   while ((buf[i] != '\0') && (buf[i] != '\n')){
      /*
      * Strip whitespace. Use nulls, so
      * that the previous argument is terminated
      * automatically.
      */
      while ((buf[i] == ' ') || (buf[i] == '\t') || (buf[i] == '\n')) buf[i++] = '\0';
      //Save the argument.
      args[j++] = buf + i;
      //Skip over the argument.
      while ((buf[i] != '\0') && (buf[i] != ' ') && (buf[i] != '\t') && (buf[i] != '\n')) i++;
   }
   args[j] = NULL;
   j = 0;
   //while (args[j] != NULL) printf("%s\n", args[j++]);
}