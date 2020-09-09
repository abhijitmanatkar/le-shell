#include "pinfo.h"
#include "headers.h"
#include "formatpath.h"
#include <fcntl.h>

extern int errno;

char pbuff[512];
char exec_path[128];
char* terms[64];

void pinfo(int argc, char* argv[]){
    pid_t pid;
    if(argc == 1) pid = getpid();
    else pid = atoi(argv[1]);

    char filepath[32];
    char linkpath[32];
    sprintf(filepath, "/proc/%d/stat", pid);
    sprintf(linkpath, "/proc/%d/exe", pid);

    int fd = open(filepath, O_RDONLY);
    if(fd < 0){
        perror("pinfo");
        return;    
    }    
    int sz = read(fd, pbuff, 512);
    close(fd);

    int esz = readlink(linkpath, exec_path, 128);    
    if(esz < 0){
        perror("pinfo");
        return;
    }
    exec_path[esz] = '\0';
    char* exec_path_rel = to_rel(exec_path);

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