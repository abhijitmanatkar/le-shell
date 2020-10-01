#include "jobs.h"
#include "headers.h"
#include "process.h"

extern process* PROCESSES;
extern int shell_terminal;

int jobs(int argc, char* argv[]){
    
    if(argc > 1){
        fprintf(stderr, "jobs: too many arguments\n");
        return 1;
    }

    print_processes(PROCESSES);
    return 0;
}

int kjob(int argc, char* argv[]){
    
    if(argc != 3){
        fprintf(stderr, "kjob: incorrect number of arguments\n");
        return 1;
    }

    int job_num = atoi(argv[1]);
    int signal_num = atoi(argv[2]);

    process* p = get_process_by_job_num(job_num, PROCESSES);
    if(p == NULL){
        fprintf(stderr, "kjob: job %d not found\n", job_num); 
        return 1;
    }
    if(kill(-1 * (p->pid), signal_num) < 0){
        perror("kjob");
        return 1;
    }
    return 0;
}

int bg(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "bg: incorrect number of arguments\n");
        return 1;
    }

    int job_num = atoi(argv[1]);
    
    process* p = get_process_by_job_num(job_num, PROCESSES);
    if(p == NULL){
        fprintf(stderr, "bg: job %d not found\n", job_num);
        return 1;
    }
    if(kill(-1 * (p->pid), SIGCONT) < 0){
        perror("bg");
        return 1;
    }
    return 0;
}

int fg(int argc, char* argv[]){

    if(argc != 2){
        fprintf(stderr, "fg: incorrect number of arguments\n");
        return 1;
    }

    int job_num = atoi(argv[1]);

    process* p = get_process_by_job_num(job_num, PROCESSES);
    if(p == NULL){
        fprintf(stderr, "fg: job %d not found\n", job_num);
        return 1;
    }

    int pgid = getpgid(p->pid);
    tcsetpgrp(shell_terminal, pgid);

    kill(-1 * pgid, SIGCONT);
    
    int status;
    waitpid(p->pid, &status, WUNTRACED);

    if(WIFSTOPPED(status)){
        fprintf(stderr, "Suspended [%d] %s\n", p->job_num, p->name);
    }
    else{
        PROCESSES = del_process_by_pid(p->pid, PROCESSES);    
    }
    tcsetpgrp(shell_terminal, getpid());
    
    return 0;
}