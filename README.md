Custom Shell in C++
Description
This project is a custom shell written in C++ for Windows. 
It supports basic shell commands, handles piping between commands, and includes custom commands like cd, exit, and help. 
The shell uses Windows API functions for process creation and inter-process communication, making it both powerful and flexible.

Features:

Basic Command Execution: Executes most standard shell commands available in Windows.
Piping: Supports command piping, enabling output from one command to be passed as input to another.
Custom Commands:
cd: Changes the current directory.
exit: Exits the shell.
help: Displays a list of supported commands.
hello: Greets the user with a personalized message.
User-friendly Interface: Displays a welcome message, user details, and provides prompt feedback.

Code Structure:

init_shell(): Initializes the shell environment by displaying a welcome message and user information.
*takeInput(char str)**: Reads input from the user and stores it.
printDir(): Displays the current working directory.
execArgs(char parsed)**: Executes a single command in a new process.
execArgsPiped(char parsed, char** parsedpipe)**: Executes piped commands by creating a pipe and using inter-process communication.
ownCmdHandler(char parsed)**: Handles custom commands like exit, cd, help, and hello.
parsePipe(char str, char* strpiped)**: Parses commands separated by a pipe symbol (|).
parseSpace(char str, char* parsed)**: Splits a command into individual tokens based on spaces.
processString(char str, char* parsed, char** parsedpipe)**: Processes the input command to identify if it's piped or not and splits it accordingly.
