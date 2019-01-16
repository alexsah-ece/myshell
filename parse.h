#ifndef INPUT_H
#define INPUT_H

char *args[64];
char *commands[256];
char delimiters[512];

int split_commands(char *);
char* search_delimiter(char *);
void parse_command(char *, char **);
char* extract_filename(char*);

#endif