#ifndef INPUT_H
#define INPUT_H

#define MAX_INPUT 512

char input[512];
char *args[64];

void get_input(void);
void parse_command(char *, char **);

#endif
