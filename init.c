#include "init.h"
#include "headers.h"
#include "process.h"
#include "history.h"
#include "jobs.h"

extern char HOMEDIR[DIRNAME_SZ];
extern char LASTDIR[DIRNAME_SZ];

extern int shell_terminal;
extern process* PROCESSES;

extern size_t INPBUF_SZ;
extern char* inpbuf;

extern int EXIT_STATUS;

void init(){
    // define the controlling terminal
    shell_terminal = STDIN_FILENO;
    
    // reset signals to prevent exit on interrupts
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);

    // create a new process group for the shell and set it as the 
    // foreground process group of the terminal
    int shell_pid = getpid();
    setpgid(shell_pid, shell_pid);
    tcsetpgrp(shell_terminal, shell_pid);

    getcwd(HOMEDIR, DIRNAME_SZ);
    strcpy(LASTDIR, HOMEDIR);
    
    load_history();

    PROCESSES = NULL;

    // define buffer for input
    inpbuf = (char*) malloc(INPBUF_SZ);

    EXIT_STATUS = -1;
}

void quit(){
    overkill(1);
    store_history();
    free(inpbuf);
    exit(0);
}