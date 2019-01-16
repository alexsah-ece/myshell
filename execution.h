#ifndef EXECUTION_H
#define EXECUTION_H

int status;
int first;

int execute_line(char *);
void execute_command(char*, int, int);
void execute(char **, char *, char *);

#endif