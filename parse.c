#include <stdio.h>
#include <string.h>
#include "parse.h"


/* Takes as input a char buffer and splits
 * it to one or more commands, based on the
 * detection of ";" and "&&". Every command
 * is stored to the commands[256] array. It
 * returns the number of commands to be executed.
 */
int split_commands(char *buf){
   char *delim;
   *commands = buf;
   int i = 1;
   delim = search_delimiter(buf);
   while (delim != NULL){
      add_delimiter(*delim, i-1);

      //split the command, using null character.
      if (*delim == '&') *delim++ = '\0';
      *delim++ = '\0';

      commands[i++] = delim;
      delim = search_delimiter(delim);
   }
   return i;
}

/* Adds a delimiter to the delimiters[512] array
 */
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

/* Searches for the ";" and "&&" delimiters, in a given
 * buffer. It returns a pointer to the first occurence
 * of a delimiter, or null if none is present.
 */
char* search_delimiter(char *buf){
   char *delim[2];
   delim[0] = strstr(buf, "&&");
   delim[1] = strchr(buf, ';');
   return ((delim[1] < delim[0] && delim[1] != NULL) || (delim[0] == NULL)) ? delim[1] : delim[0];
}

/* Takes as an input a command, along with an args array to be
 * filled and strips whitespace from the command, transforming
 * it to an arguments representation, the one that execvp
 * requires to execute a command.
 */
void parse_command(char *buf, char **args){
   int i=0, j=0;
   while ((buf[i] != '\0')){
      /*
      * Strip whitespace. Use nulls, so
      * that the previous argument is terminated
      * automatically.
      */
      while ((buf[i] == ' ') || (buf[i] == '\t')) buf[i++] = '\0'; 
      //if it is the end of string, don't save it at all
      if(buf[i] == '\0') break;
      //Save the argument.
      args[j++] = buf + i;
      //Skip over the argument.
      while ((buf[i] != '\0') && (buf[i] != ' ') && (buf[i] != '\t')) i++;
   }
   args[j] = NULL;
   j = 0;
}

/* Takes as an input a string, containing a filename, extracts
 * the file name, stripping whitespace and returns the filename
 */
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