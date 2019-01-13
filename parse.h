#ifndef INPUT_H
#define INPUT_H

enum delimiter {SEMICOLON, AMBERSAND, LESS_THAN, BIGGER_THAN};

char *args[64];
char *commands[256];
enum delimiter delimiters[512];

int split_commands(char *);
char* search_delimiter(char *);
void add_delimiter(char, int);
void parse_command(char *, char **);
char* extract_filename(char*);

#endif