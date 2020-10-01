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