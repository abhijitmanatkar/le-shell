#include "pinfo.h"
#include "headers.h"
#include "formatpath.h"
#include <fcntl.h>

extern int errno;

char pbuff[512];                // for reading in contents of /proc/<PID>/stat
char exec_path[DIRNAME_SZ];     // for storing the path to the executable
char* terms[64];                // for storing the seperated contents of /proc/<PID>/stat

void pinfo(int argc, char* argv[]){
    // displays info about a process with the given PID

    pid_t pid;
    if(argc == 1) pid = getpid();
    else pid = atoi(argv[1]);

    char filepath[32];
    char linkpath[32];
    sprintf(filepath, "/proc/%d/stat", pid);
    sprintf(linkpath, "/proc/%d/exe", pid);

    // open /proc/<PID>/stat and read contents into pbuf
    int fd = open(filepath, O_RDONLY);
    if(fd < 0){
        perror("pinfo");
        return;    
    }    
    int sz = read(fd, pbuff, 512);
    close(fd);

    // get path to the executable for the process
    int esz = readlink(linkpath, exec_path, 128);    
    if(esz < 0){
        perror("pinfo");
        return;
    }
    exec_path[esz] = '\0';
    char* exec_path_rel = to_rel(exec_path);

    // breakdown the read contents of pbuf into individual components
    char* delim = " ";
    int idx = 0;
    char* temp;
    temp = strtok(pbuff, delim);
    while(temp != NULL){
        terms[idx++] = temp;
        temp = strtok(NULL, delim);
    }
    printf("pid -- %d\nProcess Status -- %s\nmemory -- %s\nExecutable Path -- %s\n", pid, terms[2], terms[22], exec_path_rel);
}