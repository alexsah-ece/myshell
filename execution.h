#ifndef EXECUTION_H
#define EXECUTION_H

int status;
int first;

int execute_line(char *);
void execute_command(char*);
void execute(char *, int, int);
int execute_built_in(char*);
void input_redirect(char *);
void output_redirect(char *);

#endif