#include <stdio.h>
#include <string.h>
#include "input.h"

void get_input(){
   fgets(input, MAX_INPUT, stdin);
   //Substitute '\n' character with NULL
   *(strchr(input, '\n')) = '\0';
   return;
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
   }
   delimiters[n] = delim;
}

char* search_delimiter(char *buf){
   char *delim[5];
   delim[0] = strstr(buf, "&&");
   delim[1] = strchr(buf, ';');
   char *first = delim[0];
   for(int i=1; i < 2; i++)
     if((delim[i] < first) && (delim[i] != NULL) || (first == NULL)) first = delim[i];
   return first; 
}

void parse_command(char *buf, char **args){
   int i=0, j=0;
   while ((buf[i] != '\0')){
      /*
      * Strip whitespace. Use nulls, so
      * that the previous argument is terminated
      * automatically.
      */
      while ((buf[i] == ' ') || (buf[i] == '\t')){
         buf[i++] = '\0'; 
      }
      //if it is the end of string, don't save it at all
      if(buf[i] == '\0'){
         break;
      }
      //Save the argument.
      args[j++] = buf + i;
      //Skip over the argument.
      while ((buf[i] != '\0') && (buf[i] != ' ') && (buf[i] != '\t')){
         i++;
      }
   }
   args[j] = NULL;
   j = 0;
   //while (args[j] != NULL) printf("%s\n", args[j++]);
}

char* extract_filename(char* buf){
   int i=0, j;
   //search for first letter of filename, skipping the whitespace
   while (buf[i] == ' ' || buf[i] == '\t') i++;
   //save the position
   j = i;
   //skip over the filename until first whitespace or null is reached
   while ((buf[i] != '\0') && (buf[i] != ' ') && (buf[i] != '\t') && (buf[i] != '\n')) i++;
   //use null for the first whitespace character so it can be terminated automatically
   buf[i] = '\0';
   return (buf + j);
}