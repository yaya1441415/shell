# MY SHELL – Custom Windows Command-Line Shell

A custom-built Windows command-line shell implemented in **C++**, demonstrating knowledge in **systems programming, process management, and Windows API usage**.

---

## Key Skills Demonstrated

- **Process Creation & Management:** Using `CreateProcessA`, `STARTUPINFOA`, and `PROCESS_INFORMATION` to launch Windows commands programmatically.
- **Inter-Process Communication:** Implemented single-pipe support (`|`) to pass output from one command to another.
- **String Parsing & Tokenization:** Handling user input, trimming whitespace, and splitting commands and arguments reliably.
- **File System Operations:** Built-in commands to manipulate directories and navigate the file system.
- **Error Handling & Debugging:** Reporting errors from Windows API functions with meaningful messages.
- **User Experience:** Custom shell prompt showing current directory, greeting messages, and help system.

---

## Features

- Execute **any Windows CMD command**: `dir`, `ping`, `notepad`, etc.
- Built-in commands:
  - `exit` – Exit the shell
  - `cd [dir]` – Change current directory
  - `help` – Display shell usage and commands
  - `hello` – Personalized greeting using the Windows username
  - `cls` – Clear the terminal
- **Pipe support:** Execute two commands connected by `|` (e.g., `dir | findstr cpp`)
- Handles spaces in command arguments

---

## Usage

1. Compile the project using a **C++ compiler** like Visual Studio.
2. Run the executable in a Windows environment.
3. Use commands as you would in CMD:

```text
C:\Users\yahya\OneDrive\Bureau\buildbeloved\shell> my_shell.exe
>>> dir
>>> cd ..
>>> notepad
>>> dir | findstr cpp
```
## Known Limitations

- **`mkdir` and `rmdir` do not work properly**  
  **Reason:** These commands are implemented using `_mkdir()` and `_rmdir()` from the C runtime library. Because the shell tries to execute external processes for all commands by default (`cmd /c`), the internal handling of `_mkdir()` and `_rmdir()` is inconsistent with Windows CMD behavior.  
  The commands fail with errors like:  
  Failed to create process. Error: 2

  - **Pipe Support:** Only a single pipe is supported; multiple pipes in one command are not implemented.
- **Interactive Commands:** Some commands that require interactive input (like `more` or `ftp`) may behave unexpectedly.

---

## Design Highlights

- **Input Handling:** Robust reading of user input with support for empty inputs and trimming of whitespace.
- **Command Parsing:** Splits commands on spaces and detects pipes for inter-process communication.
- **Built-in vs External Commands:** Differentiates between custom shell commands (exit, cd, help, etc.) and external Windows commands.
- **Windows Integration:** Uses Windows API for process execution, pipes, and directory management.


