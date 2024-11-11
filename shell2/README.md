# Shell Program:
## Overview:
This shell program provides a command-line 
interface for executing various 
commands and interacting with the system

## Initialization:
When the shell starts, it reads and executes 
its configuration files. This setup process 
configures the shell environment and ensures
that all necessary settings and preferences
are applied before user interaction begins.

## Command Interpretation:
After initialization, the shell reads
commands from standard input. it parses
and executes each commands sequentially,
allowing users to perform operation and 
interact with the system effectively.

## Termination:
Upon completion of command execution,
the shell enters the termination phase.
it executes any shutdowns commands, performs
necessary cleanup tasks,and frees 
up any allocated memory.


### Parts of A Shell:

### Parser:

The parser breaks the input String
into tokens. tokens are the smallest 
units of meaningful data, such as 
command names, arguments, and operators.
it puts the tokens into a data structure 
called command table that will store the
commands that will be executed.


### Executor:

Take every Simple Command in the 
command table and create a new 
process from each one.



    