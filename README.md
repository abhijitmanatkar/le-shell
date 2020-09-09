# Le-shell

A basic shell written in C using UNIX system  calls.

## Execution instructions
In the source directory, execute:
```shell
$ make
$ ./leshell
```
Press <kbd>Ctrl + C</kbd> to exit.

## Features
- A prompt showing username, hostname and current directory.
- Built in commands: `pwd`, `cd`, `ls`, `echo`.
- Support for other system commands.
- Support for commands to be run in the foreground or background.
- Display termination information of a command executed in the background.
- User-defined command `pinfo <PID>` that displays process info on the screen.
 - Support for absolute and relative paths. Also supports `~` which points to the directory of execution.

## Implementation Details

### Main loop
- The main loop displays a prompt waiting for commands to be entered.
- The entered commands are parsed and correctly executed.

### Prompt
- The username of the current user is obtained by reading environment variable `$USER`. The current directory and  and hostname are obtained via system calls.
- The current directory is displayed as relative to the directory of execution.

### Parsing
- Entered commands are split by semicolons using `strtok`.
- Each command is further split into arguments seperated by whitespace.
- If the command is part of the above mentioned built-in or user defined commands, a call is made to the function of that command.
- Otherwise, a call is made to the `execute` function to execute the entered command as a system command.

### Built-in and user defined commands
- Each built-in and user defined command is implemented as a function as defined in its own seperate file.
- An interface to the function is provided by means of a header file.

### System commands
- System commands are executed using the `execute` command defined in `execute.c`.
- System commands can be executed in the foreground or in the background.
- This is specified by entering a '&' at the end of a command.
- Execution is carried out by forking the main process to create a child process. The child process then runs the specified command using the  `execvp` system call.
- If the command is executed in the foreground, the shell waits for the the child process to terminate.
- If it is executed in the background, the shell does not wait for the termination of the child process and continues with the main loop.
- A signal handler is set up to handle the `SIGCHLD` signal emitted by a child process running in the background. When this signal is received, the parent process prints termination details of the process.

## File wise breakdown
 - `main.c` contains the main shell loop for reading, parsing and executiong commands.
 - `headers.h` contains common header files and definitions of some important constants.
 - `prompt.c` contains the function for printing the shell prompt.
 - `formatpath.c` contains functions to convert paths from relative to absolute and from absolute to relative.
 - `exexute.c` has funtions for executing system commands in either freground or background.
 - `cd.c` contains the definition of the `cd` command.
 - `ls.c` contains the definition of the `ls` command.
 - `pwd.c` contains the definition of the `pwd` command.
 - `echo.c` contains the definition of the `echo` command.
 - `pinfo.c` contains the definition of the `pinfo` command.
 - `makefile` contains compilation instructions.

*A header file is associated with each C file.* 

## Some assumptions
- Reasonable assumptions are made about the maximum size of various parameters like:
    - Number of commands
    - Number of arguments per command
    - Length of a file path
    - Number of files in a directory
