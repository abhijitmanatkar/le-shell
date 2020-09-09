#include "cd.h"
#include "headers.h"
#include "formatpath.h"
#include <errno.h>

extern int errno;

extern char HOMEDIR[DIRNAME_SZ];

int cd(int argc, char* argv[]){
    // changes current working directory to specified argument

    if(argc > 2){
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    if(argc == 1){
        // change to home directory if no arguments are given
        if(chdir(HOMEDIR) < 0){
            perror("cd");
            return 1;
        }
        return 0;
    }
    char* path_abs = to_abs(argv[1]);
    if(chdir(path_abs) < 0){
        perror("cd");
        return 1;
    }
    return 0;
}