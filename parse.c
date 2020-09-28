#include "parse.h"
#include "headers.h"
#include "builtins.h"
#include "execute.h"
#include "formatpath.h"
#include <fcntl.h>

void exit_fn(){
    // store history and exit
    store_history();
    exit(0);
}

char* pipe_seperated_commands[MAX_COMMANDS];
char* command_args[MAX_ARGS];
int pipe_fd[2];

int parse_pipe(char* line){

    char* pipe_delim = "|";
    char* command_str = strtok(line, pipe_delim);
    int command_no = 0;
    while(command_str != NULL && command_no < MAX_COMMANDS){
        pipe_seperated_commands[command_no] = command_str;
        command_str = strtok(NULL, pipe_delim);
        command_no++;
    }

    int in_fd = STDIN_FILENO;
    for(int i = 0; i < command_no - 1; i++){
        pipe(pipe_fd);
        parse_redir(pipe_seperated_commands[i], in_fd, pipe_fd[1]);
        close(pipe_fd[1]);
        in_fd = pipe_fd[0];
    }
    return parse_redir(pipe_seperated_commands[command_no - 1], in_fd, STDOUT_FILENO);
}

int parse_redir(char* command, int in_fd, int out_fd){
    
    int input_redirected = 0;
    int output_redirected = 0;
    int old_in_fd, old_out_fd;

    old_in_fd = dup(STDIN_FILENO);
    old_out_fd = dup(STDOUT_FILENO);

    if(in_fd != STDIN_FILENO){
        dup2(in_fd, STDIN_FILENO);
    }
    
    if(out_fd != STDOUT_FILENO){
        dup2(out_fd, STDOUT_FILENO);
    }

    // space seperate arguments
    char* space_delim = " ";
    char* arg_str = strtok(command, space_delim);
    int arg_no = 0;
    while(arg_str != NULL && arg_no < MAX_ARGS){
        command_args[arg_no] = arg_str;
        arg_str = strtok(NULL, space_delim);
        arg_no++;
    }

    char* new_in_path;
    char* new_out_path;
    int output_redir_idx;
    int input_redir_idx;
    for(int i = arg_no - 1; i >= 0; i--){
        if(strcmp(command_args[i], ">") == 0){
            output_redirected = 1;
            arg_no = i;
            new_out_path = command_args[i + 1];
        }
        else if(strcmp(command_args[i], ">>") == 0){
            output_redirected = 2;
            arg_no = i;
            new_out_path = command_args[i + 1];
        }
        else if(strcmp(command_args[i], "<") == 0){
            input_redirected = 1;
            arg_no = i;
            new_in_path = command_args[i + 1];
        }
    }
    command_args[arg_no] = NULL;

    if(input_redirected){
        int new_in_fd = open(to_abs(new_in_path), O_RDONLY);
        if(new_in_fd < 0){
            perror(new_in_path);
            return 1;
        }
        dup2(new_in_fd, STDIN_FILENO);
    }

    if(output_redirected == 1){
        int new_out_fd = open(to_abs(new_out_path), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(new_out_fd < 0){
            perror(new_out_path);
            return 1;
        }
        dup2(new_out_fd, STDOUT_FILENO);
    }

    else if(output_redirected == 2){
        int new_out_fd = open(to_abs(new_out_path), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(new_out_fd < 0){
            perror(new_out_path);
            return 1;
        }
        dup2(new_out_fd, STDOUT_FILENO);
    }


    // built ins and user defined commands
    if(strcmp(command_args[0], "echo") == 0){
        echo(arg_no, command_args);
    }
    else if(strcmp(command_args[0], "pwd") == 0){
        pwd(arg_no);
    }
    else if(strcmp(command_args[0], "cd") == 0){
        cd(arg_no, command_args);
    }
    else if(strcmp(command_args[0], "pinfo") == 0){
        pinfo(arg_no, command_args);
    }
    else if(strcmp(command_args[0], "ls") == 0){
        ls(arg_no, command_args);
    }
    else if(strcmp(command_args[0], "history") == 0){
        show_history(arg_no, command_args);
    }
    else if(strcmp(command_args[0], "exit") == 0){
        exit_fn();
    }
    else if(strcmp(command_args[0], "setenv") == 0){
        set_env(arg_no, command_args);
    }
    else if(strcmp(command_args[0], "unsetenv") == 0){
        unset_env(arg_no, command_args);
    }

    // system commands
    else{
        int bg = 0;
        if(strcmp(command_args[arg_no - 1], "&") == 0){
            arg_no--;
            bg = 1;
        }
        command_args[arg_no] = NULL;
        execute(command_args, bg);
    }

    dup2(old_in_fd, STDIN_FILENO);
    dup2(old_out_fd, STDOUT_FILENO);

    return 0;
}