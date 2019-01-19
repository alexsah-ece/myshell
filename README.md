# myshell

An operating systems uni-project for creating a custom shell.

# Install & execution

- `git clone {repo}`
- `cd myshell`
- `make`
- `./myshell {filename}`

# Supported operators & commands

### Operators
`;`, `&&`, `>`, `>>`, `<`, `|` and every valid combination of those.

### Commands
Every command that can be executed through `execvp` plus `quit` and `cd`.

# File structure
- \*.h files contain the definitions for the corresponding \*.c files
- parse.c has helper functions for parsing the input
- execution.c has functions that implement the execution of the commands. It includes parse.h to be able to parse the input
- myshell.c detects the mode and includes execution.h to orchestrate command execution

# Logic explanation 

The shell has two modes: **interactive** and **batch**. To determine which of the two is executed, the main() args are examined. if
no args are given, the shell falls into **interactive mode**. If at least one arg has been given, the first argument is considered
as the batch filename and the shell falls into **batch mode**.

### Interactive mode

The user can insert a line to be executed. The line is seperated to one or more commands, depending on the usage or not of `;`
and`&&` operators. Every command is then *further seperated* into sub-commands, depending on the presence of `|` operator. Those
sub-commands are validated (no empty commands permitted) and executed, right after they have been transformed to args, as
execvp() demands. If necessary, input and/or output of command is redirected to the previous/next command (if it is part of apipe) or to a file (if `<` and  `<` have
been used correctly). The exit `status` of the execution of each command is saved and used to determine the execution of the next
commands. When the user's line has been executed, the procedure restarts. Interactive mode is exited when **quit** was inserted
as a command.

### Batch mode

Batch mode follows the same logic as interactive mode, as the code is reused. The difference is that the input is read from a given file. If the file exists, each line is read and executed. Else, an appropriate message appears. Batch mode exits if **quit** command is executed, or **EOF** is reached.
