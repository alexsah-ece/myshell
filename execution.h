#ifndef EXECUTION_H
#define EXECUTION_H

int status;

int execute_line(char *);
void execute_command(char*);
void execute(char **);
void execute_input_redirect(char **, char *);
void execute_output_redirect(char **, char *);

#endif