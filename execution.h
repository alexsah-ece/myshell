#ifndef EXECUTION_H
#define EXECUTION_H

int status;
char *std_out;

void execute_input_redirect(char **, char *);
void execute_output_redirect(char **, char *);
void execute(char **);

#endif