# Le-shell

A basic shell written in C using UNIX system  calls.

## Execution instructions
In the source directory, execute:
```shell
$ make
$ ./leshell
```
Press <kbd>Ctrl + D</kbd> to exit or execute: 
```shell
$ quit
```

## Features
- A prompt showing username, hostname, current directory and exit status of previous command.
- Built in commands: `pwd`, `cd`, `ls`, `echo`, `setenv`, `unsetenv`, `jobs`, `kjob`, `fg`, `bg`, `overkill`.
- Support for other system commands.
- Support for redirection of input/output and chaining commands together with pipes.
- Support for commands to be run in the foreground or background.
- Support displaying background jobs and for continuing suspended jobs in foreround or background.
- Support for sending signals to background jobs.
- Display termination information of a command executed in the background.
- User-defined command `pinfo <PID>` that displays process info on the screen.
- Commands to set and unset environment variables.
- Support for absolute and relative paths. Also supports `~` which points to the directory of execution.
- A persistent history of upto 20 commands.

## Implementation Details

### Initialization
- At initialization, the shell process puts itself in a different process group.
- `SIGINT`, `SIGTSTP`, `SIGTTOU`, `SIGTTIN` and other signals are ignored to prevent the shell from terminating on receiving them.
- The shell process group takes control of the terminal.
- Stored history is loaded and memory is allocated for the input buffer.

### Main loop
- The main loop displays a prompt waiting for commands to be entered.
- The entered commands are parsed and correctly executed.

### Prompt
- Depending on the exit status of the last executed command, `:')` or `:'(` is printed at the start of the prompt.
- The username of the current user is obtained by reading environment variable `$USER`. The current directory and  and hostname are obtained via system calls.
- The current directory is displayed as relative to the directory of execution.

### Parsing
- Entered commands are split by semicolons using `strtok`.
- Each command is checked for piping and is further seperated into commands seperated by pipes.
- Each of those commands is further split into arguments seperated by whitespace and checks are made for input/output redirection.
- If input/output redirection is needed, the necessary file descriptors are set.
- If the command is part of the above mentioned built-in or user defined commands, a call is made to the function of that command.
- Otherwise, a call is made to the `execute` function to execute the entered command as a system command.

### Built-in and user defined commands
- Each built-in and user defined command is implemented as a function as defined in its own seperate file.
- An interface to the function is provided by means of a header file.
- Each command returns an exit code for succesful/erroneous execution.

### System commands
- System commands are executed using the `execute` command defined in `execute.c`.
- System commands can be executed in the foreground or in the background.
- This is specified by entering a '&' at the end of a command.
- Before execution, signal handlers for `SIGINT`, `SIGTSTP`, `SIGTTIN` and `SIGTTOU` are set to their default actions so that <kbd>Ctrl + Z</kbd> and <kbd>Ctrl + C</kbd> signals can be received by foreground processes and if a background process tries to access the terminal's stdin or stdout, it can be suspended.
- Execution is carried out by forking the main process to create a child process. The child process then runs the specified command using the  `execvp` system call.
- If the command is executed in the foreground, the shell waits for the the child process to terminate or be stopped. If the process stops, it is added to the `PROCESSES` linked list which stores background jobs. An exit status is returned by a stopping/terminating background process.
- If it is executed in the background, the shell does not wait for the termination of the child process and continues with the main loop. The process is added to the `PROCESSES` linked list.
- A signal handler is set up to handle the `SIGCHLD` signal emitted by a child process running in the background. When this signal is received, the parent process prints termination details of the process.

### Setenv and Unsetenv
- `setenv` and `unsetenv` both make use of the built in system calls to change the environment variables.

### Job related commands
- Background prcesses are stored in the `PROCESSES` linked list. A job number is associated with every background process.
- The `jobs` command goes over this list and prints each of them.
- The `kjob` command sends a signal to a process group corresponding to the given job number using the `kill` system call.
- When `bg` is called, the job if found, is sent the `SIGCONT` signal.
- When `fg` is called, the job is given control of the terminal and then sent the `SIGCONT` signal while the shell process waits for it to either terminate or stop. If it terminates, it is removed from the `PROCESSES` linked list. 

### Pinfo
- The PID is obtained by using the `getpid()` system call.
- The path to the executable is obtained by reading the symbolic link `/proc/<PID/exe>`.
- Other information about the process is found in the file `/proc/<PID>/stat`.

### History
- History is stored in program memory as an array of fixed size and in the file  `history.txt` in the directory of execution.
- History is loaded from the file at the start of the shell. 
- The file is not updated after every command. Instead, the array in program memory is updated with every command. Changes are written to the file only when the shell exits. 
- Two pointers are used to keep track of the start and end positions in the array. 
- Old commands are overwritten when the maximum limit of 20 is reached.

## File wise breakdown
 - `init.c` contains the definitions of the `init` function run at initialization and the `quit` function which is used to terminate the shell.
 - `main.c` contains the main shell loop for reading, parsing and executiong commands.
 - `process.c` contains the definition of the `process` struct and functions for modifying and printing the processes linked list.
 - `jobs.c` contains definitions of the `jobs`, `kjob`, `fg`, `bg` and `overkill` commands.
 - `headers.h` contains common header files and definitions of some important constants.
 - `prompt.c` contains the function for printing the shell prompt.
 - `formatpath.c` contains functions to convert paths from relative to absolute and from absolute to relative.
 - `parse.c` has functions for parsing a command string(taking into account piping, I/O redirection and foreground/background execution) and calling its execution.
 - `exexute.c` has funtions for executing system commands in either foreground or background.
 - `history.c` contains functions for displaying the history, adding a command to history and for loading and storing history in a local file.
 - `cd.c` contains the definition of the `cd` command.
 - `ls.c` contains the definition of the `ls` command.
 - `pwd.c` contains the definition of the `pwd` command.
 - `echo.c` contains the definition of the `echo` command.
 - `pinfo.c` contains the definition of the `pinfo` command.
 - `makefile` contains compilation instructions.
 - `history.txt` stores the history of commands entered.

## Some assumptions
Reasonable assumptions are made about the maximum size of various parameters like:
- Number of commands
- Number of arguments per command
- Length of a file path
- Number of files in a directory
