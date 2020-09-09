#include "execute.h"
#include "headers.h"
#include <signal.h>

char* cmd_name;

void handler(int signum){
    // handler for SIGCHLD when background child process exits
    int prstatus;
    int pid = waitpid(-1, &prstatus, WNOHANG);
    if(pid > 0){
        fprintf(stderr, "\n%s with pid %d exited ", cmd_name, pid);
        if(WEXITSTATUS(prstatus) == 0) fprintf(stderr, "normally\n");
        else fprintf(stderr, "with exit status %d\n", WEXITSTATUS(prstatus));
    }
    return;
}

int execute(char* argv[], int bg){
    // execute commands with arguments either in foreground or in background

    cmd_name = argv[0];
    
    signal(SIGCHLD, handler);
    
    int fork_ret = fork();
    
    if(fork_ret < 0){
        fprintf(stderr, "Error forking\n");
        return 1;
    }
    if(fork_ret == 0){
        // child process
        int exec_ret = execvp(argv[0], argv);
        if(exec_ret < 0){
            perror(argv[0]);
        }
    }
    else{
        // parent process
        // wait for child to terminate only if it is run in the foreground
        if(bg == 0) wait(NULL);
        return 0;
    }
}
