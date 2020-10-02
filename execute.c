#include "execute.h"
#include "headers.h"
#include "process.h"
#include <signal.h>

char* cmd_name;

extern int shell_terminal;
extern process* PROCESSES;

void sigchld_handler(int signum){
    // handler for SIGCHLD when background child process exits
    
    int prstatus;
    int pid = waitpid(-1, &prstatus, WNOHANG | WUNTRACED);
    
    if(pid > 0){
        
        process* proc = get_process_by_pid(pid, PROCESSES);

        if(WIFSTOPPED(prstatus)){
            fprintf(stderr, "\nSuspended [%d] %s\n", proc->job_num, proc->name);
        }
        
        else if(WIFEXITED(prstatus)){
            fprintf(stderr, "\n%s with pid %d exited ", proc->name, pid);
            if(WEXITSTATUS(prstatus) == 0) fprintf(stderr, "normally\n");
            else fprintf(stderr, "with exit status %d\n", WEXITSTATUS(prstatus));
            PROCESSES = del_process_by_pid(pid, PROCESSES);
        }

        else if(WIFSIGNALED(prstatus)){
            fprintf(stderr, "\n%s with pid %d was terminated by signal %d\n", proc->name, pid, WTERMSIG(prstatus));
        }
    }
    return;
}

int execute(char* argv[], int bg){
    // execute commands with arguments either in foreground or in background

    cmd_name = argv[0];
    
    signal(SIGCHLD, sigchld_handler);
    
    int fork_ret = fork();
    
    if(fork_ret < 0){
        fprintf(stderr, "Error forking\n");
        return 1;
    }
    if(fork_ret == 0){
        // child process

        int pid = getpid();
        setpgid(pid, pid);

        if(bg == 0){
            tcsetpgrp(shell_terminal, pid);
        }

        signal (SIGINT, SIG_DFL);
        signal (SIGQUIT, SIG_DFL);
        signal (SIGTSTP, SIG_DFL);
        signal (SIGTTIN, SIG_DFL);
        signal (SIGTTOU, SIG_DFL);
        signal (SIGCHLD, SIG_DFL);

        int exec_ret = execvp(argv[0], argv);
        if(exec_ret < 0){
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }
        exit(0);
    }
    else{
        // parent process
        // wait for child to terminate only if it is run in the foreground

        setpgid(fork_ret, fork_ret);

        if(bg == 0){
            int status;
            waitpid(fork_ret, &status, WUNTRACED);

            if(WIFSTOPPED(status)){
                PROCESSES = add_process(fork_ret, argv[0], PROCESSES);
                process* added_process = get_process_by_pid(fork_ret, PROCESSES);
                fprintf(stderr, "Suspended [%d] %s\n", added_process->job_num, added_process->name);
                tcsetpgrp(shell_terminal, getpid());
                return 1;
            }
            tcsetpgrp(shell_terminal, getpid());
            return WEXITSTATUS(status);
        }
        else{
            PROCESSES = add_process(fork_ret, argv[0], PROCESSES);
        }
        return 0;
    }
}
