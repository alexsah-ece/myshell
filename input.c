#include <stdio.h>
#include <string.h>
#include "input.h"

void get_input(){
    fgets(input, MAX_INPUT, stdin);
    parse_command(input, args);
}

void parse_command(char *buf, char **args){
    int i=0, j=0;
    /*
     *Substitute '\n' character with NULL
    */    
    *(strchr(buf, '\n')) = '\0';
    while ((buf[i] != NULL) && (buf[i] != '\n')){
        /*
        * Strip whitespace. Use nulls, so
        * that the previous argument is terminated
        * automatically.
        */
        while ((buf[i] == ' ') || (buf[i] == '\t') || (buf[i] == '\n')) buf[i++] = NULL;
        /*
        * Save the argument.
        */
        args[j++] = buf + i;
        /*
        * Skip over the argument.
        */
        while ((buf[i] != NULL) && (buf[i] != ' ') && (buf[i] != '\t') && (buf[i] != '\n')) i++;
    }
    args[j] = NULL;
    j = 0;
    //while (args[j] != NULL) printf("%s\n", args[j++]);
}