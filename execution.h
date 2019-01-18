#ifndef EXECUTION_H
#define EXECUTION_H

int status;
int first;

int execute_line(char *);
void execute_command(char*, int, int);
void execute(char **, char *, char *);
int execute_built_in(char**);
void input_redirect(char *);
void output_redirect(char *);

#endif