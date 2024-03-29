#include <stdio.h>
#include <string.h>
#include "parse.h"

#define true 1
#define false 0


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
      delimiters[i-1] = *delim;

      //split the command, using null character.
      if (*delim == '&') *delim++ = '\0';
      *delim++ = '\0';

      commands[i++] = delim;
      delim = search_delimiter(delim);
   }
   return i;
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

/* Splits buf based on '|' operator and stores the tokens in piped_commands.
 * The function returns the number of pipes that exist into buf.
 */
int split_pipes(char** piped_commands, char *buf){
   int i = 0;
   char *token = strtok(buf, "|");
   while(token != NULL){
      piped_commands[i++] = token;
      token = strtok(NULL, "|");
   }
   return i-1;
}

/* Checks if the given commands are non empty and thus, considered valid.
 */
int is_valid(char** piped_commands, int cmd_count){
	char *cmd;
	int valid = (cmd_count <= 0) ? false : true;
	for(int i=0; i < cmd_count; i++){
		cmd = strdup(piped_commands[i]);
        parse_command(cmd, args);
        if(args[0] == NULL){
            valid = false;
         }
	}
	return valid;
}