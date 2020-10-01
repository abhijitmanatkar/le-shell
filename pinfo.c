#include "pinfo.h"
#include "headers.h"
#include "formatpath.h"
#include <fcntl.h>

extern int errno;

char pbuff[512];                // for reading in contents of /proc/<PID>/stat
char exec_path[DIRNAME_SZ];     // for storing the path to the executable
char* proc_terms[64];                // for storing the seperated contents of /proc/<PID>/stat

void print_pinfo(int pid){
    char* exec_path_rel = to_rel(exec_path);
    printf("pid -- %d\nProcess Status -- %s\nmemory -- %s\nExecutable Path -- %s\n", pid, proc_terms[2], proc_terms[22], exec_path_rel);
}

int get_pinfo(int pid, int print_errs){
    // gets relevant info about the process from the appropriate /proc/<PID> files

    char filepath[32];
    char linkpath[32];
    sprintf(filepath, "/proc/%d/stat", pid);
    sprintf(linkpath, "/proc/%d/exe", pid);

    // open /proc/<PID>/stat and read contents into pbuf
    int fd = open(filepath, O_RDONLY);
    if(fd < 0){
        if(print_errs)
            perror("pinfo");
        return 1;    
    }    
    int sz = read(fd, pbuff, 512);
    close(fd);

    // get path to the executable for the process
    int esz = readlink(linkpath, exec_path, 128);    
    if(esz < 0){
        if(print_errs)
            perror("pinfo");
        return 1;
    }
    exec_path[esz] = '\0';

    // breakdown the read contents of pbuf into individual components
    char* delim = " ";
    int idx = 0;
    char* temp;
    temp = strtok(pbuff, delim);
    while(temp != NULL){
        proc_terms[idx++] = temp;
        temp = strtok(NULL, delim);
    }

    return 0;
}

void pinfo(int argc, char* argv[]){
    // displays info about a process with the given PID

    pid_t pid;
    if(argc == 1) pid = getpid();
    else pid = atoi(argv[1]);

    if(!get_pinfo(pid, 1))
        print_pinfo(pid);
}

