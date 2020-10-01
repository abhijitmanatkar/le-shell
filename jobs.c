#include "jobs.h"
#include "headers.h"
#include "process.h"

extern process* PROCESSES;

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
        fprintf(stderr, "kjob: job not found\n");
        return 1;
    }
    kill(-1 * (p->pid), signal_num);
    return 0;
}