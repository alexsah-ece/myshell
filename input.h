#ifndef INPUT_H
#define INPUT_H

#define MAX_INPUT 512

enum delimiter {SEMICOLON, AMBERSAND, LESS_THAN, BIGGER_THAN};

char input[512];
char *args[64];
char *commands[256];
enum delimiter delimiters[512];

void add_delimiter(char, int);
int split_commands(char *);
int get_input(void);
void parse_command(char *, char **);
char* search_delimiter(char *);

#endif
